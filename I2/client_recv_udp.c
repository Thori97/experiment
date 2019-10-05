#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char** argv){
    int s = socket(PF_INET, SOCK_DGRAM, 0);
    if(s==-1){
    perror("socket miss\n");
    exit(1);
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atol(argv[2]));
    int ret = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    if(s==-1){
    perror("connect miss\n");
    exit;
    }
    unsigned char data[10];
    int N = 10;
    while(1){
        int n = recv(s, data, N, 0);
        for(int i=0;i<n;i++){
            printf("%c", data[i]);
            if(data[i] == EOF){
                printf("end");
                shutdown(s, SHUT_WR);
                return 0;
            }
        }
    }
}