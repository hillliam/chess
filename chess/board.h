#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

enum piecetype
{
	none,
	pawn,
	castle,
	horse,
	bishop,
	queen,
	king
};
struct location
{
	int x;
	int y;
};
class board
{
public:

	board();
	void settype(piecetype);
	piecetype getcurrenttype();
	bool isplayerpice();
	void setplayerpiece(bool value);
	vector<location> getposiblemoves(location here);
	void draw();
	~board();
private:
	bool firstmove = true;
	piecetype current;
	bool playerpice; // if false and not none is enemy piece
	vector<location> posiblemovespawn(location here);
	vector<location> posiblemoveshorse(location here);
	vector<location> posiblemovescastle(location here);
	vector<location> posiblesmovesbishop(location here);
	vector<location> posiblemovesqueen(location here);
	vector<location> posiblemovesking(location here);
	vector<location> combine(vector<location> a, vector<location> b, vector<location> c, vector<location> d);
	vector<location> posiblemovesfpawn(location here);
	vector<location> filter(vector<location> it);
};

