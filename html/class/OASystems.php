<?php

/* The class definition for /{UID}/OASystems */
class OASystems extends RestApiInterface {
  protected $tbl = 'OASystemCfg';

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
    $ret = $this->db->all('SELECT bPublic FROM '.$this->tbl.' WHERE sUsername=:sUsername AND sSystemId=:sSystemId', array('sUsername' => $user, 'sSystemId' => $uid));
    if($ret[0]->bPublic || $auth)
      return $this->db->all('SELECT * FROM '.$this->tbl.' WHERE sUsername=:sUsername AND sSystemId=:sSystemId', array('sUsername' => $user, 'sSystemId' => $uid));
  }

  public function add($user, $uid, $opts, $auth, $data) {
    // curl -i -X POST -H "Content-Type: application/json" -d "{\"sUsername\": \"nestinator\"}" http://localhost/index.php/nestinator/OASystems
    if(!$auth) throw new NotFoundException();
    if(!isset($data)) $data = json_decode('{}');
    $this->validateUser($user);
    $this->validateData($data);

    // Create a random system ID
    $data->sUsername = $user;
    $data->sSystemId = $this->generateUID($this->tbl, 'sSystemId', 5);

    // Create the new database entry using the supplied JSON field
    $ret = $this->prepareExecute($data);
    $data->id = $this->db->execute('INSERT INTO '.$this->tbl.' ('.$ret['cols'].') VALUES ('.$ret['vals'].')', (array)$data);
    return $data;
  }

  public function put($user, $uid, $opts, $auth, $data) {
    if(!$auth) throw new NotFoundException();
    if(!isset($data)) $data = json_decode('{}');
    $this->validateUser($user);
    $this->validateData($data);

    // Sanity check on the incoming request
    $ret = $this->db->all('SELECT sUsername FROM OASystemCfg WHERE sSystemId=:sSystemId', array('sSystemId' => $uid));
    if(empty($ret) || ($ret[0]->sUsername != $user)) throw new ForbiddenException();

    if(isset($data->sSystemId)) {
      if($data->sSystemId != $uid) throw new ValidationException('sSystemId mismatch.');
    }

    // Make sure these data fields are present
    $data->sUsername = $user;
    $data->sSystemId = $uid;

    // Create the new database entry using the supplied JSON field
    $ret = $this->prepareExecute($data);
    $data->id = $this->db->execute('UPDATE '.$this->tbl.' SET '.$ret['pairs'].' WHERE sSystemId=:sSystemId', (array)$data);
    return $data;
  }

  public function del($user, $uid, $opts, $auth, $del) {
    if(!$auth) throw new NotFoundException();

    $ret = $this->db->all('SELECT sUsername FROM OASystemCfg WHERE sSystemId=:sSystemId', array('sSystemId' => $uid));
    if(empty($ret) || ($ret[0]->sUsername != $user)) throw new ForbiddenException();

    // TODO - Should this just toggle an enable/disable disable field in the database?
    $this->db->execute('DELETE FROM '.$this->tbl.' WHERE sSystemId=:sSystemId', array('sSystemId' => $uid));

    // TODO - Should also delete the sSystemId's for all of the matching OANodes

    return true;
  }

  public function validateData($data) {
    if(isset($data->bPublic)) {
      if(($data->bPublic != 0) && ($data->bPublic != 1)) throw new ValidationException('bPublic can only be 1 or 0.');
    }
  }

  public function validateUser($user) {
    $ret = $this->db->all('SELECT sUsername FROM OAUserInfo WHERE sUsername=:sUsername', array('sUsername' => $user));
    if(empty($ret)) throw new NotFoundException();
  }
}

/* The SLIM application callback for the /{UID}/OASystems URL */
$app->map('/v1/:user/OASystems(/(:uid(/)))', function($user, $uid = null){
  $app = Slim::getInstance();
  $class = 'OASystems';
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
    else if($method == 'DELETE' && $uid != null) $res = $class->del($user, $uid, $opts, '1', null);
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

