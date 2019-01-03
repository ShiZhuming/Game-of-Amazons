/*
Amazons game v4.3
from:Zhuming Shi
Student ID:1800011723
E-mail:shizhuming@pku.edu.cn
College of Chemistry and Molecular Engineering
*/

/*
v4.3

�޸ĵ����
0 ������ļ�ʵ�ִ��̣����̵Ĺ���
1 ��whileѭ�������ݴ����
2 �޸��ӽ��վ�ʱ�ݹ鵽�յ��޷���������������bug

���޸ĵ����
0 ������λ�����¼���޸�����
1 ����menu��������v1.4�е�goto���
2 ͨ�������¼��������λ�ü������ش���64->8

Dec.30 11:00 2018
*/

#include <iostream>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>

#define GRIDSIZE 8
#define OBSTACLE 2
#define judge_black 0
#define judge_white 1
#define grid_black 1
#define grid_white -1
#define MAXX 1000000
#define MINN -1000000
#define Changefre 10 //ÿ��������һ���������
#define StartDepth 1

using namespace std;

int currBotColor; // ����ִ����ɫ��1Ϊ�ڣ�-1Ϊ�ף�����״̬��ͬ��
int gridInfo[GRIDSIZE][GRIDSIZE] = { 0 }; // ��x��y����¼����״̬
int dx[] = { -1,-1,-1,0,0,1,1,1 };//���з���
int dy[] = { -1,0,1,-1,1,-1,0,1 };//���з���

int SetDepth = 1;//���õ��������
//int Changefre = 10;//ÿ��������һ���������
int beginPos[2], possiblePos[2], obstaclePos[2];//��¼������ӷ�������Ai���������ݸ�main����ʹ��

int Queen[GRIDSIZE][GRIDSIZE][2] = { 0 };//��¼queenmove
int King[GRIDSIZE][GRIDSIZE][2] = { 0 };//��¼kingmove

double AdjustK = 0.5;//������������3����Ӧ��queenmove���ʱ�յصļ�ֵ

int nowtime = 0;//��ʾ��ǰ���濪�֣��оֻ��߲о�

double Adjust[3][5] = {
	0.14,0.37,0.13,0.13,0.20,
	0.30,0.25,0.20,0.20,0.05,
	0.80,0.10,0.05,0.05,0.00};

int turnID = 1;
int turns = 0;
int remember[140][7] = { 0 };//TA_only ���̸����ã�ÿһ��һ�����turnInfo[][0]��¼�ڻ��ǰף����������¼����

char nameOfFlie[7] = { "r0.txt" };

