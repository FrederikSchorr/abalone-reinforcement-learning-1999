
/*	(C) LIPSOFT 1990

	ABAUTL.C

	(gebraucht von ABALONE.C + ABADEF.C,ABALONE.PRJ)
	Abalone
*/



/* prozeduren
	betrag
	moueventhandler
	nextinrichtung
	moglrichtung
	abaende
	sechseck
	kugelfall
	ende
	kugelout
	setfeld
	grundstellung
	initfeld
	initaba
	zeichnen
	kugelkann
	paintpfeile
	defaultpfeile
	setpfeile
	pressedbox
	infeld
*/



#include <GRAPHICS.H>
#include <TIME.H>
#include "C:\PRO\TC\PROG\ABALONE\ABALONE.CD"
#include "C:\PRO\TC\PROG\UTL\MAUS.C"
#include "C:\PRO\TC\PROG\MCURSOR\MCLOAD.CD"
#include "C:\PRO\TC\PROG\UTL\FILE\IFF.C"
#include "C:\PRO\TC\PROG\UTL\SETCOLOR.C"

#define    button(x,y,rad,rand,farbe)   {setcolor (rand);\
					 setfillstyle (1,farbe);\
					 mouseoff ();\
					 circle (x,y,rad);\
					 floodfill (x,y,rand);\
					 mouseon ();}


extern int akt,ax,ay,but,ev;
extern char mcblitz[69];
extern char mcpfeil[69];
extern char mca[69];

int blackcount;
int textbkcol = BLUE;
int textcol = YELLOW;
int invtextbkcol = WHITE;
int invtextcol = RED;



int betrag (int zahl)
 {
  if (zahl < 0) return (zahl * -1);
  return (zahl);
 }


void moueventhandler (int evflags,int butstate,int x,int y)
 {
  akt = -1;
  ax = x;
  ay = y;
  but = butstate;
  ev = evflags;
 }




void nextinrichtung (position *pos,byte richtung)
 {
  switch (richtung)
   {
    case 0:    				/* links oben */
      if ((pos->z) <= 5) (pos->sp)--;
      (pos->z)--;
      break;

    case 1:                              /* rechts oben */
      if ((pos->z) > 5) (pos->sp)++;
      (pos->z)--;
      break;

    case 2:				/* rechts */
      (pos->sp)++;
      break;

    case 3:				/* rechts unten */
      if ((pos->z) < 5) (pos->sp)++;
      (pos->z)++;
      break;

    case 4:				/* links unten */
      if ((pos->z) >= 5) (pos->sp)--;
      (pos->z)++;
      break;

    default:				 /* links */
      (pos->sp)--;
   }
 }



