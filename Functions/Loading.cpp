#include <iostream>
#include <windows.h>
using namespace std;
int main()
{
 system("color 0e");
 cout<<"\n\n\n\t\t\t\tPlease wait while loading\n\n";
 char a=177, b=219;
 cout<<"\t\t\t\t";
 for (int i=0;i<=20;i++)
 cout<<a;
 cout<<"\r";
 cout<<"\t\t\t\t";
 for (int i=0;i<=20;i++)
 {
  cout<<b;
  Sleep(350);
 }
 system("cls");
 cout<<endl<<endl<<"Complated...";
 return 0;
}
