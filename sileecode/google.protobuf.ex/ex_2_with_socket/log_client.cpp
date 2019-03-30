#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <iostream>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/io/coded_stream.h>
#include "log_packet.pb.h"

using namespace google::protobuf::io;

using namespace std;
int main(int argv, char** argc){

	/* Coded output stram */

	log_packet payload ;

	payload.set_log_time(10);
	payload.set_log_micro_sec(10);
	payload.set_sequence_no(1);
	payload.set_shm_app_id(101);
	payload.set_packet_id("TST");
	payload.set_log_level("DEBUG");
	payload.set_log_msg("What shall we say then");

	cout<<"size after serilizing is "<<payload.ByteSize()<<endl;
	int siz = payload.ByteSize()+4;
	char *pkt = new char [siz];
	//google::protobuf::io::ArrayOutputStream aos(pkt,siz);
	ArrayOutputStream aos(pkt,siz);
	CodedOutputStream *coded_output = new CodedOutputStream(&aos);
	coded_output->WriteVarint32(payload.ByteSize());
	payload.SerializeToCodedStream(coded_output);

	int host_port= 1101;
	const char* host_name="127.0.0.1";

	struct sockaddr_in my_addr;

	char buffer[1024];
	int bytecount;
	int buffer_len=0;

	int hsock;
	int * p_int;
	int err;

	hsock = socket(AF_INET, SOCK_STREAM, 0);
	if(hsock == -1){
		printf("Error initializing socket %d\n",errno);
		goto FINISH;
	}

	p_int = (int*)malloc(sizeof(int));
	*p_int = 1;

	if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
			(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
		printf("Error setting options %d\n",errno);
		free(p_int);
		goto FINISH;
	}
	free(p_int);

	my_addr.sin_family = AF_INET ;
	my_addr.sin_port = htons(host_port);

	memset(&(my_addr.sin_zero), 0, 8);
	my_addr.sin_addr.s_addr = inet_addr(host_name);
	if( connect( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
		if((err = errno) != EINPROGRESS){
			fprintf(stderr, "Error connecting socket %d\n", errno);
			goto FINISH;
		}
	}




	for (int i =0;i<10000;i++){
		for (int j = 0 ;j<10;j++) {

			if( (bytecount=send(hsock, (void *) pkt,siz,0))== -1 ) {
				fprintf(stderr, "Error sending data %d\n", errno);
				goto FINISH;
			}
			printf("Sent bytes %d\n", bytecount);
			usleep(1);
		}
	}
	delete pkt;

FINISH:
	close(hsock);

}
