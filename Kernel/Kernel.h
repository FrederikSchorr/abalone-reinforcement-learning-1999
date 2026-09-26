/***********************************
/
/  Kernel.h
/ 
/  Frederik Schorr
/  frederik@fsmat.htu.tuwien.ac.at
/
/  09/98
/  
/***********************************/ 

#ifndef KERNEL_H
#define KERNEL_H


/*** include ***********************/ 
#pragma warning( disable : 4786 )

#include <iostream.h>
#include <fstream.h>
#include <exception>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <time.h>

/*** misc **************************/
ostream &operator<<(ostream &os, std::string s);


/*** typedef ***********************/ 
typedef unsigned char Pos;
typedef signed char Marble;
typedef unsigned char Dir;
typedef signed char Count;


/*** defines ***********************/ 
const Pos     POS_NUM          = 61;
const Dir     DIR_NUM          = 6;
const Count   SEL_MAX          = 3;
const Count   PUSH_ENEMY_MAX   = 2;
const Count   PLAYERS_MAX      = 4;

const char FILE_BOARD_CODE[] =   "Malin Board Data";
const char FILE_END_CODE[] =     "Malin EOF";
const char FILE_BOARD[] =        "Board.ar";
const char FILE_SIM212[] =       "Aba212.ar";
const char FILE_BOUB[] =         "Boub.ar";
const char FILE_CAESAR[] =       "Caesar.ar";
const char FILE_EMIL[] =         "Emil.ar";
const char FILE_FREUD[] =        "Freud.ar";
const char FILE_DEBUG[] =        "Debug.log";
const char FILE_LOG[] =          "Malin.log";
const char FILE_TOURNAMENT[] =   "Tournament.at";
const char FILE_USERGUIDE[] =    "Userguide/index.htm";

const char DIRECTORY_GAME[] =    "Game/";
const char DIRECTORY_PLAYER[] =  "Player/";

const char EXTENSION_GAME[] =    "ag";
const char EXTENSION_PLAYER[] =  "ap";
const char EXTENSION_INIT[] =    "ar";
const char EXTENSION_TRAINING[] ="as";

enum {MARBLE_BORDER = -2, MARBLE_EMPTY = -1, MARBLE_0 = 0, MARBLE_1, 
MARBLE_2, MARBLE_3, MARBLE_4, MARBLE_5};


/*** kernel ***********************/ 
class Game;
class Situation;
class Player;
class Simulation;
class SimHum;


/*** log ***************************/
extern ofstream g_log;

#ifdef _DEBUG
  extern ofstream g_logDebug;
  #define LOG(_o)                         (g_logDebug << _o << endl)
  #define LOG2(_o1, _o2)                  (g_logDebug << _o1 << " " << _o2 << endl)
  #define LOG3(_o1, _o2, _o3)             (g_logDebug << _o1 << " " << _o2 << " " << _o3 << endl)
  #define LOG4(_o1, _o2, _o3, _o4)        (g_logDebug << _o1 << " " << _o2 << " " << _o3 << " " << _o4 << endl)
  #define LOG5(_o1, _o2, _o3, _o4, _o5)   (g_logDebug << _o1 << " " << _o2 << " " << _o3 << " " << _o4 << " " << _o5 << endl)
  
#else
  #define LOG(_o)
  #define LOG2(_o1, _o2)            
  #define LOG3(_o1, _o2, _o3)       
  #define LOG4(_o1, _o2, _o3, _o4)  
  #define LOG5(_o1, _o2, _o3, _o4, _o5)

#endif


/*** exceptions ********************/

class FileE : public exception
{
public: FileE(const std::string& s) : 
  exception(("File error\n" + s).c_str()) { };
};

class MemoryE : public exception
{
public: 
  MemoryE() {}
  MemoryE(const std::string& s) : 
    exception(("Not enough memory\n" + s).c_str()) { };
};

class CancelE : public exception
{
public: 
  CancelE() {}
  CancelE(const std::string& s) : 
  exception(s.c_str()) { };
};

class TimeOutE : public exception
{
public: 
  TimeOutE() {}
  TimeOutE(const std::string& s) : 
  exception(s.c_str()) { };
};


class AsyncE : public exception
{
public: 
  AsyncE() {}
  AsyncE(const std::string& s) : 
  exception(s.c_str()) { };
};

class GeneticE : public exception
{
public: 
  GeneticE() {}
  GeneticE(const std::string& s) : 
    exception(s.c_str()) { };
};





/*** Kernel *************************/ 
class Kernel
{
public: 
  static void init (const std::string& sRootDir);
  static void init ();

public:
  // Random number in [0, nUpper) 
  static int rand(int nUpper);
  static float rand(float lower, float upper);
  static int rand();

public: 
  static Pos next[POS_NUM+1][DIR_NUM];
  static char nBorderDist[POS_NUM + 1];
  static char attack[POS_NUM + 1][4];

  static std::string sDirectoryGame;
  static std::string sDirectoryPlayer;
  static std::string sDirectoryRoot;

private:
  static int m_nRand;
};


#endif
/*** Kernel.h end ******************/
