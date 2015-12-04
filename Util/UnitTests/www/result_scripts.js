function LoadSQLModel() {
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
  // fps_min : float
  // fps_avg : float
  // fps_max : float
  // history : an array containing history data from other runs of the same test
  //
  // The history array contains one entry for every previous run of this testcase on every test server sorted by their timestamp
  // The history object members are:
  // time : the timestamp of the testrun
  // server_id : the id of the server that ran this test
  // server_name : the name of the server that ran this test
  // fps_min : float
  // fps_avg : float
  // fps_max : float
  
  var res = []
  
  // Connect database
  $.ajax({
      type: 'GET',
      url: 'get_results.php',
      dataType: 'json',
      success: function(data) { res = data; },
      async: false
  });
  
  return res;
}
   
var all_graphs = [];
var display_options =  {
  properties: {
    available: ["fps_max"],
    selected: ["fps_avg"],
  },
  servers:  {
    available: [],
    selected: [],
  }
};

function generateDisplayOptions(model) {
  for(var i = 0; i < model.length; i++) {
    if(model[i].hasOwnProperty('children'))
      generateDisplayOptions(model[i].children); 
    else {
      for(var j = 0; j < model[i].testcases.length; j++) {
      var testcase = model[i].testcases[j];
        if($.inArray(testcase.server_name, display_options.servers.available)) {
          display_options.servers.available.push(testcase.server_name);
        }
        if(display_options.servers.selected.length == 0) {
          display_options.servers.selected.push(testcase.server_name);
        }
        for(var propertyName in testcase) {
          if($.inArray(propertyName, display_options.properties.available) == -1) {
            display_options.properties.available.push(propertyName);
          }             
        }
      }
    }               
  }
}


function generateDatasets(testcase) {
  var datasets = [];
  var h = 0;
  
  for(var propertyName in testcase) {
    if(($.inArray(propertyName, display_options.properties.selected) > -1) && ($.inArray(testcase.server_name, display_options.servers.selected) > -1)) {
      var color = "hsla("+h+", 70%, 70%";
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
      highest_y_value : 60
      };
      h = (h + 50 + Math.random()*50) % 360;
      
      var highest = 60;
      var new_dataset = [];
      
      highest = Math.max(testcase[propertyName]);
      for(var j = 0; j < testcase.history.length; j++) {
        if(($.inArray(testcase.history[j].server_name, display_options.servers.selected) > -1)) {
          config.labels.push(testcase.history[j].time);
          highest = Math.max(testcase.history[j][propertyName]);
          new_dataset.push({x: testcase.history[j].time, y: testcase.history[j][propertyName]});
        }
      }
      new_dataset.push({x: testcase.time, y: testcase[propertyName]});
      config.labels.push(testcase.time);
      config.highest_y_value = Math.max(highest, config.highest_y_value);
      config.data = new_dataset;
      datasets.push(config);
    }
   }
  return datasets;
}

function ConstructTestCases(model, target) {
  var container = $('<div>');
  container.addClass('Test_Container');
  for(var i = 0; i < model.length; i++) {
    var div = $('<div>');
    div.addClass('TestCase');
    var name_div = $('<div>');
    name_div.addClass('TestCase_name');
    name_div.append(model[i].name);
    div.append(name_div);
    
    var graph_div = $('<div>');
    graph_div.addClass('TestCase_graph');        
    
    var graph_canvas = $('<canvas>');
    graph_div.append(graph_canvas);
    
    var graph_data = generateDatasets(model[i]);
    var highest = 60;
    for(dataset in graph_data) {
      highest = Math.max(highest, dataset.highest_y_value);
    }
         
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
            for (var k=0; k<data.datasets.length; k++){
              res += "<span style=\"color:" +data.datasets[k].pointBorderColor+"\">";
              if(data.datasets[k].label){
                res += data.datasets[k].label;
              }
              res += "</span></br>";                  
            }
            res += "</p>";							
            return res;
          },
            scales: {
                xAxes: [{
                    display: true,
                    ticks : {
                      userCallback: function(value) {return value.substring(0, value.indexOf(' '));}
                    }
                }],
                yAxes: [{
                    display: true,
                    scaleLabel: {
                        show: true,
                        labelString: 'Framerate'
                    },
                    ticks : {
                      beginAtZero: true,
                      suggestedMin: 0,
                      suggestedMax: highest,
                      userCallback: function(value) {return value;}
                    }
                }]
            }			
        }
      }       
    
    // Chart.js will fail if the canvas isn't visible in the document when Chart() is called. For that reason we append it to the body, call Chart() and then detach it so we can put it in graph_div afterwards.
    $('body').append(graph_div);


    model[i].chart = new Chart(graph_canvas[0].getContext('2d'), graph_config);        
    
    graph_div.detach();
    all_graphs.push(model[i].chart);
    div.append(graph_div);       
    var data_div = $('<div>');
    data_div.addClass('TestCase_data');
    var data_list = $('<ul>');
    data_list.addClass('TestCase_data_list');
    data_list.append('Latest:');
    data_list.append('<li>Min FPS:'+model[i].fps_min+'</li>');
    data_list.append('<li>Avg FPS:'+model[i].fps_avg+'</li>');
    data_list.append('<li>Max FPS:'+model[i].fps_max+'</li>');
    var getprops = function getAllProperties(obj){
      var allProps = []
        , curr = obj
      do{
          var props = Object.getOwnPropertyNames(curr)
          props.forEach(function(prop){
              if (allProps.indexOf(prop) === -1)
                  allProps.push(prop)
          })
      }while(curr = Object.getPrototypeOf(curr))
      return allProps
    }
    console.log(getprops(model[i].chart));
    
    data_list.append(model[i].chart.generateLegend());
    data_div.append(data_list);
    div.append(data_div);
    container.append(div);
  }
  target.append(container);
}

var CategoryCount = 0;

  var first = true;
function ConstructList(model, target) {
  for (var i = 0; i < model.length; i++) {
    var ul = $('<ul>');
    ul.attr('class', 'Category_Item');
    
    var label = $('<label>');
    label.attr('class', 'noselect');       
    var glyph = $('<div>');
    glyph.attr('class', 'glyph');
    glyph.html('▶');
      
    label.append(glyph);
    label.append('<h3>'+model[i].name+'</h3>');
    label.attr('for', 'category'+CategoryCount);
    
    var input = $('<input>');
    input.attr('type', 'checkbox');
    input.attr('id', 'category'+CategoryCount);
    if(first)
      input.prop('checked', true);
    ul.append(input);
    ul.append(label);
    
    CategoryCount++;
    
    if(model[i].hasOwnProperty('children'))
      ConstructList(model[i].children, ul);
    else if (model[i].hasOwnProperty('testcases')) {
      ConstructTestCases(model[i].testcases, ul);
      first = false;
    }
           
    target.append(ul);
  }      
}

var model = LoadSQLModel();

function updateVisibleCharts() {
  //$('.TestCase:visible').each
}

$(document).ready(function(){
  $('#Options').draggable();
  model.push(model[0]);
  generateDisplayOptions(model);
  ConstructList(model, $('#Categories'));
});    

          
