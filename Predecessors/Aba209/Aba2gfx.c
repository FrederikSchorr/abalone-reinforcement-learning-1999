#ifndef _LIP_
#define _LIP_

/*

(C) LIPSOFT 1994

	ABA2GFX.C  gehoert zu ABA2.C

	grafik routinen fuer ABA2.C


void beispiel (void)
 {

 }

*/


/******** own functions list **********


********* own functions list end ******/




/************* includes ***************/

#include <GRAPHICS.H>
#include "ABA2.H"

/********** includes end **************/




/********** defines *******************/

#define		MATH_FACTOR	1.1547

/*********** defines end **************/




/********** structures ****************/


/********** structures end ************/





/*********** global variables ********/

int gDriver=0, gMode;

extern FIELDPOS pAbaCursor;

struct pointtype xyField[NUM_STONES+1];
struct pointtype xyMax, xyDivide, xyChar, xyMsg;
int cxRadius;

signed char cFieldGfx[NUM_STONES+1];
ABAGAME *ag;

/******** global variables end ********/




/************ C functions *************/

int sprintf (char *string,const char *format, ...);
int getch (void);

/*********** C functions end **********/




/************ functions def ***********/

void flash (int, char *);

/******** functions def end ***********/




/********** own functions *************/

void xyPut (int x, int y, char *sz)
 {
  setfillstyle (1,BLACK);
  bar (x * xyChar.x, y * xyChar.y, (x * xyChar.x) + textwidth (sz),\
    (y+1) * xyChar.y);
  outtextxy (x * xyChar.x, y * xyChar.y, sz);
 }

void GfxMsg (char *sz, char bWait)
 {
  char buffer[80], c;

  if (bWait) sprintf (buffer, "%s - Press Return", sz);
  else sprintf (buffer, "%s", sz);

  setfillstyle (1,BLACK);
  bar (1, xyMsg.y, xyMax.x-1, xyMsg.y+ xyChar.y);
  setcolor (WHITE);
  outtextxy ((xyMax.x-textwidth(buffer))/2, xyMsg.y, buffer);

  if (bWait)
   {
    while (TRUE)
     {
      switch (c= getch ())
       {
	case ' ':
	case '\r':
	case 27:
	  break;
	default: continue;
       }
      break;
     }
    bar (1, xyMsg.y, xyMax.x-1, xyMsg.y+ xyChar.y);
   }
 }


void GfxCursorPut (void)
 {
  int nColorAct;

  if (pAbaCursor==0) return;

  if (cFieldGfx[pAbaCursor] == ST_EMPTY) nColorAct= ag->nColor[COL_EMPTY];
  else nColorAct= ag->player[cFieldGfx[pAbaCursor]].nColor;

  setfillstyle (LINE_FILL,nColorAct);
  setcolor (LIGHTGRAY);
  fillellipse (xyField[pAbaCursor].x, xyField[pAbaCursor].y, cxRadius, cxRadius);
 }




void GfxStonePut (FIELDPOS p)
 {
  int nColorAct;

  if (p==0) return;
  else if (p == pAbaCursor)
   {
    GfxCursorPut ();
    return;
   }

  if (cFieldGfx[p] == ST_EMPTY) nColorAct= ag->nColor[COL_EMPTY];
  else nColorAct= ag->player[cFieldGfx[p]].nColor;

  setfillstyle (1,nColorAct);
  setcolor (ag->nColor[COL_TEXT]);
  fillellipse (xyField[p].x, xyField[p].y, cxRadius, cxRadius);
 }

void GfxFieldShow (void)
 {
  int i;

  for (i=1; i<=NUM_STONES; i++)
   {
    if (ag->cField[i]!=cFieldGfx[i])
     {
      cFieldGfx[i]=ag->cField[i];
      GfxStonePut (i);
     }
   }
 }



void GfxStoneMark (FIELDPOS p)
 {
  setfillstyle (SOLID_FILL,ag->nColor[COL_MARK]);
  setcolor (ag->nColor[COL_TEXT]);
  circle (xyField[p].x, xyField[p].y, cxRadius + 3);
  floodfill (xyField[p].x + cxRadius + 1, xyField[p].y, ag->nColor[COL_TEXT]);
 }


void GfxStoneUnMark (FIELDPOS p)
 {
  setfillstyle (SOLID_FILL,BLACK);
  floodfill (xyField[p].x - (cxRadius + 1), xyField[p].y, ag->nColor[COL_TEXT]);
  floodfill (xyField[p].x + (cxRadius + 1), xyField[p].y, ag->nColor[COL_TEXT]);
  setcolor (BLACK);
  circle (xyField[p].x, xyField[p].y, cxRadius + 3);
 }


