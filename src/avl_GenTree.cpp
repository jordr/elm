/*
 *	avl::GenTree class implementation
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2013, IRIT UPS.
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

#include <elm/avl/GenTree.h>
#include <elm/avl/Set.h>
#include <elm/avl/Map.h>
#include <elm/data/Vector.h>
#include <elm/compare.h>

namespace elm { namespace avl {

using namespace elm;

/*
 * Let T supporting a partial-order (T, ≤),
 * AVLTree(T) = node(val: T, bal: ℤ, left: AVLTree(T), right: AVLTree(T)) | nil
 *
 * Invariant — let t ∈ AVLTree(T),
 * left_sorted(nil) = ⊤
 * left_sorted(node(x, _, l, r)) =
 * 		(∀ y ∈ values(l) ⇒ y < x) ∧ left_sorted(l) ∧ left_sorted(r)
 * right_sorted(nil) = ⊤
 * right_sorted(node(x, _, l, r)) =
 * 		(∀ y ∈ values(l) ⇒ x < y) ∧ right_sorted(l) ∧ right_sorted(r)
 * balanced(nil) = ⊤
 * balanced(node(_, b, l, r) =
 * 		b = h(r) - h(l) ∧ -1 ≤ b ≤ 1 ∧ balanced(l) ∧ balanced(r)
 * invariant(t) = left_sorted(t) ∧ right_sorted(t) ∧ balanced(t)
 *
 * Reference:
 * http://interactivepython.org/runestone/static/pythonds/Trees/AVLTreeImplementation.html
 */


#ifdef ELM_AVL_INVARIANT
bool AbstractTree::invariant(Node *n, int& h, Node *& max, Node *& min) {

	// empty tree
	if(n == nullptr) {
		h = 0;
		max = nullptr;
		min = nullptr;
	}

	else {

		// recursive call
		int lh, rh;
		Node *lmax, *rmax;
		Node *lmin, *rmin;
		if(!invariant(n->_left, lh, lmax, lmin))
			return false;
		if(!invariant(n->_right, rh, rmax, rmin))
			return false;

		// compute results
		h = elm::max(lh, rh) + 1;
		if(rmax == nullptr)
			max = n;
		else
			max = rmax;
		if(rmin == nullptr)
			min = n;
		else
			min = rmin;

		// check and compute heights
		if(rh - lh != n->_bal) {
			cerr << "bad balance factor at ";
			print(cerr, n);
			cerr << "(expected " << (rh - lh) << ")\n";
			return false;
		}

		// check maximum at left
		if(lmax != nullptr && !(cmp(lmax, n) < 0)) {
			cerr << "not left ordered at ";
			print(cerr, n);
			cerr << io::endl;
			return false;
		}

		// check minimum at right
		if(rmin != nullptr && !(cmp(rmin, n) > 0)) {
			cerr << "not right ordered at ";
			print(cerr, n);
			cerr << io::endl;
			return false;
		}
	}

	return true;
}

bool AbstractTree::invariant(Node *n) {
	int h;
	Node *max, *min;
	bool r = invariant(n, h, max, min);
	if(!r)
		printTree(cout);
	return r;
}

AbstractTree::Node *AbstractTree::pointed(const Stack& s) const {
	if(s.isEmpty())
		return _root;
	else if(s.topDir() == LEFT)
		return s.topNode()->_left;
	else
		return s.topNode()->_right;
}

void AbstractTree::printTree(io::Output& out, Node *n, t::uint32 m, int l, dir_t d) const {
	if(n == nullptr)
		return;

	// print right
	t::uint32 rm = m | (1 << l);
	if(l != 0 && d == RIGHT)
		rm &= ~(1 << (l - 1));
	printTree(out, n->_right, rm, l + 1, RIGHT);

	// print node
	for(int i = 0; i < l; i++)
		if((m & (1 << i)) == 0)
			cout << "    ";
		else
			cout << "   |";
	print(out, n);
	out << " (" << n->_bal << ")\n";

	// print left
	t::uint32 lm = m | (1 << l);
	if(l != 0 && d == LEFT)
		lm &= ~(1 << (l - 1));
	printTree(out, n->_left, lm, l + 1, LEFT);

}

