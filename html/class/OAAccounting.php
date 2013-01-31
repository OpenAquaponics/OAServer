<?php

/* The class definition for /{UID}/OANodes */
class OAAccounting extends RestApiInterface {
  protected $tbl = 'OAAccounting';

  public function all($user, $uid, $opts, $auth) {
    /* PRIVATE - Show all of available systems */
    if($auth) return $this->db->all('SELECT * FROM '.$this->tbl.' WHERE sUsername=:sUsername', array('sUsername' => $user));
  }

  public function one($user, $uid, $opts, $auth) {
    // curl -i -X GET http://localhost/index.php/seperated1/OASystems/44F181DF
    if($auth) return $this->db->all('SELECT * FROM '.$this->tbl.' WHERE sUsername=:sUsername AND mCnt=:mCnt', array('sUsername' => $user, 'mCnt' => $uid));
  }

  public function add($user, $uid, $opts, $auth, $data) {
    // Validate the user and incoming data
    if(!$auth) throw new ForbiddenException();
    if(!isset($data)) $data = json_decode('{}');
    $this->validateUser($user);
    $this->validateData($data);

    // Make sure these data fields are present
    $data->sUsername = $user;
    $ret = $this->db->all('SELECT mCnt FROM '.$this->tbl.' WHERE sUsername=:sUsername ORDER BY mCnt DESC LIMIT 1', array('sUsername' => $user));
    if(empty($ret)) $data->mCnt = 0;
    else $data->mCnt = $ret[0]->mCnt + 1;

    // Create the new database entry using the supplied JSON field
    $ret = $this->prepareExecute($data);
    $data->id = $this->db->execute('INSERT INTO '.$this->tbl.' ('.$ret['cols'].') VALUES ('.$ret['vals'].')', (array)$data);

    return $data;
  }

  public function put($user, $uid, $opts, $auth, $data) {
    // Validate the user and incoming data
    if(!$auth) throw new ForbiddenException();
    if(!isset($data)) $data = json_decode('{}');
    $this->validateUser($user);
    $this->validateData($data);

    // Sanity check on the incoming request
    $ret = $this->db->all('SELECT mCnt FROM '.$this->tbl.' WHERE sUsername=:sUsername AND mCnt=:mCnt', array('sUsername' => $user, 'mCnt' => $uid));
    if(empty($ret)) throw new NotFoundException();

    // Make sure these data fields are present
    $data->sUsername = $user;
    $data->mCnt = $uid;

    // Create the new database entry using the supplied JSON field
    $ret = $this->prepareExecute($data);
    $data->id = $this->db->execute('UPDATE '.$this->tbl.' SET '.$ret['pairs'].' WHERE sUsername=:sUsername AND mCnt=:mCnt', (array)$data);
    return $data;
  }

  public function del($user, $uid, $opts, $auth, $data) {
    if(!$auth) throw new ForbiddenException();

    // Make sure $user owns the database row.
    $ret = $this->db->all('SELECT sUsername FROM OANodeCfg WHERE sNodeId=:sNodeId', array('sNodeId' => $uid));
    if(empty($ret) || ($ret[0]->sUsername != $user)) throw new ForbiddenException();

    // Delete the node information from the database and remove the data table
    $this->db->execute('DELETE FROM '.$this->tbl.' WHERE sNodeId=:sNodeId', array('sNodeId' => $uid));
    $this->db->execute('DROP TABLE IF EXISTS OAData.'.$uid);

    return true;
  }

  public function validateData($data) {
    if(isset($data->mIdx)) {
      throw new ForbiddenException();
    }
    if(isset($data->mCnt)) {
    }
/*
    if(isset($data->sSystemId)) {
      $ret = $this->db->all('SELECT sSystemId FROM OASystemCfg WHERE sSystemId=:sSystemId', array('sSystemId' => $data->sSystemId));
      if(empty($ret)) throw new ValidationException('sSystemId needs to be a valid ID.');
    }
*/
    if(isset($data->sNodeId)) {
      $ret = $this->db->all('SELECT sNodeId FROM OANodeCfg WHERE sNodeId=:sNodeId', array('sNodeId' => $data->sNodeId));
      if(empty($ret)) throw new ValidationException('sNodeId needs to be a valid ID.');
    }
  }

  public function validateUser($user) {
    $ret = $this->db->all('SELECT sUsername FROM OAUserInfo WHERE sUsername=:sUsername', array('sUsername' => $user));
    if(empty($ret)) throw new NotFoundException();
  }

}

/* The SLIM application callback for the /{UID}/OANodes URL */
$app->map('/v1/:user/OAAccounting(/(:uid(/)))', function($user, $uid = null){
  $app = Slim::getInstance();
  $class = 'OAAccounting';
  try {
    // Check that class exists
    if(!class_exists($class)) throw new NotFoundException();
    // Check that class implements RestApiInterface
    if(!is_subclass_of($class, 'RestApiInterface')) throw new NotFoundException();

    $class = $class::getInstance();
    $method = $app->request()->getMethod();
    $opts = $app->request();  // TODO - This should parse out from /URL/?options component

//    Need to get authentication running at some point
//    $auth = $app->getEncryptedCookie('auth');
//    if(in_array($method, array('POST', 'PUT', 'DELETE')) && !$auth) throw new ForbiddenException();

    if($method == 'GET' && $uid == null) $res = $class->all($user, $uid, $opts, '1');
    else if($method == 'GET' && $uid != null) $res = $class->one($user, $uid, $opts, '1');
    else if($method == 'POST' && $uid == null) $res = $class->add($user, $uid, $opts, '1', json_decode($app->request()->getBody()));
    else if($method == 'PUT' && $uid != null) $res = $class->put($user, $uid, $opts, '1', json_decode($app->request()->getBody()));
    else if($method == 'DELETE' && $uid != null) $res = $class->del($user, $uid, $opts, '1', NULL);
    else $app->halt(501); // Not implemented

    if(empty($res)) throw new NotFoundException();

    $json = json_encode($res);
    echo $json;

  }
  catch(ValidationException $e) {
    $app->halt(400, $e->getMessage());
  }
  catch(ForbiddenException $e) {
    $app->halt(403);
  }
  catch(NotFoundException $e) {
    $app->halt(404);
  }
  catch(Exception $e) {
    $app->halt(500, $e->getMessage());
  }
})->via('GET', 'POST', 'PUT', 'DELETE')->conditions(array('id' => '\d+'));

