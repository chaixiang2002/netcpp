#include "mysocket.h"
#include "util.h"
#include <cstdlib>
#include <iostream>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <utility>
using namespace std;

void fun(int argc,char *argv[]){
    mysocket clnt_sock(AF_INET,SOCK_STREAM);
    // clnt_sock._bind(AF_INET, "192.168.127.130", 8088);
    clnt_sock._connect(AF_INET,"192.168.127.130", 8088);
    int sockfd=clnt_sock.getsockfd();
    while (true)
    {
        char buf[1024];
        bzero(&buf,sizeof(buf));
        scanf("%s", buf);
        ssize_t read_bytes=write(sockfd,buf,sizeof(buf));
        if(read_bytes>0){
            printf("message from server: %s\n", buf);          // 打印buf
        }else if(read_bytes == 0){
            printf("server socket disconnected!\n");
            break;
        }else if(read_bytes == -1){
            close(sockfd);
            // errif(true, "socket read error");
        }
    }
}

int main(int argc,char *argv[]){
    // if (argc!=2) {
    //     print("参数不合法");
    // }
    
    fun(argc,argv);
    return 0;
}