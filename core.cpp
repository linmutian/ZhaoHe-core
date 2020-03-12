#include "core.h"
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
using namespace std;
int sc[5] = { 10, 40, 160, 640, 4000 };
int dropList0227[12] = { 2,1,3,1,1,2,2,3,2,1,3,3 };
int table0227[8][8] = {
{99,99,99,99,99, 2,99,99},
{99,99,99,99, 1, 3,99,99},
{99,99,99, 3, 2, 1,99,99},
{99,99, 2, 1, 1, 2,99,99},
{99, 1, 1, 3, 2, 1,99,99},
{ 3, 3, 2, 3, 3, 1,99,99},
{99,99,99,99,99,99,99,99},
{99,99,99,99,99,99,99,99}
};

void vecCopy(vector<int> v1, vector<int> v2)
{
	v2.clear();
	for (int i = 0; i < v1.size(); i++)
	{
		v2.push_back(v1[i]);
	}
}

SandTable::SandTable()
{
	setTable(table0227);
	setList(dropList0227);
}

void SandTable::output()
{
	//cout << "输出当前沙盘 :" << endl;
	for (int i = 0; i < tableW; i++)
	{
		for (int j = 0; j < tableH; j++)
		{
			int item = table[i][j];
			if (item > 0 && item < 60) {
				switch (item / 10)
				{
				case 0:  // 白 - 普通
					SetConsoleTextAttribute(hOut, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
					break;
				case 1:  // 绿 - 高级
					SetConsoleTextAttribute(hOut, BACKGROUND_GREEN | BACKGROUND_INTENSITY);
					break;
				case 2:  // 蓝 - 精英
					SetConsoleTextAttribute(hOut, BACKGROUND_BLUE | BACKGROUND_INTENSITY);
					break;
				case 3:  // 紫 - 史诗 - 暂用粉红？
					SetConsoleTextAttribute(hOut, BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
					break;
				case 4: // 橙 - 传说 - 暂用红
					SetConsoleTextAttribute(hOut, BACKGROUND_RED | BACKGROUND_INTENSITY);
					break;

				}
				cout << " " << item % 10 << " "; // 家族种类
				SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else if (item < 0) {  // <0 为石块 
				SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				if (item < -9)
					cout << -item << " ";
				else
					cout << " " << -item << " ";
			}
			else if (item == 0) {  // 暂无，待下落
				cout << " * ";
			}
			else if (item == 99) {  // 边界之外
				cout << "   ";
			}
		}
		cout << endl;
	}
}

void SandTable::outputMatch()
{
	cout << " Mark : " << endl;
	for (int i = 0; i < tableW; i++)
	{
		for (int j = 0; j < tableH; j++)
		{
			if (match[i][j] < 10)
				cout << match[i][j] << "  ";
			else
				cout << match[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void SandTable::outputMatchList()
{
	for (int index = 0; index < matchList.size(); index++)
	{
		for (int i = 0; i < 3; i++)
		{
			cout << matchList[index][i] << " ";
		}
		cout << endl;
	}
}

// 基础函数，间接调用
//返回值： 0空，1怪，9边界，-1石头
int SandTable::type(int i, int j)
{
	if (table[i][j] == 0)
		return 0;
	else if (table[i][j] == 99)
		return 9;
	else if (table[i][j] > 0 && table[i][j] < 60)
		return 1;
	else if (table[i][j] < 0)
		return -1;
	else
		return 42;
}

int SandTable::isLack()
{
	for (int i = 0; i < tableH; i++) {
		for (int j = 0; j < tableW; j++) {
			if (table[i][j] == 0)
				return 1;
		}
	}
	return 0;
}

int  SandTable::isLineLack(int j)
{
	for (int i = 0; i < tableH; i++) {
		if (table[i][j] == 0)
			return 1;
	}
	return 0;
}

// 中间函数，间接调用,
// 初始消除
int SandTable::elimMatch0(int x1, int y1, int x2, int y2)
{
	// 清空数据
	for (int i = 0; i < tableH; i++)
	{
		for (int j = 0; j < tableW; j++)
		{
			match[i][j] = 0;
		}
	}
	int index = 0; // 消除块序号
	for (int i = 0; i < tableH; i++)
	{
		for (int j = 0; j < tableW; j++)
		{
			if (type(i, j) == 1)
			{
				if (i >= 2)
				{
					if (table[i][j] == table[i - 1][j] && table[i][j] == table[i - 2][j])
					{
						int indexExisted = 0; // 已经存在序号了吗？
						if (match[i - 1][j] > 0)
							indexExisted = match[i - 1][j];
						else if (match[i - 2][j] > 0)
							indexExisted = match[i - 2][j];
						else if (match[i][j] > 0)
							indexExisted = match[i][j];
						if (indexExisted) // 有旧序号
						{
							match[i][j] = indexExisted;
							match[i - 1][j] = indexExisted;
							match[i - 2][j] = indexExisted;
						}
						else // 新序号
						{
							index++;
							match[i][j] = index;
							match[i - 1][j] = index;
							match[i - 2][j] = index;
						}
					}
					if (j >= 2 && table[i][j] == table[i - 1][j - 1] && table[i][j] == table[i - 2][j - 2])
					{

						int indexExisted = 0; // 已经存在序号了吗？
						if (match[i - 1][j - 1] > 0)
							indexExisted = match[i - 1][j - 1];
						else if (match[i - 2][j - 2] > 0)
							indexExisted = match[i - 2][j - 2];
						else if (match[i][j] > 0)
							indexExisted = match[i][j];
						if (indexExisted) // 有旧序号
						{
							match[i][j] = indexExisted;
							match[i - 1][j - 1] = indexExisted;
							match[i - 2][j - 2] = indexExisted;
						}
						else // 新序号
						{
							index++;
							match[i][j] = index;
							match[i - 1][j - 1] = index;
							match[i - 2][j - 2] = index;
						}
					}
					if (j <= tableW - 3 && table[i][j] == table[i - 1][j + 1] && table[i][j] == table[i - 2][j + 2])
					{
						int indexExisted = 0; // 已经存在序号了吗？
						if (match[i - 1][j + 1] > 0)
							indexExisted = match[i - 1][j + 1];
						else if (match[i - 2][j + 2] > 0)
							indexExisted = match[i - 2][j + 2];
						else if (match[i][j] > 0)
							indexExisted = match[i][j];
						if (indexExisted) // 有旧序号
						{
							match[i][j] = indexExisted;
							match[i - 1][j + 1] = indexExisted;
							match[i - 2][j + 2] = indexExisted;
						}
						else // 新序号
						{
							index++;
							match[i][j] = index;
							match[i - 1][j + 1] = index;
							match[i - 2][j + 2] = index;
						}
					}
				}
				if (j >= 2 && table[i][j] == table[i][j - 1] && table[i][j] == table[i][j - 2])
				{
					int indexExisted = 0; // 已经存在序号了吗？
					if (match[i][j - 1] > 0)
						indexExisted = match[i][j - 1];
					else if (match[i][j - 2] > 0)
						indexExisted = match[i][j - 2];
					else if (match[i][j] > 0)
						indexExisted = match[i][j];
					if (indexExisted) // 有旧序号
					{
						match[i][j] = indexExisted;
						match[i][j - 1] = indexExisted;
						match[i][j - 2] = indexExisted;
					}
					else // 新序号
					{
						index++;
						match[i][j] = index;
						match[i][j - 1] = index;
						match[i][j - 2] = index;
					}
				}
			}
		}
	}

	for (int i = 0; i < tableH; i++)
	{
		for (int j = 0; j < tableW; j++)
		{
			if (type(i, j) == -1)
			{
				int isHit = 0;
				if (i > 0 && match[i - 1][j] > 0)
					isHit = 1;
				if (j > 0 && match[i][j - 1] > 0)
					isHit = 1;
				if (i < tableH - 1 && match[i + 1][j] > 0)
					isHit = 1;
				if (j < tableW - 1 && match[i][j + 1] > 0)
					isHit = 1;
				if (isHit)
					table[i][j]++;
			}
		}
	}

	int isMatch1 = match[x1][y1] > 0 ? 1 : 0;
	int isMatch2 = match[x2][y2] > 0 ? 1 : 0;
	if (!isMatch1 && !isMatch2)
		return 0;
	int lv1 = 0;
	int lv2 = 0;
	int count1 = 0;
	int count2 = 0;
	for (int i = 0; i < tableH; i++) {
		for (int j = 0; j < tableW; j++) {
			if (match[i][j] > 0)
			{
				if (match[i][j] == 1)
				{
					count1++;
					lv1 = table[i][j] / 10;
				}
				else
				{
					count2++;
					lv2 = table[i][j] / 10;
				}
				if (i == x1 && j == y1) {
					table[i][j] += 10;
				}
				else if (i == x2 && j == y2) {
					table[i][j] += 10;
				}
				else {
					table[i][j] = 0;
				}
			}
		}
	}

	//outputMatch();
	int score1 = count1 < 3 ? 0 : count1 - 2;
	int score2 = count2 < 3 ? 0 : count2 - 2;

	score += (score1*sc[lv1] + score2 * sc[lv2]);

	return 1;
}

int SandTable::elimMatch()
{
	// 清空数据
	for (int i = 0; i < tableH; i++)
	{
		for (int j = 0; j < tableW; j++)
		{
			match[i][j] = 0;
		}
	}
	int index = 0; // 消除块序号
	for (int i = 0; i < tableH; i++)
	{
		for (int j = 0; j < tableW; j++)
		{
			if (type(i, j) == 1)
			{
				if (i >= 2)
				{
					if (table[i][j] == table[i - 1][j] && table[i][j] == table[i - 2][j])
					{
						int indexExisted = 0; // 已经存在序号了吗？
						if (match[i - 1][j] > 0)
							indexExisted = match[i - 1][j];
						else if (match[i - 2][j] > 0)
							indexExisted = match[i - 2][j];
						else if (match[i][j] > 0)
							indexExisted = match[i][j];
						if (indexExisted) // 有旧序号
						{
							match[i][j] = indexExisted;
							match[i - 1][j] = indexExisted;
							match[i - 2][j] = indexExisted;
							//cout << "旧:" << index << " (" << i + 1 << ", " << j + 1 << ")-上" << endl;
						}
						else // 新序号
						{
							index++;
							match[i][j] = index;
							match[i - 1][j] = index;
							match[i - 2][j] = index;
							vector<int> currMatch;
							currMatch.push_back(table[i][j]);
							currMatch.push_back(i - 1);
							currMatch.push_back(j);
							matchList.push_back(currMatch);
							//cout << "新:" << index << " (" << i + 1 << ", " << j + 1 << ")-上" << endl;
						}
					}
					if (j >= 2 && table[i][j] == table[i - 1][j - 1] && table[i][j] == table[i - 2][j - 2])
					{

						int indexExisted = 0; // 已经存在序号了吗？
						if (match[i - 1][j - 1] > 0)
							indexExisted = match[i - 1][j - 1];
						else if (match[i - 2][j - 2] > 0)
							indexExisted = match[i - 2][j - 2];
						else if (match[i][j] > 0)
							indexExisted = match[i][j];
						if (indexExisted) // 有旧序号
						{
							match[i][j] = indexExisted;
							match[i - 1][j - 1] = indexExisted;
							match[i - 2][j - 2] = indexExisted;
							//cout << "旧:" << index << " (" << i + 1 << ", " << j + 1 << ")-左上" << endl;
						}
						else // 新序号
						{
							index++;
							match[i][j] = index;
							match[i - 1][j - 1] = index;
							match[i - 2][j - 2] = index;
							vector<int> currMatch;
							currMatch.push_back(table[i][j]);
							currMatch.push_back(i - 1);
							currMatch.push_back(j - 1);
							matchList.push_back(currMatch);
							//cout << "新:" << index << " (" << i + 1 << ", " << j + 1 << ")-左上" << endl;
						}
					}
					if (j <= tableW - 3 && table[i][j] == table[i - 1][j + 1] && table[i][j] == table[i - 2][j + 2])
					{
						int indexExisted = 0; // 已经存在序号了吗？
						if (match[i - 1][j + 1] > 0)
							indexExisted = match[i - 1][j + 1];
						else if (match[i - 2][j + 2] > 0)
							indexExisted = match[i - 2][j + 2];
						else if (match[i][j] > 0)
							indexExisted = match[i][j];
						if (indexExisted) // 有旧序号
						{
							match[i][j] = indexExisted;
							match[i - 1][j + 1] = indexExisted;
							match[i - 2][j + 2] = indexExisted;
							//cout << "旧:" << index << " (" << i + 1 << ", " << j + 1 << ")-右上" << endl;
						}
						else // 新序号
						{
							index++;
							match[i][j] = index;
							match[i - 1][j + 1] = index;
							match[i - 2][j + 2] = index;
							vector<int> currMatch;
							currMatch.push_back(table[i][j]);
							currMatch.push_back(i - 1);
							currMatch.push_back(j + 1);
							matchList.push_back(currMatch);
							//cout << "新:" << index << " (" << i + 1 << ", " << j + 1 << ")-右上" << endl;
						}
					}
				}
				if (j >= 2 && table[i][j] == table[i][j - 1] && table[i][j] == table[i][j - 2])
				{
					int indexExisted = 0; // 已经存在序号了吗？
					if (match[i][j - 1] > 0)
						indexExisted = match[i][j - 1];
					else if (match[i][j - 2] > 0)
						indexExisted = match[i][j - 2];
					else if (match[i][j] > 0)
						indexExisted = match[i][j];
					if (indexExisted) // 有旧序号
					{
						match[i][j] = indexExisted;
						match[i][j - 1] = indexExisted;
						match[i][j - 2] = indexExisted;
						//cout << "旧:" << index << " (" << i + 1 << ", " << j + 1 << ")-左" << endl;
					}
					else // 新序号
					{
						index++;
						match[i][j] = index;
						match[i][j - 1] = index;
						match[i][j - 2] = index;
						vector<int> currMatch;
						currMatch.push_back(table[i][j]);
						currMatch.push_back(i);
						currMatch.push_back(j - 1);
						matchList.push_back(currMatch);
						//cout << "新:" << index << " (" << i + 1 << ", " << j + 1 << ")-左" << endl;
					}
				}
			}
		}
	}
	// 消石头
	for (int i = 0; i < tableH; i++)
	{
		for (int j = 0; j < tableW; j++)
		{
			if (type(i, j) == -1)
			{
				int isHit = 0;
				if (i > 0 && match[i - 1][j] > 0)
					isHit = 1;
				if (j > 0 && match[i][j - 1] > 0)
					isHit = 1;
				if (i < tableH - 1 && match[i + 1][j] > 0)
					isHit = 1;
				if (j < tableW - 1 && match[i][j + 1] > 0)
					isHit = 1;
				if (isHit)
					table[i][j]++;
			}
		}
	}

	for (int index = 0; index < matchList.size(); index++)
	{
		for (int i = 0; i < tableH; i++)
		{
			for (int j = 0; j < tableW; j++)
			{
				if (i == matchList[index][1] && j == matchList[index][2]) // 消除点
				{
					table[i][j] += 10;
				}
				else if (match[i][j] == index + 1)
				{
					table[i][j] = 0;
				}
			}
		}
	}
	//outputMatch();

	vector<int> count;
	for (int index = 0; index < matchList.size(); index++)
	{
		count.push_back(0);
	}

	for (int i = 0; i < tableH; i++)
	{
		for (int j = 0; j < tableW; j++)
		{
			int maTemp = match[i][j];
			if (maTemp > 0)
				count[maTemp - 1]++;
		}
	}

	for (int i = 0; i < matchList.size(); i++)
	{
		int addNum = count[i] < 3 ? 1 : count[i] - 2;
		int lvMatched = matchList[i][0] / 10;
		score += sc[lvMatched] * addNum;;
	
	}

	matchList.clear();
	return index;

}

void SandTable::drop()
{
	for (int j = 0; j < tableW; j++) // 逐列掉落
	{
		vector<int> v;
		for (int i = tableH - 1; i >= 0; i--) { // 从下至上
			if (type(i, j) == 1)
			{
				v.push_back(table[i][j]);
				table[i][j] = 0;
			}
		}
		int it = 0;
		for (int i = tableH - 1; i >= 0; i--)
		{
			if (table[i][j] == 0 && it < v.size())
			{
				table[i][j] = v[it];
				it++;
			}
		}
	}
};

void SandTable::fill()
{
	// 补充
	while (isLack())
	{
		for (int j = 0; j < tableW; j++)
		{
			if (isLineLack(j)) {
				int i = tableH - 1;
				while (table[i][j] != 0)
					i--;
				table[i][j] = dropList[dropNum % 12];

				dropNum++;

			}
		}
	}
};

int SandTable::canSlim(int x1, int y1, int x2, int y2)
{
	if (type(x1, y1) != 1 || type(x2, y2) != 1 || table[x1][y1] == table[x2][y2])
		return 0;
	else
		return 1;
}
// 直接调用成员函数
void SandTable::reInit()
{
	for (int i = 0; i < tableW; i++)
	{
		for (int j = 0; j < tableH; j++)
		{
			table[i][j] = tableStart[i][j];
			table[i][j] = tableStart[i][j];
		}
	}
	score = 0;
	dropNum = 0;
	stepNum = 44;
}

void SandTable::setTable(int arr[tableW][tableH])
{
	for (int i = 0; i < tableW; i++)
	{
		for (int j = 0; j < tableH; j++)
		{
			tableStart[i][j] = arr[i][j];
			tableStart[i][j] = arr[i][j];
		}
	}
	reInit();
}

void SandTable::setList(int arr[12])
{
	dropList.clear();
	for (int i = 0; i < 12; i++)
		dropList.push_back(arr[i]);

}

int SandTable::swap(int x1, int y1, int x2, int y2)
{
	if (type(x1, y1) != 1|| type(x2, y2) != 1)
		return 0;
	// 交换
	int temp = table[x1][y1];
	table[x1][y1] = table[x2][y2];
	table[x2][y2] = temp;
	//output();

	// 消除1 掉落补充
	elimMatch0(x1, y1, x2, y2);
	drop();
	fill();

	// 消除2
	while (elimMatch())
	{
		drop();
		fill();
	}
	stepNum--;
	//cout << "score = " << score << endl;
	//output();
	return 1;
}

int SandTable::drag(int x, int y)
{
	if (type(x, y) != 1)
		return 0;
	// 拖出

	if (table[x][y] / 10 <= 3)
		stepNum--;
		
	score += sc[table[x][y] / 10];

	table[x][y] = 0;
	// 掉落补充
	drop();
	fill();

	// 消除2
	while (elimMatch())
	{
		drop();
		fill();
	}
	//output();
	return 1;
}

int SandTable::getScore(vector<int> list)
{
	reInit();
	bool notValid = false;
	for (int i = 0; i < list.size() / 4 && stepNum>0; i++)
	{
		bool thisStepNotValid = false;
		if (list[4 * i + 2] > tableW)
			notValid = !drag(list[4 * i], list[4 * i + 1]);
		else
			notValid = !swap(list[4 * i], list[4 * i + 1], list[4 * i + 2], list[4 * i + 3]);
		if (notValid)
			break;
	}
	if (notValid)
		return 0;
	else
		return score;
}
