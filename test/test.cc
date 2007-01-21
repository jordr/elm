/*
 * $Id$
 * Copyright (c) 2003-07 IRIT-UPS <casse@irit.fr>
 *
 * Unit tests
 */

#include <elm/io.h>

using namespace elm;

// Prototypes
void test_avl(void);
void test_bitvector(void);
void test_dllist(void);
void test_file(void);
void test_formatter(void);
void test_frag_table(void);
void test_hashtable(void);
void test_io_format(void);
void test_io_output(void);
void test_lock(void);
void test_option(void);
void test_plugin(void);
void test_process(void);
void test_ref(void);
void test_sllist(void);
void test_sorted_bintree(void);
void test_string(void);
void test_string_buffer(void);
void test_table(void);
void test_utility(void);
void test_vararg(void);
void test_vector(void);
void test_vector_queue(void);
#ifdef HAS_LIBXML
	void test_serial(void);
	void test_xom(void);
#endif

// List of tests
struct test_t {
	CString name;
	void (*fun)(void);
} tests[] = {
	{ "avl", test_avl },
	{ "bitvector", test_bitvector },
	{ "dllist", test_dllist },
	{ "file", test_file },
	{ "formatter", test_formatter },
	{ "hashtable", test_hashtable },
	{ "io_format", test_io_format },
	{ "io_output", test_io_output },
	{ "lock", test_lock },
	{ "option", test_option },
	{ "process", test_process },
#	ifdef IS_SHARED
		{ "plugin", test_plugin },
#	endif
	{ "ref", test_ref },
	{ "sllist", test_sllist },
/*	{ "sorted_bintree", test_sorted_bintree }, */
	{ "string", test_string },
	{ "string_buffer", test_string_buffer },
	{ "table", test_table },
	{ "frag_table", test_frag_table },
	{ "utility", test_utility },
	{ "vector", test_vector },	
	{ "vararg", test_vararg },
	{ "vector_queue", test_vector_queue },
	#ifdef HAS_LIBXML
		{ "serial", test_serial },
		{ "xom", test_xom },
	#endif
	{ "", 0 }
};

// Entry point
int main(int argc, char *argv[]) {
	
	// Test all
	if(argc == 1)
		for(struct test_t *test = tests; test->fun; test++)
			test->fun();
	
	// Test some
	else 
		for(int i = 1; i < argc; i++) {
			bool found = false;
			for(struct test_t *test = tests; test->fun; test++)
				if(test->name == argv[i]) {
					test->fun();
					found = true;
				}
			if(!found)
				cerr << "ERROR: no test called \"" << argv[i] << "\"\n";
		}
}
