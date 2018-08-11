// Drawing Program
// Date Start: June 1, 1995
// Revision: May 16 - 1996
// DRAW V 6

#include "draw.h"
#include <dos.h>
#include <bios.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <conio.h>

#pragma inline
#define TOTALSHAPE 10
#define TOTALANIMS 10
#define TOTALOBJECTS 20

void Init_Screen(void);
void initarray(void);
void CutImage(void);
void PutImage(void);
void RestoreBack(void);
void PutColor(void);
void BoxImage(void);
int getch(void);
int Get_Mouse(int command);
void DrawCursor(void);
void DrawBox(void);
void DrawMask(void);
void Calc_Coords(void);
void Check_Color(void);
void DrawGrid(void);
void Colors(void);
void Check_Draw(void);
void check_thedata(void);
void nextshape(int , int);
void drawshape(int , int);
void Draw_Data_to_Grid(int , int);
void clearshape(void);
void cleargrid(void);
void placeshape(int , int);
void draw_screen(int , int);
void layout_screen(void);
int filewrite(void);
int fileread(void);
void EraseObject(void);
void SaveLastImage(void);
void UndoLastChange(void);
void copyshape(void);
void flipshape(void);
void Animate(void);
void Debug(void);
void BigBox(int , int);
void CutObject(void);
void Check_Select(void);
void draw_plus_minus(void);
void SaveRow(void);
void DrawRow(int rw);
void SyncToVerticalRetrace( void );
void copyshapeRC(void);
int filewrite2(void);
int fileread2(void);
void ClearData(void);
void EraseOverflow(void);
void move_shape(void);
void Current_Screen(void);
void AnimateBackground(int rw, int sp);
void AnimObject(int rw, int sp,int n, int m);

char far *screen = MK_FP(0xa000,0);
extern char password;


int lc,rc,hc;
int nff=0;
int flag=1;
int spy=100;
int spritenum=0;
int lp;
int x,y,xy1,xy,fl,tm,width,oy,ym,xm,button,chkdrw=0;
int j=2;                // start off color
int z=0;
int offset = 0;
int numspr=10;
char far *savearea;
char far *p;

int nx,ny,val,mspeed,oldoffset,oldy,oldval,tm,oval,ptflag;
int c,b,st,ac,step,tim;
int inc=5;
int lso=0;
int acxm,acym;          // Actuall Y Mouse and X mouse
char far *point;
char far *p;
char far *st1;
char undoarea[260];
int sp=0,rw=0;
int allflg=0;
int layout=0;
char sc[20];
int ga=47,gb=61,gc=144,gd=126;
int sval=5;                             // step value for draw
int chgpnt=0;
int memflag=0;


int txy=24;			// text Y Location
int sty=4;			// Start of Y text
int stx=30;

int k,jx,jy;
int c,m1,m2,m3,m4,m5;

int vc = 36109;		// Upper Left hand corner of MOve Object for Erase
int oc = 36430;		// Original Coordinate y *320 + x
int toc = 36430;		// Backup of Original Coordinate y *320 + x

int ap=0,saverw=0,dn=0;;

int saveblk=0;

int maxscreenwidth=5; 		// 5 x 20 blocks = 100 Accross
int maxscreenheight=5;		// 5 x 10 blocks = 50  Down

int loy=0,lox=0;
int layarr[100][100];	// Row & Coloumn
int maxscreens=2;		// 5 screens accross x 5 screens down = 25
int screenx=19;
int screeny=10;
int screenystart=0;
int screenxstart=0;
int scrollx=0;
int savedobjects[20][10];
int animscreen=0;
int curval=0;
int t1=0;
int newobj;
int obj1s=35;	// Object 1 start
int obj1e=37;	// Object 1 end


FILE *in, *out, *outdata, *indata, *std, *map, *mapin;


extern char box[6][6];
extern char plus[7][7];
extern char minus[7][7];
extern char sixteen[17][17];
extern char boxmask[6][6];

struct savebg

	{
	int bgflag;
	char far backgnd[256];

	} savedarea[TOTALOBJECTS];


struct fshape
	{

		int w,h;
		int n,c;
		int flag;
		int rowflag;
		char far shp[260];

					// TOTALSHAPE  =  20
	};



struct animshape {
		int active;
		int animwidth;
		int animheight;
		int animox;
		int animoy;
		int animx;
		int animy;
		int prox;
		int animspeed;
		int currentshape;
		int oldshape;
		int max;
		int row;

		struct fshape *fshp[TOTALSHAPE];        // 10 Pointers to data

	 } animobjects[TOTALANIMS];



