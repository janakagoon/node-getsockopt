/**
 * Repository of Unit Tests for this module.
 *
 * To run, do: nodeunit test.js
 */
"use strict";
var getsockopt = require('./src/build/Release/getsockopt');
var net = require('net');
var http = require('http');


var server = null;
var socket = null;
function setUp(done) {
    server = new http.createServer(function(req, resp) {
      	resp.end('reply');
    });  
    server.listen(8000);

    socket = new net.Socket();
    done();
}

function tearDown(done) {
    server.close();
    socket.destroy();
    done();
}

function testSimple(test) {
    test.expect(2);

    socket.connect(8000, '127.0.0.1', function(err){
	console.log(err);

        // TODO: We are using a private member _handle to get to the FD. This 
        // can be potentially fragile and may not work on non-linux systems, 
        // May break if the internal structure of socket changes. Get a more 
        // robust way to get to the socket FD.

        var dest_addr_port = getsockopt.original_dest(socket._handle.fd)
                .split(':'),
            dest_ip = dest_addr_port[0],
            dest_port = dest_addr_port[1];

        test.ok(dest_ip === '127.0.0.1', 'dest ip must match');
        test.ok(dest_port === '8000', 'dest port must match');
	test.done();
        }
    );
}

// Start of exports
exports.setUp = setUp;
exports.tearDown = tearDown;
exports.testSimple = testSimple