// TA_only ��ӡ��ӭ����
// 1080p�ֱ��ʣ�125%���ŵ�windows�ʼǱ�������Ļ�Ͽ�����ʾ,����֤������ʾ��������ȷ��ʾ��
void welcome()
{
	cout << R"(          _____                    _____                    _____                    _____                   _______                   _____                    _____          )" << endl;
	cout << R"(         /\    \                  /\    \                  /\    \                  /\    \                 /::\    \                 /\    \                  /\    \         )" << endl;
	cout << R"(        /::\    \                /::\____\                /::\    \                /::\    \               /::::\    \               /::\____\                /::\    \        )" << endl;
	cout << R"(       /::::\    \              /::::|   |               /::::\    \               \:::\    \             /::::::\    \             /::::|   |               /::::\    \       )" << endl;
	cout << R"(      /::::::\    \            /:::::|   |              /::::::\    \               \:::\    \           /::::::::\    \           /:::::|   |              /::::::\    \      )" << endl;
	cout << R"(     /:::/\:::\    \          /::::::|   |             /:::/\:::\    \               \:::\    \         /:::/~~\:::\    \         /::::::|   |             /:::/\:::\    \     )" << endl;
	cout << R"(    /:::/__\:::\    \        /:::/|::|   |            /:::/__\:::\    \               \:::\    \       /:::/    \:::\    \       /:::/|::|   |            /:::/__\:::\    \    )" << endl;
	cout << R"(   /::::\   \:::\    \      /:::/ |::|   |           /::::\   \:::\    \               \:::\    \     /:::/    / \:::\    \     /:::/ |::|   |            \:::\   \:::\    \   )" << endl;
	cout << R"(  /::::::\   \:::\    \    /:::/  |::|___|______    /::::::\   \:::\    \               \:::\    \   /:::/____/   \:::\____\   /:::/  |::|   | _____    ___\:::\   \:::\    \  )" << endl;
	cout << R"( /:::/\:::\   \:::\    \  /:::/   |::::::::\    \  /:::/\:::\   \:::\    \               \:::\    \ |:::|    |     |:::|    | /:::/   |::|   |/\    \  /\   \:::\   \:::\    \ )" << endl;
	cout << R"(/:::/  \:::\   \:::\____\/:::/    |:::::::::\____\/:::/  \:::\   \:::\____\_______________\:::\____\|:::|____|     |:::|    |/:: /    |::|   /::\____\/::\   \:::\   \:::\____\)" << endl;
	cout << R"(\::/    \:::\  /:::/    /\::/    / ~~~~~/:::/    /\::/    \:::\  /:::/    /\::::::::::::::::::/    / \:::\    \   /:::/    / \::/    /|::|  /:::/    /\:::\   \:::\   \::/    /)" << endl;
	cout << R"( \/____/ \:::\/:::/    /  \/____/      /:::/    /  \/____/ \:::\/:::/    /  \::::::::::::::::/____/   \:::\    \ /:::/    /   \/____/ |::| /:::/    /  \:::\   \:::\   \/____/ )" << endl;
	cout << R"(          \::::::/    /               /:::/    /            \::::::/    /    \:::\~~~~\~~~~~~          \:::\    /:::/    /            |::|/:::/    /    \:::\   \:::\    \     )" << endl;
	cout << R"(           \::::/    /               /:::/    /              \::::/    /      \:::\    \                \:::\__/:::/    /             |::::::/    /      \:::\   \:::\____\    )" << endl;
	cout << R"(           /:::/    /               /:::/    /               /:::/    /        \:::\    \                \::::::::/    /              |:::::/    /        \:::\  /:::/    /    )" << endl;
	cout << R"(          /:::/    /               /:::/    /               /:::/    /          \:::\    \                \::::::/    /               |::::/    /          \:::\/:::/    /     )" << endl;
	cout << R"(         /:::/    /               /:::/    /               /:::/    /            \:::\    \                \::::/    /                /:::/    /            \::::::/    /      )" << endl;
	cout << R"(        /:::/    /               /:::/    /               /:::/    /              \:::\____\                \::/____/                /:::/    /              \::::/    /       )" << endl;
	cout << R"(        \::/    /                \::/    /                \::/    /                \::/    /                 ~~                      \::/    /                \::/    /        )" << endl;
	cout << R"(         \/____/                  \/____/                  \/____/                  \/____/                                           \/____/                  \/____/         )" << endl;
	cout << R"(                                                                                                                                                                               )" << endl;
}

// TA_only ��ӡ����
// 1080p�ֱ��ʣ�125%���ŵ�windows�ʼǱ�������Ļ�Ͽ�����ʾ,����֤������ʾ��������ȷ��ʾ��
void printchess(int Info)
{
	switch (Info) {
	case 0:cout << "��"; return;
	case 1:cout << "  "; return;
	case 2:cout << "��"; return;
	case 3:cout << "��"; return;
	}
}

// TA_only ��ӡ����
// 1080p�ֱ��ʣ�125%���ŵ�windows�ʼǱ�������Ļ�Ͽ�����ʾ,����֤������ʾ��������ȷ��ʾ��
void printgridInfo()
{
	//char point[4] = { 'o',0,'*','#' };
	int i, j;
	cout << "   1   2   3   4   5   6   7   8  " << endl;
	cout << " ���������Щ������Щ������Щ������Щ������Щ������Щ������Щ�������" << endl;
	for (i = 0; i < 7; i++) {
		cout << i + 1;
		for (j = 0; j < 8; j++) {
			cout << "��";cout << " ";
			printchess(gridInfo[j][i] + 1);
			
		}
		cout << "��" << endl << " ���������੤�����੤�����੤�����੤�����੤�����੤�����੤������" << endl;
	}
	cout << 8;
	for (j = 0; j < 8; j++) {
		cout << "��";cout << " ";
		printchess(gridInfo[j][7] + 1);
		
	}
	cout << "��" << endl << " ���������ة������ة������ة������ة������ة������ة������ة�������" << endl;
}

