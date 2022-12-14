/*
 *	$Id$
 *	SwitchOption class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2004-09, IRIT UPS.
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

#include <elm/option/Manager.h>
#include <elm/option/SwitchOption.h>

namespace elm { namespace option {

/**
 * @class SwitchOption
 * An option to handle simple switch option (boolean).
 * @ingroup options
 */

/**
 */
usage_t SwitchOption::usage(void) {
	return option::arg_none;
}


/**
 */
cstring SwitchOption::argDescription(void) {
	return "";
}


/**
 */
void SwitchOption::process(string arg) {
	val = true;
}


/**
 * Build a switch option.
 * @param make	Make information.
 */
SwitchOption::SwitchOption(const Make& make): Option(make), val(false) {
}

} } // otawa::option

