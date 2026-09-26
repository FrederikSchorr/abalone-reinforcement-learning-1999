#ifndef _LIP_
#define _LIP_

/*

(C) LIPSOFT 1994

	ABA2MENU.C

	Menu Modul fuer ABA2.C

void beispiel (void)
 {

 }

*/


/******** own functions list **********

 main

********* own functions list end ******/




/************* includes ***************/

#include "ABA2.H"
#include <ALLOC.H>
#include <CONIO.H>
#include <STDIO.H>
#include <STRING.H>
#include <DIR.H>

#include <MENUDOS.H>
#include <READ\READSTR.H>
#include <READ\READFL.C>
#include <FILE\FILEFUNK.H>

/********** includes end **************/




/********** defines *******************/

#define 	COL_MENU	LIGHTGRAY
#define		FILE_TOURN	"TOURNAM.AT"

enum {EXT_AI=0, EXT_AG, EXT_AP, EXT_LAST};

/*********** defines end **************/




/********** structures ****************/

struct StructFap
 {
  char szInfo[NUM_INFO+1];
  int nColor;
  int nType;
  COMPPLAYER sim;
 };

struct StructFai
 {
  char szInfo[NUM_INFO+1];
  int nPlayerNum;
  int nKickedWon;
  signed char cField[NUM_STONES+1];
  FIELDPOS pCursor[NUM_PLAYERS];
  int nColor[COL_LAST];
 };

struct StructFag
 {
  char szInfo[NUM_INFO+1];
  char FPlayer[NUM_PLAYERS][9];
  int nPlayerNum, nPlayerAct;
  int nPlayerKicked[NUM_PLAYERS];
  int nPlayerLost[NUM_PLAYERS];
  FIELDPOS pPlayerCursor[NUM_PLAYERS];
  int nMarkNum;
  FIELDPOS pMark[NUM_SEL];
  int nKickedWon;
  signed char cField[NUM_STONES+1];
  int nColor[COL_LAST];
 };

struct StructFai FaiStandard={{0}, 2, 6,
  {ST_BORDER, 0,0,0,0,0, 0,0,0,0,0,0,
  -1,-1,0,0,0,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,1,1,1,-1,-1, 1,1,1,1,1,1, 1,1,1,1,1},
  {15, 47, 1, 1}, {LIGHTGRAY, DARKGRAY, WHITE}};
struct StructFap FapStandard={{0}, LIGHTBLUE, TYPE_HUMAN, {3,
  {5,5,4,4,4, 3,3,3,3}, {1.0,0.95,0.90,0.85,0.80, 0.75,0.70,0.65,0.60},
  {1.0,0.7,0.7,0.7},
  {-7.0,-5.0,-1.0,2.0,5.0, 10.0,
    1000.0,1.0,-3.0,-4.0, 50.0, -3.0, 4.0, 0.001}}};

char *g_szPoints[POI_LAST] = {"Corner", "Border", "1 from border",
  "2 from border", "1 from center", "Center", "Kicked a marble",
  "Neighbour", "Alone", "At border with enemy",
  "Attack enemy", "Being pushed", "Intruder",
  "Tolerance"};

/********** structures end ************/





/*********** global variables ********/

char szFileBuffer[EXT_LAST][LIPSTRMAX], szFileLast[EXT_LAST][13];
char szExt[EXT_LAST][3]= {{"AI"}, {"AG"}, {"AP"}};

/******** global variables end ********/




/************ C functions *************/

int toupper (int ch);

/*********** C functions end **********/




/************ functions def ***********/

/* routines from ABA2.C */
int GamePlay (ABAGAME *, int nWait);
void PlayerInit (ABAGAME *, int nPlayerAct, FIELDPOS *pPlayer, int nColor, \
  int nType);
void aba2flash (int nError, char *sz);
void SimInitStandard (ABAGAME *);

/******** functions def end ***********/




/********** own functions *************/

/** FILE begin ************************/

void MenuFileList (char *szText, int nExt)
 {
  char bDone;
  char szBuffer[80];
  struct ffblk ffblk;
  int fh;
  char szInfo[NUM_INFO+1];

  Blanks (TRUE);
  printf (szText);
  sprintf (szBuffer, "*.%s", szExt[nExt]);
  bDone= findfirst (szBuffer, &ffblk, 0);
  if (bDone) printf (" <NONE>");
  while (!bDone)
   {
    fh= fileopen (ffblk.ff_name, LIPFREAD); /* if error fuck the neighbour */
    fileread (fh, NUM_INFO+1, szInfo);
    fileclose (fh);
    *strchr (ffblk.ff_name, '.')= 0;
    Blanks (TRUE);
    printf ("  %-8s: %s", ffblk.ff_name, szInfo);
    bDone= findnext (&ffblk);
   }
 }

