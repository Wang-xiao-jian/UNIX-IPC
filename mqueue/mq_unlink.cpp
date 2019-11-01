#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>
#include <stdio.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main(int argc, char const *argv[])
{
	if (2 != argc) {
		printf("usage : mqunlink <name>\n");
		_exit(0);
	}

	mq_unlink(argv[1]);
	return 0;
}