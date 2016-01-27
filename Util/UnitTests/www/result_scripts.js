function LoadSQLModel(test_run_id, result_callback) {
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
  
  // Connect database
  $.ajax({
      type: 'GET',
      url: 'get_results.php?test_run_id=' + test_run_id,
      dataType: 'json',
      success:result_callback,
      async: true
  });
}
   
var all_graphs = [];
var display_options =  {
  properties: {
    available: ["min_fps", "avg_fps", "mean_fps", "max_fps"],
    selected: ["avg_fps", "mean_fps"],
    ignore: ["name", "time", "history", "server_id", "server_name", "test_run_id", "filename", "result_type", "step_name", "stdout", "stderr", "console_output", "console_baseline", "console_diff", "id", "success"],
  },
  servers:  {
    available: [],
    selected: [],
  }
};

function generateDisplayOptionsList(model) {
  if(model) {
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
}

function refreshDisplayOptions(model) {
  $('#Option_Properties').empty();
  $('#Option_Servers').empty();
  generateDisplayOptionsList(model);
  $('#Option_Properties').append('<h3 class="Options_Header">Properties:</h3>');
  $('#Option_Servers').append('<h3 class="Options_Header">Servers:</h3>');
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




var highest_x = 60;
var lowest_time = 0;
var highest_time = 0;
var highest_y = 0;
function generateDatasets(testcase) {
  var stack_count = 0;
  var datasets = [];
    for(var s = 0; s < display_options.servers.selected.length; s++) {
    var server = display_options.servers.selected[s];
    for(var propertyName in testcase) {
      if(($.inArray(propertyName, display_options.properties.selected) > -1)) {
    ++stack_count;
        var new_dataset = [];
        if(testcase.hasOwnProperty("history")) {
          for(var j = 0; j < testcase.history.length; j++) {
            if((testcase.history[j].server_name == server)) {
              if(lowest_time == 0 || new Date(testcase.history[j].time) < new Date(lowest_time)) {
                lowest_time = testcase.history[j].time;
              }
              if(highest_time == 0 || testcase.history[j].time > highest_time) {
                highest_time = testcase.history[j].time;
              }              
              new_dataset.push([new Date(testcase.history[j].time).getTime(), testcase.history[j][propertyName]]);
            }
          } 
        }
        if(!isNaN(testcase[propertyName])) {
          if(lowest_time == 0 || new Date( testcase.time) < new Date(lowest_time)) {
            lowest_time = testcase.time;
          }
          if(highest_time == 0 || new Date(testcase.time) > new Date(highest_time)) {
            highest_time = testcase.time;
          }           
          new_dataset.push([new Date(testcase.time).getTime(), testcase[propertyName]]);
        }
      datasets.push({label: propertyName, stack: stack_count, data: new_dataset, server_name: testcase['server_name']});
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

  var graph_div = $('<div>');
  graph_div.addClass('TestResult_graph');
  
  if(lowest_time == highest_time) {
    highest_time = (new Date(lowest_time).getTime()+1)
  }
  highest_time = (new Date(highest_time).getTime())
  lowest_time = (new Date(lowest_time).getTime())
  
  
  var graph_options = {
    series: {
      lines: {
        show: true,
        fill: true,
        steps: false
      },
      points: {
        show: true
      }
    },
    grid:  { hoverable: true }, //important! flot.tooltip requires this
    tooltip: {
      show: true,
      content: function(label, xval, yval, flotItem) {
        return flotItem.series.server_name + "<br/>" + label + ": " + yval;
      }
    },
    xaxis: {
      mode: "time",
      min: lowest_time,
      max: highest_time,
      tickFormatter: function (val, axis) {
          var d = new Date(val);
          return d.toISOString().split("T")[0];
      }
    },
    yaxis: {
      min: 0
    },
    zoom: {
      interactive: true
    },
    pan: {
      interactive: true
    }    
  }
		
  $('body').append(graph_div);

  // Options can be set globally. 
 
  $.plot(graph_div, graph_data, graph_options);
  
  div.append(graph_div); 

           
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
  
  var download_name = testcase.name + "_" + testcase.step_name + ".png";
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
    if((testcase.output_image != "") && (testcase.baseline_image != "")) {
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
  output.append("<b style='text-transform:capitalize;'>" + testcase.result_type + ":</b></br></br>");
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
  std.append(testcase.stdout.split('\n').join('</br>'));
  std.append("</br></br><b>stderr:</b></br></br>");
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
        case_div.addClass('Category_Item'); 
        var case_name = $('<div>');
        case_name.addClass("TestResult_name");
        
        case_name.addClass("minimized");
        case_name.click(function(){ // onclick function for toggling the presence of a minimized-class
          $(this).toggleClass("minimized");
        });
        
        case_name.append("Case: " + model.testcases[i].name);
        if(model.testcases[i].success == 'Y') {
          case_name.addClass("test_successful");
        } else {
          case_name.addClass("test_failed");
        }
        
        case_div.append(case_name);        
        
        container.append(case_div);
        current_case_name = model.testcases[i].name;
      }
      
      if(model.testcases[i].success != 'Y') {
        case_name.removeClass("test_successful");
        case_name.addClass("test_failed");
      }
      
      // This is specifically for suppressing the green label
//      if(model.testcases[i].result_type == 'error' && $(".TestResult", case_div).length == 0 && (i < model.testcases.length-1 && (model.testcases[i+1].name != current_case_name)) ) {
//        case_name.removeClass("test_successful");
//        case_name.addClass("test_failed");
 //     }
            
      var step_div = $('<div>');
      step_div.addClass('TestResult');
      var name_div = $('<div>');
      name_div.addClass('TestStep_name');
      name_div.click(function(){ // onclick function for toggling the presence of a minimized-class
        $(this).toggleClass("minimized");
      });
        
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

var first = false;
function ConstructList(model, target) {

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
      if(first) {
        input.prop('checked', true);
        first = false;
      }
      ul.append(input);
      ul.append(label);
      
      CategoryCount++;
      
      if(model[i].hasOwnProperty('children')) {
        ConstructList(model[i].children, ul);
        if($('.test_failed', ul).length > 0)
          name.addClass('test_failed');                
        else
          name.addClass('test_successful');
      }
      else if (model[i].hasOwnProperty('testcases')) {
        if(model[i].success == undefined || (model[i].success))
          name.addClass('test_successful');
        else
          name.addClass('test_failed');
        ConstructTestCases(model[i], ul);
        if($('.test_failed', ul).length > 0)
          name.addClass('test_failed');                
        else
          name.addClass('test_successful');        
        
        first = false;
      }
             
      target.append(ul);
    }
  }      
}

function GetServerList() {
  var res = [];
  display_options.servers.available = [];
  display_options.servers.selected = [];
  $('#Options_Toggle').hide();
  
  // Connect database
  $.ajax({
      type: 'GET',
      url: 'get_servers.php',
      dataType: 'json',
      success: function(data) { 
      var res = data; 
      var target = $('#Servers_List');
      target.empty();
      for(var i = 0; i < res.length; ++i) {
        var div = $('<div>');
        div.addClass('TestServer');
        div.addClass("noselect");
        if(!res[i].success) {
          div.addClass('test_failed');
        }
        div.append(res[i].name);
        div.data("server_id", res[i].id);
        div.data("server_name", res[i].name);
        div.click(function(){
          SetServer($(this).data("server_id"), $(this).data("server_name"));
        });
        target.append(div);
      }
      },
  });
}

function SetServer(server_id, server_name) {
  GetTestRunList(server_id);
  $(".Selected_Server").removeClass('Selected_Server');
  $(".Selected_TestRun").removeClass('Selected_TestRun');
  $(this).addClass('Selected_Server');
  $('#Categories_List').empty();
  display_options.servers.available = [server_name];
  display_options.servers.selected = [server_name];
  refreshDisplayOptions();
}

function GetTestRunList(server_id) {
  // Connect database
  $.ajax({
      type: 'GET',
      url: 'get_test_runs.php?server_id=' + server_id,
      dataType: 'json',
      success: function(data) { 
        var res = data;
        $("#TestRuns").show();
        var target = $("#TestRuns_List");
        target.empty();
        for(var i = 0; i < res.length; ++i) {
          var div = $('<div>');
          div.addClass('TestRun');
          div.addClass("noselect");
          if(!res[i].success) {
            div.addClass('test_failed');
          }
          div.append(res[i].timestamp);
          div.data("test_run_id", res[i].id);
          if(res[i].has_results) {
            div.click(function(){
              SetTestRun($(this).data("test_run_id"));
              $(".Selected_TestRun").removeClass('Selected_TestRun');
              $(this).addClass('Selected_TestRun');
             // $('#Categories_List').empty();
            });
          } else {
            div.addClass('TestRun_NoResults');
          }
          target.append(div);
        }      
      }
  });

}

function SetTestRun(test_run_id) {
  LoadSQLModel(test_run_id, function(data) {
    $('#Categories_List').empty();
    model = data;
    refreshDisplayOptions(model);
    $('#Options_Toggle').show();
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
  
  });
}



var model = null;

$(document).ready(function(){
  GetServerList();
  
  // Set up the toggle buttons.
  $('#Options_Toggle_Categories').data('collapsed', true);
  $('#Options_Toggle_Categories').prop('value', 'Expand All Categories');
  $('#Options_Toggle_Categories').click(function(){
    if($(this).data('collapsed')) {
      $('.category_list_checkbox').prop('checked', true);
      $(this).data('collapsed', false);
      $('#Options_Toggle_Categories').prop('value', 'Collapse All Categories');
    } else {
      $('.category_list_checkbox').prop('checked', false);
      $(this).data('collapsed', true);
      $('#Options_Toggle_Categories').prop('value', 'Expand All Categories');
    }
  });
  
  
  
  $('#Options_Toggle_Cases').data('collapsed', true);
  $('#Options_Toggle_Cases').prop('value', 'Expand Visible Cases');
  $('#Options_Toggle_Cases').click(function(){
    if($(this).data('collapsed')) {
      $('.TestResult_name:visible').removeClass('minimized');
      $(this).data('collapsed', false);
      $('#Options_Toggle_Cases').prop('value', 'Collapse Visible Cases');
    } else {
      $('.TestResult_name:visible').addClass('minimized');
      $(this).data('collapsed', true);
      $('#Options_Toggle_Cases').prop('value', 'Expand Visible Cases');
    }
  });

  $('#Options_Toggle_Steps').data('collapsed', true);
  $('#Options_Toggle_Steps').prop('value', 'Expand Visible Steps');
  $('#Options_Toggle_Steps').click(function(){
    if($(this).data('collapsed')) {
      $('.TestStep_name:visible').removeClass('minimized');
      $(this).data('collapsed', false);
      $('#Options_Toggle_Steps').prop('value', 'Collapse Visible Steps');
    } else {
      $('.TestStep_name:visible').addClass('minimized');
      $(this).data('collapsed', true);
      $('#Options_Toggle_Steps').prop('value', 'Expand Visible Steps');
    }
  });
  
});    

          
