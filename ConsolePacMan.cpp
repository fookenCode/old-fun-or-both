/*
File: ConsolePacMan.cpp
Built: 2011
Author: fookenCode
*/
#include <iostream>
using namespace std;
#include <windows.h>

class PacGame {
      public:
              
      enum MovementDirections { LEFT=0, UP, RIGHT, DOWN, INVALID_DIRECTION };
      enum AIColors { GHOST_BLUE=1, GREEN=10, CYAN, RED, PINK, INVALID_COLOR };
      enum WallGroups { INNER=0, OUTER, BOTH, INVALID_GROUP };
      enum GameState { ATTRACT=0, PAUSED, RUNNING, WINNING };
      struct Entity {
             private:
             int movementSpeed, movementDirection, xPos, yPos;
             public:
             int getMovementSpeed() { return movementSpeed; }
             int getMovementDirection() { return movementDirection; }
             int getXPosition() { return xPos; }
             int getYPosition() { return yPos; }
             
             void setMovementSpeed(int newSpeed) { movementSpeed = newSpeed; }
             void setMovementDirection(int newDirection) { movementDirection = newDirection; }
             void setXPos(int newXPos) { xPos = newXPos; }
             void setYPos(int newYPos) { yPos = newYPos; }
             void updatePos()
             {
                  switch ( movementDirection )
                  {
                         case LEFT:
                              xPos -= movementSpeed;
                              break;
                         case RIGHT:
                              xPos += movementSpeed;
                              break;
                         case UP:
                              yPos -= movementSpeed;
                              break;
                         case DOWN:
                              yPos += movementSpeed;
                              break;
                         Default:
                              break;
                  }
             }
             
      };
      unsigned int lives, score, mapSizeX, mapSizeY;
      char mapStrings[30][37], unalteredMapStrings[30][37], playerCharacterIcons[4], currentPlayerIcon;
      int commandQueue, totalDots, gameState, playerCredits;
      bool switchedSides, creditInserted;
      Entity playerEnt;
      
