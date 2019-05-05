#include <stdio.h>
#include "csapp.h"
#include "cache.h"
/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 16777216 
#define MAX_OBJECT_SIZE 8388608 


/* You won't lose style points for including this long line in your code */
//Pre-declartion of request header and line
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
static const char *connection_hdr = "Connection: close\r\n";
static const char *proxy_connection_hdr = "Proxy-Connection: close\r\n";
static const char *host_hdr = "Host:%s\r\n";
static const char *request_hdr = "GET %s HTTP/1.0\r\n";
static const char *end = "\r\n";


//Pre_declartion of function
void doit(int connection_fd);
void parse_uri(char *uri,char *host,char *path,int *port);
void generate_request(char  *request, char *host, char *path);
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);

//Global value cache
pCache *cache;



//Main, a while loop continue accept the client's requenst and connection. It's almost same 
//as the main function in tiny.c on the textbook. 
int main(int argc, char **argv)
{
    int listenfd,connection_fd;
    char hostname[MAXLINE]; 
    char port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    cache = Malloc(sizeof(pCache));
    init_cache(cache);

    //check if it's a vaild argument
    if(argc != 2){
       fprintf(stderr, "usage: %s <port>\n", argv[0]);
       exit(1); 
    }
    //As mentioned in instruction, the SignPipe should be ignored 
    Signal(SIGPIPE,SIG_IGN);
    //Opening the listening fd to accept the connection from user using the assigned port
    listenfd = Open_listenfd(argv[1]);
    while(1){
      clientlen = sizeof(clientaddr);
      connection_fd = Accept(listenfd,(SA *)&clientaddr,&clientlen);
      //print the accepted information in proxy
      Getnameinfo((SA*)&clientaddr,clientlen,hostname,MAXLINE,port,MAXLINE,0);
      printf("Accepted connection from(%s, %s)\n",hostname, port);
      //execute client's request
      doit(connection_fd);
    }
    //after finish
    free_cache(cache);     
    return 0;
}


/*In doit function, we need to run client's each request*/
void doit(int connection_fd){
  char buf[MAXLINE], method[MAXLINE],uri[MAXLINE], version[MAXLINE];
  char host[MAXLINE],path[MAXLINE],uri_nport[MAXLINE];
  char host_copy[MAXLINE],path_copy[MAXLINE];
  int port;
  rio_t client_rio, server_rio;
  int server_fd = 0;
  char request[MAXLINE]; 
  int n = 0;
  char portS[100];
  //initilize the node to be used
  cNode *c_node = Malloc(sizeof(cNode));
  cNode *c_node1 = Malloc(sizeof(cNode));
  char *webContent = Malloc(MAX_OBJECT_SIZE);
  strcpy(webContent, "");
  int sizeContent = 0;

  //start to read in client's request
  Rio_readinitb(&client_rio, connection_fd);
  if(!Rio_readlineb(&client_rio,buf, MAXLINE)){
     printf("Read is not successful!\n");
     return;}
  //get method, uri, and version from buf
  sscanf(buf,"%s %s %s",method,uri,version);
  
  //compare if it's a GET command,if not GET command, tell users we are unable to run this request
  if(strcasecmp(method,"GET")){
     clienterror(connection_fd, method, "501", "Not Implemented", "Proxy does not implement this method");
     return;}
   //divide the uri into host, path, and port
   parse_uri(uri,host,path,&port);
   //create the uri_index, which is uri_nport here and doesn't include the port
   strcpy(host_copy,host);
   strcpy(path_copy,path); 
   strcat(uri_nport,host);
   strcat(uri_nport,path);  
   if((c_node = search_uri(cache,uri_nport))!= NULL){
      //read from cache's webcontent, if there is the same request in the
      //cache and send it back to the user, then clsoe the connection_fd.
      //Finally, retrun to the main.
     Rio_writen(connection_fd,c_node->web_content,c_node->content_size);   
     Close(connection_fd);
     return;}
   else{ 
     //generate the HTTP request using host and path
     generate_request(request,host,path); 
     sprintf(portS, "%d",port); 
     //Open the connection to the server
     server_fd = Open_clientfd(host,portS);
     if(server_fd<0){
         printf("connection failed\n");
         return;}
     //start send the request to server
     Rio_readinitb(&server_rio,server_fd);
     Rio_writen(server_fd,request,strlen(request));
      
     //when write back, if it's size is enough, keeping add the content 
     //into web content, and increase the content size by adding n
     while((n = Rio_readlineb(&server_rio,buf,MAXLINE))!=0){
      if(sizeContent<MAX_OBJECT_SIZE){
        strcat(webContent,buf);
        sizeContent+=n;
        }
        printf("proxy received %d bytes,then send\n",n);
        //send it back to user
        Rio_writen(connection_fd,buf,n);
    }
   //close the connection fd
   Close(server_fd); 
   Close(connection_fd);
   //store the new webcontent in to cache
    if(sizeContent <MAX_OBJECT_SIZE){
     //initlize the cache block and insert it into cache
     init_cacheNode(c_node1,uri_nport,webContent,sizeContent);
     insert_cacheNode(cache,c_node1);
     //print(cache);
    
   }
    //free the assigned memory and empty the uri for next time use
   strcpy(uri_nport,"");
   free(webContent);
   return;
  }
}

/*In parse_uri function, we divide the uri into three parts, hostname
, path and port for later use.
*/
void parse_uri(char *uri, char *host, char *path, int *port){
   //initalize a empty pointer
   char *host_pos = NULL;
   //check if it's a vaild uri, return the error
   if(strncasecmp(uri,"http://",7)!= 0){//if doesn't has the http header
       fprintf(stderr,"Error: invalid uri!\n");
       exit(0); 
   }
   else{
     //if it is a vail a request, skip the http://
     host_pos = uri + 7;
   }
   //Then, we will have four cases need to be dealt with
   char *port_pos = strchr(host_pos,':');
   char *path_pos = strchr(host_pos,'/');
   if(port_pos ==  NULL){// if not port
      *port = 80;
      if (path_pos == NULL){// if not path
         sscanf(host_pos, "%s",host);
          } 
      else{//if path
         *path_pos='\0';
         sscanf(host_pos,"%s",host);
         *path_pos='/';
         sscanf(path_pos + 1,"%s",path);
      }
   }
   else{//if port
        *port_pos='\0';
      if(path_pos == NULL)//if not path
      {  sscanf(host_pos,"%s",host);
         *port_pos='/';
         sscanf(port_pos + 1,"%d",port);     
      }else{ // if all 
        sscanf(host_pos, "%s", host);
        sscanf(port_pos + 1, "%d%s", port, path);
      }     
    }   
   return;
}
/*generate a request by combine them together
 */
void generate_request(char *request,char *host, char *path){
    char request_line[MAXLINE],host_line[MAXLINE];
    sprintf(request_line, request_hdr, path);
    sprintf(host_line, host_hdr, host);
    sprintf(request,"%s%s%s%s%s%s", request_line, host_line, connection_hdr, proxy_connection_hdr, user_agent_hdr, end);
    return;
}
/*error return function, which is the same as the one in tiny.c 
*/
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg){
  char buf[MAXLINE], body[MAXBUF];
  // Build the HTTP response body, copy this from book.
  sprintf(body, "<html><title>Tiny Error</title>");
  sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
  sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
  sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
  sprintf(body, "%s<hr><em>The Proxy Web server</em>\r\n", body);
  //  Print the HTTP response 
  sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
  Rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "Content-type: text/html\r\n");
  Rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
  Rio_writen(fd, buf, strlen(buf));
  Rio_writen(fd, body, strlen(body));
}
