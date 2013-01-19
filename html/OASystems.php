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
    if($auth) return $this->db->all('SELECT * FROM '.$this->tbl.' WHERE sUsername="' . $user . '"');
    /* PUBLIC - Show only the public systems */
    else return $this->db->all('SELECT * FROM '.$this->tbl.' WHERE sUsername=:sUsername AND bPublic=1', array('sUsername' => $user));
  }

  public function one($user, $uid, $opts, $auth) {
    // curl -i -X GET http://localhost/index.php/seperated1/OASystems/44F181DF
    $ret = $this->db->all('SELECT bPublic FROM '.$this->tbl.' WHERE sUsername=:sUsername AND sSystemId=:sSystemId', array('sUsername' => $user, 'sSystemId' => $uid));
    if($ret[0]->bPublic || $auth)
      return $this->db->all('SELECT * FROM '.$this->tbl.' WHERE sUsername=:sUsername AND sSystemId=:sSystemId', array('sUsername' => $user, 'sSystemId' => $uid));
  }

  public function add($user, $uid, $opts, $auth, $data) {
    // curl -i -X POST -H "Content-Type: application/json" -d "{\"sUsername\": \"nestinator\"}" http://localhost/index.php/nestinator/OASystems
    if(!$auth) throw new NotFoundException();
    $this->validateUser($user);
    $this->validateData($data);
    if($user!=$data->sUsername) throw new NotFoundException();

    // Create a random system ID
    $rand = 0;
    $ret = 1;
    for($i = 0; ($i < 5) && (!empty($ret)); $i++) {
      $rand = strtoupper(dechex(rand(0,getrandmax())));
      $ret = $this->db->all('SELECT sSystemId FROM '.$this->tbl.' WHERE sSystemId=:sSystemId', array('sSystemId' => $rand));
    }

    // TODO - Need to actually look at the incoming JSON data
    $data->id = $this->db->execute('INSERT INTO '.$this->tbl.' (sUsername, sSystemId) VALUES (:sUsername, :sSystemId)', array('sUsername' => $user, 'sSystemId' => $rand));
    return $data;
  }

  public function put($user, $uid, $opts, $auth, $data) {
    // curl -i -X PUT -H "Content-Type: application/json" -d "{\"sUsername\":\"seperated1\",\"sSystemId\":\"16F47357\",\"mNumNodes\":3,\"bPublic\":0,\"sDescription\":\"Adding to the description\"}" http://localhost/index.php/seperated1/OASystems/16F47357
    if(!$auth) throw new NotFoundException();
    $this->validateUser($user);
    $this->validateData($data);
    if($user!=$data->sUsername) throw new NotFoundException();

    // TODO - There must be a better way to build a SQL query based on an incoming JSON string
    $this->db->execute('UPDATE '.$this->tbl.' SET mNumNodes=:mNumNodes, bPublic=:bPublic, sDescription=:sDescription WHERE sSystemId=:sSystemId', array('mNumNodes' => $data->mNumNodes, 'bPublic' => $data->bPublic, 'sDescription' => $data->sDescription, 'sSystemId' => $data->sSystemId));
    return $data;
  }

  public function del($user, $uid, $opts, $auth) {
    if(!$auth) throw new NotFoundException();

    // TODO - Should this just toggle an enable/disable disable field in the database?
    $this->db->execute('DELETE FROM '.$this->tbl.' WHERE sSystemId=:sSystemId', array('sSystemId' => $uid));
    return true;
  }

  public function validateData($data) {
// TODO - This should validate the data and also build an array that can be used for the SQL query string
// TODO - Does the username need to be validated?  Seems like each message is going to have its own requirements
    if(empty($data->sUsername)) throw new ValidationException('Username is required');
  }

  public function validateUser($user) {
    $ret = $this->db->all('SELECT sUsername FROM OAUserInfo WHERE sUsername=:sUsername', array('sUsername' => $user));
    if(empty($ret)) throw new NotFoundException();
  }
}

/* The SLIM application callback for the /{UID}/OASystems URL */
$app->map('/:user/OASystems(/:uid)', function($user, $uid = null){
  $app = Slim::getInstance();
  $class = 'OASystems';
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

