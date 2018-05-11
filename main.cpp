/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */

#include <string.h>
#include <CommonThings.h>
#include <Maze.h>
#include <iostream>
#include <Timer.h>
#include <player.h>
#include "saveGame.h"
#include <vector>
#include <string>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <Enemies.h>

#include <wall.h>
#include <math.h>
#include <Graph.h>

/* GLUT callback Handlers */

using namespace std;

Maze *M = new Maze(10);                         // Set Maze grid size
Player *P = new Player();                       // create player

wall W[100];                                    // wall with number of bricks
Enemies E[10];                                  // create number of enemies
Timer *T0 = new Timer();                        // animation timer
Timer *moveTimer = new Timer();

float wWidth, wHeight;                          // display window width and Height
float xPos,yPos;                                // Viewpoar mapping
const int n = 10;
int eIndex = 0;
int matrix[n][n];
Graph* graph;
bool shoot = false;
bool hasArrows = true;
bool aQuiver = true;
bool collide = false;
bool ammo = false;
bool win = false;
bool lose = false;

void display(void);                             // Main Display : this runs in a loop

void resize(int width, int height)              // resizing case on the window
{
    wWidth = width;
    wHeight = height;

    if(width<=height)
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);
}

void init()
{
    glEnable(GL_COLOR_MATERIAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0,0.0,0.0,0.0);
    gluOrtho2D(0, wWidth, 0, wHeight);

    T0->Start();                                        // set timer to 0
    moveTimer->Start();

    glEnable(GL_BLEND);                                 //display images with transparent
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    M->loadBackgroundImage("images/bak.jpg");           // Load maze background image
    P->loadArrowImage("images/arr.png");                // Load arrow image

    string loc1, loc2;
    int enemies = 20;
    int wIndex = 0;
    vector<string> tokens = readObjects("mazedoc.txt");
    //Initialize as empty maze
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            matrix[i][j] = 0;
        }
    }

    while(!tokens.empty()){
        if(tokens.front() == "WALL"){
            tokens.erase(tokens.begin());
            loc1 = tokens.front();
            tokens.erase(tokens.begin());
            loc2 = tokens.front();
            tokens.erase(tokens.begin());
            matrix[stoi(loc1)][stoi(loc2)] = 1;

            W[wIndex].wallInit(M->getGridSize(),"images/wall.png"); // Load walls
            W[wIndex].placeWall(stoi(loc1),stoi(loc2));             // place each brick
            wIndex++;

        }
        else if (tokens.front() == "ENEMY") {
            tokens.erase(tokens.begin());
            loc1 = tokens.front();
            tokens.erase(tokens.begin());
            loc2 = tokens.front();
            tokens.erase(tokens.begin());
            if (enemies < 30){
                matrix[stoi(loc1)][stoi(loc2)] = enemies;
                enemies++;

                E[eIndex].initEnm(M->getGridSize(),4,"images/e.png"); //Load enemy image
                E[eIndex].placeEnemy(stoi(loc1), stoi(loc2));
                eIndex++;
            }

        }
        else if (tokens.front() == "PLAYER") {
            tokens.erase(tokens.begin());
            loc1 = tokens.front();
            tokens.erase(tokens.begin());
            loc2 = tokens.front();
            tokens.erase(tokens.begin());
            matrix[stoi(loc1)][stoi(loc2)] = 3;

            P->initPlayer(M->getGridSize(),6,"images/p.png");   // initialize player pass grid size,image and number of frames
            P->placePlayer(stoi(loc1),stoi(loc2));              // Place player

        }
        else if (tokens.front() == "ARROW") {
            tokens.erase(tokens.begin());
            loc1 = tokens.front();
            tokens.erase(tokens.begin());
            loc2 = tokens.front();
            tokens.erase(tokens.begin());
            matrix[stoi(loc1)][stoi(loc2)] = 5;

            M->loadSetOfArrowsImage("images/arrwset.png");      // load set of arrows image
            M->placeStArrws(stoi(loc1),stoi(loc2));             // place set of arrows

        }
        else if (tokens.front() == "CHEST") {
            tokens.erase(tokens.begin());
            loc1 = tokens.front();
            tokens.erase(tokens.begin());
            loc2 = tokens.front();
            tokens.erase(tokens.begin());
            matrix[stoi(loc1)][stoi(loc2)] = 4;

            M->loadChestImage("images/chest.png");              // load chest image
            M->placeChest(stoi(loc1),stoi(loc2));               // place chest in a grid
        }
    }

    graph = new Graph(matrix);
}

