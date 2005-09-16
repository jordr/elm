/*
 * $Id$
 * Copyright (c) 2005, IRIT-UPS.
 *
 * test/test_vararg.cpp -- unit tests for elm/util/VarArg.h.
 */

#include <elm/util/test.h>
#include <elm/util/VarArg.h>

using namespace elm;

// Test function
static void perform(int x, ...) {
	CHECK_BEGIN("VarArg 1");

	VARARG_BEGIN(args, x)
		CHECK(args.next<bool>() == true);
		CHECK(args.next<int>() == 666);
		CHECK(args.next<double>() == 111.666);
		CHECK(!strcmp(args.next<char *>(), "123"));
		CHECK(args.next<unsigned int>() == 111);
		CHECK(!strcmp(args.next<char *>(), "456"));
	VARARG_END

	CHECK_END;	
}

// Test function 2
static void perform22(int x, VarArg& args) {
	CHECK_BEGIN("VarArg 2");

	CHECK(args.next<bool>() == true);
	CHECK(args.next<int>() == 666);
	CHECK(args.next<double>() == 111.666);
	CHECK(!strcmp(args.next<char *>(), "123"));
	CHECK(args.next<unsigned int>() == 111);
	CHECK(!strcmp(args.next<char *>(), "456"));

	CHECK_END;	
}
static void perform21(int x, ...) {
	VARARG_BEGIN(args, x)
		perform22(x, args);
	VARARG_END
}


// Test function 3
static void perform32(int x, VarArg args) {
	CHECK_BEGIN("VarArg 3");

	CHECK(args.next<bool>() == true);
	CHECK(args.next<int>() == 666);
	CHECK(args.next<double>() == 111.666);
	CHECK(!strcmp(args.next<char *>(), "123"));
	CHECK(args.next<unsigned int>() == 111);
	CHECK(!strcmp(args.next<char *>(), "456"));

	CHECK_END;	
}
static void perform31(int x, ...) {
	VARARG_BEGIN(args, x)
		perform32(x, args);
	VARARG_END
}


// Test function 4
static void perform43(int x, VarArg& args) {
	CHECK_BEGIN("VarArg 4");

	CHECK(args.next<bool>() == true);
	CHECK(args.next<int>() == 666);
	CHECK(args.next<double>() == 111.666);
	CHECK(!strcmp(args.next<char *>(), "123"));
	CHECK(args.next<unsigned int>() == 111);
	CHECK(!strcmp(args.next<char *>(), "456"));

	CHECK_END;	
}
static void perform42(int x, VarArg& args) {
	perform43(x, args);
}
static void perform41(int x, ...) {
	VARARG_BEGIN(args, x)
		perform42(x, args);
	VARARG_END
}


// Utility tests
void test_vararg(void) {
	String s("456");
	perform(0, true, 666, 111.666, "123", 111, &s);
	perform21(0, true, 666, 111.666, "123", 111, &s);
	perform31(0, true, 666, 111.666, "123", 111, &s);
	perform41(0, true, 666, 111.666, "123", 111, &s);
}
