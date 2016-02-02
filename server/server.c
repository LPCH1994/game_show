#include "start.h"

int main()
{
	int n;
	while(1){
		printf("1.START\n");
		printf("2.SUA CAU HOI\n");
		printf("3.QUIT\n");
		scanf("%d",&n);
		while(getchar()!='\n');
		if(n==1) start_server();
		if(n==2) sua_cauhoi();
		if(n==3) break;
	}

}