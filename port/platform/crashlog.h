/* The program's log, and what it says when the game crashes or sticks (crashlog.c; docs/PORT.md, The log). */
#ifndef CRASHLOG_H
#define CRASHLOG_H

/* A player's run: the output goes to log.txt in FOLDER (ending in /), the last run's kept as log-previous.txt, unless it's been sent to
   a file or a pipe already, as `crashbash > out.txt` does. A test run (testing) keeps its output where it is. */
void crashlog_open(const char *folder, int testing);
/* log.txt's path, or NULL if the output isn't going there */
const char *crashlog_path(void);
/* The first lines: the version and build, and the machine it's on */
void crashlog_system(void);
/* Catches a crash, to say what and where in the log, and on a player's run, in a message too */
void crashlog_install(void);
/* The time of day, for a line of the log */
void crashlog_time(char *out, int size);
#ifdef _WIN32
/* Where another thread is (a stuck game's, for the watchdog): its stack, as a crash's is */
void crashlog_thread_stack(void *thread);
#endif

#endif
