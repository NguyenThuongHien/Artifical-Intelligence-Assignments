#include "DotEating.hpp"
#include "Maze.hpp"
#include <string>
#include <iostream>
#include <fstream>

int main()
{
	std::string function;
	char file_name[100];
	Maze maze;

	scanf("%s",&file_name);
	std::ifstream myfile(file_name);

	maze.ReadMaze(myfile);
	int_pair start_pos = maze.FindStart();
	maze.FindGoal();




	return 0;
}
