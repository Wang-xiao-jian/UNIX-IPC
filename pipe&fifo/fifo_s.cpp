#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_BUF 1024
#define SERV_FIFO "/tmp/serv.fifo"
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main(int argc, char const *argv[])
{
	if (mkfifo(SERV_FIFO, FILE_MODE) < 0 && errno != EEXIST) {
		printf("cant create fifo : %s\n", SERV_FIFO);
		_exit(0);
	}

	int readfd = open(SERV_FIFO, O_RDONLY);
	int yummyfd = open(SERV_FIFO, O_WRONLY);	//这样没有客户端进程以只写打开serv_fifo时也不用关闭readfd重新打开

	int n;
	char buff[MAX_BUF + 1] = { 0 };
	int child_pid;
	while ( (n = read(readfd, buff, MAX_BUF)) > 0) {
		if (buff[n - 1] == '\n') 
			--n;
		buff[n] = '\0';
		
		if ( (child_pid = fork()) == 0) {
			close(yummyfd);
			close(readfd);
			char* ptr;
			if ( (ptr = strchr(buff, ' ')) == NULL) {
				printf("bad request : %s\n", buff);
				_exit(0);
			}
			*ptr++ = 0;	//buff = pid, ptr = pathname

			char fifoname[32] = { 0 };
			snprintf(fifoname, sizeof(fifoname), "/tmp/fifo.%ld", atol(buff));
			int wd;
			if ( (wd = open(fifoname, O_WRONLY)) < 0) {
				printf("cant open fifo : %s\n", fifoname);
				_exit(0);
			}

			int fd;
			if ( (fd = open(ptr, O_RDONLY)) < 0) {
				printf("cant open %s : %s\n", ptr, strerror(errno));
				snprintf(buff + n, sizeof(buff) - n, "cant open : %s", strerror(errno));
				n = strlen(ptr);
				write(wd, ptr, n);
				close(wd);
			} else {
				while ( (n = read(fd, buff, MAX_BUF)) > 0) {
					write(wd, buff, n);
				}
				close(wd);
				close(fd);
			}
			_exit(0);
		}
		
		waitpid(child_pid, NULL, 0);
	}

	return 0;
}