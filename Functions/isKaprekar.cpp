#include <math.h>

bool isKaprekar(int n)
{
	short int digit=log10(n)+1;
	
	return (((int)(pow(n,2))%((int)pow(10,digit))+((int)(pow(n,2))/((int)pow(10,digit)))))==n;
}

