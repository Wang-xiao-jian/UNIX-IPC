LINK := -lrt
 
all : mqcreate mqgetattr mqunlink mqsend mqreceive Mq_notify
	
mqcreate : mq_create.cpp
	g++ $^ -o $@ $(LINK)

mqgetattr : mq_getattr.cpp
	g++ $^ -o $@ $(LINK)

mqunlink : mq_unlink.cpp
	g++ $^ -o $@ $(LINK)

mqsend : mq_send.cpp
	g++ $^ -o $@ $(LINK)

mqreceive : mq_receive.cpp
	g++ $^ -o $@ $(LINK)

Mq_notify : mq_notify.cpp
	g++ $^ -o $@ $(LINK)