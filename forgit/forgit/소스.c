//�ΰ���б� ���յ��÷��� ���а� �л����� C���α׷��� ������Ʈ�Դϴ�.
#pragma warning(disable:4996)
#include <stdio.h>
#include <string.h>
#include <process.h>
#include <stdlib.h>//malloc, free �Լ��� ���Ե��ִ�.
#include <windows.h>//Ŀ����ġ
#include <conio.h>//sleep�Լ� ����
#include<time.h>//rand�Լ��� ����� �ϱ�����
#define MAXMEMBER 100  //�ִ� 100��
#define MAXSCORE 30
#define MAXLEVEL 5
#define MAXWORD 100

unsigned int WINAPI key(void *arg);
unsigned long ThreadID;
HANDLE Thread;

struct member  //����ü�� �̿��� ȸ������ ����
{
	char name[10];
	char sex[10];
	char age[10];
	char id[20];
	char password[20];
	int level;
	int score;
};
enum state // ������ ���ڰ� �ϳ��� �ö󰣴� unshow = 0 �̸� show =1 disalbe = 2�̴�. ���� ������ ����
{
	unshow = 0,
	show,
	disable,
};
struct word { //�������� �ܾ� ��ġ
	int x, y;
	enum state s;
};

void exit(char);
void ranking(int, char);
void ranking_output(void);
void ranking_input(void);
int fileread(int); //���� �д��Լ�
void game_main(int);
void gotoxy(int, int);
void gamebasic_design(); //����ȭ�� �⺻ ������
int main_screen(); //���� �������̽�
void join_member(); //ȸ������ (ȸ������)
int log_in(); //�α���
int findID(char *id); //���̵�->�ε���
void getpassword(char *buf);
int lv(int); // ���� ����
void start(int);//���� ī��Ʈ
void mainscreen_design();//����ȭ�� ������
void alramscreen_design();//�˶� ������
void rainking_design();//��ŷ ������
int printword(int);
int memcnt = 0; //�����
struct member info[MAXMEMBER]; //ȸ������ ���� �迭 ����
char words_out[MAXWORD][10];
struct word loc[MAXWORD];
int word_count = 0;



/*void rainking_input(int score, char name) { //��ũ �Է�
FILE *rank;
rank = fopen("ranking.txt", "w+");
if (rank == NULL) {
puts("Rank�� ������ ���� �ʽ��ϴ�.");
}
for (int i = 0; i < MAXMEMBER; i++) {    //�̰͵� �պ��ߵ�
fprintf(rank, "%s %d\n", info[i].name, info[i].score);
}
fclose(rank);
}*/

