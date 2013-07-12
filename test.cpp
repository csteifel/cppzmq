#include <zmq.hpp>
#include <iostream>
#include <cstring>

int main(){

	int parts = 0;

	zmq::context_t context(1);

	zmq::socket_t sideOne(context, ZMQ_PAIR);
	zmq::socket_t sideTwo(context, ZMQ_PAIR);

	sideOne.connect("ipc://localhost:2335");
	sideTwo.bind("ipc://localhost:2335");


	zmq::message_t msgOne(5);
	strncpy((char *) msgOne.data(), "hello", 5);

	zmq::message_t msgTwo(5);
	strncpy((char *) msgTwo.data(), "world", 5);	


	sideOne.send(msgOne, ZMQ_SNDMORE);
	sideOne.send(msgTwo, 0);


	while(true){
		zmq::message_t recvMsg;
		if(sideTwo.recv(&recvMsg)){
			std::cout << (char *) recvMsg.data() << std::endl;
		}

		parts++;

		if(recvMsg.more() == false){
			break;
		}

	}

	if(parts != 2){
		return 1;
	}



	zmq::message_t msgThree(5);
	strncpy((char *) msgThree.data(), "hi", 2);

	zmq::message_t msgFour(5);
	strncpy((char *) msgFour.data(), "there", 5);	


	sideOne.send(msgThree, 0);
	sideOne.send(msgFour, 0);

	parts = 0;
	while(true){
		zmq::message_t recvMsg;
		if(sideTwo.recv(&recvMsg)){
			std::cout << (char *) recvMsg.data() << std::endl;
		}

		parts++;

		if(recvMsg.more() == false){
			break;
		}
	}



	if(parts != 1){
		return 1;
	}


	return 0;
}