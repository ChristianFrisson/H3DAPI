<?php
require('DB/mysqli.php');

$db = mysqli_connect("localhost", "ResultReader", "results", "testserver");
if(mysqli_connect_errno($db)) {
  echo "Failed to connect to MySQL: " . mysqli_connect_error();
}

$query = sprintf("SELECT * FROM servers");

$data = generate_results($db, $query);

// After this is done we've successfully built our object and just need to convert it to json.
echo json_encode($data, JSON_PRETTY_PRINT);
	
function generate_results($db, $query){	
  if(!$fetch_result = mysqli_query($db, $query)) {
      die("ERROR: " . mysql_error());
    }	

  $data = array();
  
  while($row = mysqli_fetch_assoc($fetch_result)) {
    $server = array(
      "id" => $row['id'],
      "name" => $row['server_name']
    );
    array_push($data, $server);
  }
  return $data;
}


?>