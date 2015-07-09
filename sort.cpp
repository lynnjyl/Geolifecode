/*	Sort the stay points file according to the altitude*/

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

bool myfunc(string s1, string s2)
{
	int p1, p2;
	p1 = s1.find_first_of(" ");
	p2 = s2.find_first_of(" ");
	double d1, d2;
	d1 = atof(s1.substr(0, p1).c_str());
	d2 = atof(s2.substr(0, p2).c_str());
	
	return (d1 < d2);
}


int main(int argc, char * argv[])
{
	ifstream ifs(argv[1]);
	ofstream ofs(argv[2]);
	int num = atoi(argv[3]);
	string data[num];
//	string data[7971];
	for(int i = 0; i < num; i++)
	{
		getline(ifs, data[i], '\n');
	}
	sort(data, data+num, myfunc);
	ofs.precision(10);
	
	for(int i = 0; i < num; i++)
	{
		ofs << endl << data[i];
	}
	return  0;

}
