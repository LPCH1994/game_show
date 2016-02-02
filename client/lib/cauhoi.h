#include <unistd.h>
#include <poll.h>
#include "protocol.h"

#define TIME_OUT 15000
// thoi gian cho viec tra loi cau hoi
// neu ko tra loi, cau tra loi mac dinh la 'F'
char get_answer(int time_out)
{
    struct pollfd mypoll = { STDIN_FILENO, POLLIN|POLLPRI };
    char answer;

    if( poll(&mypoll, 1, time_out) )
    {
        scanf("%c", &answer);
        while(getchar()!='\n');
        return answer;
    }
    else return 'F';
}
void in_cauhoi(cauhoi ch)
{
	printf("  %s\n\n",ch.cauhoi );
	printf("---------------------------------------\n");
	printf("\tA.%s\n",ch.dapan1);
	printf("\tB.%s\n",ch.dapan2);
	printf("\tC.%s\n",ch.dapan3);
	printf("\tD.%s\n",ch.dapan4);
}

