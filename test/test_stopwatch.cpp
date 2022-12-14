/*
 * test_stopwatch.cpp
 *
 *  Created on: 5 févr. 2013
 *      Author: casse
 */

#include <elm/sys/StopWatch.h>
#include <unistd.h>
#include "../include/elm/test.h"

using namespace elm;

TEST_BEGIN(stopwatch)

	sys::StopWatch sw;
	sw.start();

	static const int n = 10000;
	static int t[n];
	for(int i = 0; i < 10000; i++)
		for(int j = 0; j < n - 1; j++)
			t[j + 1] = t[j] + t[j + 1];

	sw.stop();
	cout << "delay = " << sw.delay() << io::endl;
	CHECK(sw.delay() != 0);

TEST_END
