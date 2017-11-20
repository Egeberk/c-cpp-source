#include <iostream>
using namespace std;
int main()
{
	for(int i=0;i<n/2;i++)
	{
		fake=arr[i];
		arr[i]=arr[n-i-1];
		arr[n-i-1]=fake;
	}
	
}

