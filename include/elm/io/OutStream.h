/*
 *	$Id$
 *	OutStream class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2004-07, IRIT UPS.
 * 
 *	OTAWA is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	OTAWA is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with OTAWA; if not, write to the Free Software 
 *	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef ELM_IO_OUTSTREAM_H
#define ELM_IO_OUTSTREAM_H

#include <elm/string/CString.h>

namespace elm { namespace io {

// OutStream class
class OutStream {
public:
	virtual ~OutStream(void) { };
	virtual int write(const char *buffer, int size) = 0;
	virtual int write(char byte);
	virtual int flush(void) = 0;
	virtual CString lastErrorMessage(void);
	virtual bool supportsANSI() const;
	
	static OutStream& null;
};

} }	// elm::io

#endif // ELM_IO_OUTSTREAM_H
