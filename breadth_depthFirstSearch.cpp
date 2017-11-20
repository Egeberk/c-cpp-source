/*Egeberk Özberk 
DEV C++ platform 
14.8.2016*/
/*egeberk@gmail.com*/

/* ADDITIONAL INFO
Implemented algorithms are :
-Breadth-first search
-Depth-first search
-Random walk
This program expects an input .txt file with 3 different mazes with sizes 10x10 12x12 15x15
You can omit the input file and modify this program to make it accept a hard-coded maze.
*/
#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <cstdlib> // rand()
#include <time.h> // srand(time())
//[dikey][yatay]
using namespace std;
#define SIZE1 10 // Maze1
#define SIZE2 12 // Maze2
#define SIZE3 15 // Maze3
#define newline cout<<endl
// CLASS POINT
class Point{
public:
	int x;
	int y;
	Point()
	{
		x = 0;
		y = 0;
	}
	Point(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};
// CLASS NODE
class Node{
public:
	Point coordinate;
	Node *parent;
	Node()
	{
		Point p(0, 0);
		coordinate = p;
		parent = NULL;
	}
	Node(int x, int y)
	{
		Point p(x, y);
		coordinate = p;
		parent = NULL;
	}
};
// CLONE ARRAY
void clone_array(int **dest, int **src,int N)
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			dest[i][j] = src[i][j];
}

// READ MAZE (INIT MAZES)
void readMaze(int **m1, int **m2, int **m3, char *argv[])
{
		ifstream myfile1(argv[1]);
	for (int i = 0; i < SIZE1; i++)
		for (int j = 0; j < SIZE1; j++)
			myfile1 >> m1[i][j];
	myfile1.close();

	ifstream myfile2(argv[2]);
	for (int i = 0; i < SIZE2; i++)
		for (int j = 0; j < SIZE2; j++)
			myfile2 >> m2[i][j];
	myfile2.close();

	ifstream myfile3(argv[3]);
	for (int i = 0; i < SIZE3; i++)
		for (int j = 0; j < SIZE3; j++)
			myfile3 >> m3[i][j];
	myfile3.close();
	
}

// PRINT MAZE
void printMaze(int **arr, int N)
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			cout << arr[i][j] << " ";
		newline;
	}	
}

// CHECK IF SPOT IS SAFE TO MOVE
bool is_safe(int x, int y, int N)
{
	return (((x >= 0) && (y >= 0)) && ((x < N) && (y < N))) ? true : false;
}

// BREADTH FIRST SEARCH
int BFS(int **maze,int N)
{
	// 1 FOR SUCCESS -1 FOR FAILURE FINDING THE PATH
	// Check if destination is reachable.
	if (maze[N - 1][N - 1] == 1)
		return -1;
	// INIT VISITED
	bool **visited = new bool*[N];
	for (int i = 0; i < N; i++)
		visited[i] = new bool[N];
	for (int i = 0; i < N;i++)
		for (int j = 0; j < N; j++)
			visited[i][j] = false;

	queue<Node*> q;
	visited[0][0] = true; // We are starting on this point.
	Node *start=new Node;
	start->parent = NULL;
	start->coordinate.x = 0; 
	start->coordinate.y = 0;
	maze[0][0] = 9; // completely unnecessary :P
	q.push(start);
	while (!q.empty())
	{
		Node *current = q.front();
		Point p = current->coordinate;
		q.pop();
		// GOAL POINT
		if ((p.x == N - 1) && (p.y == N - 1))
		{
			stack<Point> path;
			printf("Path for BST: "); // PRINT PATH
			while (current->parent != NULL)
			{
				p.x = current->coordinate.x; p.y = current->coordinate.y;
				path.push(p);
				maze[current->coordinate.x][current->coordinate.y] = 9;
				current = current->parent;
			}
			while (!path.empty())
			{
				p = path.top();
				cout << "([" << p.x << "]" << "[" << p.y << "]) ";
				path.pop();
			}
			newline;
			return 1;
		}
		// SOUTH
		if (is_safe(p.x + 1, p.y,N))
		{
			if (!maze[p.x + 1][p.y])
			{
				if (visited[p.x + 1][p.y] == false)
				{
					Node *neighbor = new Node;
					neighbor->coordinate.x = p.x + 1; neighbor->coordinate.y = p.y;
					neighbor->parent = current;
					q.push(neighbor);
					visited[p.x + 1][p.y] = true;
				}
			}
		}
		// EAST
		if (is_safe(p.x, p.y - 1,N))
		{
			if (!maze[p.x][p.y - 1])
			{
				if (visited[p.x][p.y - 1] == false)
				{
					Node *neighbor = new Node;
					neighbor->coordinate.x = p.x; neighbor->coordinate.y = p.y - 1;
					neighbor->parent = current;
					q.push(neighbor);
					visited[p.x][p.y - 1] = true;
				}
			}
		}
		// WEST
		if (is_safe(p.x, p.y + 1,N))
		{
			if (!maze[p.x][p.y + 1])
			{
				if (visited[p.x][p.y + 1] == false)
				{
					Node *neighbor = new Node;
					neighbor->coordinate.x = p.x; neighbor->coordinate.y = p.y + 1;
					neighbor->parent = current;
					q.push(neighbor);
					visited[p.x][p.y + 1] = true;
				}
			}
		}
		// NORTH
		if (is_safe(p.x - 1, p.y,N))
		{
			if (!maze[p.x - 1][p.y])
			{
				if (visited[p.x - 1][p.y] == false)
				{
					Node *neighbor = new Node;
					neighbor->coordinate.x = p.x - 1; neighbor->coordinate.y = p.y;
					neighbor->parent = current;
					q.push(neighbor);
					visited[p.x - 1][p.y] = true;
				}
			}
		}
		// WEST
		if (is_safe(p.x, p.y + 1,N))
		{
			if (!maze[p.x][p.y + 1])
			{
				if (visited[p.x][p.y + 1] == false)
				{
					Node *neighbor = new Node;
					neighbor->coordinate.x = p.x; neighbor->coordinate.y = p.y + 1;
					neighbor->parent = current;
					q.push(neighbor);
					visited[p.x][p.y + 1] = true;
				}
			}
		}


	}
	// THERE IS STILL NO PATH
		return -1;
}

