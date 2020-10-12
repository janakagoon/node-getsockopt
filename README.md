A module that exposes Unix getsockopt's 'get original destination' functionality in node.js.

To build, type:

1. npm install
2. make

To test, type:

1. nodeunit test.js

This module was created because the existing node getsockopt modules do not have the ability to return the original destination from getsockopt. 

This module can be easily extended to support additional properties. 

Add something

[TEST-1]
