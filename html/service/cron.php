<?php

//curl -i -X POST -H "Content-Type: application/json" -d "{\"sData\":\"32,543,456.4\"}" http://localhost/index.php/v1/nestinator/OANodes/624B0F36/data

defined('CFGPATH')||define('CFGPATH', '/var/www/html/service/');
defined('CFGFILE')||define('CFGFILE', 'config.json');

$cfg = json_decode(file_get_contents(CFGPATH.CFGFILE));

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

  // Take a sample
  $data = array((string)time(), (string)time() + 132, (string)time() + 500);
  $data_string = json_encode(array("sData" => implode($data, ',')));

  // TODO - Do some kind of authentication


  // TODO - Should the config.json file be loaded everytime and update the system configure


  // Build and send the data packet over the network
  $ch = curl_init('http://localphost/index.php/v1/'.$cfg->sUsername.'/OANodes/'.$cfg->sNodeId.'/data');
  curl_setopt($ch, CURLOPT_CUSTOMREQUEST, 'POST');
  curl_setopt($ch, CURLOPT_POSTFIELDS, $data_string);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
  curl_setopt($ch, CURLOPT_HTTPHEADER, array(
    'Content-Type: application/json',
    'Content-Length: ' . strlen($data_string))
  );
  $result = curl_exec($ch);

  // Network failed or unavailable, save to disk for transfer later
  if(empty($result)) {
    file_put_contents('Test.dat', $data_string."\n", FILE_APPEND);
  }

  // Compensate for the execution time when computing the timeout period
  usleep(($cfg->mPollingPeriod - (microtime(true) - $start_time)) * 1000000);
}

