function LoadSQLModel(test_run_id) {
  // The model is a tree implemented with nested arrays
  // Every node is a category and every leaf is a test file.
  //
  // The node members are:
  // name : string
  // children : an array of other nodes
  //
  // The leaf (ie. test file) members are:
  // name : the test filename
  // testcases : an array of testcases
  // 
  // The testcases array has one entry for every testcase in the test file
  // The testcase members are:
  // name : the testcase name
  // time : the timestamp of the test run
  // server_id : the id of the server that ran this test
  // server_name : the name of the server that ran this test
  // min_fps : float
  // avg_fps : float
  // max_fps : float
  // history : an array containing history data from other runs of the same test
  //
  // The history array contains one entry for every previous run of this testcase on every test server sorted by their timestamp
  // The history object members are:
  // time : the timestamp of the testrun
  // server_id : the id of the server that ran this test
  // server_name : the name of the server that ran this test
  // min_fps : float
  // avg_fps : float
  // max_fps : float
  
  var res = []
  
  // Connect database
  $.ajax({
      type: 'GET',
      url: 'get_results.php?test_run_id=' + test_run_id,
      dataType: 'json',
      success: function(data) { res = data; },
      async: false
  });
  
  return res;
}
   
var all_graphs = [];
var display_options =  {
  properties: {
    available: ["min_fps", "avg_fps", "mean_fps", "max_fps"],
    selected: ["min_fps", "avg_fps", "mean_fps", ""],
    ignore: ["name", "time", "history", "server_id", "server_name", "test_run_id", "filename", "result_type", "step_name", "stdout", "stderr", "console_output", "console_baseline", "console_diff"],
  },
  servers:  {
    available: [],
    selected: [],
  }
};

function generateDisplayOptionsList(model) {
  for(var i = 0; i < model.length; i++) {
    if(model[i]) {
      if(model[i].hasOwnProperty('children'))
        generateDisplayOptionsList(model[i].children); 
      else {
        for(var j = 0; j < model[i].testcases.length; j++) {
          var testcase = model[i].testcases[j];
          if(testcase.result_type=='performance') {
            if($.inArray(testcase.server_name, display_options.servers.available) < 0) {
              display_options.servers.available.push(testcase.server_name);
            }
            if(display_options.servers.selected.length == 0) {
              display_options.servers.selected.push(testcase.server_name);
            }
            for(var propertyName in testcase) {
              if ($.inArray(propertyName, display_options.properties.ignore) < 0) {
                if($.inArray(propertyName, display_options.properties.available) < 0) {
                  display_options.properties.available.push(propertyName);
                }             
              }
            }
            for(var k = 0; k < testcase.history.length; k++) {
              if($.inArray(testcase.history[k].server_name, display_options.servers.available) < 0) {
                display_options.servers.available.push(testcase.history[k].server_name);
              }        
              for(var propertyName in testcase.history[k]) {
                if ($.inArray(propertyName, display_options.properties.ignore) < 0) {
                  if($.inArray(propertyName, display_options.properties.available) < 0) {
                    display_options.properties.available.push(propertyName);
                  }             
                }
              }
            }
          }
        }
      }               
    }
  }
}

