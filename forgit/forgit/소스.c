//부경대학교 융합디스플레이 공학과 학생들의 C프로그래밍 프로젝트입니다.
#pragma warning(disable:4996)
#include <stdio.h>
#include <string.h>
#include <process.h>
#include <stdlib.h>//malloc, free 함수가 포함되있다.
#include <windows.h>//커서위치
#include <conio.h>//sleep함수 포함
#include<time.h>//rand함수를 제대로 하기위함
#define MAXMEMBER 100  //최대 100명
#define MAXSCORE 30
#define MAXLEVEL 5
#define MAXWORD 100

unsigned int WINAPI key(void *arg);
unsigned long ThreadID;
HANDLE Thread;

struct member  //구조체를 이용한 회원가입 정보
{
	char name[10];
	char sex[10];
	char age[10];
	char id[20];
	char password[20];
	int level;
	int score;
};
enum state // 열거형 숫자가 하나씩 올라간다 unshow = 0 이면 show =1 disalbe = 2이다. 직접 지정도 가능
{
	unshow = 0,
	show,
	disable,
};
struct word { //내려오는 단어 위치
	int x, y;
	enum state s;
};

void exit(char);
void ranking(int, char);
void ranking_output(void);
void ranking_input(void);
int fileread(int); //파일 읽는함수
void game_main(int);
void gotoxy(int, int);
void gamebasic_design(); //게임화면 기본 디자인
int main_screen(); //메인 인터페이스
void join_member(); //회원가입 (회원정보)
int log_in(); //로그인
int findID(char *id); //아이디->인덱스
void getpassword(char *buf);
int lv(int); // 레벨 지정
void start(int);//시작 카운트
void mainscreen_design();//메인화면 디자인
void alramscreen_design();//알람 디자인
void rainking_design();//랭킹 디자인
int printword(int);
int memcnt = 0; //사람수
struct member info[MAXMEMBER]; //회원정보 저장 배열 생성
char words_out[MAXWORD][10];
struct word loc[MAXWORD];
int word_count = 0;



/*void rainking_input(int score, char name) { //랭크 입력
FILE *rank;
rank = fopen("ranking.txt", "w+");
if (rank == NULL) {
puts("Rank와 연결이 되지 않습니다.");
}
for (int i = 0; i < MAXMEMBER; i++) {    //이것도 손봐야됨
fprintf(rank, "%s %d\n", info[i].name, info[i].score);
}
fclose(rank);
}*/

int compareword(char *key_p) {//입력받은 단어가 맞는지 확인
	int i;
	for (i = 0; i < MAXWORD; i++) {
		if (!strcmp(words_out[i], key_p)) {
			info[memcnt].score += 10;
			gotoxy(loc[i].x, loc[i].y);
			printf("          ");
			loc[i].s = disable;
		}
	}
	return info[memcnt].score;
}

unsigned int WINAPI key(void *arg) { //스레드
	int score;
	int location = 0;
	char key2[21] = { 0 };
	char *key_p;
	key_p = key2;
	while (1) {  //키입력
		int key = getch();
		if ((key >= 33 && key <= 127) && location < 20) {
			gotoxy(23 + location, 26);
			printf("%c", key);
			key2[location++] = key;
			key2[location] = '\0';
			if (location > 20)
				location = 20;
		}
		else if (key == 8) {
			if (location > 0) {
				key2[location--] = key;
				key2[location] = '\0';
				gotoxy(23 + location, 26);
				printf(" ");
			}
		}
		else if (key == 13) {
			compareword(key2);
			location = 0;
			gotoxy(23, 26);
			printf("                      ");
		}
	}
	return info[memcnt].score;
}

