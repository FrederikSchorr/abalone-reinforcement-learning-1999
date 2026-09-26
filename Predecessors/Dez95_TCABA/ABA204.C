#ifndef _LIP_
#define _LIP_

/*

(C) LIPSOFT 1994

ABA2.C

void beispiel (void)
 {

 }

*/


/******** own functions list **********

 main

********* own functions list end ******/




/************* includes ***************/

#include "ABA2.H"
#include <STDLIB.H>
#include <STDIO.H>
#include <STRING.H>
#include <TIME.H>

#include <FILEFUNK.H>

/********** includes end **************/




/********** defines *******************/

#define		FILE_RES	"aba2_1.ar"

/*********** defines end **************/



/*********** global variables ********/

/*
 * pNeigh[pPos][nDir]: neighbour to pPos in direction nDir
 * pCursor: actual position of graohic cursor in the field
 */
FIELDPOS pNeigh[NUM_STONES+1][NUM_DIR];		/* p... position in cField */
FIELDPOS pCursor=0;
unsigned char cDistCenter[NUM_STONES+1];

/* data for simulation */
int nLevelsSim= 3;
long lPointsSim[]= {-7, -4, 0, 1, 3, 4, 1000, 2};
int nLevelCalcNumSim[]= {8, 4, 3, 2, 2, 2};
float fLevelFactorSim[]= {1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

SIMMOVE simPassBack;
int nPlayerSim;

char buffer[80];

/******** global variables end ********/




/************ C functions *************/

int getch (void);
int putch (int ch);
int kbhit (void);
int toupper (int);

/*********** C functions end **********/




/************ functions def ***********/

/* routines from ABA2GFX.C */
void GfxInit (void);
void GfxEnd (void);
void GfxOn (ABAGAME *);
void GfxOff (void);
void GfxMsg (char *, char);
void GfxCursorMove (FIELDPOS);
void GfxStoneMark (FIELDPOS);
void GfxStoneUnMark (FIELDPOS);
void GfxFieldShow (void);
void GfxKickedShow (void);

/* routines from ABA2MENU.C */
void MenuABA2 (void);
void MenuGameInit3 (ABAGAME *ag, int, int, int);

/******** functions def end ***********/




/********** own functions *************/

void aba2flash (int nError, char *sz)
 {

  fprintf (stderr, " ");
  fprintf (stderr, "\n\nAba2");
  if (nError) fprintf (stderr, " Error");
  fprintf (stderr, ": %s", sz);
  if (nError == ERR_FILE)
   {
    fprintf (stderr, "\nFollowing files required:");
    fprintf (stderr, "\n%c %s", 254, FILE_RES);
   }
  fprintf (stderr, "\n");
  exit (nError);
 }

void aba2Init (char *szFileNeigh)
 {
  int i,e,fh;
  char buffer[80];

  if ((fh=fileopen (szFileNeigh,LIPFREAD))==LIPKEINZUGRIFF)
    aba2flash (ERR_FILE, "Opening the Neigh File");
  if (fileread (fh, strlen (CODE_NEIGH), buffer)==LIPKEINZUGRIFF)
    aba2flash (ERR_FILE, "Reading Neigh Code");
  buffer[strlen (CODE_NEIGH)]= 0;
  if (strcmp (CODE_NEIGH,buffer)!=0)
    aba2flash (ERR_FILE, "Comparing Neigh Code");
  buffer[3]=0;
  for (i=1; i<=NUM_STONES; i++)
   {
    if (fileread (fh, 2, buffer)!=2)
      aba2flash (ERR_FILE, "Reading CR/LF");
    for (e=0; e<NUM_DIR; e++)
     {
      if (fileread (fh, 3, buffer)!=3)
	aba2flash (ERR_FILE, "Reading Neigh Data");
      pNeigh[i][e]= atoi (buffer);
     }
    if (fileread (fh, 3, buffer)!=3)
      aba2flash (ERR_FILE, "Reading Distance Data");
    cDistCenter[i]= atoi (buffer);
   }
  if (fileclose (fh)== LIPKEINZUGRIFF)
    aba2flash (ERR_FILE, "Closing Neigh File");
 }



int PlayerKicked (ABAGAME *ag)
/* EV_OK / EV_KICKED / EV_WON */
 {
  if (ag->cField[0] != ST_BORDER)
   {
    ag->player[ag->cField[0]].nKicked++;
    GfxKickedShow ();
    if (ag->player[ag->cField[0]].nKicked == ag->nKickedMax)
     {
      sprintf (buffer, "%s (%d) looses the game !!!", \
	ag->player[ag->cField[0]].szName, ag->cField[0]);
      GfxMsg (buffer, TRUE);
      return (EV_WON);
     }
    else return (EV_KICKED);
   }
  return (EV_OK);
 }


void CursorMoveRel (int nDir)
 {
  if (pNeigh[pCursor][nDir] == 0) return;
  GfxCursorMove (pNeigh[pCursor][nDir]);
 }

void StoneMarkAll (ABAGAME *ag, char bMark)
 {
  int i;

  for (i=0; i<ag->nMarkNum; i++)
   {
    if (bMark) GfxStoneMark (ag->pMark[i]);
    else GfxStoneUnMark (ag->pMark[i]);
   }
  if (!bMark) ag->nMarkNum= 0;
 }


int WaitEvent (ABAGAME *ag)
 {
  signed char buffer[80];
  int i;

  GfxCursorMove (ag->player[ag->nPlayerAct].pCursor);
  while (TRUE)
   {
    ag->player[ag->nPlayerAct].pCursor= pCursor;
    switch (toupper(getch ()))
     {
      case 27:
       {
	return (EV_ESC);
       }
      case '\r':
      case ' ':
       {
	for (i=0; i<ag->nMarkNum; i++)
	 {
	  if (pCursor == ag->pMark[i])
	   {
	    GfxStoneUnMark (pCursor);
	    ag->pMark[i]= ag->pMark[ag->nMarkNum-1];
	    ag->nMarkNum--; i= NUM_STONES;
	    break;
	   }
	 }
	if (i == ag->nMarkNum)
	 {
	  if (ag->nPlayerAct != ag->cField[pCursor])
	   {
	    sprintf (buffer, "You can't mark this stone, %s (%d) !", \
	      ag->player[ag->nPlayerAct].szName, ag->nPlayerAct);
	    GfxMsg (buffer, TRUE);
	   }
	  else if (ag->nMarkNum >= NUM_MARK)
	   {
	    sprintf (buffer, "Already marked %d stones !", NUM_MARK);
	    GfxMsg (buffer, TRUE);
	   }
	  else
	   {
	    GfxStoneMark (pCursor);
	    ag->pMark[ag->nMarkNum]= pCursor;
	    ag->nMarkNum++;
	   }
	 }
	break;
       }
      case '6':
      case 'D':
	CursorMoveRel (0);
	break;
      case '9':
      case '8':
      case 'E':
      case 'W':
	CursorMoveRel (1);
	break;
      case '7':
      case 'Q':
	CursorMoveRel (2);
	break;
      case '4':
      case 'A':
	CursorMoveRel (3);
	break;
      case '1':
      case '2':
      case 'X':
      case 'Y':
      case 'Z':
	CursorMoveRel (4);
	break;
      case '3':
      case 'C':
	CursorMoveRel (5);
	break;
      case 4:
	return (0);
      case 5:
      case 23:
	return (1);
      case 17:
	return (2);
      case 1:
	return (3);
      case 24:
      case 25:
      case 26:
	return (4);
      case 3:
	return (5);
      case 0:
       {
	switch (getch ())
	 {
	  case 77:
	    CursorMoveRel (0);
	    break;
	  case 73:
	  case 72:
	    CursorMoveRel (1);
	    break;
	  case 71:
	    CursorMoveRel (2);
	    break;
	  case 75:
	    CursorMoveRel (3);
	    break;
	  case 79:
	  case 80:
	    CursorMoveRel (4);
	    break;
	  case 81:
	    CursorMoveRel (5);
	    break;
	  case 116:
	    return (0);
	  case 132:
	  case 141:
	    return (1);
	  case 119:
	    return (2);
	  case 115:
	    return (3);
	  case 117:
	  case 145:
	    return (4);
	  case 118:
	    return (5);
	 }
	break;
       }
     }
   }
  return (EV_ERROR);
 }


void PlayTest (ABAGAME *ag)
 {
  GfxOn (ag);
  GfxMsg ("Just fool around ...",FALSE);
  while (WaitEvent (ag) != EV_ESC);
 }


int StoneMarkMove (ABAGAME *ag, int nDir)
/* TRUE / FALSE */
 {
  int i, n, nStoneOwn, nDirRev;
  FIELDPOS *p, pAct;

  p= ag->pMark;
  ag->cField[0]= ST_BORDER;

  for (i=0; i<ag->nMarkNum; i++)
    if (ag->cField[pNeigh[p[i]][nDir]] != ST_EMPTY) break;
  if (i == ag->nMarkNum)
   {
    for (i=0; i<ag->nMarkNum; i++)
      {
       ag->cField[p[i]]= ST_EMPTY;
       ag->cField[pNeigh[p[i]][nDir]]= ag->nPlayerAct;
      }
    return (TRUE);
   }

  if (ag->nMarkNum == 2)
   {
    if (pNeigh[p[0]][nDir]==p[1]) pAct= p[0];
    else if (pNeigh[p[1]][nDir]==p[0]) pAct= p[1];
    else return (FALSE);
   }
  else if (ag->nMarkNum == 3)
   {
    if (pNeigh[p[0]][nDir]==p[1]) pAct= p[0];
    else if (pNeigh[p[2]][nDir]==p[1]) pAct= p[2];
    else return (FALSE);
   }
  else pAct= p[0];

  for (i=0,n=0; i<NUM_MARK; i++,n++)
   {
    if (ag->cField[pAct] != ag->nPlayerAct) break;
    pAct= pNeigh[pAct][nDir];
   }
  if (pAct == 0) return (FALSE);
  nDirRev= (nDir+NUM_DIR/2) % NUM_DIR;
  for (nStoneOwn=i,i=0; i<nStoneOwn; i++,n++)
   {
    if (ag->cField[pAct] == ag->nPlayerAct) return (FALSE);
    if (ag->cField[pAct] == ST_EMPTY || pAct == 0) break;
    pNeigh[pNeigh[pAct][nDir]][nDirRev]= pAct;
    pAct= pNeigh[pAct][nDir];
   }
  if (i >= nStoneOwn) return (FALSE);

  for (; n>0; n--)
   {
    ag->cField[pAct]= ag->cField[pNeigh[pAct][nDirRev]];
    pAct= pNeigh[pAct][nDirRev];
   }
  ag->cField[pAct]= ST_EMPTY;
  return (TRUE);
 }

int StoneMarkLegal (ABAGAME *ag)
 {
  int i,n;
  FIELDPOS *p, pHelp;

  p= ag->pMark;

  if (ag->nMarkNum == 1) return (TRUE);
  else if (ag->nMarkNum == 2)
   {
    for (i=0; i<NUM_DIR; i++)
      if (pNeigh[p[0]][i] == p[1])
	return (TRUE);
    return (FALSE);
   }
  else if (ag->nMarkNum == 3)
   {
    for (n=0; n<NUM_MARK-1; n++)
     {
      for (i=0; i<NUM_MARK-1; i++)
       {
	if (p[i]<p[i+1])
	 {
	  pHelp= p[i];
	  p[i]=p[i+1];
	  p[i+1]=pHelp;
	 }
       }
     }

    for (i=0; i<NUM_DIR; i++)
      if (pNeigh[p[0]][i] == p[1] && pNeigh[p[1]][i] == p[2])
	return (TRUE);
    return (FALSE);
   }
  return (FALSE);
 }


void SimMoveSort (int nSimMove, SIMMOVE *SimMove, char *nSimMoveIndex, \
  char bSmallLast)
 {
  int i,e;

  for (i=NUM_SIM_MOVES*(-1); i<NUM_SIM_MOVES; i++) nSimMoveIndex[i]=i;
  for (i=0; i<nSimMove; i++)
   {
    for (e=i; e>0; e--)
     {
      if ((bSmallLast && SimMove[nSimMoveIndex[e]].lPoints > \
	SimMove[nSimMoveIndex[e-1]].lPoints) || \
	(!bSmallLast && SimMove[nSimMoveIndex[e]].lPoints < \
	SimMove[nSimMoveIndex[e-1]].lPoints))
       {
	nSimMoveIndex[e]= nSimMoveIndex[e-1];
	nSimMoveIndex[e-1]= i;
       }
     }
   }
 }


signed long lSimPoints (ABAGAME *ag)
 {
  signed long l= 0, lSignum=-1;
  int nDir;
  FIELDPOS p;


  for (p=1; p<=NUM_STONES; p++)
   {
    if (ag->cField[p] == nPlayerSim) lSignum=-1;
    else lSignum=1;
    if (ag->cField[p] != ST_EMPTY)
     {
      l-= lSignum * ag->player[nPlayerSim].sim.lPoints[cDistCenter[p]];
      for (nDir=0; nDir<NUM_DIR; nDir++)
       {
	if (ag->cField[pNeigh[p][nDir]] == ag->cField[p])
	  l-= lSignum * ag->player[nPlayerSim].sim.lPoints[POI_NEIGH];
       }
     }
   }
  if (ag->cField[0] != ST_BORDER)
   {
    if (ag->cField[0] == nPlayerSim) lSignum=-1;
    l+= lSignum * ag->player[nPlayerSim].sim.lPoints[POI_KICK];
/*    sprintf (buffer, "player: %d, points: %ld", nPlayerSim, l);
    GfxFieldShow ();
    GfxMsg (buffer, TRUE);
*/   }
  return (l);
 }

signed long lSimMoveCalc (ABAGAME *ag, int nLevelAct)
 {
  FIELDPOS p;
  int nDir;
  SIMMOVE PlatzHalter1[NUM_SIM_MOVES]={{0,0,0}};
  SIMMOVE SimMove[NUM_SIM_MOVES];
  char PlatzHalter2[NUM_SIM_MOVES]={0};
  char nSimMoveIndex[NUM_SIM_MOVES];
  signed int nSimMove= 0, nSimMoveKick= -1;
  signed char cFieldOri[NUM_STONES+1];
  int nHelp, i;
  signed long lReturn, lHelp;
  char bWorstCase= ((nLevelAct%ag->nPlayerNum)!=0);
#ifdef DEBUG
char buffer[80];
#endif

  if (nLevelAct >= ag->player[nPlayerSim].sim.nLevels) return (0L);
  if (ag->player[ag->nPlayerAct].nKicked >= ag->nKickedMax)
   {
    ag->nPlayerAct= (ag->nPlayerAct % ag->nPlayerNum) + 1;
    lHelp=lSimMoveCalc (ag, nLevelAct+1);
    ag->nPlayerAct= ((ag->nPlayerAct+ag->nPlayerNum-2) % ag->nPlayerNum) + 1;
    return (lHelp);
   }

  ag->nMarkNum= 1;
  strncpy (cFieldOri, ag->cField, NUM_STONES+1);

  for (p=1; p<=NUM_STONES; p++)
   {
    if (ag->cField[p] != ag->nPlayerAct) continue;
    for (nDir=0; nDir<NUM_DIR; nDir++)
     {
      ag->pMark[0]= p;
      if (StoneMarkMove (ag, nDir))
       {
	lHelp= lSimPoints (ag);
#ifdef DEBUG
if (nLevelAct == -1)
 {
  GfxFieldShow ();
  sprintf (buffer, "Points: %ld", lHelp);
  GfxMsg (buffer, TRUE);
 }
#endif
	if (ag->cField[0] == ST_BORDER)
	 {
	  if (nSimMove<NUM_SIM_MOVES)
	   {
	    SimMove[nSimMove].nMarkNum=1;
	    SimMove[nSimMove].pMark[0]= p;
	    SimMove[nSimMove].nDir= nDir;
	    SimMove[nSimMove].lPoints= lHelp;
	    nSimMove++;
	   }
	 }
	else
	 {
	  if (nSimMoveKick>=(-1)*NUM_SIM_MOVES)
	   {
	    SimMove[nSimMoveKick].nMarkNum=1;
	    SimMove[nSimMoveKick].pMark[0]= p;
	    SimMove[nSimMoveKick].nDir= nDir;
	    SimMove[nSimMoveKick].lPoints= lHelp;
	    nSimMoveKick--;
	   }
	 }
	strncpy (ag->cField, cFieldOri, NUM_STONES+1);
       }
     }
   }
  SimMoveSort (nSimMove, SimMove, nSimMoveIndex, (nLevelAct%ag->nPlayerNum)==0);
  nSimMove= min (nSimMove, ag->player[nPlayerSim].sim.nLevelCalcNum[nLevelAct]-1);

  if (bWorstCase) lReturn= NUM_LONG;
  else lReturn= NUM_LONG * (-1);

  for (i=nSimMoveKick+1; i<nSimMove; i++)
   {
    ag->nMarkNum=SimMove[nSimMoveIndex[i]].nMarkNum;
    for (nHelp=0; nHelp<ag->nMarkNum; nHelp++)
      ag->pMark[nHelp]= SimMove[nSimMoveIndex[i]].pMark[nHelp];

    StoneMarkMove (ag, SimMove[nSimMoveIndex[i]].nDir);
    ag->nPlayerAct= (ag->nPlayerAct % ag->nPlayerNum) + 1;
    lHelp= (float)SimMove[nSimMoveIndex[i]].lPoints * ag->player[nPlayerSim].sim.fLevelFactor[nLevelAct];
    lHelp+= lSimMoveCalc (ag, nLevelAct+1);

#ifdef DEBUG
if (nLevelAct == -1)
 {
  GfxFieldShow ();
  sprintf (buffer, "Level: %d, Points: %ld", nLevelAct, lHelp);
  GfxMsg (buffer, TRUE);
 }
#endif

    ag->nPlayerAct= ((ag->nPlayerAct+ag->nPlayerNum-2) % ag->nPlayerNum) + 1;
    strncpy (ag->cField, cFieldOri, NUM_STONES+1);

    if (nLevelAct == 0)
     {
      if (simPassBack.nMarkNum == 0 || \
	(random (2) && lHelp+NUM_TOL >= lReturn) || \
	lHelp-NUM_TOL >= lReturn)
	simPassBack= SimMove[nSimMoveIndex[i]];
     }

    if ((bWorstCase && lHelp < lReturn) ||
      (!bWorstCase && lHelp > lReturn))
      lReturn= lHelp;
   }
#ifdef DEBUG
if (nLevelAct == -1)
 {
  sprintf (buffer, "Returning from Level %d with %ld", nLevelAct, lReturn);
  putch (7);
  GfxMsg (buffer, TRUE);
 }
#endif
  return (lReturn);
 }


int GamePlay (ABAGAME *ag)
/* EV_ESC / EV_WON*/
 {
  int nEvent;
  char buffer[80], bEsc= FALSE;
  int i, nPlayerLast=0;

  randomize ();
  GfxOn (ag);
  while (TRUE)
   {
    if (ag->player[ag->nPlayerAct].nKicked >= ag->nKickedMax)
     {
      ag->nPlayerAct= (ag->nPlayerAct % ag->nPlayerNum) + 1;
      continue;
     }
    if (nPlayerLast == ag->nPlayerAct)
     {
      sprintf (buffer, "Hey lonely desperado (%s (%d)), you won !!!", \
	ag->player[nPlayerLast].szName, nPlayerLast);
      GfxMsg (buffer, TRUE);
      GfxOff ();
      return (EV_WON);
     }

    if (ag->player[ag->nPlayerAct].nType == TYPE_HUMAN)
     {
      sprintf (buffer, "It's %s's (%d) move.", \
	ag->player[ag->nPlayerAct].szName, ag->nPlayerAct);
      GfxMsg (buffer, FALSE);

      nEvent= WaitEvent (ag);
      if (nEvent<0)
       {
	GfxOff ();
	return (nEvent);
       }

      if (!StoneMarkLegal (ag))
	GfxMsg ("Sorry, illegal stone combination", TRUE);
      else if (!StoneMarkMove (ag, nEvent))
	GfxMsg ("Sorry, illegal move", TRUE);
      else
       {
	StoneMarkAll (ag, FALSE);
	GfxFieldShow ();

	PlayerKicked (ag);

        nPlayerLast= ag->nPlayerAct;
        ag->nPlayerAct= (ag->nPlayerAct % ag->nPlayerNum) + 1;
       }
     }
    else /* computer player */
     {
      sprintf (buffer, "Computing %s's (%d) move ...", \
	ag->player[ag->nPlayerAct].szName, ag->nPlayerAct);
      GfxMsg (buffer, FALSE);
      nPlayerLast= ag->nPlayerAct;

      nPlayerSim= ag->nPlayerAct;
      simPassBack.nMarkNum= 0;
      lSimMoveCalc (ag, 0);
      while (kbhit ())
	if (getch () == 27) bEsc= TRUE;
      if (simPassBack.nMarkNum == 0)
       {
	sprintf (buffer, "%s (%d) can't move anymore !!!", \
	  ag->player[ag->nPlayerAct].szName, ag->nPlayerAct);
	GfxMsg (buffer, TRUE);
	ag->nPlayerAct= (ag->nPlayerAct % ag->nPlayerNum) + 1;
	ag->nMarkNum= 0;
	continue;
       }

      ag->nMarkNum= simPassBack.nMarkNum;
      for (i=0; i<ag->nMarkNum; i++) ag->pMark[i]= simPassBack.pMark[i];
      StoneMarkMove (ag, simPassBack.nDir);
      GfxFieldShow ();

      PlayerKicked (ag);

      ag->nMarkNum=0;
      ag->nPlayerAct= (ag->nPlayerAct % ag->nPlayerNum) + 1;

      if (bEsc)
       {
	GfxOff ();
	return (EV_ESC);
       }
     }
   }
  return (EV_ERROR);
 }


void main (void)
 {
  aba2Init (FILE_RES);
  GfxInit ();
  MenuABA2 ();
  GfxEnd ();

  aba2flash (0, "Terminated succesfully");
 }


/********** own functions end *********/

#endif