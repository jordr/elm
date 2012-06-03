/*
 *	ClientsSocket class implementation
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2012, IRIT UPS.
 *
 *	ELM is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	ELM is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with ELM; if not, write to the Free Software
 *	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <elm/io/UnixInStream.h>
#include <elm/io/UnixOutStream.h>
#include <elm/net/ClientSocket.h>
#include <elm/net/ServerSocket.h>
#include <elm/string.h>

namespace elm { namespace net {

/**
 * @class ClientSocket
 * Class implementing a client through a socket connection.
 */


/**
 * Build a client on the given port in the local host.
 * @param port	Port to connect to.
 */
ClientSocket::ClientSocket(int port)
: _host("127.0.0.1"), _port(port), _fd(-1), _in(0), _out(0) {
}


/**
 * Build a client socket connecting to the given host on the given port.
 * @param host	Host to connect to (numeric or symbolic form).
 * @param port	Port to connect to.
 */
ClientSocket::ClientSocket(const string& host, int port)
: _host(host), _port(port), _fd(-1), _in(0), _out(0) {
}


/**
 * Build a client socket connecting to the given host with the given service.
 * @param host		Host to connect to (numeric or symbolic form).
 * @param service	Service to connect to.
 */
ClientSocket::ClientSocket(const string& host, const string& service)
: _host(host), _serv(service), _fd(-1), _in(0), _out(0) {
}


/**
 * Automatically close the connection if connected.
 */
ClientSocket::~ClientSocket(void) {
	disconnect();
}


/**
 * @fn int ClientSocket::port(void) const;
 * Get the port of the client.
 * @return	Connection port.
 */


/**
 * Connect the client to the server.
 * @throw Exception		If there is an error.
 */
void ClientSocket::connect(void) throw(Exception) {
	/*int sock, bytes_recieved;
	        char send_data[1024],recv_data[1024];
	        struct hostent *host;
	        struct sockaddr_in server_addr; */

	// find the host
	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_NUMERICSERV;
	hints.ai_protocol = 0;
	struct addrinfo *info;
	string service = _serv ? _serv: (_ << _port);
	if(getaddrinfo(_host.toCString().chars(), service.toCString().chars(), &hints, &info) != 0)
		throw Exception(_ << "cannot get the host address: ");

	// build the socket
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(_fd < 0)
		throw Exception(_ << "cannot create the socket: " << strerror(errno));

	// perform the connection
	if(::connect(_fd, info->ai_addr, info->ai_addrlen) == -1) {
		disconnect();
		throw Exception(_ << "cannot connect: " << strerror(errno));
	}
}


/**
 * Disconnect from the server.
 * @throw	Exception	If there is an error.
 */
void ClientSocket::disconnect(void) throw(Exception) {
	if(_fd >= 0) {
		::close(_fd);
		_fd = -1;
		if(_in)
			delete _in;
		if(_out)
			delete _out;
	}
}


/**
 * Obtain the input channel for the connection.
 * @return	Input channel.
 * @throw	Exception	If there is an error.
 */
io::InStream& ClientSocket::in(void) throw(Exception) {
	if(_fd < 0)
		throw Exception("client not connected");
	if(!_in)
		_in = new io::UnixInStream(_fd);
	return *_in;
}


/**
 * Obtain the output channel for the connection.
 * @return	Output channel.
 * @throw	Exception	If there is an error.
 */
io::OutStream& ClientSocket::out(void) throw(Exception) {
	if(_fd < 0)
		throw Exception("client not connected");
	if(!_out)
		_out = new io::UnixOutStream(_fd);
	return *_out;
}


/**
 * @fn const string& ClientSocket::host(void) const;
 * Get the host name of the server the client has to connect to.
 * @return	Server host name.
 */


/**
 * @fn const string& ClientSocket::service(void) const;
 * If any, get the name of the service of connection.
 * @return	Connected service name.
 */

} }	// elm::net