/* defines, includes for ABA2.C, ABA2SIM.C, ABA2GFX.C */

#ifndef BOUB_H
#define BOUB_H

#include <iostream.h>


namespace boub
{

#undef    TRUE
#undef    FALSE
#define 	TRUE		(0==0)
#define		FALSE		(0==1)

#define		NUM_STONES	61
#define		NUM_BORDER	30
#define		NUM_SEL		3
#define		NUM_DIR		6
#define		NUM_ATTACK_DIR	3
#define		NUM_SIM_MOVES	30
#define		NUM_LEVELS	9
#define		NUM_PLAYERS	4
#define		NUM_NAME	9
#define		NUM_TOL		1
#define		NUM_INFO	60
#define 	NUM_KICKS	12
#define		NUM_PLAYERS_TOURN	15
#define		NUM_HIST	(NUM_PLAYERS * 50)

#define		ST_EMPTY	(-1)
#define		ST_BORDER	(-2)

#define		CODE_NEIGH	"ABA2 Neighbour Data"

enum {COL_TEXT=0, COL_EMPTY, COL_MARK, COL_LAST};
enum {TYPE_HUMAN, TYPE_COMP};
enum {EV_ESC=(-10), EV_ERROR, EV_WON, EV_OK, EV_KICKED, EV_NOMOVE, EV_BACKSPACE};
enum {POI_BORDER= 0, POI_CENTER= 5, 
  POI_KICKED, POI_NEIGH, POI_ALONE, POI_BORDER_ENEMY, 
  POI_ATTACK, POI_TOLERANCE, POI_LAST};
enum {ERR_FILE=1, ERR_MEM, ERR_GRA, ERR_LOGIC};
enum {WAIT_USER, WAIT_SHOW, WAIT_FAST};

#if	!defined(__COLORS)
#define __COLORS
enum {
    BLACK = 0,		    /* dark colors */
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHTGRAY,
    DARKGRAY,		    /* light colors */
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE
};
#endif // __COLORS
/********** structures ****************/

typedef unsigned char FIELDPOS;

typedef struct
 {
  signed char cField[NUM_STONES+1];
  char nKicked[NUM_PLAYERS];
  char nLost[NUM_PLAYERS];
  char nWon;
 }
PLAYFIELD;

class COMPPLAYER
 {
public:
  int nDepth;
  int nMoves[NUM_LEVELS];
  float fDepthFactor[NUM_LEVELS];
  float fPlayerFactor[NUM_PLAYERS];
  float fPoints[POI_LAST];

public:
  ostream & write(ostream&) const;
 };


typedef struct
 {
  int nColor;
  int nType;
  FIELDPOS pCursor;
  char szName[NUM_NAME];
  COMPPLAYER cp;
 }
PLAYER;

typedef struct
 {
  FIELDPOS p[NUM_SEL];
  int nCount;
 }
SELECTION;

typedef struct
 {
  FIELDPOS p[NUM_SEL];
  int nCount;
  int nDir;
 }
MOVE;

typedef struct
 {
  int nPlayerAct;
  PLAYFIELD pf;
 }
HISTORY;

typedef struct
 {
  PLAYER player[NUM_PLAYERS];
  char nPlayerNum, nPlayerAct;
  int nColor[COL_LAST];
  char nKickedWon;
  SELECTION sel;
  PLAYFIELD pf;

  int nHistLower, nHistUpper;
  HISTORY hist[NUM_HIST + 1];
 }
ABAGAME;

typedef struct
 {
  int pLower, pUpper;
  int nMoves, nMovesReg;
  int nNext[NUM_SIM_MOVES + 1];
  float fValue[NUM_SIM_MOVES + 1];
  PLAYFIELD pf[NUM_SIM_MOVES + 1];
  float fPoints[NUM_PLAYERS][NUM_SIM_MOVES + 1];
 }
SAFE;


/********** structures end ************/

/********* func. dec. *****************/

void aba2flash (int errcode, char *sz);

/********* func. dec. end *************/


}

#endif //BOUB_H