void display(void)
{
    if(P->arrowStatus){
        if(matrix[P->getArrowLoc().x][P->getArrowLoc().y] == 1){
            P->arrowStatus = false;
        }
    }

  glClear (GL_COLOR_BUFFER_BIT);        // clear display screen

        glPushMatrix();
         M->drawBackground();
        glPopMatrix();


        for(int i=0; i< 100;i++)
        {
           W[i].drawWall();
        }

        glPushMatrix();
            M->drawGrid();
        glPopMatrix();

        glPushMatrix();
            P->drawplayer();
        glPopMatrix();

        for(int i=0; i<10;i++)
        {
            if(E[i].live)
                E[i].drawEnemy();
        }

        glPushMatrix();
           P->drawArrow();
        glPopMatrix();

        glPushMatrix();
           M->drawChest();
        glPopMatrix();

        glPushMatrix();
           M->drawArrows();
        glPopMatrix();

    glutSwapBuffers();
    if(win){ cout<< "YOU WIN!" << endl; exit(0);}
}

void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case ' ':                      // enter/exit shooting stance
          if(shoot) {shoot = false;}
          else{shoot = true;}
        break;
        case 's':                     // saves the current matrix into a file
          saveGame::saveObjects(matrix);
        break;
        case 'l':                     // could load matrix into the game
            cout << "Load" << endl;
        case 27 :                       // esc key to exit
        case 'q':
            exit(0);
            break;
    }

    glutPostRedisplay();
}

 void GetOGLPos(int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    xPos =posX ;
    yPos =posY ;
}

 void idle(void)
{

    //Your Code here

    glutPostRedisplay();
}


void mouse(int btn, int state, int x, int y){

    switch(btn){
        case GLUT_LEFT_BUTTON:

        if(state==GLUT_DOWN){

              GetOGLPos(x,y);
            }
            break;


      case GLUT_RIGHT_BUTTON:

        if(state==GLUT_DOWN){

              GetOGLPos(x,y);
            }
            break;
    }
     glutPostRedisplay();
};

