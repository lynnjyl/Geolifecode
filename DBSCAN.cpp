#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <iomanip>
#include <sstream>

using namespace std;
#define pi 3.14159265358979323846

struct Point
{
	double altitude;
	double longitude;
	int ClusterID;
	int noise;
};

void PrintPoint(Point p)
{
	cout << setprecision(10) << p.altitude << " " << p.longitude << endl;
}
/**************************************************************************************************/
/********************************calculate distance between two points*****************************/
/**************************************************************************************************/
double deg2rad(double deg) 
{
	return (deg * pi / 180);
}
double rad2deg(double rad) 
{
	return (rad * 180 / pi);
}
double distance(double &lat1, double &lon1, double &lat2, double &lon2) 
{
	double theta, dist;
	
	theta = lon1 - lon2;
	dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
	
	dist = acos(dist);
	dist = rad2deg(dist);
	dist = dist * 60 * 1.1515;
	dist = dist * 1609.344;
	return (dist);
}
/**************************************************************************************************/
/**************************************************************************************************/

vector <int> GetRegion(vector <Point> points, Point p, double eps, int index)
{
	vector <int> region;
	double dist;
	// PrintPoint(p);
	// cout << "region：" << endl;
	for(int i = 0; i < points.size(); i++)
	{
		dist = distance(points[i].altitude, points[i].longitude, p.altitude, p.longitude);
		// if(dist <= eps && i != index)
		// {
			// region.push_back(i);
			// PrintPoint(points[i]);
		// }
		// cout << index << " and " << i << " distance is " << dist << endl;
		if(dist <= eps)
		{
			region.push_back(i);
			// PrintPoint(points[i]);
		}
	}
	return region;
}

bool ExpandCluster(vector <Point> &points, Point &p, int index, int clusterId, double eps, int minPts)
{
	// cout << "In ExpandCluster function, index is " << index << endl;
	vector <int> seeds = GetRegion(points, p, eps, index);
	if(seeds.size() < minPts)
	{
		p.noise = 1;
		return false;
	}
	else
	{
		// cout << "seeds.size() = " << seeds.size() << endl;
		p.ClusterID = clusterId;
		for(int i = 0 ; i < seeds.size(); i++)
		{
			// cout << seeds[i] << " ";
			points[seeds[i]].ClusterID = clusterId;
		}
		// cout << endl;
		while(seeds.size() > 0)
		{
			if(seeds[0] != index)
			{
				vector <int> result = GetRegion(points, points[seeds[0]], eps, seeds[0]);
				if(result.size() >= minPts)
				{
					for(int i = 0; i < result.size(); i++)
					{
						if(points[result[i]].ClusterID == 0 || points[result[i]].noise == 1)
						{
							if(points[result[i]].ClusterID == 0)
								seeds.push_back(result[i]);
							points[result[i]].ClusterID = clusterId;
						}
					}
				}
			}
			seeds.erase(seeds.begin());
		}
		return true;
	}
}


vector <vector <Point> > GetClusters(vector <Point> &points, double eps, int minPts)
{
	
	vector <vector <Point> > clusters;
	int clusterId = 1;
	int i;
	if(points.size() == 0)
		return clusters;
	for(i = 0; i < points.size(); i++)
	{
		// Point p = points[i];
		
		if(points[i].ClusterID == 0)
		{
			if(ExpandCluster(points, points[i], i, clusterId, eps, minPts))
				clusterId++;
		}
	}
	if(clusterId >= 1)
	{
		cout << "there are " << clusterId << "clusters" << endl;
		clusters.resize(clusterId);
		for(i = 0; i < points.size(); i++)
		{
			if(points[i].ClusterID > 0)
			{
				clusters[points[i].ClusterID].push_back(points[i]);
			}
		}
	}
	return clusters;
}


/**************************************************************************************************/
void ReadPoints(string input, vector<Point> &points)
{
	ifstream ifs(input.c_str());
	int pos;
	Point p = {0,0,0,-1};
	string str;
	while(!ifs.eof())
	{
		getline(ifs, str,'\n');
		pos = str.find_first_of(" ");
		p.altitude = atof(str.substr(0, pos).c_str());
		p.longitude = atof(str.substr(pos+1, str.length()-pos).c_str());
		points.push_back(p);
		//PrintPoint(p);
	}
}
/**************************************************************************************************/
void WriteClusters(string output, vector <vector <Point> > clusters)
{
	
	ofstream ofs;
	ofs.precision(10);
	//of.precision(10);
	//of.open("clusters.txt");
	// double avgalt = 0;
	// double avglngt = 0;
	ofs.open(output.c_str());
	for(int i = 0; i < clusters.size(); i++)
	{
		// stringstream index;
		// index << i;
		//name = output + index.str() + ".txt";
		// cout << name << endl;
		
		for(int j = 0; j < clusters[i].size(); j++)
		{
			// avgalt += clusters[i][j].altitude;
			// avglngt += clusters[i][j].longitude;
			ofs << clusters[i][j].altitude << " " << clusters[i][j].longitude << " ";
			//of << clusters[i][j].altitude << " " << clusters[i][j].longitude <<" ";
		}
		ofs<<endl;
		// avgalt /= clusters[i].size();
		// avglngt /= clusters[i].size();
		
		// ofs << avgalt << " " << avglngt <<endl;
		// avgalt = avglngt = 0;
		
		//index.str("");
	}
	ofs.close();
}

int main(int argc, char* argv[])
{
	string input = argv[1];
	string eps_str = argv[2];
	string minpts_str = argv[3];
	double eps = atof(argv[2]);
	int minPts = atoi(argv[3]);
	// string output = argv[4];
	
	string output = input + "_" + eps_str + "_" + minpts_str + ".txt";
	input = input + ".txt";
	vector <Point> points;
	
	
	ReadPoints(input, points);
	vector <vector <Point> > clusters = GetClusters(points, eps, minPts);
	// cout << points.size() << endl;
	WriteClusters(output, clusters);
	
	
	
	
	return 0;
}