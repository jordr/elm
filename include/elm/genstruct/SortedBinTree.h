/*
 *	$Id$
 *	SortedBinTree class interface
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
#ifndef ELM_GENSTRUCT_SORTEDBINTREE_H
#define ELM_GENSTRUCT_SORTEDBINTREE_H

#include <elm/utility.h>
#include <elm/Iterator.h>
#include <elm/genstruct/Vector.h>
#include <elm/genstruct/VectorQueue.h>
#include <elm/inhstruct/BinTree.h>

namespace elm { namespace genstruct {

#include <elm/util/Comparator.h>

// DefaultKey class
template <class T>
class DefaultIdent {
public:
	typedef T key_t;
	static const T& key(const T& v) { return v; }
};

// SortedBinTree class implements MutableCollection
template <class T, class C = Comparator<T>, class K = DefaultIdent<T> >
class SortedBinTree {
private:
	class Node: public inhstruct::BinTree::Node {
	public:
		inline Node(const T& value): val(value) { } 
		T val;
	};
	
public:
	
	// Methods
	inline SortedBinTree(void) { }
	SortedBinTree(const SortedBinTree<T, C>& tree);
	inline ~SortedBinTree(void) { clear(); }

	// Collection concept
	inline int count(void) const { return root.count(); }
	inline bool contains(const T& value) const { return find(K::key(value)); }
	inline bool isEmpty(void) const { return root.isEmpty(); }
 	inline operator bool(void) const { return !isEmpty(); }

	// Iterator class
	class Iterator: public PreIterator<Iterator, T> {
	public:
		inline Iterator(const SortedBinTree& tree)
			{ stack.push((Node *)tree.root.root()); }
		inline Iterator(const Iterator& iter)
			{ stack = iter.stack; }
		bool ended(void) const { return !stack; }
		void next(void) {
			Node *top = stack.pop();
			if(top->right()) stack.push((Node *)top->right());
			if(top->left()) stack.push((Node *)top->left());
		}
		const T& item(void) const { return stack.top()->val; }
	private:
		Vector<Node *> stack;
	};
	
	// MutableCollection concept
	void clear(void);
	void add(const T &item);
	template <template <class _> class S>
	void addAll (const S<T> &items) {
		for(typename S<T>::Iterator iter(items); iter; iter++)
			add(iter);
	}
	void remove(const T& item);
	template <template <class _> class S>
	void removeAll(const S<T> &items) {
		for(typename S<T>::Iterator iter(items); iter; iter++)
			remove(iter);
	}
	inline void remove(const Iterator &iter) {
		remove(*iter);
	}

	// MutableLookable<K> implementation
	inline const T *look(const typename K::key_t& key) const
		{ Node * node = find(key); return node ? &node->val : 0; }
	inline T *look(const typename K::key_t& key) 
		{ Node * node = find(key); return node ? (T *)&node->val : 0; }

private:
	inhstruct::BinTree root;
	Node *find(const typename K::key_t& item) const;
	void replace(Node *parent, Node *old, Node *_new);
};

template <class T, class C, class K>
inline typename SortedBinTree<T, C, K>::Node *
SortedBinTree<T, C, K>::find(const typename K::key_t& value) const {
	Node *node = (Node *)root.root();
	while(node) {
		int cmp = C::compare(value, K::key(node->val));
		if(cmp == 0)
			break;
		else if(cmp < 0)
			node = (Node *)node->right();
		else
			node = (Node *)node->left();
	}
	return node;
}

template <class T, class C, class K>
void SortedBinTree<T, C, K>::add(const T& value) {
	Node *node = (Node *)root.root();
	Node *new_node = new Node(value);
	if(!node)
		root.setRoot(new_node);
	else
		while(node) {
			int cmp = C::compare(K::key(value), K::key(node->val));
			if(cmp <= 0) {
				if(!node->right()) {
					node->insertRight(new_node);
					break;
				}
				else
					node = (Node *)node->right();
			}
			else {
				if(!node->left()) {
					node->insertLeft(new_node);
					break;
				}
				else
					node = (Node *)node->left();
			}
		}
}

template <class T, class C, class K>
void SortedBinTree<T, C, K>::clear(void) {
	VectorQueue<Node *> todo;
	todo.put((Node *)root.root());
	while(todo) {
		Node *node = todo.get();
		if(node->left())
			todo.put((Node *)node->left());
		if(node->right())
			todo.put((Node *)node->right());
		delete node;
	}
}

template <class T, class C, class K>
void SortedBinTree<T, C, K>::replace(Node *parent, Node *old, Node *_new) {
	if(!parent)
		root.setRoot(_new);
	else if(parent->left() == old)
		parent->insertLeft(_new);
	else
		parent->insertRight(_new);
}

template <class T, class C, class K>
void SortedBinTree<T, C, K>::remove(const T& value) {
	Node *node = (Node *)root.root(), *parent = 0;
	while(true) {
		ASSERT(node);
		int cmp = C::compare(K::key(value), K::key(node->val));
		if(cmp == 0)
			break;
		parent = node;
		if(cmp < 0)
			node = (Node *)node->right();
		else
			node = (Node *)node->left();
	}
	Node *left = (Node *)node->left(), *right = (Node *)node->right();
	delete node;
	if(!left)
		replace(parent, node, right);
	else if(!right)
		replace(parent, node, left);
	else {
		replace(parent, node, left);
		for(node = left; node->right(); node = (Node *)node->right());
		node->insertRight(right);
	}
}

} } // elm::genstruct

#endif // ELM_GENSTRUCT_SORTEDBINTREE_H
