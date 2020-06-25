/*
 *	IOException class implementation
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

#include <string.h>
#include <elm/io/io.h>

namespace elm { namespace io {

/**
 * @defgroup ios Input / Output System
 * 
 * The input/output of ELM is a merge of the C++ standard library system and of
 * the Java standard library. From the former, it uses the "<<" and ">>"
 * operators overload. From the latter, it implements the two IO access:
 * low-level classes manage streams of bytes and the high-level provides
 * formatting facilities.
 * 
 * @section high_level High-Level Input / Output
 * 
 * This level provides formatted input/output as in C++ standard library, that
 * is, through the "<<" and ">>" operators. The formatted output class is
 * @ref Output and the formatted input class is @ref Input.
 * 
 * Three global variables give access to the standard input/output:
 * @li @ref elm::cin -- standard input,
 * @li @ref elm::cout -- standard output,
 * @li @ref elm::cerr -- standard error.
 * 
 * Most basic types are supported (including @ref String and @ref CString).
 * Some constants are provided for special output:
 * @li @ref elm::io::endl -- new line.
 * 
 * Unlike the C++ standard library, the output format is managed through special
 * formatting objects. For integer, the work is performed by @ref IntFormat
 * that provides display features like representation base, field width,
 * alignment, uppercase letters for base greater than 10, signess, padding.
 * For float values, it exists the class @ref FloatFormat and for strings,
 * @ref StringFormat can be used.
 *
 * This format objects may easily be built using @ref io::fmt() primitives
 * and using internal constructors as in the example below:
 * @code
 * t::uint32 x;
 * cout << fmt(x).width(8).hex().right().width(8).pad('0');
 * @endcode
 *
 * A format can be built and used latter:
 * @code
 * IntFormat address_fmt = IntFormat().width(8).hex().right().width(8).pad('0');
 *
 * cout << address_fmt(x);
 * @endcode
 *
 *
 * The errors are managed here using exception objects derived from the
 * @ref IOException class.
 *
 * As with streams of the STL, the operators "<<" and ">>" can be overload to
 * be adapted to custom types:
 * @code
 * class T { ... };
 * io::Output& operator<<(io::Output& out, const T& v) { ...; return out; }
 * io::Input& operator>>(io::Input& out, T& v) { ...; return in; }
 * @endcode
 *
 * To group together input, output and error stream, the @ref Monitor class
 * provides the base of a configurable monitoring system with error and warning
 * display support.
 *
 * @section int_format Formatting
 *
 * The class @ref IntFormat embeds the value of an integer and
 * its format configuration. These objects are usually passed to @ref Output object
 * and is built from a list of usual inline functions (described below):
 *
 * @code
 * void *p;
 * cout << pointer(p) << io::endl;
 * @endcode
 *
 * The basic to display a formatted integer is to use a constructor inline
 * function as io::fmt(). This builds an @ref IntFormat object to display
 * the passed integer. The format can then be modified by calling specific
 * functions as listed below:
 * @li IntFormat::base() -- select a specific base,
 * @li IntFormat::bin() -- binary base,
 * @li IntFormat::dec() -- decimal base,
 * @li IntFormat::hex() -- hexadecimal base,
 * @li IntFormat::width() -- field with (in characters),
 * @li IntFormat::align() -- alignment (one of io::LEFT, io::CENTER or io::RIGHT),
 * @li IntFormat::left() -- left alignment,
 * @li IntFormat::center() -- centered alignment,
 * @li IntFormat::right() -- right alignment,
 * @li IntFormat::pad() -- padding character (if the number does not occupy the full width),
 * @li IntFormat::uppercase() -- uppercase character for digits greater than 10,
 * @li IntFormat::lowercase() -- lowercase character for digits greater than 10.
 * 
 * One way to implement the pointer of the above example is to define an inline function as this:
 * @code
 * inline IntFormat pointer(void *p) {
 * 	return io::f(t::uint64(p)).width(16).pad('0').right().hex();
 * }
 * @endcode
 *
 * Notice the function io::f() that is a shortcut to create an @ref IntFormat object.
 *
 * Another way to define this format is to declare an IntFormat variable and to benefit
 * from the overlaod of operator() of this class to pass the actual value to the format:
 * @code
 * 	IntFormat pointer = IntFormat().width(16).pad('0').right().hex();
 * @endcode
 *
 * A last facility to specialize the way a data is formatted is the use of @ref Tag
 * class. This template class takes a class as parameter that will be called
 * to perform the actual display. Using tags, it is possible to specialize
 * the display of particular object inside the usual flow of "<<" operators.
 * The example below displays a string escaping the special characters "<", ">"
 * and "&" in order, for example, to output HTML text:
 * @code
 * class Escape {
 * public:
 * 		typedef string t;
 * 		static void print(io::Output& out, t s) {
 * 			for(int i = 0; i < s.length(); i++)
 * 				switch(s[i]) {
 * 				case '>':	out << "&gt;"; break;
 * 				case '<':	out << "&lt;"; break;
 * 				case '&':	out << "&amp;"; break;
 * 				default:	out << s[i]; break;
 * 				}
 * 		}
 * };
 *
 * string my_string;
 * cout << Tag<Escape>(my_string) << io::endl;
 * @endcode
 * 
 * ELM proposes some predefined formats:
 * @li @ref pointer() -- hexadecimal representation of a pointer on a fixed size.
 * @li @ref byte() -- hexadecimal representation of a byte on a fixed size.
 *
 * @section ansi ANSI Codes
 * On OS's supporting them (Linux and MacOSX), ANSI codes allows to change the look of
 * displayed text. ANSI codes are supported by @ref io::Output class and can be used
 * if the header file <elm/io/ansi.h> is include. The look change is implemented by
 * sending special values to the output.
 *
 * The text color can be changed using io::BLACK, io::RED, io::GREEN, io::YELLOW,
 * io::BLUE, io::MAGENTA, io::CYAN or io::WHITE. Bright colors equivalent colors
 * exists and are name by prefixing the color with "BRIGHT_": io::BRIGHT_XXX.
 * Background color can also be customize by prefixing the color name with "BACK_":
 * io::BACK_XXX.
 *
 * Style of the characters can also be changed by displaying special values as
 * io::BOLD, io::ITALIC, io::UNDERLINE, io::BLINK, ... To reset the display of
 * the characters, one can use the value io::PLAIN.
 *
 * @section low_level Byte Streams
 * 
 * The low-level IO system is only responsible for exchanging streams of bytes.
 * 
 * All input streams must inherit from the @ref InStream class and defines the
 * following functions:
 * @li @ref InStream::read(void *buffer, int size) -- read some bytes into the given buffer,
 * @li @ref InStream::read(void) -- read one byte.
 * ELM provides byte input streams for files (@ref InFileStream), for memory
 * blocks (@ref BlockInStream) or system pipes (@ref PipeInStream).
 * 
 * The errors are returned by the called functions. Either a positive value,
 * or a value of @ref InStream::FAILED (error on the media), or
 * @ref InStream::ENDED (end of the media reached). Information about the
 * errors may be obtained by the @ref InStream::lastErrorMessage() method.
 * 
 * All output streams must inherit from the @ref OutStream class and defines the
 * following functions:
 * @li @ref OutStream::write(const char *buffer, int size) -- write some bytes from the given buffer,
 * @li @ref OutStream::write(char byte) -- write one byte,
 * @li @ref OutStream::flush() -- ensures that all written bytes have been transferred to
 * the media.
 * ELM provides byte output streams for files (@ref OutFileStream), for memory
 * blocks (@ref BlockOutStream) or system pipes (@ref PipeOutStream).
 * 
 * The errors are returned as negative value by this functions. Information about the errors may be obtained by the
 * @ref OutStream::lastErrorMessage() method.
 *
 * Byte-stream level of IO provides also some convenient classes:
 * @li @ref TeeOutStream -- duplicate an output to two other outputs (convenient for checksumming and writing to a file).
 * @li @ref BufferedOutStream -- buffered output.
 * @li @ref BufferedInStream -- buffered input.
 * @li @ref RandomAccessStream --both input and output stream with free move on the media.
 * @li @ref StreamPipe -- connect an in stream to an out stream and perform the transmission to the end.
 *
 * @section io_helper Helper Classes and Functions
 *
 * Some classes provides shortcut to perform input / output on files or on strings:
 *	* @ref StringInput -- formatted input from a string,
 *	* @ref FileInput -- formatted input from a file,
 *	* @ref StringOutput -- formatted output to a string,
 *	* @ref FileOutput -- formatted output to a file.
 *
 * @subsection io_help_tag Tag Class
 *
 * The class io::Tag can be used to display an object in a different as usual
 * (that is not using the default overload of operator <<). This class takes
 * a class P as generic parameter that must implement a static print function.
 * It will be called with, as parameters, the output to use and the object
 * to display.
 *
 * The example below display integers in a special way, surrounded by "!".
 * The display is handled by the class SpecInt. A tag for SpecInt is defined
 * using a typedef and the integer is display in this special way using the
 * tag type SPEC_INT.
 *
 * @code
 *	class SpecInt {
 *		public:
 *		typedef int t;
 *		static void print(io::Output& out, int i)
 *			{ out << "!" << i << "!"; }
 *	};
 *
 *	typedef io::Tag<SpecInt> SPEC_INT;
 *	cout << SPEC_INT(666);
 * @endcode
 *
 *
 * @subsection io_helper_printable Printable Class
 *
 * The class io::Printable and the function io::p() allows to perform a
 * display requiring a third-party object (often called a manager). This
 * means that the third-party object must define a function named print()
 * and taking as parameter the object to display and the output.
 *
 * In the example below, there is a special manager named IntManager that
 * provides some integers as text. The print() function of this class
 * is invoked using the function @ref io::p().
 *
 * @code
 *	class IntManager {
 *	public:
 *		static cstring t[4];
 *		void print(int i, io::Output& out) const {
 *			if(i >= 4)
 *				out << "more";
 *			else
 *				out << t[i];
 *		}
 *	};
 *	cstring IntManager::t[4] = { "zero", "one", "two", "three" };
 *	IntManager man;
 *	buf << io::p(2, man);
 * @endcode
 *
 * @subsection io_helper_list_printer List Printer
 *
 * The list printer can be used to display easily and conveniently a list of
 * values inside an output << operator sequence. The class ListPrinter records
 * the list and the separator. The display of the resulting object can be
 * displayed directly using operator <<. A faster way to do this is to use
 * the function io::list() as in the example below:
 * @code
 *	Vector<cstring> names;
 *	names.add("Albert");
 *	names.add("Ben");
 *	names.add("Cindy");
 *	cout << "Names: " << io::list(names, ", ") << io::endl;
 * @endcode
 *
 * The output will be:
 * @code
 * names: Albert, Ben, Cindy
 * @endcode
 */


