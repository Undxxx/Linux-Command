#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <dirent.h>
#include<time.h>
#include<grp.h>
#include<pwd.h>

typedef struct QNode{
    char name[1000];
    struct QNode * next;
}QNode;
//创建链式队列的函数
QNode * initQueue(){
    //创建一个头节点
    QNode * queue=(QNode*)malloc(sizeof(QNode));
    //对头节点进行初始化
    queue->next=NULL;
    return queue;
}