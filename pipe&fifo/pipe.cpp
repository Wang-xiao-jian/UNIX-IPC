#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#define MAX_BUF 1024

void server(int, int);
void client(int, int);

int main()
{
	int pipe1[2], pipe2[2];
	pipe(pipe1);
	pipe(pipe2);	
	pid_t child_pid;
	if (0 == (child_pid = fork())) {
		close(pipe1[0]);
		close(pipe2[1]);

		server(pipe2[0], pipe1[1]);
		_exit(0);
	}

	close(pipe1[1]);
	close(pipe2[0]);
	client(pipe1[0], pipe2[1]);

	waitpid(child_pid, NULL, 0);

	return 0;
}

void server(int rd, int wd)
{
	char buff[MAX_BUF + 1] = { 0 };
	int n;
	
	if ( (n = read(rd, buff, MAX_BUF)) == 0) {
		printf("EOF while reading pathname\n");
		_exit(0);
	}

	buff[n - 1] = '\0';
	int fd;

	if ( (fd = open(buff, O_RDONLY)) < 0) {
		snprintf(buff + n, sizeof(buff) - n, ": open error, %s\n", strerror(errno));
		write(wd, buff, strlen(buff));
		_exit(0);
	} else {
		memset(buff, 0, sizeof(buff));
		while ( (n = read(fd, buff, MAX_BUF)) > 0) {
			write(wd, buff, strlen(buff));
			memset(buff, 0, sizeof(buff));
		}
	}
	close(fd);
}

void client(int rd, int wd)
{
	char buff[MAX_BUF + 1] = { 0 };
	fgets(buff, MAX_BUF, stdin);

	write(wd, buff, strlen(buff));
	int n;
	memset(buff, 0, sizeof(buff));
	while ( (n = read(rd, buff, MAX_BUF)) > 0) {
		write(STDOUT_FILENO, buff, n);
		memset(buff, 0, sizeof(buff));
	}
}