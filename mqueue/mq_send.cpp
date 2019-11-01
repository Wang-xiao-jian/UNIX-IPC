#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	mqd_t mqd;
	void* ptr;
	size_t len;
	uint prio;
	if (argc != 4) {
		printf("usage : mqsend <name> <#byte> <priority>\n");
		_exit(0);
	}

	len = atoi(argv[2]);
	prio = atoi(argv[3]);
	mqd = mq_open(argv[1], O_WRONLY);

	ptr = calloc(len, sizeof(char));
	mq_send(mqd, (char *)ptr, len, prio);
	return 0;
}