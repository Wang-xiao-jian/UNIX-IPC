LINK := -lrt
 
all : mqcreate mqgetattr mqunlink mqsend mqreceive
	
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