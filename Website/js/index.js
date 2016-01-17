/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
var app = {
	// Application Constructor
	initialize: function(){
		settings.init();
		this.bindEvents();
	},
	// Bind Event Listeners
	//
	// Bind any events that are required on startup. Common events are:
	// 'load', 'deviceready', 'offline', and 'online'.
	bindEvents: function(){
		document.addEventListener("deviceready", this.onDeviceReady, false);
		
		if(!navigator.userAgent.match(/(iPad)|(iPhone)|(iPod)|(android)|(webOS)/i)){ // Override if browser
			app.receivedEvent("deviceready");
		}
	},
	// deviceready Event Handler
	//
	// The scope of 'this' is the event. In order to call the 'receivedEvent'
	// function, we must explicitly call 'app.receivedEvent(...);'
	onDeviceReady: function(){
		app.receivedEvent("deviceready");
	},
	// Update DOM on a Received Event
	receivedEvent: function(id){
		if(page.init()){
			// Update DOM
			document.getElementById("content").className = "";
		}
	}
};
// User settings
var settings = {
	path: "",
	
	init: function(){	
		var path = window.location.href;
		this.path = path.substring(0, path.lastIndexOf("/"));
	}
};

// Create AJAX request
var ajax = {
	// Get AJAX response
	get: function(obj, func, link, method, async, type, text){
		var request = new XMLHttpRequest();
		request.open(method, link, async);
		//request.responseType = "text"; // Auto detection of response type because of XML injection...
		request.onreadystatechange = function(){
			if(request.readyState === 4){
				if(request.status === 200){
					obj[func](request.responseText, type); // Set data, when available
				}
			}
		};
		request.send(text);
	}
};

var page = {
	firstLoad: true,
	lastInput: "",
	selectedText: "",
	// Initialization of the body element
	init: function(){
		if(document.getElementById("body").className === ""){
			document.getElementById("body").className = "main";
		}
		
		this.get(document.getElementById("body").className);
		
		if(this.firstLoad){
			this.inject();
			this.firstLoad = false;	
		}
		
		return true;
	},
	// Get body data
	get: function(value){
		ajax.get(this, "set", settings.path+"/pages/"+value+".xml", "GET", false, "XML");
		body.className = value;
	},
	// Set body data
	set: function(response, type){
		document.getElementById("content").innerHTML = response;
	},
	// Dependency injection
	inject: function(){
		// Add Event Listeners to buttons.
		var navlinks = document.getElementById("navigation").getElementsByTagName("a");
		for(link in navlinks){
			var href = navlinks[link].href;
			if(href !== undefined){
				navlinks[link].addEventListener("click", function(e){
					page.prevent(e);
					console.log(this.getAttribute("href"));
					page.get(this.getAttribute("href"));
				});
			}
		}
	},
		
		
	// Prevent standard events in case of custom action
	prevent: function(e){
		if(e !== null){
			if(e.preventDefault){
				e.preventDefault();
			}
			else if(window.event && window.event.returnValue){
				window.event.returnValue = false;
			}
		}
	}
};

window.onload = function(){
	app.initialize(); // Start initialization of App
};