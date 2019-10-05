#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
//#define N 200
//int N = 200;
int N = 20000;
int main(int argc,char **argv){
    //引数チェック
    if(argc == 2){//server
    //char * addr_ip;
    int port;
    //addr_ip = argv[1];
    port = atoi(argv[1]);
    //socket
    int ss = socket(PF_INET,SOCK_STREAM,0); 
    if(ss == -1){perror("open");exit(1);}
    //bind 
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(ss,(struct sockaddr *)&addr,sizeof(addr)) == -1){
        perror("bind");
        close(ss);
        exit(1);
    }
    listen(ss,10);
    //accept
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    int s = accept(ss,(struct sockaddr *)&client_addr,&len);
    if(s == -1){
        perror("accept");
        exit(1);
    }

    //rec開始
    FILE *fp = popen("rec -t raw -b 16 -c 1 -e s -r 44100 - ","r");
    if(fp == NULL){
        perror("popen");
        exit(1);
    }
    char ch;
    char ch1;
    while(1){
            ch = getc(fp);
            send(s,&ch,1,0);
            recv(s,&ch1,1,0) ;
            printf("%c",ch1);
        }        
    close(s);
    close(ss);
    pclose(fp);
    }else if(argc == 3){//client
    N = 10;


    char * addr_ip;
    int port;
    addr_ip = argv[1];
    port = atoi(argv[2]);
    int s = socket(PF_INET,SOCK_STREAM,0); 
    if(s == -1){perror("open");exit(1);}
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(addr_ip);
    addr.sin_port = htons(port);
    int ret = connect(s,(struct sockaddr *)& addr,sizeof(addr));
    if(ret == -1){perror("connection");exit(1);}
    char *data =  (char*)malloc(sizeof(char)*N);
    char ch;
    char ch1;
    while(1){
       /// ch1 = getc();
        ch = getchar();
        send(s,&ch,1,0);
        recv(s,&ch1,1,0) ;
        printf("%c",ch1);
        
    }
    free(data);
    close(s);
    }   
}