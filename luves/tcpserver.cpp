//
//  tcp.cpp
//  Luves
//
//  Created by leviathan on 16/5/17.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#include "tcpserver.h"
#include "eventhandle.h"

namespace luves {
    
    
    //
    //TCP Server模块
    //用于新建TCP服务端
    //
    TcpServer::~TcpServer()
    {
        delete listen_channel_;
    }
    void  TcpServer::Bind()
    {
        bzero(&serverAddr_, sizeof(serverAddr_));
        serverAddr_.sin_family=AF_INET;
        serverAddr_.sin_addr.s_addr=inet_addr(addr_.GetIp().c_str());
        serverAddr_.sin_port=htons(addr_.GetPort());
        
        Socket::Bind(listenFd_,this->GetServerAddrPointer());
    }
    
    void TcpServer::Listen()
    {
        listenning_=true;
        Socket::Listen(listenFd_);
    }
    
    void TcpServer::RunServer()
    {
        this->Bind();
        this->Listen();

        loop_->SetChannelPtr(&channel_fd_);
        loop_->AddChannel(listen_channel_);
    }
    
    //当有新的连接,epoll被触发,回调HandleAccept()函数并建立新的连接
    void TcpServer::HandleAccept()
    {
        struct sockaddr_in client_addr;
        int accept_fd;
        while((accept_fd=Socket::Accept(listen_channel_->GetFd(), &client_addr))>=0)
            this->NewChannel(accept_fd);
    }
    
    void TcpServer::NewChannel(int accept_fd)
    {
        Channel * new_channel = new Channel(loop_, accept_fd, false);
        
        new_channel->SetReadCb([=]{new_channel->GetConnectionPtr()->HandleRead();});
        new_channel->SetWriteCb([=]{new_channel->GetConnectionPtr()->HandleWrite();});
        if (read_cb_)
            new_channel->GetConnectionPtr()->SetReadCb(read_cb_);
        if (write_cb_)
            new_channel->GetConnectionPtr()->SetWriteCb(write_cb_);
        
        channel_fd_[accept_fd] = new_channel;
        loop_->AddChannel(new_channel);
        
    }
    
    //
    //TCP Client
    //

/*
    void TcpClient::RunClient()
    {
        this->Bind();
        this->HandleConnect();
        client_channel_->SetEvent(read_event);
        loop_->UpdateChannel(client_channel_);
        
        TcpConnectionPtr conn=TcpConnectionPtr(new TcpConnection(loop_));
        if (readcb_)
        {
            conn->SetReadCb(readcb_);
        }
        if (writecb_) {
            conn->SetWriteCb(writecb_);
        }
        
    }
    
    void TcpClient::Bind()
    {
        struct sockaddr_in client_addr;
        bzero(&client_addr, sizeof(client_addr));
        client_addr.sin_family=AF_INET;
        client_addr.sin_addr.s_addr=htons(INADDR_ANY);
        client_addr.sin_port=htons(0);
        
        bind(client_fd_, (struct sockaddr *)&client_addr, sizeof(client_addr));
    }
    
    void TcpClient::HandleConnect()
    {
        struct sockaddr_in server_addr;
        server_addr.sin_family=AF_INET;
        server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
        server_addr.sin_port=htons(server_addr_.GetPort());
        
        Socket::Bind(client_fd_, &server_addr);
        connect(client_fd_, (struct sockaddr *)&server_addr, sizeof(server_addr));
    }
*/
}
