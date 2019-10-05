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
    if(argc == 2){
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


    int BUF = 256;
    FILE	*fp;
	char	*cmdline = "rec -t raw -b 16 -c 1 -e s -r 44100 -";
	if ( (fp=popen(cmdline,"r")) ==NULL) {
		perror ("can not exec commad");
		exit(EXIT_FAILURE);
	}
	unsigned char buf[BUF];
    unsigned char buf1[1];
	while (!feof(fp)) {
		buf1[0] = fgetc(fp);
        send(s, buf1, 1, 0);
        int n = recv(s, buf, BUF, 0);
        for(int i=0;i<n;i++){
            printf("%c", buf[i]);
        }
	}

    /*while(1){
        int n = read(0, data, N);
        send(s, data, n, 0);
    }*/
    close(s);
    (void) pclose(fp);
    }
    
    else if(argc == 3){
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
        fgets(data, n, stdin);
        send(s, data, n, 0);
    }
    }
}