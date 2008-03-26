/*
 * $Id$
 * Copyright (c) 2007 IRIT-UPS <casse@irit.fr>
 * 
 * checksum::Fletcher class interface
 */ 
#ifndef ELM_CHECKSUM_FLETCHER_H
#define ELM_CHECKSUM_FLETCHER_H

#include <elm/io/InStream.h>

namespace elm { namespace checksum {

// Fletcher class
class Fletcher {
	unsigned long sum1, sum2;
	unsigned long len;
	char half[2];
	unsigned short size;
	inline void shuffle(void);
	inline void add(void);
public:
	Fletcher(void);
	unsigned long sum(void);
	void put(io::InStream& in);
	void put(io::InStream& in, int length);
	void put(const void *buffer, int length);
	void put(const CString& str);
	void put(const String& str);
	inline Fletcher& operator<<(const char *str)
		{ put(CString(str)); return *this; }
	inline Fletcher& operator<<(const CString& str)
		{ put(str); return *this; }
	inline Fletcher& operator<<(const String& str)
		{ put(str); return *this; }
	template <class T> inline Fletcher& operator<<(const T& value)
		{ put(&value, sizeof(T)); return *this; }
};

} } // elm::checksum

#endif // ELM_CHECKSUM_FLETCHER_H
 
