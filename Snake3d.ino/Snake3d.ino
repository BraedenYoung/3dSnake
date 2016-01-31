#include <Rainbowduino.h>

int VrXPin = A0;
int VrYPin = A1;

//head
int playerZ = 1;
int playerX = 1;
int playerY = 1;

// max length of 10, reset if greater
int tail[][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
int playerLength = 2;

int playerSpeed = 5;
int currentDifficulty = 5;
int playerInput = 0;

// 1 +Z, -1 -Z    
int playerVerticalDirection = 0;
// 0 -X, 1 +Y, 2 +X, 3 -Y,
int playerHorizontalDirection = 0;

int pellet[] = {1,2,1};


void setup() {
   Serial.begin(9600);
   Serial.println("Snake is started");
   
   Rb.init();
}

void loop() 
{
  Rb.blankDisplay(); 

  //player
  movePlayerHead();
 
  paintTail();
  Rb.setPixelZXY(playerZ, playerX, playerY, 0, 0, 0xFF); //uses R, G and B color bytes

  //food
  Rb.setPixelZXY(pellet[0], pellet[1], pellet[2], random(0xFF), random(0xFF), random(0xFF)); //uses R, G and B color bytes

  delay(100); 
}


void movePlayerHead()
{
  int VrY = map(analogRead(VrYPin), 0, 1023, -1, 1); 
  int VrX = map(analogRead(VrXPin), 0, 1023, -1, 1);
  
  //move
  if (playerSpeed <= 0)
  {
    movePlayerTail();
    
    // 0 -X, 3 +Y, 2 +X, 1 -Y,
    if (VrY != 0)
    {
      playerVerticalDirection = playerVerticalDirection + VrY;
      // no inverted 
      if(playerVerticalDirection > 1) playerVerticalDirection = 1;
      if(playerVerticalDirection < -1) playerVerticalDirection = -1;
    }
    else if (VrX != 0 && playerVerticalDirection == 0)
    {
      playerHorizontalDirection = playerHorizontalDirection + VrX;
      if (playerHorizontalDirection > 3) playerHorizontalDirection = 0;
      if (playerHorizontalDirection < 0) playerHorizontalDirection = 3;
    }
    
    if(playerVerticalDirection == 0)
    {
      if (playerHorizontalDirection == 2) playerX = playerX + 1;
      else if (playerHorizontalDirection == 1) playerY = playerY - 1;
      else if (playerHorizontalDirection == 0) playerX = playerX - 1;
      else if (playerHorizontalDirection == 3) playerY = playerY + 1;
    } 
    else if (playerVerticalDirection == 1) playerZ = playerZ + 1;
    else if (playerVerticalDirection == -1) playerZ = playerZ - 1;
    
    playerSpeed = currentDifficulty;
  }   
  playerSpeed = playerSpeed - 1;

  // Wrap around
  if (playerX > 3) playerX = 0;
  if (playerX < 0) playerX = 3;
  if (playerY > 3) playerY = 0;
  if (playerY < 0) playerY = 3;  
  if (playerZ > 3) playerZ = 0;
  if (playerZ < 0) playerZ = 3;

  checkCollision();
}

void movePlayerTail()
{ 
  // Move the rest of of the body
  unsigned int x;
  for (x = playerLength; x > 0; --x)
  {
    tail[x][0] = tail[x-1][0];
    tail[x][1] = tail[x-1][1];
    tail[x][2] = tail[x-1][2];
  }
 // Match the head's current location
  tail[0][0] = playerZ;
  tail[0][1] = playerX;
  tail[0][2] = playerY;
}

void paintTail()
{
  unsigned int x;
  for (x = 0; x < playerLength; ++x)
  {
     Rb.setPixelZXY(tail[x][0], tail[x][1], tail[x][2], 0xFF, 0, 0xFF); //uses R, G and B color bytes
  }
}

void createFood()
{
  pellet[0] = random(0, 4);
  pellet[1] = random(0, 4);
  pellet[2] = random(0, 4);
}

void checkCollision()
{
  // check food collision 
  if (playerZ == pellet[0] && playerX == pellet[1] && playerY == pellet[2] )
  {
    playerLength = playerLength + 1;
    createFood();
  }

  // check tail collision
  unsigned int x;
  for (x = 0; x < playerLength; ++x)
  {
    if (playerZ == tail[x][0] && playerX == tail[x][1] && playerY == tail[x][2] ) 
    {
      gameOver();
    }
  }
}

void gameOver()
{
  unsigned char x,y,z;
  for(z=0;z<4;z++)
  {
     for(y=0;y<4;y++)
     {
        for(x=0;x<4;x++)
        {
          Rb.setPixelZXY(z,x,y,random(0xFF),random(0xFF),random(0xFF));
          delay(10);
        }
     }
  } 
  playerLength = 0;
}

