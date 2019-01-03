/*
Amazons game v4.3
from:Zhuming Shi
Student ID:1800011723
E-mail:shizhuming@pku.edu.cn
College of Chemistry and Molecular Engineering
*/

/*
v4.3

修改的事项：
0 完成用文件实现存盘，读盘的功能
1 用while循环完善容错机制
2 修复接近终局时递归到终点无法调用评估函数的bug

待修改的事项：
0 尝试用位运算记录和修改棋盘
1 增添menu函数代替v1.4中的goto语句
2 通过数组记录现有棋子位置减少搜素次数64->8

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
#define Changefre 10 //每几步增加一层搜索深度
#define StartDepth 1

using namespace std;

int currBotColor; // 我所执子颜色（1为黑，-1为白，棋盘状态亦同）
int gridInfo[GRIDSIZE][GRIDSIZE] = { 0 }; // 先x后y，记录棋盘状态
int dx[] = { -1,-1,-1,0,0,1,1,1 };//可行方向
int dy[] = { -1,0,1,-1,1,-1,0,1 };//可行方向

int SetDepth = 1;//设置的搜索深度
//int Changefre = 10;//每几步增加一层搜索深度
int beginPos[2], possiblePos[2], obstaclePos[2];//记录最佳走子方法，由Ai给出，传递给main函数使用

int Queen[GRIDSIZE][GRIDSIZE][2] = { 0 };//记录queenmove
int King[GRIDSIZE][GRIDSIZE][2] = { 0 };//记录kingmove

double AdjustK = 0.5;//评估函数调参3，对应于queenmove相等时空地的价值

int nowtime = 0;//表示当前局面开局，中局或者残局

double Adjust[3][5] = {
	0.14,0.37,0.13,0.13,0.20,
	0.30,0.25,0.20,0.20,0.05,
	0.80,0.10,0.05,0.05,0.00};

int turnID = 1;
int turns = 0;
int remember[140][7] = { 0 };//TA_only 存盘复盘用，每一步一共七项，turnInfo[][0]记录黑还是白，其余六项记录操作

char nameOfFlie[7] = { "r0.txt" };

// TA_only 打印欢迎界面
// 1080p分辨率，125%缩放的windows笔记本电脑屏幕上可以显示,不保证所有显示器都能正确显示！
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

// TA_only 打印棋子
// 1080p分辨率，125%缩放的windows笔记本电脑屏幕上可以显示,不保证所有显示器都能正确显示！
void printchess(int Info)
{
	switch (Info) {
	case 0:cout << "○"; return;
	case 1:cout << "  "; return;
	case 2:cout << "●"; return;
	case 3:cout << "▲"; return;
	}
}

// TA_only 打印棋盘
// 1080p分辨率，125%缩放的windows笔记本电脑屏幕上可以显示,不保证所有显示器都能正确显示！
void printgridInfo()
{
	//char point[4] = { 'o',0,'*','#' };
	int i, j;
	cout << "   1   2   3   4   5   6   7   8  " << endl;
	cout << " ┌───┬───┬───┬───┬───┬───┬───┬───┐" << endl;
	for (i = 0; i < 7; i++) {
		cout << i + 1;
		for (j = 0; j < 8; j++) {
			cout << "│";cout << " ";
			printchess(gridInfo[j][i] + 1);
			
		}
		cout << "│" << endl << " ├───┼───┼───┼───┼───┼───┼───┼───┤" << endl;
	}
	cout << 8;
	for (j = 0; j < 8; j++) {
		cout << "│";cout << " ";
		printchess(gridInfo[j][7] + 1);
		
	}
	cout << "│" << endl << " └───┴───┴───┴───┴───┴───┴───┴───┘" << endl;
}

// TA_only 打印菜单
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

// TA_only 选择棋子颜色
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

// Both 清空棋盘并且放置初始落子
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

// Both 判断是否在地图内
inline bool inMap(int x, int y)
{
	if (x < 0 || x >= GRIDSIZE || y < 0 || y >= GRIDSIZE)
		return false;
	return true;
}

// Both 检查是否合法
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

// Both Queen数组清零
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

// Both queenmove生成工具
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

// Both kingmove生成工具
void KingMove(int color)
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

// Both mobility计算函数
void Mobility(int &posCount)
{
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
}

// Both Amazons评估函数
double AmazonsCount()
{
	int mobility = 0;

	Mobility(mobility);//计算灵活度

	double tqueen = 0;//由queenmove给出的领地
	double tking = 0;//由kingmove给出的领地

	//生成queenmove和kingmove
	cleanQueenAndKing();

	QueenMove(currBotColor);
	QueenMove(-currBotColor);

	KingMove(currBotColor);
	KingMove(-currBotColor);

	//计算领地t
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

	//计算位置p
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

// Both 装备了alpha-beta剪枝的深度优先搜索
double AmazonsAi(int color, int depth, double alpha, double beta)
{
	int posCount = 0;
	double maxCount = MINN;
	double minCount = MAXX;

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


							//下午来检查剪枝算法
							//printgridInfo();
							//if(depth<1)cout << "depth = " << depth <<"\ttempcount = "<<tempCount<< " \tlpha = " << alpha << "\tbeta = " << beta << endl;
							//if(depth<0)system("pause");




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
	if (depth % 2 == 0)return maxCount;//也就是这一层子节点的值，depth从0开始，第0层的值应为第1层中的最大
	else if (depth % 2 == 1)return minCount;
}

// Both 判断是否无子可下
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

// Both 执行落子
void putchess(int x0, int y0, int x1, int y1, int x2, int y2, int color)
{
	gridInfo[x0][y0] = 0;
	gridInfo[x1][y1] = color;
	gridInfo[x2][y2] = OBSTACLE;
}

// Both 撤销落子
void removechess(int x0, int y0, int x1, int y1, int x2, int y2, int color)
{
	gridInfo[x2][y2] = 0;
	gridInfo[x1][y1] = 0;
	gridInfo[x0][y0] = color;
}

// TA_only 记录位置
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

// TA_only 人类落子交互
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
			x0--; y0--; x1--; y1--; x2--; y2--;//从自然语言1开头转化为机器语言0开头
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

// TA_only 存盘
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

// TA_only 选择读哪盘
void choosewhichRound()
{
	ifstream infile("numberOfRound.txt", ios::in);
	if (!infile)cout << "error" << endl;
	int rounds = 0;
	infile >> rounds;
	if (rounds == 0) {
		remember[0][0] = 0; //传递标记，当没盘被存时
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

// TA_only 读盘
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

// TA_only 以下是和助教下棋所用的main函数
int main()
{
	welcome();
	while (1) {
		int tem = menu();// 显示主菜单，返回人的选择
		switch (tem) {
		case 1: {
			int x0, y0, x1, y1, x2, y2;

			choosecolor();// 人类选择颜色
			cleangridInfo();// 初始化棋盘
			printgridInfo();// 打印棋盘
			turnID = 1;
			while (1) {
				if (turnID != 1 || currBotColor != grid_black) {// 黑方先手，如果机器是黑方，机器第一轮不等待输入直接落子
					bool temp = manputchess();// 人下棋
					if (temp) break;// 如果人输入-1则返回true，中断，返回主菜单
				}

				switch (turnID/10) {
				case 0:nowtime = 0; break;
				case 5:
				case 6:nowtime = 2; break;
				default:nowtime = 1;
				}
				SetDepth = turnID / Changefre + StartDepth;

				AmazonsAi(currBotColor, 0, MINN, MAXX);// 调用AI生成落子方法

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