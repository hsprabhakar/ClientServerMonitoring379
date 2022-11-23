#include "includes.h"
#include "tands.h"
#include "helpers.h"


void client_logger(int trans_type, int trans_val) {


}


int main(int argc, char* argv[]){

	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	//char buffer[1024] = "\0";
	char hostname[HOST_NAME_MAX + 1];
	gethostname(hostname, HOST_NAME_MAX+1); // https://stackoverflow.com/questions/5190553/linux-c-get-server-hostname
    if (argc != 3) {
        fprintf(stderr, "Please use the following syntax: $ client <PORT> <IP-ADDRESS>\n");
        return -1;
    }

    int CLIENT_PORT = atoi(argv[1]);
    char* SERVER_IP = argv[2];
    if (CLIENT_PORT > 64000 || CLIENT_PORT < 5000) {
        fprintf(stderr, "Invalid port. Must be between 5000 and 64000\n");
        return -1; 
    }
	printf("Using port %d\n", CLIENT_PORT);
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	//printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(CLIENT_PORT);
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	printf("Using server address %s\n", SERVER_IP);
	printf("Host %s.%d\n", hostname, ntohs(serverAddr.sin_port));
	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	

	//printf("[+]Connected to Server.\n");

	while(1){
		//printf("Client: \t");

        //char *line = NULL;
		char line[1024];
		char action_char = 0;
		int trans_value = 0, trans_id = 0, mylen;

		while (fgets(line, sizeof(line), stdin)) { // https://stackoverflow.com/a/74541088/12229659 thank you David :)
			sscanf(line, "%c%d%n", &action_char, &trans_value, &mylen);
			if (action_char == 'S') {
				//client_logger(0, trans_value);
				Sleep(trans_value);
				continue;
			}
			else if (action_char == 'T') {
				printf("Sending transaction with value: %d\n", trans_value);
				//client_logger(1, trans_value); Sent T of value x
				send(clientSocket, &trans_value, sizeof(trans_value), 0);
			}

			if (recv(clientSocket, &trans_id, sizeof(trans_id), 0) < 0) { // received transaction number from server into trans_value
				fprintf(stderr, "error in receiving data. \n");
				exit(1);
			}
			printf("Server gave T%d this transaction ID: %d\n", trans_value, trans_id);

		}
		// exit 

        // client now receives stuff from server
        // client now closes itself

		trans_value = 101; // send exiting command to server after file is done
		send(clientSocket, &trans_value, sizeof(trans_value), 0);

		if(trans_value == 101){
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}

		// if(recv(clientSocket, trans_id, sizeof(trans_id), 0) < 0){
		// 	printf("[-]Error in receiving data.\n");
		// }else{
		// 	printf("Server: \t%d\n", trans_id);
		// }
	}

	return 0;
}