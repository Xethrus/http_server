#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string>
#include <arpa/inet.h>
#include <ostream>
#include <cstring>
//macro
#define PORT 8080
class Request {
	private:
		std::string the_request;
	public:
		std::string get_request() {
			return the_request; 
		}
		void set_request(std::string new_request) {
			the_request = new_request;
		}
		static request_object &parse_from_buffer(std::string& buffer) {
			request_object *object = new request_object();
			object->buffer = buffer;
			return *object;
		} 								
};
class response {
	public:
		bool recieved_request() {
			if(parsed_reqeust ==  NULL) {
				return false;
			} else {
				return true;
			}
				
		}
		std::string the_response = "generic response";		
}
//main
int main(int argc, char const *argv[]) {
	int server_fd, new_socket; long valread;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	char *greeting = "GREETING FROM SERVER";

	//create socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)	{
		std::cerr << "IN SOCKET" << std::endl;
		exit(EXIT_FAILURE);
	}		
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

	memset(address.sin_zero, '\0', sizeof address.sin_zero);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
		std::cerr << "IN BIND" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 10)< 0) {
		std::cerr << "IN LISTEN" << std::endl;
		exit(EXIT_FAILURE);
	}
	while(1) {
		std::cout << "\nWAITING FOR NEW CONNECTION\n" << std::endl;
		if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
			std::cerr << "IN ACCEPT" << std::endl;
			exit(EXIT_FAILURE);
		}
		
		char buffer[30000] = {0};
		valread = read( new_socket , buffer , 30000);
		std::cout << "\n" << buffer << std::endl;
		write(new_socket , greeting , strlen(greeting));
		std::cout << "---Greeting has been sent---" << std::endl;
		std::string the_buffer;
		bool found_start;
		std::string found_path;
		for(int i = 0; i < valread; i++) {
			if(buffer[i] == '/') {
				found_start = true;	
			}
			if(found_start) {
				found_path += buffer[i];
				if(buffer[i] == ' ') {
					found_start = false;
				}
			}	
		}
		std::cout << "==== " << found_path << " ====" << std::endl;
		found_start = false;
		close(new_socket);
	}
	return 0;
}