// TA_only ��ӡ�˵�
int menu()
{
	cout << "Welcome to Amazons Games !\n";
	cout << "Selecte one in the menu and continue !\n";
	cout << "1	Start a new turn\n"
		<< "2	Save this round\n"
		<< "3	review a round\n"
		<< "4	exit\n"
		<< "You can enter the number to choose one : ";
	int tempnumber;
	while (1) {
		cin >> tempnumber;
		if (tempnumber >= 1 && tempnumber < 5)break;
		else cout << endl << "Input number 1 to 4 to choose . Please try again : ";
	}
	cout << endl;
	return tempnumber;
}

// TA_only ѡ��������ɫ
void choosecolor()
{
	cout << "Please choose a color for your chess . If you choose black , enter 'B', else , enter 'W' :";
	while (1) {
		char temp;
		cin >> temp;
		switch (temp) {
		case 'W':
		case 'w':currBotColor = grid_black; return;
		case 'B':
		case 'b':currBotColor = grid_white; return;
		default:cout << "Sorry but you entered something wrong just now . Please try again :";
		}
	}
}

// Both ������̲��ҷ��ó�ʼ����
void cleangridInfo()
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			gridInfo[i][j] = 0;
	gridInfo[0][(GRIDSIZE - 1) / 3] = gridInfo[(GRIDSIZE - 1) / 3][0]
		= gridInfo[GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)][0]
		= gridInfo[GRIDSIZE - 1][(GRIDSIZE - 1) / 3] = grid_black;
	gridInfo[0][GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)] = gridInfo[(GRIDSIZE - 1) / 3][GRIDSIZE - 1]
		= gridInfo[GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)][GRIDSIZE - 1]
		= gridInfo[GRIDSIZE - 1][GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)] = grid_white;
}

// Both �ж��Ƿ��ڵ�ͼ��
inline bool inMap(int x, int y)
{
	if (x < 0 || x >= GRIDSIZE || y < 0 || y >= GRIDSIZE)
		return false;
	return true;
}

// Both ����Ƿ�Ϸ�
bool ProcStep(int x0, int y0, int x1, int y1, int x2, int y2, int color)
{
	if ((!inMap(x0, y0)) || (!inMap(x1, y1)) || (!inMap(x2, y2))) return false;
	if (gridInfo[x0][y0] != color || gridInfo[x1][y1] != 0) return false;
	if ((gridInfo[x2][y2] != 0) && !(x2 == x0 && y2 == y0)) return false;
	return true;
}

// ���Amazons�ƶ�·�����Ƿ�û���ϰ������û�з���true���򷵻�false
bool PathCheck(int x0, int y0, int x1, int y1)
{
	int dY = 0, dX = 0;
	if (x1 == x0) dY = y1 > y0 ? 1 : -1;
	else if (y1 == y0) dX = x1 > x0 ? 1 : -1;
	else if (x1 + y1 == x0 + y0 || x1 - y1 == x0 - y0) { dY = y1 > y0 ? 1 : -1; dX = x1 > x0 ? 1 : -1; }
	while (1) {
		x0 += dX;
		y0 += dY;
		if (gridInfo[x0][y0] != 0) return false;
		if (y0 == y1 && x0 == x1) break;
	}
	return true;
}

// TA_only ���ż�·�����Ƿ����ϰ������û�з���true���򷵻�false
bool SotCheck(int x0, int y0, int x1, int y1, int x2, int y2)
{
	int dY = 0, dX = 0;
	if (x2 == x1) dY = y2 > y1 ? 1 : -1;
	else if (y2 == y1) dX = x2 > x1 ? 1 : -1;
	else if (x2 + y2 == x1 + y1 || x2 - y2 == x1 - y1) { dY = y2 > y1 ? 1 : -1; dX = x2 > x1 ? 1 : -1; }
	while (1) {
		x1 += dX;
		y1 += dY;
		if (gridInfo[x1][y1] != 0 && !(x1 == x0 && y1 == y0)) return false;
		if (y2 == y1 && x2 == x1) break;
	}
	return true;
}

// Both Queen��������
void cleanQueenAndKing()
{
	for (int i = 0; i < GRIDSIZE; ++i) {
		for (int j = 0; j < GRIDSIZE; ++j) {
			Queen[i][j][0] = MAXX;
			Queen[i][j][1] = MAXX;
			King[i][j][0] = MAXX;
			King[i][j][1] = MAXX;
		}
	}
}

