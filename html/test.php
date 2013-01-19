<?php


class Test {
  protected $json = '{"sUsername":"nestinator","sSystemId":"SD234JUS","mNumNodes":3,"bPublic":0,"sDescription":"Testing the application"}';
  protected $arr  = array(
    'sUsername' => 'nestinator',
    'sSystemId' => 'SD234JUS',
    'mNumNodes' => 3,
    'bPublic' => 0,
    'sDescription' => 'Testing the application'
  );

  public function process() {
    $data = json_decode($this->json);

    $this->validateData($data);

    $ret = $this->buildSqlColsAndVals((array)$data);
    echo 'INSERT INTO OASystemCfg ('.$ret['cols'].') VALUES ('.$ret['vals'].')';
    echo '<br><br>';

    //$ret = $this->prepareExecute(get_object_vars($data));
    $ret = $this->prepareExecute((array)$data);
    echo 'INSERT INTO OASystemCfg ('.$ret['cols'].') VALUES ('.$ret['vals'].')';
    
  }

  public function validateData($data) {
    // Update these to JSON 
    if($data->sUsername) {
      echo 'Got this field<br><br>';
    // => 'nestinator', 
    }
    if($data->hello)  { echo 'Yup <br><br>'; }
    else { echo 'Nope <br><br>'; }

    $data->hello = 23;

    if($data->hello)  { echo 'Yup <br><br>'; }
    else { echo 'Nope <br><br>'; }
//    'sSystemId' => 'SD234WER',
//    'sNodeId' => 'KI123SFR',
//    'mNumChannels' => 4
  }

  public function prepareExecute($data) {
    $ret = array('cols' => '', 'vals' => '');

    foreach(array_keys($data) as $key) {
      $ret['cols'] .= $key.',';
      $ret['vals'] .= ':'.$key.',';
    }
    $ret['cols'] = substr_replace($ret['cols'], '', -1);
    $ret['vals'] = substr_replace($ret['vals'], '', -1);

    echo 'Array keys: '.$ret['cols'].'<br>';
    echo 'Array vals: '.$ret['vals'].'<br><br>';

    return($ret);
  }

  public function buildSqlColsAndVals($data) {
    $ret = array('cols' => '', 'vals' => '');

    foreach(array_keys($data) as $key) {
      $ret['cols'] .= $key.',';
      $ret['vals'] .= '\''.$data[$key].'\',';
    }
    $ret['cols'] = substr_replace($ret['cols'], '', -1);
    $ret['vals'] = substr_replace($ret['vals'], '', -1);

    echo 'Array keys: '.$ret['cols'].'<br>';
    echo 'Array vals: '.$ret['vals'].'<br><br>';

    return($ret);
  }
}

$app = new Test();

$app->process();


