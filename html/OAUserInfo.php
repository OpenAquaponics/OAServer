<?php

/* The class definition for /{UID}/OAUserInfo */
class OAUserInfo extends RestApiInterface {
  protected $table_name = 'OAUserInfo';

  public function all($user, $uid, $opts, $auth) {
    /* PRIVATE - List all username information */
    if($auth) return $this->db->all('SELECT * FROM ' . $this->table_name . ' WHERE sUsername="' . $user . '"');
    /* PUBLIC - List only the username */
    else return $this->db->all('SELECT sUsername FROM ' . $this->table_name . ' WHERE sUsername="' . $user . '"');
  }
  public function one($user, $uid, $opts, $auth) {
    return $this->db->all('SELECT * FROM ' . $this->table_name . ' WHERE sUsername="' . $user . '"');
  }
  public function add($user, $uid, $opts, $auth, $data) {
    $this->validate($data);

    if($this->db->one('SELECT * FROM ' . $this->table_name . ' WHERE sUsername=:sUsername', array('sUsername' => $data->sUsername))) throw new ValidationException('Duplicate username: ' . $data->sUsername);
    if($this->db->one('SELECT * FROM ' . $this->table_name . ' WHERE sEmail=:sEmail', array('sEmail' => $data->sEmail))) throw new ValidationException('Duplicate email: ' . $data->sEmail);

    $data->id = $this->db->execute('INSERT INTO ' . $this->table_name . ' (sUsername, sPassword, sEmail) VALUES (:sUsername, :sPassword, :sEmail)', (array)$data);
    $data->sPassword = '-- HIDDEN --';
    return $data;
  }
  public function put($user, $uid, $opts, $auth, $data) {
    $this->validate($data);

    $this->db->execute('UPDATE users SET name=:name, email=:email, age=:age WHERE id=:id', array_merge((array)$data, array('id' => $id)));
    return $data;
  }
  public function del($user, $uid, $opts, $auth) {
    $this->db->execute('DELETE FROM users WHERE id=:id', array('id' => $id));
    return true;
  }
  public function validate($data) {
    if(empty($data->sUsername)) throw new ValidationException('sUsername is required');
    if(empty($data->sPassword)) throw new ValidationException('sPassword is required');
    if(empty($data->sEmail)) throw new ValidationException('sEmail is required');
    if(!filter_var($data->sEmail, FILTER_VALIDATE_EMAIL)) throw new ValidationException('Wrong email');
//    if(!filter_var($data->age, FILTER_VALIDATE_INT, array('min_range' => 1, 'max_range' => 999))) throw new ValidationException('Wrong age');
  }
}


/* The SLIM application callback for the /{UID}/OAUserInfo URL */
/*
$app->map('/:user(/:class(/:uid))', function($user, $class, $uid = null){
  $app = Slim::getInstance();
  try {
    $class_str = (string)$class;
    // Check that class exists
    if(!class_exists($class)) throw new NotFoundException();
    // Check that class implements RestApiInterface
    if(!is_subclass_of($class, 'RestApiInterface')) throw new NotFoundException();

    $class = $class::getInstance();
    $method = $app->request()->getMethod();

//    $auth = $app->getEncryptedCookie('auth');
//    if(in_array($method, array('POST', 'PUT', 'DELETE')) && !$auth) throw new ForbiddenException();

    if($method == 'GET' && $uid == null) $res = $class->all($user, '1');
    else if($method == 'GET' && $uid != null) $res = $class->one($user, $uid, '1');
    else if($method == 'POST' && $uid == null) $res = $class->add($user, json_decode($app->request()->getBody()));
    else if($method == 'PUT' && $id != null) $res = $class->put($id, json_decode($app->request()->getBody()));
//    else if($method == 'DELETE' && $id != null) $res = $class->del($id);
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

*/

