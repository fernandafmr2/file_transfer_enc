#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#define BUFF_LEN 1024

bool write_file(int sockfd, FILE *fp);
void decrypt(char *in);

int main(void) 
{
        int e;
        char *filename = "output";

        int sockfd, new_sock;
        FILE *fp;
        struct sockaddr_in server_addr, new_addr;
        socklen_t addr_size;

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd < 0) {
                perror("err socket");
                exit(1);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = 8080;
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

        e = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
        if(e < 0) {
                perror("[-]Error in bind");
                close(sockfd);
                exit(1);
        }

        if (listen(sockfd, 10) == 0) {
                printf("listen...\n");
        } else {
                perror("listen");
                close(sockfd);
                exit(1);
        }

        addr_size = sizeof(new_addr);
        new_sock = accept(sockfd, (struct sockaddr*) &new_addr, &addr_size);
        
        fp = fopen(filename, "w");

        if (write_file(new_sock, fp) == false) {
                perror("error write");
                close(new_sock);
                close(sockfd);
                exit(1);
        }

        decrypt(filename);

        close(new_sock);
        close(sockfd);

        return 0;
}


void decrypt(char *in)
{
        int key;
        char c;
        FILE *fin;
        FILE *fout;

        printf("Enter Key: ");
        scanf("%d", &key);

        fin = fopen(in, "rb");
        strcpy(in, "qutput");
        fout = fopen(in, "wb");
        while (!feof(fin))
        {
            c = fgetc(fin);
            int temp = (c - key);
            fputc((char)temp, fout);
        }
        fclose(fin);
        fclose(fout);
}


bool write_file(int sockfd, FILE *fp)
{
        int n;
        char buffer[BUFF_LEN];

        while (1) {
                n = recv(sockfd, buffer, BUFF_LEN, 0);
                if (n == 0) {
                        break;
                } else if (n == -1) {
                        return false;
                }

                fprintf(fp, "%s", buffer);
                memset(&buffer, 0, sizeof(buffer));
        }
        return true;
}


