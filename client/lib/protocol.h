// danh sach cac flag duoc su dung cho giao thuc
enum{
	LOGIN,SIGNUP,SUCCESS,LOGIN_FAIL,SIGNUP_FAIL,SUB_QUES,
	QUES,ANSWER,ENTER_ROOM,WRONG_ANSWER,SUB_ANSWER,NO_ACCOUNT,
	FULL,WIN,MUON,CHANGE_QUES,VIEW_SCORE,DONE,VIEWER,OUT
};
// luu tru diem so va thoi gian cua nguoi choi
typedef struct _score{
	char thoi_gian[32];
	int score;
}diem;
//thong tin dang nhap
typedef struct user {
	char account[32];
	char password[32];
}user;
//thong tin ve cau hoi
typedef struct cauhoi{
	char cauhoi[100];
	char dapan1[20];
	char dapan2[20];
	char dapan3[20];
	char dapan4[20];
	char dapan_dung;
}cauhoi;
//giao thuc trao chuyen giua client va server
typedef struct protocol{
	int flag; 
	user u; 
	cauhoi ch;
	char answer;
	diem sc;
	int count;
}protocol;
