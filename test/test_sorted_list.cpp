/*
 *	$Id$
 *	Test for SortedSLList class.
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2008, IRIT UPS.
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
#include <elm/data/SortedList.h>
#include <elm/util/test.h>

using namespace elm;

TEST_BEGIN(sorted_list)

	SortedList<int> list;
	
	// Addition
	list.add(5);
	CHECK(list.contains(5));
	CHECK(!list.contains(0));
	CHECK(!list.isEmpty());
	list.add(9);
	CHECK(list.contains(9));
	CHECK(list.contains(5));
	list.add(2);
	CHECK(list.contains(2));
	CHECK(list.contains(9));
	
	// Traversal
	SortedList<int>::Iter item(list);
	CHECK(item);
	if(item) {
		CHECK(item == 2);
		item++;
		CHECK(item);
		if(item) {
			CHECK(item == 5);
			item++;
			CHECK(item);
			if(item) {
				CHECK(item == 9);
				item++;
				CHECK(!item);
			}
		}
	}
	
	// Removal
	list.remove(5);
	CHECK(!list.contains(5));
	CHECK(list.contains(2));
	CHECK(list.contains(9));
	list.remove(9);
	CHECK(!list.contains(5));
	CHECK(list.contains(2));
	CHECK(!list.contains(9));
	list.remove(2);
	CHECK(!list.contains(5));
	CHECK(!list.contains(2));
	CHECK(!list.contains(9));
	CHECK(list.isEmpty());
	
	// Sorted list clone
	list.add(3);
	list.add(0);
	list.add(5);
	SortedList<int> cloned_list(list);
	CHECK(!list.isEmpty());
	CHECK(list.contains(3));
	CHECK(list.contains(0));
	CHECK(list.contains(5));
	CHECK(!list.contains(2));

TEST_END