// Both queenmove���ɹ���
void QueenMove(int color)
{
	bool visited[GRIDSIZE][GRIDSIZE] = { 0 };//�ݹ������м�¼�Ƿ��Ѽ����
	int wait[2][100];//һ�����У���¼��һ����չ�Ľڵ�
	int start = 0, end = 0;
	int move = 0;

	//Ѱ�ҵ�һ����չ�ڵ�
	for (int i = 0; i < GRIDSIZE; ++i) {
		for (int j = 0; j < GRIDSIZE; ++j) {
			if (gridInfo[i][j] != color) continue;//��֦������Amazons�Ͳ�����
			wait[0][end] = i; wait[1][end++] = j;//����չ�ڵ㣬��¼��ͬʱ��end�����һλ
		}
	}

	if (color == currBotColor)color = 0; else if (color == -currBotColor) color = 1;

	//�����������
	while (end > start) {
		int tempstart = start;
		int tempend = end;
		start = end;
		move++;

		for (int i = tempstart; i < tempend; i++) {
			for (int k = 0; k < 8; ++k) {
				for (int delta1 = 1; delta1 < GRIDSIZE; delta1++) {
					int xx = wait[0][i] + dx[k] * delta1;
					int yy = wait[1][i] + dy[k] * delta1;
					if (gridInfo[xx][yy] != 0 || !inMap(xx, yy))
						break;
					if (visited[xx][yy])continue;
					wait[0][end] = xx; wait[1][end++] = yy;
					Queen[xx][yy][color] = move;
					visited[xx][yy] = 1;
				}
			}
		}
	}

	// Just for test
	/*if (color) {
		cout << "queenmove" << endl;
		for (int i = 0; i < GRIDSIZE; i++) {
			for (int j = 0; j < GRIDSIZE; j++) {
				cout << '(' << Queen[j][i][0] << ',' << Queen[j][i][1] << ")  ";
			}
			cout << endl;
		}
	}*/
}

// Both kingmove���ɹ���
void KingMove(int color)
{
	bool visited[GRIDSIZE][GRIDSIZE] = { 0 };//�ݹ������м�¼�Ƿ��Ѽ����
	int wait[2][100];//һ�����У���¼��һ����չ�Ľڵ�
	int start = 0, end = 0;
	int move = 0;

	//Ѱ�ҵ�һ����չ�ڵ�
	for (int i = 0; i < GRIDSIZE; ++i) {
		for (int j = 0; j < GRIDSIZE; ++j) {
			if (gridInfo[i][j] != color) continue;//��֦������Amazons�Ͳ�����
			wait[0][end] = i; wait[1][end++] = j;//����չ�ڵ㣬��¼��ͬʱ��end�����һλ
		}
	}

	if (color == currBotColor)color = 0; else if (color == -currBotColor) color = 1;

	//�����������
	while (end > start) {
		int tempstart = start;
		int tempend = end;
		start = end;
		move++;

		for (int i = tempstart; i < tempend; i++) {
			for (int k = 0; k < 8; ++k) {
				int xx = wait[0][i] + dx[k];
				int yy = wait[1][i] + dy[k];
				if (gridInfo[xx][yy] != 0 || !inMap(xx, yy))
					continue;
				if (visited[xx][yy])continue;
				wait[0][end] = xx; wait[1][end++] = yy;
				King[xx][yy][color] = move;
				visited[xx][yy] = 1;
			}
		}
	}

	//Just for test
	/*if (color) {
		cout << "kingmove" << endl;
		for (int i = 0; i < GRIDSIZE; i++) {
			for (int j = 0; j < GRIDSIZE; j++) {
				cout << '(' << King[j][i][0] << ',' << King[j][i][1] << ")  ";
			}
			cout << endl;
		}
	}*/
}