int MenuFileNewLoad (char *szText, int nExt)
/* returns EV_ESC: notthing happened
 * filehandle: file already opened
 */
 {
  int fh;

  while (TRUE)
   {
    Blanks (TRUE);
    printf (szText);
    if (readstring (wherex(), wherey (), 10, LIPCURSEND, \
      szFileBuffer[nExt], TRUE) == LIPERROR)
      return (EV_ESC);
    sprintf (szFileLast[nExt],"%.8s.%s",strupr (szFileBuffer[nExt]), szExt[nExt]);
    if (fileexist (szFileLast[nExt]) != LIPKEINZUGRIFF)
     {
      Blanks (TRUE);
      printf ("  File <%s> already exists !", szFileLast[nExt]);
      continue;
     }
    if ((fh=filecreat (szFileLast[nExt])) == LIPKEINZUGRIFF)
     {
      Blanks (TRUE);
      printf ("  Cannot open <%s> !", szFileLast[nExt]);
      continue;
     }
    break;
   }
  return (fh);
 }


int MenuFileReadLoad (char *szText, int nExt, int length, void *pStruc)
/* EV_ESC / EV_OK */
 {
  int fh;

  while (TRUE)
   {
    Blanks (TRUE);
    printf (szText);
    if (readstring (wherex(), wherey (), 10, LIPCURSEND, szFileBuffer[nExt], \
      TRUE) == LIPERROR)
      return (EV_ESC);
    sprintf (szFileLast[nExt], "%.8s.%s",strupr (szFileBuffer[nExt]), szExt[nExt]);
    if ((fh=fileopen (szFileLast[nExt], LIPFREAD)) == LIPKEINZUGRIFF)
     {
      Blanks (TRUE);
      printf ("  Cannot open <%s> !", szFileLast[nExt]);
      continue;
     }
    if (fileread (fh, length, pStruc) != length)
     {
      Blanks (TRUE);
      printf ("  Cannot read complete data from <%s> !", szFileLast[nExt]);
      continue;
     }
    if (fileclose (fh) != LIPFILEOKAY)
     {
      Blanks (TRUE);
      printf ("  Couldn't close <%s> !", szFileLast[nExt]);
     }
    break;
   }
  return (EV_OK);
 }

int MenuFileWriteLoad (char *szText, int nExt)
/* returns EV_ESC: nothing happened
 * filehandle: file already opened
 */
 {
  int fh;
  char szRead[LIPSTRMAX];

  while (TRUE)
   {
    Blanks (TRUE);
    printf (szText);
    if (readstring (wherex(), wherey (), 10, LIPCURSEND, szFileBuffer[nExt], \
      TRUE) == LIPERROR)
      return (EV_ESC);
    sprintf (szFileLast[nExt],"%.8s.%s",strupr (szFileBuffer[nExt]), szExt[nExt]);
    if (fileexist (szFileLast[nExt]) != LIPKEINZUGRIFF)
     {
      Blanks (TRUE);
      printf ("  <%s> already exists, overwrite ? (y/n): ", szFileLast[nExt]);
      strcpy (szRead, "n");
      readstring (wherex (), wherey (), 4, LIPCURSEND, szRead, FALSE);
      if (toupper(*szRead)=='N') continue;
     }
    if ((fh=filecreat (szFileLast[nExt])) == LIPKEINZUGRIFF)
     {
      Blanks (TRUE);
      printf ("  Cannot open <%s> !", szFileLast[nExt]);
      continue;
     }
    break;
   }
  return (fh);
 }

