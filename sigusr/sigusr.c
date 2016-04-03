#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void handler(int signal, siginfo_t* info, void* h) {
	int num = signal == SIGUSR1 ? 1 : 2;
	printf("SIGUSR%d from %d\n", num, info->si_pid);
}	

int main() {
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_sigaction = handler;
	act.sa_flags = SA_SIGINFO | SA_NODEFER;
	sigset_t set;
	sigemptyset(&set);
	act.sa_mask = set;
	sigaction(SIGUSR1, &act, 0);
	sigaction(SIGUSR2, &act, 0);
	if (sleep(10) == 0) {
		printf("No signals were caught\n");
	}	
}	