//------------------------------------
//cac ham de an mat khau tren terminal

static struct termios old, new;
void initTermios(int echo) {
	tcgetattr(0, &old);                       
	new = old;                                
	new.c_lflag &= ~ICANON;                   
	new.c_lflag &= echo ? ECHO : ~ECHO;       
	tcsetattr(0, TCSANOW, &new);              
}

void resetTermios(void) {
  tcsetattr(0, TCSANOW, &old);
}

char getch_(int echo) {
	char ch;
	initTermios(echo);
	ch = getchar();
	resetTermios();
	return ch;
}

char getch(void) {
    return getch_(0);
}

int get_pass(char pass[]){
	int i=0;

	while(1){
		pass[i]=getch();
		if(pass[i]== '\n') break;
		printf("*");
		i++;
	}
	pass[i]='\0';
	return 1;
}
//---------------------------------------------

#define SIGNUP_SUCCESS 12
//tao giao dien dang nhap va luu vao giao thuc chuyen di

void c_login(protocol *p)
{
	p->flag = LOGIN;
	printf("Account:\n");
	gets(p->u.account);
	printf("Password:\n");
	get_pass(p->u.password);
}
//tao giao dien dang nhap va luu vao giao thuc chuyen di
int c_signup(protocol *p)
{
	char t;
	char temp[32];
	p->flag = SIGNUP;
	do{
		printf("Account:\n");
		gets(p->u.account);
		printf("Password:\n");
		get_pass(p->u.password);
		printf("\nComfirmation:\n");
		get_pass(temp);
		if(!strcmp(p->u.password,temp)){
			return SUCCESS;
		}
		else {
			printf("%s\n",SIGNUP_ERROR1);
			printf("Do you want enter again(y/n)\n");
			t=getchar();
			while(getchar()!='\n');
		}
	}while(t=='y');
	return SIGNUP_FAIL;
}