void AbstractTree::printTree(io::Output& out, Node *n) {
	if(n == nullptr)
		out << "empty";
	else
		printTree(out, n, 0, 0, NONE);
}

void AbstractTree::printTree(io::Output& out) {
	printTree(out, _root);
}

#	define INVARIANT(n)		ASSERT(invariant(n))
#else
#	define INVARIANT(n)
#endif


/**
 * Get the link to the pointer of the top-node corresponding
 * to the followed path (basically used to relink nodes).
 * @param s	Stack to the concerned node.
 */
AbstractTree::Node **AbstractTree::link(const Stack& s) {
	if(s.isEmpty())
		return &_root;
	else if(s.topDir() == LEFT)
		return &s.topNode()->_left;
	else
		return &s.topNode()->_right;
}


/**
 * Exchange two nodes values (does not update the parent link).
 * @param n		First node.
 * @param m		Second node.
 */
void AbstractTree::exchange(Node *n, Node *m) {
	auto nl = n->_left;
	auto nr = n->_right;
	auto nb = n->_bal;
	n->_left = m->_left;
	n->_right = m->_right;
	n->_bal = m->_bal;
	m->_left = nl;
	m->_right = nr;
	m->_bal = nb;

}


/**
 * Perform right-balancing of the node at the top of the stack.
 * @param s		Stack which top-node must be balanced.
 */
void AbstractTree::rotateRight(Stack& s) {
	Node *r = s.topNode();
	s.pop();
	Node *nr = r->_left;

	// relink
	//set(s, nr);
	*link(s) = nr;
	r->_left = nr->_right;
	nr->_right = r;

	// udpate balance
	r->_bal = r->_bal + 1 - min(int(nr->_bal), 0);
	nr->_bal = nr->_bal + 1 + max(int(r->_bal), 0);
}


/**
 * Perform left-balancing of the node at the top of the stack.
 * @param s		Stack which top-node must be balanced.
 */
void AbstractTree::rotateLeft(Stack& s) {
	Node *r = s.topNode();
	s.pop();
	Node *nr = r->_right;

	// relink
	//set(s, nr);
	*link(s) = nr;
	r->_right = nr->_left;
	nr->_left = r;

	// udpate balance
	r->_bal = r->_bal - 1 - max(int(nr->_bal), 0);
	nr->_bal = nr->_bal - 1 + min(int(r->_bal), 0);
}


/**
 * Perform actual insertion and re-balancing of a new value.
 * @param s		Tree traversal stack.
 * @param node	Added node.
 */
void AbstractTree::insert(Stack& s, Node *node) {
	_cnt++;

	// empty tree
	if(_root == nullptr) {
		_root = node;
		return;
	}

	// insert the node
	//set(s, node);
	*link(s) = node;

	// update the balance factor and find the unbalancing point
	while(!s.isEmpty()) {

		// consume current node
		int dir = s.topDir();
		Node *n = s.topNode();

		// fix the balance
		if(dir == LEFT)
			n->_bal -= 1;
		else
			n->_bal += 1;

		// balanced!
		if(n->_bal == 0)
			break;

		// too high on left
		else if(n->_bal < -1) {
			if(n->_left->_bal > 0) {
				s.push(n->_left, LEFT);
				rotateLeft(s);
			}
			rotateRight(s);
			break;
		}

		// too high on right
		else if(+1 < n->_bal) {
			if(n->_right->_bal < 0) {
				s.push(n->_right, RIGHT);
				rotateRight(s);
			}
			rotateLeft(s);
			break;
		}

		// propagate addition
		s.pop();
	}

}


/**
 * Perform removal and re-balance operation.
 * @param s		Parent stack.
 * @param n		Replacing node.
 */
