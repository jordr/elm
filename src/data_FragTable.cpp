/*
 *	FragTable class implementation
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2017, IRIT UPS.
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

#include <elm/data/FragTable.h>

namespace elm {

/**
 * @class FragTable
 * This container class allows indexed access to its data. It is implemented
 * as expandable fragmented table using a two-level table traversal. The first
 * level table selects a sub-table that contains the actual stored items.
 * @par
 * Consequently, the access time is a bit longer than a classic table but small
 * and constant. Yet, the table may be expanded without inducing a copy of already
 * stored content. According the granularity / size of the sub-tables, the 
 * place / item count ratio overhead is pretty small : this class is a good
 * candidate for big tables requiring dynamic expansion.
 * 
 * Complexity information (N items, chunks of size S):
 * * access time: O(0)
 * * addition time: O(0)
 * * insertion time: O(N/2) average, O(N) worst case
 * * removal time: O(N/2) average, O(N) worst case
 * * object memory footprint: 6 ints + 8 pointers
 * * per-item memory foot print: |N / S| pointers
 *
 * Implemented concepts:
 * * @ref concept::Collection
 * * @ref concept::MutableCollection
 * * @ref concept::Array
 * * @ref concept::MutableArray.
 *
 * @param T	Type of stored items.
 * @param M	Type of the data structure manager (default to @ref EquivManager).
 * @ingroup data
 */


/**
 * @fn FragTable::FragTable(int size_pow, M& man);
 * The constructor of a fragmented table.
 * @param size_pow	2^size_pow is the size of sub-tables (default to 8).
 * @param man		data structure manager (default to Single<EquivManager<T> >::_).
 */


/**
 * @fn FragTable::FragTable(const FragTable& tab);
 * Build a fragmented table by copy.
 * @param tab	Table to copy.
 */


/**
 * @fn int FragTable::pageSize() const;
 * Get the page size (in elements).
 * @return Page size (in elements).
 */


/**
 * @fn int FragTable::pagePower() const;
 * Get the page size as a power of 2.
 * @return	Power of 2 page size.
 */


/**
 * @fn bool FragTable::pageFull() const;
 * Test if the current page is full.
 * @return	True if the page is full, false else.
 */


/**
 * @fn int FragTable::count (void) const;
 * Same as @length().
 */


/**
 * @fn bool FragTable::isEmpty(void) const;
 * Test if the fragmented table is empty.
 * @return	True if it is empty, false else.
 */


/**
 * @fn FragTable::operator bool (void) const;
 * Same as !isEmpty().
 */


/**
 * @fn bool FragTable::contains(const T &item) const;
 * Test if the table contains the given item.
 * @param item	Item to look for.
 * @return		True if the item is contained in the table, false else.
 */


/**
 * @fn void FragTable<T>::clear(void);
 * Remove all items from the fragmeted table and release as many memory
 * as possible.
 */
 
 
/**
 * @fn void FragTable::add(const T& value);
 * Add an item to the table.
 * @param value	Value of the item to add.
 */


/**
 * @fn void FragTable::addAll(const C<T> &items);
 * Add a collection of item to the table.
 * @param items	Items to add.
 */


/**
 * @fn void FragTable::remove(const T &item);
 * Remove an item from the table.
 * @param item	Item to remove.
 */


/**
 * @fn void FragTable::remove(const Iterator &iter);
 * Remove an item pointed by an iterator.
 * @param iter	Iterator pointing to the item to remove.
 */


/**
 * @fn void FragTable::removeAll(const C<T> &items);
 * Remove a collection of items.
 * @param items	Items to remove.
 */


/**
 * @fn int FragTable::length(void) const;
 * Get the length / item count of the table.
 * @return	Table length.
 */


/**
 * @fn const T& FragTable::get(int index) const;
 * Get the item at the given index.
 * @warning It is an error to pass an index greater or equal to the table length.
 * @param index	Index of the looked item.
 * @return		Item matching the given index.
 */
 

/**
 * @fn int FragTable::indexOf(const T &value, int start = 0) const;
 * Get the first index of a value in the array.
 * @param value	Value to look for.
 * @param start	Start index to look at (default to 0).
 * @return		Index of the first value, -1 if the value is not found.
 */


/**
 * @fn int FragTable::lastIndexOf(const T &value, int start = -1) const;
 * Get the last index of a value in the array.
 * @param value	Value to look for.
 * @param start	Start index to look back (default to -1 for end of array).
 * @return		Index of the last value, -1 if the value is not found.
 */


/*
 * @fn const T& FragTable::operator[](int index) const;
 * Operator implementing @ref FragTable::get().
 */


/**
 * @fn void FragTable::shrink(int length);
 * Shrink the size of the table.
 * @param length	New length of the table.
 */


/**
 * @fn void FragTable::set(int index, const T& value) const;
 * Change the value of an item in the table.
 * @warn It is an error to pass an index greater or equal to the table length.
 * @param index	Index of the item to set.
 * @param value	Value to set.
 */


/**
 * @fn T& FragTable::get(int index);
 * Same as @ref FragTable::get() const but return a mutable reference.
 * @param index	Index of the looked item.
 * @return		Reference on the matching item.
 */


/**
 * @fn T& FragTable::operator[](int index);
 * Operator implementing @ref FragTable::get().
 */


/**
 * @fn void FragTable::insert(int index, const T &item);
 * Insert an item at the given position and move the following item in the
 * next indexes.
 * @param index	Index to insert to.
 * @param item	Item to insert.
 */


/**
 * @fn void FragTable::insert(const Iterator &iter, const T &item);
 * Insert an item at the given position and move the following item in the
 * next indexes.
 * @param iter	Location to insert to.
 * @param item	Item to insert.
 */


/**
 * @fn void FragTable::removeAt(int index);
 * Remove the item at the given index and shift back following items.
 * @param index	Index of the item to remove.
 */


/**
 * @fn void FragTable::removeAt(const Iterator &iter):
 * Remove the item at the given location and shift back following items.
 * @param iter	Location of the item to remove.
 */


 /**
  * @fn int FragTable<T>::alloc(int count);
  * Allocate the given count of items in sequence in the table and return
  * the index of the first item.
  * @param count	Count of items to allocate.
  * @return			Index of the first item.
  */
 

/**
 * @fn FragTable<T>& FragTable::operator+=(T value);
 * Operator implementing @ref FragTable::add().
 * @deprecated
 */

} // elm
