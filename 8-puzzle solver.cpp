#include <iostream>
#include <stack> //Path
#include <queue> //Frontier
#include <fstream>
#include <string.h>
#include <list>
#include <vector>
#include <math.h>
using namespace std;

int SOLUTION_ALGORITHM;
vector<vector<int> > puzzle;
int solutionCost = 0;
int frontierEntry = 0;
int frontierExit = 0;

enum Solution
{
	BFS,
	ASTAR,
	DNE
};

enum Direction
{
	Downward,
	Right,
	Left,
	Upward
};

typedef struct Point
{
	int i;
	int j;
}Point;

class Node {
public:
	Node* parent;

	vector<vector<int> > map;

	int heuristic;

	Node(vector<vector<int> > p)
	{
		heuristic = 99999;
		map.resize(3);
		for (int i = 0; i < 3; i++)
		{
			map[i].resize(3);
			for (int j = 0; j < 3; j++)
			{
				map[i][j] = p[i][j];
			}
		}

		parent = NULL;
	}
};

struct NodeCompare
{
	bool operator()(Node* n1, Node* n2)
	{
		int val1 = n1->heuristic;
		int val2 = n2->heuristic;
		return val1 > val2;
	}
};

bool operator<(const Node &a, const Node &b)
{
	return a.heuristic < b.heuristic;
}

void printPuzzle(vector<vector<int> > map)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cout << map[i][j];
		}
		cout << endl;
	}
	cout << endl;
}

//Reads puzzle from the input file.
void readInputFile()
{
	ifstream reader;
	reader.open("input.txt");
	if (!reader) {
		cerr << "Dosya okunamadi.";
		exit(1);   // call system to stop
	}
	puzzle.resize(3);
	for (int i = 0; i < 3; i++)
	{
		puzzle[i].resize(3);
		for (int j = 0; j < 3; j++)
		{
			reader >> puzzle[i][j];
		}
	}
	reader.close();
}

//Checks 8-puzzle solvability.
bool isSolvable()
{
	int inversionCounter = 0;
	for (int i = 0; i < 3 - 1; i++)
		for (int j = i + 1; j < 3; j++)
			if (puzzle[j][i] > 0 && puzzle[j][i] > 0 &&
				puzzle[j][i] > puzzle[i][j])
				inversionCounter++;

	return (inversionCounter % 2) ? false : true;
}

//Checks if goal state is reached.
bool isGoal(vector<vector<int> > map)
{
	int goal[3][3] = { {0,1,2},{3,4,5},{6,7,8} };

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (goal[i][j] != map[i][j])
				return false;
		}
	}
	return true;
}

//Checks if the movement is possible.
bool isMoveable(Direction movement,Point p)
{
	if (movement == Downward && p.i < 2)
	{
		return true;
	}
	else if (movement == Upward && p.i > 0)
	{
		return true;
	}
	else if (movement == Left && p.j > 0)
	{
		return true;
	}
	else if (movement == Right && p.j < 2)
	{
		return true;
	}

	return false;
}

//Checks if the state has already been visited.
bool isVisited(list<vector<vector<int> > > l, vector<vector<int> > map)
{
	vector<vector<int> > temp;

	while (!l.empty())
	{
		temp = l.front();
		l.pop_back();
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (temp[i][j] != map[i][j])
					return false;
			}
		}
	}
		
	return true;
}

//Finds blank tile's location.
Point findBlank(vector<vector<int> > map)
{
	Point p;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (map[i][j] == 0)
			{
				p.i = i;
				p.j = j;
				return p;
			}
		}
	}
}

//Calculates heuristic for A*.
int manhattanDist(vector<vector<int> > map)
{
	int manhattan = 0;

	int goal[3][3] = { {0,1,2},{3,4,5},{6,7,8} };


	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (map[i][j] != 0)
			{
				for (int k = 0; k < 3; k++)
				{
					for (int l = 0; l < 3; l++)
					{
						if (map[i][j] == goal[k][l])
						{
							manhattan += abs(i - k) + abs(j - l);
						}
					}
				}
			}
		}
	}

	return manhattan;
}

//Moves blank tile downwards.
vector<vector<int> > moveDown(vector<vector<int> > map, Point blank)
{
	int temp = map[(blank.i) + 1][blank.j];

	map[(blank.i) + 1][blank.j]= map[blank.i][blank.j];

	map[blank.i][blank.j] = temp;

	return map;
}

//Moves blank tile upwards.
vector<vector<int> > moveUp(vector<vector<int> > map, Point blank)
{
	int temp = map[(blank.i) - 1][blank.j];

	map[(blank.i) - 1][blank.j] = map[blank.i][blank.j];

	map[blank.i][blank.j] = temp;

	return map;
}

//Moves blank tile to the left.
vector<vector<int> > moveLeft(vector<vector<int> > map, Point blank)
{
	int temp = map[blank.i][(blank.j) - 1];

	map[blank.i][(blank.j) - 1] = map[blank.i][blank.j];

	map[blank.i][blank.j] = temp;

	return map;
}

//Moves blank tile to the Right.
vector<vector<int> > moveRight(vector<vector<int> > map, Point blank)
{
	int temp = map[blank.i][(blank.j) + 1];

	map[blank.i][(blank.j) + 1] = map[blank.i][blank.j];

	map[blank.i][blank.j] = temp;

	return map;
}

