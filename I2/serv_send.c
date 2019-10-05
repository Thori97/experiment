#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>

#define handle_error(msg) \
           do { perror(msg); close(ss); exit(EXIT_FAILURE); } while (0)

int N = 10;

int main(int argc, char** argv){
    int ss = socket(PF_INET, SOCK_STREAM, 0);
    if(ss==-1){
    perror("socket miss\n");
    exit(1);
            }

    unsigned char data[N];
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atol(argv[1]));
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(ss, (struct sockaddr *) &addr,
            sizeof(addr)) == -1)
        {handle_error("bind miss\n");
        }
    else
        printf("bind succeeded!\n");
    
    if (listen(ss, 10) == -1)
               handle_error("listen\n");
    else
        printf("listen succeeded!\n");
    
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    int s = accept(ss, (struct sockaddr *)&client_addr, &len);
    close(ss);
    if(s==-1){
    perror("accept miss\n");
    exit(1);
    }
    else{
        printf("accept accepted!\n");
    }

    while(1){
        int n = read(0, data, N);
        send(s, data, n, 0);
    }
    close(s);

}