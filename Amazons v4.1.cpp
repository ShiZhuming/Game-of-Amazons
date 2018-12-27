/*
Amazons game v4.1
from:Zhuming Shi
Student ID:1800011723
E-mail:shizhuming@pku.edu.cn
College of Chemistry and Molecular Engineering
*/

/*
v4.1

�޸ĵ����
0 �����-�¼�֦�㷨

���޸ĵ����
0 �������ü�֦�㷨
1 ������λ�����¼���޸�����
2 ����menu��������v1.4�е�goto���
3 ͨ�������¼��������λ�ü������ش���64->8
4 �޸��˽ӽ��վ�ʱ�ݹ鵽�յ��޷���������������bug
Dec.27 12:50 2018
*/

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#define GRIDSIZE 8
#define OBSTACLE 2
#define judge_black 0
#define judge_white 1
#define grid_black 1
#define grid_white -1
#define MAXX 1000000
#define MINN -1000000
#define Changefre 10 //ÿ��������һ���������

using namespace std;

int currBotColor; // ����ִ����ɫ��1Ϊ�ڣ�-1Ϊ�ף�����״̬��ͬ��
int gridInfo[GRIDSIZE][GRIDSIZE] = { 0 }; // ��x��y����¼����״̬
int dx[] = { -1,-1,-1,0,0,1,1,1 };//���з���
int dy[] = { -1,0,1,-1,1,-1,0,1 };//���з���

int SetDepth = 1;//���õ��������
//int Changefre = 10;//ÿ��������һ���������
int beginPos[2], possiblePos[2], obstaclePos[2];//��¼������ӷ�������Ai���������ݸ�main����ʹ��

int Queen[GRIDSIZE][GRIDSIZE][2] = { 0 };//��¼queenmove

double AdjustK1 = 5;//������������1����Ӧ���ӷ�������ռ�ı���
double AdjustK2 = 5;//������������2����Ӧ��ռ�ݵ���������ռ�ı���
double AdjustK3 = 0.5;//������������3����Ӧ��queenmove���ʱ�յصļ�ֵ
double AdjustK4 = 1;//������������4����Ӧ����С���Χ��ռ�ı���

int turnID = 1;
int remember[7][70] = { 0 };//TA_only ���̸����ã�ÿһ��һ�����turnInfo[0]��¼�ڻ��ǰף����������¼����

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
		<< "2	Save this turn\n"
		<< "3	review last turn\n"
		<< "4	exit\n"
		<< "You can enter the number to choose one\n";
	int tempnumber;
	cin >> tempnumber;
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
void cleanQueen()
{
	for (int i = 0; i < GRIDSIZE; ++i) {
		for (int j = 0; j < GRIDSIZE; ++j) {
			Queen[i][j][0] = 0;
			Queen[i][j][1] = 0;
		}
	}
}

//queenmove���ɹ���
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
}

// Both Amazons���Χ���㺯��
double AmazonsCount()
{
	double area = 0;

	int posCount = 0;//��¼���Ե��߷�

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

	cleanQueen();
	QueenMove(currBotColor);
	QueenMove(-currBotColor);

	for (int i = 0; i < GRIDSIZE; i++) {
		for (int j = 0; j < GRIDSIZE; j++) {
			if (Queen[i][j][0] > Queen[i][j][1])area--;
			else if (Queen[i][j][0] < Queen[i][j][1])area++;
			else if (Queen[i][j][0] == Queen[i][j][1] && Queen[i][j][0] != 0)area = area + AdjustK3;
		}
	}
	return AdjustK1 * posCount + AdjustK2 * area;
}

// Both ���ɲ�ģ�����ӣ�������ȵݹ�����
/*double AmazonsAi(int color, int depth)
{
	int posCount = 0;
	int maxCount = MINN;
	int minCount = MAXX;

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
							double tempCount = AmazonsAi(-color, depth + 1);

							//��һ��Ҫ������
							if (depth == 0 && tempCount >= maxCount)
							{
								beginPos[0] = i;
								beginPos[1] = j;
								possiblePos[0] = xx;
								possiblePos[1] = yy;
								obstaclePos[0] = xxx;
								obstaclePos[1] = yyy;
							}

							maxCount = tempCount >= maxCount ? tempCount : maxCount;
							minCount = tempCount <= maxCount ? tempCount : minCount;

							//����
							gridInfo[xxx][yyy] = 0;
							gridInfo[xx][yy] = 0;
							gridInfo[i][j] = color;
							posCount++;
						}
					}
				}
			}
		}
	}
	if (depth % 2 == 0)return maxCount;
	else if (depth % 2 == 1)return minCount;
}*/