function refreshDisplayOptions(model) {
  $('#Option_Properties').empty();
  $('#Option_Servers').empty();
  generateDisplayOptionsList(model);
  $('#Option_Properties').append('<h3>Properties:</h3>');
  $('#Option_Servers').append('<h3>Servers:</h3>');
  for(var i = 0; i < display_options.properties.available.length; i++) {
    var cb = $('<input>');
    cb.attr('type', 'checkbox');    
    if($.inArray(display_options.properties.available[i], display_options.properties.selected) > -1)
      cb.prop('checked', true);
      
    cb.data('propName', display_options.properties.available[i]);
    
    $('#Option_Properties').append(cb);
    $('#Option_Properties').append(display_options.properties.available[i] + "<br/>")
    
    cb.change(function() {
      if(!$(this).prop('checked')) {
        if(display_options.properties.selected.length > 1) {
         var index = $.inArray($(this).data('propName'), display_options.properties.selected);
          if(index > -1) { // If it's in the selected list then remove it from the list
            display_options.properties.selected.splice(index, 1); // This just removes this one element from the list.
          }
        } else {
          $(this).prop('checked', true);
          return;
        }
      } else {
       var index = $.inArray($(this).data('propName'), display_options.properties.selected);
        if(index < 0) { // If it isn't in the selected list then add it
          display_options.properties.selected.push($(this).data('propName'));
        }     
      }
      $('.TestResult').each(function() { if($(this).data('model').result_type=="performance") generateGraph($(this));}); // regenerates all the graphs
    });    
    
  }
  for(var i = 0; i < display_options.servers.available.length; i++) {
    var cb = $('<input>');
    cb.attr('type', 'checkbox');
    if($.inArray(display_options.servers.available[i], display_options.servers.selected) > -1)
      cb.prop('checked', true);
      
    cb.data('propName', display_options.servers.available[i]);
    
    $('#Option_Servers').append(cb);
    $('#Option_Servers').append(display_options.servers.available[i] + "<br/>");
    
    cb.change(function() {
      if(!$(this).prop('checked')) {
        if(display_options.servers.selected.length > 1) {
         var index = $.inArray($(this).data('propName'), display_options.servers.selected);
          if(index > -1) { // If it's in the selected list then remove it from the list
            display_options.servers.selected.splice(index, 1); // This just removmes this one element from the list.
          }
        } else {
          $(this).prop('checked', true);
          return;
        }
      } else {
       var index = $.inArray($(this).data('propName'), display_options.servers.selected);
        if(index < 0) { // If it isn't in the selected list then add it
          display_options.servers.selected.push($(this).data('propName'));
        }     
      }
      $('.TestResult').each(function() { if($(this).data('model').result_type=="performance") generateGraph($(this));}); // regenerates all the graphs
    });
  }
}


