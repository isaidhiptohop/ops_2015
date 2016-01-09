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

        // handle delete graph
        d3.select("#delete-graph").on("click", function(){
            thisGraph.deleteGraph(false);
            thisGraph.instance.delete();
        });

        // handle start simulation
        d3.select("#start-simulation").on("click", function(){

            thisGraph.state.simulationStarted = true;

            console.log(JSON.stringify(thisGraph.nodes));
            console.log(JSON.stringify(thisGraph.edges));

            var transferEdges = [];
            thisGraph.edges.forEach(function(e){
                transferEdges.push({source: e.source.id, target: e.target.id, value: "40"});
            });

            var transfer = JSON.stringify(transferEdges);

            thisGraph.instance = new Module.AntColonyJSON(thisGraph.nodes.length, transfer, 1, 0);

            // console.log('antColony result: ' + thisGraph.instance.nextStep(50));

            var result = JSON.parse(thisGraph.instance.nextStep(50));

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
        circleGClass: "conceptG",
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

    AntGraph.prototype.deleteGraph = function(skipPrompt){
        var thisGraph = this,
            doDelete = true;
        if (!skipPrompt){
            doDelete = window.confirm("Press OK to delete this graph");
        }
        if(doDelete){
            thisGraph.nodes = [];
            thisGraph.edges = [];
            thisGraph.updateGraph();
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

        thisGraph.dragLine.classed("hidden", true);

        if (mouseDownNode !== d){
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

    // mouseup on main svg
    AntGraph.prototype.svgMouseUp = function(){
        var thisGraph = this,
            state = thisGraph.state;
        if (state.justScaleTransGraph) {
            // dragged not clicked
            state.justScaleTransGraph = false;
        } else if (state.graphMouseDown && d3.event.shiftKey){
            // clicked not dragged from svg
            var xycoords = d3.mouse(thisGraph.svgG.node()),
                d = {id: thisGraph.idct++, type: 0, x: xycoords[0], y: xycoords[1]};
            thisGraph.nodes.push(d);
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
            .classed('link40', function(d){
                return d.value == 40;
            })
            .classed('link100', function(d){
                return d.value == 100;
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

        newGs.classed('root', function(d){
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



        // remove old nodes
        thisGraph.circles.exit().remove();
    };