// CHECK IF RW FAILED
bool is_stuck(int **maze, bool **visited, int N, Point p)
{
	if (is_safe(p.x + 1, p.y, N))
		if (!maze[p.x + 1][p.y])
			if (visited[p.x + 1][p.y] == false)
				return false;

	if (is_safe(p.x - 1, p.y, N))
		if (!maze[p.x - 1][p.y])
			if (visited[p.x - 1][p.y] == false)
				return false;
	
	if (is_safe(p.x, p.y-1, N))
		if (!maze[p.x][p.y-1])
			if (visited[p.x][p.y-1] == false)
				return false;

	if (is_safe(p.x, p.y + 1, N))
		if (!maze[p.x][p.y + 1])
			if (visited[p.x][p.y + 1] == false)
				return false;
	// Nowhere to go..
	return true;

}

// RANDOM WALK
int RW(int **maze,int N)
{
	// 1 FOR SUCCESS -1 FOR FAILURE FINDING THE PATH
	// Check if destination is reachable.
	if (maze[N - 1][N - 1] == 1)
		return -1;
	// INIT VISITED
	bool **visited = new bool*[N];
	for (int i = 0; i < N; i++)
		visited[i] = new bool[N];
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			visited[i][j] = false;
	}

	Point labRat(0,0);
	visited[labRat.x][labRat.y] = true;
	cout << "([" << labRat.x << "]" << "[" << labRat.y << "]) ";
	maze[labRat.x][labRat.y] = 9;
	srand(time(NULL));
	while (maze[N - 1][N - 1] != 9)
	{
		// STOP IF RW FAILED
		if (is_stuck(maze, visited, N, labRat)){
			newline;
			return -1;
		}

		// RANDOM DIRECTION
		int rnd = rand() % 4;
		if (rnd == 0)
		{
			if (is_safe(labRat.x + 1, labRat.y, N))
			{
				if (!maze[labRat.x + 1][labRat.y])
				{
					if (visited[labRat.x + 1][labRat.y] == false)
					{
						maze[labRat.x + 1][labRat.y] = 9;
						visited[labRat.x + 1][labRat.y] = true;
						labRat.x++;
						cout << "([" << labRat.x << "]" << "[" << labRat.y << "]) ";
					}
				}
			}
		}
		else if (rnd == 1)
		{
			if (is_safe(labRat.x - 1, labRat.y, N))
			{
				if (!maze[labRat.x - 1][labRat.y])
				{
					if (visited[labRat.x - 1][labRat.y] == false)
					{
						maze[labRat.x - 1][labRat.y] = 9;
						visited[labRat.x - 1][labRat.y] = true;
						labRat.x--;
						cout << "([" << labRat.x << "]" << "[" << labRat.y << "]) ";
					}
				}
			}
		}
		else if (rnd == 2)
		{
			if (is_safe(labRat.x, labRat.y + 1, N))
			{
				if (!maze[labRat.x][labRat.y + 1])
				{
					if (visited[labRat.x][labRat.y + 1] == false)
					{
						maze[labRat.x][labRat.y + 1] = 9;
						visited[labRat.x][labRat.y + 1] = true;
						labRat.y++;
						cout << "([" << labRat.x << "]" << "[" << labRat.y << "]) ";
					}
				}
			}
		}
		else if (rnd == 3)
		{
			if (is_safe(labRat.x, labRat.y - 1, N))
			{
				if (!maze[labRat.x][labRat.y - 1])
				{
					if (visited[labRat.x][labRat.y - 1] == false)
					{
						maze[labRat.x][labRat.y - 1] = 9;
						visited[labRat.x][labRat.y - 1] = true;
						labRat.y--;
						cout << "([" << labRat.x << "]" << "[" << labRat.y << "]) ";
					}
				}
			}
		}
		// END RANDOM DIRECTION
	}

	// Success
	return 1;
}

