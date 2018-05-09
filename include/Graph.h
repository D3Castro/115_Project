#ifndef GRAPH_H
#define GRAPH_H

#include <CommonThings.h>
#include <queue>
#include <string.h> /*memset*/
#include <math.h> /*round()*/

struct qNode
{
    GridLoc pt;  // The cordinates of a cell
    vector<loc> sPath;  // shortest path
};

class Graph
{
    public:
        Graph();
        Graph(int matrix[10][10]);
        virtual ~Graph();

        bool isValid(int row, int col);
        void BFS(GridLoc src, GridLoc dest, vector<loc> *path);

    protected:

    private:
        //Helper arrays to get adjacent squares for BFS
        int rowNum[4] = {-1, 0, 0, 1};
        int colNum[4] = {0, -1, 1, 0};
        int n = 10;
        int **adj;
};

#endif // GRAPH_H
