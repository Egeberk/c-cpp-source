
void prime(int a)
{
	int prime=1, k=2;
	
	
	while (prime==1 && k<a)
	{
		if (a%k==0)
		{
			prime=0;
		}
		k++;
		
	}
	
}
