<?php

/* The class definition for /{UID}/OANodes */
class OANodes extends RestApiInterface {
  protected $tbl = 'OANodeCfg';

  public function all($user, $uid, $opts, $auth) {
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
    if(!$auth) throw new ForbiddenException();
    if(!isset($data)) $data = json_decode('{}');
    $this->validateUser($user);
    $this->validateData($data);

    // Make sure these data fields are present
    $data->sUsername = $user;
    $data->sNodeId = $this->generateUID($this->tbl, 'sNodeId', 5);

    // Create the new database entry using the supplied JSON field
    $ret = $this->prepareExecute($data);
    $data->id = $this->db->execute('INSERT INTO '.$this->tbl.' ('.$ret['cols'].') VALUES ('.$ret['vals'].')', (array)$data);

    // Create a new table to hold the sampling data
    // For some reason the database call returns a failure, but it
    //   performs the operation correctly in the database
    //   SQLSTATE[HY000]: General error
    try {
      $str = 'OAData.'.$data->sNodeId;
      $data->tbl = $this->db->all('CREATE TABLE IF NOT EXISTS '.$str.' (mNum INT PRIMARY KEY AUTO_INCREMENT NOT NULL, sData VARCHAR(128))');
    }
    catch(Exception $e) {
    }

    return $data;
  }

  public function put($user, $uid, $opts, $auth, $data) {
    // Validate the user and incoming data
    if(!$auth) throw new ForbiddenException();
    if(!isset($data)) $data = json_decode('{}');
    $this->validateUser($user);
    $this->validateData($data);

    // Sanity check on the incoming request
    $ret = $this->db->all('SELECT sUsername FROM OANodeCfg WHERE sNodeId=:sNodeId', array('sNodeId' => $uid));
    if(empty($ret) || ($ret[0]->sUsername != $user)) throw new ForbiddenException();

    if(isset($data->sNodeId)) {
      if($data->sNodeId != $uid) throw new ValidationException('sNodeId mismatch.');
    }

    // Make sure these data fields are present
    $data->sUsername = $user;
    $data->sNodeId = $uid;

    // Create the new database entry using the supplied JSON field
    $ret = $this->prepareExecute($data);
    $data->id = $this->db->execute('UPDATE '.$this->tbl.' SET '.$ret['pairs'].' WHERE sNodeId=:sNodeId', (array)$data);
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
$app->map('/v1/:user/OANodes(/(:uid))', function($user, $uid = null){
  $app = Slim::getInstance();
  $class = 'OANodes';
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


/*******************************************/

/* The class definition for /{UID}/OANodes/{NID}/data */
class OAData extends RestApiInterface {
  protected $dbname = 'OAData';

  public function all($user, $uid, $opts, $auth) {
    /* TODO - Need to verify the OANode settings before providing data, is it public, who's the owner, etc */

    /* PRIVATE - Show all of available systems */
    if($auth) return $this->db->all('SELECT * FROM '.$this->dbname.'.'.$uid.' ORDER BY mNum DESC LIMIT 32');
    /* PUBLIC - Show only the public systems */
//    else return $this->db->all('SELECT * FROM '.$db.'.'.$uid.' ORDER BY mNum DESC LIMIT 32 WHERE sUsername=:sUsername AND bPublic=1', array('sUsername' => $user));
  }

  public function one($user, $uid, $opts, $auth) {
  }

  public function add($user, $uid, $opts, $auth, $data) {
    // Validate the user and incoming data
    if(!$auth) throw new ForbiddenException();
    if(!isset($data)) throw new NotFoundException();
    $this->validateUser($user);

    // This is a batch transfer of samples
    if(isset($data->batch)) {
      for($i = 0; $i < count($data->batch); $i++) {
        $this->postSample($uid, $data->batch[$i]);
      }
    }
    // This is a single sample
    else {
      $this->postSample($uid, $data);
    }

    return $data;
  }

  public function put($user, $uid, $opts, $auth, $data) {
    // Validate the user and incoming data
    if(!$auth) throw new ForbiddenException();
    if(!isset($data)) $data = json_decode('{}');
    $this->validateUser($user);
    $this->validateData($data);

    // Sanity check on the incoming request
    if(!isset($data->mNum)) throw new NotFoundException();
    $ret = $this->db->all('SELECT mNum FROM '.$this->dbname.'.'.$uid.' WHERE mNum=:mNum', array('mNum' => $data->mNum));
    if(empty($ret)) throw new NotFoundException();

    /* TODO - Need to verify the OANode settings before updating data, is it public, who's the owner, etc */

    // Create the new database entry using the supplied JSON field
    $ret = $this->prepareExecute($data);
    $data->id = $this->db->execute('UPDATE '.$this->dbname.'.'.$uid.' SET '.$ret['pairs'].' WHERE mNum=:mNum', (array)$data);
    return $data;
  }

  public function del($user, $uid, $opts, $auth, $data) {
    if(!$auth) throw new ForbiddenException();
    if(!isset($data)) $data = json_decode('{}');
    $this->validateUser($user);
    $this->validateData($data);

    // Sanity check on the incoming request
    if(!isset($data->mNum)) throw new NotFoundException();
    $ret = $this->db->all('SELECT mNum FROM '.$this->dbname.'.'.$uid.' WHERE mNum=:mNum', array('mNum' => $data->mNum));
    if(empty($ret)) throw new NotFoundException();

    /* TODO - Need to verify the OANode settings before updating data, is it public, who's the owner, etc */

    // Delete the node information from the database and remove the data table
    $this->db->execute('DELETE FROM '.$this->dbname.'.'.$uid.' WHERE mNum=:mNum', array('mNum' => $data->mNum));

    return true;
  }

  public function validateData($data) {
  }

  public function validateUser($user) {
    $ret = $this->db->all('SELECT sUsername FROM OAUserInfo WHERE sUsername=:sUsername', array('sUsername' => $user));
    if(empty($ret)) throw new NotFoundException();
  }

  public function postSample($uid, $data) {
    $this->validateData($data);

    /* TODO - Need to verify the OANode settings before saving data, is it public, who's the owner, etc */

    // Create the new database entry using the supplied JSON field
    $ret = $this->prepareExecute($data);
    return($this->db->execute('INSERT INTO '.$this->dbname.'.'.$uid.' ('.$ret['cols'].') VALUES ('.$ret['vals'].')', (array)$data));
  }
}


/* The SLIM application callback for the /{UID}/OANodes/{NID}/data URL */
$app->map('/v1/:user/OANodes/:uid/data(/)', function($user, $uid = null){
  $app = Slim::getInstance();
  $class = 'OAData';
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

    if($method == 'GET' && $uid != null) $res = $class->all($user, $uid, $opts, '1');
    else if($method == 'POST' && $uid != null) $res = $class->add($user, $uid, $opts, '1', json_decode($app->request()->getBody()));
    else if($method == 'PUT' && $uid != null) $res = $class->put($user, $uid, $opts, '1', json_decode($app->request()->getBody()));
    else if($method == 'DELETE' && $uid != null) $res = $class->del($user, $uid, $opts, '1', json_decode($app->request()->getBody()));
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

