#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	int c, flags;
	mqd_t mqd;
	ssize_t n;
	uint prio;
	void* buff;
	struct mq_attr attr;
	flags = O_RDONLY;
	while (-1 != (c = getopt(argc, argv, "n"))) {
		switch (c) {
			case 'n' :
				flags |= O_NONBLOCK;
				break;
		}
	}

	if (optind != argc - 1) {
		printf("usage : mqreceive [ -n ] <name>\n");
		_exit(0);
	}

	mqd = mq_open(argv[optind], flags);
	mq_getattr(mqd, &attr);

	buff = malloc(attr.mq_msgsize);

	n = mq_receive(mqd, (char *)buff, attr.mq_msgsize, &prio);
	printf("read %ld bytes, priority = %u\n", n, prio);
	return 0;
}