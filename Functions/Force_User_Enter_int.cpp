#include <stdio.h>
int empty_input() 
{
    while (getchar()!='\n');
    return 1;
}
int main()
{
	int age;
	char dummy;
	do
	{
		printf("Enter your age with the correct format: ");
	}while((scanf("%d%c",&age,&dummy)!=2 || dummy!='\n')&& empty_input()); 
	return 0;
}
