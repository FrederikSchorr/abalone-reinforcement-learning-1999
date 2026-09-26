/* defines, includes for ABA2.C, ABA2SIM.C, ABA2GFX.C */

#define 	TRUE		(0==0)
#define		FALSE		(0==1)

#define		NUM_STONES	61
#define		NUM_BORDER	30
#define		NUM_SEL		3
#define		NUM_DIR		6
#define		NUM_ATTACK_DIR	3
#define		NUM_SIM_MOVES	60
#define		NUM_LEVELS	9
#define		NUM_PLAYERS	4
#define		NUM_NAME	9
#define		NUM_TOL		1
#define		NUM_INFO	60
#define 	NUM_KICKS	20

#define		ST_EMPTY	(-1)
#define		ST_BORDER	(-2)

#define		CODE_NEIGH	"ABA2 Neighbour Data"

enum {COL_TEXT=0, COL_EMPTY, COL_MARK, COL_LAST};
enum {TYPE_HUMAN, TYPE_COMP};
enum {EV_ESC=(-10), EV_ERROR, EV_WON, EV_OK, EV_KICKED, EV_NOMOVE};
enum {POI_BORDER= 0, POI_CENTER= 5, POI_KICKED= 6, POI_LOST,
  POI_NEIGH, POI_ALONE, POI_BORDER_ENEMY, POI_ATTACK,
  POI_PUSH, POI_INTRUDER, POI_LAST};
enum {ERR_FILE=1, ERR_MEM, ERR_GRA, ERR_LOGIC};

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
 }
PLAYFIELD;

typedef struct
 {
  int nDepth;
  int nMoves[NUM_LEVELS];
  float fDepthFactor[NUM_LEVELS];
  float fPlayerFactor[NUM_PLAYERS];
  float fPoints[POI_LAST];
 }
COMPPLAYER;

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
  signed long lPoints;
 }
MOVE;

typedef struct
 {
  PLAYER player[NUM_PLAYERS];
  char nPlayerNum, nPlayerAct;
  int nColor[COL_LAST];
  char nKickedWon;
  SELECTION sel;
  PLAYFIELD pf;
 }
ABAGAME;

typedef struct
 {
  unsigned char pLower, pUpper, pFree;
  int nMovesReg;
  unsigned char cNext[NUM_SIM_MOVES + 1];
  float fValue[NUM_SIM_MOVES + 1];
  PLAYFIELD pf[NUM_SIM_MOVES + 1];
  float fPoints[NUM_PLAYERS][NUM_SIM_MOVES + 1];
 }
SAFE;


/********** structures end ************/

/********* func. dec. *****************/

void aba2flash (int errcode, char *sz);

/********* func. dec. end *************/