double AmazonsAi(int color, int depth, int alpha, int beta)
{
	int posCount = 0;
	int maxCount = MINN;
	int minCount = MAXX;

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
								beginPos[0] = i;
								beginPos[1] = j;
								possiblePos[0] = xx;
								possiblePos[1] = yy;
								obstaclePos[0] = xxx;
								obstaclePos[1] = yyy;
							}

							maxCount = tempCount >= maxCount ? tempCount : maxCount;
							minCount = tempCount <= maxCount ? tempCount : minCount;

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
									if(beta<alpha)return minCount;
								}
							}


						}
					}
				}
			}
		}
	}
	if (depth % 2 == 0)return maxCount;
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
				putchess(x0, y0, x1, y1, x2, y2, -currBotColor); return false;
			}
			else {
				cout << "Chess in wrong position . Please try again .\n";
			}
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

				SetDepth = turnID++ / Changefre + 2;
				//AmazonsAi(currBotColor, 0);

				AmazonsAi(currBotColor, 0, MINN, MAXX);// ����AI�������ӷ���

				if (judge(currBotColor)) putchess(beginPos[0], beginPos[1], possiblePos[0], possiblePos[1], obstaclePos[0], obstaclePos[1], currBotColor);
				else { cout << "You are the winnner ! ^_^" << endl; break; }

				system("cls");
				cout << "Welcome to Amazons Games!\n";
				printgridInfo();
				if (!judge(-currBotColor)) { cout << "You lost . Don't worry , you can try again !" << endl; break; }
			}
			break;
		}
		case 2:
		case 3:
		case 4:return 0;
		default:cout << "Input number 1 to 4 to choose . Please try again .\n"; continue;
		}
	}
	return 0;
}

