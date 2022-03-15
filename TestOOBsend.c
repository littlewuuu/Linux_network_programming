#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <assert.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <string.h>

int main(int argc, char* argv[])
{
    //输入不满足要求时输出提示
    if(argc <= 2)
    {
        printf("usage: %s ip_address port_number backlog\n",basename(argv[0]));
        return 1;
    }

    const char* ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;

    //转换成网络地址格式
    inet_pton (AF_INET, ip, &server_address.sin_addr);
    server_address.sin_port = htons(port);

    //创建 socket
    int sockfd = socket(PF_INET,SOCK_STREAM, 0);
    assert(sockfd >= 0);
    
    //与服务器建立连接
    //连接成功建立后 sockfd 唯一表示这个连接
    if( connect(sockfd, (struct sockaddr*)&server_address, sizeof(sizeof(server_address))<0) )
    {
        printf("connection failed\n");
    }
    else
    {
        const char* oob_data = "abc";//带外数据
        const char* normal_data = "123";
        sed(sockfd, normal_data, strlen(normal_data),0);
        sed(sockfd, oob_data, strlen(oob_data),MSG_OOB);
        sed(sockfd, normal_data, strlen(normal_data),0);
    }

    close(sockfd);
    return 0;
}