/*
 * $Id$
 * Copyright (c) 2007, IRIT - UPS
 *
 * GDBCrashHandler class implementation
 */

#include <elm/debug/GDBCrashHandler.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

namespace elm {
	
/**
 * @class GDBCrashHandler
 * This crash handler calls GDB to debug the current program.
 * 
 * This handler performs only if the ELM_DEBUG environment variable is
 * set to yes.
 * 
 * @warning It is only compiled  if GDB is available and if /proc
 * filesystem is available.
 */


/**
 * Default handler for GDB.
 */
GDBCrashHandler GDBCrashHandler::DEFAULT;


/**
 */
void GDBCrashHandler::handle(void) {
	int main_pid = getpid();
	
	// Test for a tty ?
	if(!isatty(0) || !isatty(1))
		abort();
	
	// Ask use to use GDB ?
	printf("Do you want to start GDB ? [n]\b\b");
	char chr = getchar();
	if(chr != 'y' && chr != 'Y')
		abort(); 
	
	// Make wait process
	int wait_pid = fork();
	if(wait_pid == -1)
		fatal("no fork");
		
	// Main process
	else if(wait_pid != 0) {
		while(waitpid(wait_pid, 0, 0) != wait_pid) ;
		abort();
	}
	
	// Make GDB proces
	wait_pid = getpid();
	int gdb_pid = fork();
	if(gdb_pid == -1)
		fatal("no fork");
		
	// Wait process
	if(gdb_pid != 0)
		pause();
	
	// GDB process
	else {
		char number[16], buf[256], command[64];
		char *args[] = {
			"gdb",
			buf,
			number,
			"-ex",
			command,
			"-ex",
			"c",
			0
		};

		// run in new session
		if(setsid() == -1)
			fatal("cannot create new session !");

		// Build the command
		snprintf(number, 16, "%d", main_pid);
		snprintf(buf, 256, "/proc/%d/exe", main_pid);
		snprintf(command, 64, "shell kill -SIGTERM %d", wait_pid);
	
		// launch the command
		if(execvp("gdb", args) == -1)
			fatal("no gdb");
	} 
}


/**
 */
void GDBCrashHandler::fatal(const char *msg) {
	fprintf(stderr, "FATAL:%s\n", msg);
	exit(1);
}

} // elm