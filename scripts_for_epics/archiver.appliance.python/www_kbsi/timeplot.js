// Author  : Jeong Han Lee
// email   : jhlee@ibs.re.kr

var timeplot1 = null;
var timeplot2 = null;
var timeplot3 = null;
var timeplot4 = null;

function onLoad() {

    // var color1 = new Timeplot.Color('#FFD176');
    // var color2 = new Timeplot.Color('#FFB43D');
    // var color3 = new Timeplot.Color('#EB800F');
    var color4 = new Timeplot.Color('#964D1D');
    // var color5 = new Timeplot.Color('#66463E');
    // var quietLine  = new Timeplot.Color('#82A6A2');
    // var loudLine   = new Timeplot.Color('#284452');
    // var lightFill  = new Timeplot.Color('#DEE5D1');
    var gridColor  = new Timeplot.Color('#888888');

    // var blue   = new Timeplot.Color("#455681");
    // var azur   = new Timeplot.Color("#0090FF");
    var yellow = new Timeplot.Color("#ffd700");
    // var orange = new Timeplot.Color("#BB4A1B");
    var red    = new Timeplot.Color("#ff0000");
    var green = new Timeplot.Color('#468966');
    // var lightGreen = new Timeplot.Color('#5C832F');

//   var gridColor  = new Timeplot.Color('#333333');


    var monitor1URL = "./data/rpi_dht11_tem.txt";
    var monitor2URL = "./data/rpi_dht11_hum.txt";

    var timeGeometry = new Timeplot.DefaultTimeGeometry({
        gridColor: gridColor
	, axisLabelsPlacement: "top"
	, min : 0

    });

    var geometry1 = new Timeplot.DefaultValueGeometry({
        gridColor: gridColor
	, axisLabelsPlacement: "left"
//	, min : -15
//	, max : 30
    });



    
    var eventSource1 = new Timeplot.DefaultEventSource();
    var dataSource1  = new Timeplot.ColumnSource(eventSource1,1);
    //    var avgDataSource1 = new Timeplot.Processor(dataSource1, Timeplot.Operator.average, { size:600 });

    var plotInfo1 = [
	Timeplot.createPlotInfo
	({
	    id: "temperature"
	    , dataSource: dataSource1
	    , timeGeometry: timeGeometry
	    , valueGeometry: geometry1
	    , lineColor: red
 //   	    , dotColor: red.lighten(60)
	    //	    ,fillColor: quietLine.lighten(60)
	    , showValues: true
	    , roundValues: false
	})

        // Timeplot.createPlotInfo
	// ({
        //     id: "average temperature"
	//     , dataSource: avgDataSource1
	//     , timeGeometry: timeGeometry
	//     , valueGeometry: geometry1
	//     , lineColor: color4	   
        // })
	
    ];
    
    timeplot1 = Timeplot.create(document.getElementById("timeplot1"), plotInfo1);
    timeplot1.loadText(monitor1URL, ",", eventSource1);

    var geometry2 = new Timeplot.DefaultValueGeometry({
        gridColor: gridColor
	, axisLabelsPlacement: "left"
//	, min : -15
//	, max : 30
    });


    var eventSource2 = new Timeplot.DefaultEventSource();
    var dataSource2  = new Timeplot.ColumnSource(eventSource2,1);
 //   var avgDataSource2 = new Timeplot.Processor(dataSource2, Timeplot.Operator.average, { size:600 });

    var plotInfo2 = [
    	Timeplot.createPlotInfo
    	({
    	    id: "temperature"
    	    , dataSource: dataSource2
    	    , timeGeometry: timeGeometry
    	    , valueGeometry: geometry2
	    , lineColor: green
 //   	    , dotColor: green.lighten(30)
    	    , showValues: true	    
    	    , roundValues: false

    	})

        // Timeplot.createPlotInfo
    	// ({
        //     id: "average temperature",
        //     dataSource: avgDataSource2,
    	//     timeGeometry: timeGeometry,
        //     valueGeometry: geometry1,
        //     lineColor: red.darken(100)
    	//     //            dotColor: blue,
        // })
	
    ];
    
    timeplot2 = Timeplot.create(document.getElementById("timeplot2"), plotInfo2);
    timeplot2.loadText(monitor2URL, ",", eventSource2);


}





var resizeTimerID = null;
function onResize() {
    if (resizeTimerID == null) {
        resizeTimerID = window.setTimeout(function() {
            resizeTimerID = null;
            if (timeplot1) timeplot1.repaint();
            if (timeplot2) timeplot2.repaint();
        }, 0);
    }
}