int MenuFileDelete (char *szText, int nExt)
 {
  char bDel= TRUE;
  char szRead[LIPSTRMAX];

  if (MenuFileReadLoad (szText, nExt, 0, NULL) == EV_ESC)
    bDel= FALSE;

  if (bDel)
   {
    Blanks (TRUE);
    printf ("  Do you want to delete <%s> ? (y/n): ", szFileLast[nExt]);
    strcpy (szRead, "n");
    readstring (wherex (), wherey (), 4, LIPCURSEND, szRead, FALSE);
    if (toupper(*szRead)=='N') bDel= FALSE;
   }
  if (!bDel)
   {
    Blanks (TRUE);
    printf ("File not deleted.");
    return (EV_ESC);
   }
  else
   {
    if (filedelete (szFileLast[nExt]) == LIPKEINZUGRIFF)
     {
      Blanks (TRUE);
      printf ("  Error while deleting <%s>.", szFileLast[nExt]);
     }
    Blanks (TRUE);
    printf ("<%s> deleted.", szFileLast[nExt]);
   }
  return (EV_OK);
 }

/** FILE end **************************/

int MenuColorRead (char *szText, double fDefault)
 {
  int i;

  Blanks (TRUE);
  printf ("Colorpalette: ");
  for (i=0; i<=WHITE; i++)
   {
    textcolor (i);
    cprintf ("%d ", i);
   }
  textcolor (COL_MENU);
  Blanks (TRUE);
  printf (szText);
  return (readfloat (wherex(), wherey(), 5, fDefault, 0, WHITE, 0));
 }

void MenuInfoRead (char *szInfo)
 {
  char szBuffer[LIPSTRMAX];

  Blanks (TRUE);
  printf ("Enter short description:");
  Blanks (TRUE);
  printf ("  ");

  sprintf (szBuffer, "%.*s", NUM_INFO, szInfo);
  readstring (wherex(), wherey (), NUM_INFO+2, LIPCURSEND, szBuffer, TRUE);
  sprintf (szInfo, "%.*s", NUM_INFO, szBuffer);
 }

/*** GAME begin ***********************/
int MenuGameSave (ABAGAME *ag, char *szInfo)
/* EV_ESC / EV_OK */
 {
  char szRead[LIPSTRMAX];
  struct StructFag Fag;
  int i, fh;

  Blanks (TRUE);
  printf ("Do you want to save the Game ? (y/n): ");
  strcpy (szRead, "y");
  readstring (wherex (), wherey (), 4, LIPCURSEND, szRead, FALSE);
  if (toupper(*szRead)=='N') return (EV_ESC);

  MenuInfoRead (szInfo);

  if ((fh= MenuFileWriteLoad ("Save Game to: ", EXT_AG)) == EV_ESC)
   {
    Blanks (TRUE);
    printf ("Game not saved.");
    return (EV_ESC);
   }

  strcpy (Fag.szInfo, szInfo);
  for (i=0; i<ag->nPlayerNum; i++)
   {
    strcpy (Fag.FPlayer[i], ag->player[i].szName);
    Fag.nPlayerKicked[i]= ag->pf.nKicked[i];
    Fag.nPlayerLost[i]= ag->pf.nLost[i];
    Fag.pPlayerCursor[i]= ag->player[i].pCursor;
   }
  Fag.nPlayerNum= ag->nPlayerNum;
  Fag.nPlayerAct= ag->nPlayerAct;
  Fag.nMarkNum= ag->sel.nCount;
  for (i=0; i<ag->sel.nCount; i++) Fag.pMark[i]= ag->sel.p[i];
  memcpy (Fag.cField, ag->pf.cField, NUM_STONES+1);
  for (i=0; i<COL_LAST; i++) Fag.nColor[i]= ag->nColor[i];
  Fag.nKickedWon= ag->nKickedWon;

  if (filewrite (fh, sizeof (Fag), (void *) &Fag) != sizeof (Fag))
   {
    Blanks (TRUE);
    printf ("  Couldn't write to <%s> !", szFileLast[EXT_AG]);
   }
  if (fileclose (fh) != LIPFILEOKAY)
   {
    Blanks (TRUE);
    printf ("  Couldn't close <%s> !", szFileLast[EXT_AG]);
   }

  Blanks (TRUE);
  printf ("Game saved to <%s>.", szFileLast[EXT_AG]);
  return (EV_OK);
 }