byte moglrichtung (abalone *aba,byte richtung)
 {
  position pos;

  pos = aba->pos;

  switch (aba->feld[pos.z][pos.sp])
   {
    case OUT: return (ABANO);
    case HG: return (ABANO);
    case SP1:
      nextinrichtung (&pos,richtung);
      switch (aba->feld[pos.z][pos.sp])
       {
	case OUT: return (ABANO);
	case HG: return (ABAYES);
	case SP1:
        case SET:
          nextinrichtung (&pos,richtung);
	  switch (aba->feld[pos.z][pos.sp])
	   {
	    case OUT: return (ABANO);
	    case HG: return (ABAYES);
	    case SP1:
	    case SET:
              nextinrichtung (&pos,richtung);
	      switch (aba->feld[pos.z][pos.sp])
	       {
		case OUT: return (ABANO);
		case HG: return (ABAYES);
		case SP1:
		case SET: return (ABANO);
		case SP2:
                  nextinrichtung (&pos,richtung);
		  switch (aba->feld[pos.z][pos.sp])
		   {
		    case OUT: return (ABAYES);
		    case HG: return (ABAYES);
		    case SP1:
                    case SET: return (ABANO);
		    case SP2:
                      nextinrichtung (&pos,richtung);
		      switch (aba->feld[pos.z][pos.sp])
		       {
			case OUT: return (ABAYES);
			case HG: return (ABAYES);
			case SP1:
                        case SET: return (ABANO);
			case SP2: return (ABANO);
		       }
		   }
	       }
	    case SP2:
              nextinrichtung (&pos,richtung);
	      switch (aba->feld[pos.z][pos.sp])
	       {
		case OUT: return (ABAYES);
		case HG: return (ABAYES);
		case SP1:
                case SET: return (ABANO);
		case SP2: return (ABANO);
	       }
	   }
	case SP2: return (ABANO);
       }
    case SP2:
      nextinrichtung (&pos,richtung);
      switch (aba->feld[pos.z][pos.sp])
       {
	case OUT: return (ABANO);
	case HG: return (ABAYES);
	case SP2:
        case SET:
          nextinrichtung (&pos,richtung);
	  switch (aba->feld[pos.z][pos.sp])
	   {
	    case OUT: return (ABANO);
	    case HG: return (ABAYES);
	    case SP2:
            case SET:
              nextinrichtung (&pos,richtung);
	      switch (aba->feld[pos.z][pos.sp])
	       {
		case OUT: return (ABANO);
		case HG: return (ABAYES);
		case SP2:
                case SET: return (ABANO);
		case SP1:
                  nextinrichtung (&pos,richtung);
		  switch (aba->feld[pos.z][pos.sp])
		   {
		    case OUT: return (ABAYES);
		    case HG: return (ABAYES);
		    case SP2:
                    case SET: return (ABANO);
		    case SP1:
                      nextinrichtung (&pos,richtung);
		      switch (aba->feld[pos.z][pos.sp])
		       {
			case OUT: return (ABAYES);
			case HG: return (ABAYES);
			case SP2:
                        case SET: return (ABANO);
			case SP1: return (ABANO);
		       }
		   }
	       }
	    case SP1:
              nextinrichtung (&pos,richtung);
	      switch (aba->feld[pos.z][pos.sp])
	       {
		case OUT: return (ABAYES);
		case HG: return (ABAYES);
		case SP2:
                case SET: return (ABANO);
		case SP1: return (ABANO);
	       }
	   }
	case SP1: return (ABANO);
       }
    default: return (ABANO);
   }
 }





void abaende (void)
 {
  mouseend ();
  setallkanale ();
  closegraph ();
 }



void sechseck (abalone *aba)
 {
  int i,l;
  int cosseite = aba->seite / 2;
  int sinseite = aba->seite * 0.866;
  int x,y;
  int hx,hy;

  mouseoff ();
  setcolor (UNUSEDCOLOR);
  x = aba->e.x + aba->seite / 10;
  y = aba->b.y + sinseite / 10;
  hx = aba->b.x + cosseite / 10;
  for (i = 0;i < 5;i++)
   {
    line (x,aba->e.y,hx,y);
    x += aba->seite / 5;
    hx += cosseite / 5;
    y += sinseite / 5;
   }
  x = aba->a.x + aba->seite / 10;
  y = aba->f.y + sinseite / 10;
  hx = aba->f.x + cosseite / 10;
  for (i = 0;i < 5;i++)
   {
    line (x,aba->a.y,hx,y);
    x += aba->seite / 5;
    hx += cosseite / 5;
    y += sinseite / 5;
   }
  x = aba->d.x - aba->seite / 10;
  y = aba->a.y + sinseite / 10;
  hx = aba->a.x - cosseite / 10;
  for (i = 0;i < 5;i++)
   {
    line (x,aba->e.y,hx,y);
    x -= aba->seite / 5;
    hx -= cosseite / 5;
    y += sinseite / 5;
   }
  x = aba->b.x - aba->seite / 10;
  y = aba->c.y + sinseite / 10;
  hx = aba->c.x - cosseite / 10;
  for (i = 0;i < 5;i++)
   {
    line (x,aba->b.y,hx,y);
    x -= aba->seite / 5;
    hx -= cosseite / 5;
    y += sinseite / 5;
   }
  x = aba->e.x - cosseite / 10;
  y = aba->e.y - sinseite / 10;
  hy = aba->a.y + sinseite / 10;
  hx = aba->d.x + cosseite / 10;
  for (i = 0;i < 5;i++)
   {
    line (x,y,hx,y);
    line (x,hy,hx,hy);
    hy += sinseite / 5;
    x -= cosseite / 5;
    hx += cosseite / 5;
    y -= sinseite / 5;
   }

  setfillstyle (1,15);
  y = aba->e.y - sinseite / 5;
  x = aba->e.x + cosseite / 5;
  for (l = 0;l < 5;l++)
   {
    hx = x;
    hy = y;
    for (i = 0;i < 5;i++)
     {
      floodfill (hx,hy,UNUSEDCOLOR);
      hx -= cosseite / 5;
      hy -= sinseite / 5;
     }
    x += aba->seite / 5;
   }
  y = aba->d.y - 2 * sinseite / 5;
  x = aba->d.x;
  for (l = 0;l < 4;l++)
   {
    hx = x;
    hy = y;
    for (i = 0;i < 5;i++)
     {
      floodfill (hx,hy,UNUSEDCOLOR);
      hx -= cosseite / 5;
      hy -= sinseite / 5;
     }
    x += cosseite / 5;
    y -= sinseite / 5;
   }
  y = aba->b.y + sinseite / 5;
  x = aba->b.x - 3 * aba->seite / 10;
  for (l = 0;l < 4;l++)
   {
    hx = x;
    hy = y;
    for (i = 0;i < 4;i++)
     {
      floodfill (hx,hy,UNUSEDCOLOR);
      hx -= cosseite / 5;
      hy += sinseite / 5;
     }
    x -= aba->seite / 5;
   }

  setcolor (WHITE);
  line (aba->a.x,aba->a.y,aba->b.x,aba->b.y);
  line (aba->b.x,aba->b.y,aba->c.x,aba->c.y);
  line (aba->c.x,aba->c.y,aba->d.x,aba->d.y);
  line (aba->d.x,aba->d.y,aba->e.x,aba->e.y);
  line (aba->e.x,aba->e.y,aba->f.x,aba->f.y);
  line (aba->f.x,aba->f.y,aba->a.x,aba->a.y);
  mouseon ();
 }


