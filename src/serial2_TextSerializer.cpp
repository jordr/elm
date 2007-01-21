/*
 * $Id$
 * Copyright (c) 2006, IRIT UPS.
 *
 * TextSerializer class implementation.
 */

#include <elm/io.h>
#include <elm/serial2/serial.h>
#include <elm/serial2/TextSerializer.h>

namespace elm { namespace serial2 {

/**
 * @class TextSerializer
 * Serialize the given data to a character output stream, readable by
 * human being.
 */


/**
 * Display prefix information.
 * @param type		Type of the object.
 * @param object	Current object.
 */
void TextSerializer::prefix(AbstractType& type, const void *object) {
	cout << '*' << io::pointer(object);
	objects.put(object, true);
	if(!level)
		cout << ':' << type.name();
	cout << '=';
	level++;
}


/**
 * Display suffix information.
 */
void TextSerializer::suffix(void) {
	level--;
}


/**
 */
void TextSerializer::flush(void) {
	while(to_process) {
		delay_t delayed = to_process.get();
		if(!objects.get(delayed.fst, false))
			delayed.snd->serialize(*this, delayed.fst);
	}
	objects.clear();
}


/**
 */
void TextSerializer::beginObject(AbstractType& type, const void *object) {
	prefix(type, object);
	cout << "{";
}


/**
 */
void TextSerializer::endObject(void) {
	suffix();
	cout << "}";
}


/**
 */
void TextSerializer::beginField(CString name) {
	cout << name << "->";
}


/**
 */
void TextSerializer::endField(void) {
}


/**
 */
void TextSerializer::onPointer(AbstractType& type, const void *object) {
	cout << "&" << io::pointer(object) << ';';
	if(!objects.exists(object)) {
		to_process.put(pair(object, &type));
		objects.put(object, false);
	}
}


/**
 */
void TextSerializer::onValue(const bool& v) {
	prefix(type<bool>(), &v);
	cout << (v ? "true" : "false") << ';';
	suffix();
}


/**
 */
void TextSerializer::onValue(const signed int& v) {
	prefix(type<signed int>(), &v);
	cout << v << ';';
	suffix();
}


/**
 */
void TextSerializer::onValue(const unsigned int& v) {
	prefix(type<unsigned int>(), &v);
	cout << v << ';';
	suffix();
}


/**
 */
void TextSerializer::onValue(const signed char& v) {
	prefix(type<signed char>(), &v);
	cout << (signed int)v << ';';
	suffix();
}


/**
 */
void TextSerializer::onValue(const unsigned char& v) {
	prefix(type<unsigned char>(), &v);
	cout << (unsigned int)v << ';';
	suffix();
}


/**
 */
void TextSerializer::onValue(const signed short& v) {
	prefix(type<signed short>(), &v);
	cout << v << ';';
	suffix();
}


/**
 */
void TextSerializer::onValue(const unsigned short& v) {
	prefix(type<unsigned short>(), &v);
	cout << v << ';';
	suffix();
}


/**
 */
void TextSerializer::onValue(const signed long& v) {
	prefix(type<signed long>(), &v);
	cout << v << ';';
	suffix();
}


/**
 */
void TextSerializer::onValue(const unsigned long& v) {
	prefix(type<unsigned long>(), &v);
	cout << v << ';';
	suffix();
}


/**
 */
void TextSerializer::onValue(const signed long long& v) {
	prefix(type<signed long long>(), &v);
	cout << v << ';';
	suffix();
}


/**
 */
void TextSerializer::onValue(const unsigned long long& v) {
	prefix(type<unsigned long long>(), &v);
	cout << v << ';';
	suffix();
}


/**
 */
void TextSerializer::onValue(const float& v) {
	prefix(type<float>(), &v);
	cout << v << ';';
	suffix();
}


/**
 */
void TextSerializer::onValue(const double& v) {
	prefix(type<double>(), &v);
	cout << v << ';';
	suffix();
}


/**
 */
void TextSerializer::onValue(const long double& v) {
	prefix(type<long double>(), &v);
	cout << (double)v << ';';
	suffix();
}


/**
 */
void TextSerializer::onValue(const CString& v) {
	prefix(type<signed int>(), &v);
	cout << '"' << v << "\";";
	suffix();
}


/**
 */
void TextSerializer::onValue(const String& v) {
	prefix(type<signed int>(), &v);
	cout << '"' << v << "\";";
	suffix();
}


/**
 */
void TextSerializer::onEnum(const void *address, int value, AbstractEnum& clazz)
{
	prefix(clazz, address);
	CString name = clazz.nameOf(value);
	assert(name);
	cout << name;
	suffix(); 
}


/**
 */
void TextSerializer::beginCompound(void) {
	level++;
	cout << '{';
}


/**
 */
void TextSerializer::onItem(void) {
}


/**
 */
void TextSerializer::endCompound(void) {
	cout << '}';
	level--;
}

} } // elm::serial2