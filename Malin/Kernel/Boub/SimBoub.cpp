/***********************************
/
/   SimBoub.cpp
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 



/*** include ***********************/ 
#include "SimBoub.h"

#include "../Utl/fstreamExt.h"

//#include <afxwin.h>


/*** declarations ******************/ 
namespace boub
{

  int SimMove (ABAGAME *ag, const COMPPLAYER *cp);

  extern FIELDPOS g_pAbaNeigh[NUM_STONES+1][NUM_DIR];
  extern char g_nAbaDirAttack[NUM_STONES+1][NUM_ATTACK_DIR];
  extern unsigned char g_cAbaDistBorder[NUM_STONES+1];
  extern volatile bool g_bCancel;
}


/*** globals ***********************/ 


using namespace boub;


/* * * * * * * * * * * * * * * * * */
  
/* Conversion warning: const double -> float */
#pragma warning( disable : 4305 )
COMPPLAYER SimBoub::m_cpDefault = {3,
  {5,5,4,4,4, 3,3,3,3}, 
  {0.1,0.2,0.1,0.2,0.1, 0.2,0.1,0.2,0.1},
  {1.0,0.7,0.7,0.7},
  {-7.0, -5.0, -1.0, 2.0, 5.0, 10.0, 
    1000.0, 1.0, -3.0, -1.0, 
    20.0, 0.01}};
#pragma warning( default : 4305 )

char *SimBoub::m_szPoints[boub::POI_LAST] = {"Corner", "Border", "1 from border",
  "2 from border", "1 from center", "Center", 
  "Kicked a marble", "Neighbour", "Alone", "At border with enemy",
  "Attack enemy", "Tolerance"};


/* * * * * * * * * * * * * * * * * */
SimBoub
::SimBoub(int nDepth)
{
  m_type = Simulation::boub;

  m_cp = m_cpDefault;
  
  m_cp.nDepth = nDepth;
  m_cp.fDepthFactor[nDepth-1] = 1.0;
}

/* * * * * * * * * * * * * * * * * */
SimBoub
::SimBoub(COMPPLAYER& cp)
{
  m_type = Simulation::boub;
  m_cp = cp;
}


/* * * * * * * * * * * * * * * * * */
SimBoub
::SimBoub()
{
  m_type = Simulation::boub;
  m_cp = m_cpDefault;
}


/* * * * * * * * * * * * * * * * * */
void SimBoub
::load(istream& is)
{
  is.read((char *)&m_cp, sizeof(COMPPLAYER));
}


/* * * * * * * * * * * * * * * * * */
void SimBoub
::save(ostream& os) const
{
  os.write((char *)&m_cp, sizeof(COMPPLAYER));
}



/* * * * * * * * * * * * * * * * * */
void SimBoub
::modify(Modify &mod)
{
  char buffer[256];
  mod.println("Boub computer player, heuristic & quiesence search");
  mod.modify("Search depth (in plies) (2-9)", m_cp.nDepth, 2, 9);
  
  
  for (int i=0; ; i++)
  {
    sprintf (buffer, "How important is ply #%d (0.0-1.0)", i + 1);
    mod.modify(buffer, m_cp.fDepthFactor[i], 0.0, 1.0, 2);
    
    if (i >= m_cp.nDepth - 1) break;
    
    sprintf (buffer, "How many moves shall be analyzed after ply #%d (1-%d)",
      i + 1, NUM_SIM_MOVES - 8);
    mod.modify(buffer, m_cp.nMoves[i], 1, NUM_SIM_MOVES - 8);
  }
  
  sprintf(buffer, "Importance of your own move (0.0-1.0)");
  for (i = 0; ; i++)
  {
    mod.modify(buffer, m_cp.fPlayerFactor[i], 0.0, 1.0, 2);
    
    if (i >= NUM_PLAYERS - 1) break;
    
    sprintf(buffer, "Importance of the (next)^%d player (0.0-1.0)", i + 1);
  }
  
  for (i=0; i<POI_LAST; i++)
  {
    mod.modify(m_szPoints[i], m_cp.fPoints[i], -10000, 10000, 2);
  }
}



/* * * * * * * * * * * * * * * * * */
bool SimBoub
::bIsHuman() const
{ 
  return false; 
}