function generateDatasets(testcase) {
  var datasets = [];
  var hue = 80;
  
  for(var s = 0; s < display_options.servers.selected.length; s++) {
    var server = display_options.servers.selected[s];
    for(var propertyName in testcase) {
      if(($.inArray(propertyName, display_options.properties.selected) > -1) || (propertyName == "server_name") || (propertyName == "time")) {
        var color = "hsla("+Math.round(hue)+", 60%, 60%";
        var config = {
        label: propertyName, 
        fill: false,
        backgroundColor: color+", 0.2)",
        borderColor: color+", 1)",
        pointBorderColor: color+", 1)",
        pointBackgroundColor: "#fff",
        pointBorderWidth: 1,
        pointHoverRadius: 5,
        pointHoverBackgroundColor: color+", 1)",
        pointHoverBorderColor: color+", 1)",
        pointHoverBorderWidth: 2,
        data: [],
        labels : [],
        server_name : server,
        highest_y_value : 60
        };
        hue = (hue + 20 + Math.random()*40) % 360;
        
        var highest = 60;
        var new_dataset = [];
        if(!isNaN(testcase[propertyName]))
          highest = Math.max(testcase[propertyName], highest);
        if(testcase.hasOwnProperty("history")) {
          for(var j = 0; j < testcase.history.length; j++) {
            if((testcase.history[j].server_name == server)) {
              config.labels.push(testcase.history[j].time);
              if(!isNaN(testcase.history[j][propertyName]))
                highest = Math.max(testcase.history[j][propertyName], highest);
              new_dataset.push({x: testcase.history[j].time, y: testcase.history[j][propertyName]});
            }
          }
        }
        if(testcase.server_name == server) {
          new_dataset.push({x: testcase.time, y: testcase[propertyName]});
          config.labels.push(testcase.time);
          if(!isNaN(highest))
            highest = Math.max(highest, config.highest_y_value);
        }
        config.highest_y_value = highest;
        config.data = new_dataset;
        if(config.labels.length > 0) { // Only add this to the dataset list if it actually contains something.
          // But first make sure every previous dataset has matching points. And that this has matching points for every previous dataset.
          // Algorithm: For every previously existing dataset:
          //   Store an index into their dataset label array and my dataset label array.
          //   If one of the two indexes has reached the end of its list then add all remaining timestamps in the other list to the first list
          //   If the two timestamps at these indexes are equal then increment both indexes.
          
          for(var prev = 0; prev < datasets.length; prev++) {
            var theirs = 0;
            var len_t = datasets[prev].labels.length;
            var mine = 0;
            var len_m = config.labels.length;
            while((theirs < len_t) || (mine < len_m)) {
              if(theirs < len_t) {
                var their_timestamp = Date.parse(datasets[prev].labels[theirs]);
              } else {
                var their_timestamp = Date.now();
              }
              if(mine < len_m) {
                var my_timestamp = Date.parse(config.labels[mine]);
              } else {
                var my_timestamp = Date.now();
              }
                            
              if(their_timestamp == my_timestamp) {
                theirs++;
                mine++;
              } else if(their_timestamp < my_timestamp) {
                config.labels.splice(mine, 0, datasets[prev].labels[theirs]);
                if(config.label == "server_name")
                  config.data.splice(mine, 0, {x: datasets[prev].labels[theirs], y: config[config.label]});
                else
                  config.data.splice(mine, 0, {x: datasets[prev].labels[theirs]});
                theirs++;
                mine++;
              } else if(their_timestamp > my_timestamp) {
                datasets[prev].labels.splice(theirs, 0, config.labels[mine]);
                if(datasets[prev].label == "server_name")
                  datasets[prev].data.splice(theirs, 0, {x: config.labels[mine], y: datasets[prev][config.label]});
                else
                  datasets[prev].data.splice(theirs, 0, {x: config.labels[mine]});
                theirs++;
                mine++;
              } else {
                //Something went wrong if we end up here.
                break;                
              }
              
            }
          }
          datasets.push(config);
        }
     }
    }
  }
  return datasets;
}


