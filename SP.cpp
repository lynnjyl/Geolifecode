/*deal with the result of StayPoints.cpp. get all the stay points in one file.*/

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <sstream>

using namespace std;

struct pot
{
	double lat;
	double lngt;
};


void getposition(string str, pot &p)
{
	cout << str << endl;
	int pos1, pos2;
	pos1 = str.find_first_of(" ", 0);
	p.lat = atof(str.substr(0, pos1).c_str());
	pos2 = str.find_first_of(" ", pos1+1);
	p.lngt = atof(str.substr(pos1, pos2 - pos1).c_str());
	cout << p.lat << " " << p.lngt << endl;
}


void readfile(string filename)
{
	ifstream ifs(filename.c_str());
	ofstream ofs("../StayPoints/SP.txt", ios::app);
	string str;
	int pos;
	pot p;
	
	while(!ifs.eof())
	{
		getline(ifs, str, '\n');
		pos = str.find_first_of("t", 0);
		if(pos == string::npos)
		{
			getposition(str, p);
			ofs.precision(10);
			ofs << p.lat << " " << p.lngt << endl;
		}
	}
	// cout << "ok here " << endl;
}

int main(int argc, char * argv[])
{
	string fname = argv[1];
	string filename;
	int i;
	stringstream index;
	for(i = 0; i < 10; i++)
	{
		index << i;
		filename = fname + "/00" + index.str() + ".txt";
		cout << "filename is " << filename << endl;
		readfile(filename);
		index.str("");
		// cout << "ok here " << endl;
	}
	
	for(i = 10; i < 100; i++)
	{
		index << i;
		filename = fname + "/0" + index.str() + ".txt";
		readfile(filename);
		index.str("");
	}
	
	for(i = 100; i < 182; i++)
	{
		index << i;
		filename = fname + "/" + index.str() + ".txt";
		readfile(filename);
		index.str("");
	}
	
	
	return 0;
}
