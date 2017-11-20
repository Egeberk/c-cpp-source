#include <iostream>
#include <windows.h>
using namespace std;
int main()
{
	SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM) 2);
	Sleep(2000);
	return 0;
}

