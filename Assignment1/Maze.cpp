#include "Maze.hpp"
#include "DotEating.hpp"
#include <queue>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <map>

int dr[] = {-1,1,0,0}; //North/South/East/West
int dc[] = {0,0,1,-1};

//Read maze from file
void Maze::ReadMaze(std::ifstream &myfile)
{
	std::string line;
	int i = 0;

	if (myfile.is_open()){
		while(getline(myfile,line)){
			maze[i] = line;
			i++;
		}
	}
}

//find start location
int_pair Maze::FindStart()
{
	int len = maze[0].size();

	for (int i = 0; i < 1000; i++){
		for (int j = 0; j < len; j++){
			if (maze[i][j] == 'S'){
				return int_pair(i,j);
			}
		}
	}
}

//Find goal location
void Maze::FindGoal()
{
	int len = maze[0].size();

	for (int i = 0; i < 1000; i++){
		for (int j = 0; j < len; j++){
			if (maze[i][j] == 'G'){
				xGoal = i;
				yGoal = j;
				break;
			}
		}
	}
}

//Can we move to this position? Can't go outside of maze or into % area.
bool Maze::IsValidMove(int i, int j)
{
    return (maze[i][j] != '%' && i >= 0 && j >= 0);
}

//Are we at the goal?
bool Maze::IsSolution(int i, int j)
{
    return maze[i][j] == 'G';
}

//Heuristic used to estimate # of steps until we reach goal
int ManhattanDist(int a, int b, int c, int d)
{
	return abs(a-c)+abs(b-d);
}

//Show how we reached goal
void Maze::ShowPath()
{
	int len = maze[0].size();

	for (int i = 0; i < 1000; i++){
		for (int j = 0; j < len; j++){
			printf("%c",maze[i][j]);
	    }
		printf("\n");
	}
}

//Mark path from Start to Goal and return path length
int Maze::PrintPath(int i, int j)
{
	if (maze[i][j] == 'S'){
		maze[xGoal][yGoal] = 'G';
		return 0;
	}
	maze[i][j] = '.';
	if (parent[i][j] == 'u'){
		return 1+PrintPath(i-1,j);
	}
	else if (parent[i][j] == 'd'){
		return 1+PrintPath(i+1,j);
	}
	else if (parent[i][j] == 'l'){
		return 1+PrintPath(i,j-1);
	}
	else{
		return 1+PrintPath(i,j+1);
	}
}

//Declare all positions to be unvisited
void Maze::InitMaze()
{
	int len = maze[0].size();

    for (int i = 0; i < 1000; i++){
        for (int j = 0; j < len; j++){
            visited[i][j] = false;
        }
    }
}

void Maze::BFS(int x, int y)
{
    InitMaze();

    std::queue < std::pair<int,int> > q;
    q.push(std::make_pair(x,y));
    visited[x][y] = true;
    int nodes_expanded = 0;

    while (!q.empty()){
        std::pair<int,int> position = q.front();
        q.pop();

        if (IsSolution(position.first, position.second)){
            printf("Nodes expanded: %d\n",nodes_expanded);
            printf("Path length: %d\n",PrintPath(position.first,position.second));
            break;
        }

        nodes_expanded++;

        for (int i = 0; i < 4; i++){
            int new_x = position.first+dr[i];
            int new_y = position.second+dc[i];

            if (IsValidMove(new_x, new_y) && !visited[new_x][new_y]){
                visited[new_x][new_y] = true;
                q.push(std::make_pair(new_x,new_y));

                if (i == 0) parent[new_x][new_y] = 'd'; //down
                else if (i == 1) parent[new_x][new_y] = 'u'; //up
                else if (i == 2) parent[new_x][new_y] = 'l'; //left
                else parent[new_x][new_y] = 'r'; //right
            }
        }
    }
}

void Maze::DFS(int x, int y)
{
    InitMaze();
    DFS_Main(x,y,0,false);
}


void Maze::DFS_Main(int x, int y, int nodes_expanded, bool done)
{
    if (done) return;

    if (IsSolution(x,y)){
        printf("Nodes expanded: %d\n",nodes_expanded);
        printf("Path length: %d\n",PrintPath(x,y));
        done = true;
        return;
    }

    visited[x][y] = true;
    nodes_expanded++;

    for (int i = 0; i < 4; i++){
        int new_x = x+dr[i];
        int new_y = y+dc[i];

        if (IsValidMove(new_x, new_y) && !visited[new_x][new_y]){
            if (i == 0) parent[new_x][new_y] = 'd'; //down
            else if (i == 1) parent[new_x][new_y] = 'u'; //up
            else if (i == 2) parent[new_x][new_y] = 'l'; //left
            else parent[new_x][new_y] = 'r'; //right

            DFS_Main(new_x, new_y, nodes_expanded, done);
        }
    }
}