void kugelfall (abalone *aba,int x,byte sp)
 {
  int i;
  int y;

  y = 479 - (aba->radius + 3 + (5 * (2 * aba->radius + 6)));

  mouseoff ();
  for (i = 5;i >= aba->outsp[sp];i--)
   {
    button (x,y,aba->radius,LIGHTGRAY,LIGHTGRAY);
    y = 479 - (aba->radius + 3 + (i * (2 * aba->radius + 6)));
    button (x,y,aba->radius,aba->color[sp + 2],aba->color[sp]);
    delay (100);
   }
  mouseon ();
 }



void ende (abalone *aba,byte sp)
 {
  int i;
  unsigned ms = 40;
  int x,y;

  settextstyle (1,0,1);
  setcolor (aba->color[sp]);
  mouseoff ();
  x=208; y=0;
  gprintf (&x,&y,"Spieler %d hat gewonnen !",(int) sp);
  mouseon ();

  initallkanale ();

  settextstyle (0,0,1);

  for (i = 0;i < 3;i++)
   {
    setcolorkanal (aba->color[sp],EGA_BLUE);
    delay (ms);
    setcolorkanal (aba->color[sp],EGA_GREEN);
    delay (ms);
    setcolorkanal (aba->color[sp],EGA_CYAN);
    delay (ms);
    setcolorkanal (aba->color[sp],EGA_RED);
    delay (ms);
    setcolorkanal (aba->color[sp],EGA_MAGENTA);
    delay (ms);
    setcolorkanal (aba->color[sp],EGA_BROWN);
    delay (ms);
    setcolorkanal (aba->color[sp],EGA_LIGHTBLUE);
    delay (ms);
    setcolorkanal (aba->color[sp],EGA_LIGHTGREEN);
    delay (ms);
    setcolorkanal (aba->color[sp],EGA_LIGHTCYAN);
    delay (ms);
    setcolorkanal (aba->color[sp],EGA_RED);
    delay (ms);
    setcolorkanal (aba->color[sp],EGA_MAGENTA);
    delay (ms);
    setcolorkanal (aba->color[sp],EGA_YELLOW);
    delay (ms);
    setcolorkanal (aba->color[sp],EGA_WHITE);
    delay (ms);
   }

  setcolorkanal (aba->color[sp],aba->color[sp]);
  while (!pressedbox (505,405,545,415,"Quit",mca,mcblitz));
  exit (0);
 }


void kugelout (abalone *aba,int sp)
 {
  switch (sp)
   {
    case SP1:
      kugelfall (aba,4 + aba->radius,1);
      aba->outsp[sp]++;
      break;

    case SP2:
      kugelfall (aba,635 - aba->radius,2);
      aba->outsp[sp]++;
      break;

    default:
      putch (7);
      mouseoff ();
      printf ("Fehler in 'kugelout': \"%d\"\nPos: %d/%d",sp,(int)aba->pos.z,(int)aba->pos.sp);
      while (getch () != 27);
      exit (1);
   }

  if (aba->outsp[sp] == 6) ende (aba,3 - sp);
 }




