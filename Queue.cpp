/*Egeberk Özberk 
Visual Studio 2013
24.2.2016*/
/*egeberk@gmail.com*/
#include <iostream>
#include <fstream>
using namespace std;
#define max 100
typedef double elemtype;
class queue
{
private:
	elemtype items[max];
	int rear;
	int front;
public:
	queue()
	{
		rear = -1;
		front = 0;
	}
	bool isEmpty()
	{
		if (rear < front)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool isFull()
	{
		if (rear == max)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	void enqueue( elemtype item)
	{
		if (isFull())
			cout <<endl<< "Full.";

		else if (rear == max - 1)
		{
			items[0] = item;
			rear = 0;
		}
		else
		{
			rear++;
			items[rear] = item;
		}
	}
	elemtype dequeue()
	{
		elemtype item;

		if (isEmpty())  
		{
			cout <<endl<< "Empty..";
			exit(0);
		}
		else 
		{
			item = items[front];
			front++;
		}
		return item;
	}
	void print()
	{
		while (!isEmpty())
		{
			cout << " " << dequeue();
		}
	}
};

int main()
{
	queue q;
	q.print();
	system("pause");

	
	return 0;

}


