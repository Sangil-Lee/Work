# See README.txt.

.PHONY: all cpp  clean

all: cpp 

cpp:    log_client   log_server

clean:
	rm -f log_client log_server
	rm -f protoc_middleman log_packet.pb.cc log_packet.pb.h

protoc_middleman: log_packet.proto
	protoc --cpp_out=. log_packet.proto
	@touch protoc_middleman

log_client: log_client.cpp protoc_middleman
	pkg-config --cflags protobuf  # fails if protobuf is not installed
	c++ log_client.cpp log_packet.pb.cc -o log_client `pkg-config --cflags --libs protobuf`

log_server: log_server.cpp protoc_middleman
	pkg-config --cflags protobuf  # fails if protobuf is not installed
	c++ log_server.cpp log_packet.pb.cc -o log_server `pkg-config --cflags --libs protobuf`
