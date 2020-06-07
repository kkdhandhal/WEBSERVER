#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h> 
#define PORT 8080
#define buff_size 512

using namespace std;

sig_atomic_t stopflag = 0;
char* route(const char *);
int _sendfile(int,char *);
void send_now(int fd,char *msg)
{
    int len = strlen(msg);
    printf("%s \n",msg);
    if(send(fd,msg,len,0)==-1)
    {
        printf("Error in send \n");
    }
}

char *substr(char *str, int count,const char * delm)
{
    char *tmp;
    char *result;
    int i;
    tmp = strtok(str, delm);
    result =tmp;
    for(i=0;i<=count-2;i++)
    {
        tmp= strtok(NULL,delm);
        result = tmp;
    }
    return result;
}
void signalhandler(int )
{
    stopflag = 1;
}
int main()
{
    int sockfd, newsockfd, portno, n;
    socklen_t clientlen;
    char buffer[buff_size];
    char r_path[buff_size];
    struct sockaddr_in serv_addr, client_addr;
    char * tocken;
    char * path;
    printf("Initilizing Socket \n");
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    //close(sockfd);
    signal(SIGINT, &signalhandler);
    if(sockfd < 0)
    {
        printf("Error Opening Socket \n");
        close(sockfd);
        exit(0);
    }
    else
    {
        printf("Connected Sucessfuly \n");
       
    }
    
    bzero(&serv_addr, sizeof(serv_addr));   
    
    portno =PORT;
    serv_addr.sin_family =  AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port=htons(portno);
    int flag=1;
    if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag))==-1)
    {
        printf("socket failed");
        close(sockfd);
    }
    if(bind(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr) ) < 0)
    {
        printf("Error on binding \n");
        printf("Error code: %d\n", errno);
        close(sockfd);
        exit(0);
    }
    else
    {
        printf("Socket binding is sucessfully \n");
       
    }
    
    if(listen(sockfd,5)<0)
    {
        printf("listen failed \n");
        close(sockfd);
        exit(0);
    }
    else
    {
        printf("Server Listing... \n");
    }
    
    while(stopflag == 0)
    {
        printf("while start \n");
        clientlen = sizeof(client_addr);
        newsockfd  = accept(sockfd,(struct sockaddr *) &client_addr, &clientlen);
        if(newsockfd<0)
        {
            printf("server accept failed \n");
            close(sockfd);
            exit(0);
        }
        else
        {
            printf("server accept the client \n");
        }
   
        //bzero(buffer,sizeof(buffer));
        // read(sockfd,buffer,sizeof(buffer));
        printf("receiving \n");
        recv(newsockfd, buffer,buff_size,0);
       // printf("responce is :: %s",buffer);
        tocken = strtok(buffer,"\r\r\n");
        strcpy(r_path,substr(tocken,2," "));
        printf("From Client :::::: %s \n",r_path);
        
        //bzero(buffer,sizeof(buffer));
         printf("From Client :::::: %s \n",r_path);
        strcpy(r_path,route(r_path));
        printf("\n\n PATH IS ---- -- -- ::: %s",r_path);
        //bzero(buffer,sizeof(buffer));
        if(_sendfile(newsockfd,r_path)>0)
        {
            printf("Send Succesflyy");
        }
        //n=0;
       // strcpy(buffer,"HTTP/1.1 200 OK\r\n\n<!doctype html><html><head><script src=\"js/script.js\"></script></head>this hi c from<html>");

       // send_now(newsockfd, (char *)"HTTP/1.1 200 OK\r\n\nContent-Type: text/html; charset=UTF-8\r\n\n\n\n<html><head><title>An Example Page</title></head><body><p>Hello World, this is a very simple HTML document.</p></body></html>");
          //  send(newsockfd,buffer,sizeof(buffer),0)
        /* if(send(newsockfd,buffer,sizeof(buffer),0)==-1)
        {
             printf("send fail");
        }
        else
        {
            printf("send sucessfull \n");
        } */
        
        //send_now(newsockfd, (char *)"Content-Type: text/html; charset=UTF-8\r\n\n");
        //send_now(newsockfd, (char *)"Content-Length: 155\r\n\n");
        //send_now(newsockfd, (char *)"Server: C Web/1.0\r\n\n");
        //send_now(newsockfd, (char *)"Connection: close\r\n\n\n\n");
        //send_now(newsockfd, (char *)"<html><head><title>An Example Page</title></head><body><p>Hello World, this is a very simple HTML document.</p></body></html>");
        
        if(close(newsockfd)==-1)
        {
            printf("error in close new socket");
        }
        else
        {
                printf("close newsockfd \n");
        }
       
       // close(newsockfd);
       
    }
    
    close(sockfd);
    
    
    printf("Socket closed now \n");
    return 0;
}

char* route(const char * g_path)
{
    char * r_path;
    printf("\n g_path is %s",g_path);
    if(strcmp(g_path,"/")==0)
    {
        strcpy(r_path,"index.html");
    }
    else
    {
        strcpy(r_path,g_path);
    }
  
    printf("\nnew R Path is :%s",r_path);
    return r_path;
}

int _sendfile(int sockfd, char *s_path)
{

    FILE *fp;
    char buff[buff_size];
    off_t offset = 0;
    struct stat stat_buff;
    fp = fopen(s_path,"rb");
    int n;

    printf("\n\nSend file path is ---- %s\n",s_path);
    if(fp==NULL)
    {
        return 0;    
    }
    else
    {
        bzero(buff,buff_size);
        strcpy(buff,"HTTP/1.1 200 OK\r\n\n");
       /* if(send(sockfd,buff,buff_size,0)<0)
        {
            printf("Error: Failed to send %s\n ",buff);
            return 0;
        }*/
        fseek(fp, 0, SEEK_SET);
        bzero(buff,buff_size);
        while(!feof(fp))
        {
            n=fread(buff,sizeof(char),buff_size,fp);    
            
            if(send(sockfd,buff,buff_size,0)<0)
            {
                printf("ERROR: Failed to send file %s.\n", s_path);
	            return 0;
            }
            else
            {
                 printf("\n %s",buff);
            }
            
           // sleep(1);
            bzero(buff,buff_size);
        }
    }
    return 1;

}