// DEPTH FIRST SEARCH
int DFS(int **maze, int N)
{
	// 1 FOR SUCCESS -1 FOR FAILURE FINDING THE PATH
	// Check if destination is reachable.
	if (maze[N - 1][N - 1] == 1)
		return -1;
	// INIT VISITED
	bool **visited = new bool*[N];
	for (int i = 0; i < N; i++)
		visited[i] = new bool[N];
	for (int i = 0; i < N; i++)
	for (int j = 0; j < N; j++)
		visited[i][j] = false;

	stack<Node*> q;
	visited[0][0] = true; // We are starting on this point.
	Node *start = new Node;
	start->parent = NULL;
	start->coordinate.x = 0;
	start->coordinate.y = 0;
	maze[0][0] = 9; // completely unnecessary :P
	q.push(start);
	bool does_have_neighbors = true;
	while (!q.empty())
	{
		Node *current = q.top();
		Point p = current->coordinate;
		if (!does_have_neighbors)
			q.pop();
		// GOAL POINT
		if ((p.x == N - 1) && (p.y == N - 1))
		{
			stack<Point> path;
			printf("Path for DST: "); // PRINT PATH
			while (current->parent != NULL)
			{
				p.x = current->coordinate.x; p.y = current->coordinate.y;
				path.push(p);
				maze[current->coordinate.x][current->coordinate.y] = 9;
				current = current->parent;
			}
			while (!path.empty())
			{
				p = path.top();
				cout << "([" << p.x << "]" << "[" << p.y << "]) ";
				path.pop();
			}
			newline;
			return 1;
		}
		does_have_neighbors = false;
		// SOUTH
		if (is_safe(p.x + 1, p.y, N))
		{
			if (!maze[p.x + 1][p.y])
			{
				if (visited[p.x + 1][p.y] == false)
				{
					Node *neighbor = new Node;
					neighbor->coordinate.x = p.x + 1; neighbor->coordinate.y = p.y;
					neighbor->parent = current;
					q.push(neighbor);
					visited[p.x + 1][p.y] = true;
					does_have_neighbors = true;
				}
			}
		}
		// EAST
		if (is_safe(p.x, p.y - 1, N))
		{
			if (!maze[p.x][p.y - 1])
			{
				if (visited[p.x][p.y - 1] == false)
				{
					Node *neighbor = new Node;
					neighbor->coordinate.x = p.x; neighbor->coordinate.y = p.y - 1;
					neighbor->parent = current;
					q.push(neighbor);
					visited[p.x][p.y - 1] = true;
					does_have_neighbors = true;
				}
			}
		}
		// WEST
		if (is_safe(p.x, p.y + 1, N))
		{
			if (!maze[p.x][p.y + 1])
			{
				if (visited[p.x][p.y + 1] == false)
				{
					Node *neighbor = new Node;
					neighbor->coordinate.x = p.x; neighbor->coordinate.y = p.y + 1;
					neighbor->parent = current;
					q.push(neighbor);
					visited[p.x][p.y + 1] = true;
					does_have_neighbors = true;
				}
			}
		}
		// NORTH
		if (is_safe(p.x - 1, p.y, N))
		{
			if (!maze[p.x - 1][p.y])
			{
				if (visited[p.x - 1][p.y] == false)
				{
					Node *neighbor = new Node;
					neighbor->coordinate.x = p.x - 1; neighbor->coordinate.y = p.y;
					neighbor->parent = current;
					q.push(neighbor);
					visited[p.x - 1][p.y] = true;
					does_have_neighbors = true;
				}
			}
		}
		// WEST
		if (is_safe(p.x, p.y + 1, N))
		{
			if (!maze[p.x][p.y + 1])
			{
				if (visited[p.x][p.y + 1] == false)
				{
					Node *neighbor = new Node;
					neighbor->coordinate.x = p.x; neighbor->coordinate.y = p.y + 1;
					neighbor->parent = current;
					q.push(neighbor);
					visited[p.x][p.y + 1] = true;
					does_have_neighbors = true;
				}
			}
		}

	}
	// THERE IS STILL NO PATH
	return -1;
}

