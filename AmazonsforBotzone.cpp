/*
Amazons game v4.0
from:Zhuming Shi
Student ID:1800011723
E-mail:shizhuming@pku.edu.cn
College of Chemistry and Molecular Engineering
*/

/*
v4.0

修改的事项：
0 进一步优化了queenmove的算法

待修改的事项：
0 尽快运用剪枝算法
1 尝试用位运算记录和修改棋盘
2 增添menu函数代替v1.4中的goto语句
3 通过数组记录现有棋子位置减少搜素次数64->8

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

int currBotColor; // 我所执子颜色（1为黑，-1为白，棋盘状态亦同）
int gridInfo[GRIDSIZE][GRIDSIZE] = { 0 }; // 先x后y，记录棋盘状态
int dx[] = { -1,-1,-1,0,0,1,1,1 };//可行方向
int dy[] = { -1,0,1,-1,1,-1,0,1 };//可行方向

int SetDepth = 1;//设置的搜索深度
int Changefre = 15;//每几步增加一层搜索深度
int beginPos[2], possiblePos[2], obstaclePos[2];//记录最佳走子方法，由Ai给出，传递给main函数使用

double AdjustK1 = 5;//评估函数调参1，对应落子方法数所占的比重
double AdjustK2 = 5;//评估函数调参2，对应于占据地盘总量所占的比重
double AdjustK3 = 0.5;//评估函数调参3，对应于queenmove相等时空地的价值
//double AdjustK4 = 1;//评估函数调参4，对应于最小活动范围所占的比重

int remember[7][70][11] = { 0 };//TA_only 存盘复盘用，每一步一共七项，turnInfo[0]记录黑还是白，其余六项记录操作
int turnid[11] = { 0 };//TA_only 存盘复盘用，记录被保存的盘的总回合数

// TA_only 打印棋盘
void printgridInfo()
{
	char point[4] = { 'o',0,'*','#' };
	int i, j;
	cout << "   1   2   3   4   5   6   7   8  " << endl;
	cout << " ┌───┬───┬───┬───┬───┬───┬───┬───┐" << endl;
	for (i = 0; i < 7; i++) {
		cout << i + 1;
		for (j = 0; j < 8; j++) {
			cout << "│" << " " << point[(gridInfo[j][i]) + 1] << " ";
		}
		cout << "│" << endl << " ├───┼───┼───┼───┼───┼───┼───┼───┤" << endl;
	}
	cout << 8;
	for (j = 0; j < 8; j++) {
		cout << "│" << " " << point[(gridInfo[j][7]) + 1] << " ";
	}
	cout << "│" << endl << " └───┴───┴───┴───┴───┴───┴───┴───┘" << endl;
}

// Both 判断是否在地图内
inline bool inMap(int x, int y)
{
	if (x < 0 || x >= GRIDSIZE || y < 0 || y >= GRIDSIZE)
		return false;
	return true;
}

// Both 在坐标处落子，检查是否合法或模拟落子
bool ProcStep(int x0, int y0, int x1, int y1, int x2, int y2, int color)
{
	if ((!inMap(x0, y0)) || (!inMap(x1, y1)) || (!inMap(x2, y2))) return false;
	if (gridInfo[x0][y0] != color || gridInfo[x1][y1] != 0) return false;
	if ((gridInfo[x2][y2] != 0) && !(x2 == x0 && y2 == y0)) return false;
	return true;
}

// 检查Amazons移动路径上是否没有障碍，如果没有返回true否则返回false
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

// TA_only 检查放箭路径上是否有障碍，如果没有返回true否则返回false
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

int Queen[GRIDSIZE][GRIDSIZE][2] = { 0 };//记录queenmove

// Both Queen数组清零
void cleanQueen()
{
	for (int i = 0; i < GRIDSIZE; ++i) {
		for (int j = 0; j < GRIDSIZE; ++j) {
			Queen[i][j][0] = 0;
			Queen[i][j][1] = 0;
		}
	}
}

//queenmove生成工具
void QueenMove(int color)
{
	bool visited[GRIDSIZE][GRIDSIZE] = { 0 };//递归求算中记录是否已计算过
	int wait[2][100];//一个队列，记录下一步扩展的节点
	int start = 0, end = 0;
	int move = 0;

	//寻找第一轮扩展节点
	for (int i = 0; i < GRIDSIZE; ++i) {
		for (int j = 0; j < GRIDSIZE; ++j) {
			if (gridInfo[i][j] != color) continue;//剪枝，不是Amazons就不搜索
			wait[0][end] = i; wait[1][end++] = j;//可扩展节点，记录，同时把end向后移一位
		}
	}

	if (color == currBotColor)color = 0; else if (color == -currBotColor) color = 1;

	//宽度优先搜索
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

// Both Amazons活动范围计算函数
double AmazonsCount()
{
	double area = 0;

	int posCount = 0;//记录可以的走法

	//搜索Amazons
	for (int i = 0; i < GRIDSIZE; ++i) {
		for (int j = 0; j < GRIDSIZE; ++j) {
			if (gridInfo[i][j] != currBotColor && gridInfo[i][j] != -currBotColor) continue;//剪枝，不是Amazons就不搜索

			//以下生成Amazons走法
			for (int k = 0; k < 8; ++k) {
				for (int delta1 = 1; delta1 < GRIDSIZE; delta1++) {
					int xx = i + dx[k] * delta1;
					int yy = j + dy[k] * delta1;
					if (gridInfo[xx][yy] != 0 || !inMap(xx, yy))
						break;

					//以下生成放箭方式
					for (int l = 0; l < 8; ++l) {
						for (int delta2 = 1; delta2 < GRIDSIZE; delta2++) {
							int xxx = xx + dx[l] * delta2;
							int yyy = yy + dy[l] * delta2;
							if (!inMap(xxx, yyy))
								break;
							if (gridInfo[xxx][yyy] != 0 && !(i == xxx && j == yyy))
								break;

							//获得一种完整走法
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

// Both 生成并模拟走子，进行深度递归搜索
double AmazonsAi(int color, int depth, int alpha, int beta)
{
	int posCount = 0;
	int maxCount = MINN;
	int minCount = MAXX;

	//到达设定的深度，返回评估函数的值，退出
	if (depth == SetDepth) {
		return AmazonsCount();
	}

	//中间深度，模拟落子
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

							//模拟落子
							gridInfo[i][j] = 0;
							gridInfo[xx][yy] = color;
							gridInfo[xxx][yyy] = OBSTACLE;

							//进入下一层
							double tempCount = AmazonsAi(-color, depth + 1, alpha, beta);

							//第一层要做的事
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

							//回溯
							gridInfo[xxx][yyy] = 0;
							gridInfo[xx][yy] = 0;
							gridInfo[i][j] = color;
							posCount++;

							//修改alpha，beta的值
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


// TA_only 判断人类是否无子可下
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
// Botzone_only 以下是Botzone平台运行的main函数

int main()
{
	int x0, y0, x1, y1, x2, y2;

	// 初始化棋盘
	gridInfo[0][(GRIDSIZE - 1) / 3] = gridInfo[(GRIDSIZE - 1) / 3][0]
		= gridInfo[GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)][0]
		= gridInfo[GRIDSIZE - 1][(GRIDSIZE - 1) / 3] = grid_black;
	gridInfo[0][GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)] = gridInfo[(GRIDSIZE - 1) / 3][GRIDSIZE - 1]
		= gridInfo[GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)][GRIDSIZE - 1]
		= gridInfo[GRIDSIZE - 1][GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)] = grid_white;

	int turnID;
	cin >> turnID;
	SetDepth = turnID / Changefre + 1;
	// 读入到当前回合为止，自己和对手的所有行动，从而把局面恢复到当前回合
	currBotColor = grid_white; // 先假设自己是白方
	for (int i = 0; i < turnID; i++)
	{
		// 根据这些输入输出逐渐恢复状态到当前回合

		// 首先是对手行动
		cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2;
		if (x0 == -1)
			currBotColor = grid_black; // 第一回合收到坐标是-1, -1，说明我是黑方
		else
		{
			gridInfo[x0][y0] = 0;
			gridInfo[x1][y1] = -currBotColor;
			gridInfo[x2][y2] = OBSTACLE;
		}

		// 然后是自己当时的行动
		// 对手行动总比自己行动多一个
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