/**
 * @class IOException
 * This exception is thrown when an IO error occurs.
 * @ingroup ios
 */


/**
 *@fn  IOException::IOException(CString message, ...);
 * Build a new IOException with the given message.
 * @param message	Message formatted a-la printf.
 * @param ...		Arguments used in the message.
 */


/**
 *@fn  IOException::IOException(CString message, VarArg& args);
 * Build a new IOException with the given message.
 * @param message	Message formatted a-la printf.
 * @param args		Arguments used in the message.
 */


/**
 * @fn StringInput read(const char *s);
 * Shortcut to perform an input from a string.
 * @param s		String to use as input.
 * @ingroup ios
 */

/**
 * @fn StringInput read(cstring& s);
 * Shortcut to perform an input from a string.
 * @param s		String to use as input.
 * @ingroup ios
 */

/**
 * @fn StringInput read(string& s);
 * Shortcut to perform an input from a string.
 * @param s		String to use as input.
 * @ingroup ios
 */

/**
 * @fn FileInput read(sys::Path p);
 * Shortcut to perform an input from a file.
 * @param p					Path to the file to read from.
 * @raise io::IOException	If the file cannot be opened.
 * @ingroup ios
 */

/**
 * @fn FileOutput write(sys::Path p);
 * Shortcut to perform an output to a file.
 * @param p					Path to the file to write to.
 * @raise io::IOException	If the file cannot be created.
 * @ingroup ios
 */

/**
 * @fn FileOutput append(sys::Path p);
 * Shortcut to append an output to a file.
 * @param p					Path to the file to append to.
 * @raise io::IOException	If the file cannot be opened.
 * @ingroup ios
 */

} } // elm::io