function generateGraph(div) {
  $('.TestResult_graph', div).remove();
  $('.TestResult_data', div).remove();
  var model = div.data('model');
  var graph_data = generateDatasets(model);
  var highest = 60;
  for(dataset in graph_data) {
    highest = Math.max(highest, dataset.highest_y_value);
  }
    var graph_div = $('<div>');
    graph_div.addClass('TestResult_graph');        
    
    var graph_canvas = $('<canvas>');
    graph_div.append(graph_canvas);

  var graph_config = 
    {
      type : 'line',
      data: {
        labels: graph_data[0].labels,
        datasets: graph_data
        },
      options:{
        maintainAspectRatio: true,
        responsive: true,
        scaleShowLabels: true,
        legendCallback : function(controller){
        var data = controller.data;
          var res = "<p>Legend:</br>";
          var server = data.datasets[0].server_name;
            res += "<span>[" + server + "]</span></br>";
          for (var k=0; k<data.datasets.length; k++){
            if(data.datasets[k].server_name != server) {
              server = data.datasets[k].server_name;
              res += "<span>[" + server + "]</span></br>";
            }
            if($.inArray(data.datasets[k].label, display_options.properties.ignore) < 0) {
              res += "<span style=\"color:" +data.datasets[k].pointBorderColor+"\">";
              if(data.datasets[k].label){
                res += data.datasets[k].label;
              }
              res += "</span></br>";                  
            }
          }
          res += "</p>";							
          return res;
        },
          tooltips: {
              mode: 'label',
              callbacks : {
                label: function(tooltipItem, data) {
                  return data.datasets[tooltipItem.datasetIndex].label + ': ' + data.datasets[tooltipItem.datasetIndex].data[tooltipItem.index].y;
                }
              }
          },        
          scales: {
              xAxes: [{
                  display: true,
                  ticks : {
                    userCallback: function(value) {
                      if(value){
                        return value;//.substring(0, value.indexOf(' '));
                      } else {
                        return '';
                      }
                    }
                  }
              }],
              yAxes: [{
                  display: true,
                  scaleLabel: {
                      show: true,
                      labelString: 'Value'
                  },
                  ticks : {
                    beginAtZero: true,
                    suggestedMin: 0,
                    suggestedMax: highest,
                    userCallback: function(value) {
                    return value;
                    }
                  }
              }]
          },
      }
    }       
  
  // Chart.js will fail if the canvas isn't visible in the document when Chart() is called. For that reason we append graph_div to the body, call Chart() and then detach it so we can put it in div afterwards.
  $('body').append(graph_div);

  model.chart = Chart.Line(graph_canvas[0].getContext('2d'), graph_config);

  graph_div.detach();
 
  div.append(graph_div); 
  var data_div = $('<div>');
  data_div.addClass('TestResult_data');
  var data_list = $('<ul>');
  data_list.addClass('TestResult_data_list');
  data_list.append('Latest:');
  for(var i = 0; i < display_options.properties.selected.length; i++) {
    var prop = display_options.properties.selected[i];
    var line = '<li>' + prop + ': ';
    if(model.hasOwnProperty(prop))
      line += model[prop];
    else
      line += 'N/A';
    line += '</li>';
    data_list.append(line);
  }

           
  var getprops = function getAllProperties(obj){
    var allProps = []
      , curr = obj
    do{
        var props = Object.getOwnPropertyNames(curr)
        props.forEach(function(prop){
            if (allProps.indexOf(prop) === -1)
                allProps.push(prop)
        })
    }  while(curr = Object.getPrototypeOf(curr))
    return allProps
  }
  
  data_list.append(model.chart.generateLegend());
  data_div.append(data_list);
  div.append(data_div);
}


function getImageBlobURL(blob, download_name) {
  var byteCharacters = atob(blob);
  var byteNumbers = new Array(byteCharacters.length);
  for (var i = 0; i < byteCharacters.length; i++) {
      byteNumbers[i] = byteCharacters.charCodeAt(i);
  }  
  var arrayBufferView = new Uint8Array(byteNumbers);
  var blob = new Blob( [ arrayBufferView ], { type: "image/png" } );
  var urlCreator = window.URL || window.webkitURL;
  var imageUrl = urlCreator.createObjectURL( blob );
  
  var link = $("<a>");
  link.attr("href", imageUrl);
  link.attr("target", imageUrl);
  link.attr("download", download_name);
  var img = $("<img>");
  img.attr("src", imageUrl);
  img.addClass("TestResult_image");
  link.append(img);
  return link;
}