void Specialkeys(int key, int x, int y) // Handles player movement and collisions
{
    if(eIndex == 0) win = true;//CHARACTER WINS
    //Utilizes matrix for collision detection by looking at the potential move
    if (moveTimer->GetTicks() > 75){
        switch(key)
        {
          case GLUT_KEY_UP:
                  if(P->checkQuiver()){hasArrows = true;}
                  else{hasArrows = false;}
                  if(matrix[P->getPlayerLoc().x][P->getPlayerLoc().y+1] == 1){collide = true;}
                  if(matrix[P->getPlayerLoc().x][P->getPlayerLoc().y+1] == 5){ammo = true;}
                  if(matrix[P->getPlayerLoc().x][P->getPlayerLoc().y+1] == 4){win = true;}
                  if(!shoot && !collide){
                      matrix[P->getPlayerLoc().x][P->getPlayerLoc().y] = 0;
                      P->movePlayer("up");
                      if(ammo){P->addToQuiver(3); M->collectArrows();}
                      matrix[P->getPlayerLoc().x][P->getPlayerLoc().y] = 3;
                  }
                  else if(shoot){ P->facePlayer("up"); if(hasArrows){P->shootArrow();}}
                  else if(collide){  P->facePlayer("up");}
                  collide = false;
                  ammo = false;
                  moveTimer->Reset();
          break;

          case GLUT_KEY_DOWN:
                  if(P->checkQuiver()){hasArrows = true;}
                  else{hasArrows = false;}
                  if(matrix[P->getPlayerLoc().x][P->getPlayerLoc().y-1] == 1){collide = true;}
                  if(matrix[P->getPlayerLoc().x][P->getPlayerLoc().y-1] == 5){ammo = true;}
                  if(matrix[P->getPlayerLoc().x][P->getPlayerLoc().y-1] == 4){win = true;}
                  if(!shoot && !collide){
                      matrix[P->getPlayerLoc().x][P->getPlayerLoc().y] = 0;
                      P->movePlayer("down");
                      if(ammo){P->addToQuiver(3); M->collectArrows();}
                      matrix[P->getPlayerLoc().x][P->getPlayerLoc().y] = 3;
                  }
                  else if(shoot){ P->facePlayer("down"); if(hasArrows){P->shootArrow();}}
                  else if(collide){  P->facePlayer("down");}
                  collide = false;
                  ammo = false;
                  moveTimer->Reset();
          break;

          case GLUT_KEY_LEFT:
                  if(P->checkQuiver()){hasArrows = true;}
                  else{hasArrows = false;}
                  if(matrix[P->getPlayerLoc().x-1][P->getPlayerLoc().y] == 1){collide = true;}
                  if(matrix[P->getPlayerLoc().x-1][P->getPlayerLoc().y] == 5){ammo = true;}
                  if(matrix[P->getPlayerLoc().x-1][P->getPlayerLoc().y] == 4){win = true;}
                  if(!shoot && !collide){
                      matrix[P->getPlayerLoc().x][P->getPlayerLoc().y] = 0;
                      P->movePlayer("left");
                      if(ammo){P->addToQuiver(3); M->collectArrows();}
                      matrix[P->getPlayerLoc().x][P->getPlayerLoc().y] = 3;
                  }
                  else if(shoot){ P->facePlayer("left"); if(hasArrows){P->shootArrow();}}
                  else if(collide){  P->facePlayer("left");}
                  collide = false;
                  ammo = false;
                  moveTimer->Reset();
          break;

          case GLUT_KEY_RIGHT:
                  if(P->checkQuiver()){hasArrows = true;}
                  else{hasArrows = false;}
                  if(matrix[P->getPlayerLoc().x+1][P->getPlayerLoc().y] == 1){collide = true;}
                  if(matrix[P->getPlayerLoc().x+1][P->getPlayerLoc().y] == 5){ammo = true;}
                  if(matrix[P->getPlayerLoc().x+1][P->getPlayerLoc().y] == 4){win = true;}
                  if(!shoot && !collide){
                      matrix[P->getPlayerLoc().x][P->getPlayerLoc().y] = 0;
                      P->movePlayer("right");
                      if(ammo){P->addToQuiver(3); M->collectArrows();}
                      matrix[P->getPlayerLoc().x][P->getPlayerLoc().y] = 3;
                  }
                  else if(shoot){ P->facePlayer("right"); if(hasArrows){P->shootArrow();}}
                  else if(collide){ P->facePlayer("right"); }
                  collide = false;
                  moveTimer->Reset();
          break;

        }
    }

   for(int i = 0; i < eIndex; i++){
       if(E[i].live){
           int fromX = E[i].getEnemyLoc().x;
           int fromY = E[i].getEnemyLoc().y;
           GridLoc temp = E[i].moveEnemy(P->getPlayerLoc(), graph);
           matrix[fromX][fromY] = 0;
           matrix[temp.x][temp.y] = 20+i;

           if(P->arrowStatus){
             if(matrix[P->getArrowLoc().x][P->getArrowLoc().y] == matrix[E[i].getEnemyLoc().x][E[i].getEnemyLoc().y]){
                P->arrowStatus = false;
                matrix[temp.x][temp.y] = 0;
                E[i].live = false;
             }
           }

           if(matrix[P->getPlayerLoc().x][P->getPlayerLoc().y] == matrix[temp.x][temp.y])
                exit(0);
       }
   }

  glutPostRedisplay();
}


/* Program entry point */

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);

   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize (800, 800);                //window screen
   glutInitWindowPosition (100, 100);            //window position
   glutCreateWindow ("Maze");                    //program title
   init();

   glutDisplayFunc(display);                     //callback function for display
   glutReshapeFunc(resize);                      //callback for reshape
   glutKeyboardFunc(key);                        //callback function for keyboard
   glutSpecialFunc(Specialkeys);
   glutMouseFunc(mouse);
   glutIdleFunc(idle);
   glutMainLoop();

   return EXIT_SUCCESS;
}
