#include "util.h"
#include <cstdlib>
#include <iostream>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <utility>
#include "mysocket.h"
using namespace std;

void fun(int argc,char *argv[]){
    // create
    mysocket serv_sock(AF_INET,SOCK_STREAM);

    // addr

    // bind
    serv_sock._bind(AF_INET, "192.168.127.130", 8080);

    // listen
    serv_sock._listen(SOMAXCONN);

    // accpet
    mysocket clnt_mysock=serv_sock._accpet();

    // callback
    std::function<void (int)> callback_fun=[](int clnt_sockfd){
        while (true)
        {
            char buf[1024];
            bzero(&buf,sizeof(buf));
            ssize_t read_bytes=read(clnt_sockfd,buf,sizeof(buf));// <unistd.h>
            if (read_bytes>0)
            {
                printf("message from client fd %d: %s\n",clnt_sockfd,buf);
                write(clnt_sockfd,buf,sizeof(buf));
            } else if(read_bytes == 0){
                printf("client fd %d disconnected\n", clnt_sockfd);
                close(clnt_sockfd);
                break;
            } else if(read_bytes == -1){
                close(clnt_sockfd);
                // errif(true, "socket read error");
            }
            
        }
    };
    clnt_mysock.fun(clnt_mysock.getsockfd());

}

int main(int argc,char *argv[]){
    // if (argc!=2) {
    //     print("参数不合法");
    // }
    
    fun(argc,argv);
    return 0;
}