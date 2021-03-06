<?php
require_once 'Slim/Slim/Slim.php';
require_once 'config.php';

// Application specific exceptions
// Will return 404 Not Found response code
class NotFoundException extends Exception {}
// Will return 403 Forbidden response code
class ForbiddenException extends Exception {}
// Will return 400 Bad Request response code
class ValidationException extends Exception {}

// Abstract class providing singleton behaviour
// will be used by handlers and database classes
abstract class Singleton {
  protected static $instances = array();
  protected function __construct(){}
  private function __clone(){}
  private function __wakeup(){}

  public static function getInstance() {
    $class = get_called_class();
    if(!isset(self::$instances[$class])) {
      self::$instances[$class] = new $class();
    }
    return self::$instances[$class];
  }
}

// Simple PDO database wrapper
// it is not needed by this example and only
// used for prettier request handlers code examples
class Database extends Singleton {
  protected static $dbh;
  protected function __construct() {
    self::$dbh = new PDO('mysql:host=' . DBHOST . ';dbname=' . DBNAME, DBUSER, DBPASS);
    self::$dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
  }
  public function all($sql, $params = array()) {
    $stmt = self::$dbh->prepare($sql);
    $stmt->execute($params);
    return $stmt->fetchAll(PDO::FETCH_OBJ);
  }
  public function one($sql, $params = array()) {
    $stmt = self::$dbh->prepare($sql);
    $stmt->execute($params);
    return $stmt->fetch(PDO::FETCH_OBJ);
  }
  public function execute($sql, $params = array()) {
    $stmt = self::$dbh->prepare($sql);
    $stmt->execute($params);
    if(preg_match('/insert/i', $sql)) return self::$dbh->lastInsertId();
    else return $stmt->rowCount();
  }
}

// Main class defining interface for request handlers
// each class must provide basic CRUD method
abstract class RestApiInterface extends Singleton {
  protected $db;
  protected $app;
  protected function __construct() {
    $this->db = Database::getInstance();
    $this->app = Slim::getInstance();
  }
  abstract public function all($user, $uid, $opts, $auth);
  abstract public function one($user, $uid, $opts, $auth);
  abstract public function add($user, $uid, $opts, $auth, $data);
  abstract public function put($user, $uid, $opts, $auth, $data);
  abstract public function del($user, $uid, $opts, $auth, $data);
  abstract public function validateData($data);
  abstract public function validateUser($user);

  public function prepareExecute($data) {
    $ret = array('cols' => '', 'vals' => '', 'pairs' => '');

    foreach(array_keys((array)$data) as $key) {
      $ret['cols']  .= $key.',';
      $ret['vals']  .= ':'.$key.',';
      $ret['pairs'] .= $key.'=:'.$key.',';
    }
    $ret['cols'] = substr_replace($ret['cols'], '', -1);
    $ret['vals'] = substr_replace($ret['vals'], '', -1);
    $ret['pairs'] = substr_replace($ret['pairs'], '', -1);

    return($ret);
  }

  public function generateUID($tbl, $col, $attempts) {
    $rand = 0;
    $ret = 1;
    for($i = 0; ($i < $attempts) && (!empty($ret)); $i++) {
      $rand = strtoupper(dechex(rand(0,getrandmax())));
      $ret = $this->db->all('SELECT '.$col.' FROM '.$tbl.' WHERE '.$col.'=:sSystemId', array('sSystemId' => $rand));
    }
    if(!empty($ret)) $app->halt(501);

    return($rand);
  }
}


// And here is slim app
// we are going to catch all requests that starts with api
// and give them to appropriate classes method declared above
$app = new Slim(array(
  'cookies.secret_key' => SECRET
));


// Include all of the URL classes
// NOTE: $app needs to have already been instantiated.
require_once 'class/OAUserInfo.php';
require_once 'class/OASystems.php';
require_once 'class/OANodes.php';
require_once 'class/OAAccounting.php';


// Auth samples
$app->get('/api/v1/logout', function(){
  Slim::getInstance()->deleteCookie('auth');
});


$app->post('/api/v1/login', function(){
  $app = Slim::getInstance();
  try {
    $data = json_decode($app->request()->getBody());

    if($data->login == DBUSER && $data->password == DBPASS) {
      $app->setEncryptedCookie('auth', $data->login, '7 days');
    }
  }
  catch(Exception $e) {
    $app->halt(400, $e->getTrace());
  }
});

$app->get('/hello/:name', function ($name) {
  echo "Hello, $name";
});

$app->run();
