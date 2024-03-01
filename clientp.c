#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>//necesario
#include <fcntl.h>
#include <termios.h>//necesaria
#include <pthread.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>

int X,Y,sX=11,sY=94,score=0, end=0,size=100,flag=1,o_score=0,chances=3,stop_sig=0;
char key,XD,YD,me[20],name[20];
int s_fd,a_fd,aX,aY;

void score_board(int f);
/**********************************************/


void  data_transfer_r(){
char buff[size];
int s,co[5];

bzero(co,sizeof(co));
recv(s_fd, co, sizeof(co), 0);
if(co[4]==1){
score+=1;
clear();
score_board(1);
refresh();
sleep(2);
endwin();
exit(1);
}
X=co[0];
Y=43;
XD='R';
YD='U';
aY=+1;   
aX=co[1];
o_score=co[2];
if(aX==+1)
YD='D';
flag=co[3];
stop_sig=0;
}


/***********************************************/


void data_transfer_s(int a,int b){
int size=100,c[5];
char buff[size];
move(X,Y);
addstr(" ");
refresh();
c[0]=X;
c[1]=-1;
c[2]=score;
if(YD=='D')
c[1]=+1;
c[2]=score;
c[3]=a;
c[4]=b;
send(s_fd, c, sizeof(c), 0);

aX=0;
aY=0;
}

/***********************************************/

void build_connection()
{

int b_fd,l_fd,a_fd,len;
int port=8080;
char *ip="127.0.0.1";

struct sockaddr_in c_addr;

bzero(&c_addr,sizeof(c_addr));
if((s_fd=socket(AF_INET,SOCK_STREAM,0))<0){
perror("[-] Error at socket creation");
exit(1);
}
printf("[+] Socket creation done\n");

c_addr.sin_family=AF_INET;
c_addr.sin_port=port;
c_addr.sin_addr.s_addr=atoi(ip);

len=sizeof(c_addr);
if((a_fd=connect(s_fd,(struct sockaddr *)&c_addr,len))<0){
perror("[-] Error at connecting server");
exit(1);
}
printf("[+] Connected successfully\n");
printf("Please start the chat\n");

}

/*******************************************************/

char kbhit(){
      struct termios oldt, newt;
      int ch;
      int oldf;    
      tcgetattr(STDIN_FILENO, &oldt);
      newt = oldt;
      newt.c_lflag &= ~(ICANON | ECHO);
      tcsetattr(STDIN_FILENO, TCSANOW, &newt);
      oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
      fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);    
      ch = getchar();    
      tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
      fcntl(STDIN_FILENO, F_SETFL, oldf);    
      if(ch != EOF){
        return ch;
        }    
      return '*';
      }

/*********************************************/


void print_border(){
move(9,41);
addstr("----------------------------------------------------");
for(int i=9;i<=38;i+=2){
   move(i,41);
   addstr(".");
}
move(38,41);
addstr("----------------------------------------------------");
refresh();
}


/********************************************/

int surf_board_contact_check(int o,int p,int s)
{
if( sY-1== p && sX<=o && sX+6>=o){
   score=s+1;
   return 1;
   }
else if(sY<=p && stop_sig==0){
   stop_sig=1;
   chances-=1;
   move(o,p);
   addstr(" ");
   score_board(0);
   refresh();
   if(chances==0){
   data_transfer_s(1,1);
   o_score+=1;
   clear();
   score_board(1);
   refresh();
   sleep(2);
   endwin();
   exit(1);
   }
   else
   data_transfer_s(1,0);
   return -1;
   }

return 0;
}

/********************************************/


void score_board(int f){
char sc[100];
if(f==1){
move(7,41);
if(score>o_score)
sprintf(sc,"%s is winnerr!!, final score is : %d",me, score);
else if(score<o_score)
sprintf(sc,"%s is winnerr!!, final score is : %d",name, score);
else if(score==o_score)
sprintf(sc,"It's a TIE!!, final score is : %d", score);
addstr(sc);
}
else{
move(7,41);
sprintf(sc,"my score: %d, %s's score: %d     chances: %d",score,name,o_score,chances);
addstr(sc);
}
}


/********************************************/


void * ping_pong(){

print_border();
score_board(0);
int  LE=41,RE=92,TP=9,BT=38,f,f1,speed=(rand()%1000000)+100000;
X=11;
Y=45;
start:
data_transfer_r();
if(flag==1){
X=(rand()%29)+9;
Y=69;
XD='R';
YD='D';
aX=+1;
aY=+1;
score+=1;
flag=0;
}
while(key!='q'){
   score_board(0);
   refresh();
   move(X+=aX,Y+=aY);
   addstr("o");
   refresh();
   usleep(speed);
   move(X,Y);
   addstr(" ");
   if(X==TP+1){
      if(Y==LE+1 || (XD=='R' && YD=='U') || f1==0){
         aX=+1;
         aY=+1;
         XD='R';
         YD='D';
      }
      else if(Y==RE-1 || (XD=='L' && YD=='U') || f1==1){
         aY=-1;
         aX=+1;
         XD='L';
         YD='D';
      }
      f=0;
      f1=-1;
   }

   else if(X==BT-1){
      if(Y==LE+1|| (XD=='R' && YD=='D') || f1==0){
         aY=+1;
         aX=-1;
         XD='R';
         YD='U';
      }
      else if(Y==RE-1 || (XD=='L' && YD=='D') || f1==1){
         aX=-1;
         aY=-1;
         XD='L';
         YD='U';
      }
      f=0;
      f1=-1;
   }

   else if(surf_board_contact_check(X,Y,score)==1){
      if(X==sX-1 || (XD=='R' && YD=='U') || f1==0){
         aX=-1;
         aY=-1;
         XD='L';
         YD='U';
      }
      else if(X==TP+1 || (XD=='R' && YD=='D') || f1==1){
         aY=-1;
         aX=+1;
         XD='L';
         YD='D';
      }
      f=0;
      f1=-1;
   }

   else if(Y==LE+1){
     data_transfer_s(0,0);
     goto start;
   }
   
   if(stop_sig==1)
   goto start;
   
}

}


/***********************************/

void *paddle(){
char c, b2[20]="#",clr[20]=" ";
while((key=kbhit())!='q'){
if(key=='w'){
move(sX+6,sY);
addstr(clr);
sX-=1;
move(sX,sY);
addstr(b2);
refresh();
}
if(key=='s'){
move(sX,sY);
addstr(clr);
refresh();
sX+=1;
move(sX+6,sY);
addstr(b2);
refresh();
}
else{
end=1;
}

}
}

/****************************************/
void main()
{
build_connection();
printf("Please share your name with client (NO SPACES USE UNDERSCORE) :\n");
scanf("%s",me);
if(recv(s_fd,name,20,0)<0){
perror("error in name sending:");
exit(1);
}
if(send(s_fd,me,20,0)<0){
perror("error in name sending:");
exit(1);
}

initscr();
clear();
pthread_t t1,t2,t3;
pthread_create(&t1,NULL,ping_pong,NULL);
pthread_create(&t2,NULL,paddle,NULL);

pthread_join(t1,NULL);
pthread_join(t2,NULL);


endwin();
}
