/***********************************
/
/   Tournament.cpp
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   11/98
/   Malin
/  
/***********************************/ 


/*** includes **********************/
#pragma warning (disable : 4786)

#include "Tournament.h"
//#include <afxwin.h>


/*** definition ********************/
typedef std::vector<int> IntVector;
typedef std::vector<time_t> TimeVector;


/*** Tournament ********************/

/* * * * * * * * * * * * * * * * * */
void Tournament
::init(const PlayerMap & players)
{
  m_players = players;
  m_bCancel = false;
}



/* * * * * * * * * * * * * * * * * */
void Tournament
::run()
{
  int playerNum = m_players.size();
  char sz[256];

  if (playerNum < 2) 
    throw GameE("Not enough players for a tournament");
  
  // Init statistics
  IntVector kicked(playerNum, 0);
  IntVector lost(playerNum, 0);
  IntVector wonG(playerNum, 0);
  IntVector lostG(playerNum, 0);
  IntVector plies(playerNum, 0);
  TimeVector timeSum(playerNum, 0);
  g_log << "\n\n*** New tournament ***" << endl;

  // Loop through games
  int p1, p2;
  PlayerMap players;

  try
  {
    for (p1 = 0; p1 < playerNum; p1++)
    {
      for (p2 = 0; p2 < playerNum; p2++)
      {
        // Do not play against yourself
        if (p1 == p2) continue;

        // Initialize game
        players[0] = m_players[p1];
        players[1] = m_players[p2];
        m_game.load(players);

        // Play game
        while (m_game.playerWon() == MARBLE_EMPTY)
        {
          m_game.move(m_game.calcMove());
          if (m_game.nPlies() >= 2000) break;
        }
        m_game.cleanup();

        // Write result
        sprintf(sz, "(%02d:%02d) %16s %d :: %d %-16s (%02d:%02d), %5d", 
          m_game.timePlayers()[0] / 60, m_game.timePlayers()[0] % 60, 
          players[0]->sName().c_str(),
          m_game.fullBoard().m_kicked[0], 
          m_game.fullBoard().m_kicked[1], 
          players[1]->sName().c_str(), 
          m_game.timePlayers()[1] / 60, m_game.timePlayers()[1] % 60,
          m_game.nPlies());
        g_log << sz << endl;

        // Statistics
        kicked[p1]  += m_game.sit().m_kicked[0];
        lost[p1]    += m_game.sit().m_lost[0];
        plies[p1]   += m_game.nPlies() / 2;
        timeSum[p1] += m_game.timePlayers()[0];
      
        kicked[p2]  += m_game.sit().m_kicked[1];
        lost[p2]    += m_game.sit().m_lost[1];
        plies[p2]   += m_game.nPlies() / 2;
        timeSum[p2] += m_game.timePlayers()[1];

        if (m_game.playerWon() == 0) 
        {
          wonG[p1]++;
          lostG[p2]++;
        }
        else if (m_game.playerWon() == 1) 
        {
          wonG[p2]++;
          lostG[p1]++;
        }
      }
    }
  }
  catch(CancelE e)
  {
    g_log << "Tournament cancelled\n";
  }
  catch(exception e)
  {
    g_log << "Unexpected error: " << e.what() << endl;
  }

  // Write final statistics
  g_log << endl;
  int nGames = (playerNum - 1) * 2;
  for (int n = 0; n < playerNum; n++)
  {
    sprintf(sz, "%16s (#%2d): won %3.0f:%3.0f%%=%2d:%2d:%2d, %3d::%3d, (%02d:%02d)=%.3f*%d",
      m_players[n]->sName().c_str(), n + 1, 
      100.0 * (float)wonG[n] / (float)nGames, 100.0 * (float)lostG[n] / (float)nGames, 
      wonG[n], lostG[n], nGames - wonG[n] - lostG[n],
      kicked[n], lost[n], 
      timeSum[n] / 60, timeSum[n] % 60, 
      (float)timeSum[n] / (float)plies[n],
      plies[n] / nGames);
    g_log << sz << endl;
  }

#ifdef STATISTICS
  g_log << endl;
  for (n = 0; n < playerNum; n++)
  {
    sprintf(sz, "%16s (#%2d): ", m_players[n]->sName().c_str(), n + 1);
    g_log << sz << m_players[n]->statistics() << endl;
  }
#endif //STATISTICS

  // Free players
  m_players.clear();
}


/* * * * * * * * * * * * * * * * * */
void Tournament
::cancel()
{
  m_game.cancel();
  m_bCancel = true;
}


