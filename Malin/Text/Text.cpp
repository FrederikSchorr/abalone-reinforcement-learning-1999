/***********************************
/
/   Text.cpp
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 




/*** includes **********************/

#include <conio.h>
#include <time.h>
#include "../Kernel/Aba212/Sim212.h"

extern "C" 
{ 
#include "menudos.h" 
}



/*** Frame class *******************/

class Frame
{
  /* * * * * * * * * * * * * * * * * */
public: 
  int run()
  {
    
    void *menu;
    
    menu= MenuInit ("Abalone Malin 3.0");
    {
      MenuChild (3, "Play Game", FuncNix);
      {
        MenuChild (4, "Start new Game", MenuGameNew);
        MenuSister ("Load old Game", MenuGameLoad);
        MenuSister ("Modify old Game", FuncNix);
        MenuSister ("Delete old Game", FuncNix);
      }
      MenuSister ("Play Tournament", FuncNix);
      MenuSister ("Player Database", FuncNix);
      {
        MenuChild (3, "Create new Player", FuncNix);
        MenuSister ("Modify old Player", FuncNix);
        MenuSister ("Delete old Player", FuncNix);
      }
    }
    MenuStart (menu);
    return 0;
  }
  
  
  /* * * * * * * * * * * * * * * * * */
public:
  static int MenuGameNew(void)
  {
    /* Create a new 2 player game */
    
    SmartPtrSim spSim = new SimHum();
    SmartPtrPlayer spPlayer0 = 
      new Player(spSim, "Human", "Human Player"); 
    spPlayer0->save();
    
    /*spSim = new Sim212(3);
    spPlayer0 = 
      new Player(spSim, "Aba212_3", "Computer player, 3 plies in advance");
    spPlayer0->save();*/
    
    spSim = new Sim212(7);
    SmartPtrPlayer spPlayer1 = 
      new Player(spSim, "Aba7", "Computer player, 7 plies in advance");
    spPlayer1->save();
    
    Game game(spPlayer0, spPlayer1);
    
    game.save("HumAba7");

    play(&game);
        
    return true;
  }
  

  /* * * * * * * * * * * * * * * * * */
public:
  static int MenuGameLoad(void)
  {
    Game game("MyFirstGame");
    
    play(&game);
    
    game.save("MySecondGame.ag");
    
    return true;
  }
  

  /* * * * * * * * * * * * * * * * * */
private:
  static void play (Game *pGame)
  {
    cout << endl << endl;
    
    /* Main loop */
    Move move;
    FullMove fullMove;
    std::string sMove;
    while (true)
    {
      try
      {
        /* Show board */
        showBoard(pGame);
        
        /* ? Human */
        if (pGame->bIsHuman())
        {
          sMove = askMove(pGame);
          move.set(sMove);
          fullMove = pGame->move(move);
        }
        /* ? Computer */
        else 
        {
          cout << pGame->spPlayerAct()->sName() << " calculating ... " << endl;
          move = pGame->calcMove();
          fullMove = pGame->move(move);
        }
        
        /* Show move */
        showMove(fullMove);

        /* Won the game */
        if (pGame->playerWon() != MARBLE_EMPTY) 
        {
          cout << pGame->spPlayerAct()->sName() << "won the game !" << endl;
          return;
        }
      }
      catch (CancelE e)
      {
        cout << "Game cancelled" << endl;
        return;
      }
      catch (MoveE e)
      {
        cout << "Move error: " << e.what() << "." << endl;
      }
    }
  }
  
    

  /* * * * * * * * * * * * * * * * * */
private: 
  static void showBoard(Game *pGame)
  {
    cout << pGame->sit() << endl;
  }
  
  /* * * * * * * * * * * * * * * * * */
private: 
  static void showMove(FullMove& fullMove)
  {
    cout << "Move: " << fullMove << endl;
  }
  
  /* * * * * * * * * * * * * * * * * */
private: 
  static std::string askMove(Game *pGame)
  {
    
    cout << "Hey " << pGame->spPlayerAct()->sName().c_str() << " (Player " << 
      (int)pGame->playerAct() << "), it is your turn (q to quit): " << flush;
    
    char szBuffer[256] = "";
    cin >> szBuffer;
    
    std::string sMove(szBuffer);
    if (sMove == "q") throw CancelE();
    
    return sMove;
  }
  
  
  };
  
  
  /*** main **************************/
  int main(int argc, char* argv[])
  {
    
    try
    {
      /* Initialize */
      char szBuffer[256];
      _strtime(szBuffer);
      LOG2(endl, endl);
      LOG("/*** ");
      LOG(szBuffer);
      LOG(" **************************/");
      LOG(endl);
      
      Kernel::init(FILE_BOARD);
      Sim212::init(FILE_SIM212);
      cin.setf(ios::skipws);
      
      /* Start framework */
      Frame malin;
      malin.run();
      
      /* Ende gut, alles gut */
      cout << endl;
      return 0;
      
    }
    catch (exception e)
    {
      cout << "\nCaught exception\n" << e.what() << endl;
      getch();
      return 1;
    }
  }
  
  /*** Text.cpp end ******************/