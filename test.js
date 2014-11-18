/**
 * @fileoverview HTTPS whitelist proxy server.
 * @author jgoonasekera@boingo.com.
 *
 * This server will forward or redirect HTTPS requests, based on if the
 * common name in the server certificate is in the whitelist or not.
 */
"use strict";
var getsockopt = require('./src/build/Release/getsockopt');
var net = require('net');

function testGetOriginalDestination(test) {
    var socket = new net.Socket();

    socket.connect(80, 'http://www.google.com', function(err, socket){

        // TODO: We are using a private member _handle to get to the FD. This 
        // can be potentially fragile and may not work on non-linux systems, 
        // May break if the internal structure of socket changes. Get a more 
        // robust way to get to the socket FD.

        dest_addr_port = getsockopt.original_dest(socket._handle.fd)
                .split(':'),
            dest_ip = dest_addr_port[0],
            dest_port = dest_addr_port[1];


        console.log(dest_ip);
        console.log(dest_port);
        }
    );
}
