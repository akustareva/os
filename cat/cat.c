#include <stdio.h>
#include <string.h>
#include <errno.h>

int const SIZE = 1024;

int main() {
	int rec_len;
	char buf[SIZE];
	while ((rec_len = read(0, buf, SIZE)) > 0) {
		int send_len = 0, tmp = 0;
		while ((tmp = write(1, buf + send_len, rec_len - send_len)) != 0 && send_len < rec_len) {
			if (tmp == -1) {
				printf("%s", strerror(errno));
			}
			send_len += tmp;
		}
	}
	if (rec_len == -1) {
		printf("%s", strerror(errno));
	}
	return 0;
}
