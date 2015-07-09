/*select the stay points belong to Beijing*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/sysinfo.h>
// #include <dirent.h>

using namespace std;
void checksp(vector <string> &sp, string str)
{
	int pos;
	double alt, lngt;
	pos = str.find_first_of(" ");
	alt = atof(str.substr(0, pos).c_str());
	lngt = atof(str.substr(pos+1, str.length()-pos).c_str());
	if(alt <= 41.05 && alt >= 39.4)
	{
		if(lngt <= 117.5 && lngt >= 115.4166)
			sp.push_back(str);
	}
}
int main(int argc, char * argv[])
{
	ifstream ifs(argv[1]);
	ofstream ofs(argv[2]);
	string str;
	vector <string> sp;
	while(!ifs.eof())
	{
		getline(ifs, str, '\n');
		checksp(sp, str);
	}
	cout << sp.size() << endl;
	
	for(int i = 0; i < sp.size(); i++)
	{
		ofs << sp[i] << endl;
	}
	
}
