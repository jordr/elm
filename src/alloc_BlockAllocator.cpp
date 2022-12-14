/*
 *	$Id$
 *	BlockAllocator class interface
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

#include <elm/types.h>
#include <elm/alloc/BlockAllocator.h>
#include <elm/compare.h>

namespace elm {

/**
 * @class BlockAllocator
 * An allocator for fixed-size block. Initial allocation is performed
 * quickly in a stack allocator but fried blocks are added to a free list
 * allowing quick re-use of fried blocks.
 *
 * @ingroup alloc
 */


/**
 * @fn BlockAllocator::BlockAllocator(int block_per_chunk);
 * Block allocator builder.
 * @param block_per_chunk	Number of block in each allocated chunk.
 * @param T					Type of managed blocks.
 */

/**
 * @fn T *BlockAllocator::allocate(void);
 * Allocate an object.
 * @return	Allocated object.
 */


/**
 * @fn void BlockAllocator::free(void *block);
 * Free a previously allocated object.
 * @param block	Block to free.
 */

} // elm