void AbstractTree::remove(Stack& s, Node *n) {

	// relink
	_cnt--;
	*link(s) = n;

	// propagate the balance update
	while(!s.isEmpty()) {
		Node *p = s.topNode();
		dir_t d = s.topDir();
		ASSERT(d == LEFT || d == RIGHT);

		// removed on left
		if(d == LEFT) {

			// fix balance factor
			p->_bal++;
			//cout << "DEBUG: LEFT "; print(cerr, p); cout << " (" << p->_bal << ") ";

			// done: height reduction
			// remove B: (B) <-- (A)
			if(p->_bal == 0) {
				//cerr << " cleared!\n";
				s.pop();
			}

			// done: no height reduction
			// remove B: (B) <-- (A) --> (C)
			else if(p->_bal == +1) {
				//cerr << "done\n";
				break;
			}

			// re-balance needed (BAL = +2)

			// right full: single left rotate and no height reduction
			// remove B: (B) <-- (A) --> (D <-- C --> E)
			else if(p->_right->_bal == 0){
				//cerr << "single left and done\n";
				rotateLeft(s);
				break;
			}

			// left left: single left rotate and level reduction
			// remove B: (D <-- C) <-- (A) --> (B)
			else if(p->_right->_bal == +1) {
				//cerr << "single left\n";
				rotateLeft(s);
			}

			// left right: double left rotate and level reduction
			// remove B:  (B) <-- (A) --> (D <-- C)
			else {
				//cerr << "double left\n";
				s.pop();
				s.push(p, RIGHT);
				s.push(p->_right, NONE);
				rotateRight(s);
				rotateLeft(s);
			}
		}

		// removed on right
		else {

			// fix balance factor
			p->_bal--;
			//cout << "DEBUG: RIGHT "; print(cerr, p); cout << " (" << p->_bal << ") ";

			// done: height reduction
			// remove B: (A) --> (B)
			if(p->_bal == 0) {
				//cerr << " cleared!\n";
				s.pop();
			}

			// done: no height reduction
			// remove B: (C) <-- (A) --> (B)
			else if(p->_bal == -1) {
				//cerr << "done\n";
				break;
			}

			// re-balance needed (BAL = -2)

			// left full: single right rotate and no height reduction
			// remove B: (D <-- C --> E) <-- (A) --> (B)
			else if(p->_left->_bal == 0){
				//cerr << "single right and done\n";
				rotateRight(s);
				break;
			}

			// left left: single right rotate and level reduction
			// remove B: (D <-- C) <-- (A) --> (B)
			else if(p->_left->_bal == -1) {
				//cerr << "single right\n";
				rotateRight(s);
			}

			// left right: double right rotate and level reduction
			// remove B:  (C --> D) <-- (A) --> (B)
			else {
				//cerr << "double right\n";
				s.pop();
				s.push(p, LEFT);
				s.push(p->_left, NONE);
				rotateLeft(s);
				rotateRight(s);
			}
		}

		INVARIANT(pointed(s))
	}

	INVARIANT(_root);
}


/**
 * Find the left node starting at this point.
 * @param s		Stack containing parents of n and to fill with parent of leftmost node.
 * @param n		Current node.
 * @return		Leftmost node.
 */
AbstractTree::Node *AbstractTree::leftMost(Stack& s, Node *n) {
	while(n->_left) {
		s.push(n, LEFT);
		n = n->_left;
	}
	return n;
}


/**
 * @fn int AbstractTree::count(void) const;
 * Count the number of nodes.
 * @return	Number of nodes.
 */


/**
 * @class GenTree
 * This class implements an AVL tree collection based on C++ templates as provided by:
 * Ben Pfaff, "An Introduction to
 * Binary Search Trees and Balanced Trees",
 * Libavl Binary Search Tree Library, Volume 1: Source Code, Version 2.0.2.
 * @par
 * This class is rarely used as is but used as a base class for @ref elm::avl::Set or @ref elm::avl::Map.
 * 
 * @param T		Type of contained items.
 * @param C		Comparator for T items (default to @ref elm::Comparator<T>).
 * 
 * @par Implemented concepts
 * @li @ref elm::concept::Collection<T>
 * @li @ref elm::concept::MutableCollection<T>
 *
 * @see			@ref elm::avl::Set, @ref elm::avl::Map
 * @ingroup		data
 */

