/*
 *	BufferedOutStream class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2009, IRIT UPS. *	$Id$
 *
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
#ifndef ELM_IO_BUFFERED_OUT_STREAM_H_
#define ELM_IO_BUFFERED_OUT_STREAM_H_

#include <elm/io.h>
#include <elm/io/OutStream.h>

namespace elm { namespace io {

// BufferedOutStream class
class BufferedOutStream: public OutStream {
public:
	static const int default_size = 4096;

	BufferedOutStream(OutStream& output, size_t size = default_size);
	BufferedOutStream(OutStream *output, bool close = false, size_t size = default_size);
	~BufferedOutStream() override;

	void reset();
	void setStream(OutStream& str);
	inline OutStream& stream() const { return *out; }

	int write(const char *buffer, int size) override;
	int write(char byte) override;
	int flush(void) override;
	CString lastErrorMessage(void) override;
	bool supportsANSI() const override;

private:
	OutStream *out;
	char *buf;
	size_t top, buf_size;
	bool _close;
};

} } // elm::io

#endif /* ELM_IO_BUFFERED_OUT_STREAM_H_ */
