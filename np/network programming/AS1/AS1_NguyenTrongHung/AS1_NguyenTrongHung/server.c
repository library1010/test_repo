#include <sys/types.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "user.h"

Player player[100];
PlayingChess playing[100];
int online = 0;// Luu so acc online	
int idx = 0; // Chi so mang cua Playingchess					
int main(){

  int server_sockfd, client_sockfd;
  int server_len, client_len,bytes_recieved ;

  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;
  
  char acc[60];
  int status,dem;
  char recv_data[1024],send_data[1024],recvdata[1024];
  char win[1024]="WIN";
  char end[1024]="THEEND";
  int result,doithu;

  struct timeval tv;
  fd_set fdset, testfds;

  server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(9091);
  server_len = sizeof(server_addr);
  
  //Create a TCP/IP socket to use
  if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
     perror("Socket() error");
     exit(1);
  }
  if (bind(server_sockfd, (struct sockaddr *)&server_addr, server_len)== -1) {
     perror("bind() error");
     exit(1);
  }
  if (listen(server_sockfd, 5) == -1) {
     perror("Listen");
     exit(1);
  }

  FD_ZERO(&fdset);    /*clear all bits in fdset*/
  FD_SET(server_sockfd, &fdset);  /*turn on the bit for fd in fdset*/

  while(1) {
    char ch;
    int fd;
    int nread;
    
    /* wait until either socket has data ready to be recv()d (timeout 5.5 secs)*/

    tv.tv_sec = 5;
    tv.tv_usec = 500000;
    testfds = fdset;
    printf("Server waiting\n");
    
    result = select(FD_SETSIZE, &testfds, (fd_set *)NULL,(fd_set *)NULL, &tv);
    
    if(result == -1) {
       perror("Select() error");   // error occurred in select() 
       exit(1);
    }
    else if(result == 0){
       printf("Timeout occurred! No data after 5.5 seconds.\n");	
       
    }
    else{
    	//Client connected
      for(fd = 0; fd < FD_SETSIZE; fd++){
      if(FD_ISSET(fd,&testfds)) {
	  if(fd == server_sockfd){
	     client_len = sizeof(client_addr);
	     client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_addr, &client_len);
	     FD_SET(client_sockfd, &fdset);
	     printf("Client %d connected !\n", client_sockfd);
	  }else {
	     ioctl(fd, FIONREAD, &nread); //???
	     if(nread != 0) {
				printf("\nThong tin dang duoc xu ly tai fd %d\n",fd);
              	bytes_recieved = recv(fd,recv_data,1024,0);
              	recv_data[bytes_recieved] = '\0';
              	fflush(stdout);	
              	puts(recv_data);
              	// Client dang ki Account
              	if(strcmp(recv_data,"REG") == 0){
              		do{
              			bytes_recieved = recv(fd,acc,60,0);
              			acc[bytes_recieved] = '\0';              				
              			fflush(stdout);	
              			status = checkAcc(acc,1,player,online);
              			if(status==0){
              				strcpy(send_data,"Username is exit\n");
              				send(fd,send_data,1024,0);
              			}
              			else if(status == 5){
              				strcpy(send_data,"Username is incorrect(a-z,A-Z,0-9) ! Please try again!\n");
              				send(fd,send_data,1024,0);
              			}
              			else if(status == 6){
              				strcpy(send_data,"Password is incorrect(a-z,A-Z,0-9) ! Please try again!\n");
              				send(fd,send_data,1024,0);
              			}
              			else break;
              		}while(1);
              		fflush(stdout);	
              		strcpy(send_data,"OK"); // Dang ki thanh cong
              		send(fd,send_data,1024,0);              			
              	}
              	// ---------------------------------------------------------Client login
              	else if(strcmp(recv_data,"LOGIN") == 0){
              		printf("\nLoi o day?\n");
              		checkLogin(acc,fd,player,&online,playing,&idx);
              	}
              	// ------------------------------------------------------------Thoat
              	else if(strcmp(recv_data,"LOGOUT")==0){
              		for(dem=0;dem<online;dem++){
						if(player[dem].fd == fd){
							do{
							strcpy(player[dem].name,player[dem+1].name);
							player[dem].fd= player[dem+1].fd;
							dem++;
							}while(dem< online);
							online--;
							break;
						}
                	}
                	removeChessPlayer(fd,playing,&idx);
					close(fd);
					FD_CLR(fd, &fdset);  /*turn off the bit for fd in fdset*/
					printf("Not connect client on fd %d\n", fd);
              	}
              	else if(strcmp(recv_data,"LAPGAME") == 0){
					connectGame(playing,&idx,player,&online,fd);
              	}
              	// ________________________________________-Nhan thong tin game
              	else {
              	bytes_recieved= recv(fd,recvdata,1024,0);
              	recvdata[bytes_recieved]= '\0';
              	printf("\n________Cac player dang choi________\n");
              	viewChessPlayer(playing,&idx);
              	doithu= searchPlaying(fd,playing,idx);
     			send(doithu,recv_data,1024,0);
     			send(doithu,recvdata,1024,0);
     			puts(recv_data);
     			puts(recvdata);
     			if(strcmp(recvdata,"WIN")==0|| strcmp(recvdata,"END")==0)
     				removeChessPlayer(fd,playing,&idx);
              	}
		} 
	    else {
	     		for(dem=0;dem<online;dem++){
						if(player[dem].fd == fd){
							do{
							strcpy(player[dem].name,player[dem+1].name);
							player[dem].fd= player[dem+1].fd;
							dem++;
							}while(dem< online);
							online--;
							break;
						}
                }
                removeChessPlayer(fd,playing,&idx);
            	close(fd);
				FD_CLR(fd, &fdset);  /*turn off the bit for fd in fdset*/
				printf("Not connect client on fd %d\n", fd);
           	}
	  	}
       }
     }
   }
 }
  close(server_sockfd);
}