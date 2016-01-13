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
    $success = false;  
    if($test_run_result = mysqli_query($db, sprintf("
      SELECT (SELECT success FROM rendering_results WHERE rendering_results.test_run_id=%d AND success='N'  LIMIT 1) IS NULL AS rendering_success,
      (SELECT success FROM custom_results WHERE custom_results.test_run_id=%d AND success='N' LIMIT 1) IS NULL AS  custom_success,
      (SELECT success FROM console_results WHERE console_results.test_run_id=%d AND success='N' LIMIT 1) IS NULL AS console_success,
      (SELECT COUNT(error_results.id) FROM error_results WHERE error_results.test_run_id=%d LIMIT 1)=0 AS exec_success"
      , $row['id'], $row['id'], $row['id'], $row['id']))) {
      if($test_run_row = mysqli_fetch_assoc($test_run_result)) {
        $success = $test_run_row['rendering_success'] && $test_run_row['custom_success'] && $test_run_row['console_success'] && $test_run_row['exec_success'];
      }
    }
  
    $server = array(
      "id" => $row['id'],
      "timestamp" => $row['timestamp'],
      "has_results" => $row['result_count'] > 0,
      "success" => $success
    );
    array_push($data, $server);
  }
  return $data;
}


?>