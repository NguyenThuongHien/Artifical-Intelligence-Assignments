#ifndef DotEating_H_
#define DotEating_H_

#include <vector>

typedef std::pair<int,int> int_pair;

class State
{
 public:
    void SetX(int _x){x = _x;}
    void SetY(int _y){y = _y;}
    void SetG(int _g){g = _g;}
    void SetH(int _h){h = _h;}
    void SetF(){f = g+h;}
    int GetH(){return h;}
    int GetF(){return f;}
    int GetG(){return g;}
    int GetX(){return x;}
    int GetY(){return y;}

    //Compare based on distance traveled so far
    bool operator<(const State& other) const
    {
        if (f < other.f) return true;
        if (int_pair(x,y) < int_pair(other.x,other.y)) return true;
        if (dots_remaining < other.dots_remaining) return true;

        return false;
    }

    bool operator>(const State& other) const
    {
        if (f > other.f) return true;
        if (int_pair(x,y) > int_pair(other.x,other.y)) return true;
        if (dots_remaining > other.dots_remaining) return true;

        return false;
    }

    bool operator==(const State& other) const
    {
        return (g == other.g && x == other.x && y == other.y && dots_remaining == other.dots_remaining);
    }

 private:
    int x,y; //current location in maze
    int f,g,h; //g = distance traveled, h = heuristic distance, f = g+h

    std::vector<int_pair> dots_remaining;
    std::vector<int_pair> eaten_order;

    int OnDotLocation(int x, int y, std::vector<int_pair> &dots);
    int GetHeuristic(int x, int y, std::vector<int_pair> &dots);
    void PacmanSolve(int x, int y);

};

#endif
