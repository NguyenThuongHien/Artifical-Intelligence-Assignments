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
#include <chrono>


using namespace std;

typedef pair<int,int> ii;

struct state
{
    vector< vector<char> > board;
    int val;
    int x,y;
    char color;
};

int values[6][6];
vector<vector<char> > bestMove;
int nodesExpanded;
long long int totalNodesB = 0;
long long int totalNodesG = 0;
double bTime = 0;
double gTime = 0;
int dr[] = {1,0,-1,0,1,1,-1,-1}; // S,E,N,W,SW,SE,NE,NW neighbors
int dc[] = {0,1,0,-1,-1,1,1,-1};

void print_board(vector<vector<char> > &board)
{
    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 6; j++){
            if (board[i][j] == ' ') printf("X");
            else printf("%c",board[i][j]);
        }
        printf("\n");
    }
}

bool isTerminalState(vector<vector<char> > &board)
{
    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 6; j++){
            if (board[i][j] == ' '){
                return false;
            }
        }
    }
    return true;
}

bool in_bounds(int i, int j)
{
    return (i >= 0 && i < 6 && j >= 0 && j < 6);
}

//check if there is adjacent candy of same color
bool sameAdjacent(vector<vector<char> > &board, int x, int y, char color)
{
    for (int i = 0; i < 4; i++){
        if (in_bounds(dr[i]+x,dc[i]+y) && board[dr[i]+x][dc[i]+y] == color){
            return true;
        }
    }
    return false;
}

//update to 8 positions
void update_board(state &node, int x, int y, char color)
{
    if (sameAdjacent(node.board,x,y,color)){
        for (int i = 0; i < 4; i++){
            if (in_bounds(dr[i]+x,dc[i]+y) && node.board[dr[i]+x][dc[i]+y] == 'G' && color == 'B'){
                node.board[dr[i]+x][dc[i]+y] = 'B';
            }
            else if (in_bounds(dr[i]+x,dc[i]+y) && node.board[dr[i]+x][dc[i]+y] == 'B' && color == 'G'){
                node.board[dr[i]+x][dc[i]+y] = 'G';
            }
        }
    }
}

//sum of candies than can be stolen in next turn
int weakness(vector<vector<char> > &board, char color)
{
    bool stolen[6][6];
    int sum = 0;

    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 6; j++){
            stolen[i][j] = false;
        }
    }

    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 6; j++){
            if (board[i][j] == ' ' && color == 'B' && sameAdjacent(board,i,j,'B')){
                for (int k = 0; k < 4; k++){
                    if (in_bounds(dr[k]+i,dc[k]+j) && board[dr[k]+i][dc[k]+j] == 'G' && !stolen[dr[k]+i][dc[k]+j]){
                        stolen[dr[k]+i][dc[k]+j] = true;
                        sum += values[dr[k]+i][dc[k]+j];
                    }
                }
            }
            else if (board[i][j] == ' ' && color == 'G' && sameAdjacent(board,i,j,'G')){
                for (int k = 0; k < 4; k++){
                    if (in_bounds(dr[k]+i,dc[k]+j) && board[dr[k]+i][dc[k]+j] == 'B' && !stolen[dr[k]+i][dc[k]+j]){
                        stolen[dr[k]+i][dc[k]+j] = true;
                        sum += values[dr[k]+i][dc[k]+j];
                    }
                }
            }
        }
    }
    return sum;
}

//final score
int score(vector<vector<char> > &board)
{
    int bSum = 0;
    int gSum = 0;

    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 6; j++){
            if (board[i][j] == 'B'){
                bSum += values[i][j];
            }
            else if (board[i][j] == 'G'){
                gSum += values[i][j];
            }
        }
    }
    return bSum-gSum;
}

int heuristic(state &node)
{
    int w1 = 0;
    int w2 = 0;

    w1 = score(node.board);
    w2 = weakness(node.board,node.color);

    if (node.color == 'G'){
        return w1-w2;
    }
    else return w1+w2;
}


state minimax(state node, int depth)
{
    state bestMove;
    if (isTerminalState(node.board)){
        bestMove.val = score(node.board);
        bestMove.board = node.board;
        return bestMove;
    }
    if (depth == 0){
        bestMove.val = heuristic(node);
        bestMove.board = node.board;
        return bestMove;
    }

    if (node.color == 'B'){
        bestMove.val = -1000000;
        for (int i = 0; i < 6; i++){
            for (int j = 0; j < 6; j++){
                if (node.board[i][j] == ' '){
                    state next;
                    next.color = 'G';
                    next.board = node.board;
                    next.board[i][j] = 'B';
                    update_board(next,i,j,'B');
                    next.val = minimax(next,depth-1).val;
                    nodesExpanded++;

                    if (next.val > bestMove.val){
                        bestMove.val = next.val;
                        bestMove.board = next.board;
                        bestMove.x = i;
                        bestMove.y = j;
                    }
                }
            }
        }
    }
    else{
        bestMove.val = 1000000;
        for (int i = 0; i < 6; i++){
            for (int j = 0; j < 6; j++){
                if (node.board[i][j] == ' '){
                    state next;
                    next.color = 'B';
                    next.board = node.board;
                    next.board[i][j] = 'G';
                    update_board(next,i,j,'G');
                    next.val = minimax(next,depth-1).val;
                    nodesExpanded++;

                    if (next.val < bestMove.val){
                        bestMove.val = next.val;
                        bestMove.board = next.board;
                        bestMove.x = i;
                        bestMove.y = j;
                    }
                }
            }
        }
    }
    return bestMove;
}