//comparison for Best First (only consider Manhattan distance heuristic)
struct cmp
{
    bool operator()(State &a, State &b)
    {
        return a.GetH() > b.GetH();
    }
};


//Min priority queue based on heuristic (greedy approach)
void Maze::BestFirstSearch(int x, int y)
{
    InitMaze();
    std::priority_queue <State, std::vector<State>, cmp> pq;

    State start;
    start.SetX(x);
    start.SetY(y);
    start.SetG(0);
    start.SetH(0);
    start.SetF();

    pq.push(start);
    int nodes_expanded = 0;

    while (!pq.empty()){
        State current = pq.top();
        int x1 = current.GetX();
        int y1 = current.GetY();
        visited[x1][y1] = true;
        pq.pop();

        if (IsSolution(x1,y1)){
            printf("Nodes expanded: %d\n",nodes_expanded);
            printf("Path length: %d\n",PrintPath(x1, y1));
            break;
        }

        nodes_expanded++;

        for (int i = 0; i < 4; i++){
            int new_x = x1+dr[i];
            int new_y = y1+dc[i];

            if (IsValidMove(new_x, new_y) && !visited[new_x][new_y]){
                if (i == 0) parent[new_x][new_y] = 'd'; //down
                else if (i == 1) parent[new_x][new_y] = 'u'; //up
                else if (i == 2) parent[new_x][new_y] = 'l'; //left
                else parent[new_x][new_y] = 'r'; //right

                State successor;
                successor.SetX(new_x);
                successor.SetY(new_y);
                successor.SetH(ManhattanDist(new_x,new_y,xGoal,yGoal));
                pq.push(successor);
            }
        }
    }
}

void Maze::A_Star_Search(int x, int y)
{
	 std::map<int_pair,int> open;
	 std::map<int_pair,int> closed;
	 std::priority_queue< State, std::vector<State>, std::greater<State> > pq;

	 State start;
	 start.SetX(x);
	 start.SetY(y);
	 start.SetG(0);
	 start.SetH(0);
	 start.SetF();

	open.insert(std::make_pair(int_pair(x,y),start.GetF()));
	pq.push(start);
	int nodes_expanded = 0;

	while (!pq.empty()){
		State current = pq.top();
		int x1 = current.GetX();
		int y1 = current.GetY();
	    open.erase(int_pair(x1,y1));
	    pq.pop();

	    if (IsSolution(x1,y1)){
	    	printf("Nodes expanded: %d\n",nodes_expanded);
	    	printf("Path length: %d\n",PrintPath(x1,y1));
	    	break;
	    }

	    nodes_expanded++;

	    for (int i = 0; i < 4; i++){
	    	int new_x = x1+dr[i];
	    	int new_y = y1+dc[i];

	        if (IsValidMove(new_x,new_y)){
	        	State successor;
	        	successor.SetX(new_x);
	        	successor.SetY(new_y);
	        	successor.SetG(current.GetG()+1);
	        	successor.SetH(ManhattanDist(new_x,new_y,xGoal,yGoal));
	        	successor.SetF();

	        	if (open.count(int_pair(new_x,new_y)) == 0 && closed.count(int_pair(new_x,new_y)) == 0){
	                    pq.push(successor);
	                    open[int_pair(new_x,new_y)] = successor.GetF();
	                    if (i == 0) parent[new_x][new_y] = 'd'; //down
	                    else if (i == 1) parent[new_x][new_y] = 'u'; //up
	                    else if (i == 2) parent[new_x][new_y] = 'l'; //left
	                    else parent[new_x][new_y] = 'r'; //right
	            }
	        	else if (open.count(int_pair(x1,y1)) > 0){
	        		int f = open[int_pair(x1,y1)];
	        		if (successor.GetF() < f){
	        			pq.push(successor);
	        			open[int_pair(x1,y1)] = successor.GetF();
	        			if (i == 0) parent[new_x][new_y] = 'd'; //down
	        			else if (i == 1) parent[new_x][new_y] = 'u'; //up
	        			else if (i == 2) parent[new_x][new_y] = 'l'; //left
	        			else parent[new_x][new_y] = 'r'; //right
	        		}
	        	}
	        	else if (closed.count(int_pair(x1,y1)) > 0){
	        		int f = closed[int_pair(x1,y1)];
	        		if (successor.GetF() < f){
	        			pq.push(successor);
	        			open[int_pair(x1,y1)] = successor.GetF();
	        			if (i == 0) parent[new_x][new_y] = 'd'; //down
	        			else if (i == 1) parent[new_x][new_y] = 'u'; //up
	        			else if (i == 2) parent[new_x][new_y] = 'l'; //left
	        			else parent[new_x][new_y] = 'r'; //right
	                }
	            }
	        }
	        closed[int_pair(x1,y1)] = current.GetF();
	    }
	}

}
