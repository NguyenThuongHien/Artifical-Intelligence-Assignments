#ifndef MAZE_H_
#define MAZE_H_

#include <string>

typedef std::pair<int,int> int_pair;

class Maze
{
 public:
    void ReadMaze(std::ifstream &myfile);
    int_pair FindStart();
    void FindGoal();
    void BFS(int x, int y);
    void A_Star_Search(int x, int y);
    void DFS(int x, int y);
    void DFS_Main(int x, int y, int nodes_expanded, bool done);
    void BestFirstSearch(int x, int y);
    //void Pacman(int x, int y);
    int xGoal,yGoal;


 private:
    int M,N; //maze dimensions
    int start_x, start_y;
    std::string maze[1000]; //maze itself
    int parent[1000][1000]; //for finding path between start and finish
    bool visited[1000][1000];

    bool IsValidMove(int i, int j);
    bool IsSolution(int i, int j);
    void ShowPath();
    int PrintPath(int i, int j);
    void InitMaze();


};


#endif /* MAZE_H_ */