      PacGame() 
      {
        gameState = ATTRACT;
        creditInserted = false;
        lives = 3;
        score = 0;
        playerCredits = 99;
        mapSizeX = 36;
        mapSizeY = 30;
        commandQueue = -1;
        switchedSides = false;
        sprintf(playerCharacterIcons, "%c%c%c%c", 0x3E, 'V', 0x3C, 0x5E);
        currentPlayerIcon = playerCharacterIcons[LEFT];
        playerEnt.setMovementSpeed(0);
        playerEnt.setMovementDirection(INVALID_DIRECTION);
        playerEnt.setXPos(17);
        playerEnt.setYPos(22);
        char transferMapBuffer[][37] = {
             // Line 1
             { 0xC9, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xBB, 
               0xC9, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xBB},
             // Line 2
             { 0xBA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xBA,
               0xBA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xF9, 0xBA },
             // Line 3
             { 0xBA, 0xFA, 0xDA, 0xC4, 0xC4, 0xC4, 0xBF, 0xFA, 0xDA, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xBF, 0xFA, 0xBA,
               0xBA, 0xFA, 0xDA, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xBF, 0xFA, 0xDA, 0xC4, 0xC4, 0xC4, 0xBF, 0xFA, 0xBA},
             // Line 4
             { 0xBA, 0xFA, 0xB3, 'a', 'a', 'a', 0xB3, 0xFA, 0xB3, 'a', 'a', 'a', 'a', 'a', 0xB3, 0xFA, 0xBA, 0xBA, 0xFA, 0xB3,
               'a', 'a', 'a', 'a', 'a', 'a', 'a', 0xB3, 0xFA, 0xB3, 'a', 'a', 'a', 0xB3, 0xFA, 0xBA},
             // Line 5
             { 0xBA, 0xFA, 0xC0, 0xC4, 0xC4, 0xC4, 0xD9, 0xFA, 0xC0, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xD9, 0xFA, 0xC8, 0xBC, 
               0xFA, 0xC0, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xD9, 0xFA, 0xC0, 0xC4, 0xC4, 0xC4, 0xD9, 0xFA, 0xBA},
             // Line 6
             { 0xBA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA,
               0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xBA},
             // Line 7
             { 0xBA, 0xFA, 0xDA, 0xC4, 0xC4, 0xC4, 0xBF, 0xFA, 0xDA, 0xC4, 0xBF, 0xFA, 0xDA, 0xC4, 0xC4, 0xC4, 0xC4, 
               0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xBF, 0xFA, 0xDA, 0xC4, 0xBF, 0xFA, 0xDA, 0xC4, 0xC4, 0xC4, 0xBF, 0xFA, 0xBA},
             // Line 8
             { 0xBA, 0xFA, 0xB3, 'a', 'a', 'a', 0xB3, 0xFA, 0xB3, 'a', 0xB3, 0xFA, 0xC0, 0xC4, 0xC4, 0xC4, 0xBF, 'a', 0xDA,
               0xC4, 0xC4, 0xC4, 0xC4, 0xD9, 0xFA, 0xB3, 'a', 0xB3, 0xFA, 0xB3, 'a', 'a', 'a', 0xB3, 0xFA, 0xBA},
             // Line 9
             { 0xBA, 0xFA, 0xC0, 0xC4, 0xC4, 0xC4, 0xD9, 0xFA, 0xB3, 'a', 0xB3, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xB3, 'a', 
               0xB3, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xB3, 'a', 0xB3, 0xFA, 0xC0, 0xC4, 0xC4, 0xC4, 0xD9, 0xFA, 0xBA},
             // Line 10
             { 0xBA, 0xF9, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xB3,  'a', 0xC0, 0xC4, 0xC4, 0xC4, 0xBF, ' ', 0xB3, 'a', 
               0xB3, ' ', 0xDA, 0xC4, 0xC4, 0xC4, 0xC4, 0xD9, 'a', 0xB3, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xBA},
             // Line 11
             { 0xC8, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xFA, 0xB3, 'a', 0xDA, 0xC4, 0xC4, 0xC4, 0xD9, ' ', 0xC0, 0xC4, 0xD9, 
               ' ', 0xC0, 0xC4, 0xC4, 0xC4, 0xC4, 0xBF, 'a', 0xB3, 0xFA, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xBC},
             // Line 12
             { ' ', ' ', ' ', ' ', ' ', ' ', ' ', 0xFA, 0xB3, 'a', 0xB3,  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
               ' ', ' ', 0xB3, 'a', 0xB3, 0xFA, ' ', ' ', ' ', ' ', ' ', ' ', ' '},
             // Line 13
             { 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xBB, 0xFA, 0xB3, 'a', 0xB3, ' ', 0xDA, 0xC4, 0xC4, 0xC4, ' ', ' ', ' ', 0xC4,
               0xC4, 0xC4, 0xC4, 0xBF, ' ', 0xB3, 'a', 0xB3, 0xFA, 0xC9, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD},
             // Line 14
             { ' ', ' ', ' ', ' ', ' ', ' ', 0xBA, 0xFA, 0xC0, 0xC4, 0xD9, ' ', 0xB3, ' ', 0x81/*' '*/, 0x98/*' '*/, ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
               0xB3, ' ', 0xC0, 0xC4, 0xD9, 0xFA, 0xBA, ' ', ' ', ' ', ' ', ' ', ' '},
             // Line 15
             { ' ', ' ', ' ', ' ', ' ', ' ', 0xBA, 0xFA, ' ', ' ', ' ', ' ', 0xB3, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
               0xB3, ' ', ' ', ' ', ' ', 0xFA, 0xBA, ' ', ' ', ' ', ' ', ' ', ' '},
             // Line 16
             { 0xC9, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xBC, 0xFA, 0xDA, 0xC4, 0xBF, ' ', 0xC0, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 
               0xC4, 0xC4, 0xC4, 0xD9, ' ', 0xDA, 0xC4, 0xBF, 0xFA, 0xC8, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xBB},
             // Line 17
             { 0xBA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xB3, 'a', 0xB3, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
               ' ', ' ', 0xB3, 'a', 0xB3, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xBA},
             // Line 18
             { 0xBA, 0xFA, 0xDA, 0xC4, 0xC4, 0xC4, 0xBF, 0xFA, 0xB3, 'a', 0xB3, ' ', 0xDA, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 
               0xC4, 0xC4, 0xBF, ' ', 0xB3, 'a', 0xB3, 0xFA, 0xDA, 0xC4, 0xC4, 0xC4, 0xBF, 0xFA, 0xBA},
             // Line 19
             { 0xBA, 0xFA, 0xB3, 'a', 'a', 'a', 0xB3, 0xFA, 0xC0, 0xC4, 0xD9, ' ', 0xC0, 0xC4, 0xC4, 0xC4, 0xBF, 'a', 0xDA, 0xC4, 0xC4, 0xC4, 
               0xC4, 0xD9, ' ', 0xC0, 0xC4, 0xD9, 0xFA, 0xB3, 'a', 'a', 'a', 0xB3, 0xFA, 0xBA},
             // Line 20
             { 0xBA, 0xFA, 0xB3, 'a', 'a', 'a', 0xB3, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xB3, 'a', 0xB3, 0xFA, 0xFA, 0xFA, 
               0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xB3, 'a', 'a', 'a', 0xB3, 0xFA, 0xBA},
             // Line 21
             { 0xBA, 0xFA, 0xB3, 'a', 'a', 'a', 0xB3, 0xFA, 0xDA, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xBF, 0xFA, 0xB3, 'a', 0xB3, 0xFA, 0xDA, 0xC4, 
               0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xBF, 0xFA, 0xB3, 'a', 'a', 'a', 0xB3, 0xFA, 0xBA},
             // Line 22
             { 0xBA, 0xFA, 0xC0, 0xC4, 0xBF, 'a', 0xB3, 0xFA, 0xC0, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xD9, 0xFA, 0xC0, 0xC4, 0xD9, 0xFA, 0xC0,
               0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xD9, 0xFA, 0xB3, 'a', 0xDA, 0xC4, 0xD9, 0xFA, 0xBA},
             // Line 23
             { 0xBA, 0xFA, 0xFA, 0xFA, 0xB3, 'a', 0xB3, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 'a', 0xFA, 0xFA, 0xFA, 0xFA, 
               0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xB3, 'a', 0xB3, 0xFA, 0xFA, 0xF9, 0xBA},
             // Line 24
             { 0xC8, 0xCD, 0xBB, 0xFA, 0xB3, 'a', 0xB3, 0xFA, 0xDA, 0xC4, 0xBF, 0xFA, 0xDA, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 
               0xC4, 0xBF, 0xFA, 0xDA, 0xC4, 0xBF, 0xFA, 0xB3, 'a', 0xB3, 0xFA, 0xC9, 0xCD, 0xBC},
             // Line 25
             { 0xC9, 0xCD, 0xBC, 0xFA, 0xC0, 0xC4, 0xD9, 0xFA, 0xB3, 'a', 0xB3, 0xFA, 0xC0, 0xC4, 0xC4, 0xC4, 0xBF, 'a', 0xDA, 0xC4, 0xC4, 0xC4, 0xC4,
               0xD9, 0xFA, 0xB3, 'a', 0xB3, 0xFA, 0xC0, 0xC4, 0xD9, 0xFA, 0xC8, 0xCD, 0xBB},
             // Line 26
             { 0xBA, 0xF9, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xB3, 'a', 0xB3, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xB3, 'a', 0xB3, 0xFA, 0xFA, 0xFA, 
               0xFA, 0xFA, 0xFA, 0xB3, 'a', 0xB3, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xBA},
             // Line 27
             { 0xBA, 0xFA, 0xDA, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xD9, 'a', 0xC0, 0xC4, 0xC4, 0xC4, 0xBF, 0xFA, 0xB3, 'a', 0xB3, 0xFA, 0xDA, 0xC4, 0xC4, 0xC4, 
               0xC4, 0xD9, 'a', 0xC0, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xBF, 0xFA, 0xBA},
             // Line 28
             { 0xBA, 0xFA, 0xC0, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xD9, 0xFA, 0xC0, 0xC4, 0xD9, 0xFA, 0xC0, 0xC4, 0xC4, 0xC4, 0xC4, 
               0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xD9, 0xFA, 0xBA},
             // Line 29
             { 0xBA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA,
               0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xBA},
             // Line 30
             { 0xC8, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 
               0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xBC},
               };
               
        for (int i = 0; i < mapSizeY; ++i)
        {
            strcpy(mapStrings[i], transferMapBuffer[i]);
            strcpy(unalteredMapStrings[i], transferMapBuffer[i]);
        }
        Reset();
      } // End Constructor
      
