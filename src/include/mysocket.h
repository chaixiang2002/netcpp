#pragma once

#include "util.h"
#include <arpa/inet.h>
#include <cstdint>
#include <functional>
#include <sys/socket.h>
#include <unistd.h>
class mysocket{
    int sockfd;
public:
    std::function<void(int)> fun=nullptr;

    mysocket(int domain,int type){
        sockfd=socket(domain, type, 0);
        judge(sockfd,"create socket error");
    }
    mysocket(int _sockfd):sockfd(_sockfd){}

    ~mysocket(){
        if (sockfd!=-1)
        close(sockfd);
    }

    mysocket(const mysocket&)=delete;
    mysocket &operator=(const mysocket&)=delete;

    void _bind(int family,const char *ip,int port){
        struct sockaddr_in serv_addr;
        bzero(&serv_addr,sizeof(serv_addr));
        serv_addr.sin_family=family;
        serv_addr.sin_addr.s_addr=inet_addr(ip);
        serv_addr.sin_port=htons(port);
        judge(bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)), "sock bind failed");
    }

    void _listen(int n){
        judge(listen(sockfd, n), "sock listen failed");
    }

    mysocket&& _accpet(){
        struct sockaddr_in clnt_addr;
        socklen_t clint_addr_len=sizeof(clnt_addr);
        bzero(&clnt_addr,sizeof(clnt_addr));
        int clnt_sockfd=accept(sockfd,(sockaddr*)&clnt_addr,&clint_addr_len);
        judge(clnt_sockfd,"socket accept error");
        printf("new client fd: %d   IP: %s   Port: %d  \n",clnt_sockfd,inet_ntoa(clnt_addr.sin_addr),ntohs(clnt_addr.sin_port));
        mysocket clnt_sock(clnt_sockfd);
        return std::move(clnt_sock);
    }

    void _connect(int family,const char *ip,int port){
        struct sockaddr_in serv_addr;
        bzero(&serv_addr,sizeof(serv_addr));
        serv_addr.sin_family=family;
        serv_addr.sin_addr.s_addr=inet_addr(ip);
        serv_addr.sin_port=htons(port);
        judge(connect(sockfd,(sockaddr*)&serv_addr,sizeof(serv_addr)),"socket connect failed");
    }
    int getsockfd(){
        return sockfd;
    }  
    void set_callback_fun(std::function<void(int)> _fun){
        fun=_fun;
        // std::bind(_fun,sockfd);
    } 

    void closefd(){
        if (sockfd!=-1) {
            close(sockfd);
        }
    }
    void judge(int ret,const char* s){
        misjudgment(ret,s,std::bind(&mysocket::closefd,this));
    }


};