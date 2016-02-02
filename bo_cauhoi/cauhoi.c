#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct cauhoi{
	char cauhoi[100];
	char dapan1[20];
	char dapan2[20];
	char dapan3[20];
	char dapan4[20];
	char dapan_dung;
};
struct cauhoi nhap_cauhoi(i)
{
	struct cauhoi ch;
	printf("nhap cau hoi %d: \n",i);
	gets(ch.cauhoi);
	printf("nhap dap an 1:\n");
	gets(ch.dapan1);
	printf("nhap dap an 2:\n");
	gets(ch.dapan2);
	printf("nhap dap an 3:\n");
	gets(ch.dapan3);
	printf("nhap dap an 4:\n");
	gets(ch.dapan4);
	printf("nhap dan an dung:\n");
	ch.dapan_dung=getchar();
	while(getchar()!='\n');
	return ch;
}

void them_cauhoi(){
	int i=1;
	struct cauhoi ch;
	char temp[5];
	FILE *fp;
	char bo_cauhoi[15];
	printf("bo cau hoi ban muon them:\n");
	gets(bo_cauhoi);
	strcat(bo_cauhoi,".dat");
	fp=fopen(bo_cauhoi,"wb");
	do{
		ch=nhap_cauhoi(i);
		fwrite(&ch,sizeof(struct cauhoi),1,fp);
		printf("tiep tuc khong (y/n)\n");
		gets(temp);
		i++;
	}while(!strcmp(temp,"y"));
	fclose(fp);
}
struct cauhoi *lay_cauhoi(char bo_cauhoi[15],int vi_tri)
{

	struct cauhoi *ch;
	FILE *fp;
	strcat(bo_cauhoi,".dat");
	ch=(struct cauhoi*)malloc(sizeof(struct cauhoi));
	fp= fopen(bo_cauhoi,"r+b");
	if(fp==NULL){
		printf("khong ton tai bo cau hoi tren.\n");
		return NULL;
	}
	if(fseek(fp,vi_tri*sizeof(struct cauhoi),SEEK_SET)!=0){
		printf("Fseek failed!!\n");
		return NULL;
	}
	fread(ch,sizeof(struct cauhoi),1,fp);
	fclose(fp);
	return ch;
}
void in_cauhoi(struct cauhoi ch)
{
	printf("%s\n",ch.cauhoi );
	printf("1.%s\n",ch.dapan1);
	printf("2.%s\n",ch.dapan2);
	printf("3.%s\n",ch.dapan3);
	printf("4.%s\n",ch.dapan4);
}
int main()
{
	char bo_cauhoi[15];
	struct cauhoi *ch;
	char temp[5];
	int check,vi_tri;
	while(1){
		printf("1.them cau hoi\n");
		printf("2.xem cau hoi trong bo cau hoi\n");
		scanf("%d",&check);
		while(getchar()!='\n');
		if(check ==1){
			do{
			them_cauhoi();
			printf("ban muon tao them bo cau hoi nua khong (y/n)\n");
			gets(temp);
			}while(!strcmp(temp,"y"));
			continue;
		}
		if(check ==2){
			ch=(struct cauhoi*)malloc(sizeof(struct cauhoi));
			printf("nhap bo cau hoi muon xem\n");
			gets(bo_cauhoi);
			printf("nhap vi tri trong bo cau hoi\n");
			scanf("%d",&vi_tri);
			ch=lay_cauhoi(bo_cauhoi,vi_tri);
			in_cauhoi(ch[0]);
		}
		
	}
	
}