void main(void)
{

	int ext=0;

	char *mypass ={"amiga"};

	sp=0;                   // sprite value
	ptext();

     //	if (password == mypass)
     //	{
      //	printf("%s",password);
      ///	getch();
      //	}
  //	else
    //	{
     //	exit(1);
     //	}


	Init_Mode();
	Init_Screen();
	initarray();
	PutColor();
	DrawGrid();

	BoxImage();

	gotoxy (29,21);
	printf("F1 - Help ");


//      if (!Get_Mouse(1))
 //    {
  //        printf("No Mouse Driver Present\n");
   //       exit(1);
    //  }

       //       gotoxy (1,25);         //       Calculate Start of gride 10 lines down
 //     printf("Sprite %d  Row %d ",sp,rw);

      while (ext != 256)

		{
		Get_Mouse(0);
		Calc_Coords();

		Check_Draw();
		Check_Color();
		gotoxy (stx,sty);
		printf("Sp:%d",sp);

		gotoxy (stx,sty+1);
		printf("Rw:%d",rw);

		gotoxy (stx,sty+2);
		// printf("fg:%d",animobjects[rw].fshp[sp]->flag);
		printf("Max:%d",animobjects[rw].max);

		gotoxy (stx,sty+3);
		printf("Colour: %d",j);

		gotoxy (stx,sty+4);
		printf("Flag: %d",animobjects[rw].fshp[sp]->flag);

		if (bioskey(1))
		{
			rc = bioskey(0);
			hc = (rc>>8)&0xff;
			lc = rc&0xff;
		    //  printf("\n");
		    //  printf("%d %d \n",hc,lc);

	if (hc == 1 && lc == 27)   // ESC Key & Exit
		{
		ext=256;
		printf("Exit\n");
		}

		if (hc == 59 && lc == 0)        // 'F1' Help Screen
		{

		nextshape(rw,sp);
		Close_Mode();
		helpscreen();
		Init_Mode();
		placeshape(rw,sp);
		Draw_Data_to_Grid(rw,sp);
		PutColor();
		DrawGrid();

		hc=0;lc=0;
		   }

		if (hc == 60 && lc == 0)        // 'F2' Copy shape to diff
		{				// row and coloum

		nextshape(rw,sp);
		copyshapeRC();
		hc=0;lc=0;
		   }


		if (hc == 63 && lc == 0)        // 'F5' Save Row (current)
		{
		SaveRow();

		hc=0;lc=0;
		   }

		if (hc == 72 && lc == 0)        // Up Arrow
		{
		oc = oc - 320;
		EraseOverflow();
		move_shape();
		nextshape(rw,sp);
		EraseOverflow();
		placeshape(rw,sp);
		Draw_Data_to_Grid(rw,sp);
		hc=0;lc=0;

		}
		if (hc == 80 && lc == 0)        // Down Arrow
		{
		oc = oc + 320;
		EraseOverflow();
		move_shape();
		nextshape(rw,sp);
		EraseOverflow();
		placeshape(rw,sp);
		Draw_Data_to_Grid(rw,sp);

		hc=0;lc=0;
		}

		if (hc == 77 && lc == 0)        // Right Arrow
		{
		oc = oc + 1;
		EraseOverflow();
		move_shape();
		nextshape(rw,sp);
		EraseOverflow();
		placeshape(rw,sp);
		Draw_Data_to_Grid(rw,sp);

		hc=0;lc=0;
		}

		if (hc == 75 && lc == 0)        // Left Arrow
		{
		oc = oc - 1;
		EraseOverflow();
		move_shape();
		nextshape(rw,sp);
		EraseOverflow();
		placeshape(rw,sp);
		Draw_Data_to_Grid(rw,sp);

		hc=0;lc=0;
		}



		if (hc == 26 && lc == 91)        // '[' Save Old(current)
		{
		gotoxy (1,txy);
		printf("Save Old");
		screenx=19;
		screeny=10;
		screenystart=0;
		screenxstart=0;

		filewrite2();
		sp=0;rw=0;
		hc=0;lc=0;
		   }

		if (hc == 27 && lc == 93)        // ']' Load Old(current)
		{
		gotoxy (1,txy);
		printf("Load Old");
		screenx=19;
		screeny=10;
		screenystart=0;
		screenxstart=0;

		fileread2();
		sp=0;rw=0;
		placeshape(rw,sp);
		Draw_Data_to_Grid(rw,sp);

		hc=0;lc=0;
		   }

		if (hc == 62 && lc == 0)        // 'F4' Blank
		{

		hc=0;lc=0;
		}



		if (hc == 22 && lc == 117)        // 'U' Undp Last Change
		{


		clearshape();
		UndoLastChange();
		nextshape(rw,sp);
		Draw_Data_to_Grid(rw,sp);
		hc=0;lc=0;
		   }


		if (hc == 30 && lc == 'a')        // 'A'Animate
		   {
		   nextshape(rw,sp);
		   Animate();
		   hc=0;lc=0;
		   }



		   if (hc == 34 && lc == 'g')        // 'G'Draw Grid
		   {
		DrawGrid();
		hc=0;lc=0;
		   }

		   if (hc == 68 && lc == 0)        // 'F-10' Debug Mode
		   {
		   Debug();
		   hc=0;lc=0;
		   }

		   if (hc == 66 && lc == 0)        // 'F-8' Layout Mode
		   {
		   nextshape(rw,sp);
		   saveblk=sp;
		   saverw=rw;

		   layout_screen();

		   sp=saveblk;         //  Reset
		   rw=saverw;

		   placeshape(rw,sp);
		   Draw_Data_to_Grid(rw,sp);

		   hc=0;lc=0;
		   }


		   if (hc == 46 && lc == 'c')        // 'C'Copy Shape
		   {
		   copyshape();
		hc=0;lc=0;
		   }

		   if (hc == 18 && lc == 'e')        // 'E'Erase Shape
		   {
		   EraseObject();
		   hc=0;lc=0;
		   }

		   if (hc == 33 && lc == 'f')        // 'F'Flip Shape
		   {
		   nextshape(rw,sp);
		   flipshape();
		   nextshape(rw,sp);
		   Draw_Data_to_Grid(rw,sp);
		   hc=0;lc=0;
		   }

		   if (hc == 50 && lc == 'm')        // 'M'Move Shape
		   {
		    DrawGrid();
		     hc=0;lc=0;
		   }







			if (hc == 73 && lc == 0 && rw >0)        // Page Up
			{

				nextshape(rw,sp);
				clearshape();
				cleargrid();

				if (rw >0)
				{

				sp=0;
				rw = rw - 1;
				gotoxy (10,txy);
				placeshape(rw,sp);
				Draw_Data_to_Grid(rw,sp);


				hc=0;lc=0;
				}
			}

			if (hc == 81 && lc == 0 && rw < 9)        // Page Down
			{

				nextshape(rw,sp);
				clearshape();
				cleargrid();

				if (animobjects[rw].fshp[sp]->flag==1 && chkdrw ==1 )
				{
				animobjects[rw].max+=1;
				chkdrw=0;
				}

				if (rw < (TOTALANIMS-1))
				{
				rw = rw + 1;
				sp=0;
				placeshape(rw,sp);
				Draw_Data_to_Grid(rw,sp);

				hc=0;lc=0;
				}
			}

			if (hc == 47 && lc == 118)        // 'V'View Shapes
			{
			nextshape(rw,sp);
			draw_screen(rw,sp);
			hc=0;lc=0;

			}




			if (hc == 31 && lc == 115)        // 'S'Save Shapes
			{
			nextshape(rw,sp);
			if (animobjects[rw].max>0)
			{
			filewrite();
			placeshape(rw,sp);
			Draw_Data_to_Grid(rw,sp);
			}
			else
			{
			gotoxy (1,txy);
			printf("Nothing to Save ");
			}

			hc=0;lc=0;

			}

			if (hc == 38 && lc == 108)        // 'L'Load Shapes
			{
			sp=0;
			gotoxy (1,txy);
			printf("Loading ");
			fileread();
			placeshape(rw,sp);
			Draw_Data_to_Grid(rw,sp);

			hc=0;lc=0;
			}




			if (hc == 74 && lc == 45 && sp >0)        // '-' Moved down 1 object
			{
			 hc=0;lc=0;

			 nextshape(rw,sp);

			 if (sp >0)
			 {
			 sp = sp - 1;

			 clearshape();
			 cleargrid();
			 placeshape(rw,sp);
			 Draw_Data_to_Grid(rw,sp);

			 }


			}


				if (hc == 78 && lc == 43 && sp <9)        // + Moved over 1 object
				{
				hc=0;lc=0;

				nextshape(rw,sp);

				if (sp <10)
				{
				sp = sp + 1;

				clearshape();
				cleargrid();
				placeshape(rw,sp);
				Draw_Data_to_Grid(rw,sp);

				}


		}


		hc=0;lc=0;

		}

	}
	farfree(st1);
	farfree(savearea);
	Close_Mode();

}

