/*Egeberk Özberk 
Visual Studio 2013
12.4.2016*/
/*egeberk@gmail.com*/
#include <iostream>
using namespace std;
#define variable int
#define stack_size 100
#define Newline cout << endl;
typedef struct STACK
{
	variable datas[stack_size];
	int top = -1;
}stack;

// Main functions for stack.
bool is_Full(stack s)
{
	return (s.top == stack_size) ? true : false;
}
bool is_Empty(stack s)
{
	return (s.top == -1) ? true : false;
}
void push(stack *s, variable data)
{
	(s->top)++;
	s->datas[(s->top)]=data;
}
variable pop(stack *s)
{
	variable res = s->datas[(s->top)];
	(s->top)--;
	return res;
}

// Helper functions for stack.
void stack_copy(stack *dest, stack src)
{
	stack temp;
	while (!is_Empty(src))
		push(&temp, pop(&src));
	while (!is_Empty(temp))
		push(dest, pop(&temp));
}
void print_stack(stack s)
{
	while (!is_Empty(s))
	{
		cout << pop(&s) << " ";
	}
	Newline
}

int main()
{
	stack s1, s2;
	system("pause");
	return 0;
}