// Both mobility���㺯��
void Mobility(int &posCount)
{
	//����Amazons
	for (int i = 0; i < GRIDSIZE; ++i) {
		for (int j = 0; j < GRIDSIZE; ++j) {
			if (gridInfo[i][j] != currBotColor && gridInfo[i][j] != -currBotColor) continue;//��֦������Amazons�Ͳ�����

			//��������Amazons�߷�
			for (int k = 0; k < 8; ++k) {
				for (int delta1 = 1; delta1 < GRIDSIZE; delta1++) {
					int xx = i + dx[k] * delta1;
					int yy = j + dy[k] * delta1;
					if (gridInfo[xx][yy] != 0 || !inMap(xx, yy))
						break;

					//�������ɷż���ʽ
					for (int l = 0; l < 8; ++l) {
						for (int delta2 = 1; delta2 < GRIDSIZE; delta2++) {
							int xxx = xx + dx[l] * delta2;
							int yyy = yy + dy[l] * delta2;
							if (!inMap(xxx, yyy))
								break;
							if (gridInfo[xxx][yyy] != 0 && !(i == xxx && j == yyy))
								break;

							//���һ�������߷�
							if (gridInfo[i][j] == currBotColor) posCount++;
							else if (gridInfo[i][j] == -currBotColor) posCount--;
						}
					}
				}
			}
		}
	}
}

// Both Amazons��������
double AmazonsCount()
{
	int mobility = 0;

	Mobility(mobility);//��������

	double tqueen = 0;//��queenmove���������
	double tking = 0;//��kingmove���������

	//����queenmove��kingmove
	cleanQueenAndKing();

	QueenMove(currBotColor);
	QueenMove(-currBotColor);

	KingMove(currBotColor);
	KingMove(-currBotColor);

	//�������t
	for (int i = 0; i < GRIDSIZE; i++) {
		for (int j = 0; j < GRIDSIZE; j++) {

			if (gridInfo[i][j] != 0)continue;

			if (Queen[i][j][0] > Queen[i][j][1])tqueen--;
			else if (Queen[i][j][0] < Queen[i][j][1])tqueen++;
			else if (Queen[i][j][0] == Queen[i][j][1] && Queen[i][j][0] != MAXX)tqueen = tqueen + AdjustK;

			if (King[i][j][0] > King[i][j][1])tking--;
			else if (King[i][j][0] < King[i][j][1])tking++;
			else if (King[i][j][0] == King[i][j][1] && King[i][j][0] != MAXX)tking = tking + AdjustK;
		}
	}

	double pqueen = 0;
	double pking = 0;
	double temp = 1;

	//����λ��p
	for (int i = 0; i < GRIDSIZE; i++) {
		for (int j = 0; j < GRIDSIZE; j++) {
			
			if (gridInfo[i][j] != 0)continue;
			
			if (Queen[i][j][0] != Queen[i][j][1]) pqueen += 2.0*(pow(2.0, (double)-Queen[i][j][0]) - pow(2.0, (double)-Queen[i][j][1]));

			if (King[i][j][0] != King[i][j][1]) {
				temp = 1;
				temp = -1 <= (King[i][j][1] - King[i][j][0]) / 6 ? (King[i][j][1] - King[i][j][0]) / 6 : -1;
				pking = 1 >= temp ? temp : 1;
			}
			
			//justfortest
			//cout << "Queen[" << i << "][" << j << "][0] = " << Queen[i][j][0] << " Queen[" << i << "][" << j << "][1] = " << Queen[i][j][1] << endl;
			//cout << "King[" << i << "][" << j << "][0] = " << King[i][j][0] << " King[" << i << "][" << j << "][1] = " << King[i][j][1] << endl;
			//cout << "pqueen = " << pqueen << " pking = " << pking << endl;
		}
	}
	/*
	cout << "tqueen" << tqueen << " tking" << tking << endl;
	cout << "pqueen" << pqueen << " pking" << pking << endl;
	cout << Adjust[nowtime][0] << "\t" << tqueen << "\t" << Adjust[nowtime][0] * tqueen << endl
		<< "\t" << Adjust[nowtime][1] << "\t" << tking << "\t" << Adjust[nowtime][1] * tking << endl
		<< "\t" << Adjust[nowtime][2] << "\t" << pqueen << "\t" << Adjust[nowtime][2] * pqueen<<endl
		<< "\t" << Adjust[nowtime][3] << "\t" << pking << "\t" << Adjust[nowtime][3] * pking<<endl
		<< "\t" << Adjust[nowtime][4] << "\t" << mobility << "\t" << Adjust[nowtime][4] * mobility << endl;
	cout << Adjust[nowtime][0] * tqueen + Adjust[nowtime][1] * tking + Adjust[nowtime][2] * pqueen + Adjust[nowtime][3] * pking + Adjust[nowtime][4] * mobility;
	*/
	return Adjust[nowtime][0] * tqueen + Adjust[nowtime][1] * tking + Adjust[nowtime][2] * pqueen + Adjust[nowtime][3] * pking + Adjust[nowtime][4] * mobility;
}