void Calc_Coords(void)
{


	   xm/=2;
	   mspeed=2;
	   xm = xm / sval;
	   ym = ym / sval;
	   acxm = xm;                   // Coords for little box
	   acym = ym;
	   xm = xm * sval+1;
	   ym = ym * sval;

	   ny = ym * 320;
	   val = xm + ny;
	   oval = val;

	   tim = oval;


}

void Init_Screen(void)
{

	if ((savearea = (char far  *) farmalloc(350)) == NULL)
	{
	printf("Not enough memory to allocate buffer\n");
	exit(1);  /* terminate program if out of memory */
	}


}

void initarray(void)
{

	int i,j,s;
	char far *q;

	if ((p=(char far *)farmalloc(sizeof (struct fshape)*TOTALSHAPE*TOTALANIMS))==NULL)
	{
		printf("Could Not Allocate Memory for Objects\n");
		exit(1);
	}

	_fmemset(p,0,sizeof(struct fshape)*TOTALSHAPE*TOTALANIMS);

	st1 = p;


	for (i=0;i<TOTALANIMS;i++)
	{
		for (j=0;j<TOTALSHAPE;j++)
			{
			animobjects[i].fshp[j]=(struct fshape *)p;
			p+=sizeof(struct fshape);
			animobjects[i].animx=100;
			animobjects[i].animy=100;

			}
	}
	for (i=0;i<=9;i++)
	{
		savedarea[i].bgflag=0;
	}
	// int layarr[100][50];	// Row & Coloumn
	for (i=0;i<100;i++)
	{
		for (j=0;j<50;j++)
			{
			layarr[i][j]=-1;	// Row & Coloumn
			}
	}






}


void PutColor()
{
	int ad,j,n,v;
	v=0;
	n=0;
	ad=0;
	for (j=0;j<256;j++)
	{
	   for (x=0;x<4;x++)
	   {
		for (y=0;y<4;y++)
		{
		  *(screen+y+ad+offset+n+321)=j;
		}
		offset = offset + 320;      //bytes to next line
	   }
	 ad=ad + 5;                     // distance between colors

	 if (ad > 314)                  // if at end go to next line
	 {

		v = v + 5;
		ad = 0;
		n = 320 * v;


	 }
	 offset =0;

	 }
}

void BoxImage()