void setfeld (abalone *aba,int inhalt)
 {
  float x,y;

  if (aba->feld[aba->pos.z][aba->pos.sp] == OUT)
    kugelout (aba,inhalt);
  else
   {
    x = (2 * aba->pos.sp + betrag (5 - aba->pos.z)) * (aba->seite / 10);
    x = x + aba->f.x - aba->radius;
    y = (aba->pos.z * aba->seite * 0.1732) + aba->a.y - aba->radius;

    mouseoff ();
    putimage (x,y,aba->buffer[inhalt],COPY_PUT);
    mouseon ();
    aba->feld[aba->pos.z][aba->pos.sp] = inhalt;
   }
 }





void grundstellung (abalone *aba)
 {
  aba->pos.z = 1;
  for (aba->pos.sp = 1;aba->pos.sp <= 5;aba->pos.sp++)
    setfeld (aba,SP1);
  aba->pos.z = 2;
  for (aba->pos.sp = 1;aba->pos.sp <= 6;aba->pos.sp++)
    setfeld (aba,SP1);
  aba->pos.z = 3;
  for (aba->pos.sp = 3;aba->pos.sp <= 5;aba->pos.sp++)
    setfeld (aba,SP1);
  aba->pos.z = 9;
  for (aba->pos.sp = 1;aba->pos.sp <= 5;aba->pos.sp++)
    setfeld (aba,SP2);
  aba->pos.z = 8;
  for (aba->pos.sp = 1;aba->pos.sp <= 6;aba->pos.sp++)
    setfeld (aba,SP2);
  aba->pos.z = 7;
  for (aba->pos.sp = 3;aba->pos.sp <= 5;aba->pos.sp++)
    setfeld (aba,SP2);
 }



void initfeld (abalone *aba)
 {
  int i,e;

  for (i = 0;i < 11;i++)
    for (e = 0;e < 11;e++)
      aba->feld[i][e] = OUT;
  for (e = 1;e <= 5;e++)
    for (i = 1;i < (e + 5);i++)
      aba->feld[e][i] = HG;
  for (e = 6;e <= 9;e++)
    for (i = 1;i <= (14 - e);i++)
      aba->feld[e][i] = HG;
 }




void initaba (abalone *aba,int x,int y,int seite)
 {
  int cosseite = seite / 2;
  int sinseite = seite * 0.866;
  int o,l,u,r;
  int hx,hy;
  unsigned size;

  aba->outsp[SP1] = aba->outsp[SP2] = 0;
  x -= seite / 2;
  y -= sinseite;

  aba->a.x = aba->e.x = x;
  aba->b.x = aba->d.x = x + seite;
  aba->f.x = x - cosseite;
  aba->c.x = aba->f.x + 2 * seite;
  aba->a.y = aba->b.y = y;
  aba->d.y = aba->e.y = y + 2 * sinseite;
  aba->c.y = aba->f.y = y + sinseite;
  aba->seite = seite;
  sechseck (aba);

  aba->radius = seite / 13;
  hx = aba->a.x + seite / 10;
  hy = aba->a.y + sinseite / 5;
  o = hy - aba->radius;
  u = o + 2 * aba->radius;
  l = hx - aba->radius;
  r = l + 2 * aba->radius;

  mouseoff ();
  size = imagesize (l,o,r,u);
  aba->buffer[HG] = malloc (size);
  getimage (l,o,r,u,aba->buffer[HG]);

  setcolor (aba->color[3]);
  setfillstyle (1,aba->color[1]);
  circle (hx,hy,aba->radius);
  floodfill (hx,hy,aba->color[3]);
  size = imagesize (l,o,r,u);
  aba->buffer[SP1] = malloc (size);
  getimage (l,o,r,u,aba->buffer[SP1]);

  setcolor (aba->color[4]);
  setfillstyle (1,aba->color[2]);
  circle (hx,hy,aba->radius);
  floodfill (hx,hy,aba->color[4]);
  size = imagesize (l,o,r,u);
  aba->buffer[SP2] = malloc (size);
  getimage (l,o,r,u,aba->buffer[SP2]);

  setcolor (aba->color[1]);
  setfillstyle (1,aba->color[3]);
  circle (hx,hy,aba->radius);
  floodfill (hx,hy,aba->color[1]);
  size = imagesize (l,o,r,u);
  aba->buffer[PRSP1] = malloc (size);
  getimage (l,o,r,u,aba->buffer[PRSP1]);

  setcolor (aba->color[2]);
  setfillstyle (1,aba->color[4]);
  circle (hx,hy,aba->radius);
  floodfill (hx,hy,aba->color[2]);
  size = imagesize (l,o,r,u);
  aba->buffer[PRSP2] = malloc (size);
  getimage (l,o,r,u,aba->buffer[PRSP2]);

  for (hx = 0;hx < 6;hx++)
   {
    o = 479 - (aba->radius + 3 + (hx * (2 * aba->radius + 6)));
    button (aba->radius + 4,o,aba->radius + 2,WHITE,LIGHTGRAY);
    button (635 - aba->radius,o,aba->radius + 2,WHITE,LIGHTGRAY);
   }

  mouseon ();
  initfeld (aba);
  grundstellung (aba);
 }