function generateImages(div) {
  var testcase = $(div).data('model');
  var container = $('<div>');
  container.addClass('TestResult_rendering');
  
  var download_name = testcase.filename.split('/').pop() + "_" + testcase.step_name + ".png";
  var diff_download_name =  "diff_" + download_name;
  // If it succeded then show the baseline image
  // If it failed and there's no baseline then show the output and complain about the lack of a baseline
  // If it failed and there is a baseline then show the baseline, the diff and the output
  if(testcase.success == 'Y') {
    var succeeded = $("<span>");
    succeeded.addClass('test_successful');
    succeeded.append("Step successful!");
    div.append(succeeded);
    var image_container = $('<div>');
    if(testcase.baseline_image != "") {
      var image_container = $('<div>');
      image_container.addClass('TestResult_image_div'); 
      image_container.append("Baseline:</br>");
      image_container.append(getImageBlobURL(testcase.baseline_image, download_name));
      container.append(image_container);
    }
  } else { // Didn't succeed
    var succeeded = $("<span>");
    succeeded.addClass('test_failed');
    div.append(succeeded);
    
    if (testcase.diff_image == "") {
      if(testcase.output_image == "")
         succeeded.append("Step failed - No image output!");
      else
         succeeded.append("Step failed - No diff available!");
    }    
    else if (testcase.baseline_image == "")
      succeeded.append("Step failed - No baseline found!");        
    else
      succeeded.append("Step failed - Invalid output!");
        
    if(testcase.baseline_image != "") {
      var image_container = $('<div>');
      image_container.addClass('TestResult_image_div'); 
      image_container.append("Baseline:</br>");
      image_container.append(getImageBlobURL(testcase.baseline_image, download_name));
      container.append(image_container);
    }
    if(testcase.output_image != "") {
      var image_container = $('<div>');
      image_container.addClass('TestResult_image_div'); 
      image_container.append("Output:</br>");
      image_container.append(getImageBlobURL(testcase.output_image, download_name));
      container.append(image_container);
    }
    if(testcase.output_image != "") {
      var image_container = $('<div>');
      image_container.addClass('TestResult_image_div'); 
      image_container.append("Diff:</br>");
      image_container.append(getImageBlobURL(testcase.diff_image, download_name));
      container.append(image_container);
    } 
  }
  div.append(container);
}

function generateConsole(div) {
  var testcase = $(div).data('model');
  var container = $('<div>');
  var succeeded = $("<span>");
  div.append(succeeded);
  
  if(testcase.success == "N"){
    succeeded.addClass('test_failed');
    if(testcase.text_output == "")
      succeeded.append("Step failed - No output!");
    else if(testcase.text_baseline == "")
      succeeded.append("Step failed - No baseline!");
    else
      succeeded.append("Step failed - Invalid output!");
  } else {
    succeeded.addClass('test_successful');
    succeeded.append("Step successful!");
  } 
  
  container.addClass('TestResult_console');
  var output = $('<div>');
  output.addClass('stdout_div');
  output.append("<b>Output:</b></br></br>");
  output.append(testcase.text_output.split('\n').join('</br>'));
  container.append(output);
  if(testcase.success == "N") {  
    var baseline = $('<div>');
    baseline.addClass('stdout_div');
    baseline.append("<b>Baseline:</b></br></br>");
    baseline.append(testcase.text_baseline.split('\n').join('</br>'));
    container.append(baseline);
    var diff = $('<div>');
    diff.addClass('stdout_div');
    diff.append("<b>Diff:</b></br></br>");
    diff.append(testcase.text_diff.split('\n').join('</br>'));
    container.append(diff);
  }
  div.append(container);
}

function generateError(div) {
  var testcase = $(div).data('model');
  var container = $('<div>');
  container.addClass('TestResult_error');
  
  var std = $('<div>');
  std.addClass('std_div');
  std.append("<b>stdout:</b></br></br>");
  std.append(testcase.stdout.split('\n').join('</br>'));
  std.append("<b>stderr:</b></br></br>");
  std.append(testcase.stderr.split('\n').join('</br>'));
  container.append(std);
  
  div.append(container);
}


function ConstructTestCases(model, target) {
  var container = $('<div>');
  container.addClass('Test_Container');

  if(model.testcases.length > 0) {
    var current_case_name = 'placeholder that shouldn\'t ever match';
    for(var i = 0; i < model.testcases.length; i++) {
      if(model.testcases[i].name != current_case_name) {
        var case_div = $('<div>');
        case_div.addClass('TestCase');
        case_div.addClass('.Category_Item'); 
        var case_name = $('<div>');
        case_name.addClass("TestResult_name");
        case_name.append("Case: " + model.testcases[i].name);
        case_div.append(case_name);
        container.append(case_div);
        current_case_name = model.testcases[i].name;
      }
      var step_div = $('<div>');
      step_div.addClass('TestResult');
      var name_div = $('<div>');
      name_div.addClass('TestStep_name');
      if(model.testcases[i].result_type == 'error') {
        name_div.append("Testcase failed");
        name_div.addClass("test_failed");
      } else {
        name_div.append("Step: " + model.testcases[i].step_name);
      }
      step_div.append(name_div);
      step_div.data('model', model.testcases[i]); // Store the associated testCase with the div
      case_div.append(step_div);
    }
  }  
  target.append(container);
}


