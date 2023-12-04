#pragma once

#include "squeue.h"
#include <cstdio>
#include <iostream>

inline void misjudgment(int ret,const char *s){
    if (ret==-1) {
        perror(s);
        // cout<< s<<" failed"<<endl;
    }
}

inline void isLegal(int argc, char* argv[]){
    std::cout << "Number of arguments: " << argc << std::endl;
    for(int i = 0; i < argc; i++) {
        std::cout << "argv[" << i << "]: " << argv[i] << std::endl;
    }
    if (argc<=1) {
        std::cout << "缺少参数，程序已退出！ " << argc << std::endl; 
        exit(-1);
    }
}

inline void print(const char* s){
    std::cout << s << std::endl; 
}

inline bool str_eq(const char* a,const char* b){
    return strcmp(a, b)==0?1:0;
}
