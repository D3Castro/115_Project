#include "Graph.h"

Graph::Graph()
{
    //ctor
}

Graph::~Graph()
{
    //dtor
}

Graph::Graph(int matrix[10][10])
{
    adj = new int* [n];
    for (int i = 0; i < n; i++){
        adj[i] = new int [n];
        for(int j = 0; j < n; j++){
            if(matrix[i][j] == 1) {
                adj[i][j] = 0;
            } else {
                adj[i][j] = 1;
            }
        }
    }
}

bool Graph::isValid(int row, int col)
{
    return (row >= 0) && (row < n) &&
           (col >= 0) && (col < n);
}

void Graph::BFS(GridLoc src, GridLoc dest, vector<loc> *path)
{
    if(!path->empty())
        path->clear();

    bool visited[10][10]; memset(visited, false, sizeof visited);
    visited[(int)src.x][(int)src.y] = true;

    queue<qNode> q;
    vector<loc> shortPath;
    qNode s = {src, shortPath};
    q.push(s);

    cout<< "Dest: " << dest.x << "    " << dest.y << endl;
    while(!q.empty()){
        qNode curr = q.front(); GridLoc currLoc = curr.pt;

        loc pathloc; pathloc.x = currLoc.x; pathloc.y = currLoc.y;
        curr.sPath.push_back(pathloc);

        if(currLoc.x == dest.x && currLoc.y == dest.y){
            if(curr.sPath.size() > 1){
                for (auto i = curr.sPath.begin(); i != curr.sPath.end(); ++i)
                    cout << i->x << ' ' << i->y << '\n';

                path->assign(curr.sPath.begin(),curr.sPath.end());
            }
            return;
        }

        q.pop();

        //Get 4 adjacent cells (N,S,E,W)
        for(int i = 0; i < 4; i++){
            int row = currLoc.x + rowNum[i];
            int col = currLoc.y + colNum[i];

             if (isValid(row, col) && adj[row][col] != 0 && !visited[row][col])
            {
                visited[row][col] = true;
                qNode adjacentCell = { {row, col}, curr.sPath };
                q.push(adjacentCell);
            }
        }
    }
    //Returns empty path in case of error
    path->clear();
}
