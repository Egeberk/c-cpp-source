#include <iostream>
#include <stdlib.h> // Free() { delete[] * } in C++
using namespace std;
#define variable int // So that the variable type could be changed.
#define Newline cout<<endl;

typedef struct LINKEDLIST // Node.
{
	variable data;
	struct LINKEDLIST *next;
}linkedlist;

// Generates new node.
linkedlist* generate_node(variable data)
{
	linkedlist *new_one = new linkedlist[1];
	if (new_one == NULL)
	{
		cout << "Memory failure."; Newline
			exit(0);
	}
	new_one->data = data;
	new_one->next = NULL;
	return new_one;
}

// Prints the whole list.
void traverse(linkedlist *head) 
{
	Newline
	linkedlist *step = head;
	while (step != NULL)
	{
		cout << step->data << " -> ";
		step = step->next;
	}
	cout << "END";Newline
}

// Inserts new node to the beginning.
linkedlist* insert_to_beginning(linkedlist *head, variable data)
{
	linkedlist *new_one = generate_node(data);
	if (head == NULL)
		head = new_one;
	else
	{
		new_one->next = head;
		head = new_one;
	}
	return head;	
}

// Inserts new node to the end.
linkedlist* insert_to_end(linkedlist *head, variable data)
{
	linkedlist *new_one = generate_node(data);
	if (head == NULL)
		head = new_one;
	else
	{
		linkedlist *step = head;
		while (step->next != NULL)
		{
			step = step->next;
		}
		step->next = new_one;
	}
	return head;
}

// Inserts new to its ordered position.
linkedlist* insert_sorted(linkedlist *head, variable data)
{
	linkedlist *prev_step=NULL, *step=head;
	linkedlist *new_one = generate_node(data);
	if (head == NULL)
		return generate_node(data);

	while (step != NULL)
	{
		if (step->data > data)
			break;
		prev_step = step;
		step = step->next;
	}
	if (prev_step == NULL)
	{
		new_one->next = head;
		head = new_one;
	}
	else
	{
		new_one->next = step;
		prev_step->next = new_one;
	}
	return head;
}

// Removes first node.
linkedlist* delete_first(linkedlist *head)
{
	if (head == NULL)
		return head;
	//linkedlist *temp = head; 
	head = head->next; // } For better memory management you can use commented statements.
	//free(temp);
	return head;
}

// Removes last node.
linkedlist* delete_last(linkedlist *head)
{
	if (head == NULL)
		return head;
	if (head->next == NULL)
	{
		return NULL;
	}
	linkedlist *step=head, *prev_step=NULL;
	while (step->next != NULL)
	{
		prev_step = step;
		step = step->next;
	}
	prev_step->next = NULL;
	free(step);
	return head;
}

// Removes the wanted node.
linkedlist* delete_keyNode(linkedlist *head, variable data)
{
	if (head == NULL)
		return head;
	linkedlist *step = head, *prev_step = NULL;
	while (step != NULL)
	{
		if (step->data == data)
			break;
		prev_step = step;
		step = step->next;
	}
	if (prev_step == NULL)
	{
		head = head->next;
		free(step);
	}
	else if (step != NULL)
	{
		prev_step->next = step->next;
		free(step);
	}
	return head;
}

// Destroys the list.
linkedlist* clear(linkedlist *head)
{
	linkedlist *destroyed;
	while (head != NULL)
	{
		destroyed = head;
		head = head->next;
		free(destroyed);
	}
	return head;
}
int main()
{
	linkedlist *head = NULL;
	head = insert_sorted(head, 5);
	head = insert_sorted(head, 1);
	head = insert_sorted(head, 7);
	head = insert_sorted(head, 10);
	head = delete_keyNode(head, 5);
	traverse(head);
	system("pause");
	return 0;
}
