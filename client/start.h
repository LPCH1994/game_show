#include "lib/cauhoi.h"
#include "lib/test.h"
#include "lib/trogiup.h"

//---------------------------------
int sockfd;

//---------------------------------
// delay game trong 4s truoc khi bat dau
void delay()
{
	printf(" The game will start in: \n");
	get_answer(1000);
	printf("3...\n");
	get_answer(1000);
	printf("2...\n");
	get_answer(1000);
	printf("1....\n");
	get_answer(1000);
}
// hien thi lich su nguoi choi 
void in_lichsu()
{
	protocol p;
	p.flag=VIEW_SCORE;
	printf("Lich su nguoi choi\n");
	while(1){
		send(sockfd,&p,sizeof(protocol),0);
		recv(sockfd,&p,sizeof(protocol),0);

		if(p.flag==DONE) break; //

		printf("Thoi gian: %s\n",p.sc.thoi_gian);
        printf("Diem so:%d\n",p.sc.score);
		 
	}
}
// phan khan gia , nhan thong tin cau hoi va dap an nguoi choi
void viewer()
{
	protocol p;
	p.flag=VIEWER;
	send(sockfd,&p,sizeof(protocol),0);
	while(1){
		recv(sockfd,&p,sizeof(protocol),0);
		if(p.flag==QUES){
			if(p.count>1)
			printf("Player answer : %c \n",p.answer );

			get_answer(1000);//
			
			in_cauhoi(p.ch);
		}
		if(p.flag==WRONG_ANSWER){
			printf("Player has wrong answer\n");
			printf("The correct answer is: %c\n",p.ch.dapan_dung);
			break;
		}
		if(p.flag==WIN){
			printf("Player win!!!\n");
			break;
		}
	}
}
// bat dau game
int start(protocol p)
{
	int temp;
	while(1){
		printf("Hello,%s\n",p.u.account);
		printf("1.Enter room and Watting for other user to play\n");
		printf("2.Xem lich su\n");
		printf("3.khan gia\n");
		printf("4.Quit\n");
		scanf("%d",&temp);
		while(getchar()!='\n');
		if(temp==4) break;
		if(temp == 1) {
		printf("Watting for other user....\n");
		start_game();
		}
		if(temp==2) in_lichsu();
		if(temp==3) viewer();
	}

}

int start_game()
{
	//--------------------------------------------
	// bien check trang thai cua tro giup
	int check_50_50=1,check_ch_q=1;
	int t=3; //bien check trang thai cua tro giup
	char temp;
	int count=0;
	protocol p;
	p.flag=ENTER_ROOM;
	int check=1;//bien check cau hoi dau tien
	//---------------------------------------------

	while(1){
		send(sockfd,&p,sizeof(protocol),0);
		recv(sockfd,&p,sizeof(protocol),0);
		switch(p.flag){
			case SUB_QUES: 	printf("Da du so luong nguoi choi\n");
							delay();
							printf("phan choi cau hoi phu bat dau\n");
							p.flag = SUB_ANSWER;
							in_cauhoi(p.ch);
							p.answer=get_answer(TIME_OUT);
							break;
			case QUES: 	if((temp=='g')) printf("Thay doi cau hoi\n"); 
						else printf("		Correct!!\n");
						get_answer(1000);
						if(check==1){
							printf("Chuc mung ban da la nguoi choi chinh\n");
							get_answer(1000);
							delay();
							check=0;
						}
						printf("-Ques %d\n",p.count);
						tro_giup(t);
						in_cauhoi(p.ch);
						temp=get_answer(TIME_OUT);
						if((temp=='h')&&(check_50_50==1)) {
							t=t-1; //t = 2 <=> con tro giup doi cau hoi 
							tro_giup(t);
							check_50_50=_50_50(p.ch);
							p.flag=ANSWER;
							temp=get_answer(TIME_OUT);
							//count=p.count;
						}
						else if((temp=='g')&&(check_ch_q==1)){
							p.flag=CHANGE_QUES;
							check_ch_q=0;
							t=t-2;
						}
						else {
							p.flag=ANSWER;
							p.answer=temp;
						}
						count=p.count;
						break;
			case WRONG_ANSWER:  if(p.answer=='F'){
									printf("%s\n",TIME_OUT_ERROR);
								}else printf("Wrong answer !!\n");
								break;
			case MUON: printf("You had answered too late\n");

		}
		if(p.flag==WIN){
			printf("CONGRATULATION,YOU ARE THE MILLIONAIRE\n");
			congra();
			break;
		}
		if(p.flag==FULL){
			printf("%s\n\n",FULL_ROOM);
			break;
		}
		if((p.flag == WRONG_ANSWER)||(p.flag==MUON)){
			printf("End Game :( :( :(\n\n");
			break;
			}
	}
	printf("\n\nYour score: %d\n\n\n",score(count));
}