// Both װ����alpha-beta��֦�������������
double AmazonsAi(int color, int depth, double alpha, double beta)
{
	int posCount = 0;
	double maxCount = MINN;
	double minCount = MAXX;

	//�����趨����ȣ���������������ֵ���˳�
	if (depth == SetDepth) {
		return AmazonsCount();
	}

	//�м���ȣ�ģ������
	for (int i = 0; i < GRIDSIZE; ++i) {
		for (int j = 0; j < GRIDSIZE; ++j) {
			if (gridInfo[i][j] != color) continue;
			for (int k = 0; k < 8; ++k) {
				for (int delta1 = 1; delta1 < GRIDSIZE; delta1++) {
					int xx = i + dx[k] * delta1;
					int yy = j + dy[k] * delta1;
					if (gridInfo[xx][yy] != 0 || !inMap(xx, yy))
						break;
					for (int l = 0; l < 8; ++l) {
						for (int delta2 = 1; delta2 < GRIDSIZE; delta2++) {
							int xxx = xx + dx[l] * delta2;
							int yyy = yy + dy[l] * delta2;
							if (!inMap(xxx, yyy))
								break;
							if (gridInfo[xxx][yyy] != 0 && !(i == xxx && j == yyy))
								break;

							//ģ������
							gridInfo[i][j] = 0;
							gridInfo[xx][yy] = color;
							gridInfo[xxx][yyy] = OBSTACLE;

							//������һ��
							double tempCount = AmazonsAi(-color, depth + 1, alpha, beta);
							

							//��һ��Ҫ������
							if (depth == 0 && tempCount >= maxCount)
							{
								//printgridInfo();
								//system("pause");
								beginPos[0] = i;
								beginPos[1] = j;
								possiblePos[0] = xx;
								possiblePos[1] = yy;
								obstaclePos[0] = xxx;
								obstaclePos[1] = yyy;	
							}

							maxCount = tempCount >= maxCount ? tempCount : maxCount;
							minCount = tempCount <= minCount ? tempCount : minCount;


							//����������֦�㷨
							//printgridInfo();
							//if(depth<1)cout << "depth = " << depth <<"\ttempcount = "<<tempCount<< " \tlpha = " << alpha << "\tbeta = " << beta << endl;
							//if(depth<0)system("pause");




							//����
							gridInfo[xxx][yyy] = 0;
							gridInfo[xx][yy] = 0;
							gridInfo[i][j] = color;
							posCount++;
							
							
							//�޸�alpha��beta��ֵ
							if (depth % 2 == 0) {
								if (tempCount >= alpha) {
									alpha = tempCount;
									if (beta < alpha)return maxCount;
								}
							}
							else if (depth % 2 == 1) {
								if (tempCount <= beta) {
									beta = tempCount;
									if (beta < alpha)return minCount;
								}
							}


						}
					}
				}
			}
		}
	}
	if (depth % 2 == 0)return maxCount;//Ҳ������һ���ӽڵ��ֵ��depth��0��ʼ����0���ֵӦΪ��1���е����
	else if (depth % 2 == 1)return minCount;
}

// Both �ж��Ƿ����ӿ���
bool judge(int color)
{
	int posCount = 0;
	for (int i = 0; i < GRIDSIZE; ++i) {
		for (int j = 0; j < GRIDSIZE; ++j) {
			if (gridInfo[i][j] == color) {
				for (int k = 0; k < 8; ++k) {
					for (int delta1 = 1; delta1 < GRIDSIZE; delta1++) {
						int xx = i + dx[k] * delta1;
						int yy = j + dy[k] * delta1;
						if (gridInfo[xx][yy] != 0 || !inMap(xx, yy)) break;
						else return true;
					}
				}
			}
		}
	}
	return posCount;
}

// Both ִ������
void putchess(int x0, int y0, int x1, int y1, int x2, int y2, int color)
{
	gridInfo[x0][y0] = 0;
	gridInfo[x1][y1] = color;
	gridInfo[x2][y2] = OBSTACLE;
}

