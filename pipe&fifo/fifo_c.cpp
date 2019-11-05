#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

#define MAX_BUF 1024
#define SERV_FIFO "/tmp/serv.fifo"
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main(int argc, char const *argv[])
{
	char buff[MAX_BUF + 1] = { 0 };
	char fifoname[32] = { 0 };
	pid_t pid = getpid();
	snprintf(fifoname, sizeof(fifoname), "/tmp/fifo.%d", pid);
	if (mkfifo(fifoname, FILE_MODE) < 0 && errno != EEXIST) {
		printf("cant create fifo %s : %s\n", fifoname, strerror(errno));
		_exit(0);
	}
	snprintf(buff, sizeof(buff), "%d ", pid);
	int len = strlen(buff);


	int n;
	if ( (n = read(STDIN_FILENO, buff + len, MAX_BUF - len)) <= 0) {
		printf("bad input\n");
		_exit(0);
	}


	len = strlen(buff);

	int wd;
	if ( (wd = open(SERV_FIFO, O_WRONLY)) < 0) {
		printf("cant open %s : %s\n", SERV_FIFO, strerror(errno));
		_exit(0);
	}

	write(wd, buff, len);

	int rd = open(fifoname, O_RDONLY);

	memset(buff, 0, sizeof(buff));

	while ( (n = read(rd, buff, MAX_BUF)) > 0) {
		write(STDOUT_FILENO, buff, n);
	}

	return 0;
}