/* * * * * * * * * * * * * * * * * */
Move SimBoub
::calcMove(const Situation& sit) 
{ 
  g_bCancel = false;

  /* Translate to boub */
  //TRACE("SimBoub::calcMove\n");
  ABAGAME ag;
  ag.nKickedWon = 6;
  ag.nPlayerAct = sit.m_playerAct;
  ag.nPlayerNum = sit.m_playerNum;
  for (Pos p = 0; p <= POS_NUM; p++) ag.pf.cField[p] = sit.m_marble[p];
  for (Count n = 0; n < PLAYERS_MAX; n++)
  {
    ag.pf.nKicked[n] = sit.m_kicked[n];
    ag.pf.nLost[n] = sit.m_lost[n];
  }
  ag.pf.nWon = ST_EMPTY;
  
  /* History */
  HistoryList::const_reverse_iterator rit;
  int nHist; 
  for (nHist = NUM_HIST - 1, rit = sit.m_liHist.rbegin(); 
  nHist > 0 && rit != sit.m_liHist.rend(); 
  nHist--, rit++)
  {
    ag.hist[nHist-1].nPlayerAct = rit->m_playerAct;
    for (p = 0; p <= POS_NUM; p++) 
      ag.hist[nHist-1].pf.cField[p] = rit->m_board.m_marble[p];
    
    for (n = 0; n < PLAYERS_MAX; n++)
    {
      ag.hist[nHist - 1].pf.nKicked[n] = sit.m_kicked[n];
      ag.hist[nHist - 1].pf.nLost[n] = sit.m_lost[n];
    }
    ag.hist[nHist - 1].pf.nWon = ST_EMPTY;
  }
  ag.nHistLower = nHist;
  ag.nHistUpper = NUM_HIST - 1;

  /* Calculate ... */
  SimMove(&ag, &m_cp);
  if (g_bCancel) 
  {
    g_bCancel = false;
    throw CancelE("Move computation cancelled");
  }
 
  
  /*** Extract move ***/
  Move move;
  Count player = sit.m_playerAct;
  
  /* Look for my moved marble */
  for (p = 0; p <= POS_NUM; p++) 
  {
    if (sit.m_marble[p] == player && ag.pf.cField[p] != player) break;
  }
  if (p == POS_NUM + 1) throw NoMoveFoundE("No moved marble found");
  
  /* Look for direction */
  Pos pMoved = p;
  for (move.m_dir = 0; move.m_dir < DIR_NUM; move.m_dir++)
  {
    p = pMoved;
    for (n = 0; n < 3; n++)
    {
      move.m_pos[n] = p;
      p = Kernel::next[p][move.m_dir];
      if (sit.m_marble[p] != player && ag.pf.cField[p] == player)
      {
        move.m_count = n + 1;
        return move;
      }
    }
  }
  
  throw SimulationE("No correct move found");
}

/* * * * * * * * * * * * * * * * * */
void SimBoub
::init()
{
  std::string sFile = FILE_BOUB;

  /* Open file */
  ifstreamExt ifile(Kernel::sDirectoryRoot, sFile, EXTENSION_INIT);
  
  /* Check for code */
  char buffer[256];
  ifile.getline(buffer, 256);
  if (strcmp(buffer, CODE_NEIGH) != 0)
    throw FileE("Invalid board init file <" + sFile + ">");
  
  /* Read tables */
  char *p;
  int i,e;
  for (i=1; i<=NUM_STONES; i++)
  {
    if (ifile.eof()) throw FileE("Invalid SimBoub init file <" + sFile + ">");
    ifile.getline(buffer, 256);
    p = buffer;
    
    /* Neighbours */
    for (e=0; e<NUM_DIR; e++)
    {
      g_pAbaNeigh[i][e]= atoi (p); p += 3;
    }
    
    /* Border dist */
    g_cAbaDistBorder[i]= atoi (p); p += 3;
    
    /* Attack dir */
    for (e=0; e<NUM_ATTACK_DIR; e++)
    {
      g_nAbaDirAttack[i][e]= atoi (p); p += 3;
    }
  }
  
  ifile.checkEnd();
}



/* * * * * * * * * * * * * * * * * */
void SimBoub
::cancel()
{
  //TRACE("SimBoub::cancel\n");
  g_bCancel = TRUE; 
}