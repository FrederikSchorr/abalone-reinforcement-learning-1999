/***********************************
/
/   Game.cpp
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 




/*** includes **********************/
#include "Utl/fstreamExt.h"


/* * * * * * * * * * * * * * * * * */
Game
::Game()
{
  clear();
}


/* * * * * * * * * * * * * * * * * */
Game
::Game(SmartPtrPlayer sp0, SmartPtrPlayer sp1)
{
  PlayerMap player;
  player[0] = sp0;
  player[1] = sp1;

  load(player);
}


/* * * * * * * * * * * * * * * * * */
Game
::Game(const std::string& sFile)
{
  load(sFile);
}


/* * * * * * * * * * * * * * * * * */
Game
::~Game()
{
}


/* * * * * * * * * * * * * * * * * */
void Game
::load(const std::string& sFile)
{
  /* Open file */
  ifstreamExt ifile(Kernel::sDirectoryGame, sFile, EXTENSION_GAME);

  /* Clean players array & board */
  clear();

  /* Read from file */
  ifile >> m_ejectWon >> m_playerNum >> m_playerAct;
  m_liHist.clear();

  for (Pos p = 0; p <= POS_NUM; p++)
    ifile >> m_marble[p];

  /* Players */
  char szBuffer[256];
  for (Count n = 0; n < m_playerNum; n++)
  {
    ifile >> m_kicked[n] >> m_lost[n] >> m_timePlayers[n];
    ifile.eatwhite(); ifile.getline(szBuffer, 255);
    m_mapPlayer[n] = new Player(szBuffer);
  }

  /* History */
  int nHist;
  ifile >> nHist; ifile.ignore();
  if (nHist > 0)
  {
    try
    {
      Situation tempSit(*this);
      Board tmpBoard;
      /* First board */
      ifile.read((char*)&tmpBoard, sizeof(Board));
      tempSit.loadBoard(tmpBoard);
      ifile >> tempSit.m_playerAct; ifile.ignore();
    
      /* Moves */
      History hist;
      FullMove full;
      for (int n = 0; n < nHist; n++)
      {
        ifile.read((char*)&full, sizeof(FullMove));
    
        hist.m_board = (Board)tempSit;
        hist.m_playerAct = tempSit.m_playerAct;
        hist.m_fMove = full;
        m_liHist.push_back(hist);

        tempSit.move(full);
      }
      
      /* Correct history */
      if (!((Board)tempSit).equals(*this) && 
        m_playerAct == tempSit.m_playerAct)
        throw GameE("Incorrect history");
    }
    catch(exception e)
    {
#ifdef _DEBUG
      throw;
#else
      m_liHist.clear();
#endif
    }
  }


  /* Check if file length is correct */
  ifile.checkEnd();

  /* And go! */
  time(&m_timeStart);
}


/* * * * * * * * * * * * * * * * * */
void Game
::save(const std::string& sFile) const
{
  /* Open file */
  ofstreamExt ofile(Kernel::sDirectoryGame, sFile, EXTENSION_GAME);

  /* Write to file */
  ofile << m_ejectWon << " " << m_playerNum << " " << m_playerAct << endl;

  for (Pos p = 0; p <= POS_NUM; p++)
    ofile << m_marble[p] << " ";
  ofile << endl;

  /* Players */
  for (Count n = 0; n < m_playerNum; n++)
  {
    ofile << m_kicked[n] << " " << m_lost[n] << " " << m_timePlayers[n] << " " 
      << m_mapPlayer.find(n)->second->sName().c_str() << endl;
  }

  /* History */
  ofile << (int)m_liHist.size() << " ";
  if (m_liHist.size() > 0)
  {
    /* First board */
    HistoryList::const_iterator it = m_liHist.begin();
    ofile.write((const char*)&it->m_board, sizeof(Board));
    ofile << it->m_playerAct << " ";
    
    /* Moves */
    for (; it != m_liHist.end(); it++)
      ofile.write((const char*)&it->m_fMove, sizeof(FullMove));
  }

  /* Write file length to file */
  ofile.writeEnd();
}




/* * * * * * * * * * * * * * * * * */
bool Game
::bIsHuman() const
{
  return spPlayerAct()->bIsHuman();
}


/* * * * * * * * * * * * * * * * * */
Move Game
::calcMove()
{
  return spPlayerAct()->calcMove(*this);
}


/* * * * * * * * * * * * * * * * * */
FullMove Game
::move(const Move& move)
{
  /* ? still not finished */
  if (playerWon() != MARBLE_EMPTY) throw GameE("Game already finished");

  FullMove fullMove;
  
  /* Check move */
  fullMove = Situation::check(move);

  /* Do move */
  Marble playerAct = m_playerAct;
  Situation::move(fullMove);

  /* Add time to timeaccount */
  time_t timeNow;
  time(&timeNow);
  m_timePlayers[playerAct] += (timeNow - m_timeStart);
  time(&m_timeStart);

  return fullMove;
}


/* * * * * * * * * * * * * * * * * */
FullMove Game
::check(const Move &move) const
{
  return Situation::check(move);
}


/* * * * * * * * * * * * * * * * * */
Marble Game
::playerWon() const
{
  return Situation::playerWon();
}



/* * * * * * * * * * * * * * * * * */
void Game
::cancel()
{
  /* Loop through all players and cancel them */
  PlayerMap::iterator it;
  for (it = m_mapPlayer.begin(); it != m_mapPlayer.end(); it++)
  {
    (*it).second->cancel();
  }
}


/* * * * * * * * * * * * * * * * * */
void Game
::load(const PlayerMap &map)
{
  clear();

  Situation::load(map.size());

  players(map);
  time(&m_timeStart);
}

/* * * * * * * * * * * * * * * * * */
void Game::players(const PlayerMap &map)
{
  if (map.size() != m_playerNum) throw GameE("Number of players incorrect");
  
  m_mapPlayer = map;
}

/* * * * * * * * * * * * * * * * * */
const PlayerMap& Game::players() const
{
  return m_mapPlayer;
}


/* * * * * * * * * * * * * * * * * */
FullMove Game
::takeBackMove()
{
  return Situation::takeBackMove(); 
}


/* * * * * * * * * * * * * * * * * */
void Game
::clear()
{
  Situation::clear();
  m_mapPlayer.clear();
  for (Count n = 0; n < PLAYERS_MAX; n++) m_timePlayers[n] = 0;
  m_timeStart = 0;
}

/* * * * * * * * * * * * * * * * * */
time_t Game::timePlayerAct() const
{
  time_t now;
  time(&now);
  return now - m_timeStart;
}

/* * * * * * * * * * * * * * * * * */
Marble Game
::computerLoop()
{
  while (playerWon() == MARBLE_EMPTY)
  {
    move(calcMove());
  }

  return playerWon();
}

/* * * * * * * * * * * * * * * * * */
const SmartPtrPlayer Game
::spPlayer(Marble m) const
{
  if (m >= m_mapPlayer.size()) throw GameE("Illegal player index");

  return m_mapPlayer.find(m)->second; 
}