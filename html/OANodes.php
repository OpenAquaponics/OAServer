<?php

/* The class definition for /{UID}/OANodes */
class OANodes extends RestApiInterface {
  protected $table_name = 'OANodeCfg';

  public function all($user, $uid, $opts, $auth) {
    /* PRIVATE - List all username information */
    if($auth) return $this->db->all('SELECT * FROM ' . $this->table_name . ' WHERE sUsername="' . $user . '"');
    /* PUBLIC - List only the username */
    else return $this->db->all('SELECT sUsername FROM ' . $this->table_name . ' WHERE sUsername="' . $user . '"');
  }
  public function one($user, $uid, $opts, $auth) {
    return $this->db->all('SELECT * FROM ' . $this->table_name . ' WHERE sUsername="' . $user . '" AND sNodeId="' . $id . '"');
  }
  public function add($user, $uid, $opts, $auth, $data) {
    $this->validate($data);

    if($this->db->one('SELECT * FROM users WHERE email=:email', array('email' => $data->email))) throw new ValidationException('Duplicate email');

    $data->id = $this->db->execute('INSERT INTO users VALUES(NULL, :name, :email, :age)', (array)$data);
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
  public function validateData($data) {
    if(empty($data->name)) throw new ValidationException('Name is required');
    if(empty($data->email)) throw new ValidationException('Email is required');
    if(empty($data->age)) throw new ValidationException('Age is required');
    if(!filter_var($data->email, FILTER_VALIDATE_EMAIL)) throw new ValidationException('Wrong email');
    if(!filter_var($data->age, FILTER_VALIDATE_INT, array('min_range' => 1, 'max_range' => 999))) throw new ValidationException('Wrong age');
  }
  public function validateUser($user) {
  }
}


/* The SLIM application callback for the /{UID}/OANodes URL */