void zeichnen (abalone *aba,int sp1,int prsp1,int sp2,int prsp2)
 {
  union REGS regs;

  maxinit ();
  mouseinit ();

  regs.h.ah = 0x12;
  regs.h.bl = 0x36;
  regs.h.al = 1;
  int86 (0x10,&regs,&regs);

  aba->color[0] = EGA_WHITE;
  aba->color[1] = sp1;
  aba->color[2] = sp2;
  aba->color[3] = prsp1;
  aba->color[4] = prsp2;
  initaba (aba,320,248,260);

  mcholen ("ABALONE",mca);
  mcholen ("BLITZ",mcblitz);
  mcholen ("ABARICHT",mcpfeil);
  mcakt (mca);

  setcolor (RED);
  setfillstyle (1,BLUE);
  bar (500,400,550,420);
  rectangle (503,403,547,417);
  setcolor (YELLOW);
  outtextxy (508,407,"Quit");

  iff ("PFEILE.LBM",0,0,1,1,6,13);
  mouseon ();

  setcolorkanal (UNUSEDCOLOR,EGA_BLACK);

  regs.h.ah = 0x12;
  regs.h.bl = 0x36;
  regs.h.al = 0;
  int86 (0x10,&regs,&regs);

  randomize ();
 }



int kugelkann (abalone *aba)
 {
  byte richtung;

  for (richtung = 0;richtung <= 5;richtung++)
    if (moglrichtung (aba,richtung) == ABAYES) return (ABAYES);

  return (ABANO);
 }


void paintpfeile (byte *richt,int jacol,int neincol)
 {
  byte color[7];
  int i;

  for (i = 0;i < 7;i++)
   {
    if (richt[i] == ABAYES) color[i] = jacol;
    else color[i] = neincol;
   }

  mouseoff ();
  setfillstyle (1,color[0]);
  floodfill (37,25,DARKGRAY);
  setfillstyle (1,color[1]);
  floodfill (83,25,DARKGRAY);
  setfillstyle (1,color[2]);
  floodfill (112,59,DARKGRAY);
  setfillstyle (1,color[3]);
  floodfill (83,100,DARKGRAY);
  setfillstyle (1,color[4]);
  floodfill (37,100,DARKGRAY);
  setfillstyle (1,color[5]);
  floodfill (11,59,DARKGRAY);
  setfillstyle (1,color[6]);
  floodfill (56,59,DARKGRAY);
  mouseon ();
 }


void defaultpfeile (void)
 {
  byte richt[7] = {0,0,0,0,0,0,0};

  paintpfeile (richt,EGA_WHITE,EGA_LIGHTGRAY);
 }


