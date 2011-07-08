/*
 *	$Id$
 *	Path class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2005-8, IRIT UPS.
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
#ifndef ELM_SYSTEM_PATH_H
#define ELM_SYSTEM_PATH_H

#include <elm/string.h>
#include <elm/io.h>

namespace elm { namespace system {

// Path class
class Path {
	String buf;
public:
#	if defined(__WIN32) || defined(__WIN64)
		static const char SEPARATOR = '\\';
		static const char PATH_SEPARATOR = ';';
		static inline bool isSeparator(char c) { return c == SEPARATOR || c == '/'; }
#	else
		static const char SEPARATOR = '/';
		static const char PATH_SEPARATOR = ':';
		static inline bool isSeparator(char c) { return c == SEPARATOR; }
#	endif
	
	// Constructors
	inline Path(void) { }
	inline Path(const char *path): buf(path) { }
	inline Path(CString path): buf(path) { }
	inline Path(const String& path): buf(path) { }
	inline Path(const Path& path): buf(path.buf) { }
	Path canonical(void) const;
	Path absolute(void) const;
	static void setCurrent(Path& path);
	Path append(Path path) const;
	Path parent(void) const;
	Path setExtension(CString new_extension) const;
	
	// Accessors
	inline const String& toString(void) const { return buf; }
	String namePart(void) const;
	String dirPart(void) const;
	Path basePart(void) const;
	String extension(void) const;
	bool isAbsolute(void) const;
	bool isRelative(void) const;
	bool isHomeRelative(void) const;
	inline bool equals(Path& path) const { return buf == path.buf; }
	inline bool contains(Path& path) const { return path.buf.startsWith(path.buf); }
	static Path current(void);
	static Path home(void);

	// path testing
	bool exists(void) const;
	bool isFile(void) const;
	bool isDir(void) const;
	bool isReadable(void) const;
	bool isWritable(void) const;
	bool isExecutable(void) const;
	/*bool operator==(const Path& path) { return buf == path.buf; }
	bool operator!=(const Path& path) { return !operator==(path); }*/
	
	// Operator
	inline Path& operator=(const char *str) { buf = str; return *this; }
	inline Path& operator=(CString str) { buf = str; return *this; }
	inline Path& operator=(const String& str) { buf = str; return *this; }
	inline Path& operator=(Path& path) { buf = path.buf; return *this; }

	inline bool operator==(Path path) const { return equals(path); }
	inline bool operator!=(Path path) const { return !equals(path); }
	inline Path operator/(const Path& path) const { return append(path); }
	inline operator const String& (void) const { return toString(); }
	inline operator bool (void) const  { return buf; }
	inline const char *operator&(void) const { return &buf; };

private:
	int nextSeparator(int start = 0) const;
};

inline io::Output& operator<<(io::Output& out, const Path& path)
	{ out << path.toString(); return out; }

}	// system

typedef elm::system::Path Path;

} // elm

#endif // ELM_SYSTEM_PATH_H
