//实现监听函数的功能
// backlog 参数
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<assert.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>


static bool stop = false;
//SIGTERM 信号的处理函数，出发时结束主程序中的循环
static void handle_term(int sig)
{
    stop = true;
}

//argc 是指命令行输入参数的个数(以空白符分隔) ;
//argv存储了所有的命令行参数。第一个参数是程序名，所以argc >= 1
int main(int argc, char* argv[])
{
    signal(SIGTERM, handle_term);//当收到 SIGTERM 信号时执行 handle_term 函数
    //(Signal Terminate) 发送给本程序的终止请求信号

    //输入不满足要求时输出提示
    if(argc <= 3)
    {
        printf("usage: %s ip_address port_number backlog\n",basename(argv[0]));
        //basename作用是得到特定的路径中的最后一个'/'后面的内容

        return 1;
    }

    const char* ip = argv[1];
    int port = atoi(argv[2]); //atoi 将字符串转化为整型
    int backlog = atoi(argv[3]); //backlog 内核监听队列的最大长度

    //创建socket
    int sock = socket(PF_INET, SOCK_STREAM, 0); //socket 返回地址描述符给 sock，失败返回-1,

    assert(sock >= 0); //socket 创建失败结束程序
    //assert断言 将通过检查表达式 expression 的值来决定是否需要终止执行程序。
    //也就是说，如果表达式 expression 的值为假（即为 0），那么它将首先向标准错误流 stderr 打印一条出错信息，
    //然后再通过调用 abort 函数终止程序运行；否则，assert 无任何作用。

    //创建一个 IPv4 socket 地址
    struct sockaddr_in address;
    bzero(&address,sizeof(address));//bzero将内存（字符串）前n个字节清零
    
    //为 socket 结构体的三个参数赋值
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);//这个函数转换字符串到网络地址，第一个参数af是地址族，第二个参数是点分十进制的字符串,转换后存在第三个参数dst中
    address.sin_port = htons(port);


    int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));//失败返回-1
    assert(ret != -1);

    //循环等待连接，直到有 sigterm 信号将它中断
    while (!stop)
    {
        sleep(1);//执行挂起一段时间
    }

    //关闭 socket
    close(sock);
    return 0;
    
}
