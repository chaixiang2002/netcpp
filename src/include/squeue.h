#pragma once

#include <cstring>
#include <iostream>
#include <ostream>
#include <sys/types.h>
#include <sys/sem.h>

#include <unistd.h>
template<class T,int MaxLength>
class squeue{
private:
    bool m_inited=false;
    T m_data[MaxLength];
    int head;   
    int tail;   
    int m_length;
    squeue(const squeue&)=delete;// 拷贝构造
    squeue &operator=(const squeue&)=delete; // 赋值函数

public:
    squeue(){init();}
    void init(){
        if (m_inited!=true) {
            m_inited=true;
            memset(m_data, 0, sizeof(m_data));
            head=0;
            tail=0;
            m_length=0;
        }
    }
    bool full(){
        return m_length==MaxLength;
    }

    bool push(const T &element){
        if (full()) {
            std::cout<< "队列已满\n" <<std::endl;
            return false;
        }
        m_data[tail]=element;

        m_length++;
        tail=(tail+1)%MaxLength;
        
        return true;
    }

    T& front(){
        return m_data[head];
    }

    bool empty(){
        return m_length==0;
    }
    bool pop(){
        if (empty()) {
            std::cout<< "队列已空\n" <<std::endl;
            return false;
        }
        head=(head+1)%MaxLength;
        m_length--;

        return true;
    }

    int size(){
        return m_length;
    }
    void print_queue(){
        std::cout<< "--------------"<<std::endl;
        std::cout<< "MaxLength : "<<MaxLength<<std::endl;
        std::cout<< "m_inited : "<<m_inited<<std::endl;
        std::cout<< "head : "<<head<<std::endl;
        std::cout<< "tail : "<<tail<<std::endl;
        std::cout<< "m_length : "<<m_length<<std::endl;
        for (int i=0; i<size(); i++) {
            std::cout<< "m_data["<<(head+i)%MaxLength<<"] : "<<m_data[(head+i)%MaxLength] <<std::endl;
            // sleep(1);
        }
        std::cout<< "              "<<std::endl;

    }
};

class csemp{
private:
    union semun{
        int val;
        struct semid_ds *buf;
        unsigned short *arry;
    };

    int m_semid;

    short m_sem_flg; 

    csemp (const csemp&)=delete;
    csemp  &operator=(const csemp&)=delete;

public:
    csemp():m_semid(-1){}
    // 若信号量用作互斥锁                      1                SEM_UNDO
    // 若信号量用作生产消费模型                 0                   0
    bool init(key_t key,unsigned short value=1,short sem_flg=SEM_UNDO);
    bool wait(short sem_op=-1);// ==0 阻塞等待 P操作
    bool post(short sem_op=1); //             V操作 
    int getvalue();
    bool destroy();
    ~csemp(){} 
};