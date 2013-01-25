<?php

//curl -i -X POST -H "Content-Type: application/json" -d "{\"sData\":\"32,543,456.4\"}" http://localhost/index.php/v1/nestinator/OANodes/624B0F36/data

while(1) {
  $data = array("sData" => (string)time());
  $data_string = json_encode($data);

  $ch = curl_init('http://localhost/index.php/v1/nestinator/OANodes/624B0F36/data');
  curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "POST");
  curl_setopt($ch, CURLOPT_POSTFIELDS, $data_string);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
  curl_setopt($ch, CURLOPT_HTTPHEADER, array(
    'Content-Type: application/json',
    'Content-Length: ' . strlen($data_string))
  );

  $result = curl_exec($ch);

  echo 'Sleeping';
  sleep(10);
}

