#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main(int argc, char** argv)
{
	int flag = O_RDWR | O_CREAT;

	struct mq_attr attr = { 0 };
	int c;
	while ( -1 != (c = getopt(argc, argv, "em:z:"))) {
		switch (c) {
			case 'e' :
				flag |= O_EXCL;
				break;
			case 'm' :
				attr.mq_maxmsg = atol(optarg);
				break;
			case 'z' :
				attr.mq_msgsize = atol(optarg);
				break;
		}
	}

	if (optind != argc -1) {
		printf("usage : mqcreate [ -e ] [-m maxmsg -z msgsize] <name>\n");
		_exit(0);
	}

	if ((attr.mq_maxmsg == 0 && attr.mq_msgsize != 0) ||
		(attr.mq_maxmsg != 0 && attr.mq_msgsize == 0)) {
		printf("must specify both -m maxmsg and -z msgsize\n");
		_exit(0);
	}

	mqd_t mqd = mq_open(argv[optind], flag, FILE_MODE, (attr.mq_maxmsg == 0) ? NULL : &attr);

	if (-1 == mqd) {
		printf("open error\n");
		printf("code : %d\n", errno);
	}
	mq_close(mqd);
	return 0;
}