// DRIVER PROGRAM
int main(int argc,char **argv)
{
	// ORIGINAL MAZES
	int **maze1, **maze2, **maze3;
	maze1 = new int*[SIZE1];
	for (int i = 0; i < SIZE1; i++)
		maze1[i] = new int[SIZE1];
	maze2 = new int*[SIZE2];
	for (int i = 0; i < SIZE2; i++)
		maze2[i] = new int[SIZE2];
	maze3 = new int*[SIZE3];
	for (int i = 0; i < SIZE3; i++)
		maze3[i] = new int[SIZE3];
	// CLONE MAZES
	int **clone1, **clone2, **clone3;
	clone1 = new int*[SIZE1];
	for (int i = 0; i < SIZE1; i++)
		clone1[i] = new int[SIZE1];
	clone2 = new int*[SIZE2];
	for (int i = 0; i < SIZE2; i++)
		clone2[i] = new int[SIZE2];
	clone3 = new int*[SIZE3];
	for (int i = 0; i < SIZE3; i++)
		clone3[i] = new int[SIZE3];
	

	// INIT MAZES
	readMaze(maze1,maze2,maze3,argv);

	// -------------SIZE 10x10------------------
	// BFS 
	clone_array(clone1, maze1, SIZE1);
	newline;
	cout << "\t----RESULT FOR BFS FOR SIZE: "<<SIZE1<<"----" << endl;
	if (BFS(clone1,SIZE1) == -1)
		cout << "No path." << endl;
	else
		printMaze(clone1,SIZE1);

	// RW
	clone_array(clone1, maze1, SIZE1);
	newline;
	cout << "\t----RESULT FOR RANDOMWALK FOR SIZE: " << SIZE1 << "----" << endl;
	if (RW(clone1, SIZE1) == -1){
		cout << "No path." << endl;
		printMaze(clone1, SIZE1);
	}
	else
		printMaze(clone1, SIZE1);

	// DFS
	clone_array(clone1, maze1, SIZE1);
	newline;
	cout << "\t----RESULT FOR DFS FOR SIZE: " << SIZE1 << "----" << endl;
	if (DFS(clone1, SIZE1) == -1)
		cout << "No path." << endl;
	else
		printMaze(clone1, SIZE1);

	// -------------SIZE 12x12------------------
	// BFS 
	clone_array(clone2, maze2, SIZE2);
	newline;
	cout << "\t----RESULT FOR BFS FOR SIZE: " << SIZE2 << "----" << endl;
	if (BFS(clone2, SIZE2) == -1)
		cout << "No path." << endl;
	else
		printMaze(clone2, SIZE2);

	// RW
	clone_array(clone2, maze2, SIZE2);
	newline;
	cout << "\t----RESULT FOR RANDOMWALK FOR SIZE: " << SIZE2 << "----" << endl;
	if (RW(clone2, SIZE2) == -1){
		cout << "No path." << endl;
		printMaze(clone2, SIZE2);
	}
	else
		printMaze(clone2, SIZE2);

	// DST
	clone_array(clone2, maze2, SIZE2);
	newline;
	cout << "\t----RESULT FOR DFS FOR SIZE: " << SIZE2 << "----" << endl;
	if (DFS(clone2, SIZE2) == -1)
		cout << "No path." << endl;
	else
		printMaze(clone2, SIZE2);

	// -------------SIZE 15x15------------------
	// BFS 
	clone_array(clone3, maze3, SIZE3);
	newline;
	cout << "\t----RESULT FOR BFS FOR SIZE: " << SIZE3 << "----" << endl;
	if (BFS(clone3, SIZE3) == -1)
		cout << "No path." << endl;
	else
		printMaze(clone3, SIZE3);

	// RW
	clone_array(clone3, maze3, SIZE3);
	newline;
	cout << "\t----RESULT FOR RANDOMWALK FOR SIZE: " << SIZE3 << "----" << endl;
	if (RW(clone3, SIZE3) == -1){
		cout << "No path." << endl;
		printMaze(clone3, SIZE3);
	}
	else
		printMaze(clone2, SIZE3);

	// DST
	clone_array(clone3, maze3, SIZE3);
	newline;
	cout << "\t----RESULT FOR DFS FOR SIZE: " << SIZE3 << "----" << endl;
	if (DFS(clone3, SIZE3) == -1)
		cout << "No path." << endl;
	else
		printMaze(clone3, SIZE3);

	delete(maze1);
	delete(clone1);
	delete(maze2);
	delete(clone2);
	delete(maze3);
	delete(clone3);
	getchar();
	getchar();
	return 0;
}
