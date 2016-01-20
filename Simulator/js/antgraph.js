/**
 * Created by sebastiankoch on 07/01/16.
 *
 * This program uses a modified version of the graph-creator by Colorado Reed (https://github.com/metacademy/directed-graph-creator)
 */

    // define AntGraph object
    var AntGraph = function(svg, nodes, edges){
        var thisGraph = this;
        thisGraph.idct = 0;

        thisGraph.nodes = nodes || [];
        thisGraph.edges = edges || [];

        thisGraph.state = {
            selectedNode: null,
            selectedEdge: null,
            mouseDownNode: null,
            mouseDownLink: null,
            justDragged: false,
            justScaleTransGraph: false,
            lastKeyDown: -1,
            shiftNodeDrag: false,
            selectedText: null,
            simulationStarted: false
        };

        thisGraph.svg = svg;
        thisGraph.svgG = svg.append("g")
            .classed(thisGraph.consts.graphClass, true);
        var svgG = thisGraph.svgG;

        // displayed when dragging between nodes
        thisGraph.dragLine = svgG.append('svg:path')
            .attr('class', 'link dragline hidden')
            .attr('d', 'M0,0L0,0');

        // svg nodes and edges
        thisGraph.paths = svgG.append("g").selectAll("g");
        thisGraph.circles = svgG.append("g").selectAll("g");


        thisGraph.drag = d3.behavior.drag()
            .origin(function(d){
                return {x: d.x, y: d.y};
            })
            .on("drag", function(args){
                thisGraph.state.justDragged = true;
                thisGraph.dragmove.call(thisGraph, args);
            });

        // listen for key events
        d3.select(window).on("keydown", function(){
                thisGraph.svgKeyDown.call(thisGraph);
            })
            .on("keyup", function(){
                thisGraph.svgKeyUp.call(thisGraph);
            });
        svg.on("mousedown", function(d){thisGraph.svgMouseDown.call(thisGraph, d);});
        svg.on("mouseup", function(d){thisGraph.svgMouseUp.call(thisGraph, d);});

        // handle start simulation
        d3.select("#start-simulation").on("click", function(){

            thisGraph.state.simulationStarted = true;

            // console.log("Node: " + JSON.stringify(thisGraph.nodes));
            console.log("Start Edges: " + JSON.stringify(thisGraph.edges));

/**
            console.log("antNumber : " + antNumber);
            console.log("updateFactor : " + updateFactor);
            console.log("delayFactor : " + delayFactor);
            console.log("PheromonWeight : " + PheromonWeight);
            console.log("CostWeight : " + CostWeight);
            console.log("stepsNum : " + stepsNum);
*/
            var transferEdges = [];
            thisGraph.edges.forEach(function(e){
                transferEdges.push({source: e.source.id, target: e.target.id, value: "40"});
            });

            var transfer = JSON.stringify(transferEdges);

            thisGraph.instance = new Module.AntColonyJSON(thisGraph.nodes.length, transfer, 0, 1, Number(antNumber), Number(updateFactor), Number(delayFactor), Number(PheromonWeight), Number(CostWeight));

            var result = JSON.parse(thisGraph.instance.nextStep(Number(stepsNum)));

            // console.log(JSON.stringify(result));

            for(var i = 0; i < thisGraph.edges.length; i++) {
                thisGraph.edges[i].value = result[i].value;
            }

            console.log("New Edges: " + JSON.stringify(thisGraph.edges));

            d3.select("#next-step").attr('disabled', null);

            thisGraph.updateGraph();
        });

        // handle delete graph
        d3.select("#next-step").on("click", function(){

            var result = JSON.parse(thisGraph.instance.nextStep(Number(stepsNum)));

            console.log(JSON.stringify(result));

            for(var i = 0; i < thisGraph.edges.length; i++) {
                thisGraph.edges[i].value = result[i].value;
            }

            console.log(JSON.stringify(thisGraph.edges));

            thisGraph.updateGraph();

        });
    };

    AntGraph.prototype.setIdCt = function(idct){
        this.idct = idct;
    };

    AntGraph.prototype.consts =  {

        selectedClass: "selected",
        connectClass: "connect-node",
        circleGClass: "node",
        graphClass: "graph",
        activeEditId: "active-editing",
        BACKSPACE_KEY: 8,
        DELETE_KEY: 46,
        ENTER_KEY: 13,
        nodeRadius: 10
    };

    /* PROTOTYPE FUNCTIONS */

    AntGraph.prototype.dragmove = function(d) {
        var thisGraph = this;
        if (thisGraph.state.shiftNodeDrag){
            d.x += d3.event.dx;
            d.y +=  d3.event.dy;
            thisGraph.updateGraph();
        } else{
            thisGraph.dragLine.attr('d', 'M' + d.x + ',' + d.y + 'L' + d3.mouse(thisGraph.svgG.node())[0] + ',' + d3.mouse(this.svgG.node())[1]);
        }
    };

    // remove edges associated with a node
    AntGraph.prototype.spliceLinksForNode = function(node) {
        var thisGraph = this,
            toSplice = thisGraph.edges.filter(function(l) {
                return (l.source === node || l.target === node);
            });
        toSplice.map(function(l) {
            thisGraph.edges.splice(thisGraph.edges.indexOf(l), 1);
        });
    };

    AntGraph.prototype.replaceSelectEdge = function(d3Path, edgeData){
        var thisGraph = this;
        d3Path.classed(thisGraph.consts.selectedClass, true);
        if (thisGraph.state.selectedEdge){
            thisGraph.removeSelectFromEdge();
        }
        thisGraph.state.selectedEdge = edgeData;
    };

    AntGraph.prototype.replaceSelectNode = function(d3Node, nodeData){
        var thisGraph = this;
        d3Node.classed(this.consts.selectedClass, true);
        if (thisGraph.state.selectedNode){
            thisGraph.removeSelectFromNode();
        }
        thisGraph.state.selectedNode = nodeData;
    };

    AntGraph.prototype.removeSelectFromNode = function(){
        var thisGraph = this;
        thisGraph.circles.filter(function(cd){
            return cd.id === thisGraph.state.selectedNode.id;
        }).classed(thisGraph.consts.selectedClass, false);
        thisGraph.state.selectedNode = null;
    };

    AntGraph.prototype.removeSelectFromEdge = function(){
        var thisGraph = this;
        thisGraph.paths.filter(function(cd){
            return cd === thisGraph.state.selectedEdge;
        }).classed(thisGraph.consts.selectedClass, false);
        thisGraph.state.selectedEdge = null;
    };

    AntGraph.prototype.pathMouseDown = function(d3path, d){
        var thisGraph = this,
            state = thisGraph.state;
        d3.event.stopPropagation();
        state.mouseDownLink = d;

        if (state.selectedNode){
            thisGraph.removeSelectFromNode();
        }

        var prevEdge = state.selectedEdge;
        if (!prevEdge || prevEdge !== d){
            thisGraph.replaceSelectEdge(d3path, d);
        } else{
            thisGraph.removeSelectFromEdge();
        }
    };

    // mousedown on node
    AntGraph.prototype.circleMouseDown = function(d3node, d){
        var thisGraph = this,
            state = thisGraph.state;
        d3.event.stopPropagation();
        state.mouseDownNode = d;
        if (!d3.event.shiftKey){
            // reposition dragged directed edge
            thisGraph.dragLine.classed('hidden', false)
                .attr('d', 'M' + d.x + ',' + d.y + 'L' + d.x + ',' + d.y);
        } else {
            state.shiftNodeDrag = d3.event.shiftKey;
        }
    };

    // mouseup on nodes
    AntGraph.prototype.circleMouseUp = function(d3node, d){
        var thisGraph = this,
            state = thisGraph.state,
            consts = thisGraph.consts;
        // reset the states
        state.shiftNodeDrag = false;
        d3node.classed(consts.connectClass, false);

        var mouseDownNode = state.mouseDownNode;

        if (!mouseDownNode) return;



        if (mouseDownNode !== d){

            // Create new edge



            // we're in a different node: create new edge for mousedown edge and add to graph
            var newEdge = {source: mouseDownNode, target: d, value: "40"};
            var filtRes = thisGraph.paths.filter(function(d){
                if (d.source === newEdge.target && d.target === newEdge.source){
                    thisGraph.edges.splice(thisGraph.edges.indexOf(d), 1);
                }
                return d.source === newEdge.source && d.target === newEdge.target;
            });
            if (!filtRes[0].length){
                thisGraph.edges.push(newEdge);
                thisGraph.updateGraph();

                thisGraph.dragLine.classed("hidden", true);
            }
        } else{
            // we're in the same node
            if (state.justDragged) {
                // dragged, not clicked
                state.justDragged = false;
            } else{
                // clicked, not dragged

                    if (state.selectedEdge){
                        thisGraph.removeSelectFromEdge();
                    }
                    var prevNode = state.selectedNode;

                    if (!prevNode || prevNode.id !== d.id){
                        thisGraph.replaceSelectNode(d3node, d);
                    } else{
                        thisGraph.removeSelectFromNode();
                    }

            }
        }
        state.mouseDownNode = null;

    }; // end of circles mouseup

    // mousedown on main svg
    AntGraph.prototype.svgMouseDown = function(){
        this.state.graphMouseDown = true;
    };

    // Create node ?
    AntGraph.prototype.svgMouseUp = function(){
        var thisGraph = this,
            state = thisGraph.state;
        if (state.justScaleTransGraph) {
            // dragged not clicked
            state.justScaleTransGraph = false;
        } else if (state.graphMouseDown && d3.event.shiftKey){

            // Create new node

            var xycoords = d3.mouse(thisGraph.svgG.node());
            var d = {id: thisGraph.idct++, type: 0, x: xycoords[0], y: xycoords[1], label: ""};
            thisGraph.nodes.push(d);

            d3.select("#next-step").attr('disabled', 'disabled');

            thisGraph.updateGraph();
            // make title of text immediently editable
            /**
            var d3txt = thisGraph.changeTextOfNode(thisGraph.circles.filter(function(dval){
                    return dval.id === d.id;
                }), d),
                txtNode = d3txt.node();
            thisGraph.selectElementContents(txtNode);
            txtNode.focus(); */
        } else {
            // dragged from node
            state.shiftNodeDrag = false;
            thisGraph.dragLine.classed("hidden", true);
        }
        state.graphMouseDown = false;
    };

    // keydown on main svg
    AntGraph.prototype.svgKeyDown = function() {
        var thisGraph = this,
            state = thisGraph.state,
            consts = thisGraph.consts;
        // make sure repeated key presses don't register for each keydown
        if(state.lastKeyDown !== -1) return;

        state.lastKeyDown = d3.event.keyCode;
        var selectedNode = state.selectedNode,
            selectedEdge = state.selectedEdge;

        switch(d3.event.keyCode) {
            case consts.BACKSPACE_KEY:
            case consts.DELETE_KEY:
                d3.event.preventDefault();

                thisGraph.dragLine.classed("hidden", true);

                if (selectedNode){
                    thisGraph.nodes.splice(thisGraph.nodes.indexOf(selectedNode), 1);
                    thisGraph.spliceLinksForNode(selectedNode);
                    state.selectedNode = null;
                    thisGraph.updateGraph();
                } else if (selectedEdge){
                    thisGraph.edges.splice(thisGraph.edges.indexOf(selectedEdge), 1);
                    state.selectedEdge = null;
                    thisGraph.updateGraph();
                }
                break;
        }
    };

    AntGraph.prototype.svgKeyUp = function() {
        this.state.lastKeyDown = -1;
    };

    // call to propagate changes to graph
    AntGraph.prototype.updateGraph = function(){

        var thisGraph = this,
            consts = thisGraph.consts,
            state = thisGraph.state;

        thisGraph.paths = thisGraph.paths.data(thisGraph.edges, function(d){
            return String(d.source.id) + "+" + String(d.target.id);
        });



        var paths = thisGraph.paths;
        // update existing paths
        paths.classed(consts.selectedClass, function(d){
                return d === state.selectedEdge;
            })
            .classed('link0', function(d){
                return d.value == 0;
            })
            .classed('link1', function(d){
                return d.value == 1;
            })
            .classed('link2', function(d){
                return d.value == 2;
            })
            .classed('link3', function(d){
                return d.value == 3;
            })
            .classed('link4', function(d){
                return d.value == 4;
            })
            .classed('link5', function(d){
                return d.value == 5;
            })
            .classed('link6', function(d){
                return d.value == 6;
            })
            .classed('link7', function(d){
                return d.value == 7;
            })
            .classed('link8', function(d){
                return d.value == 8;
            })
            .classed('link9', function(d){
                return d.value == 9;
            })
            .classed('link10', function(d){
                return d.value == 10;
            })
            .classed('link11', function(d){
                return d.value == 11;
            })
            .classed('link12', function(d){
                return d.value == 12;
            })
            .classed('link13', function(d){
                return d.value == 13;
            })
            .classed('link14', function(d){
                return d.value == 14;
            })
            .classed('link15', function(d){
                return d.value == 15;
            })
            .classed('link16', function(d){
                return d.value == 16;
            })
            .classed('link17', function(d){
                return d.value == 17;
            })
            .classed('link18', function(d){
                return d.value == 18;
            })
            .classed('link19', function(d){
                return d.value == 19;
            })
            .classed('link20', function(d){
                return d.value == 20;
            })
            .classed('link21', function(d){
                return d.value == 21;
            })
            .classed('link22', function(d){
                return d.value == 22;
            })
            .classed('link23', function(d){
                return d.value == 23;
            })
            .classed('link24', function(d){
                return d.value == 24;
            })
            .classed('link25', function(d){
                return d.value == 25;
            })
            .classed('link26', function(d){
                return d.value == 26;
            })
            .classed('link27', function(d){
                return d.value == 27;
            })
            .classed('link28', function(d){
                return d.value == 28;
            })
            .classed('link29', function(d){
                return d.value == 29;
            })
            .classed('link30', function(d){
                return d.value == 30;
            })
            .classed('link31', function(d){
                return d.value == 31;
            })
            .classed('link32', function(d){
                return d.value == 32;
            })
            .classed('link33', function(d){
                return d.value == 33;
            })
            .classed('link34', function(d){
                return d.value == 34;
            })
            .classed('link35', function(d){
                return d.value == 35;
            })
            .classed('link36', function(d){
                return d.value == 36;
            })
            .classed('link37', function(d){
                return d.value == 37;
            })
            .classed('link38', function(d){
                return d.value == 38;
            })
            .classed('link39', function(d){
                return d.value == 39;
            })
            .classed('link40', function(d){
                return d.value == 40;
            })
            .classed('link41', function(d){
                return d.value == 41;
            })
            .classed('link42', function(d){
                return d.value == 42;
            })
            .classed('link43', function(d){
                return d.value == 43;
            })
            .classed('link44', function(d){
                return d.value == 44;
            })
            .classed('link45', function(d){
                return d.value == 45;
            })
            .classed('link46', function(d){
                return d.value == 46;
            })
            .classed('link47', function(d){
                return d.value == 47;
            })
            .classed('link48', function(d){
                return d.value == 48;
            })
            .classed('link49', function(d){
                return d.value == 49;
            })
            .classed('link50', function(d){
                return d.value == 50;
            })
            .classed('link51', function(d){
                return d.value == 51;
            })
            .classed('link52', function(d){
                return d.value == 52;
            })
            .classed('link53', function(d){
                return d.value == 53;
            })
            .classed('link54', function(d){
                return d.value == 54;
            })
            .classed('link55', function(d){
                return d.value == 55;
            })
            .classed('link56', function(d){
                return d.value == 56;
            })
            .classed('link57', function(d){
                return d.value == 57;
            })
            .classed('link58', function(d){
                return d.value == 58;
            })
            .classed('link59', function(d){
                return d.value == 59;
            })
            .classed('link60', function(d){
                return d.value == 60;
            })
            .classed('link61', function(d){
                return d.value == 61;
            })
            .classed('link62', function(d){
                return d.value == 62;
            })
            .classed('link63', function(d){
                return d.value == 63;
            })
            .classed('link64', function(d){
                return d.value == 64;
            })
            .classed('link65', function(d){
                return d.value == 65;
            })
            .classed('link66', function(d){
                return d.value == 66;
            })
            .classed('link67', function(d){
                return d.value == 67;
            })
            .classed('link68', function(d){
                return d.value == 68;
            })
            .classed('link69', function(d){
                return d.value == 69;
            })
            .classed('link70', function(d){
                return d.value == 70;
            })
            .classed('link71', function(d){
                return d.value == 71;
            })
            .classed('link72', function(d){
                return d.value == 72;
            })
            .classed('link73', function(d){
                return d.value == 73;
            })
            .classed('link74', function(d){
                return d.value == 74;
            })
            .classed('link75', function(d){
                return d.value == 75;
            })
            .classed('link76', function(d){
                return d.value == 76;
            })
            .classed('link77', function(d){
                return d.value == 77;
            })
            .classed('link78', function(d){
                return d.value == 78;
            })
            .classed('link79', function(d){
                return d.value == 79;
            })
            .classed('link80', function(d){
                return d.value == 80;
            })
            .classed('link81', function(d){
                return d.value == 81;
            })
            .classed('link82', function(d){
                return d.value == 82;
            })
            .classed('link83', function(d){
                return d.value == 83;
            })
            .classed('link84', function(d){
                return d.value == 84;
            })
            .classed('link85', function(d){
                return d.value == 85;
            })
            .classed('link86', function(d){
                return d.value == 86;
            })
            .attr("d", function(d){
                return "M" + d.source.x + "," + d.source.y + "L" + d.target.x + "," + d.target.y;
            });

        // add new paths
        paths.enter()
            .append("path")
            .classed("link", true)
            .classed('link40', function(d){
                return d.value == 40;
            })
            .classed('link100', function(d){
                return d.value == 100;
            })
            .attr("d", function(d){
                return "M" + d.source.x + "," + d.source.y + "L" + d.target.x + "," + d.target.y;
            })
            .on("mousedown", function(d){
                    thisGraph.pathMouseDown.call(thisGraph, d3.select(this), d);
                }
            )
            .on("mouseup", function(d){
                state.mouseDownLink = null;
            });

        // remove old links
        paths.exit().remove();

        // update existing nodes
        thisGraph.circles = thisGraph.circles.data(thisGraph.nodes, function(d){ return d.id;});
        thisGraph.circles.attr("transform", function(d){return "translate(" + d.x + "," + d.y + ")";});


        // add new nodes
        var newGs = thisGraph.circles.enter()
            .append("g");

        newGs.classed('source', function(d){
                return d.type == 1;
            })
            .classed('target', function(d){
                return d.type == 2;
            })
            .classed(consts.circleGClass, function(d){
                return d.type != 1 && d.type != 2;
            })
            .attr("transform", function(d){return "translate(" + d.x + "," + d.y + ")";})
            .on("mouseover", function(d){
                if (state.shiftNodeDrag){
                    d3.select(this).classed(consts.connectClass, true);
                }
            })
            .on("mouseout", function(d){
                d3.select(this).classed(consts.connectClass, false);
            })
            .on("mousedown", function(d){
                thisGraph.circleMouseDown.call(thisGraph, d3.select(this), d);
            })
            .on("mouseup", function(d){
                thisGraph.circleMouseUp.call(thisGraph, d3.select(this), d);
            })
            .call(thisGraph.drag);

        newGs.append("circle")
            .attr("r", String(consts.nodeRadius));

        /* Create the text for each block */
        newGs.append("text")
            .attr("dx", function(d){return -5})
            .attr("dy", function(d){return 5})
            .text(function(d){return d.label});



        // remove old nodes
        thisGraph.circles.exit().remove();
    };


    var graph;

            function reset() {

                var width = 600;
                var height = 550;

                d3.select("svg").remove();

                var svg = d3.select("#simulator").append("svg")
                        .attr("id", "chart")
                        .attr("width", width)
                        .attr("height", height);

                var xLoc = width/2,
                        yLoc = 50;

                // initial node data
                var nodes = [{id: 0, type: 2, x: xLoc, y: yLoc, label: "A"},
                {id: 1, type: 1, x: xLoc, y: yLoc+450, label: "Z"}];
                var edges = [];


                graph = new AntGraph(svg, nodes, edges);
                graph.setIdCt(2);
                graph.updateGraph();


                d3.select("#next-step").attr('disabled', 'disabled');
            }


            var docEl = document.documentElement,
                    bodyEl = document.getElementsByTagName('body')[0];

            var width = 600;
            var height = 550;

            var xLoc = width/2,
                    yLoc = 50;

            // initial node data
            var nodes = [{id: 0, type: 2, x: xLoc, y: yLoc, label: "A"},
                {id: 1, type: 1, x: xLoc, y: yLoc+450, label: "Z"}];
            var edges = [];


            /** MAIN SVG **/
            var svg = d3.select("#simulator").append("svg")
                    .attr("id", "chart")
                    .attr("width", width)
                    .attr("height", height);
            graph = new AntGraph(svg, nodes, edges);
            graph.setIdCt(2);
            graph.updateGraph();

