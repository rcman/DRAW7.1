#include <conio.h>
#include <stdio.h>
#include <stdio.h>
#include <dos.h>
#include <bios.h>


#pragma inline

char password[7];

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
   // cprintf("\r\n");
    textcolor(4);
    cprintf("Help Screen\r\n");
   // cprintf("\r\n");
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
    cprintf("   F8      - Layout Screen \r\n");
    cprintf("   F2      - Copy Object to another Row and Object \r\n");
    cprintf("   F5      - Save Row (Cut / Copy / Paste ) For LOAD/SAVE \r\n");
    cprintf("ArrowKeys  - Move Object UP/DOWN/LEFT/RIGHT \r\n");

    cprintf("\r\n");
    cprintf("   Press Any Key for Draw ");
    getch();



}

