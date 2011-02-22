/*
 *	$Id$
 *	MD5 class test
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2010, IRIT UPS.
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

#include <elm/util/test.h>
#include <elm/checksum/MD5.h>
#include <elm/system/System.h>

using namespace elm;
using namespace elm::checksum;

void test_md5(void) {
	CHECK_BEGIN("MD5");
	MD5 md5;
	io::InStream *in = system::System::readFile("test");
	md5.put(*in);
	delete in;
	md5.print(cout);
	cout << io::endl;
	CHECK_END;
}