int MenuGameModify (void)
/* TRUE */
 {
  int i, fh;
  char szBuffer[LIPSTRMAX];
  struct StructFag Fag;

  MenuFileList ("Saved Games:", EXT_AG);
  if (MenuFileReadLoad ("Modify which Game: ", EXT_AG, sizeof (Fag), \
    (void *) &Fag) ==  EV_ESC)
    return (TRUE);

  MenuFileList ("Existing Players:", EXT_AP);
  for (i=0; i<Fag.nPlayerNum; i++)
   {
    strcpy (szFileBuffer[EXT_AP], Fag.FPlayer[i]);
    sprintf (szBuffer, "Choose Player %d: ", i);
    if (MenuFileReadLoad (szBuffer, EXT_AP, 0, NULL) ==EV_ESC)
      return (TRUE);
    strncpy (Fag.FPlayer[i], szFileBuffer[EXT_AP], 8);
    Fag.FPlayer[i][8]= 0;
   }

  for (i=0; i<COL_LAST; i++)
   {
    sprintf (szBuffer, "Color %d: ", i+1);
    Fag.nColor[i]= MenuColorRead (szBuffer, Fag.nColor[i]);
   }
  MenuInfoRead (Fag.szInfo);

  if ((fh= MenuFileWriteLoad ("Save Game to: ", EXT_AG)) == EV_ESC)
   {
    Blanks (TRUE);
    printf ("Game not saved.");
    return (TRUE);
   }
  if (filewrite (fh, sizeof (Fag), (void *) &Fag) != sizeof (Fag))
   {
    Blanks (TRUE);
    printf ("  Couldn't write to <%s> !", szFileLast[EXT_AG]);
   }
  if (fileclose (fh) != LIPFILEOKAY)
   {
    Blanks (TRUE);
    printf ("  Couldn't close <%s> !", szFileLast[EXT_AG]);
   }

  Blanks (TRUE);
  printf ("Game saved to <%s>.", szFileLast[EXT_AG]);

  return (TRUE);
 }


int MenuGameLoad (void)
/* TRUE */
 {
  int fh, i;
  struct StructFag Fag;
  struct StructFap Fap;
  ABAGAME ag;
  char szBuffer[80];

  MenuFileList ("Saved Games:", EXT_AG);
  if (MenuFileReadLoad ("Choose old Game: ", EXT_AG, sizeof (Fag), (void *) &Fag) == \
    EV_ESC)
    return (TRUE);

  ag.nPlayerNum= Fag.nPlayerNum;
  ag.nPlayerAct= Fag.nPlayerAct;

  for (i=0; i<ag.nPlayerNum; i++)
   {
    sprintf (szBuffer, "%s.AP", Fag.FPlayer[i]);
    if ((fh= fileopen (szBuffer, LIPFREAD)) == LIPKEINZUGRIFF)
     {
      Blanks (TRUE);
      printf ("  <%s> not existing, use 'Modify Game' to change Player", \
	szBuffer);
      return (TRUE);
     }
    if (fileread (fh, sizeof (Fap), (void *) &Fap) != sizeof (Fap))
     {
      Blanks (TRUE);
      printf ("  <%s> is bad, use 'Modify Game' to change Player", \
	szBuffer);
      return (TRUE);
     }
    if (fileclose (fh) != LIPFILEOKAY)
     {
      Blanks (TRUE);
      printf ("  Couldn't close <%s> !", szBuffer);
     }

    ag.pf.nKicked[i]= Fag.nPlayerKicked[i];
    ag.pf.nLost[i]= Fag.nPlayerLost[i];
    ag.player[i].pCursor= Fag.pPlayerCursor[i];
    ag.player[i].nType= Fap.nType;
    ag.player[i].nColor= Fap.nColor;
    strcpy (ag.player[i].szName, Fag.FPlayer[i]);
    ag.player[i].cp= Fap.sim;
   }
  for (i=0; i<COL_LAST; i++) ag.nColor[i]= Fag.nColor[i];
  ag.nKickedWon= Fag.nKickedWon;
  ag.sel.nCount= Fag.nMarkNum;
  for (i=0; i<ag.sel.nCount; i++) ag.sel.p[i]= Fag.pMark[i];
  memcpy (ag.pf.cField, Fag.cField, NUM_STONES+1);

  GamePlay (&ag, WAIT_SHOW);
  MenuGameSave (&ag, Fag.szInfo);

  return (TRUE);
 }

