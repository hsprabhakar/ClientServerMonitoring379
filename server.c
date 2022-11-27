#include "includes.h"
#include "helpers.h"
#include "tands.h"


void server_logger(int trans_status, int trans_val, int trans_id) {


}

double epoch_double(struct timespec *tv) {
	char time_str[32];
	sprintf(time_str, "%ld.%.9ld", tv->tv_sec, tv->tv_nsec);
	return atof(time_str);
}

double print_epoch_double() {
	struct timespec tv;
	if(clock_gettime(CLOCK_REALTIME, &tv)) {
		perror("error clock_gettime\n");
	}
	return epoch_double(&tv);
}


int main(int argc, char* argv[]){

	int sockfd, ret;
	struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;
	int trans_id = 0, trans_value = 0;
	pid_t childpid;


    if (argc != 2) {
        fprintf(stderr, "Please use the following syntax: $ server <PORT>\n");
        return -1;
    }

    int SERVER_PORT = atoi(argv[1]);
    if (SERVER_PORT > 64000 || SERVER_PORT < 5000) {
        fprintf(stderr, "Invalid server port. Must be between 5000 and 64000\n");
        return -1; 
    }
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	//printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	printf("Using port %d\n", SERVER_PORT);

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	//printf("[+]Bind to port %d\n", SERVER_PORT);

	if(listen(sockfd, 10) == 0){
		//printf("[+]Listening....\n");

		// TIMEOUT
		struct timeval tmv;
		tmv.tv_sec = 30;
		tmv.tv_usec = 0;
		setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tmv, sizeof tmv);
	}else{
		//printf("[-]Error in binding.\n");
	}


	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		//printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);
			int client_pid;
			int hostname_id;
			int received_hostname_id = 0;
			int trans_count = 0;


			while(1){
				///printf("before recv\n");
				recv(newSocket, &trans_value, sizeof(trans_value), 0);
				///printf("past here?\n");
				if(trans_value == -1){
					FILE *fp;
					fp = freopen("logger", "a", stdout);
					printf("%4d transactions from ug%d.%d\n", trans_count, hostname_id, client_pid);
					fclose(fp);
					///fprintf(stdout,"breaking\n");
					break;
				}

				else if (trans_value < -1) {
					client_pid = trans_value * -1; // back to regular PID
					// client pid was sent
				}
				else if (received_hostname_id == 0) {
					received_hostname_id += 1;
					hostname_id = trans_value;
					continue;

				}
				
				else{ // transaction received
					//server_logger(0, trans_value, trans_id); // Prints transaction that was received 
					double current_epoch = print_epoch_double();
					printf("%.2f: #%4d (T%3d) from ug%d.%d\n", current_epoch, trans_id, trans_value, hostname_id, client_pid);

					//printf("Client: %d\n", trans_value);
					Trans(trans_value); // do transaction on number
					// server_logger(1, trans_value, trans_id); // prints transaction done
					printf("%.2f: #%4d (DONE) from ug%d.%d\n", current_epoch, trans_id, hostname_id, client_pid);
					trans_count++;

					// Now have to send back: 
					
					send(newSocket, &trans_id, sizeof(trans_id), 0);
					bzero(&trans_value, sizeof(trans_value)); // dunno if it can be done
					trans_id++; 
				}
			}
			///printf("-1 RECEIVED\n");
			//break;

		}
		//printf("outside fork range\n");

	}

	close(newSocket);
	// final summary
	// this shit dont work cuz it never gets reached :(
	FILE *in_file = fopen("logger", "r");
	char ch;
	if (in_file == NULL) {
		fprintf(stderr, "Error: Can't display Summary\n");
	}
	else {
		printf("SUMMARY\n");
		do {
			ch =fgetc(in_file);
			printf("%c", ch);
		} while (ch != EOF);
		fclose(in_file);
	}


	return 0;
}