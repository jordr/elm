/*
 * $Id$
 * Copyright (c) 2006, IRIT UPS.
 *
 * Serialization of usual collections.
 */
#ifndef ELM_SERIAL2_COLLECTION_H
#define ELM_SERIAL2_COLLECTION_H

#include <elm/assert.h>
#include <elm/serial2/serial.h>
#include <elm/data/Vector.h>
#include <elm/genstruct/Vector.h>
#include <elm/genstruct/Table.h>

namespace elm { namespace serial2 {

template <class T> class Vector;

template <class C, class T>
class CollectionSerializer: public AbstractType {

public:
	static CollectionSerializer __type;

	static inline AbstractType& type(void) { return __type; }
	static inline AbstractType& type(const C& v) { return __type; }

	inline CollectionSerializer(void): AbstractType("Collection") { }
	virtual void *instantiate(void) { return new C(); };

	virtual void unserialize(Unserializer& unserializer, void *object) {
		C &coll = *static_cast<C *>(object);
		int cnt = unserializer.countItems();
		unserializer.beginCompound(&coll);
		for (int i = 0; i < cnt; i++) {
			T item;
			__unserialize(unserializer, item);
			coll.add(item);
		}
		unserializer.endCompound(&coll);
	}

	virtual void serialize(Serializer& serializer, const void *object) {
		const C &coll = *static_cast<const C *>(object);
		serializer.beginCompound(&coll);
		for (typename C::Iter iter(coll); iter; iter++) {
			serializer.onItem();
			__serialize(serializer, *iter);
		}
		serializer.endCompound(&coll);
	}

	static inline void serialize(Serializer& s, const C& v) { serialize(s, &v); }
	static inline void unserialize(Unserializer& s, C& v) { unserialize(s, &v); }
};

template <class C, class T>
CollectionSerializer<C, T> CollectionSerializer<C, T>::__type;

// specialization for Vector
template <class T> struct from_class<Vector<T> >: public CollectionSerializer<Vector<T>, T> { };



/****** Old Serialization ******/

// Table serialization
template <class T>
void __serialize(Serializer& serializer, const genstruct::Table<T>& tab) {
	serializer.beginCompound(&tab);
	for(int i = 0; i < tab.count(); i++) {
		serializer.onItem();
		__serialize(serializer, tab[i]);
	}
	serializer.endCompound(&tab);
}
template <class T>
void __serialize(Serializer& serializer, Table<T>& tab) {
	__serialize(serializer, (const Table<T>&)tab);
}
template <class T>
void __serialize(Serializer& serializer, const genstruct::AllocatedTable<T>& tab) {
	__serialize(serializer, (const Table<T>&)tab);
}
template <class T>
void __serialize(Serializer& serializer, genstruct::AllocatedTable<T>& tab) {
	__serialize(serializer, (const Table<T>&)tab);
}


// Table unserialization
template <class T>
void __unserialize(Unserializer& serializer, genstruct::AllocatedTable<T>& tab) {
	int cnt = serializer.countItems();
	if(!cnt)
		tab.allocate(0);
	else {
		tab.allocate(cnt);
		ASSERT(serializer.beginCompound(&tab));
		for(int i = 0; i < cnt; i++) {
			__unserialize(serializer, tab[i]);
			serializer.nextItem();
		}
		serializer.endCompound(&tab);
	}
}


// Collection serialization

// common template
template <template <class I> class Coll, class T>
class CollecAC: public AbstractType {

public:
	static CollecAC __type;
	
	static inline AbstractType& type(void) { return __type; }
	static inline AbstractType& type(const Coll<T>& v) { return __type; }

	inline CollecAC() : AbstractType("Collection") {
	};
	
	virtual void *instantiate(void) { return new Coll<T>(); }; 
	
	virtual void unserialize(Unserializer& unserializer, void *object) {
	
		Coll<T> &coll = *static_cast<Coll<T> *>(object);
		int cnt = unserializer.countItems();
		unserializer.beginCompound(&coll);
		for (int i = 0; i < cnt; i++) {
			T item;
			__unserialize(unserializer, item);
			coll.add(item);
		}
		unserializer.endCompound(&coll);
	}
	
	virtual void serialize(Serializer& serializer, const void *object) {
		const Coll<T> &coll = *static_cast<const Coll<T> *>(object);
		serializer.beginCompound(&coll);
		
		for (typename Coll<T>::Iterator iter(coll); iter; iter++) {
			serializer.onItem();
			__serialize(serializer, *iter);
		}
		
		serializer.endCompound(&coll);
	}
	
	static inline void serialize(Serializer& s, const Coll<T>& v) { serialize(s, &v); }
	static inline void unserialize(Unserializer& s, Coll<T>& v) { unserialize(s, &v); }
};

template <template <class I> class Coll, class T>
CollecAC<Coll,T> CollecAC<Coll,T>::__type;



// vector handling
template <class T> struct from_class<genstruct::Vector<T> > : public CollecAC<genstruct::Vector, T> {
};

} } // elm::serial2

#endif // ELM_SERIAL2_COLLECTION_H


