#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main(int argc, char const *argv[])
{
	if (2 != argc) {
		printf("usage : mqgetattr <name>\n");
		_exit(0);
	}

	mqd_t mqd = mq_open(argv[1], O_RDONLY);
	struct mq_attr myattr;
	mq_getattr(mqd, &myattr);

	printf("max #msg = %ld, max #bytes/msg = %ld, "
		"#currently on queue = %ld\n", 
		myattr.mq_maxmsg, myattr.mq_msgsize,
		myattr.mq_curmsgs);

	return 0;
}