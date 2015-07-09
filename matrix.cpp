#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

struct Point
{
	double alt;
	double lngt;
	int clusterId;
};

struct Element
{
	int userId;
	int clusterId;
};

void printpoint(Point p)
{
	
	cout << p.alt << " " << p.lngt << " " << p.clusterId << endl;
}

bool pointequal(Point p1, Point p2)
{
	if(p1.alt == p2.alt)
	{
		if(p1.lngt == p2.lngt)
			return true;
		else
			return false;
	}
	else
		return false;
}

void readclusters(string &input, vector <Point> &clusters)
{
	ifstream ifs(input.c_str(), ios::in);
	int cluId = 0;
	string str;
	Point p;
	int pos1, pos2;
	int t;
	while(!ifs.eof())
	{
		p.clusterId = cluId;
		getline(ifs, str, '\n');
		pos2 =  0;
		pos1 = str.find_first_of(" ", pos2);
	//	cout << pos1 << endl;
		while(pos1!=string::npos)
		{
			p.alt = atof(str.substr(pos2, pos1 - pos2).c_str());
			//cout << p.alt << endl;
			//cin >> t;
			pos2 = str.find_first_of(" ", ++pos1);
			//cout << pos2 << endl;
			p.lngt = atof(str.substr(pos1, pos2 - pos1).c_str());	
			clusters.push_back(p);
			pos1 = str.find_first_of(" ", ++pos2);
		}
		cluId++;
	}


}

int GetClusterId(Point p, vector <Point> &clusters)
{
	if(p.alt < 39.4 || p.alt > 41.05 || p.lngt < 115.4166 || p.lngt > 117.5)
		return -1;
	else
	{
		int size = clusters.size();
		int i = 0; 
		while (i < size)
		{
			if(pointequal(p, clusters[i]))
				return clusters[i].clusterId;
			i++;
		}
		return -1;
	}
}


void creatematrix(string input, vector <Point> &clusters, int uid, string output, string output2)
{
	ifstream ifs(input.c_str());
	ofstream ofs2(output2.c_str(), ios::app);
	string str;
	int found;
	int pos1, pos2;
	int prel, nowl;
	prel = -1;
	Point p;
	vector <Element> matrix; 
	Element ele;
	while (!ifs.eof())
	{
		getline(ifs, str, '\n');
		found = str.find_first_of("t", 0);
		//cout << str << endl;
		if(found == string::npos)
		{
		//	cout << "it's right, prel = " << prel << endl;
		//	int a;
			//cin >> a;
			pos1 = str.find_first_of(" ", 0);
			p.alt = atof(str.substr(0, pos1).c_str());
			pos2 = str.find_first_of(" ", ++pos1);
			p.lngt = atof(str.substr(pos1, pos2 - pos1).c_str());	
			p.clusterId = GetClusterId(p, clusters);
			//cout << "clusterId is " << p.clusterId << endl;
			nowl = p.clusterId;
			if(p.clusterId != -1)
			{
				ele.userId = uid;
				ele.clusterId = p.clusterId;
				matrix.push_back(ele);
				// cout << uid << " " << p.clusterId << endl;

				if(prel != -1)
				{
					ofs2 << prel << " " << nowl << endl;
					cout << "ok" << endl;
					// prel = nowl;	
				}
			}
			prel = nowl;
		}			
		else
		{
			prel = -1;
			// cout << "----------" << endl;
		}
	}
	ifs.close();
	ofstream ofs(output.c_str(), ios::app);
	for(int i = 0; i < matrix.size(); i++)
		ofs << matrix[i].userId << " " << matrix[i].clusterId << endl; 
}

void readusertraj(string directory, vector <Point> &clusters, string output, string output2)
{
	int i;
	int id;
	string name;
	stringstream index;
	for(i = 1; i < 10; i++)
	{
		index << i;
		name = directory + "/00" + index.str() + ".txt";
		creatematrix(name, clusters, i, output, output2);		
		index.str("");
	}

	for(i = 10; i < 100; i++)
	{
		index << i;
		name = directory + "/0" + index.str() + ".txt";
		creatematrix(name, clusters, i, output, output2);
		index.str("");
	}
	for(i = 100; i < 182; i++)
	{
		index << i;
		name = directory + "/" + index.str() + ".txt";
		creatematrix(name, clusters, i, output, output2);
		index.str("");
	}
}
int main(int argc, char* argv[])
{
	
	string inputclusters = argv[1];
	string directory = argv[2];
	string output = argv[3];
	string output2 = argv[4];
	vector  <Point> clusters;
	readclusters(inputclusters, clusters);		
	readusertraj(directory, clusters, output, output2);
	

	return 0;
}
