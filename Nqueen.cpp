#include <iostream>
#include <vector>
using namespace std;
#define newline cout<<endl;
typedef struct queen
{
	int x;
	int y;
}queen;

vector<vector<int> > init_map(vector<vector<int> > map,int size) // Generating the map.
{
	map.resize(size); 
	for (int i = 0; i < size; i++)
	{
		map[i].resize(size);  
		for (int j = 0; j < size; j++)
		{
			map[i][j] = 0;
		}
	}
	return map;
} 
void print_map(vector<vector<int> > map, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			cout << map[i][j]<<" ";
		}
		newline
	}
	newline newline
}
bool check_verticals(vector<vector<int> > map, int size,int x,int y)
{
	// Scanning..
	for (int i = y; i >= 0; i--)
	{
		if (map[i][x] == 1)
			return false; // Queen found.
	}
	return true; // Position is safe.
}
bool check_diagonals(vector<vector<int> > map, int size, int x, int y)
{
	int func_x = x;
	int func_y = y;
	// Scanning converging diagonal..
	for (int i = func_y; i>=0; i--)
	{
		if (func_x < 0)
			break;
		if (map[i][func_x] == 1)
			return false; // Queen found
		func_x--;
	}
	func_x = x;
	func_y = y;
	// Scanning diverging diagonal..
	for (int i = func_y; i>=0; i--)
	{
		if (func_x > size-1)
			break;
		if (map[i][func_x] == 1)
			return false; // Queen found
		func_x++;
	}
	return true; // Position is safe.
}

int main()
{
	// map[ Y ][ X ]
	bool diagonal_check, vertical_check,start=false,found_safe;
	int map_size,queen_count=0; // We'll use map_size as maximum queens.
	cout << "Enter size : ";
	cin >> map_size; newline newline
	vector<queen> queens;
	vector<vector<int> > map;
	map=init_map(map, map_size);
	
	while (queen_count < map_size)
	{
		queen temp;
		if (queen_count == 0 && start==false) // Add first queen manually.
		{
			start = true;
			temp.x = 0;
			temp.y = 0;
			queens.push_back(temp);
			queen_count++;
			map[temp.y][temp.x] = 1;
		}
		else // Find a safe position.
		{
			found_safe = false;
			for (int i = 0; i < map_size; i++) // Try all and save when find.
			{
				vertical_check = check_verticals(map, map_size, i, queen_count);
				if (vertical_check)
					diagonal_check = check_diagonals(map, map_size, i, queen_count);
				if (vertical_check && diagonal_check)
				{
					temp.x = i;
					temp.y = queen_count;
					queens.push_back(temp);
					queen_count++;
					map[temp.y][temp.x] = 1;
					found_safe = true;
					break;
				}
			}
			while(found_safe == false) // Find failure, try previous row again.
			{
				int starting_position = queens[queen_count-1].x;
				map[queen_count - 1][starting_position] = 0;
				if ((starting_position + 1) < map_size) // Don't try to find out of boundaries.
				{
					for (int i = starting_position + 1; i < map_size; i++) // Try again
					{
						vertical_check = check_verticals(map, map_size, i, queen_count-1 );
						if (vertical_check)
							diagonal_check = check_diagonals(map, map_size, i, queen_count-1 );
						if (vertical_check && diagonal_check)
						{
							temp.x = i;
							temp.y = queen_count-1;
							//queen_count++;
							queens.pop_back();
							queens.push_back(temp);
							map[temp.y][temp.x] = 1;
							found_safe = true;
							break;
						}
					}
					// Erase operations if previous queen fails to change position.
					if (found_safe == false)
					{
						map[queens[queen_count - 1].y][queens[queen_count - 1].x] = 0;
						queens.pop_back();
						queen_count--;
					}
				}
				else // Starting point +1 is out of boundaries..
				{
					map[queens[queen_count - 1].y][queens[queen_count - 1].x] = 0;
					queens.pop_back();
					queen_count--;
				}
			} // End while..
		}
	}
	print_map(map, map_size);
	return 0;
}