// Both ��������
void removechess(int x0, int y0, int x1, int y1, int x2, int y2, int color)
{
	gridInfo[x2][y2] = 0;
	gridInfo[x1][y1] = 0;
	gridInfo[x0][y0] = color;
}

// TA_only ��¼λ��
void rememberchess(int x0, int y0, int x1, int y1, int x2, int y2, int color)
{
	remember[turnID - 1][1] = x0;
	remember[turnID - 1][2] = y0;
	remember[turnID - 1][3] = x1;
	remember[turnID - 1][4] = y1;
	remember[turnID - 1][5] = x2;
	remember[turnID - 1][6] = y2;
	remember[turnID - 1][0] = color;
}

// TA_only �������ӽ���
bool manputchess()
{
	int x0, y0, x1, y1, x2, y2;
	cout << "Please enter six numbers\n"
		<< "Interrupt and back to menu please enter -1\n";
	while (1) {
		cin >> x0;
		if (x0 == -1) return true;
		else {
			cin >> y0 >> x1 >> y1 >> x2 >> y2;
			x0--; y0--; x1--; y1--; x2--; y2--;//����Ȼ����1��ͷת��Ϊ��������0��ͷ
			if (ProcStep(x0, y0, x1, y1, x2, y2, -currBotColor) && PathCheck(x0, y0, x1, y1) && SotCheck(x0, y0, x1, y1, x2, y2)) {
				putchess(x0, y0, x1, y1, x2, y2, -currBotColor); 
				rememberchess(x0, y0, x1, y1, x2, y2, -currBotColor);
				turnID++;
				return false;
			}
			else {
				cout << "Chess in wrong position . Please try again .\n";
			}
		}
	}
}

// TA_only ����
void saveTurn()
{
	ifstream infile1("numberOfRound.txt", ios::in);
	if (!infile1)cout << "error" << endl;
	int rounds = 0;
	infile1 >> rounds;
	infile1.close();

	ifstream infile2("namelist.txt", ios::in);
	if (!infile2)cout << "error" << endl;
	cout << rounds << " rounds have been saved ." << endl;
	char name[100];
	cout << "No.\tName" << endl;
	if (rounds != 0) {
		for (int i = 0; i < rounds; i++) {
			infile2 >> name;
			cout << i + 1 << "\t" << name << endl;
		}
	}
	infile2.close();

	ofstream outfile1("numberOfRound.txt", ios::out);
	if (!outfile1)cout << "error" << endl;
	outfile1 << rounds + 1;
	outfile1.close();

	ofstream outfile2("namelist.txt", ios::app);
	if (!outfile2)cout << "error" << endl;
	cout << "Please input the name of file with \".txt\" as the end ." << endl
		<< "If you want to cover a round saved , you can use the same file name ." << endl;
	cin >> name;
	outfile2 << name<< " " ;
	outfile2.close();

	ofstream outfile3(name, ios::out);
	outfile3 << turnID - 1 << " ";
	for (int i = 0; i < turnID - 1; i++) {
		for (int j = 0; j < 7; j++) {
			outfile3 << remember[i][j] << " ";
		}
	}
	cout << "File has been saved as " << name << endl;
	outfile3.close();
	return;
}

// TA_only ѡ�������
void choosewhichRound()
{
	ifstream infile("numberOfRound.txt", ios::in);
	if (!infile)cout << "error" << endl;
	int rounds = 0;
	infile >> rounds;
	if (rounds == 0) {
		remember[0][0] = 0; //���ݱ�ǣ���û�̱���ʱ
		return;
	}
	infile.close();

	ifstream infile1("namelist.txt", ios::in);
	if (!infile1)cout << "error" << endl;
	cout << rounds << " rounds have been saved . Which round would you like to read ?" << endl;
	char name[100];
	cout << "No.\tName" << endl;
	for (int i = 0; i < rounds; i++) {
		infile1 >> name;
		cout << i + 1 << "\t" << name << endl;
	}
	infile1.close();

	ifstream infile3("namelist.txt", ios::in);
	cout << "Enter the No.to choose : ";
	int roundToRead = 0;
	while (1) {
		cin >> roundToRead;
		if (roundToRead > 0 && roundToRead <= rounds)break;
		else cout << "Input number 1 to " << rounds << " to choose.Please try again : ";
	}
	for (int i = 0; i < roundToRead; i++) {
		infile3>> name;
	}
	infile3.close();

	

	ifstream infile2(name, ios::in);
	if (!infile2)cout << "error" << endl;
	infile2 >> turns;
	for (int i = 0; i < turns; i++){
		for (int j = 0; j < 7; j++) {
			infile2 >> remember[i][j];
		}
	}
}

