/*	Get all stay points in the dataset
/*	input parameter: 
/*		[1] Number	: file name in ../Geolife Trajectories 1.3/Data/
/*		[2]	D		: distance threshold
/*		[3] T		: time threshold
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/sysinfo.h>
#include <dirent.h>
#include <math.h>

using namespace std;

#define pi 3.14159265358979323846

vector <string> filenames;
struct GPSlog
{
	double Latitude;
	double Longitude;
	double Time;
};
struct StayPoints
{
	double Lat;
	double Lngt;
	double arvT;
	double levT;
};

double deg2rad(double deg) 
{
	return (deg * pi / 180);
}

double rad2deg(double rad) 
{
	return (rad * 180 / pi);
}

double timebetween(GPSlog &l1, GPSlog &l2)
{
	return (l2.Time - l1.Time)*24*3600;
}

double distance(double &lat1, double &lon1, double &lat2, double &lon2) 
// double distance(GPSlog log1, GPSlog log2)
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

void GetAllFilenames(string directory)
{
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (directory.c_str())) != NULL) {
	/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
			// printf ("%s\n", ent->d_name);
			if( !strcmp( ent->d_name, "." ) == 0 && !strcmp( ent->d_name, ".." ) == 0)
				filenames.push_back(ent->d_name);
		}
		closedir (dir);
	} else {
	/* could not open directory */
	perror ("");
	// return EXIT_FAILURE;
	// return EXIT_FAILURE;
	}	
}

string getdate(string s)
{
	int pos1, pos2;
	pos1 = pos2 = 0;
	for(int i = 0; i < 4; i++)
	{
		pos1 = s.find_first_of(',', pos1);
		pos1++;
	}
	pos2 = s.find_first_of('.', pos1);
	// printf("%d, %d\n", pos1, pos2);
	return s.substr(pos1, pos2 - pos1);
}

void getinformation(string &s, GPSlog &l)
{
	string latitude, longitude, time;
	int pos1, pos2;
	pos1 = pos2 = 0;
	pos1 = s.find_first_of(',', pos1);
	latitude = s.substr(0, pos1);
	pos1++;
	pos2 = s.find_first_of(',', pos1);
	longitude = s.substr(pos1, pos2 - pos1);
	
	pos1 = s.find_first_of(',', ++pos2);
	pos2 = s.find_first_of(',', ++pos1);
	pos1 = s.find_first_of(',', ++pos2);

	time = s.substr(pos2, pos1 - pos2);
	
	l.Latitude = atof(latitude.c_str());
	l.Longitude = atof(longitude.c_str());
	l.Time = atof(time.c_str());	
}


void readGPSlogs(string &directory, string &filename, double &D, double &T, string &outputname)
{
	string name = directory + "/" + filename;
	ifstream ifs(name.c_str());
	ofstream ofs(outputname.c_str(), ios::app);
	ofs << endl << filename ;
	//ignore the useless lines
	int commlines = 6;
	string str;
	while(commlines > 0)
	{
		getline(ifs, str, '\n');
		commlines--;
	}
		
	vector <string> s;
	while(!ifs.eof())
	{
		getline(ifs, str, '\n');
		s.push_back(str);
	}
	s.pop_back();
	
	int num = s.size();
	int i;
	GPSlog *log;
	log = new GPSlog[num];
	
	for(i = 0; i < num; i++)
		getinformation(s[i], log[i]);
	
	int startindex = 1;
	int m = 0;
	StayPoints sp = {0,0,0,0};
	while(m < num)
	{
		/*find the index startindex that satisfy the time threshold*/
		while((timebetween(log[m], log[startindex]) < T) && startindex < num)
			startindex++;
		/*check if all the points satisfy the distance threshold*/
		i = startindex;
		while((distance(log[i].Latitude, log[i].Longitude, log[m].Latitude, log[m].Longitude) <= D) && i > m )
			i--;
		
		if(i == m + 1 && (timebetween(log[m], log[startindex]) >= T))
		{
			/*if all the points satisfy the distance,increase startindex to find the first index that un-satisfy*/
			startindex++;
			while(startindex < num &&(distance(log[startindex].Latitude, log[startindex].Longitude, log[m].Latitude, log[m].Longitude) <= D))
				startindex++;
			startindex--;
			
			/*create stay point*/
			for(int k = m; k <= startindex; k++)
			{
				sp.Lat += log[k].Latitude;
				sp.Lngt+= log[k].Longitude;
			}
			sp.arvT = log[m].Time;
			sp.levT = log[startindex].Time;
			sp.Lat = sp.Lat/(startindex-m+1);
			sp.Lngt = sp.Lngt/(startindex-m+1);
			
			ofs.precision(10);
			ofs << endl << sp.Lat << " " << sp.Lngt << " " ;
			ofs.precision(16);
			ofs << sp.arvT << " " << sp.levT;
			
			
			printf("log %d - log %d can be seen as a stay point\n", m, startindex);
			printf("time is %f\n", log[startindex].Time);
			
			m = startindex + 1;
			startindex = m + 1;
			// printf("ok\n");
		}
		else
		{
			m++;
		}
	}
	
}

int main(int argc, char * argv[])
{
	string Number = argv[1];
	double D = atof(argv[2]);
	double T = atof(argv[3]);
	string output = argv[4];
	string directory = "../Geolife Trajectories 1.3/Data/" + Number + "/Trajectory";
	GetAllFilenames(directory);
	printf("there is %d files in this folder.\n", filenames.size());
	string outputname = output+"/" + Number + ".txt";
	for(int i = 0; i < filenames.size(); i++)
	{
		printf("%s\n", filenames[i].c_str());
		readGPSlogs(directory, filenames[i], D, T, outputname);
	}
}
