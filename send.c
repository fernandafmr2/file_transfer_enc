#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <arpa/inet.h>
#define BUFF_LEN 1024

bool send_file(FILE *fp, int sockfd);
void sending();
// ssize_t total = 0; // get bytes number 


int main(int argc, char *argv[])
{
	int key;
	char c;
	FILE *fin;
	FILE *fout;
	(void)argc;

	if (argc != 2) {
		printf("usage: ./exec filepath \n");
		exit(1);
	}

	printf("Enter key: ");
	scanf("%d", &key);


	fin = fopen(argv[1], "rb");
	strcpy(argv[1], "output");
	fout = fopen(argv[1], "wb");
	while (!feof(fin)) {
		c = fgetc(fin);
		int temp = (c + key);
		fputc((char) temp, fout);
	}
	fclose(fin);
	fclose(fout);

	sending(argv[1]);

	return 0;
}

void sending(char in[260])
{
	int e;

	int sockfd;
	struct sockaddr_in serveraddr;
	FILE *fp;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("err socket");
		exit(1);
	}

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = 8080;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	e = connect(sockfd, (struct sockaddr*) &serveraddr, sizeof(serveraddr));
	if (e < 0) {
		perror("create socket"); 
		close(sockfd);
		exit(1);
	}
	printf("Connected to server!\n");

	fp = fopen(in, "r");
	if (fp == NULL) {
		perror("error in reading file");
		exit(1);
	}

	if (send_file(fp, sockfd) == false) {
		perror("error send");
		exit(1);
	};

	printf("Send file!..\n");

	close(sockfd);
}


bool send_file(FILE *fp, int sockfd) 
{
	char data[BUFF_LEN] = {0};

	while(fgets(data, BUFF_LEN, fp) != NULL) {
		if (send(sockfd, data, sizeof(data), 0) == -1) {
			perror("error send file");
			close(sockfd);
			return false;
		}
		memset(&data, 0, sizeof(data));
	}
	return true;
}