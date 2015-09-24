// chess.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "board.h"
#include <iostream>
#include <vector>

using namespace std;

const int boardsize = 8;
board map[boardsize][boardsize];
void populateboard();
void populateplayer();
void populateenamy();
piecetype toenum(int a);
void remove(location a, vector<location> list);
void removebcrossover(location start, vector<location> list);
void removeccrossover(location start, vector<location> list);
void removetreson(vector<location> it);
void addextra(location start, vector<location> list);
void addextrae(location start, vector<location> list);
void checkupgrade(location start, piecetype it);
void checkupgradee(location start, piecetype it);
void displayupgradeoption();
void enamymove();
void player2move();
location getenamylocation();
vector<location> getallenamylocation();
vector<location> filter(vector<location> it);
void displayoptions(vector<location> list);
int getposiblemove(int max);
bool haswon();
bool haslost();
void drawboard();
location getselection();
int main()
{
	populateboard();
	drawboard();
	while (!haswon() || !haslost())
	{
		location a;
		vector<location> list;
		do
		{
			do
				a = getselection();
			while (! map[a.x][a.y].isplayerpice() && map[a.x][a.y].getcurrenttype() == none);
			list = filter(map[a.x][a.y].getposiblemoves(a));// what if 0
			if (map[a.x][a.y].getcurrenttype() == pawn)// check rule breaker
				addextra(a, list);
			if (map[a.x][a.y].getcurrenttype() == bishop)
				removebcrossover(a, list);
			if (map[a.x][a.y].getcurrenttype() == castle)
				removeccrossover(a, list);
			if (map[a.x][a.y].getcurrenttype() == queen)
			{
				removeccrossover(a, list);
				removebcrossover(a, list);
			}
			list = filter(list);
			removetreson(list);
		}
		while (list.size() == 0);
		displayoptions(list);
		cout << "please select a move from the list"<<endl;
		int move = getposiblemove(list.size());// player move
		map[list[move].x][list[move].y].settype(map[a.x][a.y].getcurrenttype());
		map[list[move].x][list[move].y].setplayerpiece(false);
		map[a.x][a.y].settype(none);
		map[a.x][a.y].setplayerpiece(true);
		checkupgrade(a , map[a.x][a.y].getcurrenttype());
		drawboard();
		if (!haswon())// enemy cant move with no pieces 
			player2move();
		//enamymove();//could cause error
	}
	return 0;
}
void removebcrossover(location start, vector<location> list)
{//remove location that are blocked
	location a = start;
	bool blocked = false;
	while (a.x < 8 && a.y < 8)
	{
		if (blocked)
		{// set item to -1,-1 for filter()
			remove(a, list);
		}
		if (map[a.x][a.y].getcurrenttype() != none)
		{
			blocked = true;
		}
		a.x = a.x + 1;
		a.y = a.y + 1;
	}
	a = start;
	blocked = false;
	while (a.x > 0 && a.y > 0)
	{
		if (blocked)
		{
			remove(a, list);
		}
		if (map[a.x][a.y].getcurrenttype() != none)
		{
			blocked = true;
		}
		a.x = a.x - 1;
		a.y = a.y - 1;
	}
	a = start;
	blocked = false;
	while (a.y > 0 && a.x < 8)
	{
		if (blocked)
		{
			remove(a, list);
		}
		if (map[a.x][a.y].getcurrenttype() != none)
		{
			blocked = true;
		}
		a.y = a.y - 1;
		a.x = a.x + 1;
	}
	a = start;
	blocked = false;
	while (a.y < 8 && a.x > 0)
	{
		if (blocked)
		{
			remove(a, list);
		}
		if (map[a.x][a.y].getcurrenttype() != none)
		{
			blocked = true;
		}
		a.y = a.y + 1;
		a.x = a.x - 1;
	}
}
void removeccrossover(location start, vector<location> list)
{//remove location that are blocked
	location a = start;
	bool blocked = false;
	while (a.x < 8)
	{
		if (blocked)
		{// set item to -1,-1 for filter()
			remove(a, list);
		}
		if (map[a.x][a.y].getcurrenttype() != none)
		{
			blocked = true;
		}
		a.x = a.x + 1;
	}
	a = start;
	blocked = false;
	while (a.x > 0)
	{
		if (blocked)
		{
			remove(a, list);
		}
		if (map[a.x][a.y].getcurrenttype() != none)
		{
			blocked = true;
		}
		a.x = a.x - 1;
	}
	a = start;
	blocked = false;
	while (a.y > 0)
	{
		if (blocked)
		{
			remove(a, list);
		}
		if (map[a.x][a.y].getcurrenttype() != none)
		{
			blocked = true;
		}
		a.y = a.y - 1;
	}
	a = start;
	blocked = false;
	while (a.y < 8)
	{
		if (blocked)
		{
			remove(a, list);
		}
		if (map[a.x][a.y].getcurrenttype() != none)
		{
			blocked = true;
		}
		a.y = a.y + 1;
	}
}
void populateboard()
{
	populateenamy();
	populateplayer();
}
void populateplayer()
{
	map[boardsize - 1][0].settype(castle);
	map[boardsize - 1][boardsize-1].settype(castle);
	map[boardsize - 1][1].settype(horse);
	map[boardsize - 1][boardsize - 2].settype(horse);
	map[boardsize - 1][2].settype(bishop);
	map[boardsize - 1][boardsize - 3].settype(bishop);
	map[boardsize - 1][3].settype(queen);
	map[boardsize - 1][boardsize - 4].settype(king);
	for (int i = 0; i != boardsize; i++)
	{
		map[boardsize - 2][i].settype(pawn);
		map[boardsize - 1][i].setplayerpiece(true);
		map[boardsize - 2][i].setplayerpiece(true);
	}
}
void populateenamy()
{
	map[0][0].settype(castle);
	map[0][boardsize- 1].settype(castle);
	map[0][1].settype(horse);
	map[0][boardsize - 2].settype(horse);
	map[0][2].settype(bishop);
	map[0][boardsize - 3].settype(bishop);
	map[0][3].settype(queen);
	map[0][boardsize - 4].settype(king);
	for (int i = 0; i != 8; i++)
	{
		map[1][i].settype(pawn);
	}
}
void removetreson(vector<location> it)
{
	for (int i = 0; i < it.size(); i++)
	{
		if (map[it[i].x][it[i].y].isplayerpice())
			it.erase(it.begin()+i);
	}
}
void addextra(location start, vector<location> list)
{
	if (start.x != 0 && (start.y != 0 || start.y != boardsize - 1))// check bounds
	{
		if (!map[start.x - 1][start.y + 1].isplayerpice() && map[start.x - 1][start.y + 1].getcurrenttype() != none)
		{
			location a;
			a.x = start.x - 1;
			a.y = start.y + 1;
			list.push_back(a);
		}
		if (!map[start.x - 1][start.y - 1].isplayerpice() && map[start.x - 1][start.y - 1]. getcurrenttype() != none)
		{
			location a;
			a.x = start.x - 1;
			a.y = start.y - 1;
			list.push_back(a);
		}
	}
}
void addextrae(location start, vector<location> list)
{
	if (start.x != 0 && (start.y != 0 || start.y != boardsize - 1))// check bounds
	{
		if (!map[start.x + 1][start.y + 1].isplayerpice() && map[start.x + 1][start.y + 1].getcurrenttype() != none)
		{
			location a;
			a.x = start.x + 1;
			a.y = start.y + 1;
			list.push_back(a);
		}
		if (!map[start.x + 1][start.y - 1].isplayerpice() && map[start.x + 1][start.y - 1].getcurrenttype() != none)
		{
			location a;
			a.x = start.x + 1;
			a.y = start.y - 1;
			list.push_back(a);
		}
	}
}
void checkupgrade(location start, piecetype it)
{
	if (start.x == 0 && it == pawn)//
	{
		displayupgradeoption();
		int result = getposiblemove(3);
		map[start.x][start.y].settype(toenum(result - 1));
	}
}
void checkupgradee(location start, piecetype it)
{
	if (start.x == boardsize-1 && it == pawn)//
	{
		displayupgradeoption();
		int result = getposiblemove(3);
 		map[start.x][start.y].settype(toenum(result - 1)); 
	}
}
void displayupgradeoption()
{
	cout << "1. castle" << endl;
	cout << "2. jester" << endl;
	cout << "3. horse" << endl;
	cout << "4. queen" << endl;
}
void player2move()
{
	location a;
	vector<location> list;
	do
	{
		do
			a = getselection();
		while (map[a.x][a.y].isplayerpice() && map[a.x][a.y].getcurrenttype() == none);
		list = filter(map[a.x][a.y].getposiblemoves(a));// what if 0
		if (map[a.x][a.y].getcurrenttype() == pawn)// check rule breaker
			addextrae(a, list);
		removetreson(list);
	} while (list.size() == 0);
	displayoptions(list);
	int move = getposiblemove(list.size());// player move
	map[list[move].x][list[move].y].settype(map[a.x][a.y].getcurrenttype());
	map[a.x][a.y].settype(none);
	checkupgradee(a, map[a.x][a.y].getcurrenttype());
	drawboard();
}
void enamymove()
{
	location enamypice;
	vector<location> list;
	do
	{
		enamypice = getenamylocation();
		list = filter(map[enamypice.x][enamypice.y].getposiblemoves(enamypice));// what if 0
		if (map[enamypice.x][enamypice.y].getcurrenttype() == pawn)
			addextrae(enamypice, list);
	} while (list.size() == 0);
	int moves = rand() % list.size();
	map[list[moves].x][list[moves].y].settype(map[enamypice.x][enamypice.y].getcurrenttype());
	map[list[moves].x][list[moves].y].setplayerpiece(false);
	map[enamypice.x][enamypice.y].settype(none);
	map[enamypice.x][enamypice.y].setplayerpiece(false);
}
location getenamylocation()
{
	vector<location> list = getallenamylocation();
	int number = rand() % list.size();
	return list[number];
}
vector<location> getallenamylocation()
{
	vector<location> list;
	for (int i = 0; i != boardsize; i++)
	{
		for (int j = 0; j != boardsize; j++)
		{
			if (map[i][j].isplayerpice() && map[i][j].getcurrenttype() != none)
			{
				location a;
				a.x = i;
				a.y = j;
				list.push_back(a);
			}
		}
	}
	return list;
}
void displayoptions(vector<location> list)
{
	for (int i = 0; i != list.size(); i++)
	{
		cout << i << " " << list[i].x << " " << list[i].y << endl;
	}
}
int getposiblemove(int max)
{
	int a;
	cin >> a;
	while (a < 0 || a >= max)
	{
		cout << "enter again " << endl;
		cin >> a;
	}
	return a;
}
bool haswon()
{//all enemy pieces gone
	bool won = true;
	for (int i = 0; i != boardsize; i++)
	{
		for (int j = 0; j != boardsize; j++)
		{
			if (map[i][j].getcurrenttype() != none && !map[i][j].isplayerpice())
				won = false;
		}
	}
	return won;
}
bool haslost()
{//all player pieces gone
	bool lost = true;
	for (int i = 0; i != boardsize; i++)
	{
		for (int j = 0; j != boardsize; j++)
		{
			if (map[i][j].getcurrenttype() != none && map[i][j].isplayerpice())
				lost = false;
		}
	}
	return lost;
}
void drawboard()
{
	bool onice = true;
	for (int i = 0; i != boardsize;i++)
	{	
		if (onice)
		{
			cout << "0 ";
			for (int j = 0; j != boardsize; j++)
			{
				cout << " " << j << " ";
			}
			cout << endl;
			onice = false;
		}
		cout << i << " ";
		for (int j = 0; j != boardsize; j++)
		{
			cout << "|";
			map[i][j].draw();
			cout << "|";
		}
		cout << endl;
	}
}
location getselection()
{
	location a;
	cout << "please enter the x cord " << endl;
	cin >> a.x;
	cout << "please enter the y cord " << endl;
	cin >> a.y;
	return a;
}
vector<location> filter(vector<location> it)
{
	vector<location> list;
	for (int i = 0; i != it.size(); i++)
	{
		if (!(it[i].x >= boardsize || it[i].y >= boardsize || it[i].x < 0 || it[i].y < 0))
			list.push_back(it[i]);
	}
	return list;
}
piecetype toenum(int a)
{
	switch (a)
	{
	case 1:
		return pawn;
	case 2:
		return castle;
	case 3:
		return horse;
	case 4:
		return bishop;
	case 5:
		return queen;
	case 6:
		return king;
	default:
		return none;
	}
}
void remove(location a, vector<location> list)
{
	for (int i = 0; i != list.size(); i++)
	{
		if (list[i].x == a.x && list[i].y == a.y)
			list[i].x = list[i].y = -1;
	}
}