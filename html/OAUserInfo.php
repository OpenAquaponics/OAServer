<?php

/* The class definition for /{UID}/OAUserInfo */
class OAUserInfo extends RestApiInterface {
  protected $tbl = 'OAUserInfo';

  public function all($user, $uid, $opts, $auth) {
  }

  public function one($user, $uid, $opts, $auth) {
    /* PRIVATE - List all username information */
    if($auth) return $this->db->all('SELECT * FROM '.$this->tbl.' WHERE sUsername=:sUsername', array('sUsername' => $user));
    /* PUBLIC - List only the username */
    else return $this->db->all('SELECT sUsername FROM '.$this->tbl.' WHERE sUsername=:sUsername AND bPublic=1', array('sUsername' => $user));
  }

  public function add($user, $uid, $opts, $auth, $data) {
    // Validate the user and incoming data
    if(!$auth) throw new ForbiddenException();
    if(!isset($data)) $data = json_decode('{}');
    $this->validateData($data);

    // Make sure required parameters are present
    if($user > 32) throw new ValidationException('sUsername is too long (max 32).');
    // TODO - Validate the username to be only alphanumeric
    if(!isset($data->sEmail)) throw new ValidationException('sEmail is required.');

    // Make sure these data fields are present
    $data->sUsername = $user;

    // Check for duplicates
    if($this->db->one('SELECT * FROM '.$this->tbl.' WHERE sUsername=:sUsername', array('sUsername' => $data->sUsername))) throw new ValidationException('Duplicate username: '.$data->sUsername);
    if($this->db->one('SELECT * FROM '.$this->tbl.' WHERE sEmail=:sEmail', array('sEmail' => $data->sEmail))) throw new ValidationException('Duplicate email: '.$data->sEmail);

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

    // Make sure these data fields are present
    $data->sUsername = $user;

    $ret = $this->prepareExecute($data);
    $data->id = $this->db->execute('UPDATE '.$this->tbl.' SET '.$ret['pairs'].' WHERE sUsername=:sUsername', (array)$data);
    return $data;
  }

  public function del($user, $uid, $opts, $auth, $data) {
    if(!$auth) throw new ForbiddenException();

    // Make sure $user owns the database row.
    $ret = $this->db->all('SELECT sUsername FROM OAUserInfo WHERE sUsername=:sUsername', array('sUsername' => $user));
    if(empty($ret) || ($ret[0]->sUsername != $user)) throw new NotFoundException();

    // TODO - This should also erase all of the data from the database

    $this->db->execute('DELETE FROM '.$this->tbl.' WHERE sUsername=:sUsername', array('sUsername' => $user));
    return true;
  }

  public function validateData($data) {
    if(isset($data->bPublic)) {
      if(($data->bPublic != 0) && ($data->bPublic != 1)) throw new ValidationException('bPublic can only be 1 or 0.');
    }
    // TODO - Does this validation work?
    if(isset($data->sEmail)) {
      if(!filter_var($data->sEmail, FILTER_VALIDATE_EMAIL)) throw new ValidationException('Wrong email');
    }
  }

  public function validateUser($user) {
    $ret = $this->db->all('SELECT sUsername FROM OAUserInfo WHERE sUsername=:sUsername', array('sUsername' => $user));
    if(empty($ret)) throw new NotFoundException();
  }

}


/* The SLIM application callback for the /{UID}/OAUserInfo URL */
$app->map('/v1/:user/OAUser', function($user, $uid = null){
  $app = Slim::getInstance();
  $class = 'OAUserInfo';
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

    if($method == 'GET') $res = $class->one($user, $uid, $opts, '1');
    else if($method == 'POST' && $uid == null) $res = $class->add($user, $uid, $opts, '1', json_decode($app->request()->getBody()));
    else if($method == 'PUT' && $uid == null) $res = $class->put($user, $uid, $opts, '1', json_decode($app->request()->getBody()));
    else if($method == 'DELETE' && $uid == null) $res = $class->del($user, $uid, $opts, '1', null);
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

