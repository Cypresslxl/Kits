//
// Created by Evila on 2021/6/27.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char** argv)
{
       //1.创建一个sock fd
    int cfd = socket(AF_INET,SOCK_STREAM,0);
    if(cfd<0)
    {
        perror("socket");
        exit(0);
    }
    //设置请求连接的server地址 domain ip port

        // 2. 设置请求连接的server 地址
        sockaddr_in addr = {};    // sockaddr_in为网络地址的结构体
        addr.sin_family = AF_INET;
        int port = 9999;
        addr.sin_port = htons(port);

        string ip;
        cout << "please input server ip: ";
        cin >> ip;

        if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0)
        {
//                cout <<"inet_pton error for " << ip << endl;
            perror("inet_pton");
                exit(0);
        }
    cout<<"connect"<<endl;
        // 3. 连接服务端
        int ret = connect(cfd, (struct sockaddr*)&addr, sizeof(addr));
        if (ret < 0)
        {
//                cout << "connect error: " << strerror(errno) << "errno: " << errno << endl;
            perror("connect");
                exit(0);
        }
        else
        {
                cout << "connect to server: " << ip << ":" << port << " success!" << endl;
        }
    cout<<"send"<<endl;
    char recvBuf[1024];
    char sendBuf[64];
        // 4. 向服务端发送数据
        while(true)
        {
            //send
            cout<<"please input sendBuf :"<<endl;
            cin>>sendBuf;
            send(cfd,sendBuf,sizeof sendBuf,0);
            
            //recv
//            while((len = recv(cfd,temp,sizeof temp,0))>2)  //blocking
//            {
//                cout<<"len : "<<len<<endl;
//                memcpy(recvBuf + total,temp,len);
//                total += len;
//                len = 0;
//            }
            
            int len = recv(cfd,recvBuf,sizeof recvBuf,0);
            recvBuf[len] = '\0';
            cout<<"recvMsg : "<<recvBuf<<endl;
        }

        close(cfd);
        exit(0);
}
