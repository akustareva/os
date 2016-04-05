#include <stdio.h>
#include <fcntl.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

int const SIZE = 1024;
std::vector<pid_t> all;

void handler(int signal, siginfo_t* info, void* h) {
	for (int i = 0; i < all.size(); i++) {
			kill(all[i], signal);
	}	
}

int main(int argc, char* argv[]) {
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_sigaction = handler;
	act.sa_flags = SA_SIGINFO | SA_NODEFER;
	if (sigaction(SIGINT, &act, 0) < 0) {
		return 0;
	}	
		
	while (true) {
		std::string command;
		int rec_len;
		char buf[SIZE];
		printf("\n%s\n", "$");
		while ((rec_len = read(0, buf, SIZE)) > 0) {
			bool end = false;
			for (int i = 0; i < rec_len; i++) {
				if (buf[i] == '\n') {
					if (i + 1 < rec_len) {
						for (int j = i + 1; j < rec_len; j++) {
							printf("%c", buf[j]);
						}	
						printf("\n");
					}
					rec_len = i;
					end = true;
					break;
				}
			}	
			command.append(buf, rec_len);
			if (end) {
				break;
			}	
		}
		if (rec_len == -1 || rec_len == 0) {
			break;
		}	
		int left_pipe[2];
		int right_pipe[2];
		int pos = 0;
		int num = 0;
		while (pos != std::string::npos) {
			pos = command.find("|");
			std::string cmd;
			cmd	= command.substr(0, pos == std::string::npos ? command.length() : pos);
			command = pos == std::string::npos ? "" : command.substr(pos + 2);
			
			bool name = true;
			std::string prog_name, args;
			for (int i = 0; i < cmd.length(); i++) {
				if (cmd[i] == ' ') {
					if (name) {
						name = false;
						continue;
					}
				}	
				if (name) {
					prog_name += cmd[i];
				} else {
					args += cmd[i];
				}	
			}
			std::vector<std::string> strArgs;
			std::string arg;
			for (int i = 0; i < args.length(); i++) {
				if (args[i] == ' ') {
					strArgs.push_back(arg);
					arg = "";
				} else {
					arg += args[i];
				}	
			}
			if (arg.length() > 0) {
					strArgs.push_back(arg);
			}
			const char* str2Args[strArgs.size() + 2];
			str2Args[0] = prog_name.c_str();
			for (int i = 0; i < strArgs.size(); i++) {
				str2Args[i + 1] = strArgs[i].c_str();
			}
			str2Args[strArgs.size() + 1] = NULL;
			
			pipe(right_pipe);
			pid_t nw = fork();
			all.push_back(nw);
			if (!nw) { //child
				if (num) {
					dup2(left_pipe[0], 0);
					close(left_pipe[0]);
					close(left_pipe[1]);
				}	
				if (pos != -1) {
					dup2(right_pipe[1], 1);
				}
				close(right_pipe[0]);
				close(right_pipe[1]);
				execvp(prog_name.c_str(), (char* const*)str2Args);
				perror("error in execvp");
			} else { //parent
				if (num) {
					close(left_pipe[0]);
					close(left_pipe[1]);
				}
				if (pos == -1) {	
					close(right_pipe[0]);
					close(right_pipe[1]);
				}	
			}
			left_pipe[0] = right_pipe[0];
			left_pipe[1] = right_pipe[1];
			num++;	
		}
		pid_t wpid;
		int status;
		while ((wpid = wait(&status)) > 0) {}	
	}
	return 0;
}