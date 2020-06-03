#include "server.h"

void startServer(){
    int client;
    int sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sfd == -1) {
        perror("create failed.");
        return;
    }

    sockaddr_in saddr;
    sockaddr_in peer_saddr;
    socklen_t peer_len = sizeof(sockaddr);
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(80);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sfd, (sockaddr*)&saddr,sizeof(sockaddr)) == -1){
        perror("bind error.");
        return;
    }

    if (listen(sfd, 20) == -1){
        perror("listen error.");
        return;
    }
    
    while(1){
        
        client = accept(sfd, (sockaddr*)&peer_saddr, &peer_len);
        if (client == -1){
            perror("accept error.");
            return;
        } else {
            printf("accept client:%s:%d\n",inet_ntoa(peer_saddr.sin_addr),peer_len);
        }

        pid_t pid = fork();
        if (pid < 0){
            perror("fork error.");
            return;
        } else if (pid == 0){
            close(sfd);
            char* recvbuff = (char*)malloc(1024);
            memset(recvbuff, '\0', sizeof(recvbuff));
            recv(client, recvbuff, 1024,0);

            if (strlen(recvbuff) == 0) continue;
            printf("recv:\n%s\n",recvbuff);
            // 进行请求解析，判断等操作。
            ParseHttpRequest* http = new ParseHttpRequest(recvbuff);

            char* sendbuff = (char*)malloc(1024);
            memset(sendbuff, '\0', sizeof(sendbuff));
            strncpy(sendbuff,(char*)dealing(http).c_str(), 1024);

            std::cout << "sendbuff:\n" << sendbuff << std::endl;
            send(client, sendbuff, ((std::string)sendbuff).size(), 0);
            close(client);
            exit(0);
        } else {
            close(client);
        }
    }

    close(sfd);
}

 