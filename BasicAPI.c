//-------------------------------------------
//实现主机字节序和网络字节序的转换
#include<netinet/in.h>
unsigned long int htonl(unsigned long int hostlong);
unsigned short int htons(unsigned long int hostshort);
unsigned long int ntohl(unsigned long int netlong);
unsigned short int ntohs(unsigned long int netshort);

//-------------------------------------------
//socket地址结构体
#include<bits/socket.h>
struct sockaddr
{
    sa_family_t sa_family; //sa_family_t 是地址族类型
    char sa_data[14]; //socket 地址值
};

//-------------------------------------------
//通用socket地址结构体，内对齐的
#include<bits/socket.h>
struct sockaddr_storage
{
    sa_family_t sa_family;
    unsigned long int __ss_align;
    char __ss_padding[128 - sizeof(__ss_align)];
};

//-------------------------------------------
// Unix 本地域协议族使用专用 socket 地址结构体
#include<sys/un.h>
struct sockaddr_un
{
    sa_family_t sin_family; //地址族:AF_UNIX
    char sun_path[108]; //文件路径
};


//-------------------------------------------
//TCP/IP 协议族的专用 socket 地址结构体
//这两个专用 socket 地址结构体（包括 sockaddr_storage）在实际使用中都要转化为通用 socket 地址类型 sockaddr（强制转换即可）
//因为虽有 socket 编程接口使用的地址参数都是 sockaddr
struct sockaddr_in
{
    sa_family_t sin_family; //地址族：AF_INET
    u_int16_t sin_port; //端口号，用网络字节序表示
    struct in_addr sin_addr; //IPv4地址结构体
};
//IPv4 地址结构体
struct in_addr
{
    u_int32_t s_addr; //IPv4 地址，用网络字节表示
};
struct sockaddr_in6
{
    sa_family_t sin6_family; //地址族：AF_INET6
    u_int16_t sin6_port; //端口号，用网络字节序表示
    u_int32_t sin6_flowinfo; //信息流，应设置为 0
    struct in6_addr sin6_addr; //IPv6地址结构体
};
//IPv6 地址结构体
struct in6_addr 
{
    unsigned char sa_addr[16]; //IPv6 地址，用网络字节表示
};

//--------------------
//IP 地址转换函数
#include<arpa/inet.h>
in_addr_t inet_addr(const char* strptr); //用点分十进制字符串表示的 IPv4 地址转化为网络字节序整数表示的 IPv4 地址
int inet_aton(const char* cp, struct in_addr* inp); //完成和 inet_addr 一样的功能，但是将结果存储于 inp 指向的地址结构中
char* inet_ntoa(struct in_addr in); //将网络字节序整数表示的 IPv4 地址转化为用点分十进制字符串表示的 IPv4 地址，用函数内的静态变量存储结果，返回值指向该静态内存

//-------------------------
//创建 socket
#include<sys/types.h>
#include<sys/socket.h>
int socket(int domain, int type, int protocol);
//domain 告诉系统使用哪个底层协议，对于 tcp/ip 协议族，应设置为 PF_INET(protocol family of internet, for IPv4) 或 PF_INET6
//对于 Unix 本地协议族:PF_UNIX

//type 指定服务类型
//服务类型主要有SOCK STREAM服务（流服务）和SOCK UGRAM(数据报)服务。
//对TCP/IP协议族而言，其值取SOCK STREAM表示传输层使用TCP协议，取SOCK DGRAM表示传输层使用UDP协议。

//protocol参数是在前两个参数构成的协议集合下，再选择一个具体的协议。
//不过这个值通常都是唯一的（前两个参数已经完全决定了它的值）。几乎在所有情况下，我们都应该把它设置为0，表示使用默认协议。
//socket系统调用成功时返回一个socket整型描述符，失败则返回 -1 并设置errno。

//------------------------
//命名 socket 的系统调用是 bind
//命名：将一个 socket 与 socket 地址绑定
#include <sys/types.h>
#include <sys/socket.h> 
int bind(int sockfd, const struct sockaddr* my_addr, socklen_t addrlen )
//bind将my_addr所指的socket地址分配给未命名的sockfd文件描述符，addrlen参数指出该socket地址的长度。
//bind成功时返回O,失败则返回 -1 并设置errno。
//其中两种常见的errno是EACCES和EADDRINUSE,它们的含义分别是： 
//EACCES,被绑定的地址是受保护的地址，仅超级用户能够访问。比如普通用户将socket绑定到知名服务端口（端口号为0~l023)上时，bind将返回EACCES错误。
//EADDRINUSE,被绑定的地址正在使用中。比如将socket绑定到一个处于TIME WAIT状态的socket地址。

//监听 socket，创建一个监听队列存放待处理的客户连接
#include<sys/socket.h>
int listen(int sockfd, int backlog)
//sockfd 指定被监听的 socket
//backlog 提示内核监听队列的最大长度
//成功：返回 0


//从 listen 监听队列中接受一个连接
#include<sys/types.h>
#include<sys/socket.h>
int accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen);
//sockfd 是执行过 listen 系统调用的监听 socket
//addr 获取被接受连接的远端 socket 地址，长度由 addrlen 指出
//成功：返回一个新的连接 socket
//失败：返回-1 errno


//发起连接，主动与服务器建立
#include<sys/types.h>
#include<sys/socket.h>
int connect(int sockfd, const struct* serv_addr, socklen_t addrlen);

//关闭连接
#include<sys/socket.h>
int close(int fd);

//强制终止
#include<sys/socket.h>
int shutdown(int sickfd, int howto);

//tcp 数据读写
//--------------------------------------
#include<sys/types.h>
#include<sys/types.h>
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
//读取 socket 的数据，buf 指定缓存区位置，len 指定缓存区大小。flags 通常为 0

size_t send(int sockfd, const void* buf, size_t len, int flags);
//成功返回实际写入的长度

