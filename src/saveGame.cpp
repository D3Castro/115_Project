#include "saveGame.h"

using namespace std;

saveGame::saveGame()
{
    //ctor
}

saveGame::~saveGame()
{
    //dtor
}
void saveGame::saveObjects(int m[][10])
{
    ofstream aFile;
    aFile.open("saveFile.txt");
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            switch(m[i][j])
            {
            case 0:
                continue;
                break;
            case 1:
                aFile<< "WALL " << i << " " << j << endl;
                break;
            case 3:
                aFile<< "PLAYER " << i << " " << j << endl;
                break;
            case 4:
                aFile<< "CHEST " << i << " " << j << endl;
                break;
            case 5:
                aFile<< "ARROW " << i << " " << j << endl;
                break;
            case 20:
                aFile<< "ENEMY " << i << " " << j << endl;
                break;
            case 21:
                aFile<< "ENEMY " << i << " " << j << endl;
                break;
            case 22:
                aFile<< "ENEMY " << i << " " << j << endl;
                break;
            case 23:
                aFile<< "ENEMY " << i << " " << j << endl;
                break;
            case 24:
                aFile<< "ENEMY " << i << " " << j << endl;
                break;
            case 25:
                aFile<< "ENEMY " << i << " " << j << endl;
                break;
            case 26:
                aFile<< "ENEMY " << i << " " << j << endl;
                break;
            case 27:
                aFile<< "ENEMY " << i << " " << j << endl;
                break;
            case 28:
                aFile<< "ENEMY " << i << " " << j << endl;
                break;
            case 29:
                aFile<< "ENEMY " << i << " " << j << endl;
                break;
            default:
                cout << "invalid number" << endl;
                break;
            }
        }
    }
    aFile.close();
    cout << "Game Saved!" << endl;
}
