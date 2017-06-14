/*
 *	Version class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2005-17, IRIT UPS.
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
#ifndef ELM_UTIL_VARIANT_H_
#define ELM_UTIL_VARIANT_H_

#include <elm/types.h>

namespace elm {

class Variant {
public:
	inline Variant(bool v) { _data.b = v; }
	inline Variant(t::int8 v) { _data.i8 = v; }
	inline Variant(t::int16 v) { _data.i16 = v; }
	inline Variant(t::int32 v) { _data.i32 = v; }
	inline Variant(t::int64 v) { _data.i64 = v; }
	inline Variant(t::uint8 v) { _data.u8 = v; }
	inline Variant(t::uint16 v) { _data.u16 = v; }
	inline Variant(t::uint32 v) { _data.u32 = v; }
	inline Variant(t::uint64 v) { _data.u64 = v; }
	inline Variant(float v) { _data.f = v; }
	inline Variant(double v) { _data.d = v; }
	inline Variant(const char *v) { _data.s = v; }
	inline Variant(void *v) { _data.p = v; }

	inline bool asBool(void) const { return _data.b; }
	inline int asInt(void) const { return asInt32(); }
	inline t::int8 asInt8(void) const { return _data.i8; }
	inline t::int8 asInt16(void) const { return _data.i16; }
	inline t::int8 asInt32(void) const { return _data.i32; }
	inline t::int8 asInt64(void) const { return _data.i64; }
	inline t::uint8 asUInt8(void) const { return _data.i8; }
	inline t::uint8 asUInt16(void) const { return _data.i16; }
	inline t::uint8 asUInt32(void) const { return _data.i32; }
	inline t::uint8 asUInt64(void) const { return _data.i64; }
	inline float asFloat(void) const { return _data.f; }
	inline float asDouble(void) const { return _data.d; }
	inline cstring asString(void) const { return _data.s; }
	template <class T> inline T *as(void) const { return static_cast<T *>(_data.p); }

private:
	union {
		bool b;
		t::int8 i8;
		t::int16 i16;
		t::int32 i32;
		t::int64 i64;
		t::uint8 u8;
		t::uint16 u16;
		t::uint32 u32;
		t::uint64 u64;
		float f;
		double d;
		const char *s;
		void *p;
	} _data;
};	// Variant

}	// elm

#endif /* ELM_UTIL_VARIANT_H_ */