/**
 * @fn int GenTree::count(void) const;
 * Get the count of items in the tree.
 * @return	Item count.
 * @notice	This function call is fast as the item count is maintained
 *			during each insertion and removal.
 */

/**
 * @fn bool GenTree::contains(const T& item) const;
 * Test if the tree contains the given item.
 * @param item	Item to look for.
 * @return		True if it is contained, false else.
 * @notice		Access time in log2(item number).
 */

/**
 * @fn bool GenTree::containsAll(const Co<T>& coll) const;
 * Test if the given collection is contained in the current one.
 * @param coll	Collection to test.
 * @return		True if the collection is containted, false else.
 * @param C		Type of the collection.
 */

/**
 * @fn bool GenTree::isEmpty(void) const;
 * Test if the tree is empty.
 * @return	True if the tree is empty, false else.
 */

/**
 * @class GenTree::Iterator
 * Iterator on items of the tree. No assumption may be made on the order of traversal
 * of the items of the tree.
 */

/**
 * @fn void GenTree::clear(void);
 * Remove all items from the tree and let it cleared.
 */

/**
 * @fn void GenTree::add(const T& item);
 * Add an item to the tree. Note that the item is still added even if it is already
 * contained in the tree, leading to duplicates.
 * @param item	Item to add.
 */

/**
 * @fn void GenTree::addAll(const Co<T>& coll);
 * Add a collection to this tree.
 * @param coll	Collection to add.
 * @param C		Type of the collection.
 */

/**
 * @fn void GenTree::remove(const T& item);
 * Remove an item from a tree. Notice that if the tree contains duplicates of the item,
 * only the first duplicate is removed.
 * @param item	Item to remove.
 * @warning	Attempting to remove an item not contained in the tree is an error.
 */

/**
 * @fn void GenTree::removeAll(const Co<T>& coll);
 * Remove a collection from this tree.
 * @param coll	Collection to remove.
 * @param C		Type of the collection.
 */

/**
 * @fn void GenTree::remove(const Iterator& iter);
 * Remove the item pointed by the iterator.
 * @param	Iterator pointing to the item to remove.
 */


/**
 * @class Set
 * Implements a set collection based on an AVL tree, that is, supporting access and modifications
 * with a O(log n) complexity.
 * @par Implemented concepts
 * @li @ref elm::concept::Collection<T>
 * @li @ref elm::concept::MutableCollection<T>
 * @li @ref elm::concept::Set<T>
 *
 * @param T		Type of stored items.
 * @param C		Comparator used to sort the items (must implements the @ref elm::concept::Comparator<T> concept,
 * 				as a default @ref elm::Comparator<T>).
 * @see			@ref elm::avl::GenTree
 * @ingroup		data
 */


/**
 * @class Map
 * Implements a map based on AVL tree, that is, a map supporting O(log n) accesses.
 * @par Implemented concepts
 * @li @ref elm::concept::Collection<T>
 * @li @ref elm::concept::Map<K, T>
 *
 * @param K		Type of keys of the map.
 * @param T		Type of stored items.
 * @param C		Comparator used to sort the items (must implements the @ref elm::concept::Comparator<K> concept,
 * 				as a default @ref elm::Comparator<K>).
 * @see			@ref elm::avl::GenTree
 * @ingroup		data
 */


/**
 * @class Queue
 * Implements a sorted queue based on AVL trees: putting or getting the head
 * element is obtained in O(log n) complexity.
 *
 * @par Implemented concepts
 *	* elm::concept::Collection<T>
 *	* elm::concept::Queue<T>
 *
 * @param T		Type of elements of the queue.
 * @param C		Comparator to sort the queue (default to elm::Comparator).
 * @param A		Alloctaor (default (to elm::DefaultAlloc).
 * @see			@ref elm::avl::GenTree
 * @ingroup		data
 */

} }	// elm::avl
