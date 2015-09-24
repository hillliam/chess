#include "board.h"

board::board()
{
}

void board::settype(piecetype a)
{
	current = a;
}

piecetype board::getcurrenttype()
{
	return current;
}

bool board::isplayerpice()
{
	return playerpice;
}

void board::setplayerpiece(bool value)
{
	playerpice = value;
}

vector<location> board::getposiblemoves(location here)
{
		switch (current)
		{
		case none:
			return vector<location>();
			break;
		case castle:
			return posiblemovescastle(here);
			break;
		case bishop:
			return posiblesmovesbishop(here);
			break;
		case queen:
			return posiblemovesqueen(here);
			break;
		case king:
			return posiblemovesking(here);
			break;
		case horse:
			return posiblemoveshorse(here);
			break;
		default:
			if (firstmove)
			{
				return posiblemovesfpawn(here);
				firstmove = false;
			}   
			else
				return posiblemovespawn(here);
		}
}

void board::draw()
{
	switch (current)
	{
	case pawn:
		cout << "P";
		break;
	case castle:
		cout << "C";
		break;
	case horse:
		cout << "H";
		break;
	case bishop:
		cout << "B";
		break;
	case queen:
		cout << "Q";
		break;
	case king:
		cout << "K";
		break;
	default:
		cout << " ";
	}
}


board::~board()
{
}

vector<location> board::posiblemovespawn(location here)
{
	if (playerpice)
	{
		vector<location> list;
		--here.x;
		list.push_back(here);
		return list;
	}
	else
	{
		vector<location> list;
		++here.x;
		list.push_back(here);
		return list;
	}
}
vector<location> board::posiblemoveshorse(location here)
{//horse
	vector<location> list;
	location a = here;
	location b = here;
	a.x = b.x = a.x - 3;
	a.y= a.y - 1;
	b.y = b.y + 1;
	list.push_back(a);//-3,-1
	list.push_back(b);//-3,1
	a = b = here;
	a.x = b.x = a.x + 3;
	a.y = a.y - 1;
	b.y = b.y + 1;
	list.push_back(a);//3,1
	list.push_back(b);//3,-1
	a = b = here;
	a.y = b.y = a.y + 3;
	a.x = a.x - 1;
	b.x = b.x - 1;
	list.push_back(a);//-1,3
	list.push_back(b);//1,3
	a = b = here;
	a.y = b.y = a.y - 3;
	a.x = a.x + 1;
	b.x = b.x - 1;
	list.push_back(a);//1,-3
	list.push_back(b );//-1,-3
	return list;
}
vector<location> board::posiblemovesfpawn(location here)
{
	if (playerpice)
	{
		vector<location> list = posiblemovespawn(here);
		here.x = here.x - 2;
		list.push_back(here);
		return list;
	}
	else
	{
		vector<location> list = posiblemovespawn(here);
		here.x = here.x + 2;
		list.push_back(here);
		return list;
	}

}
vector<location> board::posiblemovescastle(location here)
{
	vector<location> list;
	for (int i = 0; i < 8; i++)
	{
		location a;
		a.x = i;
		a.y = here.y;
		if (a.x != here.x && a.y != here.y)
			list.push_back(a);
		a.x = here.x;
		a.y = i;
		if (a.x != here.x && a.y != here.y)
			list.push_back(a);
	}
	return list;
}
vector<location> board::posiblesmovesbishop(location here)
{
	vector<location> list;
	int y;
	location a = here;
	while (a.x < 8 && a.y < 8)//fixme: both x and y have to be > 8
 	{//+1,+1
		a.x = a.x + 1;
		a.y = a.y + 1;
		list.push_back(a);
	}
	a = here;
	while (a.x < 8 && a.y > 0)
	{//+1,-1
		a.x = a.x + 1;
		a.y = a.y - 1;
		list.push_back(a);
	}
	a = here;
	while (a.x >= 0 && a.y > 0)
	{//-1,-1
		a.x = a.x - 1;
		a.y = a.y - 1;
		list.push_back(a);
	}
	a = here;
	while (a.x > 0 && a.y < 8)
	{//-1,+1
		a.x = a.x - 1;
		a.y = a.y + 1;
		list.push_back(a);
	} 
	return list;
}
vector<location> board::posiblemovesqueen(location here)
{
	vector<location> list = posiblesmovesbishop(here);
	vector<location> list2 = posiblemovescastle(here);
	vector<location> list3 = posiblemovesking(here);
	vector<location> list4 = posiblemoveshorse(here);
	return combine(list, list2, list3, list4);
}
vector<location> board::posiblemovesking(location here)
{
	vector<location> list;
	location a = here;
	--a.x;
	list.push_back(a);//-1,0
	--a.y;
	list.push_back(a);//-1,-1
	++a.x;
	list.push_back(a);//0,-1
	a.y = a.y + 2;
	list.push_back(a);//0,1
	++a.x;
	list.push_back(a);//1,1
	--a.y;
	list.push_back(a);//1,0
	--a.y;
	list.push_back(a);//1,-1
	a.y = a.y + 2;
	a.x = a.x - 2;
	list.push_back(a);//-1,1
	return list;
}
vector<location> board::combine(vector<location> a, vector<location> b , vector<location> c, vector<location> d)
{
	vector<location> list = a;
	for (int i = 0; i < b.size();i++)
	{
		list.push_back(b[i]);
	}
	for (int i = 0; i < c.size(); i++)
	{
		list.push_back(c[i]);
	}
	for (int i = 0; i < d.size(); i++)
	{
		list.push_back(d[i]);
	}
	//remove duplicates
	for (int i = 0; i < list.size(); i++)
	{
		for (int j = 0; j < list.size(); j++)
		{
			if (list[i].x == list[j].x && list[i].y == list[j].y)
			{
				list.erase(list.begin() + j);
				continue;
			}
 		}
	}
	return list;
}