{



	for (x=0;x<6;x++)
	{
		for (y=0;y<6;y++)
		{
		    *(screen+y+offset+val)=box[y][x];
		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;
	oy=width;
}


int Get_Mouse(int command)
{
      int found=0;

      if (command)
      {

	      asm     {
		      push    ds
		      mov     ax,0
		      int     0x33
		      mov     [found],ax
		      pop     ds
		      }

		  return found;
	  }

	asm     {
		push    ds
		mov     ax,3
		mov     bx,3
		int     0x33
		mov     [ym],dx
		mov     [xm],cx
		and     bx,7                        // if 0 then no button
		mov     [button],bx                 // if 1 the left button
		pop     ds
		}

	return 0;
}

void CutImage(void)
{
	int i=0;

	for (x=0;x<18;x++)
	{
		for (y=0;y<18;y++)
		{
			*(savearea+i)=*(screen+y+offset+val);
			i++;
		}
		offset = offset + 320;      //bytes to next line
	}

	offset=0;
}

void PutImage(void)
{

	for (x=0;x<4;x++)
	{
		for (y=0;y<4;y++)
		{
		  *(screen+y+offset+val)=j;
		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;

}

void RestoreBack(void)
{
	int i=0;

	for (x=0;x<18;x++)
	{
		for (y=0;y<18;y++)
		{
			 *(screen+y+offset+val)=*(savearea+i);
			i++;
		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;
}





void Check_Color(void)
{
	// Colors are divisable by 2 since they are 4 wide
	// and 4 high
	// start at 0
	// increment by 5
	int cl=320*2+2;



	if (ym <= 18 && button==1 || ym>18 && button==2)
	 {
	j = *(screen +val+cl);          // turn the block in to the color
					// 'J' is the color
	for (x=0;x<6;x++)               // draw the boax around the color
	{
		for (y=0;y<6;y++)
		{
		    if (button!=2)
		    *(screen+y+offset+val-1)^=box[y][x];
		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;
	}

}
void DrawGrid(void)
{

	st = 64 * 320+50;
	for (c=0;c<17;c++)
	{
		for (b=0;b<80;b++)             // 60 Lines high and 60 accross
		{
		*(screen+b+offset+st)^=txy;
		}
		offset = offset + 320*5;
	}
	offset=0;
	st=64 * 320+50;                 //start location

	for (c=0;c<17;c++)
	{

		for (b=0;b<80;b++)             // 60 Lines high and 60 accross
		{
		*(screen+b+offset+st+step)^=txy;
		offset = offset + 319;
		}
		offset = 0;
		step = step + 5;
	}
	offset=0;
	step=0;

}

void Colors(void)
{
	long int b;

	 for (c=16;c<32;c++)
	 {
		for (b=0;b<64000;b++)
		{
		*(screen+b)=c;
		}
	 }
}
void Check_Draw(void)
{


		if (button==0)
		{

			CutImage();
			delay (5);

			if (layout==0)
				{
				SyncToVerticalRetrace();
				BoxImage();

				}
			else
			{
			SyncToVerticalRetrace();
			BigBox(rw,sp);
			}

			SyncToVerticalRetrace();
			RestoreBack();
			lso=0;

		}
		if (button==1)

		{

			if (xm >=ga && ym >=gb && ym <= gc && xm <=gd)
			{
				if (animobjects[rw].fshp[sp]->flag==0)
				{
				animobjects[rw].max+=1;
				animobjects[rw].fshp[sp]->flag=1;
				allflg=1;
				chkdrw=1;

				}

				if (lso==0)
				{
					SaveLastImage();
					lso=1;
				}

			if (layout==1)
				{
					BigBox(rw,sp);
					layarr[acxm+lox][acym+loy]=sp+(rw*10);
					gotoxy (1,23);
					printf("%d %d %d",acxm,acym,sp*(rw+1));
				}
				else
				{
				PutImage();
				*(screen + acxm + acym * 320 + 32260) = j;
				}
			}


		}

}



void Draw_Data_to_Grid(int rw , int sp)

{

	int blkclr,gx,gy,cx,cy,incr=0,dninc=0;

	st = 65 * 320+51;
	dninc = st;

	for (gx=0;gx<16;gx++)
	{
	    for (gy=0;gy<16;gy++)
	    {
		for (cy=0;cy<4;cy++)
		{
			for (cx=0;cx<4;cx++)             // 60 Lines high and 60 accross
			{
			*(screen+cx+st+offset)=animobjects[rw].fshp[sp]->shp[gx*16+gy];

			}
			offset = offset + 320;
		}
		st = st +5;
		offset=0;
	    }
	    incr = incr + 5;
	    st = dninc + (incr*320);

	}
}


void nextshape(int rw , int sp)
{

	int i,j,vz;

	vz=0;

	for (i=0;i<16;i++)
		{
		for (j=0;j<16;j++)
		{
			animobjects[rw].fshp[sp]->shp[i*16+j]=*(screen+j+36430+vz);

		}
		vz = vz + 320;
	    }


}


void drawshape(int rw , int sp)
{

	int i,j,vz;

	vz=0;

	for (i=0;i<x;i++)

	    {
		for (j=0;j<y;j++)
		{

			*(screen+j+vz)=animobjects[rw].fshp[sp]->shp[i*16+j];

		}

		vz = vz + 320;

	    }




}


void clearshape(void)
{

	int i,j,vz;

	vz=0;

	for (i=0;i<16;i++)
	    {
		for (j=0;j<16;j++)
		{

			*(screen+j+36430+vz)=0;

		}
		vz = vz + 320;
	    }



}

void cleargrid(void)
{

	int blkclr,gx,gy,cx,cy,incr=0,dninc=0;


	st = 65 * 320+51;

	dninc = st;

	offset=0;

	for (gx=0;gx<16;gx++)
	{
	    for (gy=0;gy<16;gy++)
	    {
		for (cy=0;cy<4;cy++)
		{
			for (cx=0;cx<4;cx++)             // 60 Lines high and 60 accross
			{
			*(screen+cx+st+offset)=0;
			}
			offset = offset + 320;
		}
		st = st +5;
		offset=0;
	    }
	    incr = incr + 5;
	    st = dninc + (incr*320);

	}




}

void placeshape(int rw, int sp)
{

	int i,j,vz;

	vz=0;

	for (i=0;i<16;i++)
		{
		for (j=0;j<16;j++)
		{

			*(screen+j+36430+vz)=animobjects[rw].fshp[sp]->shp[i*16+j];

		}
		vz = vz + 320;

	    }


}


int filewrite(void)
{

   int i,numobj,numrow;
   char label[20];
   char infst[20];
   char stan[20];

   gotoxy (1,txy);
   printf("Please enter Filename: ");
   fflush(stdin);  /* flush the input stream in case of bad input */

   scanf("%8s",label);
   strcpy(infst, label);
   strcpy(stan, label);

   strcat(label,".dat");
   strcat(infst,".inf");
   strcat(stan,".std");


   if ((std = fopen(stan, "wb"))
	   == NULL)
   {
	  fprintf(stderr, "Cannot open Standard file.\n");
	  return 1;
   }


   if ((out = fopen(infst, "wb"))
	   == NULL)
   {
	  fprintf(stderr, "Cannot open output file.\n");
	  return 1;
   }

	for (numrow=0;numrow<10;numrow++)
	{
		fprintf(out,"%d\n",animobjects[numrow].max);
		for (numobj=0;numobj<10;numobj++)
		{
			if (animobjects[numrow].fshp[numobj]->flag==1)
			{

			  fwrite(animobjects[numrow].fshp[numobj],272l,1,out);
			  fwrite(animobjects[numrow].fshp[numobj]->shp,256,1,std);
			}
		}
	}

   fclose(out);
  // fclose(outdata);
   fclose(std);
   return 1;

}

int fileread(void)
{

	int i,j,numobj,numrow;
       //	int indat=0;
	char far *p;
	char label[20];
	char infst[20];

	ClearData();		// Clear the Array

	gotoxy (1,txy);
	printf("Please enter Filename: ");
	fflush(stdin);  /* flush the input stream in case of bad input */

	scanf("%8s",label);

	strcpy(infst, label);
	strcat(label,".dat");
	strcat(infst,".inf");


	if ((in = fopen(infst, "rb"))
	   == NULL)
	{
	  fprintf(stderr, "Cannot open input file.\n");
	  return 1;
	}

	for (numrow=0;numrow<=9;numrow++)
	{

	    fscanf(in,"%d",&animobjects[numrow].max);

	      fgetc(in);

		for (numobj=0;numobj<=animobjects[numrow].max;numobj++)
		{
		fread(animobjects[numrow].fshp[numobj],272l,1,in);
		animobjects[numrow].fshp[numobj]->flag=1;
		}
	}

   fclose(in);
   fclose(indata);


   return 1;



}



void draw_screen(int rw, int sp)
{
	int ap=0;
	int dn=0;
	int cnt=0;
	int t,s,y;
	long int clr;

	for (clr=0;clr<64000;clr++)
		{
		*(screen+clr)=0;
		}

	for (t=0;t<=9;t++)
	{
		for (s=0;s<=9;s++)
		{
			if (animobjects[t].fshp[s]->flag==1)
			{
				for (x=0;x<16;x++)
				{
					for (y=0;y<16;y++)
					{
					*(screen+y+offset+ap+dn)=animobjects[t].fshp[s]->shp[x*16+y];
					}
					offset = offset + 320;      //bytes to next line
				}

				offset = 0;
				ap = ap +18;
			}
		}
		cnt = cnt + 18;
		dn = cnt * 320;
		offset=0;
		ap=0;


	}
	gotoxy (1,txy);
	printf("Press Any Key to Return");
	getch();
		for (clr=0;clr<64000;clr++)
		{
		*(screen+clr)=0;
		}

	placeshape(rw,sp);
	Draw_Data_to_Grid(rw,sp);
	PutColor();
	DrawGrid();
	gotoxy (29,txy);
	printf("F1 - Help ");


}


void copyshape(void)
{

    int cp,dn,i;

    gotoxy (1,txy);
    printf("Source :");
    cp=getch()-'0';

    if (cp < 0 || cp > 9)
	printf("Object out of Row Range");
    else
    {
	gotoxy (1,txy);
	printf("Destin :");
	dn=getch()-'0';
		if (dn <0 || dn >9)
			{
			gotoxy (1,txy);
			printf("Object destination out of Range");
			}
		else
			if (animobjects[rw].fshp[dn]->flag==1)
				{
				gotoxy (1,txy);
				printf("Destination Contains Data");
				}
				else
				{
				for (i=0;i<259;i++)
				{
				animobjects[rw].fshp[dn]->shp[i]=animobjects[rw].fshp[cp]->shp[i];
				}
				animobjects[rw].fshp[dn]->flag=1;
				animobjects[rw].max+=1;
				placeshape(rw,sp);
				Draw_Data_to_Grid(rw,sp);

				}

    }





}

void copyshapeRC(void)
{

    int cp,dn,i;
    int so,sd;


    gotoxy (1,txy);
    printf("Source Object  :");
    cp=getch()-'0';				// Source Object

    if (cp < 0 || cp > 9)
	printf("Object out of Row Range");

	else
	{
		gotoxy (1,txy);
		printf("Source Row :");
		so=getch()-'0';        		// Row with object
	}

    if (so < 0 || so > 9)
	printf("Source Row out of Row Range");

	else
	{
		gotoxy (1,txy);
		printf("Destin Object:");
		dn=getch()-'0';          	// Destination Object
	}

	if (dn <0 || dn >9)
	{
		gotoxy (1,txy);
		printf("Object destination out of Range");
	}
	else
	{
	gotoxy (1,txy);
	printf("Destin Row:");
	sd=getch()-'0';				// Destination Row
	}

	if (sd <0 || sd >9)
	{
		gotoxy (1,txy);
		printf("Object destination out of Range");
	}

	else
	{

	if (animobjects[sd].fshp[dn]->flag==1)
	{
		gotoxy (1,txy);
		printf("Destination Contains Data");
	}
	else
	{
		for (i=0;i<259;i++)
		{
		      //	animobjects[sd].fshp[dn]->shp[i]=animobjects[so].fshp[cp];
		      animobjects[sd].fshp[dn]->shp[i]=animobjects[so].fshp[cp]->shp[i];
		}
			animobjects[sd].fshp[dn]->flag=1;
			animobjects[sd].max+=1;
			placeshape(sd,dn);
			Draw_Data_to_Grid(sd,dn);
	}

    }





}



void EraseObject(void)
{

	clearshape();
	cleargrid();
	if (animobjects[rw].max>0 && animobjects[rw].fshp[sp]->flag != 0)
		animobjects[rw].max--;
	animobjects[rw].fshp[sp]->flag=0;
	nextshape(rw,sp);


}


void SaveLastImage(void)
{

	int i,j,vz;

	vz=0;

	for (i=0;i<16;i++)
		{
		for (j=0;j<16;j++)
		{
			undoarea[j+i*16]=*(screen+j+36430+vz);

		}
		vz = vz + 320;
	    }

	    lso=1;

}

void UndoLastChange(void)
{

	int i,j,vz;

	vz=0;

	for (i=0;i<16;i++)
		{
		for (j=0;j<16;j++)
		{
			*(screen+j+36430+vz)=undoarea[i*16+j];

		}
		vz = vz + 320;
	    }




}


void flipshape(void)
{

	int i,j,vz,dn;

	vz=0;

	gotoxy (1,txy);
	printf("Flip (x or y)`:");

	dn=getch();

	if (dn == 'Y' || dn == 'y')
	{

	for (i=0;i<16;i++)
		{
		for (j=0;j<16;j++)
		{

			*(screen+j+36430+vz)=animobjects[rw].fshp[sp]->shp[(15-i)*16+j];

		}
		vz = vz + 320;

	    }
	}

	if (dn == 'X' || dn == 'x')
	{
	for (i=0;i<16;i++)
		{
		for (j=0;j<16;j++)
		{

			*(screen+j+36430+vz)=animobjects[rw].fshp[sp]->shp[i*16+15-j];

		}
		vz = vz + 320;

	    }
	}



}


void Animate(void)
{

    int cp,dn,i,spp;
    long int t;

    gotoxy (1,txy);
    printf("Start :      ");
    cp=getch()-'0';

    if (cp < 0 || cp > 9)
	printf("Object out of Row Range");
    else
    {
	gotoxy (1,txy);
	printf("Destin :          ");
	dn=getch()-'0';

	if (dn <0 || dn >9)
	{
		gotoxy (1,txy);
		printf("Object destination out of Range");
	}
	else
	{
		gotoxy (1,txy);
		printf("Speed 1 -8:");
		spp=getch()-'0';

		if (spp <0 || spp >8)
		{
		gotoxy (1,txy);
		printf("Wrong Speed    ");
		}

		else
		{
		gotoxy (1,txy);
		printf("                   ");

		while(!bioskey(1))
		{
		for (i=cp;i<=dn;i++)
		    {
		    placeshape(rw,i);
			delay(50*spp);
		}

		    }
		}
		getch();
	}


    }
    placeshape(rw,sp);
    Draw_Data_to_Grid(rw,sp);


}


void CutObject(void)
{
	int i=0;
	int tempx,tempy;

	for (i=0;i<TOTALOBJECTS;i++)
	{
	    tempx = animobjects[rw].animx;
	    tempy = animobjects[rw].animy * 320 ;
	    offset = tempx + tempy;

	    for (x=0;x<16;x++)
	    {
		for (y=0;y<16;y++)
		{
		savedarea[i].backgnd[i*16+j]=*(screen+offset+y);
		}
		offset = offset + 320;      //bytes to next line
	    }

	offset=0;
	}
}


void PutObject(void)
{
	// put objects on screen
	// save object to oldobject coords
	// add speed to object
	// add animation to next frame
	// check if object is at end of anim - if yes set to 0
	int i;

	for (i=0;i<TOTALOBJECTS;i++)
	{
	    for (x=0;x<16;x++)
	    {
		for (y=0;y<16;y++)
		{
		  *(screen+y+offset+val)=animobjects[rw].fshp[sp]->shp[i*16+j];
		}
		offset = offset + 320;      //bytes to next line
	    }
	offset=0;
	}
}

void RestoreObject(void)
{
	int i=0;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
			 *(screen+y+offset+val)=*(savearea+i);
			i++;
		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;
}


void Debug(void)
{


    int cp;

    gotoxy (1,txy);
    printf("Debug Max Objects:      ");
    scanf("%d",&cp);

    if (cp < 0 || cp > 10)
	printf("Object out of Row Range");
    else
    {
	animobjects[rw].max=cp;
    }


}


void layout_screen(void)
{

	int t,s,y;
	long int clr;
	int rc;
	int cp,i,spp;
	int sk;
	int c,d;
	int ext;



	offset=160*320;
	BigBox(rw,sp);

	for (clr=0;clr<64000;clr++)
		{
		*(screen+clr)=0;
		}

  //	DrawRow(rw);
    scrollx=0;
    Current_Screen();


    ap=0;
    dn=0;
    offset=0;
    ga=0;
    gb=0;
    gc=159;
    gd=319;
    sval=16;    // set step value to 1

    gotoxy (20,txy);
    printf("Right Mouse to Exit");

//    draw_plus_minus();

    do
    {

		Get_Mouse(0);
		Calc_Coords();


		layout=1;

		Check_Draw();

		if (animscreen==1)
		 AnimateBackground(rw,sp);

		Check_Select();

		if (lc == 'i')
		{
			loy--;
			if (loy<0)
			{
				loy=0;
			}
			Current_Screen();

		}
		if (lc == 'm')
		{
			loy++;
			if (loy>=100)
			{
				loy=99;
			}
			Current_Screen();
		}
		if (lc == 'j')
		{
		//	scrollx+=2;
		//	if (scrollx >= 16)
		//	{
		//		scrollx=0;
				lox--;
		 //	}
			if (lox<=0)
			{
				lox=0;
				scrollx=0;
			}
			Current_Screen();
		}
		if (lc == 'k')
		{
		  //	scrollx-=2;
		  //	if (scrollx < 0)
		    //	{
		      //		scrollx=15;
				lox++;
				if (lox>=100)
				{
					lox=99;
					scrollx=0;
				}
		      //	}
			Current_Screen();
		}


     }while (button!=2 && lc != 'q' && lc != 'Q');

	gotoxy (1,22);
	printf("Press Any Key to Return");
	getch();

		for (clr=0;clr<64000;clr++)
		{
		*(screen+clr)=0;
		}


       //	placeshape(rw,sp);
       //	Draw_Data_to_Grid(rw,sp);



	PutColor();
	DrawGrid();
	gotoxy (29,txy);
	printf("F1 - Help ");

	layout=0;
	ga=47;
	gb=61;
	gc=144;
	gd=126;
	sval=5;
	animscreen = 0;

}

void DrawRow(int rw)
{

	int x;
	int ap=0;
	int dn=0;
	int cnt=0;
	int s,v;


	offset=160*320;


		for (s=0;s<=9;s++)
		{
			if (animobjects[rw].fshp[s]->flag==1)
			{
				for (x=0;x<16;x++)
				{
					for (y=0;y<16;y++)
					{
					*(screen+y+offset+ap+dn)=animobjects[rw].fshp[s]->shp[x*16+y];
					}
					offset = offset + 320;      //bytes to next line
				}

				offset = 160*320;
				ap = ap +18;
			}
		}
		cnt = cnt + 18;
		dn = cnt * 320;
		offset=0;
		ap=0;






}

void BigBox(int rw, int sp)
{

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
		    *(screen+y+offset+val)=animobjects[rw].fshp[sp]->shp[x*16+y];;

		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;
	oy=width;



}







void Check_Select(void)
{
	int ext;

	hc=0;lc=0;
	if (bioskey(1))
	{
		rc = bioskey(0);
		hc = (rc>>8)&0xff;
		lc = rc&0xff;
		gotoxy (20,23);
		printf("SP= %d RW= %d",sp,rw);

		if (hc == 74 && lc == 45)   // - Key
		{
			if (sp <=0 && rw >0)
			{
			sp=9;
			rw = rw -1;
		   //	DrawRow(rw);
			}
			else
			{
			if (sp >0)
			{
			sp = sp -1;
			hc=0; lc=0;
			}
			}

		}

		if (hc == 78 && lc == 43)        // '+'
		{
			if (sp >8 && rw <=9)
			{
			sp=0;
			rw = rw + 1;
		     //	DrawRow(rw);
			}
			else
			{
			sp = sp + 1;
			hc=0;lc=0;
			}
		}

		if (hc == 30 && lc == 'a')        // 'a'
		{

			animscreen = 1;
			hc=0;lc=0;

		}



	}

}


void draw_plus_minus(void)

{

	int i,j,vz;

	vz=0;

	for (i=0;i<7;i++)
		{
		for (j=0;j<7;j++)
		{

			*(screen+j+57600+vz)=minus[i][j];
			*(screen+j+57610+vz)=plus[i][j];

		}
		vz = vz + 320;

	    }


}


void SaveRow(void)
{

    int cp,dn,i,v;

    if (memflag==0)
    {
	gotoxy (1,txy);
	printf("Source :");
	cp=getch()-'0';



	if (cp < 0 || cp > 9)
		printf("Object out of Row Range");


	{
	 for (i=0;i<=9;i++)
	{
	savedarea[i].bgflag=0;
	}


		for (v=0;v<10;v++)
		{
	     //	printf("%d",animobjects[cp].fshp[v]->flag);
	     //	getch();
		   if (animobjects[cp].fshp[v]->flag==1)
		   {
		       //	printf("d");
		      for (i=0;i<=256;i++)
		      {
		      savedarea[v].backgnd[i]=animobjects[cp].fshp[v]->shp[i];
		      }
		      savedarea[v].bgflag=1;
		   }
		}
		memflag=1;

	}
    }
    else
    {
    memflag=0;
    gotoxy (1,txy);
	printf("Destin :");
	dn=getch()-'0';
		if (dn <0 || dn >9)
		{
			gotoxy (1,txy);
			printf("Object destination out of Range");
		}

		else
			{


			 for (v=0;v<10;v++)
			{
			if (savedarea[v].bgflag==1)
			{
				for (i=0;i<=256;i++)
				{

				animobjects[dn].fshp[v]->shp[i]=savedarea[v].backgnd[i];
				}

			    animobjects[dn].fshp[v]->flag=1;
			    animobjects[dn].max=9;
			}
			    if (rw == dn)
			    {
			      placeshape(rw,sp);
			      Draw_Data_to_Grid(rw,sp);

			    }

			 }
			memflag=0;
			}

     }
}


void SyncToVerticalRetrace( void )
  {
  // If we happen to be in the middle of a vertical retrace
  // period wait until its over.

  while( inp( 0x3da ) & 8 )
    ;

  // Wait until we begin vertical retrace.

  while( !(inp( 0x3da ) & 8) )
    ;
  }




int filewrite2(void)
{

   int i,j,k,numobj,numrow;
   char label[20];
   char infst[20];
   char stan[20];
   char smap[20];

   gotoxy (1,22);
   printf("Please enter Filename: ");
   fflush(stdin);  /* flush the input stream in case of bad input */

   scanf("%8s",label);
   strcpy(infst, label);
   strcpy(stan, label);
   strcpy(smap, label);

   strcat(label,".dat");
   strcat(infst,".inf");
   strcat(stan, ".std");
   strcat(smap, ".map");

   if ((map = fopen(smap, "wb"))
	   == NULL)
   {
	  fprintf(stderr, "Cannot open Standard file.\n");
	  return 1;
   }


   if ((std = fopen(stan, "wb"))
	   == NULL)
   {
	  fprintf(stderr, "Cannot open Standard file.\n");
	  return 1;
   }


   if ((out = fopen(infst, "wb"))
	   == NULL)
   {
	  fprintf(stderr, "Cannot open output file.\n");
	  return 1;
   }

   if ((outdata = fopen(label, "wb")) == NULL)
      {
	fprintf(stderr, "Cannot open output file.\n");
	return 1;
      }

 //  for(k=0;k<=1;k++)
  // {
      for(i=0;i<100;i++)
      {
	for(j=0;j<100;j++)
	{
	fprintf(map,"%d\n",layarr[j][i]);
	}

      }
     // screenxstart+=20;
    //  screenystart+=10;
    //  screenx+=20;
    //  screeny+=10;

   //}
      fwrite(animobjects,sizeof(animobjects),1,outdata);


	for (numrow=0;numrow<=9;numrow++)
	{
		for (numobj=0;numobj<=9;numobj++)
		{
		       //	if (animobjects[numrow].fshp[numobj]->flag==1)
		       //	{
			  fwrite(animobjects[numrow].fshp[numobj],272l,1,out);
			  fwrite(animobjects[numrow].fshp[numobj]->shp,256,1,std);
		       //	}
		}
	}

   fclose(map);
   fclose(out);
   fclose(outdata);
   fclose(std);
   return 1;

}

int fileread2(void)
{

	int i,j,k,numobj,numrow;

 //	int indat=0;
	char far *p;
	char label[20];
	char infst[20];
	char smap[20];

	ClearData();		// Clear the Array

	gotoxy (1,22);
	printf("Please enter Filename: ");
	fflush(stdin);  /* flush the input stream in case of bad input */

	scanf("%8s",label);

	strcpy(infst, label);
	strcpy(smap, label);

	strcat(label,".dat");

	strcat(infst,".inf");
	strcat(smap, ".map");

	if ((mapin = fopen(smap, "rb"))
	 == NULL)
	 {
	 fprintf(stderr, "Cannot open Standard file.\n");
	  return 1;
	 }


	if ((in = fopen(infst, "rb"))
	   == NULL)
	{
	  fprintf(stderr, "Cannot open input file.\n");
	  return 1;
	}

	if ((indata = fopen(label, "rb")) == NULL)
	{
	     fprintf(stderr, "Cannot open output file.\n");
	     return 1;
	}

     //	for(k=0;k<maxscreens;k++)
     //	{
	   for(i=0;i<100;i++)
	   {
	     for(j=0;j<100;j++)
	     {
	     fscanf(mapin,"%d",&layarr[j][i]);
	//     printf("%d",layarr[j][i]);

	    // getch();
	     }

	   }
     // screenxstart+=20;
     // screenystart+=10;
     // screenx+=20;
     // screeny+=10;
     // }


	fread(animobjects,sizeof(animobjects),1,indata);


	p = st1;

	for (i=0;i<=9;i++)
	{
		for (j=0;j<=9;j++)
		{
		 animobjects[i].fshp[j]=(struct fshape *)p;
		 p+=sizeof(struct fshape);
		}

	}


	for (numrow=0;numrow<=9;numrow++)
	{

		for (numobj=0;numobj<TOTALSHAPE;numobj++)
		{
		 fread(animobjects[numrow].fshp[numobj],272l,1,in);
		}

	}

   fclose(mapin);
   fclose(in);
   fclose(indata);


   return 1;



}

void ClearData(void)
{
	int t,s,x;

	for (t=0;t<=9;t++)
	{
		for (s=0;s<=9;s++)
		{
			for (x=0;x<16;x++)
			{
				for (y=0;y<16;y++)
				{
				animobjects[t].fshp[s]->shp[x*16+y]=0;
				}
			}
		}

	}




}


void move_shape(void)
{

      //	int vc = 36109;		// Upper Left hand corner of MOve Object for Erase
      //	int oc = 36430;		// Original Coordinate y *320 + x

      // To Move the object all I do is redraw it at a new screen location
      // and since the draw always cuts from location 36430 it will save
      // only 16 x 16 of what it sees. Then erase what went outside that
      // 16 x 16 so it looks clean.


	int i,j,vz;

	vz=0;

	for (i=0;i<16;i++)
		{
		for (j=0;j<16;j++)
		{

			*(screen+j+oc+vz)=animobjects[rw].fshp[sp]->shp[i*16+j];

		}
		vz = vz + 320;


	    }

	oc=toc;

}

void EraseOverflow(void)
{

	int i,j,vz;

	vz=0;


	for (i=0;i<18;i++)
	{
		for (j=0;j<18;j++)
		{

			*(screen+j+vc+vz)=0;

		}
		vz = vz + 320;

	    }



}

void Current_Screen(void)
{
	int ap=0;
	int dn=0;
	int cnt=0;
	int t,s,y;
	long int clr;
	//int curval=0;
	float t1=0;

	rw = 0;
	sp =0;


	for (clr=0;clr<64000;clr++)
		{
		*(screen+clr)=0;
		}

	for (t=0;t<=10;t++)
	{
		for (s=0;s<=19;s++)
		{

		    curval = layarr[s+lox][t+loy];
		   // printf("%d",curval);
		   // getch();
		    if (curval >=0)
		    {
		    t1 = ((float)curval /10);
		    rw = t1;
		    t1 = t1 - rw;
		    t1 = t1 * 10;
		    sp = (int) (t1+.1);

		   // sp = ((t1 - rw)*10);
		    }


		    for (x=0;x<16;x++)
			{
				for (y=0;y<16;y++)
				{
				if (curval>=0)
				{
				*(screen+y+offset+ap+dn+scrollx)=animobjects[rw].fshp[sp]->shp[x*16+y];
				}
				else
				{
				*(screen+y+offset+ap+dn+scrollx)=0;
				}

				}
				offset = offset + 320;      //bytes to next line
				}

				offset = 0;
				ap = ap +16;
			}
		cnt = cnt + 16;
		dn = cnt * 320;
		offset=0;
		ap=0;


		}


	}



void AnimateBackground(int rw, int sp)
{
	// Find the animated objects
	// increase their value by 1 and do over
	// Current Animated objects
	//
	//	Tourch
	//	Water



	int m,n;
      //	int curval;
      //	int t1;
//	printf("Animate");

	// int savedobjects[20][10];

	for (m=0;m<=10;m++)
	{
	     for (n=0;n<=19;n++)
	     {

		curval = layarr[n][m];

		   if (layarr[n][m] >= 35 && layarr[n][m] <= 37 )    // Torch from 44 46
		   {
		       //	t1 = ((float)curval /10);
		      //	rw = t1;
		      //	t1 = t1 - rw;
		      //	t1 = t1 * 10;
		       //	sp = (int) (t1+.1);

		      newobj = layarr[n][m];
		      newobj +=1;
		      savedobjects[n][m]=layarr[n][m];
		      layarr[n][m]=newobj;

			if (savedobjects[n][m] == obj1e)
			{
			 savedobjects[n][m]=obj1s;
			 layarr[n][m]=obj1s;
			}
		     delay(5);

		     AnimObject(rw,sp,m,n);
		   }
		}

	     }


}

void AnimObject(int rw, int sp, int m, int n)
{

	int off,offset=0,x,y;
	int calx,caly,sxy;
	float t1;
      	sxy = (m*16*320)+n*16;

	curval = newobj;

	t1 = ((float)curval /10);
	rw = t1;
	t1 = t1 - rw;
	t1 = t1 * 10;
	sp = (int) (t1+.1);

	//t2 = ((float)curval /10);
	//t3 = t2;
	//t2 = t2 - rw;
	//t2 = t2 * 10;
	//t4 = (int) (t1+.1);


	for (x=0;x<16;x++)
	{
	   for (y=0;y<16;y++)
	   {
		*(screen+y+sxy+offset)=animobjects[rw].fshp[sp]->shp[x*16+y];
	   }

	offset = offset + 320;      //bytes to next line
	}



}