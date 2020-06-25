/*
 *	Option class test
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2006, IRIT UPS.
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

#include <elm/options.h>
#include <elm/test.h>

using namespace elm;
using namespace elm::option;

// EnumOption test
typedef enum val_t {
	one,
	two,
	three
} val_t;
EnumOption<int>::value_t vals[] = {
	{ "value", one },
	{ "one", one },
	{ "two", two },
	{ "three", three },
	{ "", one }
};


// manager initialization
static const cstring program = "my-command",
					 copyright = "copyright (c) 2010",
					description = "a test application",
					author = "H. Cassé <casse@irit.fr",
					arg_free = "free argument";
static const Version version(1, 2, 3);

class MyCommand: public Manager {
public:

	MyCommand(void): Manager(Manager::Make(::program, ::version)
		.copyright(::copyright)
		.description(::description)
		.author(::author)
		.free_argument(::arg_free))
	{
	}
};


// options
MyCommand man;

// new style options
ValueOption<string> ns(ValueOption<string>::Make(man).cmd("--ns"));

// old style options
SwitchOption b(SwitchOption::Make(man).cmd("-b").cmd("--boolean").description("boolean test"));
EnumOption<val_t> enum_opt(EnumOption<val_t>::Make(man).cmd("-e").cmd("--enum")
	.val("1", one)
	.val("one", one)
	.val("two", two)
	.val("three", three));
ValueOption<string> s(ValueOption<string>::Make(man).cmd("-s").cmd("--string").description("string test"));
SwitchOption c(SwitchOption::Make(man).cmd("--command"));
SwitchOption sw(SwitchOption::Make(man).cmd("-S").description("switch option"));
ListOption<int> l(ListOption<int>::Make(man).cmd("-l"));


class MyCommand2: public option::Manager {
public:
	MyCommand2(void): option::Manager(Make(::program, ::version)
		.author(::author)
		.copyright(::copyright)
		.description(::description)
		.free_argument(arg_free)),
	c(SwitchOption::Make(this).cmd("command")),
	sw(SwitchOption::Make(this).cmd("-S").description("switch option"))
	{ }

private:
	SwitchOption c;
	SwitchOption sw;
	/*ListOption<int> l;*/
};


// third generation option
class MyCommand3: public option::Manager {
public:
	MyCommand3(void):
		o1(option::Value<string>::Make(this).cmd("--o1")),
		o2(option::Value<int>::Make(this).cmd("--o2"))
		{ }

	option::Value<string> o1;
	option::Value<int> o2;
};


// test_option()
TEST_BEGIN(option)

	try {

		// manager test
		{
			CHECK_EQUAL(man.getProgram(), ::program);
			CHECK_EQUAL(man.getAuthor(), ::author);
			CHECK_EQUAL(man.getDescription(), ::description);
			CHECK_EQUAL(man.getCopyright(), ::copyright);
			CHECK_EQUAL(man.getVersion(), ::version);
			CHECK_EQUAL(man.getFreeArgumentDescription(), ::arg_free);
		}

		// new style value option
		{
			const char *argv[] = { "command", "--ns", "ok", 0 };
			man.parse(3, argv);
			CHECK_EQUAL(*ns,string("ok"));
		}

		// boolean option
		{
			const char *argv[] = { "command", "-b", 0 };
			FAIL_ON_EXCEPTION(OptionException, man.parse(2, argv));
			CHECK_EQUAL(*b, true);
			b = false;
		}
		{
			const char *argv[] = { "command", 0 };
			FAIL_ON_EXCEPTION(OptionException, man.parse(1, argv));
			CHECK_EQUAL(*b, false);
			b = false;
		}
		{
			const char *argv[] = { "command", "--boolean", 0 };
			FAIL_ON_EXCEPTION(OptionException, man.parse(2, argv));
			CHECK_EQUAL(*b, true);
			b = false;
		}

		// string option
		{
			cstring value = "hello, world !";
			const char *argv[] = { "command", "--string", value.chars(), 0 };
			FAIL_ON_EXCEPTION(OptionException, man.parse(3, argv));
			CHECK_EQUAL(*s, string(value));
			s.set("");
		}
		{
			cstring value = "hello, world !";
			const char *argv[] = { "command", "--string=hello, world !", 0 };
			FAIL_ON_EXCEPTION(OptionException, man.parse(2, argv));
			CHECK_EQUAL(*s, string(value));
			s.set("");
		}
		{
			cstring value = "hello, world !";
			const char *argv[] = { "command", "-s", value.chars(), 0 };
			FAIL_ON_EXCEPTION(OptionException, man.parse(3, argv));
			CHECK_EQUAL(*s, string(value));
			s.set("");
		}
		{
			cstring value = "hello, world !";
			const char *argv[] = { "command", "-s=hello, world !", 0 };
			FAIL_ON_EXCEPTION(OptionException, man.parse(2, argv));
			CHECK_EQUAL(*s, string(value));
			s.set("");
		}

		// SwicthOption test
		{
			const char *argv[] = { "command", "-S", 0 };
			FAIL_ON_EXCEPTION(OptionException, man.parse(2, argv));
			CHECK_EQUAL(*sw, true);
		}

		// free command
		{
			const char *argv[] = { "command", "command", 0 };
			FAIL_ON_EXCEPTION(OptionException, man.parse(2, argv));
			CHECK_EQUAL(*c, false);
		}

		// Unknown option
		{
			const char *argv[] = { "command", "--bad", 0 };
			CHECK_EXCEPTION(OptionException, man.parse(2, argv));
		};

		// EnumOption test
		{
			CHECK(enum_opt == one);
			enum_opt = two;
			CHECK(enum_opt == two);
			{
				const char *argv[] = { "command", "-e", "one", 0 };
				man.parse(3, argv);
				CHECK(enum_opt == one);
			}
			{
				const char *argv[] = { "command", "-e", "two", 0 };
				man.parse(3, argv);
				CHECK(enum_opt == two);
			}
			{
				const char *argv[] = { "command", "-e", "four", 0 };
				CHECK_EXCEPTION(OptionException, man.parse(3, argv));
			}
		}

		// multi test
		{
			const char *argv[] = { "command", "-l", "0", "-l", "1", "-l", "2", 0 };
			FAIL_ON_EXCEPTION(OptionException, man.parse(7, argv));
			CHECK_EQUAL(l.count(), 3);
			CHECK_EQUAL(l[0], 0);
			CHECK_EQUAL(l[1], 1);
			CHECK_EQUAL(l[2], 2);
		}

	}
	catch(OptionException& e) {
		__case.failed();
		cerr << "FAILURE: " << e.message() << io::endl;
	}

	{
		const char *argv[] = { "command", "--o1", "ok", "--o2", "666", nullptr };
		MyCommand3 man;
		FAIL_ON_EXCEPTION(OptionException, man.parse(5, argv));
		CHECK_EQUAL(*man.o1, string("ok"));
		CHECK_EQUAL(*man.o2, 666);
	}

TEST_END

