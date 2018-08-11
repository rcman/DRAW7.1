#include <conio.h>
#include <stdio.h>
#include <alloc.h>

#pragma inline

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

int nx,ny,val,mspeed,oldoffset,oldy,oldval,tm,oval,ptflag;
int c,b,st,ac,step,tim;
int inc=5;
int lso=0;
int acxm,acym;          // Actuall Y Mouse and X mouse
char far *point;
char *p;
char far *st1;
char undoarea[260];
int sp,rw=0;

char sc[20];


void ptext(void)
{


    clrscr();
    cprintf("\r\n");
    textcolor(4);
    cprintf("Draw 96\r\n");
    cprintf("Copyright 1996(C)\r\n");
    textcolor(11);
    cprintf("Program Written By:\r\n");
    cprintf("Franco Gaetan & Sean Godsell\r\n");
    cprintf("This Program is NOT FREE\r\n");
    cprintf("Enter Password:");
    getch();



}

void Init_Mode(void)
{
        printf("Initialize 320 x 200 256 Colours\n");
        asm {
                mov ax,0x13
                int 0x10
             }
}

void Close_Mode()
{

        asm {
                mov ax,0x03
                int 0x10
                }
        printf("Graphics Screen Closed Thank You\n");

}


void helpscreen(void)
{


    clrscr();
    cprintf("\r\n");
    textcolor(4);
    cprintf("Help Screen\r\n");
    cprintf("\r\n");
    textcolor(11);
    cprintf("Keys \r\n");
    cprintf("   C       - Copy Object\r\n");
    cprintf("   E       - Erase Object\r\n");
    cprintf("   F       - Flip Object\r\n");
    cprintf("   T       - Turn Object\r\n");
    cprintf("   +       - Move Forward one Object\r\n");
    cprintf("   -       - Move Backward One Object \r\n");
    cprintf("   PgUp    - Move Up One Row\r\n");
    cprintf("   PgDn    - Move Down One Row\r\n");
    cprintf("   S       - Save Data\r\n");
    cprintf("   L       - Load Data\r\n");
    cprintf("   Q       - Quit Program\r\n");
    cprintf("   V       - View All Objects \r\n");
    cprintf("   M       - Move the Object \r\n");
    cprintf("   U       - Undo Last Change\r\n");
    cprintf("   A       - Animate to and from an Object \r\n");
    cprintf("   F1      - Help Screen \r\n");
    cprintf("\r\n");
    cprintf("   Press Any Key for Draw ");
    getch();



}