int compareword(char *key_p) {//�Է¹��� �ܾ �´��� Ȯ��
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

unsigned int WINAPI key(void *arg) { //������
	int score;
	int location = 0;
	char key2[21] = { 0 };
	char *key_p;
	key_p = key2;
	while (1) {  //Ű�Է�
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
	while (1) { //���� �� �Ҷ� ���� �ݺ�
		if (printindex >= word_count)
			printindex = 0;
		loc[printindex].y = 1;
		loc[printindex].s = show; // �ܾ� �߰�
							//��ĭ�� �̷�鼭 ���(�������ִ� �ܾ)
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
int fileread(int login) // ���� �д� �Լ�
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
		printf("������ �����ϴ� ����");
		exit(0);
	}

	while (fscanf(fp, "%s", words_out[index]) != EOF) {  //���� ������ �ҷ����� END of File
		index++;
	}
	word_count = index;
	fclose(fp);
	for (int i = 0; i < word_count; i++) { // �� �ܾ�� ���ο� ��ġ ����
		loc[i].x = rand() % 45 + 1;
		loc[i].y = 1;  //������ y�� ��ġ
		loc[i].s = unshow; // �������� �Ⱥ�������
	}
	return word_count;

	//sufle �ʿ�
}
void game_main(int login) { //������ �ҷ�
sub:
	gamebasic_design();
	start(login);
	printword(login);
	if (info[login].level != 5) goto sub;
	//ranking(info[login].score,info[login].name);       // ��ŷ ����ϱ� �̰� �ذ���;;   
	//ranking_output();
	//clearâ ����� ���Ŀ� ����ȭ������ ���ư���.
}
void gotoxy(int x, int y) //Ŀ�� ������ �Լ�
{
	COORD Pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
void gamebasic_design()
{

	system("mode con: cols=100 lines=30"); //#include <stdlib.h> ȭ��ũ������
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
	printf("|�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ� ll �ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�|\n");
	printf("|                                                   ll                                           |\n");
	printf("|            Ű �Է� :                              ll           ���� :                          |\n");
	printf("|                                                   ll                                           |\n");
	printf("==================================================================================================\n");

}

int main_screen() //main �Լ�
{
	int login = MAXMEMBER; // ���� �α��ε� ����� �����ʱ�ȭ
	while (1)
	{
		//struct member info[100]; //100�����Ҽ��ְ���
		mainscreen_design();
		if (login == MAXMEMBER) { //�α����� �ȵȻ���
			int i;
			gotoxy(40, 20);
			printf("1. ȸ������\n");
			gotoxy(40, 21);
			printf("2. �α���\n");
			gotoxy(40, 22);
			printf("3. ��ŷȮ��\n");
			gotoxy(40, 23);
			printf("4. ����\n");
			gotoxy(37, 24);
			printf("��ȣ�� �Է��Ͻÿ� : ");
			scanf("%d", &i);
			printf("\n");
			switch (i)
			{
			case 1:
				join_member();//ȸ�����������̵�
				break;

			case 2:
				login = log_in(); //�α������� �̵�
				break;
			case 3: //�̱���
				break;

			case 4:
				alramscreen_design();
				printf("���α׷��� ����˴ϴ� ");
				gotoxy(10, 6);
				exit(1);
				break;

			default:
				alramscreen_design();
				printf("�ٽ� �Է��� �ֽʽÿ�.");
				Sleep(1000); //������
				system("cls");
				break;
			}
		}
		else { //�α����� �Ȼ��� login ������ �α��ε� ����������� ����Ǿ�����
			  //������ �ʿ�
			gotoxy(37, 19);
			printf("%s�� �ݰ����ϴ�.\n", info[login].name);
			gotoxy(40, 20);
			printf("1. ���ӽ���\n");
			gotoxy(40, 21);
			printf("2. ȸ������Ȯ��\n");
			gotoxy(40, 22);
			printf("3. ��ŷȮ��\n");
			gotoxy(40, 23);
			printf("4. �α׾ƿ�\n");
			gotoxy(37, 24);
			printf("��ȣ�� �Է��Ͻÿ� : ");
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
				printf("�α׾ƿ� ���Դϴ�...");
				login = MAXMEMBER;
				Sleep(1000);
				system("cls");
				break;
			default:
				printf("�ٽ� �Է��� �ֽʽÿ�.");
				Sleep(1000); //������
				system("cls");
			}
		}
	}
}

void join_member()//ȸ������  //�������� info�� �����
{
	//ȸ�������ο����� 100�� �����϶�
	if (memcnt < MAXMEMBER) {
		alramscreen_design();
		printf("�̸��� �Է��Ͻÿ� : ");
		scanf("%s", info[memcnt].name); //-> i��� memcnt
		if ((*info[memcnt].name >= 33) && (*info[memcnt].name <= 127))


			alramscreen_design();
		printf("������ �Է��Ͻÿ�(����/����) : ");
		scanf("%s", info[memcnt].sex);

		alramscreen_design();
		printf("���̸� �Է��Ͻÿ� : ");
		scanf("%d", info[memcnt].age);

		alramscreen_design();
		printf("���̵� �Է��Ͻÿ� : ");
		scanf("%s", info[memcnt].id);
		//+�߰�  �ߺ��� ���̵� �Դϴ�.
		while (1) {
			alramscreen_design();
			printf("��й�ȣ�� �Է��Ͻÿ� : ");
			scanf("%s", info[memcnt].password);

			alramscreen_design();
			gotoxy(10, 4);
			printf("��й�ȣ Ȯ���� ���� �ٽ� �Է��� �ֽʽÿ� : ");
			//��й�ȣ Ȯ���� ���� ���� password2
			char password2[20];
			scanf("%s", password2);

			if (!strcmp(info[memcnt].password, password2)) { //!strcmp ���ڿ��� 2�� ���� ���ϴ� �Լ� strncmp�� ���° ���� ����
				alramscreen_design();
				printf("��й�ȣ�� ��ġ�մϴ�");
				Sleep(1000); //������            
				printf("%d", memcnt);
				alramscreen_design();
				printf("ȸ�������� �Ϸ� �Ǿ����ϴ�.");
				info[memcnt].level = 1; //�������� �ʱ�ȭ
				info[memcnt].score = 0;
				memcnt++; // �� ������� �������� ȸ�������� ���� �̰� ������Ű�� ����ȸ�������ϴ� ����� 0 , 1 , 2 �̷������� �����
				Sleep(1000);
				system("cls");
				return; //���⼭ �����ϸ� �Ѹ�ȸ�����Եǰ� ����Ǵ°���
			}
			else
				alramscreen_design();
			printf("��й�ȣ�� ��ġ���� �ʽ��ϴ�\n");
		}
	}
	else
	{
		alramscreen_design();
		printf("ȸ������ �ο����� �ʰ��Ͽ����ϴ�."); //100������ �ʰ���� ȸ�������� �ź���
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
	printf("���̵� : ");
	scanf("%s", log_in_id);

	int found = findID(log_in_id); //���̵� ã��
	if (found == MAXMEMBER) {
		//���̵� ��ã�����
		alramscreen_design();
		printf("���̵� ��ġ���� �ʽ��ϴ�\n");
		Sleep(1000);
		alramscreen_design();
		printf("����ȭ������ ���ư��ϴ�.");
		Sleep(1000);
		system("cls");
		return MAXMEMBER;
	}
	alramscreen_design();
	printf("��й�ȣ : ");
	getpassword(log_in_password);
	if (!strcmp(info[found].password, log_in_password)) {//�ش����� ������ �ִ� ����ü ��й�ȣ�� ���� ���� ��
		alramscreen_design();
		printf("���������� �α��� �Ǿ����ϴ�\n");
		Sleep(1000);
		system("cls");
		//�α��μ���
		return found; // ���� ����� ���ڸ� ��ȯ�Ѵ�.
	}
	alramscreen_design();
	printf("��й�ȣ�� ��ġ���� �ʽ��ϴ�\n");
	Sleep(1000);
	alramscreen_design();
	printf("����ȭ������ ���ư��ϴ�");
	Sleep(1000);
	system("cls");
	return MAXMEMBER;
}
//���̵� ã�� �Լ�
int findID(char *tagetid) {
	for (int i = 0; i < memcnt; i++) {
		if (!strcmp(info[i].id, tagetid))
			return i; //���̵� ã�Ƽ� �ε����� ����
	}
	return MAXMEMBER; //���̵� ��ã�����
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
			printf("%s", "\b \b");   // �齺���̽��� ��� �ѱ��� �����
			buf[--i] = '\0';
			continue;
		}

		buf[i++] = ch;
		putchar('*');
	}
}
int lv(int login) { // ���� �Լ�
	if (info[login].score > MAXSCORE) {
		if (info[login].level < MAXLEVEL) {
			info[login].score = 0;
			info[login].level++;
			//���� ���� �ʿ�
			return 1;
		}
	}
	return 0;
}
void start(int login)
{
	for (int i = 0; i < 3; i++) // �����غ�
	{
		gotoxy(10, 12);
		printf("LEVEL :%d", info[login].level);
		gotoxy(10, 13);
		printf("%d�� �Ŀ� �����մϴ�. �غ��� �ּ���!", 3 - i);
		Sleep(1000);
	}
}

void mainscreen_design() {
	system("mode con: cols=100 lines=30"); //#include <stdlib.h> ȭ��ũ������
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
	main_screen();//���ν�ũ�� ȣ��
}