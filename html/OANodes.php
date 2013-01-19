<?php

/* The class definition for /{UID}/OANodes */
class OANodes extends RestApiInterface {
  protected $tbl = 'OANodeCfg';

  public function all($user, $uid, $opts, $auth) {
    // Example of how to accept incoming optional arguements
    // http://localhost/index.php/nestinator/OASystems/54FG23?pos[]=23.3&pos[]=34.5
    //$opts = $app->request();
    //echo var_dump($opts->get('pos'));

    /* PRIVATE - Show all of available systems */
    if($auth) return $this->db->all('SELECT * FROM '.$this->tbl.' WHERE sUsername=:sUsername', array('sUsername' => $user));
    /* PUBLIC - Show only the public systems */
    else return $this->db->all('SELECT * FROM '.$this->tbl.' WHERE sUsername=:sUsername AND bPublic=1', array('sUsername' => $user));
  }

  public function one($user, $uid, $opts, $auth) {
    // curl -i -X GET http://localhost/index.php/seperated1/OASystems/44F181DF
    $ret = $this->db->all('SELECT bPublic FROM '.$this->tbl.' WHERE sUsername=:sUsername AND sNodeId=:sNodeId', array('sUsername' => $user, 'sNodeId' => $uid));
    if($ret[0]->bPublic || $auth)
      return $this->db->all('SELECT * FROM '.$this->tbl.' WHERE sUsername=:sUsername AND sNodeId=:sNodeId', array('sUsername' => $user, 'sNodeId' => $uid));
  }

  public function add($user, $uid, $opts, $auth, $data) {
    // Validate the user and incoming data
    if(!$auth) throw new NotFoundException();
    $this->validateUser($user);
    $this->validateData($data);

    // Make sure required parameters are present
    if(isset($data->mNumChannels)) throw new ValidationException('mNumChannels is required.');

    // Make sure these data fields are present
    $data->sUsername = $user;
    $data->sNodeId = $this->generateUID($this->tbl, 'sNodeId', 5);

    // Create the new database entry using the supplied JSON field
    $ret = $this->prepareExecute($data);
    $data->id = $this->db->execute('INSERT INTO '.$this->tbl.' ('.$ret['cols'].') VALUES ('.$ret['vals'].')', (array)$data);

    // TODO - If successful, create a new table the 'data' database
//    $data->new_tbl = sNodeId;

    return $data;
  }

  public function put($user, $uid, $opts, $auth, $data) {
    // Validate the user and incoming data
    if(!$auth) throw new NotFoundException();
    $this->validateUser($user);
    $this->validateData($data);

    // Sanity check on the incoming request
    if(isset($data->sNodeId)) {
      if($data->sNodeId != $uid) throw new ValidationException('sNodeId doesn\'t match.');
    }
    $ret = $this->db->all('SELECT sUsername FROM OANodeCfg WHERE sNodeId=:sNodeId', array('sNodeId' => $uid));
    if(empty($ret)) throw new ForbiddenException();

    // Make sure these data fields are present
    $data->sUsername = $user;
    $data->sNodeId = $uid;

    // TODO - If mNumChannels matches, continue
    // TODO - If mNumChannels doesn't match, there needs to be a manner to delete the other table and create a new one.

    $ret = $this->prepareExecute($data);
    $data->id = $this->db->execute('UPDATE '.$this->tbl.' SET '.$ret['pairs'].' WHERE sNodeId=:sNodeId', (array)$data);
    return $data;
  }

  public function del($user, $uid, $opts, $auth) {
    if(!$auth) throw new NotFoundException();

    // TODO - Should this just toggle an enable/disable disable field in the database?
    $this->db->execute('DELETE FROM '.$this->tbl.' WHERE sSystemId=:sSystemId', array('sSystemId' => $uid));
    return true;
  }

  public function validateData($data) {
    if(isset($data->mNumChannels)) {
      if($data->mNumChannels > 16) throw new ValidationException('mNumChannels cannot exceed 16.');
      if($data->mNumChannels < 0) throw new ValidationException('mNumChannels cannot be less than 0.');
    }
    if(isset($data->sSystemId)) {
      $ret = $this->db->all('SELECT sSystemId FROM OASystemCfg WHERE sSystemId=:sSystemId', array('sSystemId' => $data->sSystemId));
      if(empty($ret)) throw new ValidationException('sSystemId needs to be a valid ID.');
    }
    if(isset($data->sNodeId)) {
      $ret = $this->db->all('SELECT sNodeId FROM OANodeCfg WHERE sNodeId=:sNodeId', array('sNodeId' => $data->sNodeId));
      if(empty($ret)) throw new ValidationException('sNodeId needs to be a valid ID.');
    }
    if(isset($data->mPollingPeriod)) {
      if($data->mPollingPeriod < 10) throw new ValidationException('mPollingPeriod time should be greater than 10 seconds.');
    }
    if(isset($data->bPublic)) {
      if(($data->bPublic != 0) && ($data->bPublic != 1)) throw new ValidationException('bPublic can only be 1 or 0.');
    }
  }

  public function validateUser($user) {
    $ret = $this->db->all('SELECT sUsername FROM OAUserInfo WHERE sUsername=:sUsername', array('sUsername' => $user));
    if(empty($ret)) throw new NotFoundException();
  }

}

/* The SLIM application callback for the /{UID}/OANodes URL */
$app->map('/:user/OANodes(/:uid)', function($user, $uid = null){
  $app = Slim::getInstance();
  $class = 'OANodes';
  try {
    // Check that class exists
    if(!class_exists($class)) throw new NotFoundException();
    // Check that class implements RestApiInterface
    if(!is_subclass_of($class, 'RestApiInterface')) throw new NotFoundException();

    $class = $class::getInstance();
    $method = $app->request()->getMethod();
    $opts = $app->request();  //* TODO - This should parse out from /URL/?options component


//    $auth = $app->getEncryptedCookie('auth');
//    if(in_array($method, array('POST', 'PUT', 'DELETE')) && !$auth) throw new ForbiddenException();

    if($method == 'GET' && $uid == null) $res = $class->all($user, $uid, $opts, '1');
    else if($method == 'GET' && $uid != null) $res = $class->one($user, $uid, $opts, '1');
    else if($method == 'POST' && $uid == null) $res = $class->add($user, $uid, $opts, '1', json_decode($app->request()->getBody()));
    else if($method == 'PUT' && $uid != null) $res = $class->put($user, $uid, $opts, '1', json_decode($app->request()->getBody()));
    else if($method == 'DELETE' && $uid != null) $res = $class->del($user, $uid, $opts, '1');
    else $app->halt(501); // Not implemented

    if(empty($res)) throw new NotFoundException();

    $json = json_encode($res);
    $cb = isset($_GET['callback']) ? $_GET['callback'] : false;
    if($cb) $json = "$cb($json)";

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

