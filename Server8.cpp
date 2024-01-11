//this is a client demo 


#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
using namespace std;
       
int main(int argc,char *argv[]){
    if(argc != 2){
        cout<<"Using : ./Server \n Example:./Server 5005\n\n";
        cout<<"linux need to open port 5005 in firewall\n";
        return -1;
    }

    //1 set server socket
    int lisenfd = socket(AF_INET,SOCK_STREAM,0);
    if(lisenfd == -1){
        perror("socket");
        return -1;
    }

    //2 set ip addr and port number to socket
    struct sockaddr_in servaddr;//set addr and port to this struct
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//means all ips can be used to communicate in client
    servaddr.sin_addr.s_addr = inet_addr("192.168.0.100");//means only yhis ip can used to communicate
    servaddr.sin_port = htons(atoi(argv[1]));

    //2.5   bind the ip and port to server(dispatch ip and port to socket)
    if(bind(lisenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))!=0){
        perror("bind");
        close(lisenfd);
        return -1;
    }

    //3 set socket to listen status
    if(listen(lisenfd,5) != 0){
        perror("listen");
        close(lisenfd);
        return -1;
    }

    //4 accept the request of client
    int clientfd = accept(lisenfd,0,0);
    if(clientfd == -1){
        perror("accept");
        return -1;
    }

    cout<<"client is connected!"<<endl;

    //5 send and recv message from client
    char buffer[1024];
    while(1){
        int iret;
        memset(buffer,0,sizeof(buffer));

        if((iret = recv(clientfd,buffer,sizeof(buffer),0))<=0){
            cout<<"iret ="<<iret<<endl;
        }
        cout<<"recv:"<<buffer<<endl;

        strcpy(buffer,"OK desu~");

        if((iret = send(clientfd,buffer,strlen(buffer),0))<=0){
            perror("send");
            break;
        }
        cout<<"recv: "<<buffer<<endl;
    }

    close(lisenfd);
    close(clientfd);
}