/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */

#include <Player.h>

Timer *T = new Timer();                             // Set Timer for animation

Player::Player()
{
    //ctor
    playerDir = "down";
    arrowLoc.x = 0.2;
    arrowLoc.y =0.5;
    arrAngle =0;
    arrowStatus = false;
    livePlayer = true;
}

Player::~Player()
{
    //dtor
}

void Player::drawArrow()
{

if(arrowStatus){
   glColor3f(1.0,1.0,1.0);

     if(T->GetTicks()>10)
        {
            if(arrowLoc.x>=-1 && arrowLoc.x<=1)
                arrowLoc.x += t*arrXdir;
            else arrowStatus = false;
            if(arrowLoc.y>=-1 && arrowLoc.y<=1)
                arrowLoc.y += t*arrYdir;
            else arrowStatus = false;
           T->Reset();
        }
    glTranslatef(arrowLoc.x ,arrowLoc.y,0.0);
    glRotated(arrAngle,0,0,1);
    glBindTexture(GL_TEXTURE_2D,arrowTex);
    glScaled(1.0/(float)gridSize,1.0/(float)gridSize,1);

    glBegin(GL_QUADS);
        glTexCoord2f(0,0);
        glVertex3f(1,1,0.0f);

        glTexCoord2f(1,0);
        glVertex3f(-1,1,0.0f);

        glTexCoord2f(1,1);
        glVertex3f(-1,-1,0.0f);

        glTexCoord2f(0,1);
        glVertex3f(1,-1,0.0f);
     glEnd();
  }
}

void Player::shootArrow()
{   if(livePlayer){
    if(!arrowStatus)
    {
      arrowStatus=true;

      if(strcmp(playerDir, "left")==0)
      {
        arrAngle = -90;
        arrXdir=-1;
        arrYdir=0;
        arrowLoc.x = plyLoc.x-unitWidth/2;
        arrowLoc.y = plyLoc.y;
      }
      if(strcmp(playerDir, "right")==0)
      {
        arrAngle =90;
        arrXdir=1;
        arrYdir=0;
        arrowLoc.x = plyLoc.x+unitWidth/2;
        arrowLoc.y = plyLoc.y;
      }
      if(strcmp(playerDir, "up")==0)
      {
        arrXdir=0;
        arrYdir=1;
        arrAngle =180;

        arrowLoc.y = plyLoc.y+unitWidth/2;
        arrowLoc.x = plyLoc.x;
      }
      if(strcmp(playerDir, "down")==0)
      {
        arrXdir=0;
        arrYdir=-1;
        arrAngle= 0;

        arrowLoc.y = plyLoc.y-unitWidth/2;
        arrowLoc.x = plyLoc.x;
      }
     }
    }
}


void Player::loadArrowImage(char* fileName)
{
    arrowTex = TextureLoader(fileName);
}

GridLoc Player::getArrowLoc()
{
   GridLoc val;

    val.x = (int)(ceil((arrowLoc.x +(1-unitWidth))/unitWidth));
    val.y = (int)(ceil((arrowLoc.y +(1-unitWidth))/unitWidth));

   return val;
}


void Player::drawplayer()
{
    if(livePlayer)
    {

   glColor3f(1.0,1.0,1.0);

   glTranslatef(plyLoc.x ,plyLoc.y,0.0);

    glBindTexture(GL_TEXTURE_2D,plyTex);
    glScaled(1.0/(float)gridSize,1.0/(float)gridSize,1);


    glBegin(GL_QUADS);
        glTexCoord2f(xmin,ymin);
        glVertex3f(1,1,0.0f);

        glTexCoord2f(xmax,ymin);
        glVertex3f(-1,1,0.0f);

        glTexCoord2f(xmax,ymax);
        glVertex3f(-1,-1,0.0f);

        glTexCoord2f(xmin,ymax);
        glVertex3f(1,-1,0.0f);
     glEnd();
    }
}


void Player::initPlayer(int gSize,int frams, char *FileName)
{
    frames = frams;
    gridSize = gSize;

    xmax =1/(float)frames;
    ymax =0.25;
    xmin =0;
    ymin =0;

    unitWidth = (float)2/gridSize;
    stepsPerMove = frames;

    T->Start();
    plyLoc.x= -unitWidth;
    plyLoc.y= -unitWidth;

    t= unitWidth/stepsPerMove;

    plyTex = TextureLoader(FileName);
}


void Player::placePlayer(int x, int y)
{
    plyLoc.x =  converter(x,y).x;
    plyLoc.y =  converter(x,y).y;
}

