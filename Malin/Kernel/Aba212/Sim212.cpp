/***********************************
/
/   Sim212.cpp
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 



/*** include ***********************/ 
#include "Sim212.h"
#include "Aba2.h"
#include "../Utl/fstreamExt.h"


namespace aba212
{
  /*** declarations ******************/ 
  int SimMove (ABAGAME *ag, const COMPPLAYER *cp);

  extern FIELDPOS g_pAbaNeigh[NUM_STONES+1][NUM_DIR];
  extern char g_nAbaDirAttack[NUM_STONES+1][NUM_ATTACK_DIR];
  extern unsigned char g_cAbaDistBorder[NUM_STONES+1];
  extern volatile bool g_bCancel;


  /*** globals ***********************/ 



  /*** constants *********************/ 

  #pragma warning( disable : 4305 )
  /* Conversion warning: const double -> float */

  const COMPPLAYER g_cpDefault = {3,
  {5,5,4,4,4, 3,3,3,3}, 
  {0.1,0.2,0.1,0.2,0.1, 0.2,0.1,0.2,0.1},
  {1.0,0.7,0.7,0.7},
  {-7.0,-5.0,-1.0,2.0,5.0, 10.0, 1000.0,3.0,-3.0,0.0,20.0,0.0,0.0,0.01}};
  #pragma warning( default : 4305 )

  const char *g_szPoints[POI_LAST] = {"Corner", "Border", "1 from border",
  "2 from border", "1 from center", "Center", "Kicked a marble",
  "Neighbour", "Alone", "At border with enemy",
  "Attack enemy", "Being pushed", "Intruder", "Tolerance"};
}

using namespace aba212;

/* * * * * * * * * * * * * * * * * */
Sim212
::Sim212(int nDepth)
{
  m_type = Simulation::aba212;
  m_pCp = new COMPPLAYER;
  if (!m_pCp) throw MemoryE();
  *m_pCp = g_cpDefault;
  
  m_pCp->nDepth = nDepth;
  m_pCp->fDepthFactor[nDepth-1] = 1.0;
}


/* * * * * * * * * * * * * * * * * */
Sim212
::Sim212()
{
  m_type = Simulation::aba212;
  m_pCp = new COMPPLAYER;
  if (!m_pCp) throw MemoryE();
}

/* * * * * * * * * * * * * * * * * */
Sim212
::~Sim212()
{
  if (!m_pCp) delete m_pCp;
}



/* * * * * * * * * * * * * * * * * */
void Sim212
::load(istream& is)
{
  is.read((char *)m_pCp, sizeof(COMPPLAYER));
}


/* * * * * * * * * * * * * * * * * */
void Sim212
::save(ostream& os) const
{
  os.write((char *)m_pCp, sizeof(COMPPLAYER));
}



/* * * * * * * * * * * * * * * * * */
void Sim212
::modify(Modify &mod)
{
  char buffer[256];
  mod.println("Aba 212 computer player");
  mod.modify("Search depth (in plies) (2-9)", m_pCp->nDepth, 2, 9);
  
  
  for (int i=0; ; i++)
  {
    sprintf (buffer, "How important is ply #%d (0.0-1.0)", i + 1);
    mod.modify(buffer, m_pCp->fDepthFactor[i], 0.0, 1.0, 2);
    
    if (i >= m_pCp->nDepth - 1) break;
    
    sprintf (buffer, "How many moves shall be analyzed after ply #%d (1-%d)",
      i + 1, NUM_SIM_MOVES - 8);
    mod.modify(buffer, m_pCp->nMoves[i], 1, NUM_SIM_MOVES - 8);
  }
  
  sprintf(buffer, "How important is your own move (0.0-1.0)");
  for (i = 0; ; i++)
  {
    mod.modify(buffer, m_pCp->fPlayerFactor[i], 0.0, 1.0, 2);
    
    if (i >= NUM_PLAYERS - 1) break;
    
    sprintf(buffer, "How important is the move of the (next)^%d player (0.0-1.0)", i + 1);
  }
  
  printf ("Better leave the following table of points as it is ...");
  for (i=0; i<POI_LAST; i++)
  {
    mod.modify(g_szPoints[i], m_pCp->fPoints[i], -10000, 10000, 2);
  }
}



/* * * * * * * * * * * * * * * * * */
bool Sim212
::bIsHuman() const
{ 
  return false; 
}

/* * * * * * * * * * * * * * * * * */
Move Sim212
::calcMove(const Situation& sit)
{ 
  g_bCancel = false;
  /* Translate to aba212 */
  //TRACE("Sim212::calcMove\n");
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
  SimMove(&ag, m_pCp);
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
void Sim212
::init()
{
  std::string sFile = FILE_SIM212;

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
    if (ifile.eof()) throw FileE("Invalid sim212 init file <" + sFile + ">");
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
void Sim212
::cancel()
{
  //TRACE("Sim212::cancel\n");
  g_bCancel = TRUE; 
}