int MenuGameNew (void)
 {
  int i;
  struct StructFai Fai;
  struct StructFap Fap;
  ABAGAME ag;
  char szBuffer[80];

  MenuFileList ("Existing Initialization Files:", EXT_AI);
  if (MenuFileReadLoad ("Choose Initialization File: ", EXT_AI, \
    sizeof (Fai), &Fai) == EV_ESC)
    return (TRUE);

  ag.nPlayerNum= Fai.nPlayerNum;
  ag.nPlayerAct= 0;
  for (i=0; i<COL_LAST; i++) ag.nColor[i]= Fai.nColor[i];
  ag.nKickedWon= Fai.nKickedWon;
  ag.sel.nCount=0;
  memcpy (ag.pf.cField, Fai.cField, NUM_STONES+1);

  MenuFileList ("Existing Players:", EXT_AP);
  for (i=0; i<ag.nPlayerNum; i++)
   {
    sprintf (szBuffer, "Choose Player %d: ", i);
    if (MenuFileReadLoad (szBuffer, EXT_AP, sizeof (Fap), &Fap) == EV_ESC)
      return (TRUE);

    ag.pf.nKicked[i]= 0;
    ag.pf.nLost[i] = 0;
    ag.player[i].nColor= Fap.nColor;
    ag.player[i].nType= Fap.nType;
    ag.player[i].pCursor= Fai.pCursor[i];
    strncpy (ag.player[i].szName, strupr (szFileBuffer[EXT_AP]) , 8);
    ag.player[i].szName[8]= 0;
    ag.player[i].cp= Fap.sim;
   }

  sprintf (szBuffer, "Game for %d players",  ag.nPlayerNum);
  MenuGameSave (&ag, szBuffer);
  GamePlay (&ag, WAIT_SHOW);
  MenuGameSave (&ag, szBuffer);

  return (TRUE);
 }

int MenuGameDelete (void)
/* TRUE */
 {
  MenuFileList ("Saved Games: ", EXT_AG);
  MenuFileDelete ("Delete old Game: ", EXT_AG);
  return (TRUE);
 }

/*** GAME end *************************/


/*** INIT begin ***********************/

int MenuInitEdit (struct StructFai *Fai)
 {
  int i, fh;
  char szBuffer[LIPSTRMAX];


  Blanks (TRUE);
  printf ("How many Players (1-6): ");
  Fai->nPlayerNum= readfloat (wherex(), wherey(), 4, Fai->nPlayerNum, 1, 6, 0);

  Blanks (TRUE);
  printf ("With how many kicked marbles do you win (1-%d): ", NUM_KICKS);
  Fai->nKickedWon= readfloat (wherex(), wherey(), 4, Fai->nKickedWon,
    1, NUM_STONES, 0);

  Blanks (TRUE);
  printf ("Enter now the start board, where 0,1,2,... stand");
  Blanks (TRUE);
  printf ("for Player 0,1,2,... and -1 means empty:");
  for (i=1; i<=NUM_STONES; i++)
   {
    Blanks (TRUE);
    printf ("  Position %2d: ", i);
    Fai->cField[i]= readfloat (wherex(), wherey(), 5, Fai->cField[i],
      -1, Fai->nPlayerNum-1, 0);
   }

  Blanks (TRUE);
  printf ("Where shall the marble cursor be positioned:");
  for (i=0; i<Fai->nPlayerNum; i++)
   {
    Blanks (TRUE);
    printf ("  Marble cursor for Player %d (1-%d): ", i, NUM_STONES);
    Fai->pCursor[i]= readfloat (wherex(), wherey(), 5, Fai->pCursor[i], \
      1, NUM_STONES, 0);
   }
  for (i=0; i<COL_LAST; i++)
   {
    sprintf (szBuffer, "Color %d: ", i+1);
    Fai->nColor[i]= MenuColorRead (szBuffer, Fai->nColor[i]);
   }

  if (!*Fai->szInfo)
    sprintf (Fai->szInfo, "Standard Board for %d Players", Fai->nPlayerNum);
  MenuInfoRead (Fai->szInfo);

  if ((fh= MenuFileWriteLoad ("Save Ini to: ", EXT_AI)) == EV_ESC)
   {
    Blanks (TRUE);
    printf ("Ini not saved.");
    return (EV_ESC);
   }

  if (filewrite (fh, sizeof (*Fai), (void *) Fai) != sizeof (*Fai))
   {
    Blanks (TRUE);
    printf ("  Couldn't write to <%s> !", szFileLast[EXT_AG]);
   }
  if (fileclose (fh) != LIPFILEOKAY)
   {
    Blanks (TRUE);
    printf ("  Couldn't close <%s> !", szFileLast[EXT_AI]);
   }

  Blanks (TRUE);
  printf ("Ini saved to <%s>.", szFileLast[EXT_AI]);
  return (EV_OK);
 }

