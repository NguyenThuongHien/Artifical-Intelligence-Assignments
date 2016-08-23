#include "DotEating.hpp"
#include <functional>
#include <map>
#include <queue>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

/*int State::OnDotLocation(int x, int y, std::vector<int_pair> &dots)
{
    for (int i = 0; i < dots.size(); i++){
        if (x == dots[i].first && y == dots[i].second){
            return i;
        }
    }
    return -1;
}

int ManhattanDist(int a, int b, int c, int d)
{
    return abs(a-c)+abs(b-d);
}

int State::GetHeuristic(int x, int y, std::vector<int_pair> &dots)
{
    int max_dist = 0;
    for (int i = 0; i < dots.size(); i++){
        max_dist = max(max_dist, ManhattanDist())
    }
    return ManhattanDist(x,y);
}

void State::PacmanSolve(int x, int y)
{
    std:: vector<int_pair> dots;



    std::priority_queue<State, std::vector<State>,std::greater<State> > pq;
    std::map<State, int> open;
    std::map<State, int> closed;
    int nodes_expanded = 0;

    State start;
    start.x = x;
    start.y = y;
    start.g = 0;
    start.h = GetHeuristic(x, y, dots);
    start.f = start.g + start.h;
    sort(start.dots_remaining.begin(), start.dots_remaining.end());

    pq.push(start);
    open[start] = start.f;

    while (!pq.empty()){
        State top_state = pq.top();
        pq.pop();
        open.erase(top_state);

        if (top_state.dots_remaining.empty()){
            printf("Found goal state after %d steps\n",top_state.g);
            printf("Nodes expanded: %d\n",nodes_expanded);
            break;
        }

        nodes_expanded++;


    }


}
*/