      void SidesReset() { switchedSides = false; }
      bool IsWallCharacter(char toCompare, int wallGroupToTest)
      {
           switch (wallGroupToTest)
           {
           case INNER:
                if (toCompare == 'Ú' || toCompare == 'Ä' || toCompare == '¿' ||
                  toCompare == '³' || toCompare == 'À' || toCompare == 'Ù')
               {
                  return true;
               }
                break;
           case OUTER:
                if (toCompare == 'É' || toCompare == 'Í' || toCompare == '»' ||
                  toCompare == 'º' || toCompare == 'È' || toCompare == '¼')
                {
                  return true;
                }
                break;
           case BOTH:
                if (toCompare == 'É' || toCompare == 'Í' || toCompare == '»' ||
                  toCompare == 'º' || toCompare == 'È' || toCompare == '¼' ||
                  toCompare == 'Ú' || toCompare == 'Ä' || toCompare == '¿' ||
                  toCompare == '³' || toCompare == 'À' || toCompare == 'Ù')
               {
                  return true;
               }
                break;
           
           }
           return false;
      }
      
      bool isGameOver() { return (totalDots >0); }
      int getGameState() { return gameState; }
      void Reset()
      {
           commandQueue = -1;
           totalDots = 0;
           switchedSides = false;
           currentPlayerIcon = playerCharacterIcons[LEFT];
           playerEnt.setMovementSpeed(0);
           playerEnt.setMovementDirection(INVALID_DIRECTION);
           playerEnt.setXPos(17);
           playerEnt.setYPos(22);
           for (int i = 0; i < mapSizeY; ++i)
           {
              strcpy(mapStrings[i], unalteredMapStrings[i]);
           }
           mapStrings[22][17] = playerCharacterIcons[LEFT];
           
           for (int i = 0; i < mapSizeY; ++i)
           {
             //cout << "\t\t" << mapStrings[i] << endl;
             for (int j =0; j < mapSizeX; ++j)
             {
                 if (j==0)
                 {
                    cout << "\t\t";
                 }
                 
                 if (mapStrings[i][j] == 'ù' || mapStrings[i][j] == 'ú')
                 {
                    totalDots += 1;
                 }
                 
                 if (IsWallCharacter(mapStrings[i][j], BOTH) || mapStrings[i][j]=='a')
                 {
                    if (mapStrings[i][j] == 'a')
                       mapStrings[i][j] = ' ';
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 26);
                    cout << mapStrings[i][j];
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                 }
                 else
                    cout << mapStrings[i][j];
                 
             }
             cout << endl;
           }
           RenderPlayer(true);
      }
      
      
      void Update()
      {
      }
      
      
      bool CanMoveInSpecifiedDirection(int direction, int xPos, int yPos, int movementSpeed=1)
      {
           bool returnCase = false;
           char spaceToMove;
           switch (direction)
           {
                  case LEFT:
                       if (xPos-movementSpeed <1 && yPos == 11)
                       {
                          returnCase = true;
                          switchedSides = true;
                       }
                       else
                       {
                           spaceToMove = mapStrings[yPos][xPos-movementSpeed];
                           if (spaceToMove == 'ù' || spaceToMove == 'ú' || spaceToMove == ' ')
                           {
                              returnCase = true;
                           }
                       }
                       break;
                  case RIGHT:
                       if (xPos+movementSpeed > mapSizeX-1 && yPos == 11)
                       {
                          returnCase = true;
                          switchedSides = true;
                       }
                       else
                       {    
                           spaceToMove = mapStrings[yPos][xPos+movementSpeed];
                           if (spaceToMove == 'ù' || spaceToMove == 'ú' || spaceToMove == ' ')
                           {
                              returnCase = true;
                           }
                       }
                       break;
                  case UP:
                       spaceToMove = mapStrings[yPos-movementSpeed][xPos];
                       if (spaceToMove == 'ù' || spaceToMove == 'ú' || spaceToMove == ' ')
                       {
                          returnCase = true;
                       }                    
                       break;
                  case DOWN:
                       if (((xPos >=15 && xPos <=18) && yPos == 11))
                       {
                           returnCase = false;
                       }
                       else
                       {
                           spaceToMove = mapStrings[yPos+movementSpeed][xPos];
                           if (spaceToMove == 'ù' || spaceToMove == 'ú' || spaceToMove == ' ')
                           {
                              returnCase = true;
                           }
                       }                    
                       break;
           }
           return returnCase;
      }
      
      void UpdatePlayerCharacter()
      {
           if (playerEnt.getMovementSpeed() <= 0)
           {
              return;
           }
           
           if (totalDots == 0)
           {
              
           }
           
           if (commandQueue != -1)
           {
              int plyrDirection = playerEnt.getMovementDirection();
              if ((commandQueue == UP && plyrDirection == DOWN) || (commandQueue == DOWN && plyrDirection == UP)
                 || (commandQueue == RIGHT && plyrDirection == LEFT) || (commandQueue == LEFT && plyrDirection == RIGHT))
              {
                 commandQueue = -1;
              }
           }
           
           if (commandQueue != -1 && CanMoveInSpecifiedDirection(commandQueue, playerEnt.getXPosition(), playerEnt.getYPosition()))
           {
              playerEnt.setMovementDirection(commandQueue);
              playerEnt.setMovementSpeed(1);
              MoveCharacter();
              commandQueue = -1;
           }
           else if (CanMoveInSpecifiedDirection(playerEnt.getMovementDirection(), playerEnt.getXPosition(), playerEnt.getYPosition()))
           {
              MoveCharacter();
           }
      }
      void UpdatePlayerDirection(int direction)
      {
           if (CanMoveInSpecifiedDirection(direction, playerEnt.getXPosition(), playerEnt.getYPosition()))
           {
              playerEnt.setMovementDirection(direction);
              playerEnt.setMovementSpeed(1);
           }
           else
           {
               commandQueue = direction;
           }
      }
      
      void MoveCharacter()
      {
           int movementDirection = playerEnt.getMovementDirection();
           int movementSpeed = playerEnt.getMovementSpeed();
           int xPos = playerEnt.getXPosition();
           int yPos = playerEnt.getYPosition();
           switch (movementDirection)
           {
                  case LEFT:
                       
                       if (xPos-movementSpeed ==0)
                       {
                           mapStrings[yPos][xPos] = ' ';
                           
                           mapStrings[yPos][mapSizeX-1] = playerCharacterIcons[movementDirection];
                           playerEnt.setXPos(mapSizeX-1); 
                       }
                       else
                       {
                          mapStrings[yPos][xPos] = ' ';
                          if (mapStrings[yPos][xPos-movementSpeed] == 'ú')
                          {
                             totalDots--;
                             score+=10;
                          }
                          if (mapStrings[yPos][xPos-movementSpeed] == 'ù')
                          {
                             totalDots--;
                             score+=50;
                          }
                          mapStrings[yPos][xPos-movementSpeed] = playerCharacterIcons[movementDirection];
                          playerEnt.setXPos(xPos-movementSpeed); 
                       }
                       
                       break;
                  case RIGHT:
                       mapStrings[yPos][xPos] = ' ';
                       if (xPos+movementSpeed > mapSizeX-1)
                       {
                           mapStrings[yPos][0] = playerCharacterIcons[movementDirection];
                           playerEnt.setXPos(0); 
                       }
                       else
                       {
                          if (mapStrings[yPos][xPos+movementSpeed] == 'ú')
                          { 
                            totalDots--;
                            score+=10;
                          }
                          if (mapStrings[yPos][xPos+movementSpeed] == 'ù')
                          {
                             totalDots--;
                             score+=50;
                          }
                          mapStrings[yPos][xPos+movementSpeed] = playerCharacterIcons[movementDirection];
                          playerEnt.setXPos(xPos+movementSpeed); 
                       }
                       break;
                  case UP:
                       mapStrings[yPos][xPos] = ' ';
                       if (mapStrings[yPos-movementSpeed][xPos] == 'ú')
                       {
                          totalDots--;
                          score+=10;
                       }
                       if (mapStrings[yPos-movementSpeed][xPos] == 'ù')
                       {
                          totalDots--;
                          score+=50;
                       }
                       mapStrings[yPos-movementSpeed][xPos] = playerCharacterIcons[movementDirection];
                       playerEnt.setYPos(yPos-movementSpeed);
                       break;
                  case DOWN:
                       mapStrings[yPos][xPos] = ' ';
                       if (mapStrings[yPos+movementSpeed][xPos] == 'ú')
                       {
                          totalDots--;
                          score+=10;
                       }
                       if (mapStrings[yPos+movementSpeed][xPos] == 'ù')
                       {
                          totalDots--;
                          score+=50;
                       }
                       mapStrings[yPos+movementSpeed][xPos] = playerCharacterIcons[movementDirection];
                       playerEnt.setYPos(yPos+movementSpeed);
                       break;
           } 
      } // End MoveCharacter()
      
      void GatherGamePlayInput()
      {
       switch (gameState)
       {
       case ATTRACT:
       {
            if (GetAsyncKeyState(VK_ADD) && creditInserted == false)
            {
               playerCredits++;
               creditInserted = true;
            }
            else if (!GetAsyncKeyState(VK_ADD))
            {
                creditInserted = false;
            }
            
            if (GetAsyncKeyState(VK_NUMPAD1) && playerCredits > 0)
            {
               playerCredits--;
               gameState = RUNNING;
            }
            break;
       }
       case RUNNING:
       {
            if (GetAsyncKeyState(VK_LEFT))
            {
               UpdatePlayerDirection(LEFT);
            }
            if (GetAsyncKeyState(VK_RIGHT))
            {
               UpdatePlayerDirection(RIGHT);
            }
            if (GetAsyncKeyState(VK_UP))
            {
               UpdatePlayerDirection(UP);
            }
            if (GetAsyncKeyState(VK_DOWN))
            {
               UpdatePlayerDirection(DOWN);
            }
            if (totalDots == 0) 
            {
                COORD Position;
                Position.X = 30;
                Position.Y = 16;
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
                cout << "Game Over";
                gameState = WINNING;
            }
            break;
       }
       case WINNING:
       default:
               break;
       };
      }
      
      void Render()
      {
            COORD Position;
            int xPos = playerEnt.getXPosition(), yPos = playerEnt.getYPosition(), direction = playerEnt.getMovementDirection();
            switch(direction)
            {
             case LEFT:
                  if (switchedSides == true)
                  {
                     Position.X = 17;
                     Position.Y = yPos;
                     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
                     cout << mapStrings[yPos][0];
                     Position.X = xPos+16;
                     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
                     RenderPlayer();
                     SidesReset();
                  }
                  else
                  {
                      
                      Position.X = xPos+16;
                      Position.Y = yPos;
                      SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
                      RenderPlayer();
                      if (IsWallCharacter(mapStrings[yPos][xPos+1],BOTH) == false)
                      {
                          cout << mapStrings[yPos][xPos+1];
                      }
                  }
                  break;
             case RIGHT:
                  if (switchedSides == true)
                  {
                     Position.X = (mapSizeX-1)+16;
                     Position.Y = yPos;
                     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
                     cout << mapStrings[yPos][mapSizeX-1];
                     Position.X = xPos+16;
                     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
                     RenderPlayer();
                     SidesReset();
                  }
                  else
                  {
                      if (IsWallCharacter(mapStrings[yPos][xPos-1],BOTH) == true)
                      {
                         RenderPlayer(true);
                      }
                      else
                      {
                          Position.X = xPos+15;
                          Position.Y = yPos;
                          SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
                          cout << mapStrings[yPos][xPos-1];
                          RenderPlayer();
                      }
                  }
                  break;
             case UP:
                  Position.X = xPos+16;
                  Position.Y = yPos;
                  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
                  RenderPlayer();

                  if (IsWallCharacter(mapStrings[yPos+1][xPos],BOTH)==false)
                  {
                    Position.Y = yPos+1;
                    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
                    cout << mapStrings[yPos+1][xPos];
                  }
                  break;
             case DOWN:
                  Position.X = xPos+16;
                  Position.Y = yPos;
                  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
                  RenderPlayer();
                  if (IsWallCharacter(mapStrings[yPos-1][xPos],BOTH)==false)
                  {
                      Position.Y = yPos-1;
                      SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
                      cout << mapStrings[yPos-1][xPos];
                  }
                  break;    
            }
            
            RenderScore();
            RenderLives();
            RenderCredits();
      }
      
      void RenderPlayer(bool moveCursorBeforeRender = false)
      {
           if (moveCursorBeforeRender)
           {
              COORD Position;
              Position.X = playerEnt.getXPosition()+16;
              Position.Y = playerEnt.getYPosition();
              SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
           }
           SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
           cout << mapStrings[playerEnt.getYPosition()][playerEnt.getXPosition()];
           SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
      }
      
      void RenderScore()
      {
           COORD Position;
           Position.X = mapSizeX + 16 + 8;
           Position.Y = 2;
           SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
           cout << "Score";
           Position.X = mapSizeX + 16 + 10;
           Position.Y = 3;
           SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
           cout << score; 
      }
      
      void RenderLives()
      {
           COORD Position;
           Position.X = mapSizeX + 16 + 8;
           Position.Y = 6;
           SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
           cout << "Lives";
           Position.X = mapSizeX + 16 + 9;
           Position.Y = 7;
           SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
           SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
           for (int i = 0; i < lives; ++i)
           {               
               cout << playerCharacterIcons[RIGHT] << ' ';
           }
           SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
      }
      
      void RenderCredits()
      {
           COORD Position;
           Position.X = 5;
           Position.Y = mapSizeY;
           SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
           SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
           cout << "Credits " << playerCredits;
           SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
      }
      
      
};

int main(int args, char *argv)
{
    system("cls");
    //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
    CONSOLE_CURSOR_INFO info;
    HANDLE hOutput = GetStdHandle (STD_OUTPUT_HANDLE);
    COORD bufferSize = {80, 31};
    SMALL_RECT windowDimensions = {0,0,79,30};
    SetConsoleScreenBufferSize(hOutput, bufferSize);
    SetConsoleWindowInfo(hOutput, TRUE, &windowDimensions);
    // turn the cursor off
    info.bVisible = FALSE;
    info.dwSize = 1;
    SetConsoleCursorInfo(hOutput,&info);
    
    
    PacGame myGame;
    int startTime;
    startTime = GetTickCount();
    
    
    
    while ( !GetAsyncKeyState(VK_ESCAPE) && !(myGame.getGameState() == PacGame::RUNNING && !myGame.isGameOver()) )
    {
        myGame.GatherGamePlayInput();
        if (GetTickCount() - startTime > 95)
        {
           startTime = GetTickCount();
           
           // Move Character
           myGame.UpdatePlayerCharacter();
        }
        
        myGame.Render();
    }
    COORD Position;
    Position.X = 21;
    Position.Y = 30;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
    system("PAUSE");
    return EXIT_SUCCESS;
}