int MenuInitModify (void)
 {
  struct StructFai Fai;

  MenuFileList ("Existing Initialization Files:", EXT_AI);
  if (MenuFileReadLoad ("Old Ini File: ", EXT_AI, sizeof (Fai), \
    (void *) &Fai) == EV_ESC)
    return (TRUE);

  MenuInitEdit (&Fai);

  return (TRUE);
 }

int MenuInitNew (void)
/* TRUE */
 {
  struct StructFai Fai;

  Fai= FaiStandard;
  MenuInitEdit (&Fai);

  return (TRUE);
 }


int MenuInitDelete (void)
/* TRUE */
 {
  MenuFileList ("Existing Ini Files: ", EXT_AI);
  MenuFileDelete ("Delete old Ini File: ", EXT_AI);
  return (TRUE);
 }

/** INIT end **************************/


/** PLAYER begin **********************/

int MenuPlayerEdit (struct StructFap *Fap)
 {
  int i, fh;

  Fap->nColor= MenuColorRead ("Marbles Color: ", Fap->nColor);

  Blanks (TRUE);
  printf ("Kind of Player (Human:%d, Computer:%d): ", TYPE_HUMAN, TYPE_COMP);
  Fap->nType= readfloat (wherex(), wherey(), 4, Fap->nType, 0, 1, 0);

  if (Fap->nType == TYPE_COMP)
   {
    Blanks (TRUE);
    printf ("How many plies in advance shall I analyze (2-9): ");
    Fap->sim.nDepth= readfloat (wherex(), wherey(), 4,
      Fap->sim.nDepth, 2, 9, 0);

    for (i=0; ; i++)
     {
      Blanks (TRUE);
      printf ("  %d plies in advance: ",i);

      Blanks (TRUE);
      printf ("    How important is this ply (0.0-1.0): ");
      Fap->sim.fDepthFactor[i]= readfloat (wherex(), wherey(), 6,
	Fap->sim.fDepthFactor[i], 0, 1, 2);

      if (i + 1 >= Fap->sim.nDepth) break;

      Blanks (TRUE);
      printf ("    How many different possibilites shall be analyzed (1-%d): ",
	NUM_SIM_MOVES - 8);
      Fap->sim.nMoves[i]= readfloat (wherex(), wherey(), 5,
	Fap->sim.nMoves[i], 1, NUM_SIM_MOVES, 0);
     }

    Blanks (TRUE);
    printf("How important is your own position (0.0-1.0): ");
    for (i = 0; ; i++)
     {
      Fap->sim.fPlayerFactor[i] = readfloat (wherex(), wherey(), 6,
	Fap->sim.fPlayerFactor[i], 0.0, 1.0, 2);

      if (i >= NUM_PLAYERS - 1) break;

      Blanks (TRUE);
      printf("How important is the position of the (next)^%d player (0.0-1.0): ", i + 1);
     }

    Blanks (TRUE);
    printf ("Better leave the following table of points as it is:");
    for (i=0; i<POI_LAST; i++)
     {
      Blanks (TRUE);
      printf ("  %s: ", g_szPoints[i]);
      Fap->sim.fPoints[i]= readfloat (wherex(), wherey(), 8,
	Fap->sim.fPoints[i], -10000, 10000, 3);
     }
   }

  if (!*Fap->szInfo)
   {
    if (Fap->nType == TYPE_HUMAN)
      sprintf (Fap->szInfo, "Human Player");
    else
      sprintf (Fap->szInfo, "Computer Player, calculating %d plies in advance",
	Fap->sim.nDepth);
   }
  MenuInfoRead (Fap->szInfo);

  if ((fh= MenuFileWriteLoad ("Save Player to: ", EXT_AP)) == EV_ESC)
   {
    Blanks (TRUE);
    printf ("Player not saved.");
    return (EV_ESC);
   }

  if (filewrite (fh, sizeof (*Fap), (void *) Fap) != sizeof (*Fap))
   {
    Blanks (TRUE);
    printf ("  Couldn't write to <%s> !", szFileLast[EXT_AP]);
   }
  if (fileclose (fh) != LIPFILEOKAY)
   {
    Blanks (TRUE);
    printf ("  Couldn't close <%s> !", szFileLast[EXT_AP]);
   }

  Blanks (TRUE);
  printf ("Player saved to <%s>.", szFileLast[EXT_AP]);
  return (EV_OK);
 }