// TA_only ����
void readRound()
{
	cleangridInfo();
	printgridInfo();
	cout << "Next step enter 1 , last step enter -1 , back to menu enter 0 ." << endl;
	
	int tempcin = 0;
	int turnToView = -1;
	
	while (cin >> tempcin) {
		switch (tempcin) {
		case 1: {
			turnToView++;
			if (turnToView == turns) {
				cout << "Game over" << endl;
				cout << "Last step enter -1 , back to menu enter 0 ." << endl;
				break;
			}
			putchess(
				remember[turnToView][1], remember[turnToView][2],
				remember[turnToView][3], remember[turnToView][4],
				remember[turnToView][5], remember[turnToView][6],
				remember[turnToView][0]);
			system("cls");
			printgridInfo();
			cout << "Next step enter 1 , last step enter -1 , back to menu enter 0 ." << endl;
			break;
		}
		case -1: {
			if (turnToView < 0) {
				cout << "Reach the start ." << endl;
				cout << "Next step enter 1 , back to menu enter 0 ." << endl;
				break;
			}
			removechess(
				remember[turnToView][1], remember[turnToView][2],
				remember[turnToView][3], remember[turnToView][4],
				remember[turnToView][5], remember[turnToView][6],
				remember[turnToView][0]);
			turnToView--;
			system("cls");
			printgridInfo();
			cout << "Next step enter 1 , last step enter -1 , back to menu enter 0 ." << endl;
			break;
		}
		case 0: return;
		default: cout << "Wrong input . Please try again : ";
		}
	}
}

// TA_only �����Ǻ������������õ�main����
int main()
{
	welcome();
	while (1) {
		int tem = menu();// ��ʾ���˵��������˵�ѡ��
		switch (tem) {
		case 1: {
			int x0, y0, x1, y1, x2, y2;

			choosecolor();// ����ѡ����ɫ
			cleangridInfo();// ��ʼ������
			printgridInfo();// ��ӡ����
			turnID = 1;
			while (1) {
				if (turnID != 1 || currBotColor != grid_black) {// �ڷ����֣���������Ǻڷ���������һ�ֲ��ȴ�����ֱ������
					bool temp = manputchess();// ������
					if (temp) break;// ���������-1�򷵻�true���жϣ��������˵�
				}

				switch (turnID/10) {
				case 0:nowtime = 0; break;
				case 5:
				case 6:nowtime = 2; break;
				default:nowtime = 1;
				}
				SetDepth = turnID / Changefre + StartDepth;

				AmazonsAi(currBotColor, 0, MINN, MAXX);// ����AI�������ӷ���

				if (judge(currBotColor)) {
					putchess(beginPos[0], beginPos[1], possiblePos[0], possiblePos[1], obstaclePos[0], obstaclePos[1], currBotColor);
					rememberchess(beginPos[0], beginPos[1], possiblePos[0], possiblePos[1], obstaclePos[0], obstaclePos[1], currBotColor);
					turnID++;
				}
				else { cout << "You are the winnner ! ^_^" << endl; break; }

				system("cls");
				cout << "Welcome to Amazons Games!\n";
				printgridInfo();
				if (!judge(-currBotColor)) {
					cout << "You lost . Don't worry , you can try again !" << endl;
					break;
				}
			}
			break;
		}
		case 2: {
			if (remember[0][0] == 0) {
				cout << "No round to save , please start a round first ." << endl;
				break;
			}
			saveTurn();
			break;
		}
		case 3: {
			choosewhichRound();
			if (remember[0][0] == 0) {
				cout << "No round saved , please save a round first ." << endl;
				break;
			}
			readRound();
			break;
		}
		case 4:return 0;
		default:cout << "Input number 1 to 4 to choose . Please try again .\n"; continue;
		}
	}
	return 0;
}