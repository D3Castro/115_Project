#ifndef SAVEGAME_H
#define SAVEGAME_H

#include<iostream>
#include <vector>
#include <fstream>
#include <sstream>

class saveGame
{
    public:
        saveGame();
        virtual ~saveGame();
        static void saveObjects(int m[][10]);

    protected:

    private:
};

#endif // SAVEGAME_H
