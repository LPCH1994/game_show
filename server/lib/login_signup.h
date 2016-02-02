
#define SIGNUP_SUCCESS 12

// kiem tra user da ton tai hay chua
int check_user(user u)
{

	FILE *fp;
	user temp;
	fp=fopen("user.dat","rb");
	while(!feof(fp))
	{
		fread(&temp,1,sizeof(user),fp);
		if(!strcmp(u.account,temp.account))
		{
			if(!strcmp(u.password,temp.password)) {
				fclose(fp);
				return SUCCESS;
			}
			else {
				fclose(fp);
				return LOGIN_FAIL;
			}
		}
	}
	fclose(fp);
	return SIGNUP_SUCCESS;
}

// ham kiem tra login
void s_login(protocol *p)
{
	int temp;
	temp = check_user(p->u);
	if(temp!=SIGNUP_SUCCESS) p->flag=temp;
	else p->flag=NO_ACCOUNT;

}
//ham kiem tra signup, dong thoi moi khi signup se tao ra 1 file
//luu diem cho nguoi choi
void s_signup(protocol *p)
{
	FILE *fp,*fp1;
	int temp;
	char account[50]="user/";
	temp=check_user(p->u);
	if(temp==SIGNUP_SUCCESS) {
		p->flag=SUCCESS;
		fp=fopen("user.dat","ab");
		fwrite(&(p->u),1,sizeof(user),fp);
		fclose(fp);
		strcat(account,p->u.account);
		strcat(account,".dat");
		fp1=fopen(account,"wb");
		fclose(fp1);
	}
	else p->flag= SIGNUP_FAIL;
}




