#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <windows.h>
#include <regex>

#define tableW 8
#define tableH 8



using namespace std;

class SandTable
{
public:
	SandTable();
	void output();

	void outputMatch();
	void outputMatchList();

	// 基础函数，间接调用
	int type(int x, int y);
	int isLack();
	int isLineLack(int j);

	// 中间函数，间接调用
	/* 交换: 交换 - 消除* - 下落补充 - ( 消除 - 下落补充 )
	拖出: 拖出 - 下落补充 - ( 消除 - 下落补充 )  */
	int elimMatch0(int x1, int y1, int x2, int y2);
	int elimMatch();
	void drop();
	void fill();
	int canSlim(int x1, int y1, int x2, int y2);

	// 直接调用成员函数
	void reInit();
	void setTable(int arr[tableW][tableH]);
	void setList(int arr[12]);
	int swap(int x1, int y1, int x2, int y2);
	int drag(int x, int y);

	int getScore(vector<int> list);

private:
	int tableStart[tableW][tableH];
	int table[tableW][tableH];
	int match[tableW][tableH];
	int score;
	int stepNum = 44;
	//int stepScore;
	time_t dropNum;
	vector<int> dropList;

	vector<vector<int>> matchList;

};