/*
 * $Id$
 * Copyright (c) 2005, IRIT-UPS.
 *
 * test/test_table.cpp -- unit tests for Table class.
 */

#include <elm/array.h>
#include <elm/data/Array.h>
#include <elm/test.h>
#include "check-concept.h"

using namespace elm;

class TopArray {
public:
	AllocArray<int> tab;
	TopArray(void): tab(0) { }
	TopArray(int size): tab(size) { }
	TopArray(const TopArray& tt): tab(tt.tab) { }
};

class BottomTable {
public:
	AllocArray< TopArray > tab;
	BottomTable(int size): tab(size) { }
};


TEST_BEGIN(array)

	// concepts
	{
		Array<int> a;
		const Array<int> ca;
		checkCollection(a, ca, 111);
		checkArray(ca, 111);
		checkMutableArray(a, 111);
		checkCollection(a, ca, 111);
	}

	// Simple tests
	{
		int t[] = { 0, 1, 2, 3 };
		Array<int> tab(4, t);
		CHECK(tab[0] == 0);
		CHECK(tab[1] == 1);
		CHECK(tab[2] == 2);
		CHECK(tab[3] == 3);
		CHECK(tab.size() == 4);
		int i = 0;
		for(Array<int>::Iter n(tab); !n.ended(); n++, i++) {
			CHECK(i < 4);
			CHECK(*n == t[i]);
		}
		CHECK(i == 4);
	}
	
	// Assignment tests
	{
		int t[] = { 0, 1, 2, 3 };
		Array<int> tab(4, t);
		CHECK(tab[2] == 2);
		tab[2] = 4;
		CHECK(tab[2] == 4);
		tab.set(2, 6);
		CHECK(tab[2] == 6);
	}
	
	// Complex tables
	{
		BottomTable tt(4);
		tt.tab[0] = TopArray(4);
		tt.tab[1] = TopArray(4);
		tt.tab[2] = TopArray(4);
		tt.tab[3] = TopArray(4);
	}
	
	// Boolean table
	{
		AllocArray<bool> tab(1024);
		tab[0] = false;
		CHECK(tab[0] == false);
		tab[1] = true;
		CHECK(tab[1] == true);
	}

	// Automatic maker
	{
		int t[4] = { 1, 2, 3, 4 };
		Array<int> a = _array(4, t);
		CHECK_EQUAL(a.size(), 4);
	}

	// iterable
	{
		int t[4] = { 1, 2, 3, 4 };
		Array<int> a = _array(4, t);
		bool ok = true;
		int i = 0;
		for(auto x: a) {
			ok = ok || (x == i);
			i++;
		}
		CHECK(ok);
	}

	// array operations
	if(false) {
		int t[100], u[100];
		array::copy(t, u, 100);
		array::move(t, u, 100);
		array::set(t, 100, 0);
		array::clear(t, 100);
		array::equals(t, u, 100);
		array::construct(t, 100);
	}

	if(false) {
		cstring t[100], u[100];
		array::copy(t, u, 100);
		array::move(t, u, 100);
		array::set(t, 100, cstring(""));
		array::clear(t, 100);
		array::equals(t, u, 100);
		array::construct(t, 100);
	}
	
	// back traversal
	{
		Array<int> a(4, new int[4]);
		int i = 0;
		for(i = 0; i < 4; i++)
			a[i] = i;
		for(auto x: a.back())
			if(x == i - 1)
				i--;
		CHECK_EQUAL(i, 0);
	}

TEST_END

	

