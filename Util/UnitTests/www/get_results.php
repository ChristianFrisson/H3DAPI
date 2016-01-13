<?php
require('DB/mysqli.php');

$db = mysqli_connect("localhost", "ResultReader", "results", "testserver");
if(mysqli_connect_errno($db)) {
  echo "Failed to connect to MySQL: " . mysqli_connect_error();
}

$test_run_id = $_GET['test_run_id'];
  
$query = sprintf("
(SELECT performance_results.id AS id,test_runs.timestamp,server_id,server_name,test_run_id,file_id,filename,case_id,
        'performance' AS result_type,case_name,performance_results.step_id,step_name,NULL AS success,NULL AS
        output_image,NULL AS diff_image,NULL AS baseline_image,min_fps,max_fps,mean_fps,avg_fps, NULL AS stdout, NULL AS stderr,NULL AS
        text_output,NULL AS text_baseline,NULL AS text_diff
 FROM   test_runs
        left join performance_results
               ON performance_results.test_run_id = test_runs.id
        join test_cases
          ON performance_results.case_id = test_cases.id
        join test_steps
          ON performance_results.step_id = test_steps.id
        join test_files
          ON performance_results.file_id = test_files.id
        join servers
          ON test_runs.server_id = servers.id
 WHERE  test_runs.id = %d
)
UNION ALL
(SELECT rendering_results.id AS id,test_runs.timestamp,server_id,server_name,rendering_results.test_run_id,
        rendering_results.file_id,filename,rendering_results.case_id,'rendering' AS result_type,case_name,
        rendering_results.step_id,step_name,success,rendering_results.output_image,rendering_results.diff_image,image AS
        baseline_image,NULL AS min_fps,NULL AS max_fps,NULL AS mean_fps,NULL AS avg_fps, NULL AS stdout, NULL AS stderr,NULL AS
        text_output,NULL AS text_baseline,NULL AS text_diff
 FROM   test_runs
        left join rendering_results
               ON rendering_results.test_run_id = test_runs.id
        join test_cases
          ON rendering_results.case_id = test_cases.id
        join test_steps
          ON rendering_results.step_id = test_steps.id
        join test_files
          ON rendering_results.file_id = test_files.id
        join servers
          ON test_runs.server_id = servers.id
        left join rendering_baselines AS baseline
               ON rendering_results.case_id = baseline.case_id
                  AND rendering_results.step_id = baseline.step_id
 WHERE  test_runs.id = %d
 GROUP  BY case_id,file_id,step_name
)
UNION ALL
(SELECT console_results.id AS id,test_runs.timestamp,server_id,server_name,console_results.test_run_id,
        console_results.file_id,
        filename,console_results.case_id,'console' AS result_type,case_name,console_results.step_id,step_name,success,
        NULL AS output_image,NULL AS diff_image,NULL AS baseline_image,NULL AS min_fps,NULL AS max_fps,NULL AS mean_fps,
        NULL AS avg_fps, NULL AS stdout,NULL AS stderr,output AS text_output,baseline AS text_baseline, diff AS text_diff
 FROM   test_runs
        left join console_results
               ON console_results.test_run_id = test_runs.id
        join test_cases
          ON console_results.case_id = test_cases.id
        join test_steps
          ON console_results.step_id = test_steps.id
        join test_files
          ON console_results.file_id = test_files.id
        join servers
          ON test_runs.server_id = servers.id
 WHERE  test_runs.id = %d
 GROUP  BY case_id,file_id,step_name)
UNION ALL
(SELECT custom_results.id AS id,test_runs.timestamp,server_id,server_name,custom_results.test_run_id,
        custom_results.file_id,
        filename,custom_results.case_id,'custom' AS result_type,case_name,custom_results.step_id,step_name,success,
        NULL AS output_image,NULL AS diff_image,NULL AS baseline_image,NULL AS min_fps,NULL AS max_fps,NULL AS mean_fps,
        NULL AS avg_fps, NULL AS stdout,NULL AS stderr,output AS text_output,baseline AS text_baseline, diff AS text_diff
 FROM   test_runs
        left join custom_results
               ON custom_results.test_run_id = test_runs.id
        join test_cases
          ON custom_results.case_id = test_cases.id
        join test_steps
          ON custom_results.step_id = test_steps.id
        join test_files
          ON custom_results.file_id = test_files.id
        join servers
          ON test_runs.server_id = servers.id
 WHERE  test_runs.id = %d
 GROUP  BY case_id,file_id,step_name)
UNION ALL
(SELECT error_results.id AS id,test_runs.timestamp,server_id,server_name,error_results.test_run_id,
        error_results.file_id,
        filename,error_results.case_id,'error' AS result_type,case_name,error_results.step_id,step_name, NULL AS success,
        NULL AS output_image,NULL AS diff_image,NULL AS baseline_image,NULL AS min_fps,NULL AS max_fps,NULL AS mean_fps,
        NULL AS avg_fps, stdout, stderr,NULL AS text_output,NULL AS text_baseline, null AS text_diff
 FROM   test_runs
        left join error_results
               ON error_results.test_run_id = test_runs.id
        join test_cases
          ON error_results.case_id = test_cases.id
        join test_steps
          ON error_results.step_id = test_steps.id
        join test_files
          ON error_results.file_id = test_files.id
        join servers
          ON test_runs.server_id = servers.id
 WHERE  test_runs.id = %d
 GROUP  BY case_id,file_id,step_name) 
ORDER  BY case_id, step_id ASC ", $test_run_id, $test_run_id, $test_run_id, $test_run_id, $test_run_id);

$data = generate_results($db, $query);

// After this is done we've successfully built our object and just need to convert it to json.
  echo json_encode($data, JSON_PRETTY_PRINT);
	
function generate_results($db, $query){	
if(!$fetch_result = mysqli_query($db, $query)) {
		die("ERROR: " . mysql_error());
	}	

 
  /*
    Tree-building algorithm:

    for every row:
      loop through the $data array and look for an element with a name property that matches the first name in the filename path.
      if it doesn't exist
        create and add it to data (create with a children array if it isn't the last part of the filename path, otherwise with a testcases array)
        point $node at the newly added node
      else
        point $node at it
        
      if there is more than one element in the filename path
        remove the first element of the filename path
      
      while there's more than 1 parts left of the filename path
        if the current node has a child with a name that matches the first part of the filename path
          point $node at that child node
        else
          create a new node, same as before, and add it to that children array
          point $node at the new node
        remove the first element of the filename path
      
      if node contains a children array
        if node children contains an element for this testcase
          point $node at that element
        else
          create an element for this testcase
          point $node at that element
      else
        //if we get here then that means this is a top-level leaf and we can just proceed to the end of the code
      
      add the data to $node['testcases']
        
  */									

  $data = array();
  
  if(mysqli_num_rows($fetch_result) == 0) {
  $testcase = array(
    "name"   => "No results found",
    "testcases" => array([
      "step_name"  => "",
      "filename"   => "Error",
      "result_type" => "ignore"])
    );  
    $data = array($testcase);
  }
  while($row = mysqli_fetch_assoc($fetch_result)) {
  //  echo json_encode($row);
    $filename = $row['filename'];
    $category_structure = explode('/', $filename);
    // Store a reference to the root of the data tree. We'll move this reference down the tree until we reach the part where the leaf should go,
    // creating any missing nodes on the way there.
    // NOTE: json_encode won't output an array if there are any named elements in it. It'll turn those into objects.
    $node = &$data;

    $index = -1;
    for($i = 0; $i < count($data); $i++) {
      if(strcmp($data[$i]['name'], $category_structure[0]) == 0) {
        $index = $i;
        break;
      }
    }
    if($index == -1) { // We don't have a top-level entry for this category or testcase, so let's add it.
      if(count($category_structure) > 1) {
        $node = &$data[array_push($data, array("name" => $category_structure[0], "children" => array()))-1];
      } else {
        $node = &$data[array_push($data, array("name" => $category_structure[0], "testcases" => array()))-1];
      }
    } else {
      $node = &$data[$index];
    }
    if(count($category_structure) > 1)
      array_shift($category_structure);
  //  echo '</br>it is now ' . json_encode($category_structure) ."</br>";
          
    // Go through every folder specified in the casename path and build our array structure.
    while(count($category_structure) > 1) {
      $index = -1;
      for($i = 0; $i < count($node['children']); $i++) {
        if((strcmp($node['children'][$i]['name'], $category_structure[0]) == 0)) {
          $index = $i;
          break;
        }      
      }
      if($index > -1) {
        $node = &$node['children'][$i];
      } else {     
        $new_node = array("name" => $category_structure[0], "children" => array());
        $node = &$node['children'][array_push($node['children'], $new_node)-1];
  //  echo "node is: </br>" . json_encode($node) . "</br>";
      }
      // Remove the first element from $category_structure
      array_shift($category_structure); 
    }
    //if node contains a children array then it is the category in which the testcase leaf should be
    if(array_key_exists('children', $node)) {
      
      $index = -1;
      for($i = 0; $i < count($node['children']); $i++) {
        if((strcmp($node['children'][$i]['name'], $category_structure[0]) == 0)) {
          $index = $i;
          break;
        }      
      }
      //if node children contains an element for this testfile
      if($index > -1) {
        //point $node at that element
        $node = &$node['children'][$index];
      } else {
        //create and push an element for this testfile into the array
        //point $node at that element
        $new_node = array( 'name' => $category_structure[0], 'testcases' => array(), 'success' => true);
        $node = &$node['children'][array_push($node['children'], $new_node)-1];
        }
    }

    $testcase = array(
      "name"   => $row['case_name'],
      "filename"   => $row['filename'],
      "result_type"   => $row['result_type'],
      "test_run_id" => $row['test_run_id'],
      "server_id"=> $row['server_id'],
      "server_name"=> $row['server_name'],
      "time"   => $row['timestamp']
      );
    $testcase["step_name"] = $row['step_name'];
    
    if($row['result_type'] == "rendering") {
      if($row['success'] =='N') {
        $node['success'] = false;
      }
      $testcase["success"] = $row['success'];
      $testcase["output_image"] = base64_encode($row['output_image']);
      $testcase["diff_image"] = base64_encode($row['diff_image']);
      $testcase["baseline_image"] = base64_encode($row['baseline_image']);
    }
    else if($row['result_type'] == "performance") {
      $testcase["min_fps"] = $row['min_fps'];
      $testcase["avg_fps"] = $row['avg_fps'];
      $testcase["mean_fps"] = $row['mean_fps'];
      $testcase["max_fps"] = $row['max_fps'];
    $testcase['history'] = array();
    // Now build the history array
    $query = "SELECT min_fps, avg_fps, max_fps, mean_fps, timestamp, server_id, server_name, test_run_id 
                                  FROM performance_results 
                                    JOIN test_runs
                                      ON performance_results.test_run_id=test_runs.id
                                    JOIN servers
                                      ON test_runs.server_id=servers.id
                                    WHERE case_id=" .$row['case_id']. " AND timestamp<=\"" .$row['timestamp'] . "\" AND test_run_id!=".$row['test_run_id']." ORDER BY timestamp ASC";
  //  echo "<br><br>Query: ".$query;
    $history_fetch = mysqli_query($db, $query);
    while($hist_row = mysqli_fetch_assoc($history_fetch)) {
       array_push($testcase['history'], array(
      "test_run_id" => $hist_row['test_run_id'],
      "server_id"=> $hist_row['server_id'],
      "server_name"=> $hist_row['server_name'],
      "time"   => $hist_row['timestamp'],
      "min_fps"=> $hist_row['min_fps'],
      "avg_fps"=> $hist_row['avg_fps'],
      "mean_fps"=>$hist_row['mean_fps'],
      "max_fps"=> $hist_row['max_fps'],
       ));
       }
    }
    else if ($row['result_type'] == 'console') {
      if($row['success'] =='N') {
        $node['success'] = false;
      }
      $testcase["success"] = $row['success'];
      $testcase["text_output"] = $row["text_output"];
      $testcase["text_baseline"] = $row["text_baseline"];
      $testcase["text_diff"] = $row["text_diff"];
    } else if ($row['result_type'] == 'custom') {
      if($row['success'] =='N') {
        $node['success'] = false;
      }
      $testcase["success"] = $row['success'];
      $testcase["text_output"] = $row["text_output"];
      $testcase["text_baseline"] = $row["text_baseline"];
      $testcase["text_diff"] = $row["text_diff"];
    } else if ($row['result_type'] == 'error') {
      $node['success'] = false;
      $testcase["stdout"] = $row['stdout'];
      $testcase["stderr"] = $row['stderr'];
    }
    
    // All that remains now is to push the testcase to the node's testcases array
    array_push($node['testcases'], $testcase);
  }
  return $data;
}



?>