void GfxCursorMove (FIELDPOS pCursorNew)
 {
  FIELDPOS pCursorOld= pAbaCursor;

  pAbaCursor= pCursorNew;
  GfxStonePut (pCursorOld);
  GfxCursorPut ();
 }



void GfxKickedShow (void)
 {
  int i;
  char buffer[80];

  for (i=1; i<= ag->nPlayerNum; i++)
   {
    setcolor (ag->player[i].nColor);
    sprintf (buffer, "%s (%d)", ag->player[i].szName, i);
    xyPut (1, i+4, buffer);

    setcolor (ag->nColor[COL_TEXT]);
    sprintf (buffer, ": %d", ag->player[i].nKicked);
    xyPut (13,i+4,buffer);
   }
 }


void GfxOn (ABAGAME *agLocal)
 {
  int i,y;
  char buffer[80];

  ag= agLocal;

  setgraphmode (gMode);
  setcolor (ag->nColor[COL_TEXT]);
  rectangle (0, 0, xyMax.x, xyMax.y);
  line (0, xyDivide.y, xyMax.x, xyDivide.y);
  line (xyDivide.x, 0, xyDivide.x, xyDivide.y);

  xyPut (1,2, "Ejected Marbles:");
  sprintf (buffer, " (Loosing with %d)", ag->nKickedMax);
  xyPut (1,3, buffer);
  GfxKickedShow ();
  for (i=1; i<=NUM_STONES; i++) cFieldGfx[i]= ST_BORDER;
  GfxFieldShow ();
  for (i=0; i<ag->nMarkNum; i++) GfxStoneMark (ag->pMark[i]);
  y= (xyMax.y / xyChar.y) - 16;
  xyPut (1,y++,"Use <ARROW KEYS> to");
  xyPut (1,y++,"position the cursor"); y++;
  xyPut (1,y++,"Mark Marbles with");
  xyPut (1,y++,"<SPACE>"); y++;
  xyPut (1,y++,"Move them with");
  xyPut (1,y++,"<CTRL> <ARROW>"); y++;
  xyPut (1,y++,"Take back move");
  xyPut (1,y++,"with <BACKSPACE>"); y++;
  xyPut (1,y++,"Return with <ESC>");
 }

void GfxOff (void)
 {
  restorecrtmode ();
 }

void GfxInit (void)
 {
  float fcx, fcy;
  int xCent, yCent, cx, cy;
  int nPos, nLine, i;

  initgraph (&gDriver, &gMode, "");
  if (graphresult < 0)
    flash (ERR_GRA, "Initgraph failed");

  xyMax.x= getmaxx ();
  xyMax.y= getmaxy ();
  xyChar.x= textwidth ("M");
  xyChar.y= textheight ("M")+ 2;
  xyDivide.x= 21 * xyChar.x;
  xyDivide.y= xyMax.y - 3 * xyChar.y;
  xyMsg.y= xyDivide.y+ xyChar.y;

  GfxOff ();

  fcx= xyMax.x- xyDivide.x;
  fcy= xyDivide.y;
  xCent= xyMax.x - (fcx/2);
  yCent= fcy/2;
  if (fcx/MATH_FACTOR > fcy) fcx= fcy*MATH_FACTOR;
  else fcy= fcx/MATH_FACTOR;
  fcx/=20; fcy/=10;
  cx= fcx; cy= fcy;
  cxRadius= cx-4;
  for (nPos=27,i=-4; nPos<=35; nPos++,i++)
   {
    xyField[nPos].x= xCent+ (i*2*cx);
    xyField[nPos].y= yCent;
   }

  for (nLine=1,nPos=1; nLine<=4; nLine++)
   {
    for (i=0; i<nLine+4; i++,nPos++)
     {
      xyField[nPos].x= xCent- ((nLine+3)*cx)+ (i*2*cx);
      xyField[nPos].y= yCent- ((5-nLine)*cy);
     }
   }
  for (nLine=1,nPos=61; nLine<=4; nLine++)
   {
    for (i=0; i<nLine+4; i++,nPos--)
     {
      xyField[nPos].x= xCent+ ((nLine+3)*cx)- (i*2*cx);
      xyField[nPos].y= yCent+ ((5-nLine)*cy);
     }
   }
 }

void GfxEnd (void)
 {
  closegraph ();
 }

/********** own functions end *********/

#endif