/*
#include<iostream>
using namespace std;
char a[8][8][180] = { 0 };
int f = 0, ab[60] = { 0 }, bb[4], ww[4];

void showchessboard() {
	int i, j;
	cout << "   1   2   3   4   5   6   7   8  " << endl;
	cout << " ���������Щ������Щ������Щ������Щ������Щ������Щ������Щ�������" << endl;
	for (i = 0; i < 7; i++) {
		cout << i + 1;
		for (j = 0; j < 8; j++) {
			cout << "��" << " " << a[i][j][f] << " ";
		}
		cout << "��" << endl << " ���������੤�����੤�����੤�����੤�����੤�����੤�����੤������" << endl;
	}
	cout << 8;
	for (j = 0; j < 8; j++) {
		cout << "��" << " " << a[7][j][f] << " ";
	}
	cout << "��" << endl << " ���������ة������ة������ة������ة������ة������ة������ة�������" << endl;
	cout << f / 3 + 1 << ' ' << f % 3 + 1 << ' ' << ':';
}
int ambit(int n) {
	int b = n / 10, c = n % 10, i, p = 0;
	for (i = 1; i < b; i++) {
		if (a[b - i - 1][c - 1][f] == 0) {
			ab[p] = 10 * (b - i) + c;
			p++;
		}
		else break;
	}
	for (i = 1; i < 9 - b; i++) {
		if (a[b + i - 1][c - 1][f] == 0) {
			ab[p] = 10 * (b + i) + c;
			p++;
		}
		else break;
	}
	for (i = 1; i < c; i++) {
		if (a[b - 1][c - i - 1][f] == 0) {
			ab[p] = 10 * b + c - i;
			p++;
		}
		else break;
	}
	for (i = 1; i < 9 - c; i++) {
		if (a[b - 1][c + i - 1][f] == 0) {
			ab[p] = 10 * b + c + i;
			p++;
		}
		else break;
	}
	for (i = 1; i < b&&i < 9 - c; i++) {
		if (a[b - i - 1][c + i - 1][f] == 0) {
			ab[p] = 10 * (b - i) + c + i;
			p++;
		}
		else break;
	}
	for (i = 1; i < b&&i < c; i++) {
		if (a[b - i - 1][c - i - 1][f] == 0) {
			ab[p] = 10 * (b - i) + c - i;
			p++;
		}
		else break;
	}
	for (i = 1; i < 9 - b && i < c; i++) {
		if (a[b + i - 1][c - i - 1][f] == 0) {
			ab[p] = 10 * (b + i) + c - i;
			p++;
		}
		else break;
	}
	for (i = 1; i < 9 - b && i < 9 - c; i++) {
		if (a[b + i - 1][c + i - 1][f] == 0) {
			ab[p] = 10 * (b + i) + c + i;
			p++;
		}
		else break;
	}
	return p;
}

void count() {
	int ii = 0, jj = 0;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++) {
			if (a[i][j][f] == '*') {
				bb[ii] = 10 * (i + 1) + j + 1;
				ii++;
			}
			else if (a[i][j][f] == 'o') {
				ww[jj] = 10 * (i + 1) + j + 1;
				jj++;
			}


		}
}

int calculate() {
	int s = 0;
	for (int i = 0; i < 4; i++) {
		s += ambit(ww[i]);
		s -= ambit(bb[i]);
	}
	return s;
}

void ai() {
	int i, j, k, ii, jj, kk, pa, pb, pc, pd, aba[60], abb[60], abc[60], abd[60], wwa[4], bba[4], s, min, max, rec[3];
	max = -240;

	count();
	for (int i = 0; i < 4; i++) {
		wwa[i] = ww[i];
	}

	for (i = 0; i < 4; i++) {
		a[wwa[i] / 10 - 1][wwa[i] % 10 - 1][f] = 0;

		pa = ambit(wwa[i]);
		for (int i = 0; i < pa; i++) {
			aba[i] = ab[i];
		}

		for (j = 0; j < pa; j++) {
			a[aba[j] / 10 - 1][aba[j] % 10 - 1][f] = 'o';

			pb = ambit(aba[j]);
			for (int i = 0; i < pb; i++) {
				abb[i] = ab[i];
			}

			for (k = 0; k < pb; k++) {
				a[abb[k] / 10 - 1][abb[k] % 10 - 1][f] = '#';

				min = 240;

				count();
				for (int i = 0; i < 4; i++) {
					bba[i] = bb[i];
				}

				for (ii = 0; ii < 4; ii++) {
					a[bba[ii] / 10 - 1][bba[ii] % 10 - 1][f] = 0;

					pc = ambit(bba[ii]);
					for (int i = 0; i < pc; i++) {
						abc[i] = ab[i];
					}

					for (jj = 0; jj < pc; jj++) {
						a[abc[jj] / 10 - 1][abc[jj] % 10 - 1][f] = '*';

						pd = ambit(abc[jj]);
						for (int i = 0; i < pd; i++) {
							abd[i] = ab[i];
						}

						for (kk = 0; kk < pd; kk++) {
							a[abd[kk] / 10 - 1][abd[kk] % 10 - 1][f] = '#';
							count(); s = calculate();
							if (s < min)min = s;
							a[abd[kk] / 10 - 1][abd[kk] % 10 - 1][f] = 0;
						}

						a[abc[jj] / 10 - 1][abc[jj] % 10 - 1][f] = 0;
					}
					a[bba[ii] / 10 - 1][bba[ii] % 10 - 1][f] = '*';
				}


				if (min > max) {
					max = min; rec[0] = wwa[i]; rec[1] = aba[j]; rec[2] = abb[k];
				}

				a[abb[k] / 10 - 1][abb[k] % 10 - 1][f] = 0;
			}

			a[aba[j] / 10 - 1][aba[j] % 10 - 1][f] = 0;
		}

		a[wwa[i] / 10 - 1][wwa[i] % 10 - 1][f] = 'o';
	}
	a[rec[0] / 10 - 1][rec[0] % 10 - 1][f] = 0;
	a[rec[1] / 10 - 1][rec[1] % 10 - 1][f] = 'o';
	a[rec[2] / 10 - 1][rec[2] % 10 - 1][f] = '#';
}


int main() {
	int i, j, n, m, b, c, d, e, p;
	char gg;
	a[0][2][f] = '*';
	a[0][5][f] = '*';
	a[2][0][f] = '*';
	a[2][7][f] = '*';
	a[5][0][f] = 'o';
	a[5][7][f] = 'o';
	a[7][2][f] = 'o';
	a[7][5][f] = 'o';
	showchessboard();
	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++) {
			a[i][j][f + 1] = a[i][j][f];
		}
	f++;
loop1:
	cin >> b;

	if (b == 0) {
		f -= 2;
		goto loop4;
	}
	c = b / 10; d = b % 10;

	if (c > 0 && c < 9 && d>0 && d < 9 && a[c - 1][d - 1][f] == '*')a[c - 1][d - 1][f] = 0;
	else {
		do cin.get(gg);
		while (gg != '\n'); goto loop1;
	}


	p = ambit(b);

	a[c - 1][d - 1][f] = 0;
	for (i = 0; i < p; i++)
		a[ab[i] / 10 - 1][ab[i] % 10 - 1][f] = '.';


loop6:
	system("cls");
	showchessboard();
	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++) {
			a[i][j][f + 1] = a[i][j][f];
		}
	f++;






loop2:
	cin >> b;

	if (b == 0) {
		f -= 2;
		goto loop5;
	}

	c = b / 10; d = b % 10;
	if (c > 0 && c < 9 && d>0 && d < 9 && a[c - 1][d - 1][f] == '.')a[c - 1][d - 1][f] = 0;
	else
	{
		do cin.get(gg);
		while (gg != '\n');
		goto loop2;
	}

	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++)
			if (a[i][j][f] == '.')a[i][j][f] = 0;

	p = ambit(b);
	a[c - 1][d - 1][f] = '*';

	for (i = 0; i < p; i++)
		a[ab[i] / 10 - 1][ab[i] % 10 - 1][f] = '.';


loop4:
	system("cls");
	showchessboard();
	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++) {
			a[i][j][f + 1] = a[i][j][f];
		}
	f++;






loop3:
	cin >> b;

	if (b == 0) {
		f -= 2;
		goto loop6;
	}

	c = b / 10; d = b % 10;
	if (c > 0 && c < 9 && d>0 && d < 9 && a[c - 1][d - 1][f] == '.')a[c - 1][d - 1][f] = '#';
	else {
		do cin.get(gg);
		while (gg != '\n');
		goto loop3;
	}

	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++)
			if (a[i][j][f] == '.')a[i][j][f] = 0;
	system("cls");
	showchessboard();
	ai();




loop5:
	system("cls");
	showchessboard();
	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++) {
			a[i][j][f + 1] = a[i][j][f];
		}
	f++;
	goto loop1;
	system("pause");
	return 0;
}*/