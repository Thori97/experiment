#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char** argv){
    int s = socket(PF_INET, SOCK_STREAM, 0);
    if(s==-1){
    perror("socket miss\n");
    exit(1);
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atol(argv[2]));
    int ret = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    if(ret==-1){
    perror("connect miss\n");
    exit;
    }
    else{
        printf("connect succeeded!\n");
    }
    unsigned char data[10];
    int N = 10;
    char c;
    int n;
    unsigned char data2[1];
    while( ( c = getchar()) != EOF ){
        data2[0] = c;
        send(s, data2, 1, 0);
    }
    shutdown(s, SHUT_WR);

    while(1){
    int s = socket(PF_INET, SOCK_STREAM, 0);
    if(s==-1){
    perror("socket miss\n");
    exit(1);
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atol(argv[2]));
    ret = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    if(ret==-1){
        printf("connect miss\n");
                }
    else{
        printf("s = %d", s);
        printf("connect succeeded! s = %d\n", s);
        }
    }
    while(1){
        printf("receive");
        n = recv(s, data, N, 0);
        write(1, data, n);
        for(int i=0;i<n;i++){
            if(data[i] = EOF){
                close(s);
                break; break;
            }
        }
        }
        return 0;
    }