int printword(int login) {
	int printindex = 0, i;
	gamebasic_design();
	Thread = (HANDLE)_beginthreadex(NULL, 0, key, (void*)NULL, 0, (unsigned*)&ThreadID);
	fileread(login);
	while (1) { //레벨 업 할때 까지 반복
		if (printindex >= word_count)
			printindex = 0;
		loc[printindex].y = 1;
		loc[printindex].s = show; // 단어 추가
							//한칸씩 미루면서 출력(보여져있는 단어만)
		for (i = 0; i < word_count; i++) {
			if (loc[i].s == show) {
				gotoxy(loc[i].x, loc[i].y);
				printf("         ");
				loc[i].y++;
				gotoxy(loc[i].x, loc[i].y);
				printf("%s", words_out[i]);
				if (loc[i].y >= 23) {
					loc[i].s = disable;
					gotoxy(loc[i].x, loc[i].y);
					printf("         ");
				}
			}
		}
		info[login].score += info[memcnt].score;
		gotoxy(72, 26);
		printf("%d", info[memcnt].score);
		gotoxy(23, 26);
		printindex++;
		Sleep(1000);
		if (info[memcnt].score >= MAXSCORE) {
			info[memcnt].score = 0;
			info[login].level++;
			break;
		}
	}
	TerminateThread(Thread, 0);
	return login;
}
int fileread(int login) // 파일 읽는 함수
{
	char buf[10], i = 0;
	FILE *fp = NULL;
	switch (info[login].level)
	{
	case 1:
		fp = fopen("lv1.txt", "r");
		break;
	case 2:
		fp = fopen("lv2.txt", "r");
		break;
	case 3:
		fp = fopen("lv3.txt", "r");
		break;
	case 4:
		fp = fopen("lv4.txt", "r");
		break;
	default:
		break;
	}
	int index = 0;

	if ((int)fp == -1)
	{
		printf("파일이 없습니다 오류");
		exit(0);
	}

	while (fscanf(fp, "%s", words_out[index]) != EOF) {  //파일 끝까지 불러오기 END of File
		index++;
	}
	word_count = index;
	fclose(fp);
	for (int i = 0; i < word_count; i++) { // 각 단어마다 새로운 위치 설정
		loc[i].x = rand() % 45 + 1;
		loc[i].y = 1;  //내려올 y값 위치
		loc[i].s = unshow; // 보여줄지 안보여줄지
	}
	return word_count;

	//sufle 필요
}
void game_main(int login) { //디자인 불러
sub:
	gamebasic_design();
	start(login);
	printword(login);
	if (info[login].level != 5) goto sub;
	//ranking(info[login].score,info[login].name);       // 랭킹 등록하기 이거 해결점;;   
	//ranking_output();
	//clear창 만들기 이후에 메인화면으로 돌아간다.
}
void gotoxy(int x, int y) //커서 보내는 함수
{
	COORD Pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
void gamebasic_design()
{

	system("mode con: cols=100 lines=30"); //#include <stdlib.h> 화면크기조정
	printf("==================================================================================================\n");
	printf("|                                                   ll                                           |\n");
	printf("|                                                   ll    MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM   |\n");
	printf("|                                                   ll    MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM   |\n");
	printf("|                                                   ll    MMM$$$$$$$$$$$$$8DDDDDDDDDNNNNNNNNNN   |\n");
	printf("|                                                   ll    MMM$$$$$$8888DDDDDDDDDNNNNNNNNNNMMMN   |\n");
	printf("|                                                   ll    MMM$$$$ -------------------- NNNNMMN   |\n");
	printf("|                                                   ll    MMM$$$$I MM   M     M   MM  INNNNNNN   |\n");
	printf("|                                                   ll    MMM$$$$I MM   M     M  M M  INMNMNNN   |\n");
	printf("|                                                   ll    MMMZZ$$I MM    M   M     M  INNNNNNN   |\n");
	printf("|                                                   ll    MMMZZZZI MM     M M      M  INNNMMMM   |\n");
	printf("|                                                   ll    MMMZZZZI MMMMM   M     MMMM INNNNNNN   |\n");
	printf("|                                                   ll    MMMZZZZ -------------------- DNNNNNN   |\n");
	printf("|                                                   ll    MMMZZZZZDDDDDDNNNNNNNNMMMNNNNNNMMMMM   |\n");
	printf("|                                                   ll    MMMOZZZZZZZZZZDDDDDDDNNNNNNNNNMMMMMM   |\n");
	printf("|                                                   ll    MMMOOOOOZZZZZDDDNNNNNNNNNNMMMMMMMMMM   |\n");
	printf("|                                                   ll    MMMOOOOOOOOOOZODDDDDNNNNNNNNNMMMMMMM   |\n");
	printf("|                                                   ll    MMMOOOOOOOODDDDNNNNNNNNNNNMMMMMMMMMM   |\n");
	printf("|                                                   ll                MMMMMMMMMMMM               |\n");
	printf("|                                                   ll               MMMMMMMMMMMMMM              |\n");
	printf("|                                                   ll            8NDMMMMMMMMMMMMMMDDN           |\n");
	printf("|                                                   ll       88DDDDDDMMMMMMMMMMMMMMNDDDDDD8      |\n");
	printf("|                                                   ll     ZNNNNMMMNNNMNNNNNNNNMMMMNMMMMMMMNM    |\n");
	printf("|                                                   ll                                           |\n");
	printf("|ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ ll ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ|\n");
	printf("|                                                   ll                                           |\n");
	printf("|            키 입력 :                              ll           점수 :                          |\n");
	printf("|                                                   ll                                           |\n");
	printf("==================================================================================================\n");

}

int main_screen() //main 함수
{
	int login = MAXMEMBER; // 현재 로그인된 사용자 정보초기화
	while (1)
	{
		//struct member info[100]; //100가입할수있게함
		mainscreen_design();
		if (login == MAXMEMBER) { //로그인이 안된상태
			int i;
			gotoxy(40, 20);
			printf("1. 회원가입\n");
			gotoxy(40, 21);
			printf("2. 로그인\n");
			gotoxy(40, 22);
			printf("3. 랭킹확인\n");
			gotoxy(40, 23);
			printf("4. 종료\n");
			gotoxy(37, 24);
			printf("번호를 입력하시오 : ");
			scanf("%d", &i);
			printf("\n");
			switch (i)
			{
			case 1:
				join_member();//회원가입으로이동
				break;

			case 2:
				login = log_in(); //로그인으로 이동
				break;
			case 3: //미구현
				break;

			case 4:
				alramscreen_design();
				printf("프로그램이 종료됩니다 ");
				gotoxy(10, 6);
				exit(1);
				break;

			default:
				alramscreen_design();
				printf("다시 입력해 주십시오.");
				Sleep(1000); //딜레이
				system("cls");
				break;
			}
		}
		else { //로그인이 된상태 login 변수에 로그인된 사용자정보가 저장되어있음
			  //디자인 필요
			gotoxy(37, 19);
			printf("%s님 반갑습니다.\n", info[login].name);
			gotoxy(40, 20);
			printf("1. 게임시작\n");
			gotoxy(40, 21);
			printf("2. 회원정보확인\n");
			gotoxy(40, 22);
			printf("3. 랭킹확인\n");
			gotoxy(40, 23);
			printf("4. 로그아웃\n");
			gotoxy(37, 24);
			printf("번호를 입력하시오 : ");
			int i;
			scanf("%d", &i);
			printf("\n");
			switch (i)
			{
			case 1:
				game_main(login);
				break;
			case 2:
				//profile(login);
				break;
			case 3:
				//Rank(login);
				break;
			case 4:
				gotoxy(40, 25);
				printf("로그아웃 중입니다...");
				login = MAXMEMBER;
				Sleep(1000);
				system("cls");
				break;
			default:
				printf("다시 입력해 주십시오.");
				Sleep(1000); //딜레이
				system("cls");
			}
		}
	}
}

void join_member()//회원가입  //전역변수 info를 사용함
{
	//회원가입인원수가 100명 이하일때
	if (memcnt < MAXMEMBER) {
		alramscreen_design();
		printf("이름을 입력하시오 : ");
		scanf("%s", info[memcnt].name); //-> i대신 memcnt
		if ((*info[memcnt].name >= 33) && (*info[memcnt].name <= 127))


			alramscreen_design();
		printf("성별을 입력하시오(남성/여성) : ");
		scanf("%s", info[memcnt].sex);

		alramscreen_design();
		printf("나이를 입력하시오 : ");
		scanf("%d", info[memcnt].age);

		alramscreen_design();
		printf("아이디를 입력하시오 : ");
		scanf("%s", info[memcnt].id);
		//+추가  중복된 아이디 입니다.
		while (1) {
			alramscreen_design();
			printf("비밀번호를 입력하시오 : ");
			scanf("%s", info[memcnt].password);

			alramscreen_design();
			gotoxy(10, 4);
			printf("비밀번호 확인을 위해 다시 입력해 주십시오 : ");
			//비밀번호 확인을 위한 변수 password2
			char password2[20];
			scanf("%s", password2);

			if (!strcmp(info[memcnt].password, password2)) { //!strcmp 문자열을 2개 서로 비교하는 함수 strncmp는 몇번째 까지 가능
				alramscreen_design();
				printf("비밀번호가 일치합니다");
				Sleep(1000); //딜레이            
				printf("%d", memcnt);
				alramscreen_design();
				printf("회원가입이 완료 되었습니다.");
				info[memcnt].level = 1; //레벨점수 초기화
				info[memcnt].score = 0;
				memcnt++; // 이 사람수를 기준으로 회원가입을 만듬 이걸 증가시키면 다음회원가입하는 사람은 0 , 1 , 2 이런식으로 저장됨
				Sleep(1000);
				system("cls");
				return; //여기서 리턴하면 한명회원가입되고 종료되는거임
			}
			else
				alramscreen_design();
			printf("비밀번호가 일치하지 않습니다\n");
		}
	}
	else
	{
		alramscreen_design();
		printf("회원가입 인원수를 초과하였습니다."); //100명제한 초과경우 회원가입을 거부함
		Sleep(1000);
		system("cls");
		return;
	}

}
int log_in()
{
	char log_in_id[20];
	char log_in_password[20];

	alramscreen_design();
	printf("아이디 : ");
	scanf("%s", log_in_id);

	int found = findID(log_in_id); //아이디를 찾음
	if (found == MAXMEMBER) {
		//아이디를 못찾은경우
		alramscreen_design();
		printf("아이디가 일치하지 않습니다\n");
		Sleep(1000);
		alramscreen_design();
		printf("메인화면으로 돌아갑니다.");
		Sleep(1000);
		system("cls");
		return MAXMEMBER;
	}
	alramscreen_design();
	printf("비밀번호 : ");
	getpassword(log_in_password);
	if (!strcmp(info[found].password, log_in_password)) {//해당사람의 정보가 있는 구조체 비밀번호와 얻은 것을 비교
		alramscreen_design();
		printf("성공적으로 로그인 되었습니다\n");
		Sleep(1000);
		system("cls");
		//로그인성공
		return found; // 들어가는 사람의 숫자를 반환한다.
	}
	alramscreen_design();
	printf("비밀번호가 일치하지 않습니다\n");
	Sleep(1000);
	alramscreen_design();
	printf("메인화면으로 돌아갑니다");
	Sleep(1000);
	system("cls");
	return MAXMEMBER;
}
//아이디를 찾는 함수
int findID(char *tagetid) {
	for (int i = 0; i < memcnt; i++) {
		if (!strcmp(info[i].id, tagetid))
			return i; //아이디를 찾아서 인덱스를 리턴
	}
	return MAXMEMBER; //아이디를 못찾은경우
}
void getpassword(char *buf)
{
	int i = 0;
	char ch;

	fflush(stdin);
	while (1) {
		ch = getch();
		if (ch == '\r') {
			buf[i] = '\0';
			break;
		}
		else if (ch == '\b') {
			printf("%s", "\b \b");   // 백스페이스인 경우 한글자 지우기
			buf[--i] = '\0';
			continue;
		}

		buf[i++] = ch;
		putchar('*');
	}
}
int lv(int login) { // 레벨 함수
	if (info[login].score > MAXSCORE) {
		if (info[login].level < MAXLEVEL) {
			info[login].score = 0;
			info[login].level++;
			//레벨 저장 필요
			return 1;
		}
	}
	return 0;
}
void start(int login)
{
	for (int i = 0; i < 3; i++) // 시작준비
	{
		gotoxy(10, 12);
		printf("LEVEL :%d", info[login].level);
		gotoxy(10, 13);
		printf("%d초 후에 시작합니다. 준비해 주세요!", 3 - i);
		Sleep(1000);
	}
}

void mainscreen_design() {
	system("mode con: cols=100 lines=30"); //#include <stdlib.h> 화면크기조정
	system("cls");
	printf("==================================================================================================\n");
	printf("|                                                                                                |\n");
	printf("|                                                                                                |\n");
	printf("|                                                                                                |\n");
	printf("|                                                                                                |\n");
	printf("|        MMMMMMM MMM  MMMMMMMMMM  MMMMMMM  MM        MMMMMMMM MM   MMMMMMMM   MMMMMMM MM         |\n");
	printf("|             MM MMM  MMMMMMMMMM  MMMMMMM  MM        MMMMMMMM MM  MMNMMMMMMM  MMMMMMM MM         |\n");
	printf("|        MMMMMMMMMMM    MM  MM    MM       MM              MM MM  MM      MM       MM MM         |\n");
	printf("|             MM MMM  MMMMMMMMMM  MMMMMMM MMM              MM MM  MMMMMMMMMM       MM MM         |\n");
	printf("|             MM MMM  MMMMMMMMMM  MMMMMMM MMM        MMMMMMMM MM    MMMMMM         MM MM         |\n");
	printf("|                                 MM       MM        MMMMMMMM MM                   MM MM         |\n");
	printf("|        MMMMMMMMMMM  MMMMMMMMMM  MM       MM              MM MM MMMMMMMMMMMM      MM MM         |\n");
	printf("|        MMM     MMM    MM  MM    MMMMMMM  MM              MM MM      MM           MM MM         |\n");
	printf("|        MMMMMMMMMMM    MM  MM    MMMMMMM  MM              MM MM      MM           MM MM         |\n");
	printf("|                                                                                                |\n");
	printf("|                                                                                                |\n");
	printf("|                                                                                                |\n");
	printf("|                                                                                                |\n");
	printf("|                                                                                                |\n");
	printf("|                                                                                                |\n");
	printf("|                                                                                                |\n");
	printf("|                                                                                                |\n");
	printf("|                                                                                                |\n");
	printf("|                                                                                                |\n");
	printf("|                                                                                                |\n");
	printf("|                                                                                                |\n");
	printf("|                                                                                                |\n");
	printf("|                                                                                                |\n");
	printf("==================================================================================================\n");

}
void alramscreen_design() {
	system("cls");
	system("mode con: cols=70 lines=10");
	printf("=====================================================================\n");
	printf("|                                                                   |\n");
	printf("|                                                                   |\n");
	printf("|                                                                   |\n");
	printf("|                                                                   |\n");
	printf("|                                                                   |\n");
	printf("|                                                                   |\n");
	printf("|                                                                   |\n");
	printf("=====================================================================\n");
	gotoxy(20, 4);

}

int main()
{
	main_screen();//메인스크린 호출
}