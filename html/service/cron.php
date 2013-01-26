<?php

//curl -i -X POST -H "Content-Type: application/json" -d "{\"sData\":\"32,543,456.4\"}" http://localhost/index.php/v1/nestinator/OANodes/624B0F36/data

// Setup the path to all of the directories
defined('SERVICEPATH')||define('SERVICEPATH', '/var/www/html/service/');
defined('CFGPATH')||define('CFGPATH', SERVICEPATH);
defined('CFGFILE')||define('CFGFILE', 'config.json');


// The OANode.service class (handles sampling, logging, and transmitting data
class OANodeService {
  protected $file_cnt = 0;

  public function sendJSON($path, $data_string) {
    $ch = curl_init($path);
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, 'POST');
    curl_setopt($ch, CURLOPT_POSTFIELDS, $data_string);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array(
      'Content-Type: application/json',
      'Content-Length: ' . strlen($data_string))
    );
    return(curl_exec($ch));
  }

  public function run() {
    $cfg  = json_decode(file_get_contents(CFGPATH.CFGFILE));
    $outfile_name = '';
 
    // Make sure the required fields are included
    if(!isset($cfg->sUsername)) {
      print 'sUsername required'; exit(1);
    }
    if(!isset($cfg->sNodeId)) {
      print 'sNodeId required'; exit(1);
    }
    if(!isset($cfg->mPollingPeriod)) {
      print 'mPollingPeriod required'; exit(1);
    }
 
    // Loop the service indefinately
    while(1) {
      $start_time = microtime(true);

      // TODO - Do some kind of authentication
      // TODO - Should the config.json file be loaded everytime and update the system configure
 
      // Take a sample
      $data = array((string)time(), (string)time() + 132, (string)time() + 500);
      $data_string = json_encode(array("sData" => implode($data, ',')));
 
      // Build and send the data packet over the network
      $ret = $this->sendJSON('http://localhost/index.php/v1/'.$cfg->sUsername.'/OANodes/'.$cfg->sNodeId.'/data', $data_string);
 
      // Network failed or unavailable, save to disk for transfer later
      if(empty($ret)) {
        $this->outfile_name = SERVICEPATH.'data/LOG_'.str_pad((int)$this->file_cnt,5,STR_PAD_LEFT).'.dat';
        file_put_contents($this->outfile_name, $data_string."\n", FILE_APPEND);
        if(filesize($this->outfile_name) > 50000) {
          $this->file_cnt++;
        }
      }
      else {
        // Check to see if there is data needed to be written to the server now that there is a connection
        // NOTE: I really hope this spawns a true new 'process' so there will be no conflicts with data members in the class
        $this->sendPendingTransfers();
      }
 
      // Compensate for the execution time when computing the timeout period
      usleep(($cfg->mPollingPeriod - (microtime(true) - $start_time)) * 1000000);
    }
  }

  public function sendPendingTransfers() {
    $files = scandir(SERVICEPATH.'data', 1);
    // This is a race condition waiting to happen.  Think it through
    if(count($files) > 2) {
      // Skip files: '.' and '..'
    }
    else {
      $this->file_cnt = 0;
    }
  }

  public function uploadData() {
    //////////////////////
    // TODO - Make the server support multiple rows in a single JSON object
    //////////////////////


  }

}

$app = new OANodeService();
$app->run();