int MenuPlayerModify (void)
/* TRUE */
 {
  struct StructFap Fap;

  MenuFileList ("Existing Players:", EXT_AP);
  if (MenuFileReadLoad ("Old Player's name: ", EXT_AP, sizeof (Fap), \
    (void *) &Fap) == EV_ESC)
    return (TRUE);

  MenuPlayerEdit (&Fap);

  return (TRUE);
 }

int MenuPlayerNew (void)
 {
  struct StructFap Fap;

  Fap= FapStandard;
  MenuPlayerEdit (&Fap);

  return (TRUE);
 }

int MenuPlayerDelete (void)
/* TRUE */
 {
  MenuFileList ("Existing Players: ", EXT_AP);
  MenuFileDelete ("Delete old Player: ", EXT_AP);
  return (TRUE);
 }

/** PLAYER end ************************/


int MenuInfo (void)
 {
  printf ("\n");
  Blanks (TRUE);
  printf ("Abalone 2.0   Frederik R. Schorr");
  Blanks (TRUE);
  printf ("If you like the game, are waiting for an updated version");
  Blanks (TRUE);
  printf ("or have a suggestion concerning the simulator, please send");
  Blanks (TRUE);
  printf ("an Email to: frederik@fsmat.htu.tuwien.ac.at");
  return (TRUE);
 }