void Player::movePlayer(bool collide,char* dir)
{
  collision = collide;
  if(collision){
        plyLoc.x =  converter(getPlayerLoc().x,getPlayerLoc().y).x;
        plyLoc.y =  converter(getPlayerLoc().x,getPlayerLoc().y).y;
  }
  if(strcmp(dir, "left")==0)
     {    if(strcmp(playerDir, "left")!=0)collision=false;
          playerDir = "left";
          if(T->GetTicks()>10)
          {
              if(plyLoc.x>-1+unitWidth/2)
               {
                  plyLoc.x -= t;
               }
              if(xmax>=1){
                  xmax =1/(float)frames;
                  xmin =0;
              }
              xmin += 1/(float)frames;
              xmax += 1/(float)frames;
              ymin =0.75;
              ymax =1.0;
              T->Reset();
          }
     }

     else if(strcmp(dir, "right")==0)
     {
         if(strcmp(playerDir, "right")!=0)collision=false;
          playerDir = "right";
          if(T->GetTicks()>1)
          {
              if(plyLoc.x<1-unitWidth/2)
             {
               plyLoc.x += t;
             }

              if(xmax>=1){
                  xmax =1/(float)frames;
                  xmin =0;
              }
              xmin +=1/(float)frames;
              xmax +=1/(float)frames;
              ymin =0.5;
              ymax =0.75;
              T->Reset();
          }
     }

     else if(strcmp(dir, "up")==0)
     {
         if(strcmp(playerDir, "up")!=0)collision=false;
          playerDir = "up";
          if(T->GetTicks()>1)
          { if(plyLoc.y< 1-unitWidth/2)
             plyLoc.y += t;
              if(xmax>=1){
                  xmax =1/(float)frames;
                  xmin =0;
              }
              xmin +=1/(float)frames;
              xmax +=1/(float)frames;

              ymin =0.25;
              ymax =0.50;

              T->Reset();
          }
     }

     else if(strcmp(dir, "down")==0)
     {
          if(strcmp(playerDir, "down")!=0)collision=false;
           playerDir = "down";
          if(T->GetTicks()>1)
          {
              if(plyLoc.y > -1+unitWidth/2)
                  if(!collision) plyLoc.y -= t;
              if(xmax>=1){
                  xmax =1/(float)frames;
                  xmin =0;
              }
              xmin +=1/(float)frames;
              xmax +=1/(float)frames;
              ymin =0.0;
              ymax =0.25;
              T->Reset();
          }
     }
}

void Player::facePlayer(char* dir)
{
         if(strcmp(dir, "left")==0)
   {     playerDir = "left";
        if(T->GetTicks()>10)
        {
            if(xmax>=1){
                xmax =1/(float)frames;
                xmin =0;
            }
            xmin += 1/(float)frames;
            xmax += 1/(float)frames;
            ymin =0.75;
            ymax =1.0;
            T->Reset();
        }
   }

   else if(strcmp(dir, "right")==0)
   {
        playerDir = "right";
        if(T->GetTicks()>1)
        {
            if(xmax>=1){
                xmax =1/(float)frames;
                xmin =0;
            }
            xmin +=1/(float)frames;
            xmax +=1/(float)frames;
            ymin =0.5;
            ymax =0.75;
            T->Reset();
        }
   }

   else if(strcmp(dir, "up")==0)
   {
        playerDir = "up";
        if(T->GetTicks()>1)
        {
            if(xmax>=1){
                xmax =1/(float)frames;
                xmin =0;
            }
            xmin +=1/(float)frames;
            xmax +=1/(float)frames;

            ymin =0.25;
            ymax =0.50;

            T->Reset();
        }
   }

   else if(strcmp(dir, "down")==0)
   {
         playerDir = "down";
        if(T->GetTicks()>1)
        {
            if(xmax>=1){
                xmax =1/(float)frames;
                xmin =0;
            }
            xmin +=1/(float)frames;
            xmax +=1/(float)frames;
            ymin =0.0;
            ymax =0.25;
            T->Reset();
        }
   }
}

bool Player::playerCollision(int x, int y)
{
    float Xmin = converter(x,y).x-unitWidth/2;
    float Xmax = converter(x,y).x+unitWidth/2;
    float Ymin = converter(x,y).y-unitWidth/2;
    float Ymax = converter(x,y).y+unitWidth/2;

    if( playerDir == "up"){
       if(plyLoc.y> Ymin-unitWidth/2 && plyLoc.y < Ymax && plyLoc.x> Xmin-unitWidth/4 && plyLoc.x< Xmax+unitWidth/4 )
       {
          collision =true;
          return true;
       }
    }

     if(playerDir == "down"){
        if( plyLoc.y > Ymax && plyLoc.y< Ymax+unitWidth && plyLoc.x> Xmin-unitWidth/4 && plyLoc.x< Xmax+unitWidth/4)
        {   collision =true;
            return true;
        }
    }

    if(playerDir == "left"){
        if( plyLoc.x > Xmax && plyLoc.x < Xmax+unitWidth && plyLoc.y> Ymin-unitWidth/4 && plyLoc.y< Ymax+unitWidth/4)
        {   collision =true;
            return true;
        }
    }

    if(playerDir == "right"){
        if(plyLoc.x > Xmin-unitWidth/2 && plyLoc.x < Xmax && plyLoc.y> Ymin-unitWidth/4 && plyLoc.y< Ymax+unitWidth/4 )
       {   collision =true;
            return true;
      }
    }

 collision = false;
 return false;

}

GridLoc Player::getPlayerLoc()
{
   GridLoc val;

   val.x = (int)(ceil((plyLoc.x +(1-unitWidth))/unitWidth));
   val.y = (int)(ceil((plyLoc.y +(1-unitWidth))/unitWidth));

   return val;

}

loc Player::converter(int x, int y)
{
  loc val;
        x+=1;
        y+=1;

       val.x = -1-unitWidth/2+(unitWidth)*x;
       val.y = -1-unitWidth/2+(unitWidth)*y;
       return val;
}

loc Player::getLoc()
{
    return plyLoc;
}