void Solution(Node *n)
{
	if (n == NULL)
		return;

	stack<Node*> path;

	path.push(n);

	while (n->parent != NULL)
	{
		path.push(n->parent);
		n = n->parent;
	}

	while (!path.empty())
	{
		Node* next = path.top();
		printPuzzle(next->map);
		path.pop();
		solutionCost++;
	}
	cout << "Cozum Maliyeti: " << solutionCost-1 << endl;
	cout << "Frontier\'e Giren Dugum Sayisi: " << frontierEntry << endl;
	cout << "Frontier\'den Cikan (Expanded) Dugum Sayisi: " << frontierExit << endl;
}

Node* A_Star()
{
	
	list<vector<vector<int> > > visitedList;

	std::priority_queue<Node*, std::vector<Node*>, NodeCompare> frontier;

	Node* start = new Node(puzzle);

	frontier.push(start);
	frontierEntry++;

	visitedList.push_back(start->map);

	int g = 0;
	while (!frontier.empty())
	{
		g++;
		Node* current = frontier.top();

		frontier.pop();
		frontierExit++;

		if (isGoal(current->map))
			return current;

		Point blankLocation = findBlank(current->map);

		//Up
		if (isMoveable(Upward, blankLocation))
		{
			Node* next = new Node(moveUp(current->map, blankLocation));
			if (!isVisited(visitedList, next->map))
			{
				next->parent = current;
				next->heuristic = manhattanDist(next->map) + g;
				frontier.push(next);
				frontierEntry++;
				visitedList.push_back(next->map);
			}
		}

		//Left
		if (isMoveable(Left, blankLocation))
		{
			Node* next = new Node(moveLeft(current->map, blankLocation));
			if (!isVisited(visitedList, next->map))
			{
				next->parent = current;
				next->heuristic = manhattanDist(next->map) + g;
				frontier.push(next);
				frontierEntry++;
				visitedList.push_back(next->map);
			}
		}

		//Down
		if (isMoveable(Downward, blankLocation))
		{
			Node* next = new Node(moveDown(current->map, blankLocation));
			if (!isVisited(visitedList, next->map))
			{
				next->parent = current;
				next->heuristic = manhattanDist(next->map) + g;
				frontier.push(next);
				frontierEntry++;
				visitedList.push_back(next->map);
			}
		}
		//Right
		if (isMoveable(Right, blankLocation))
		{
			Node* next = new Node(moveRight(current->map, blankLocation));
			if (!isVisited(visitedList, next->map))
			{
				next->parent = current;
				next->heuristic = manhattanDist(next->map) + g;
				frontier.push(next);
				frontierEntry++;
				visitedList.push_back(next->map);
			}
		}

	}

}

Node* BreadthFirstSearch()
{
	list<vector<vector<int> > > visitedList;

	queue<Node*> frontier;

	Node* start = new Node(puzzle);

	frontier.push(start);
	frontierEntry++;

	visitedList.push_back(start->map);

	while (!frontier.empty())
	{
		Node* current = frontier.front();

		frontier.pop();
		frontierExit++;

		if (isGoal(current->map))
			return current;

		Point blankLocation = findBlank(current->map);

		//Up
		if (isMoveable(Upward, blankLocation))
		{
			Node* next = new Node(moveUp(current->map, blankLocation));
			if (!isVisited(visitedList, next->map))
			{
				next->parent = current;
				frontier.push(next);
				frontierEntry++;
				visitedList.push_back(next->map);
			}
		}

		//Left
		if (isMoveable(Left, blankLocation))
		{
			Node* next = new Node(moveLeft(current->map, blankLocation));
			if (!isVisited(visitedList, next->map))
			{
				next->parent = current;
				frontier.push(next);
				frontierEntry++;
				visitedList.push_back(next->map);
			}
		}

		//Down
		if (isMoveable(Downward, blankLocation))
		{
			Node* next=new Node(moveDown(current->map, blankLocation));
			if (!isVisited(visitedList, next->map))
			{
				next->parent = current;
				frontier.push(next);
				frontierEntry++;
				visitedList.push_back(next->map);
			}
		}
		//Right
		if (isMoveable(Right, blankLocation))
		{
			Node* next = new Node(moveRight(current->map, blankLocation));
			if (!isVisited(visitedList, next->map))
			{
				next->parent = current;
				frontier.push(next);
				frontierEntry++;
				visitedList.push_back(next->map);
			}
		}
	}
}

int main(int argc, char *argv[])
{
	#pragma region Initialize program safely
	// Command line parameter is missing. Avoids exception.
	if (argc < 2)
		return 0;

	// Algorithm selection.
	if (strcmp("astar", argv[1]) == 0)
	{
		SOLUTION_ALGORITHM = ASTAR;
	}
	else if (strcmp("bfs", argv[1]) == 0)
	{
		SOLUTION_ALGORITHM = BFS;
	}
		

	// Algorithm parameter is wrong.
	if (SOLUTION_ALGORITHM == DNE)
		return 0;
	#pragma endregion
	
	

	readInputFile();


	if (!isSolvable())
	{
		cout <<endl<< "Given 8-puzzle is unsolvable.."<<endl;
		return 0;
	}
	if (SOLUTION_ALGORITHM == BFS) 
	{
		Solution(BreadthFirstSearch());
	}
	else if (SOLUTION_ALGORITHM == ASTAR)
	{
		Solution(A_Star());
	}
	
	return 0;
}