int MenuTournament (void)
 {
  PLAYER player[NUM_PLAYERS_TOURN];
  int nWon[NUM_PLAYERS_TOURN];
  int nNotWon[NUM_PLAYERS_TOURN];
  int nKicked[NUM_PLAYERS_TOURN];
  int nLost[NUM_PLAYERS_TOURN];

  int i, nPlayers;
  struct StructFai Fai;
  struct StructFap Fap;
  ABAGAME ag;
  PLAYFIELD pf;

  int n, n0, n1;
  char szBuffer[80];

  int fh;
  int nEvent = EV_OK;

  /* Choose ini file */
  MenuFileList ("Existing Initialization Files:", EXT_AI);
  if (MenuFileReadLoad ("Choose Initialization File: ", EXT_AI, \
    sizeof (Fai), &Fai) == EV_ESC)
    return (TRUE);

  ag.nPlayerNum= Fai.nPlayerNum;
  ag.nPlayerAct= 0;
  for (i=0; i<COL_LAST; i++) ag.nColor[i]= Fai.nColor[i];
  ag.nKickedWon= Fai.nKickedWon;
  ag.sel.nCount=0;
  memcpy (ag.pf.cField, Fai.cField, NUM_STONES+1);
  for (n = 0; n < ag.nPlayerNum; n++)
   {
    ag.pf.nKicked[n] = 0;
    ag.pf.nLost[n] = 0;
   }

  /* Select players */
  MenuFileList ("Existing Players:", EXT_AP);
  Blanks (TRUE);
  printf ("This is a %d player game.", ag.nPlayerNum);

  for (i=0; i<NUM_PLAYERS_TOURN; i++)
   {
    sprintf (szBuffer, "Choose Player %d for tournament, end with Escape: ", i);
    if (MenuFileReadLoad (szBuffer, EXT_AP, sizeof (Fap), &Fap) == EV_ESC)
      break;

    player[i].nColor= Fap.nColor;
    player[i].nType= Fap.nType;
    player[i].pCursor= Fai.pCursor[i];
    strncpy (player[i].szName, strupr (szFileBuffer[EXT_AP]) , 8);
    player[i].szName[8]= 0;
    player[i].cp= Fap.sim;

    nWon[i] = nNotWon[i] = nKicked[i] = nLost[i] = 0;
   }
  nPlayers = i;

  /* ? enough players */
  if (nPlayers < ag.nPlayerNum)
   {
    Blanks (TRUE);
    printf("Not enough players for a %d player game", ag.nPlayerNum);
    return TRUE;
   }

  /* Open log file */
  if (fileexist (FILE_TOURN) == LIPFILEOKAY)
    fh = fileopen (FILE_TOURN, LIPFWRITE);
  else
    fh = filecreat (FILE_TOURN);
  if (fh == LIPKEINZUGRIFF)
    aba2flash (ERR_FILE, "Can not open tournament file");
  fileseek (fh, LIPOFFEND, 0);

  fileprintf (fh, "\n\n*** Starting new tournament with %d players *** \n",
    ag.nPlayerNum);

  /* Start tournament */
  pf = ag.pf;
  for (n0 = 0; n0 < nPlayers && nEvent == EV_OK; n0++)
   {
    for (n1 = 0; n1 < nPlayers && nEvent == EV_OK; n1++)
     {
      /* ? same player */
      if (n0 == n1) continue;

      /* Copy field and players */
      ag.pf = pf;
      ag.player[0] = player[n0];
      ag.player[1] = player[n1];
      ag.nPlayerAct = 0;

      /* Do it */
      nEvent = GamePlay (&ag, WAIT_FAST);

      /* Print result to log */
      fileprintf (fh, "%8s %d(%d) :: ", ag.player[0].szName,
	ag.pf.nKicked[0], ag.pf.nLost[0]);
      fileprintf (fh, "%8s %d(%d)\n", ag.player[1].szName,
	ag.pf.nKicked[1], ag.pf.nLost[1]);

      /* Store results */
      nKicked[n0] += ag.pf.nKicked[0];
      nLost[n0]   += ag.pf.nLost[0];
      if (ag.pf.nKicked[0] >= ag.nKickedWon) nWon[n0]++;
      else nNotWon[n0]++;

      nKicked[n1] += ag.pf.nKicked[1];
      nLost[n1]   += ag.pf.nLost[1];
      if (ag.pf.nKicked[1] >= ag.nKickedWon) nWon[n1]++;
      else nNotWon[n1]++;

      /* ? exit */
      if (nEvent != EV_OK)
       {
	fileprintf (fh, "Tournament interrupted\n");
       }
     }
   }

  /* Result */
  fileprintf (fh, "\n");
  for (n = 0; n < nPlayers; n++)
   {
    fileprintf (fh, "%8s : won %d, not won %d, kicked %d, lost %d\n",
      player[n].szName, nWon[n], nNotWon[n], nKicked[n], nLost[n]);
   }

  /* Players preferences */
  fileprintf (fh, "\n");
  for (n = 0; n < nPlayers; n++)
   {
    fileprintf (fh, "%s : depth = %d\n", player[n].szName,
      player[n].cp.nDepth);

    fileprintf (fh, "  Depthfactor, further plies: ");
    for (i = 0; ; i++)
     {
      fileprintf (fh, "%.2f", player[n].cp.fDepthFactor[i]);
      if (i >= player[n].cp.nDepth - 1) break;
      fileprintf (fh, ",%d ", player[n].cp.nMoves[i]);
     }

    fileprintf (fh, "\n  Playerfactor: ");
    for (i = 0; i < ag.nPlayerNum; i++)
      fileprintf (fh, "%.2f ", player[n].cp.fPlayerFactor[i]);

    fileprintf (fh, "\n  Points: ");
    for (i = 0; i < POI_LAST; i++)
      fileprintf (fh, "%.2f ", player[n].cp.fPoints[i]);
    fileprintf (fh, "\n");
   }

  /* Ok */
  fileclose (fh);
  Blanks (TRUE);
  printf ("All results of the tournament are saved in file %s.\n", FILE_TOURN);
  return (TRUE);
 }

void MenuABA2 (void)
 {
  void *menu;
  int i;

  inittrstring ();
  textcolor (COL_MENU);
  for (i=0; i<EXT_LAST; i++) *szFileBuffer[i]= 0;

  menu= MenuInit ("Abalone Simulation");
    MenuChild (5, "Play Game", FuncNix);
      MenuChild (4, "Start new Game", MenuGameNew);
      MenuSister ("Load old Game", MenuGameLoad);
      MenuSister ("Modify old Game", MenuGameModify);
      MenuSister ("Delete old Game", MenuGameDelete);
    MenuSister ("Play Tournament", MenuTournament);
    MenuSister ("Player Database", FuncNix);
      MenuChild (3, "Create new Player", MenuPlayerNew);
      MenuSister ("Modify old Player", MenuPlayerModify);
      MenuSister ("Delete old Player", MenuPlayerDelete);
    MenuSister ("Initalization Files", FuncNix);
      MenuChild (3, "Create new Initialization File", MenuInitNew);
      MenuSister ("Modify Initialization File", MenuInitModify);
      MenuSister ("Delete Initialization File", MenuInitDelete);
    MenuSister ("Info", MenuInfo);
  MenuStart (menu);
 }



/********** own functions end *********/

#endif