#include <stdio.h>
#include <string.h>   
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   
#include <arpa/inet.h>    
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> 
#include <time.h>
#include "lib/cauhoi.h"
#include "lib/login_signup.h"
#include "lib/score.h"

#define FULL_CLIENT 10
#define PORT 5500
#define MAX_PLAYER 3
#define MAX_QUES 11
//-----------------------------------------
int listenSock;
//mang client de thao tac vs socket cua client ket noi den
int client[FULL_CLIENT];
//mang player de thao tac vs socket cua nguoi choi 
int player[MAX_PLAYER];
//mang viewer de thao tac vs socket cua khan gia ket noi den
int viewer[FULL_CLIENT];
fd_set readfds;
//bien check kiem tra nguoi choi nao tra loi nhanh nhat
int check=0,j=0,a=0,i,k=0,v=0; //cac bien check trang thai

//-----------------------------------------------


// tao listen Sock 
int create_listenSock(int port)
{
    struct sockaddr_in address;
    if( (listenSock = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
    {
        perror("socket failed");
        return -1;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    //bind socket
    if (bind(listenSock, (struct sockaddr *)&address, sizeof(address))<0) 
    {
        perror("bind failed");
        return -1;
    }
    printf("Listener on port %d \n", PORT);
    if (listen(listenSock, 3) < 0)
    {
        perror("listen");
        return -1;
    }
    puts("Waiting for connections ...");
    return 0;
}
//--------------------------------

//phan choi cau hoi phu
//nhan cau hoi vao va chuyen den cac client da~ ket noi
//vs tu cach la player
void play_phu(cauhoi *ch)
{
    protocol p;
    int i;   
    p.flag=SUB_QUES;
    ch_to_pro(&p,ch[0]);

    for(i=0;i<MAX_PLAYER;i++){
        send(player[i],&p,sizeof(protocol),0);
    }
    
}
//kiem tra dap an dung hay sai
// dung return 1
//sai return 0
int check_dapan(cauhoi *ch,char dap_an)
{
    if (ch->dapan_dung==dap_an) return 1;
    else return 0;
}
//chon ra nguoi choi chinh
//bien check dung de kiem tra nguoi choi nao
//tra loi nhanh nhat
void check_main_p(protocol *p, cauhoi *ch)
{
    if(check_dapan(ch,p->answer)){
        if(check==0){
            p->flag=QUES;
            p->count=1;
            check=1;
        }else p->flag=MUON;

    }else p->flag=WRONG_ANSWER;
}
//ham gui noi dung cau hoi va dap an cua nguoi choi chinh
//cho khan gia
void khan_gia(int flag,char answer,cauhoi *ch)
{
    int i;
    protocol p;
    p.flag=flag;
    p.answer=answer;
    ch_to_pro(&p,ch[0]);
    for(i=0;i<FULL_CLIENT;i++){
        if (viewer[i]>-1) send(viewer[i],&p,sizeof(protocol),0);
    }
    if((flag==WRONG_ANSWER)||(flag==WIN)){
        for (i = 0; i < FULL_CLIENT; i++) viewer[i]=-1;
    }
}
// choi du nguoi choi thi bat dau gui cau ho phu
// khi da du roi, neu co nguoi choi khac dang nhap
// se bao full phong
void enter_room(protocol *p,int sd)
{
        if((j==MAX_PLAYER)||(a==1)) p->flag=FULL;
            if(j<MAX_PLAYER){
                player[j]=sd;j++;
                printf("we have %d player\n",j);
            }
}
// check dap an cac nguoi
// tra loi nhanh nhat duoc choi tiep
void sub_answer(protocol *p,cauhoi *ch)
{
    check_main_p(p,ch);
    k++;
    if(p->flag==WRONG_ANSWER){
        j--;
        if(j==0) a=0;
    }
    if(k==MAX_PLAYER){k=0;check=0;}
}
// check dap an nguoi choi chinh,
//tra loi dung thi chuyen cau tiep theo
//dong thoi gui noi dung cau hoi cho khan gia
// neu chien thang hoac tra loi sai thi luu diem
void answer(char account[32],protocol *p,cauhoi *ch)
{
    int scr;
    if(check_dapan(ch,p->answer)){
    if(p->count==MAX_QUES){
          p->flag=WIN;
          khan_gia(WIN,p->answer,ch);
          scr=score(p->count);
          save_score(account,scr);
          j=0;a=0;
          p->count=1;

        }else {
            p->flag=QUES;
            p->count++;
        }
    }           
    else {
        p->flag=WRONG_ANSWER;
        khan_gia(WRONG_ANSWER,p->answer,ch);
        scr=score(p->count);
        printf("%d\t%s",scr,account);
        save_score(account,scr);
        p->count=1;a=0;j=0;
    }

}
//Server bat dau khoi dong
int start_server()
{   
    //--------------------------------------
    int connSock,max_sd,sd,nEvents;
    struct sockaddr_in address;  
    int addrlen = sizeof(address);

    //khoi tao client vs viewer =-1
    for(i=0;i<=FULL_CLIENT;i++) client[i]=-1;
    for(i=0;i<=FULL_CLIENT;i++) viewer[i]=-1;
    //khai bao 1 mang cac giao thuc de thao tac vs tung client
    //ket noi den
    protocol p[FULL_CLIENT];
    cauhoi *ch,*main_ch;
    char account[32][FULL_CLIENT];
    //----------------------------------------
    //tao listen sock de thao tac
    create_listenSock(PORT);

    while(1)
    {
        // phan xu ly ben server de tao lien ket vs cac client
        //---------------------------------------------
        FD_ZERO(&readfds);
        FD_SET(listenSock, &readfds);
        max_sd = listenSock;
        for ( i = 0 ; i < FULL_CLIENT ; i++) 
        {
            
            sd = client[i];
            if(sd > 0)
                FD_SET( sd , &readfds);
            if(sd > max_sd)
                max_sd = sd;
        }
        nEvents = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
        if (nEvents < 0) printf("select error...\n");
        if (FD_ISSET(listenSock, &readfds)) 
        {
            if ((connSock = accept(listenSock, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            printf("New connection , socket fd is %d \n" , connSock);
            // them socket vao socket list
            for (i = 0; i < FULL_CLIENT; i++) 
            {
                //neu vi tri chua co' socket toi

                if( client[i] == -1 )
                {
                    client[i] = connSock;
                    printf("Adding socket to socket list on %d\n" , i);
                    break;
                }
            }
        }

        //-------------------------------------------------------
        // bat dau vao phan thao tac du lieu vs client
        for (i = 0; i < FULL_CLIENT; i++) 
        {
            sd = client[i];
              
            if (FD_ISSET( sd , &readfds)) 
            {
                
                if (recv( sd , &p[i],sizeof(protocol),0) == 0)
                {
                    //neu client disconnect thi tuy vao trang thai
                    //cua no thi tra ve gia tri rieng
                    viewer[i]=-1;
                    if(p[i].flag==ENTER_ROOM){
                        j--;
                        if(j==0) a=0;
                    }
                    if(p[i].flag==QUES){
                        j=0;a=0;
                    }
                    printf("Client disconnected...,\n"); 
                    close( sd );
                    client[i] = -1;
                } 
                else{
                    switch(p[i].flag){
                    case LOGIN: s_login(&p[i]); strcpy(account[i],p[i].u.account);
                                break;
                            
                    case SIGNUP: s_signup(&p[i]); strcpy(account[i],p[i].u.account);
                                break;
                            
                    case ENTER_ROOM: enter_room(&p[i],sd); break;
                                
                    case SUB_ANSWER: sub_answer(&p[i],ch); break;
                                    
                    case ANSWER: answer(account[i],&p[i],main_ch); break;
                                 
                    case CHANGE_QUES: p[i].flag=QUES; break;
                    case VIEW_SCORE: if(get_score(account[i],&p[i],v)) v++;
                                        else{
                                            p[i].flag=DONE;
                                            v=0;
                                        }
                                    break;
                    case VIEWER: viewer[i]=sd;
                                    
                    }
                    if(p[i].flag==QUES){
                        srand(time(NULL));
                        int vitri=rand()%9;
                        main_ch=lay_cauhoi(p[i].count,vitri);
                        ch_to_pro(&p[i],main_ch[0]);
                        khan_gia(QUES,p[i].answer,main_ch);
                    }
                    if((j==MAX_PLAYER)&&(a==0)){
                        srand(time(NULL));
                        ch=lay_cauhoi(0,rand()%9);
                        play_phu(ch);
                        a=1;
                    }else{
                         if((p[i].flag!=ENTER_ROOM)&&(p[i].flag!=VIEWER)) 
                            send(sd,&p[i],sizeof(protocol),0);
                    }
                   
                }
                
            }
        }
    }
}
