/*
 * $Id$
 * Copyright (c) 2004, IRIT-UPS.
 *
 * src/inhstruct_AVLTree.cpp -- AVLTree implementation.
 */

#include <elm/assert.h>
#include <elm/inhstruct/AVLTree.h>
#include <elm/io.h>

#define MARK	cerr << __FILE__ << ":" << __LINE__ << io::endl

namespace elm { namespace inhstruct {

// Private
static inline int max(int x, int y) {
	if(x > y)
		return x;
	else
		return y;
}
static int inline abs(int x) {
	if(x >= 0)
		return x;
	else
		return -x;
}


/**
 * Replace a child from a parent, depending the child to remove.
 * @param parent	Parent to remove a child from.
 * @param old		Old child to replace.
 * @param _new		New child for replacement.
 */
static inline void replaceChild(
	AVLTree::Node *parent,
	AVLTree::Node *old,
	AVLTree::Node *_new
) {
	if(parent->_left() == old)
		parent->insertLeft(_new);
	else {
		ASSERT(parent->_right() == old);
		parent->insertRight(_new);
	}
}


static void dump(AVLTree::Node *node, int tab = 0) {
	for(int i = 0; i < tab; i++)
		cout << "  ";
	cout << node << io::endl;
	if(!node)
		return;
	dump(node->_left(), tab + 1);	
	dump(node->_right(), tab + 1);	
}


/**
 * @class AVLTree::AVLNode
 * AVL tree requires a special kind of nodes implemenetd by this class.
 * For using inherited AVL trees, the tree ndoe must inherit this class.
 */


/**
 * @class AVLTree
 * Implementation of the Adelson-Velskii and Landis (AVL) tree algorithm.
 * In order to use this class, you must make a class inheriting this class
 * and overload the compare() protected method. Nodes inserted in the tree
 * must also inherit from the AVLTree::AVLNode class.
 * 
 * Performances:
 * @li lookup: O(Log n)
 * @li add: O(Log n)
 * @li removal: O(Log n)
 */

/**
 * @fn int AVLTree::compare(Node *node1, Node *node2);
 * This method is called for comparing nodes. It must be overloaded for getting
 * an actual implementation of AVL tree.
 * @param node1	First node to compare.
 * @param node2	Second node to compare.
 * @return		0 for equality, <0 if node1 < node2, >0 else.
 */


/**
 * @fn bool AVLTree::isEmpty(void);
 * Test if the tree is empty.
 * @return	True if the tree is empty, false else.
 */


/**
 * @fn int AVLTree::count(void);
 * count the number of nodes in the tree.
 * @return	Node count.
 */


/**
 * Get a tree node equal to the given one.
 * @param node	Node to test for equality.
 */
AVLTree::Node *AVLTree::get(Node *node) {
	ASSERTP(node, "null node");
	Node *cur = _root();
	while(cur) {
		int result = compare(node, cur);
		if(!result)
			break;
		else if(result < 0)
			cur = cur->_left();
		else
			cur = cur->_right();
	}
	return cur;
}


/**
 * @fn bool AVLTree::contains(Node *node);
 * Test if the given node value is contained in the tree.
 * @return	True if the node is contained, false else.
 */


/**
 * @fn void AVLTree::visit(Visitor *visitor);
 * Visit the node of the tree.
 * @param visitor	The method process() of this object is called with each
 * tree node.
 */


/**
 * @fn void AVLTree::search(Visitor *visitor);
 * Look for a special node in the given tree.For each node, the process() method
 * of the visitor is called. If it returns 0, search stops. If it returns,
 * <0 traversal continue with left child else with the right child.
 * @param visitor	Visitor to use.
 */


/**
 * Insert a new node in the tree.
 * @param node	Node to insert.
 */
void AVLTree::insert(Node *node) {
	setRoot(insert(_root(), node));
}


/**
 * Remove a node from the tree.
 * @param node	Node to remove.
 */
void AVLTree::remove(Node *node) {
	setRoot(remove(_root(), node));
}


// Private
AVLTree::Node *AVLTree::insert(Node *cur, Node *node) {
	ASSERTP(node, "null node");
	if(!cur) {
		node->h = 1;
		return node;
	}
	else {
		int res = compare(cur, node);
		
		// Insert to left
		if(res > 0 ) {
            cur->insertLeft(insert(cur->_left(), node));
            if(height(cur->_left()) - height(cur->_right()) >= 2) {
				if(compare(cur->_left(), node) > 0)
					cur = rotateSingleLeft(cur);
				else
					cur = rotateDoubleLeft(cur);
            }
		}

		// Insert to right
		else if(res < 0 ) {
            cur->insertRight(insert(cur->_right(), node));
            if(height(cur->_right()) - height(cur->_left()) >= 2) {
				if(compare(cur->_right(), node) < 0)
					cur = rotateSingleRight(cur);
				else
					cur = rotateDoubleRight(cur);
            }
		}

		// Nothing to do
		else
			return cur;
		
		// Recompute max
		computeHeight(cur);
		ASSERT(abs(height(cur->_left()) - height(cur->_right())) < 2);
		return cur;
	}
}

// Private
AVLTree::Node *AVLTree::rotateSingleLeft(Node *root) {
	ASSERTP(root, "null root");
	Node *new_root = root->_left();
	root->insertLeft(new_root->_right());
	new_root->insertRight(root);
	computeHeight(root);
	computeHeight(new_root);
	return new_root;
}

// Private
AVLTree::Node *AVLTree::rotateSingleRight(Node *root) {
	ASSERTP(root, "null root");
	Node *new_root = root->_right();
	root->insertRight(new_root->_left());
	new_root->insertLeft(root);
	computeHeight(root);
	computeHeight(new_root);
	return new_root;
}

// Private
AVLTree::Node *AVLTree::rotateDoubleLeft(Node *root) {
	root->insertLeft(rotateSingleRight(root->_left()));
	return rotateSingleLeft(root);
}

// Private
AVLTree::Node *AVLTree::rotateDoubleRight(Node *root) {
	root->insertRight(rotateSingleLeft(root->_right()));
	return rotateSingleRight(root);
}


// Private
AVLTree::Node *AVLTree::balanceLeft(Node *cur, Node*& root) {
	if(!cur->_left()) {
		root = cur;
		return cur->_right();
	}
	else {
		cur->insertLeft(balanceLeft(cur->_left(), root));
		if(height(cur->_right()) - height(cur->_left()) > 1)
			cur = rotateSingleRight(cur);
		computeHeight(cur);
		return cur;
	}
}


// Private
AVLTree::Node *AVLTree::balanceRight(Node *cur, Node*& root) {
	if(!cur->_right()) {
		root = cur;
		return cur->_left();
	}
	else {
		cur->insertRight(balanceRight(cur->_right(), root));
		if(height(cur->_left()) - height(cur->_right()) > 1)
			cur = rotateSingleLeft(cur);
		computeHeight(cur);
		return cur;
	}
}


// Private
AVLTree::Node *AVLTree::remap(Node *left, Node *right) {
	
	/* Trivial case */
	if(!left)
		return right;
	if(!right)
		return left;
	
	/* Remap with left down */
	Node *root = 0;
	if(left->h < right->h) {
		if(!right->_left()) {
			right->insertLeft(left);
			root = right;
		}
		else {
			Node *new_right = balanceLeft(right, root);
			ASSERT(root);
			root->insertLeft(left);
			root->insertRight(new_right);
		}
	}
	
	/* Remap with right down */
	else {
		if(!left->_right()) {
			left->insertRight(right);
			root = left;
		}
		else {
			Node *new_left = balanceRight(left, root);
			ASSERT(root);
			root->insertRight(right);
			root->insertLeft(new_left);
		}
	}
	
	// Return result
	computeHeight(root);
	ASSERT(abs(height(root->_left()) - height(root->_right())) < 2);	
	return root;
}


/**
 * Dump the content of tree. For debugging purpose, only.
 */
#ifdef ELM_DEBUG_AVLTREE
void AVLTree::dump(Node *node, int level) {
	if(!node)
		dump(_root(), 0);
	else {
		for(int i = 0; i < level; i++)
			cout << "| ";
		cout << "+-";
		node->dump();
		cout << '\n';
		if(node->_left())
			dump(node->_left(), level + 1);
		if(node->_right())
			dump(node->_right(), level + 1);
	}
}
#endif


/**
 * this method is called for freeing a node in the tree. This method may be
 * overloaded for providing special freeing method. Not overloaded, do nothing.
 * @param node	Node to free.
 */
void AVLTree::free(Node *node) {
}


// Private
void AVLTree::clean(Node *node) {
	if(!node)
		return;
	else {
		clean(node->_left());
		clean(node->_right());
		free(node);
	}
}


/**
 * Remove a node from a subtree.
 * @param root	Root of the subtree.
 * @param node	Node to remove.
 * @return		New root of the tree.
 */
AVLTree::Node *AVLTree::remove(Node *root, Node *node) {
	ASSERTP(node, "null node to remove");
	ASSERT(!root || abs(height(root->_left()) - height(root->_right())) < 2);
	Node *cur = root;
	int cmp = compare(cur, node);
	
	// Look right
	if(cmp < 0) {
		root->insertRight(remove(root->_right(), node));
		cerr << "!!! " << (height(root->_left()) - height(root->_right())) << "\n";
		dump(root);
		if(height(root->_left()) - height(root->_right()) >= 2)
			root = rotateSingleLeft(root);
		dump(root);
		ASSERT(!root || abs(height(root->_left()) - height(root->_right())) < 2);
	}
	
	// Look left
	else if(cmp > 0) {
		root->insertLeft(remove(root->_left(), node));
		if(height(root->_right()) - height(root->_left()) >= 2)
			root = rotateSingleRight(root);
		ASSERT(!root || abs(height(root->_left()) - height(root->_right())) < 2);
	}
	
	// Found
	else {
		Node *left = cur->_left(), *right = cur->_right();
		delete cur;
		if(!left)
			root = right;
		else if(!right)
			root = left;
		else if(left->h > right->h) {
			left = removeGreatest(left, root);
			ASSERT(abs(height(left) - height(right) < 2));
			root->insertLeft(left);
			root->insertRight(right);
		}
		else {
			right = removeLeast(right, root);
			ASSERT(abs(height(left) - height(right) < 2));
			root->insertRight(right);
			root->insertLeft(left);
		}
	}
	
	// check and return result
	if(root)
		computeHeight(root);
	ASSERT(!root || abs(height(root->_left()) - height(root->_right())) < 2);
	return root;
}


/**
 * Remove the greatest node of a subtree.
 * @param root	Subtree to look in (must not be null).
 * @param res	To return the removed node.
 * @return		New top of the subtree.
 */
AVLTree::Node *AVLTree::removeGreatest(Node *root, Node *&res) {
	ASSERT(root);
	
	// At the end
	if(!root->_right()) {
		res = root;
		root = root->_left();
	}
	
	// Go down
	else {
		root->insertRight(removeGreatest(root->_right(), res));
		if(height(root->_left()) - height(root->_right()) >= 2)
			root = rotateSingleLeft(root);
		else
			computeHeight(root);
	}
	
	// Check
	ASSERT(!root || abs(height(root->_left()) - height(root->_right())) < 2);
	return root;
}


/**
 * Remove the least node of a subtree.
 * @param root	Subtree to look in (must not be null).
 * @param res	To return the removed node.
 * @return		New top of the subtree.
 */
AVLTree::Node *AVLTree::removeLeast(Node *root, Node *&res) {
	ASSERT(root);
	
	// At the end
	if(!root->_left()) {
		res = root;
		root = root->_right();
	}
	
	// Go down
	else {
		root->insertLeft(removeLeast(root->_left(), res));
		if(height(root->_right()) - height(root->_left()) >= 2)
			root = rotateSingleRight(root);
		else
			computeHeight(root);
	}
	
	// Check
	ASSERT(!root || abs(height(root->_left()) - height(root->_right())) < 2);
	return root;
}


/**
 * @fn void AVLTree::clean(void)
 * Remove all nodes from the tree.
 */

} } // elm::inhstruct

