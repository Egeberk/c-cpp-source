#include <iostream>
#include <time.h>
#include <fstream>
#include <string>
using namespace std;
int main()
{
	int i;
	string a,b;
	time_t mytime;
	ofstream ege;
	ege.open("lol.txt");

	
	mytime=time(NULL);
	ege<<ctime(&mytime);
	ege.close();
	ifstream ege1;
	ege1.open("lol.txt");
	ege1>>a>>b;
	cout<<"DAY= "<<a<<endl
	<<"Month= "<<b;
	return 0;
	
}
