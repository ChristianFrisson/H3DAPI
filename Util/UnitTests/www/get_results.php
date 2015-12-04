<?php
require('DB/mysqli.php');

$db = mysqli_connect("localhost", "ResultReader", "results", "testserver");
if(mysqli_connect_errno($db)) {
  echo "Failed to connect to MySQL: " . mysqli_connect_error();
}

// Fetch the performance results from the latest test, as well as filename, timestamps, etc.
$fetch_result = mysqli_query($db, "
                SELECT performance_results.id, test_run_id, min_fps, max_fps, avg_fps, timestamp, filename, case_name, case_id, server_id, server_name
									FROM performance_results
                    JOIN
                      (SELECT * FROM test_runs ORDER BY test_runs.id DESC LIMIT 1) as test_runs
                    JOIN
                      test_files
										JOIN
										  test_cases	
										JOIN
										  servers					   
                    ON 
										 	  performance_results.test_run_id=test_runs.id
										and performance_results.file_id=test_files.id
										and performance_results.case_id=test_cases.id
										and test_runs.server_id=servers.id;");
										
$data = array();
while($row = mysqli_fetch_assoc($fetch_result)) {
  $filename = $row['filename'];
  $category_structure = explode('/', $filename);
  // Store a reference to the root of the data tree. We'll move this reference down the tree until we reach the part where the leaf should go,
  // creating any missing nodes on the way there.
  $node = &$data;
  // Go through every folder specified in the casename path and build our array structure.
  while(count($category_structure) > 1) {
    if((count($node) == 0) || (!array_key_exists($category_structure[0], $node['children']))) {
      $new_node = array( 'name' => $category_structure[0], 'children' => array()); // The node didn't exist so we create it before pointing our reference to it
      if($node !== $data) {
        $node['children'][$category_structure[0]] = $new_node;
        $node = &$new_node;
      } else {
        $node = &$node[array_push($node, $new_node)-1];
      } 
    } else {     
      $node = &$node['children'][$category_structure[0]];
      }
    // Remove the first element from $category_structure
    array_shift($category_structure); 
  }
  // For the last node, which is the testfile leaf, we check if it exists and eventually create it down here instead because it shouldn't have a children array, it should have a testcases array
  if((count($node) == 0) || (!array_key_exists($category_structure[0], $node['children']))) {
      $new_node = array( 'name' => $category_structure[0], 'testcases' => array());
      if($node !== $data) {
        $node = &$node['children'][array_push($node['children'], $new_node)-1];   
      } else {
        $node = &$node[array_push($node, $new_node)-1];
      } 
  }
  $testcase = array(
    "name"   => $category_structure[0],
    "time"   => $row['timestamp'],
    "fps_min"=> $row['min_fps'],
    "fps_avg"=> $row['avg_fps'],
    "fps_max"=> $row['max_fps'],
    "server_id"=> $row['server_id'],
    "server_name"=> $row['server_name'],
    "history"=> array());
    // Now build the history array
    $history_fetch = mysqli_query($db, "SELECT min_fps, avg_fps, max_fps, timestamp, server_id, server_name 
                                  FROM performance_results 
                                    JOIN test_runs
                                      ON performance_results.test_run_id=test_runs.id
                                    JOIN servers
                                      ON test_runs.server_id=servers.id
                                    WHERE case_id=" .$row['case_id']. " AND timestamp<\"" .$row['timestamp'] . "\" ORDER BY timestamp ASC");
    while($hist_row = mysqli_fetch_assoc($history_fetch)) {
       array_push($testcase['history'], array(
      "time"   => $hist_row['timestamp'],
      "fps_min"=> $hist_row['min_fps'],
      "fps_avg"=> $hist_row['avg_fps'],
      "fps_max"=> $hist_row['max_fps'],
      "server_id"=> $hist_row['server_id'],
      "server_name"=> $hist_row['server_name']
       ));
    }
    // All that remains now is to push the testcase to the node's testcases array
    array_push($node['testcases'], $testcase);
}

// After this is done we've successfully built our object and just need to convert it to json.
  echo json_encode($data);


?>