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



class TCPserver{
    public:
        int lisenfd;
        struct sockaddr_in servaddr;
        uint16_t server_port;
        int clientfd;

        TCPserver(){
            lisenfd = -1;
            clientfd = -1;
            memset(&servaddr, 0, sizeof(servaddr));
            servaddr.sin_family = AF_INET;
            server_port = 0; 
        }

        bool TCPserver_connect(uint16_t m_port){
            server_port = m_port;
            lisenfd = socket(AF_INET,SOCK_STREAM,0);
            if(lisenfd == -1){
                perror("socket");
                return false;
            }

            memset(&servaddr,0,sizeof(servaddr));
            servaddr.sin_family = AF_INET;
            servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//means all ips can be used to communicate in client
            servaddr.sin_addr.s_addr = inet_addr("192.168.1.100");//means only yhis ip can used to communicate
            servaddr.sin_port = htons(server_port);

            if(bind(lisenfd,(struct sockaddr*)&(servaddr),sizeof(servaddr))!=0){
                perror("bind");
                close(lisenfd);
                return false;
            }

            if(listen(lisenfd,5) != 0){
                perror("listen");
                close(lisenfd);
                return false;
            }

            clientfd = accept(lisenfd,0,0);

            if(clientfd == -1){
                perror("accept");
                return false;
            }

            return true;
        }

        ~TCPserver(){
            close(lisenfd);
            close(clientfd);
        }
};
       
int main(int argc,char *argv[]){


    TCPserver TcpserverObj;

    //1 set server socket
    //2 set ip addr and port number to socket
    //struct sockaddr_in servaddr;//set addr and port to this struct
    uint16_t temp_port = atoi(argv[1]);
    if(TcpserverObj.TCPserver_connect(temp_port)== false){
        perror("server connect failed");
        return -1;
    }

    //2.5   bind the ip and port to server(dispatch ip and port to socket)

    //3 set socket to listen status


    //4 accept the request of client

    cout<<"client is connected!"<<endl;

    //5 send and recv message from client
    char buffer[1024];
    while(1){
        int iret;
        memset(buffer,0,sizeof(buffer));

        if((iret = recv(TcpserverObj.clientfd,buffer,sizeof(buffer),0))<=0){
            cout<<"iret ="<<iret<<endl;
        }
        cout<<"recv:"<<buffer<<endl;

        strcpy(buffer,"OK desu~");

        if((iret = send(TcpserverObj.clientfd,buffer,strlen(buffer),0))<=0){
            perror("send");
            break;
        }
        cout<<"recv: "<<buffer<<endl;
    }
}