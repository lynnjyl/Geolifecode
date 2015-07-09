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

double deg2rad(double deg) 
{
	return (deg * pi / 180);
}

double rad2deg(double rad) 
{
	return (rad * 180 / pi);
}


double distance(double lat1, double lon1, double lat2, double lon2) {
	double theta, dist;
	theta = lon1 - lon2;
	dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
	dist = acos(dist);
	dist = rad2deg(dist);
	dist = dist * 60 * 1.1515;
	// switch(unit) {
		// case 'M':
			// break;
		// case 'K':
			// dist = dist * 1.609344;
			// break;
		// case 'N':
			// dist = dist * 0.8684;
			// break;
	// }
	dist = dist * 1609.344;
	return (dist);
	//new line in vim using 'o'
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

void getinformation(string s, GPSlog &l)
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

void readGPSlogs(string directory, string filename)
{
	string name = directory + "/" + filename;
	ifstream ifs(name.c_str());
	//ignore the useless lines
	int commlines = 6;
	string str;
	while(commlines > 0)
	{
		getline(ifs, str, '\n');
		commlines--;
	}
	
	// string s1, s2;
	// getline(ifs, s1, '\n');
	// getline(ifs, s2, '\n');
	// ifs.close();
	
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
	{
		getinformation(s[i], log[i]);
	}
	
	double dis;
	double sec;
	for(i = 0; i < num - 1; i++)
	{
		dis = distance(log[i].Latitude, log[i].Longitude, log[i+1].Latitude, log[i+1].Longitude);
		sec = (log[i+1].Time - log[i].Time)*24*3600;
		printf("%d and %d distance: %lf, time: %lf\n", i, i+1, dis, sec);
	
	}
	// GPSlog log1, log2;
	// getinformation(s1, log1);
	// getinformation(s2, log2);
	
	// printf("%lf, %lf, %lf\n", log1.Latitude, log1.Longitude, log1.Time);
	// printf("%lf, %lf, %lf\n", log2.Latitude, log2.Longitude, log2.Time);
	
	// double dis = distance(log1.Latitude, log1.Longitude, log2.Latitude, log2.Longitude);
	// printf("distance is %lf\n", dis);
	
	/*
	string first, last, temp;
	temp = " ";
	getline(ifs, first, '\n');
	while(!ifs.eof())
	{
		last = temp;
		getline(ifs, temp, '\n');
	}
	// printf("%s\n", first.c_str());
	// printf("%s\n", last.c_str());
	string date1, date2;
	date1 = getdate(first);
	date2 = getdate(last);
	printf("%s, %s\n", date1.c_str(), date2.c_str());
	if( !strcmp( date1.c_str(), date2.c_str() ) == 0)
		printf("different in %s\n", name.c_str());
	*/
}

int main(int argc, char * argv[])
{
	string Number = argv[1];	
	string directory = "../Geolife Trajectories 1.3/Data/" + Number + "/Trajectory";
	GetAllFilenames(directory);
	printf("there is %d files in this folder.\n", filenames.size());
	for(int i = 0; i < 1/*filenames.size()*/; i++)
	{
		printf("%s\n", filenames[i].c_str());
		readGPSlogs(directory, filenames[i]);
	}
}
