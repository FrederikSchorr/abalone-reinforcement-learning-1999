/* defines, includes for ABA2.C, ABA2SIM.C, ABA2GFX.C */

#define 	TRUE		(0==0)
#define		FALSE		(0==1)

#define		NUM_STONES	61
#define		NUM_BORDER	30
#define		NUM_MARK	3
#define		NUM_DIR		6
#define		NUM_ATTACK_DIR	3
#define		NUM_SIM_MOVES	60
#define		NUM_LEVELS	9
#define		NUM_PLAYERS	6
#define		NUM_NAME	9
#define		NUM_TOL		1
#define		NUM_INFO	60

#define		NUM_LONG	((unsigned long)(-1) / 2) -10

#define		ST_EMPTY	(-1)
#define		ST_BORDER	(-2)

#define		CODE_NEIGH	"ABA2 Neighbour Data"

#define 	MS_COMP_MOVE	1500

enum {COL_TEXT=0, COL_EMPTY, COL_MARK, COL_LAST};
enum {TYPE_HUMAN, TYPE_COMP};
enum {EV_ESC=(-10), EV_ERROR, EV_WON, EV_OK, EV_KICKED, EV_NOMOVE, EV_UNDO};
enum {POI_CENTER= 5, POI_KICK= 6, POI_KICK_FACTOR, POI_NEIGH, POI_ALONE, POI_LAST};
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
  int nPlayerKicked[NUM_PLAYERS+1];
  signed long lPointsDiff;
 }
SIMFIELD;

typedef struct
 {
  int nLevels;
  int nLevelCalcNum[NUM_LEVELS];
  float fLevelFactor[NUM_LEVELS];
  signed long lPoints[POI_LAST];
 }
SIMULATION;

typedef struct
 {
  int nKicked;
  int nColor;
  int nType;
  FIELDPOS pCursor;
  char szName[NUM_NAME];
  SIMULATION sim;
 }
PLAYER;

typedef struct
 {
  FIELDPOS pMark[NUM_MARK];
  int nMarkNum;
  int nDir;
  signed long lPoints;
 }
SIMMOVE;

typedef struct
 {
  PLAYER *player;
  int nPlayerNum, nPlayerAct;
  int nColor[COL_LAST];
  int nKickedMax;
  int nMarkNum;
  FIELDPOS pMark[NUM_MARK];
  signed char cField[NUM_STONES+1];
 }
ABAGAME;

/********** structures end ************/

/********* func. dec. *****************/

void aba2flash (int errcode, char *sz);

/********* func. dec. end *************/


