/*
 *	ServerSocket class interface
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
#ifndef ELM_NET_SERVERSOCKET_H_
#define ELM_NET_SERVERSOCKET_H_

#include <elm/io/UnixInStream.h>
#include <elm/io/UnixOutStream.h>
#include <elm/net/Exception.h>

namespace elm { namespace net {

class Connection {
public:
	Connection(int fd);
	~Connection(void);
	inline io::InStream& in(void) { return _in; }
	inline io::OutStream& out(void) { return _out; }
private:
	int _fd;
	io::UnixInStream _in;
	io::UnixOutStream _out;
};

class ServerSocket {
public:
	ServerSocket(void);
	ServerSocket(int port);
	virtual ~ServerSocket(void);
	inline int port(void) const { return _port; }
	void open(void) throw(Exception);
	void manage(void) throw(Exception);
	void close(void);
protected:
	virtual void onConnection(Connection *connection) = 0;
private:
	int _port;
	int _fd;
};

} }	// elm::net

#endif /* ELM_NET_SERVERSOCKET_H_ */