state AlphaBeta(state node, int depth, int alpha, int beta)
{
    state bestMove;

    if (isTerminalState(node.board)){
        bestMove.val = score(node.board);
        bestMove.board = node.board;
        return bestMove;
    }
    if (depth == 0){
        bestMove.val = heuristic(node);
        bestMove.board = node.board;
        return bestMove;
    }

    if (node.color == 'B'){
        bestMove.val = -1000000;
        for (int i = 0; i < 6; i++){
            for (int j = 0; j < 6; j++){
                if (node.board[i][j] == ' '){
                    state next;
                    next.color = 'G';
                    next.board = node.board;
                    next.board[i][j] = 'B';
                    update_board(next,i,j,'B');
                    next.val = AlphaBeta(next,depth-1,alpha,beta).val;
                    alpha = max(alpha,next.val);
                    nodesExpanded++;

                    /*if (beta <= alpha){
                      return next; //? or bestMove?
                      }*/

                    if (next.val > bestMove.val){
                        bestMove.val = next.val;
                        bestMove.board = next.board;
                        bestMove.x = i;
                        bestMove.y = j;
                    }
                    if (beta <= alpha){
                        return bestMove;
                    }
                }
            }
        }
    }
    else{
        bestMove.val = 1000000;
        for (int i = 0; i < 6; i++){
            for (int j = 0; j < 6; j++){
                if (node.board[i][j] == ' '){
                    state next;
                    next.color = 'B';
                    next.board = node.board;
                    next.board[i][j] = 'G';
                    update_board(next,i,j,'G');
                    next.val = AlphaBeta(next,depth-1,alpha,beta).val;
                    beta = min(beta,next.val);
                    nodesExpanded++;

                    /*if (beta <= alpha){
                      return next; //??? or bestMove?
                      }*/

                    if (next.val < bestMove.val){
                        bestMove.val = next.val;
                        bestMove.board = next.board;
                        bestMove.x = i;
                        bestMove.y = j;
                    }
                    if (beta <= alpha){
                        return bestMove;
                    }
                }
            }
        }
    }
    return bestMove;
}

void printScore(vector<vector<char> > &board)
{
    int bSum = 0;
    int gSum = 0;

    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 6; j++){
            if (board[i][j] == 'B'){
                bSum += values[i][j];
            }
            else if (board[i][j] == 'G'){
                gSum += values[i][j];
            }
        }
    }
    printf("Scores: Blue: %d --- Green: %d\n",bSum,gSum);
}

//does the printing, calls to minimax or alpha-beta, simulates gameplay
void driver(state start, int a, int b)
{
    state currState = start;
    chrono::time_point<chrono::system_clock> begin,end;
    chrono::duration<double> elapsed;
    bTime = 0;
    gTime = 0;
    totalNodesB = 0;
    totalNodesG = 0;

    for (int i = 0; i < 36; i++){
        nodesExpanded = 0;
        if (i % 2 == 0){
            currState.color = 'B';
            begin = chrono::system_clock::now();
            if (a == 0){
                currState = minimax(currState,3);
            }
            else currState = AlphaBeta(currState,5,-1000000,1000000);
            end = chrono::system_clock::now();
            totalNodesB += nodesExpanded;
            elapsed = end-begin;
            bTime += elapsed.count();
            printf("Drop blue: ");
        }
        else{
            currState.color = 'G';
            begin = chrono::system_clock::now();
            if (b == 0){
                currState = minimax(currState,3);
            }
            else currState = AlphaBeta(currState,5,-1000000,1000000);
            end = chrono::system_clock::now();
            totalNodesG += nodesExpanded;
            elapsed = end-begin;
            gTime += elapsed.count();
            printf("Drop green: ");
        }
        if (currState.y == 0){
            printf("A");
        }
        else if (currState.y == 1){
            printf("B");
        }
        else if (currState.y == 2){
            printf("C");
        }
        else if (currState.y == 3){
            printf("D");
        }
        else if (currState.y == 4){
            printf("E");
        }
        else printf("F");
        printf("%d\n",currState.x+1);
    }
    printScore(currState.board);
    printf("Total nodes expanded: Blue: %lld --- Green: %lld\n",totalNodesB,totalNodesG);
    printf("Average nodes expanded per move: Blue: %lld --- Green: %lld\n",totalNodesB/18,totalNodesG/18);
    printf("Average time per move -- Blue: %lf --- Green: %lf\n",bTime/18.0,gTime/18.0);
    print_board(currState.board);
}

int main(void)
{
    state start;
    start.board.resize(6);
    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 6; j++){
            scanf("%d",&values[i][j]);
            start.board[i].push_back(' ');
        }
    }

    driver(start,0,0);
    printf("\n");
    driver(start,1,1);
    printf("\n");
    driver(start,1,0);
    printf("\n");
    driver(start,0,1);

    return 0;
}
