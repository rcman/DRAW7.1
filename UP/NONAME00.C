\void copyshapeRC(void)
{

    int cp,dn,i;
    int so,sd;


    gotoxy (1,txy);
    printf("Source Object  :");
    cp=getch()-'0';

    if (cp < 0 || cp > 9)
	printf("Object out of Row Range");
    else
    {
    gotoxy (1,txy);
    printf("Source Row :");
    so=getch()-'0';
    }
    if (so < 0 || so > 9)
	printf("Source Row out of Row Range");
    else
    {
	gotoxy (1,txy);
	printf("Destin Object:");
	dn=getch()-'0';
	if (dn <0 || dn >9)
	{
		gotoxy (1,txy);
		printf("Object destination out of Range");
	}
	else
	{
	gotoxy (1,txy);
	printf("Destin Object:");
	sd=getch()-'0';
	}

	if (sd <0 || sd >9)
	{
		gotoxy (1,txy);
		printf("Object destination out of Range");
	}

	else
	{

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

