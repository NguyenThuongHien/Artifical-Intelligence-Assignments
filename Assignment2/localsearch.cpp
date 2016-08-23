#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <math.h>
#include <string>
#include <memory.h>
#include <queue>
#include <set>
#include <map>
#include <ctime>
#include <cstdlib>
#include <random>

using namespace std;

int N, K;
int board[1000][1000];//-1 = tree, 0 = unoccupied, 1 = person
map <int,int> mymap;
bool done;
int soltns = 0;
random_device rd;
mt19937 gen(rd());

int dr[] = {1,0,-1,0,1,1,-1,-1}; // S,E,N,W,SW,SE,NE,NW neighbors
int dc[] = {0,1,0,-1,-1,1,1,-1};


void print_board()
{
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            if (board[i][j] == -1) printf("2");
            else printf("%d",board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


bool inBounds(int i, int j)
{
    return (i >= 0 && i < N && j >= 0 && j < N);
}

void placeFriend(int i, int j)
{
    board[i][j] = 1;
}

void removeFriend(int i, int j)
{
    board[i][j] = 0;
}

//check if (x,y) is a safe position
bool isSafe(int x, int y)
{
    for (int i = 0; i < 8; i++){
    	for (int j = 1; j <= N; j++){
            int a = j*dr[i]+x;
            int b = j*dc[i]+y;
            if (inBounds(a,b) && board[a][b] == 1){
                return false;
            }
            else if (inBounds(a,b) && board[a][b] == -1) break;
        }
    }
    return true;
}

//backtracking function that finds all solutions
void backtrack(int row, int col, int friends)
{
    if (friends == N){
        soltns++;
        printf("%d\n",soltns);
        return;
    }

    if (!inBounds(row,col)){
        return;
    }

    if (isSafe(row,col)){
        placeFriend(row,col);
        if (col+1 < N){
            backtrack(row,col+1,friends+1);
        }
        else backtrack(row+1,0,friends+1);
        removeFriend(row,col);
    }

    if (col+1 < N){
        backtrack(row,col+1,friends);
    }
    else backtrack(row+1,0,friends);
}

//check to see if variable is conflicted
int findConflicts(int x, int y)
{
    int conflicts = 0;

    if (board[x][y] == -1) return 100000; //can't put on tree

    for (int i = 0; i < 8; i++){
        for (int j = 1; j <= N; j++){
            int a = j*dr[i]+x;
            int b = j*dc[i]+y;
            if (inBounds(a,b) && board[a][b] == 1){
                conflicts++;
            }
            else if (inBounds(a,b) && board[a][b] == -1) break;
        }
    }
    return conflicts;
}

//assign random start
void random_start(int N)
{
    uniform_int_distribution<> dis(0, N-1);
    for (int i = 0; i < N; i++){ //each column, find a row
        int row = -1;
        while (!inBounds(row,i) || board[row][i] == -1){
            row = dis(gen);
        }
        placeFriend(row,i);
        mymap[i] = row;
    }
}

//place conflicted variable in new row with fewest conflicts
void newPlace(int N, int col)
{
    int minConflicts = 1000000;
    vector<int> possRows;
    removeFriend(mymap[col],col);

    for (int i = 0; i < N; i++){
        int conflicts = findConflicts(i,col);
        if (conflicts < minConflicts){
            minConflicts = conflicts;
            possRows.clear();
            possRows.push_back(i);
        }
        else if (conflicts == minConflicts){
            possRows.push_back(i);
        }
    }
    uniform_int_distribution<> dis(0,possRows.size()-1);
    int newRow = possRows[dis(gen)];
    mymap[col] = newRow;
    placeFriend(newRow,col);
}

bool isSoltn(int N)
{
    for (int i = 0; i < N; i++){
        int row = mymap[i];
        int col = i;
        if (findConflicts(row,col) > 0){
            return false;
        }
    }
    return true;
}


//driver method which randomly selects conflicted variables until it finds a solution or for loop terminates
void local_search(int N)
{
    random_start(N);
    int row,col;
    vector<int> conflicted;

    for (int i = 0; i < 100000; i++){
        conflicted.clear();

        if (isSoltn(N)){
            printf("Done! in %d iterations\n",i+1);
            for (int j = 0; j < N; j++){
                printf("%d %d\n",mymap[j]+1,j+1);
            }
            break;
        }
        for (int j = 0; j < N; j++){
            col = j;
            row = mymap[col];
            if (findConflicts(row,col) > 0){
                conflicted.push_back(col);
            }
        }
        uniform_int_distribution<> dis(0,conflicted.size()-1);
        int pos = dis(gen);
        newPlace(N,conflicted[pos]);
    }
}

int main(void)
{
    int x,y;
    scanf("%d%d",&N,&K);

    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            board[i][j] = 0;
        }
    }
    for (int i = 0; i < K; i++){
        scanf("%d%d",&x,&y);
        board[x-1][y-1] = -1; //tree
    }
    done = false;
    //backtrack(0,0,0);
    local_search(N);

    return 0;
}
