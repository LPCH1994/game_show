// diem so theo luong cau hoi ma nguoi choi qua duoc
int score(int count)
{
	switch(count){
		case 1: return 10;
		case 2: return 20;
		case 3: return 40;
		case 4: return 80;
		case 5: return 100;
		case 6: return 200;
		case 7: return 500;
		case 8: return 800;
		case 9: return 1000;
		case 10: return 2000;
		case 11: return 4000;
		case 12: return 6000;
		case 13: return 8000;
		case 14: return 10000;
		case 15: return 15000;
		default: return 0;
	}
}
// sinh so ngau nhien khac dap an dung
int ngaunhien(int i)
{
    int j;
    while(1){
            srand(time(NULL));
            j=1+rand()%4;
            if(j!=i) break;
    }
    return j;
}
//in dap an chi dinh trong bo cau hoi
void in_dapan(cauhoi ch,int i){
	switch(i){
		case 1: printf("	A.%s\n",ch.dapan1); break;
		case 2: printf("	B.%s\n",ch.dapan2); break;
		case 3: printf("	C.%s\n",ch.dapan3); break;
		case 4: printf("	D.%s\n",ch.dapan4); break;
	}
}
//in dap an sau khi lua trong 50-50
void in_50_50(cauhoi ch,int i,int j)
{
	printf("  %s\n",ch.cauhoi);
	if(i<j){
		in_dapan(ch,i);
		in_dapan(ch,j);
	}
	else{
		in_dapan(ch,j);
		in_dapan(ch,i);
	}
}
int _50_50(cauhoi ch)
{
	char temp;
	int j;
	temp=ch.dapan_dung;
	switch(temp){
		case 'a': j=ngaunhien(1);
				  in_50_50(ch,1,j);
				  break;
		case 'b': j=ngaunhien(2);
				  in_50_50(ch,2,j);
				  break;
		case 'c': j=ngaunhien(3);
				  in_50_50(ch,3,j);
				  break;
		case 'd': j=ngaunhien(4);
				  in_50_50(ch,4,j);
				  break;
	}

}
void tro_giup(int t)
{
	switch(t){
		case 0: printf("		Het quyen tro giup\n");
				printf("\n		tro giup: 50-50:x   change-ques:x\n\n");break;
		case 1: printf("		50-50 press key h\n");
				printf("\n		tro giup: 50-50:o   change-ques:x\n\n");break;
		case 2: printf("		change-ques press key g\n");
				printf("\n		tro giup: 50-50:x   change-ques:o\n\n");break;
		case 3: printf("		50-50 press key h\n");
				printf("		change-ques press key g\n");
				printf("\n		tro giup: 50-50:o   change-ques:o\n\n");break;
	}
}
