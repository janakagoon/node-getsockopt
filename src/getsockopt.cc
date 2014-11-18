/**
 * getsockopt.cc 
 * jgoonasekera@github.com
 * 
 * Exposes a subset of socket options as a node js module. 
 * Currently implements:
 *  original_dest
 */

#include <errno.h>
#include <node.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <v8.h>

#include <linux/netfilter_ipv4.h>

using namespace v8;
/*
 * Queries the original destination from getsockopt and returns the value as:
 * "<IPv4>:<Port>:
 */
Handle<Value> original_dest(const Arguments& args) {
    HandleScope scope;

    if (args.Length() < 1) {
        ThrowException(
            Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }

    if (!args[0]->IsNumber()) {
        ThrowException(Exception::TypeError(
            String::New("Wrong argument type. Expect a number.")));
        return scope.Close(Undefined());
    }

    // Get the file descriptor from the input value.
    int fd = args[0]->Int32Value();

    struct sockaddr_in destaddr;
    socklen_t slen = sizeof(destaddr);
    memset(&destaddr, 0, slen);

    if (0 != getsockopt(fd,
        SOL_IP,
        SO_ORIGINAL_DST,
        &destaddr,
        &slen)) {
        char err_message[40];
        snprintf(err_message, sizeof(err_message),
            "getsockopt failed with errno: (%s)",
            strerror(errno));
        ThrowException(Exception::TypeError(String::New(err_message)));
        return scope.Close(Undefined());
    }

    // Build the return string using the data in sockaddr structure.
    char host_name_port[256] = {0};
    inet_ntop(AF_INET,  reinterpret_cast<void *>(&destaddr.sin_addr),
        host_name_port, 256);
    int host_port = ntohs(destaddr.sin_port);

    ssize_t dl = strlen(host_name_port);
    snprintf(&host_name_port[dl], (256-dl), ":%d", host_port);

    return scope.Close(String::New(host_name_port));
}

/*
 * Node extensions required initializer.
 */
void init(Handle<Object> exports) {
    exports->Set(String::NewSymbol("original_dest"),
        FunctionTemplate::New(original_dest)->GetFunction());
}

NODE_MODULE(getsockopt, init)
