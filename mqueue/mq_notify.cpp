#include <cstdio>
#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>
#include <cstdlib>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("usage : Mq_notify <name>\n");
		_exit(0);
	}

	mqd_t mqd = mq_open(argv[1], O_RDONLY | O_NONBLOCK);
	struct mq_attr attr;
	mq_getattr(mqd, &attr);
	void *buff = malloc(attr.mq_msgsize);

	sigset_t newset;
	sigemptyset(&newset);
	sigaddset(&newset, SIGUSR1);
	sigprocmask(SIG_BLOCK, &newset, NULL);
	struct sigevent sigev;
	sigev.sigev_notify = SIGEV_SIGNAL;
	sigev.sigev_signo = SIGUSR1;
	mq_notify(mqd, &sigev);

	int signo;
	ssize_t n;
	while (1) {
		sigwait(&newset, &signo);
		if (SIGUSR1 == signo) {
			mq_notify(mqd, &sigev);
			while ((n = mq_receive(mqd, (char *)buff, attr.mq_msgsize, NULL)) > 0) {
				printf("read %ld bytes\n", (long)n);
			}

			if (errno != EAGAIN) {
				printf("receive error!\n");
				_exit(0);
			}
		}
	}
	return 0;
}