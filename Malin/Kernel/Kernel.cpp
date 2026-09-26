/***********************************
/
/   Kernel.cpp
/ 
/   Frederik Schorr
/   frederik@fsmat.htu.tuwien.ac.at
/
/   09/98
/   Malin
/  
/***********************************/ 




/*** includes **********************/
#include "Kernel.h"
#include "Player.h"

#include "Utl/fstreamExt.h"
#include <direct.h>


/*** globals ***********************/
Pos Kernel::next[POS_NUM+1][DIR_NUM];
char Kernel::nBorderDist[POS_NUM + 1];
char Kernel::attack[POS_NUM + 1][4];

std::string Kernel::sDirectoryGame;
std::string Kernel::sDirectoryPlayer;
std::string Kernel::sDirectoryRoot;

int Kernel::m_nRand;

/*** log ***************************/
#ifdef _DEBUG
// Debug log file
ofstream g_logDebug(FILE_DEBUG, ios::out | ios::trunc);
#endif

// Log file
ofstream g_log(FILE_LOG, ios::out | ios::ate);

/*** Table *************************/


/* * * * * * * * * * * * * * * * * */
void Kernel
::init()
{
   /* Current directory */
  char szDir[512];
  _getcwd(szDir, 511);
  init(szDir);
}


/* * * * * * * * * * * * * * * * * */
void Kernel
::init(const std::string& sRoot)
{
  std::string sFile = FILE_BOARD;

  /* Set directories */
  sDirectoryRoot = sRoot;
  if (*sRoot.rend() != '/' && *sRoot.rend() != '\\') 
    sDirectoryRoot += '/';
  
  sDirectoryGame = sDirectoryRoot + DIRECTORY_GAME;
  sDirectoryPlayer = sDirectoryRoot + DIRECTORY_PLAYER;

  /* Open file */
  ifstreamExt ifile(sDirectoryRoot, sFile, EXTENSION_INIT);

  char buffer[256];
  
  /* Check for code */
  ifile.getline(buffer, 256);
  if (strcmp(buffer, FILE_BOARD_CODE) != 0)
    throw FileE("Invalid board init file <" + sFile + ">");
  
  /* Read 'next' table */
  Pos p;
  Dir dir;
  Dir e;
  char *off;
  int nBorder = 0;

  // Border is a absorbing position
  for (dir = 0; dir < DIR_NUM; dir++) Kernel::next[0][dir] = 0;

  // Read ini file
  for (p = 1; p <= POS_NUM; p++)
  {
    if (ifile.eof()) throw FileE("Invalid board init file <" + sFile + ">");
    ifile.getline(buffer, 256);
    off = buffer;

    // Neighbours
    for (dir = 0; dir < DIR_NUM; dir++)
    {
      Kernel::next[p][dir] = atoi(off); off += 3;
    }
      
    // Border distance
    Kernel::nBorderDist[p]= atoi (off); off += 3;
    
    // Attack dir
    bool bBorder = false;
    for (e = 0; e < 3; e++)
    {
      Kernel::attack[nBorder][e+1]= atoi(off); off += 3;
      if (Kernel::attack[nBorder][e+1] < DIR_NUM) bBorder = true;
    }
    if (bBorder) Kernel::attack[nBorder++][0] = p;
  }
  Kernel::attack[nBorder][0] = 0;

  ifile.checkEnd();
  m_nRand = time(NULL);

  Player::init();
}


/* * * * * * * * * * * * * * * * * */
int Kernel
::rand()
{
  ::srand(m_nRand);
  return m_nRand = ::rand();
}


/* * * * * * * * * * * * * * * * * */
int Kernel
::rand(int nUpper)
{
  return rand() % nUpper;
}

/* * * * * * * * * * * * * * * * * */
float Kernel
::rand(float low, float high)
{
  float val = high - low; 
  val *= (float) rand(); 
  val /= (float) RAND_MAX; 
  return low + val;
}


/*** misc **************************/
ostream &operator<<(ostream &os, std::string s)
{
  os << s.c_str();
  return os;
}




