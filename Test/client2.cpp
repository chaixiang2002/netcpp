#include "mysocket.h"
#include "util.h"
#include <cstdlib>
#include <iostream>

#include <string>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <utility>
using namespace std;

void fun(int argc,char *argv[]){
    // read config
    auto[ip,port]=read_config("client");

    //  create sock
    mysocket clnt_sock(AF_INET,SOCK_STREAM);

    //  sock connect  
    clnt_sock._connect(AF_INET,ip.c_str(), port);

    //  Write business logic task;
    std::function<void(int)> clnt_task=[](int sockfd){
        while (true)
        {
            char buf[1024];  //在这个版本，buf大小必须大于或等于服务器端buf大小，不然会出错，想想为什么？
            bzero(&buf, sizeof(buf));
            scanf("%s", buf);
            ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
            if(write_bytes == -1){
                printf("socket already disconnected, can't write any more!\n");
                break;
            }
            bzero(&buf, sizeof(buf));
            ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
            if(read_bytes > 0){
                printf("message from server: %s\n", buf);
            }else if(read_bytes == 0){
                printf("server socket disconnected!\n");
                break;
            }else if(read_bytes == -1){
                close(sockfd);
                print("socket read error");
            }
        }
    };

    // Assign task
    clnt_sock.set_callback_fun(clnt_task);
    // run task
    clnt_sock.fun(clnt_sock.getsockfd());
}

int main(int argc,char *argv[]){
    // if (argc!=2) {
    //     print("参数不合法");
    // }
    
    fun(argc,argv);
    return 0;
}