#define _POSIX_SOURCE
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static pid_t cpid = -1;

static void usage(const char *pname)
{
	fprintf(stderr, "Usage: %s TIMEOUT CMD [OPTS]...\n\n", pname);
	fprintf(stderr, " TIMEOUT: minutes to wait before restarting the program\n");
	fprintf(stderr, " CMD: program to execute\n");
	fprintf(stderr, " OPTS: command line arguments to the program\n");
	exit(EXIT_FAILURE);
}

static void start_child_process(const char *file, char *const argv[])
{
	cpid = fork();
	switch(cpid) {
	case 0:
		if (execvp(file, argv) == -1) {
			perror("execvp");
			exit(EXIT_FAILURE);
		}
		break;
	case -1:
		perror("fork");
		break;
	default:
		break;
	}
}

static void kill_child(void)
{
	if (cpid != -1 && kill(cpid, 0) == 0 && kill(cpid, SIGTERM) == -1)
		perror("kill");
}

static void sig_handler(int signum)
{
	if (signum == SIGCHLD) {
		wait(NULL);
		cpid = -1;
	}

	if (signum == SIGTERM) {
		kill_child();
		exit(EXIT_SUCCESS);
	}
}

int main(int argc, char *argv[])
{
	struct sigaction action;
	unsigned long long int mins = 0;

	if (argc < 3) {
		usage(argv[0]);
	} else {
		char *err = NULL;
		mins = strtoull(argv[1], &err, 10);
		if ((err != NULL && *err != '\0'))
			usage(argv[0]);
		if (errno == ERANGE) {
			fprintf(stderr, "Please enter an integer < %llu\n", ULLONG_MAX);
			exit(EXIT_FAILURE);
		}
	}

	action.sa_handler = sig_handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;

	/* Set signal handler for SIGCHLD and SIGTERM */
	sigaction(SIGCHLD, &action, NULL);
	sigaction(SIGTERM, &action, NULL);

	while (1) {
		/* Wait until old child is done with */
		while (cpid != -1) {
			sleep(5);
		}

		start_child_process(argv[2], &argv[2]);

		for (unsigned long long int i = 0; i < mins; i++) {
			/* Restart the timer if program
			 * terminated before timeout */
			int t = 60;
			while ((t = sleep(t)) != 0);
		}

		kill_child();
	}

	return 0;
}
