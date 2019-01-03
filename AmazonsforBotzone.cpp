/*
Amazons game v4.0
from:Zhuming Shi
Student ID:1800011723
E-mail:shizhuming@pku.edu.cn
College of Chemistry and Molecular Engineering
*/

/*
v4.0

�޸ĵ����
0 ��һ���Ż���queenmove���㷨

���޸ĵ����
0 �������ü�֦�㷨
1 ������λ�����¼���޸�����
2 ����menu��������v1.4�е�goto���
3 ͨ�������¼��������λ�ü������ش���64->8

Dec.31 17:35 2018
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

using namespace std;

int currBotColor; // ����ִ����ɫ��1Ϊ�ڣ�-1Ϊ�ף�����״̬��ͬ��
int gridInfo[GRIDSIZE][GRIDSIZE] = { 0 }; // ��x��y����¼����״̬
int dx[] = { -1,-1,-1,0,0,1,1,1 };//���з���
int dy[] = { -1,0,1,-1,1,-1,0,1 };//���з���

int SetDepth = 1;//���õ��������
int Changefre = 15;//ÿ��������һ���������
int beginPos[2], possiblePos[2], obstaclePos[2];//��¼������ӷ�������Ai���������ݸ�main����ʹ��

double AdjustK1 = 5;//������������1����Ӧ���ӷ�������ռ�ı���
double AdjustK2 = 5;//������������2����Ӧ��ռ�ݵ���������ռ�ı���
double AdjustK3 = 0.5;//������������3����Ӧ��queenmove���ʱ�յصļ�ֵ
//double AdjustK4 = 1;//������������4����Ӧ����С���Χ��ռ�ı���

int remember[7][70][11] = { 0 };//TA_only ���̸����ã�ÿһ��һ�����turnInfo[0]��¼�ڻ��ǰף����������¼����
int turnid[11] = { 0 };//TA_only ���̸����ã���¼��������̵��ܻغ���

// TA_only ��ӡ����
void printgridInfo()
{
	char point[4] = { 'o',0,'*','#' };
	int i, j;
	cout << "   1   2   3   4   5   6   7   8  " << endl;
	cout << " ���������Щ������Щ������Щ������Щ������Щ������Щ������Щ�������" << endl;
	for (i = 0; i < 7; i++) {
		cout << i + 1;
		for (j = 0; j < 8; j++) {
			cout << "��" << " " << point[(gridInfo[j][i]) + 1] << " ";
		}
		cout << "��" << endl << " ���������੤�����੤�����੤�����੤�����੤�����੤�����੤������" << endl;
	}
	cout << 8;
	for (j = 0; j < 8; j++) {
		cout << "��" << " " << point[(gridInfo[j][7]) + 1] << " ";
	}
	cout << "��" << endl << " ���������ة������ة������ة������ة������ة������ة������ة�������" << endl;
}

// Both �ж��Ƿ��ڵ�ͼ��
inline bool inMap(int x, int y)
{
	if (x < 0 || x >= GRIDSIZE || y < 0 || y >= GRIDSIZE)
		return false;
	return true;
}

// Both �����괦���ӣ�����Ƿ�Ϸ���ģ������
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

int Queen[GRIDSIZE][GRIDSIZE][2] = { 0 };//��¼queenmove

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
									if (beta < alpha)return minCount;
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


// TA_only �ж������Ƿ����ӿ���
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
// Botzone_only ������Botzoneƽ̨���е�main����

int main()
{
	int x0, y0, x1, y1, x2, y2;

	// ��ʼ������
	gridInfo[0][(GRIDSIZE - 1) / 3] = gridInfo[(GRIDSIZE - 1) / 3][0]
		= gridInfo[GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)][0]
		= gridInfo[GRIDSIZE - 1][(GRIDSIZE - 1) / 3] = grid_black;
	gridInfo[0][GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)] = gridInfo[(GRIDSIZE - 1) / 3][GRIDSIZE - 1]
		= gridInfo[GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)][GRIDSIZE - 1]
		= gridInfo[GRIDSIZE - 1][GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)] = grid_white;

	int turnID;
	cin >> turnID;
	SetDepth = turnID / Changefre + 1;
	// ���뵽��ǰ�غ�Ϊֹ���Լ��Ͷ��ֵ������ж����Ӷ��Ѿ���ָ�����ǰ�غ�
	currBotColor = grid_white; // �ȼ����Լ��ǰ׷�
	for (int i = 0; i < turnID; i++)
	{
		// ������Щ��������𽥻ָ�״̬����ǰ�غ�

		// �����Ƕ����ж�
		cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2;
		if (x0 == -1)
			currBotColor = grid_black; // ��һ�غ��յ�������-1, -1��˵�����Ǻڷ�
		else
		{
			gridInfo[x0][y0] = 0;
			gridInfo[x1][y1] = -currBotColor;
			gridInfo[x2][y2] = OBSTACLE;
		}

		// Ȼ�����Լ���ʱ���ж�
		// �����ж��ܱ��Լ��ж���һ��
		if (i < turnID - 1)
		{
			cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2;
			if (x0 >= 0)
			{
				gridInfo[x0][y0] = 0;
				gridInfo[x1][y1] = currBotColor;
				gridInfo[x2][y2] = OBSTACLE;
			}
		}
	}
	int startX, startY, resultX, resultY, obstacleX, obstacleY;
	int tempp;
	tempp = AmazonsAi(currBotColor, 0, MINN, MAXX);
	if (judge(currBotColor))
	{
		//srand(time(0));
		//choice = rand() % posCount;
		startX = beginPos[0];
		startY = beginPos[1];
		resultX = possiblePos[0];
		resultY = possiblePos[1];
		obstacleX = obstaclePos[0];
		obstacleY = obstaclePos[1];
	}
	else
	{
		startX = -1;
		startY = -1;
		resultX = -1;
		resultY = -1;
		obstacleX = -1;
		obstacleY = -1;
	}
	cout << startX << ' ' << startY << ' ' << resultX << ' ' << resultY << ' ' << obstacleX << ' ' << obstacleY << endl;
	return 0;
}
