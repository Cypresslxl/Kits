#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<iostream>
using namespace std;

int main(int argc, char** argv)
{
        //    1. 建立一个Socket
        // 参数 ipv4 面向字节流的 tcp协议
        int lfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if( lfd < 0)
        {
//                cout << "create socket error: " << strerror(errno) << "%s(errno: " << errno << endl;
            perror("socket");
                exit(0);
        }
    int opt = 1;
       //设置端口复用
       int ret = setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
       if(ret==-1)
       {
           perror("setsockopt");
           return -1;
       }

        //    2. 绑定端口10000
        sockaddr_in sin = {};    // sockaddr_in为网络地址的结构体
        sin.sin_family = AF_INET;  // 设置协议类型
        int port = 9999;
        sin.sin_port = htons(port); // 设置协议源端口号
        // 计算机数据表示存在两种字节顺序：
        // 网络字节顺序(Network Byte Order, NBO)与主机字节顺序（Host Byte Order, HBO)
        // NBO是大端模式(big-endian)，也就是整数的高位字节存放在内存的低地址处
        // 在网络上使用统一的网络字节顺序，可以避免兼容性问题
        // 而主机字节序与CPU或操作系统相关, 无法统一; 因此使用htons()将主机字节序转换成网络字节序
        sin.sin_addr.s_addr = htonl(INADDR_ANY); // 协议源地址 随机ip
        if (bind(lfd, (sockaddr*)&sin, sizeof(sin)) == -1)  //sockaddr 不利于编码
        {
//                cout << "ERROR: 绑定用于接受客户端连接的网络端口失败..." << endl;
            perror("bind");
                exit(0);
        }
        else
        {
                cout << "SUCCESS: 绑定端口" << port << "成功..." << endl;
        }

        //    3. 监听网络端口 listen
        if (listen(lfd, 5) == -1) // 第二个参数 backbag 为连接为完成队列长度
        {
//                cout << "ERROR: 监听用于接受客户端连接的网络端口失败..." << endl;
            perror("listen");
                exit(0);
        }
        else
        {
                cout << "SUCCESS: 监听端口成功..." << endl;
        }

        //    4. 等待接受客户端连接 accept
        sockaddr_in _clientAddr = {};
        int cliendAddrLen = sizeof(_clientAddr);
        int _clientSock = -1; // 初始化无效的socket fd 用来存储接入的客户端

        char msgBuf[] = "Hello, I'm Server";
        char buf[128];
        while (true)
        {
                // 当客户端接入时 accept函数会得到客户端的socket地址和长度
                _clientSock = accept(lfd, (sockaddr*)&_clientAddr, (socklen_t *)&cliendAddrLen);
                if (-1 == _clientSock) //接受到无效接入
                {
                        cout << "ERROR: 接受到无效客户端SOCKET..." << endl;
                        continue;
                }
                else
                {
                    //inet_ntoa 将ip地址转换成可读的字符串
                    cout << "新Client加入： IP = " << inet_ntoa(_clientAddr.sin_addr) << endl;
                    //    5. 向客户端发送数据 send()
                    send(_clientSock, msgBuf, strlen(msgBuf) + 1, 0); // +1是为了把\0算进去
                   while(true)
                   {
                      
                   int recvLen = recv(_clientSock, buf, sizeof buf, 0);
                   buf[recvLen] = '\0';   // 设置字符串结束符
                   cout << "recv msg from client: \n" << buf << endl;
                   send(_clientSock,buf,sizeof buf,0);
                   }
                }
        }

        
        close(lfd);
        return 0;
}
