/*
 *	BlockInStream class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2005-09, IRIT UPS.
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
#ifndef ELM_IO_BLOCK_IN_STREAM_H
#define ELM_IO_BLOCK_IN_STREAM_H

#include <elm/string/String.h>
#include <elm/string/CString.h>
#include <elm/io/InStream.h>

namespace elm { namespace io {

// BlockInStream class
class BlockInStream: public InStream {
	const char *_block;
	int _size, off;
public:

	BlockInStream(const void *block, int size);
	BlockInStream(const char *str);
	BlockInStream(const CString& str);
	BlockInStream(const String& str); 
	inline const void *block() const { return _block; }
	inline int size() const { return _size; }
	inline int mark() const { return off; }
	inline void move(int mark) { off = mark; }
	inline void moveForward(int size) { off += size; }
	inline void moveBackward(int size) { off -= size; }
	inline void reset(void) { off = 0; }

	// InStream overload
	int read(void *buffer, int size) override;
	int read() override;
};

} } // elm::io

#endif	// ELM_IO_BLOCK_IN_STREAM_H
