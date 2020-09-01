/*
 * $Id$
 * Copyright (c) 2005, IRIT-UPS.
 *
 * test_sorted_bintree.cpp -- unit tests for SortedBinTree class.
 */

#include <elm/data/TreeBag.h>
#include <elm/data/TreeMap.h>
#include <elm/sys/System.h>
#include "../include/elm/test.h"

using namespace elm;

inline void get(bool b) { }

#define COUNT	10000
#define MAX		10000

// test routine
TEST_BEGIN(tree_bag)
	
	// instantiation test
	{
		if(false) {
			TreeBag<int> t;
			const TreeBag<int> ct(t);
			t.comparator();
			ct.comparator();
			t.count();
			t.contains(1);
			t.isEmpty();
			get(static_cast<bool>(t));
			t.begin();
			t.end();
			volatile int s;
			for(const auto x: t) s += x;
			t.clear();
			t.add(1);
			t.addAll(t);
			t.remove(1);
			t.removeAll(t);
			t.remove(t.begin());
			t.containsAll(t);
		}
	}

	// Base test
	{
		TreeBag<int> tree;
		tree.add(5);
		tree.add(0);
		tree.add(1);
		tree.add(2);
		tree.add(4);
		tree.add(3);
		CHECK(!tree.contains(10));
		CHECK(tree.contains(0));
		CHECK(tree.contains(1));
		CHECK(tree.contains(2));
		CHECK(tree.contains(3));
		CHECK(tree.contains(4));
		CHECK(tree.contains(5));

		int field = 0, cnt = 0;
		for(const auto x: tree) {
			cnt++;
			field |= 1 << x;
		}
		CHECK(cnt == 6);
		CHECK(field == 0x3f);
		int n = 0;
		for(const auto i: tree) {
			CHECK_EQUAL(i, n);
			n++;
		}
	}
	
	{
		int cnt = 0, vals[COUNT];
		TreeBag<int> tree;
		for(int i = 0; i < COUNT; i++) {
			int v = sys::System::random(MAX * 2);
			
			// Add
			if(!cnt || (v & 1)) {
				v >>= 1;
				//cerr << "Action " << i << ": add " << v << io::endl;
				vals[cnt++] = v;
				tree.add(v);
			}
			
			// Remove
			else {
				v = (v >> 1) % cnt;
				//cerr << "Action " << i << ": remove " << vals[v] << io::endl;
				tree.remove(vals[v]);
				for(int j = v + 1; j < cnt; j++)
					vals[j - 1] = vals[j];
				cnt --;
			}
		}
		bool robustness = true;
		CHECK(robustness);
		
		/*for(genstruct::SortedBinTree<int>::Iterator iter(tree); iter; iter++)
			cerr << *iter << ", ";
		cerr << io::endl;*/
	}
	
	// TreeMap
	{
		if(false) {
			TreeMap<int, int> t;
			const TreeMap<int, int> ct(t);
			t.comparator();
			ct.comparator();
			t.equivalence();
			t.allocator();
			t.count();
			t.contains(1);
			t.isEmpty();
			get(static_cast<bool>(t));
			t.begin();
			t.end();
			t.get(1, 2);
			t.get(1);
			t.hasKey(1);
			t.put(1, 2);
			t.remove(1);
			t.remove(t.begin());
			t.keys();
			t.pairs();
		}
	}

	{
		bool error = false;
		TreeMap<int, int *> map;
		int cnt = 0, vals[COUNT];
		for(int i = 0; !error && i < COUNT; i++) {
			int v = sys::System::random(MAX * 4);
			switch(v & 0x3) {
				
			case 0:		// remove
				if(cnt) {
					v = (v >> 2) % cnt;
					//cerr << "removing " << vals[v] << io::endl;
					int *p = map.get(vals[v]);
					map.remove(vals[v]);
					delete p;
					for(int j = v + 1; j < cnt; j++)
						vals[j - 1] = vals[j];
					cnt --;
					break;
				}
				/* no break */
				
			case 1:		// add
			add:
				v >>= 2;
				//cerr << "adding " << v << io::endl;
				vals[cnt++] = v;
				map.put(v, new int(v));
				break;
			
			default:	// Get
				{
					if(!cnt)
						goto add;
					v = (v >> 2) % cnt;
					//cerr << "getting " << vals[v] << io::endl;
					int *res = map.get(vals[v], 0);
					error = !res || *res != vals[v];
				}
				/* no break */
			}
			
			// Full check
			for(int j = 0; j < cnt; j++)
				ASSERT(map.get(vals[j], 0));
		}
		CHECK(!error);

		// cleanup in the end
		for(const auto p: map)
			delete p;
	}

TEST_END

