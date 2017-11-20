/*Egeberk Özberk 
HackerRank Compiler
11.3.2017*/
/*egeberk@gmail.com*/

/* Here is my solution for Available Workshops problem in HackerRank
see at https://www.hackerrank.com/challenges/attending-workshops/problem
implemented solution uses a greedy algorithm. */

#include<bits/stdc++.h>
using namespace std;
struct Workshop
{
	int start_time;
	int duration;
	int end_time;
};
struct Available_Workshops
{
	int n; 
	Workshop *workshops;
};

Available_Workshops* initialize(int start_time[], int duration[], int n)
{
	
	Available_Workshops* temp = new Available_Workshops();
	temp->n = n;
	temp->workshops = new Workshop[n];
	for (int i = 0; i<n; i++) 
	{
		
		temp->workshops[i].start_time = start_time[i];
		temp->workshops[i].duration = duration[i];
		temp->workshops[i].end_time = temp->workshops[i].start_time + temp->workshops[i].duration;
	}
	return temp;
}

bool operator<(Workshop const & a, Workshop const & b)
{
	return a.end_time < b.end_time;
}

int CalculateMaxWorkshops(Available_Workshops* ptr)
{
	sort(ptr->workshops, ptr->workshops+ptr->n);

	int end_time = 0,hours=0;
	for (int i=0; i<ptr->n; ++i) 
	{
		if (ptr->workshops[i].start_time >= end_time) 
		{
			hours++;
			end_time = ptr->workshops[i].end_time;
		}
	}
	return hours;
}
int main(int argc, char *argv[]) {
    int n; // number of workshops
    cin >> n;
    // create arrays of unknown size n
    int* start_time = new int[n];
    int* duration = new int[n];

    for(int i=0; i < n; i++){
        cin >> start_time[i];
    }
    for(int i = 0; i < n; i++){
        cin >> duration[i];
    }

    Available_Workshops * ptr;
    ptr = initialize(start_time,duration, n);
    cout << CalculateMaxWorkshops(ptr) << endl;
    return 0;
}