void setpfeile (abalone *aba,position *press,byte anzahl)
 {
  byte richt[7];
  byte i;
  byte inhalt;

  if (anzahl == 0)
   {
    defaultpfeile ();
    return;
   }

  if (anzahl == 1)
   {
    aba->pos = press[0];
    if ((inhalt = aba->feld[aba->pos.z][aba->pos.sp]) > SP2)
      aba->feld[aba->pos.z][aba->pos.sp] -= 2;
    for (i = 0;i < 6;i++)
      richt[i] = moglrichtung (aba,i);
    aba->feld[aba->pos.z][aba->pos.sp] = inhalt;
   }
  else if (anzahl >= 2)
   {
    aba->pos = press[0];
    nextinrichtung (&aba->pos,4);
    for (i = 0;i < 6;i++)
     {
      if (aba->feld[aba->pos.z][aba->pos.sp] == HG) richt[(i+4)%6] = ABAYES;
      else richt[(i+4)%6] = ABANO;
      nextinrichtung (&aba->pos,i);
     }
    aba->pos = press[1];
    nextinrichtung (&aba->pos,4);
    for (i = 0;i < 6;i++)
     {
      if (richt[(i+4)%6] == ABAYES && aba->feld[aba->pos.z][aba->pos.sp] != HG)
	richt[(i+4)%6] = ABANO;
      nextinrichtung (&aba->pos,i);
     }
    nextinrichtung (&aba->pos,1);
   }
  if (anzahl == 3)
   {
    aba->pos = press[2];
    nextinrichtung (&aba->pos,4);
    for (i = 0;i < 6;i++)
     {
      if (richt[(i+4)%6] == ABAYES && aba->feld[aba->pos.z][aba->pos.sp] != HG)
	richt[(i+4)%6] = ABANO;
      nextinrichtung (&aba->pos,i);
     }
    nextinrichtung (&aba->pos,1);
   }

  richt[6] = ABAYES;
  paintpfeile (richt,WHITE,LIGHTGRAY);
 }



int pressedbox (int l,int o,int r,int u,char *text,char *mc2,char *mc1)
 {
  int invers = 0;

  if (!(ax > l && ay > o && ax < r && ay < u)) return (0);
  mcakt (mc1);
  while (ax > l && ay > o && ax < r && ay < u)
   {
    if (ev & 4)
     {
      ev = ev & ~4;
      if (invers)
       {
        setfillstyle (1,textbkcol);
	setcolor (textcol);
	mouseoff ();
	bar (l,o,r,u);
	outtextxy (l + 3,o + 3,text);
	mouseon ();
       }
      mcakt (mc2);
      return (1);
     }
    if (but & 1)
     {
      if (invers) continue;
      invers = 1;
      setfillstyle (1,invtextbkcol);
      setcolor (invtextcol);
      mouseoff ();
      bar (l,o,r,u);
      outtextxy (l + 3,o + 3,text);
      mouseon ();
     }
    else
     {
      if (!invers) continue;
      invers = 0;
      setfillstyle (1,textbkcol);
      setcolor (textcol);
      mouseoff ();
      bar (l,o,r,u);
      outtextxy (l + 3,o + 3,text);
      mouseon ();
     }
   }

  if (invers)
   {
    setfillstyle (1,textbkcol);
    setcolor (textcol);
    mouseoff ();
    bar (l,o,r,u);
    outtextxy (l + 3,o + 3,text);
    mouseon ();
   }
  mcakt (mc2);
  return (0);
 }



int infeld (int l,int o,int r,int u,int color,int setcolor,char *mc1,char *mc2)
 {
  int in = 0;
  int first = 1;
  int button = 0;
  int testcol;
  int fx = 0,fy = 0;
  int x,y;

  if (!(ax >= l && ax <= r && ay >= o && ay <= u)) return (ABANO);

  for (akt = 1;;)
   {
    if (!akt) continue;
    akt = 0;

    x = ax;
    y = ay;

    if (!(x >= l && x <= r && y >= o && y <= u))
     {
      if (button)
       {
	setfillstyle (1,color);
	mouseoff ();
	floodfill (fx,fy,DARKGRAY);
	mouseon ();
       }
      if (in) mcakt (mc1);
      return (ABANO);
     }

    if (button) testcol = setcolor;
    else testcol = color;

    if (testmpix (x,y,testcol))
     {
      if (!in)
       {
	mcakt (mc2);
	in = 1;
       }
      if (but & 1)
       {
	if (!button)
	 {
	  setfillstyle (1,setcolor);
	  mouseoff ();
	  floodfill (x,y,DARKGRAY);
	  mouseon ();
          if (first)
	   {
	    first = 0;
	    fx = x;
	    fy = y;
	   }
	  button = 1;
	 }
       }
     if (ev & 4)
       {
	setfillstyle (1,color);
	mouseoff ();
	floodfill (x,y,DARKGRAY);
	mouseon ();
	ev = ev & ~4;
	mcakt (mc1);
	return (ABAYES);
       }
     }
    else
     {
      if (in)
       {
	in = 0;
	mcakt (mc1);
       }
      if (button)
       {
	button = 0;
	setfillstyle (1,color);
	mouseoff ();
	floodfill (fx,fy,DARKGRAY);
	mouseon ();
       }
     }
   }
 }






