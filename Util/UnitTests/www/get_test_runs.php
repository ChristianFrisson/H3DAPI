<?php
require('DB/mysqli.php');

$db = mysqli_connect("localhost", "ResultReader", "results", "testserver");
if(mysqli_connect_errno($db)) {
  echo "Failed to connect to MySQL: " . mysqli_connect_error();
}

$query = sprintf("
SELECT test_runs.*, 
	(SELECT COUNT(id) FROM performance_results WHERE test_run_id=test_runs.id) +
	(SELECT COUNT(id) FROM rendering_results WHERE test_run_id=test_runs.id) +
	(SELECT COUNT(id) FROM console_results WHERE test_run_id=test_runs.id) +
	(SELECT COUNT(id) FROM custom_results WHERE test_run_id=test_runs.id) +
	(SELECT COUNT(id) FROM error_results WHERE test_run_id=test_runs.id)
as result_count
FROM test_runs WHERE server_id=%d ORDER BY id DESC;
", $_GET['server_id']);

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
      "timestamp" => $row['timestamp'],
      "has_results" => $row['result_count'] > 0
    );
    array_push($data, $server);
  }
  return $data;
}


?>