var CategoryCount = 0;

  var first = true;
function ConstructList(model, target) {
  target.empty();
  for (var i = 0; i < model.length; i++) {
    if(model[i]) {
      var ul = $('<ul>');
      ul.attr('class', 'Category_Item');
      
      var label = $('<label>');
      label.attr('class', 'noselect');       
      var glyph = $('<div>');
      glyph.attr('class', 'glyph');
      glyph.html('▶');
        
      label.append(glyph);
      var name = $('<h3>'+model[i].name+'</h3>');
      label.append(name);

      label.attr('for', 'category'+CategoryCount);
      
      var input = $('<input>');
      input.attr('type', 'checkbox');
      input.attr('id', 'category'+CategoryCount);
      input.addClass('category_list_checkbox');
      if(first)
        input.prop('checked', true);
      ul.append(input);
      ul.append(label);
      
      CategoryCount++;
      
      if(model[i].hasOwnProperty('children'))
        ConstructList(model[i].children, ul);
      else if (model[i].hasOwnProperty('testcases')) {
        if(model[i].success)
          name.addClass('test_successful');
        else
          name.addClass('test_failed');
        ConstructTestCases(model[i], ul);
        first = false;
      }
             
      target.append(ul);
    }
  }      
}

function GetServerList() {
  var res = []
  
  // Connect database
  $.ajax({
      type: 'GET',
      url: 'get_servers.php',
      dataType: 'json',
      success: function(data) { res = data; },
      async: false
  });
  var target = $('#Servers_List');
  target.empty();
  for(var i = 0; i < res.length; ++i) {
    var div = $('<div>');
    div.addClass('TestServer');
    div.addClass("noselect");
    div.append(res[i].name);
    div.data("server_id", res[i].id);
    div.click(function(){
      GetTestRunList($(this).data("server_id"));
      $(".Selected_Server").removeClass('Selected_Server');
      $(this).addClass('Selected_Server');
    });
    target.append(div);
  }
}

function GetTestRunList(server_id) {
  var res = []
  
  // Connect database
  $.ajax({
      type: 'GET',
      url: 'get_test_runs.php?server_id=' + server_id,
      dataType: 'json',
      success: function(data) { res = data; },
      async: false
  });
  $("#TestRuns").show();
  var target = $("#TestRuns_List");
  target.empty();
  for(var i = 0; i < res.length; ++i) {
    var div = $('<div>');
    div.addClass('TestRun');
    div.addClass("noselect");
    div.append(res[i].timestamp);
    div.data("test_run_id", res[i].id);
    if(res[i].has_results) {
      div.click(function(){
        SetTestRun($(this).data("test_run_id"));
        $(".Selected_TestRun").removeClass('Selected_TestRun');
        $(this).addClass('Selected_TestRun');      
      });
    } else {
      div.addClass('TestRun_NoResults');
    }
    target.append(div);
  }
}

function SetTestRun(test_run_id) {
  model = LoadSQLModel(test_run_id);
  refreshDisplayOptions(model);
  first = true;
  ConstructList(model, $('#Categories_List'));
  $('.TestResult').each(function() {
  var testResult = $(this).data('model');
    if(testResult.result_type=="performance") {
      generateGraph($(this));
    } else if (testResult.result_type=="rendering") {
      generateImages($(this));
    } else if (testResult.result_type=="console") {
      generateConsole($(this));
    } else if (testResult.result_type=="custom") {
      generateConsole($(this));
    } else if (testResult.result_type=='error') {
      generateError($(this));
    }
  });  
}



var model = null;

$(document).ready(function(){
  $('#Options').draggable();
  GetServerList(); 
});    

          
