/*
 *	Tests for utilities
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2005-18, IRIT UPS.
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
#include <elm/utility.h>
#include <elm/hash.h>
#include <elm/util/strong_type.h>
#include <elm/util/Version.h>
#include <elm/test.h>

using namespace elm;

template <class T>
void read(T& x, string s) {
	io::StringInput sin(s);
	io::Input& in = sin;
	in >> x;
}

volatile int res;

typedef SolidType<int> index_t;

// Utility tests
TEST_BEGIN(utility)
	
	// Optional test
	{
		Option<int> one(666), none;
		CHECK(one.isOne() == true);
		CHECK(one.isNone() == false);
		CHECK(one.value() == 666);
		CHECK(none.isOne() == false);
		CHECK(none.isNone() == true);

		bool set = false;
		one.if_one([&set](int x) { set = true; });
		CHECK(set);

		set = false;
		none.if_else([&set]() { set = true; });
		CHECK(set);
	}
	
	// Hashing tests
	{
		CHECK(HashKey<int>::hash(0) == HashKey<int>::hash(0));
		CHECK(HashKey<int>::hash(666) == HashKey<int>::hash(666));
		CHECK(HashKey<int>::hash(-111) == HashKey<int>::hash(-111));
		CHECK(HashKey<int>::equals(0, 0) == true);
		CHECK(HashKey<int>::equals(666, 666) == true);
		CHECK(HashKey<int>::equals(666, -111) == false);
	}
	{
		char val;
		CHECK(HashKey<void *>::hash(0) == HashKey<void *>::hash(0));
		CHECK(HashKey<void *>::hash(&val) == HashKey<void *>::hash(&val));
		CHECK(HashKey<void *>::equals(0, 0) == true);
		CHECK(HashKey<void *>::equals(&val, &val) == true);
		CHECK(HashKey<void *>::equals(&val, 0) == false);
	}
	{
		String s1("0123456789"), s2, s3("0123456789");
		CHECK(HashKey<String>::hash(s2) == HashKey<String>::hash(s2));
		CHECK(HashKey<String>::hash(s1) == HashKey<String>::hash(s1));
		CHECK(HashKey<String>::hash(s1) == HashKey<String>::hash(s3));
		CHECK(HashKey<String>::equals(s2, s2) == true);
		CHECK(HashKey<String>::equals(s1, s1) == true);
		CHECK(HashKey<String>::equals(s1, s3) == true);
		CHECK(HashKey<String>::equals(s1, s2) == false);
	}
	{
		CString s1("0123456789"), s2, s3("0123456789");
		CHECK(HashKey<CString>::hash(s2) == HashKey<CString>::hash(s2));
		CHECK(HashKey<CString>::hash(s1) == HashKey<CString>::hash(s1));
		CHECK(HashKey<CString>::hash(s1) == HashKey<CString>::hash(s3));
		CHECK(HashKey<CString>::equals(s2, s2) == true);
		CHECK(HashKey<CString>::equals(s1, s1) == true);
		CHECK(HashKey<CString>::equals(s1, s3) == true);
		CHECK(HashKey<CString>::equals(s1, s2) == false);
	}
	
	// Version test
	{
		try {
			Version v = "1.2.3";
			CHECK(v.major() == 1);
			CHECK(v.minor() == 2);
			CHECK(v.release() == 3);
		}
		catch(elm::Exception& e) {
			cerr << "EXCEPTION: " << e.message() << io::endl;
		}
	}
	
	// Comparators tests
	{
		CHECK(Comparator<int>::compare(-111, 666) < 0);
		CHECK(Comparator<int>::compare(666, -111) > 0);
		CHECK(Comparator<int>::compare(666, 666) == 0);
		CHECK(Comparator<int>::compare(-111, -111) == 0);
		CHECK(Comparator<int>::compare(0, 0) == 0);
		CHECK(Comparator<int>::compare(0, 666) < 0);
		CHECK(Comparator<int>::compare(666, 0) > 0);
		CHECK(Comparator<int>::compare(0, -111) > 0);
		CHECK(Comparator<int>::compare(-111, 0) < 0);
	}
	{
		static char v[2];
		char *v1 = v, *v2 = v + 1;
		CHECK(Comparator<void *>::compare(v1,v2) < 0);
		CHECK(Comparator<void *>::compare(v2, v1) > 0);
		CHECK(Comparator<void *>::compare(v2, v2) == 0);
		CHECK(Comparator<void *>::compare(v1, v1) == 0);
		CHECK(Comparator<void *>::compare(0, 0) == 0);
		CHECK(Comparator<void *>::compare(0, v2) != 0);
		CHECK(Comparator<void *>::compare(v2, 0) != 0);
	}
	{
		String v1("ok"), v2("ko"), v3("ok"), v4("okk");
		CHECK(Comparator<string>::compare(v1, v1) == 0);		// Reflexive
		CHECK(Comparator<string>::compare(v1, v2) > 0);		// Symetric
		CHECK(Comparator<string>::compare(v2, v1) < 0);
		CHECK(Comparator<string>::compare(v1, v4) < 0);		// Transitive
		CHECK(Comparator<string>::compare(v2, v4) < 0);
		CHECK(Comparator<string>::compare(v1, v3) == 0);		// Internal representation
	}
	{
		CString v1("ok"), v2("ko"), v3("ok"), v4("okk");
		CHECK(Comparator<cstring>::compare(v1, v1) == 0);		// Reflexive
		CHECK(Comparator<cstring>::compare(v1, v2) > 0);		// Symetric
		CHECK(Comparator<cstring>::compare(v2, v1) < 0);
		CHECK(Comparator<cstring>::compare(v1, v4) < 0);		// Transitive
		CHECK(Comparator<cstring>::compare(v2, v4) < 0);
		CHECK(Comparator<cstring>::compare(v1, v3) == 0);		// Internal representation
	}
	
	// maximum and minimum
	{
		CHECK_EQUAL(max(10, 0), 10);
		CHECK_EQUAL(max(0, 10), 10);
		CHECK_EQUAL(max(10, 10), 10);
		CHECK_EQUAL(min(10, 0), 0);
		CHECK_EQUAL(min(0, 10), 0);
		CHECK_EQUAL(min(10, 10), 10);
	}

	{
		Pair<int, int> p;
		read(p, "3 5");
		CHECK(p.fst == 3 && p.snd == 5);
		string s = _ << p;
		CHECK(s == "3 5");
	}

	// strong type
	{

	}

TEST_END

