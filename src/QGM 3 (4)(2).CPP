
// QuickGun Murugan
//Author: Mohit Gvalani

#include<stdio.h>
#include<iostream.h>
#include<graphics.h>
#include<conio.h>
#include<math.h>
#include<dos.h>
#include<stdlib.h>
#include <time.h>

int x = 200,y = 200;
int lh[2][3]={x,y+20,1,x-30,y+50,1};     // left hand
int rh[2][3]={x,y+20,1,x+30,y+50,1};     // right hand
int ll[2][3]={x,y+60,1,x-30,y+100,1};     // left leg
int rl[2][3]={x,y+60,1,x+30,y+100,1};     // right leg
int t[2][3]={x,y+20,1,x,y+60,1};      	// trunk
int h[2][3]={x,y,1,0,0,0};      		// head (centre of circle)
int r[2][3];      			// result matrix of multiplication
double rd[2][3];                        // same as above (for double values)
int rotc = 1;
int rlimit = 30;
union REGS i,o;
struct SREGS s;
int xi,yi,butt;
int speed = 500,hit=0,miss=0,flag=0;

int yo[2][3]={x,y+60,1,x+20,y+100,1};     // right leg
int mo[2][3]={x,y+60,1,x-20,y+100,1};     // left leg
int temp = 0;


extern int far bold_font_far[];
extern int far tscr_font_far[];
void bold_font(void);
void tscr_font(void);

// MOUSE CURSOR - screen mask & pointer bitmap
int cursor[32] = {0x7fff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
		0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,

		/*0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,*/

		0x8000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};

int cursor2[32] = {0x0000,0x0000,0x0000,0x0000,0x8001,0xc003,0xf00f,0xfc3f,
		   0xfc3f,0xf00f,0xc003,0x8001,0x0000,0x0000,0x0000,0x0000,
		   0xffff,0x8001,0xffff,0x8001,0x4002,0x2004,0x1008,0x0240,
		   0x0240,0x0810,0x2004,0x4002,0x8001,0xffff,0x8001,0xffff};

int cursorgun[32] = {0x0000,0x0000,0x0000,0xff00,0xff00,0xff80,0xffe0,0xffe0,
		0xffe0,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,

		0xffff,0xffff,0xffff,0x00ff,0x00ff,0x007f,0x001f,0x001f,
		0x001f,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};
void revert()
{
x=200;
y=200;

lh[0][0]=x;
lh[0][1]=y+20;
lh[0][2]=1;
lh[1][0]=x-30;
lh[1][1]=y+50;
lh[1][2]=1;     // left hand

rh[0][0]=x;
rh[0][1]=y+20;
rh[0][2]=1;
rh[1][0]=x+30;
rh[1][1]=y+50;
rh[1][2]=1;     // right hand

ll[0][0]=x;
ll[0][1]=y+60;
ll[0][2]=1;
ll[1][0]=x-30;
ll[1][1]=y+100;
ll[1][2]=1;     // left leg

rl[0][0]=x;
rl[0][1]=y+60;
rl[0][2]=1;
rl[1][0]=x+30;
rl[1][1]=y+100;
rl[1][2]=1;     // right leg

t[0][0]=x;
t[0][1]=y+20;
t[0][2]=1;
t[1][0]=x;
t[1][1]=y+60;
t[1][2]=1;	// trunk

h[0][0]=x;
h[0][1]=y;
h[0][2]=1;      // head

}

void mouseCursor(int *cur)
{
	i.x.ax = 9;
	i.x.bx = 0;
	i.x.cx = 0;
	i.x.dx = (unsigned)cur;
	segread(&s);
	s.es = s.ds;
	int86x(0x33,&i,&i,&s);
}

void initmouse()
{
	i.x.ax = 0;
	int86(0x33, &i, &o);
	//return (o.x.ax);
}

void getcood(int *butt, int *xi, int*yi)
{
	i.x.ax = 3;
	int86(0x33,&i,&o);
	*butt = o.x.bx;
	*xi = o.x.cx;
	*yi = o.x.dx;
}

void showmptr()
{
	i.x.ax = 1;
	int86(0x33,&i,&o);
}

void hidemptr()
{
	i.x.ax = 2;
	int86(0x33,&i,&o);
}

void mul(int a[2][3], int b[3][3])
{
	for(int i=0;i<2;i++)
		for(int j=0;j<3;j++)
			r[i][j]=0;       // clear the result matrix

	for(i=0;i<2;i++)
		for(j=0;j<3;j++)
			for(int k=0;k<3;k++)
				r[i][j]+=a[i][k]*b[k][j];   // matrix multiplication

	for(i=0;i<2;i++)
		for(j=0;j<3;j++)
			a[i][j] = r[i][j];        // putting new values back to where old values came from

	//int[3][2]* p =&r;
	//return r;
}


void mul(int a[2][3], double b[3][3])
{
	for(int i=0;i<2;i++)
		for(int j=0;j<3;j++)
			rd[i][j]=0.0;       // clear the result matrix

	for(i=0;i<2;i++)
		for(j=0;j<3;j++)
			for(int k=0;k<3;k++)
				rd[i][j]+=(double)a[i][k]*b[k][j];   // matrix multiplication

	for(i=0;i<2;i++)
		for(j=0;j<3;j++)
			a[i][j] =(int)(rd[i][j]+0.5);        // putting new values back to where old values came from

}


void translate(int a[2][3], int x, int y)
{
	int b[3][3] = {1,0,0,0,1,0,x,y,1};      // translation matrix
	mul(a,b);
}

void rotate(int a[2][3], double angle, int xp, int yp)
{
	double x =  angle*3.14/180;	// angle from degree to radians
	double trs[3][3]={{cos(x),sin(x),0},{-sin(x),cos(x),0},{-xp*cos(x)+yp*sin(x)+xp,-xp*sin(x)-yp*cos(x)+yp,1}};
	mul(a,trs);
}

void swap()
{
	for(int i=0; i<2; i++)
		for(int j=0; j<3; j++)
		{
			temp = yo[i][j];
			yo[i][j] = mo[i][j];
			mo[i][j] = temp;
		}
}

//------------------------- MAIN ----------------------------------
void main()
{

       int av2=-8,av3=-8,diff=0,gflag = 0;
	char ans='1';

	/*textmode(64);
	textbackground(15);
	textcolor(0);
	gotoxy(37,2);
	cout<<"STAGE 1\n";*/

	settextstyle(3,0,2);
	outtextxy(250,100,"Quick Gun Murugan");
	settextstyle(3,0,1);
	outtextxy(100,120,"1.Play the Game");
	outtextxy(100,140,"2.HighScores");
	outtextxy(100,160,"3.Instructions");
	outtextxy(120,180,"3a.Select the cursor to play the Game");
	outtextxy(120,200,"3b.Hit the Head(Circular part) of Person");
	outtextxy(120,220,"3c.For every Hit your Score will be increased by 1");
	outtextxy(120,240,"3d.For every Miss your Score will be decreased by 3");

	outtextxy(400,360,"Made by:");
	outtextxy(430,380,"Mohit Gvalani(076039)");
	outtextxy(430,400,"Tarun Hasija(076041)");
	outtextxy(430,420,"Dhiraj Jagiasi(076046)");
	getch();
	setgraphmode(2);
	registerbgidriver(EGAVGA_driver);
	registerfarbgifont(sansserif_font_far);
	registerfarbgifont(gothic_font_far);
	registerfarbgifont(triplex_font_far);
	cout<<endl<<registerfarbgifont(bold_font_far);
	cout<<endl<<registerfarbgifont(tscr_font_far);
	//cout<<endl<<registerbgifont(bold_font);
	//cout<<endl<<registerbgifont(tscr_font);
	//getch();

	int gdriver = VGA;
	int gmode = 2;
	initgraph(&gdriver, &gmode, "C:\\TC\\BGI");           // initialize graph


	gotoxy(20,20);
	setfillstyle(11,LIGHTGRAY);
	bar(0,0,getmaxx(),getmaxy());
	settextstyle(4,0,8);
	setcolor(WHITE);
	outtextxy(175,100,"STAGE 1");
	settextstyle(3,0,6);
	setcolor(GREEN);
	outtextxy(120,200,"Catch the Zombie");

	char mess[] = "\nMr.Murugan, you have to catch a Zombie before he runs away.\nYou'll be given a Maths Question with 4 possible answers.\nPress the appropriate number to give your answer(1,2,3 or 4).\nIf your answer is correct, the Zombie will be caught. Else he'll escape.\nGood Luck!\n";
	int som = sizeof(mess)/sizeof(char);
	for(int i=0; i<som; i++)
	{
		cout<<mess[i];
		delay(60);
	}
	//gotoxy(20,30);
	textcolor(RED+BLINK);
	cout<<"Press any key to start";

	getch();

	//setgraphmode(1);
	setgraphmode(2);

	setfillstyle(0,BLACK);
	bar(0,0,getmaxx(),getmaxy());
	setcolor(150);
	setfillstyle(1,150);
	setlinestyle(0,1,3);
	bar3d(-50,350,getmaxx()-50,getmaxy(),120,10);
	line(getmaxx()-50,getmaxy(),getmaxx()+100,getmaxy()-100);
	setcolor(0);
	line(70,348,70,263);
	setlinestyle(0,0,1);
	setcolor(15);
	settextstyle(1,0,0);
	settextstyle(10,0,2);
	setcolor(90);
	outtextxy(150,400,"Quick Gun Murugan");
	settextstyle(3,0,4);
	int deltime=20;


	line(lh[0][0],lh[0][1],lh[1][0],lh[1][1]);
	line(rh[0][0],rh[0][1],rh[1][0],rh[1][1]);
	line(mo[0][0],mo[0][1],mo[1][0],mo[1][1]);
	line(yo[0][0],yo[0][1],yo[1][0],yo[1][1]);
	line(t[0][0],t[0][1],t[1][0],t[1][1]);
	circle(h[0][0],h[0][1],20);



	randomize();
	int n1 = random(100);
	int n2 = random(100);
	int n3 = 0;
	n3 = n1*n2;
	/*gotoxy(0,0);
	moverel(x,y);
	delline();delline();delline();delline();delline();delline();*/
	cout<<"\nQ: "<<n1<<" x "<<n2<<" = ?\n";

	cout<<"1."<<random(5000)<<"   2."<<n3<<"   3."<<random(2000)<<"   4."<<random(8000);

	do
	{
 //-------------------------STEP1-------RED------------------------
		do
		{
			setcolor(0);
			circle(h[0][0],h[0][1],20);
			line(lh[0][0],lh[0][1],lh[1][0],lh[1][1]);
			line(rh[0][0],rh[0][1],rh[1][0],rh[1][1]);
			line(yo[0][0],yo[0][1],yo[1][0],yo[1][1]);
			line(mo[0][0],mo[0][1],mo[1][0],mo[1][1]);
			line(t[0][0],t[0][1],t[1][0],t[1][1]);

			setcolor(15);
			rotate(rh,-av2/4,yo[1][0],yo[1][1]);
			rotate(lh,-av2/4,yo[1][0],yo[1][1]);
			rotate(yo,-av2/4,yo[1][0],yo[1][1]);
			rotate(mo,-av2/4,yo[1][0],yo[1][1]);
			rotate(t,-av2/4,yo[1][0],yo[1][1]);
			rotate(h,-av2/4,yo[1][0],yo[1][1]);

			line(lh[0][0],lh[0][1],lh[1][0],lh[1][1]);
			line(rh[0][0],rh[0][1],rh[1][0],rh[1][1]);
			line(mo[0][0],mo[0][1],mo[1][0],mo[1][1]);
			line(yo[0][0],yo[0][1],yo[1][0],yo[1][1]);
			line(t[0][0],t[0][1],t[1][0],t[1][1]);
			circle(h[0][0],h[0][1],20);

			delay(deltime);
			if(kbhit())
			{
				flag=1;
				break;
			}
		}
		while(yo[0][0]<=yo[1][0]);

		if(flag==1)
			break;

   //-------------------------STEP2------------GREEN-------------------
		 diff = yo[1][0]-mo[1][0];
		 //cout<<diff<<endl<<mo[1][0]-yo[1][0];
		 do
		 {
			setcolor(0);
			line(mo[0][0],mo[0][1],mo[1][0],mo[1][1]);

			setcolor(15);
			rotate(mo,av3/2,mo[0][0],mo[0][1]);
			line(mo[0][0],mo[0][1],mo[1][0],mo[1][1]);
			delay(deltime);

			if(kbhit())
			{
				flag=1;
				break;
			}
		 }
		 while(mo[1][0]-yo[1][0]<=diff);

		 if(flag==1)
			break;

 //-------------------------STEP3----------BLUE---------------------

		while(mo[1][1] < 301)
		{
			setcolor(0);
			circle(h[0][0],h[0][1],20);
			line(lh[0][0],lh[0][1],lh[1][0],lh[1][1]);
			line(rh[0][0],rh[0][1],rh[1][0],rh[1][1]);
			line(yo[0][0],yo[0][1],yo[1][0],yo[1][1]);
			line(mo[0][0],mo[0][1],mo[1][0],mo[1][1]);
			line(t[0][0],t[0][1],t[1][0],t[1][1]);

			setcolor(15);
			rotate(rh,-av2/4,yo[1][0],yo[1][1]);
			rotate(lh,-av2/4,yo[1][0],yo[1][1]);
			rotate(yo,-av2/4,yo[1][0],yo[1][1]);
			rotate(mo,-av2/4,yo[1][0],yo[1][1]);
			rotate(t,-av2/4,yo[1][0],yo[1][1]);
			rotate(h,-av2/4,yo[1][0],yo[1][1]);

			line(lh[0][0],lh[0][1],lh[1][0],lh[1][1]);
			line(rh[0][0],rh[0][1],rh[1][0],rh[1][1]);
			line(mo[0][0],mo[0][1],mo[1][0],mo[1][1]);
			line(yo[0][0],yo[0][1],yo[1][0],yo[1][1]);
			line(t[0][0],t[0][1],t[1][0],t[1][1]);
			circle(h[0][0],h[0][1],20);

			delay(deltime);

			if(kbhit())
			{
				flag=1;
				break;
			}
		}

		if(flag==1)
			break;

  //-------------------------STEP4--------YELLOW-----------------------

		do
		{
			setcolor(0);
			circle(h[0][0],h[0][1],20);
			line(lh[0][0],lh[0][1],lh[1][0],lh[1][1]);
			line(rh[0][0],rh[0][1],rh[1][0],rh[1][1]);
			line(t[0][0],t[0][1],t[1][0],t[1][1]);

			setcolor(15);
			rotate(rh,av2/4,t[1][0],t[1][1]);
			rotate(lh,av2/4,t[1][0],t[1][1]);
			rotate(t,av2/4,t[1][0],t[1][1]);
			rotate(h,av2/4,t[1][0],t[1][1]);

			line(lh[0][0],lh[0][1],lh[1][0],lh[1][1]);
			line(rh[0][0],rh[0][1],rh[1][0],rh[1][1]);
			line(t[0][0],t[0][1],t[1][0],t[1][1]);
			circle(h[0][0],h[0][1],20);

			delay(deltime);
		}
		while(t[0][0]>=t[1][0]);

		if(kbhit())
		{
			flag=1;
			break;
		}
		if(flag==1)
			break;


 // -------------------------------STEP5----------------------------

		swap();
		setcolor(0);
		circle(h[0][0],h[0][1],20);
		h[0][0] = t[0][0];
		h[0][1] = t[0][1] - 20;
		setcolor(15);
		circle(h[0][0],h[0][1],20);

 //------------------------------------------------------------------

		if(lh[1][0]>getmaxx())
			break;

	}
	while(1);
	if(flag==1)
		ans = getch();

	for(int j=0; j<=getmaxy(); j++)
		for(int k=0; k<=getmaxx(); k++)
			putpixel(k,j,0);

	gotoxy(35,15);
	if((ans=='2'))
	{
		cout<<"STAGE CLEARED";
		cout<<"\n\n\n\n\n\n              Press any key to continue.";
	}
	else
 {
	cout<<"WRONG ANSWER!\nThe Zombie escaped.\nYou're a waste!";
	cout<<"\n\n\n\n\n\n              Press any key to continue.";
	gflag = 1;
 }
	getch();


 if(gflag==0)
 {
	gotoxy(20,15);
	setfillstyle(11,LIGHTGRAY);
	bar(0,0,getmaxx(),getmaxy());
	settextstyle(4,0,8);
	setcolor(WHITE);
	outtextxy(150,50,"STAGE 2");
	settextstyle(3,0,6);
	setcolor(GREEN);
	outtextxy(140,150,"HIT the Zombie");

	char mess2[] = "\nMr.Murugan, the Zombie is now captured and straped.\nYour job is to shoot at his head and score as many points as you can.\nIf your score is the highest, you'll earn the title of 'QUCIKGUN'.\nA HIT is 1 point, a MISS is -3 points.\nIf you hit at dead centre of his head, you get +20 points.\nYou only get 3 MISSes, after which this stage terminates.\nPress appropriate number to choose gunpoint.\n1.Point(default) \n2.Gun\n3.Arrow \n4.Hour Glass \nPress any key to abort in between.\nGood Luck!\n";
	som = sizeof(mess2)/sizeof(char);
	for(i=0; i<som; i++)
	{
		cout<<mess2[i];
		delay(60);
	}

	//gotoxy(20,30);
	textcolor(RED+BLINK);
	cout<<"Press any key to start";

	//setgraphmode(1);
	//setgraphmode(2);
	char mouseopt;
	mouseopt = getch();
	char counter[15];

	setfillstyle(0,BLACK);
	bar(0,0,getmaxx(),getmaxy());






	 revert();
	//int gdriver = DETECT,gmode,errorcode;

	//cout<<"Driver : "<<getdrivername()<<"\nMode : "<<getgraphmode();
	int color = WHITE;
	//line(0,y+100,getmaxx(),y+100);            // draw ground
	int av = -2;			   	  // in degrees
	//rotate(lh,50,200,220);//lh[0][0],lh[0][1]);
	//rotate(rh,-50,200,220);//x,y+20);
	//setactivepage(0);
	initmouse();                      
	if(mouseopt=='2')
		mouseCursor(cursorgun);
	else if(mouseopt=='3')
	{}
	else if(mouseopt=='4')
		mouseCursor(cursor2);
	else
		mouseCursor(cursor);

	showmptr();
	getcood(&butt, &xi, &yi);
	//cout<<butt<<" "<<xi<<" "<<yi;
	setcolor(150);
	setfillstyle(1,150);
	setlinestyle(0,1,3);
	bar3d(-50,350,getmaxx()-50,getmaxy(),120,10);
	//bar(70,0,getmaxx(),260);
	line(getmaxx()-50,getmaxy(),getmaxx()+100,getmaxy()-100);
	setcolor(0);
	line(70,348,70,263);
	setlinestyle(0,0,1);
	setcolor(15);
	settextstyle(4,0,4);
	//settextstyle(10,0,2);
	setcolor(0);
	outtextxy(150,400,"Quick Gun Murugan");
	settextstyle(3,0,4);
	setcolor(90);
	outtextxy(10,10,"HIT : 0");
	setcolor(20);
	outtextxy(10,60,"MISS : 0");
	flag=0;
	do
	{
		hidemptr();
		setcolor(0);
		circle(x,y,20);
		line(lh[0][0],lh[0][1],lh[1][0],lh[1][1]);
		line(rh[0][0],rh[0][1],rh[1][0],rh[1][1]);
		line(ll[0][0],ll[0][1],ll[1][0],ll[1][1]);
		line(rl[0][0],rl[0][1],rl[1][0],rl[1][1]);
		line(t[0][0],t[0][1],t[1][0],t[1][1]);
		showmptr();
		//clrman();
		x+=2;
		setcolor(color);

		circle(x,y,20);
		translate(lh,2,0);
		translate(rh,2,0);
		translate(ll,2,0);
		translate(rl,2,0);
		translate(t,2,0);
		if(rotc>40)
		{
			av *= -1;
			rotc = 0;
		}

		//rotate(lh,av,/*200,220);*/lh[0][0],lh[0][1]);
		//rotate(rh,-av,/*200,220);*/x,y+20);
		//rotate(ll,av,/*200,220);*/x,y+60);
		//rotate(rl,-av,/*200,220);*/x,y+60);*/
		rotc++;
		line(lh[0][0],lh[0][1],lh[1][0],lh[1][1]);
		line(rh[0][0],rh[0][1],rh[1][0],rh[1][1]);
		line(ll[0][0],ll[0][1],ll[1][0],ll[1][1]);
		line(rl[0][0],rl[0][1],rl[1][0],rl[1][1]);
		line(t[0][0],t[0][1],t[1][0],t[1][1]);
		//cout<<lh[0][0]<<" "<<lh[0][1]<<" "<<lh[1][0]<<" "<<lh[1][1]<<"\n";
	int count = speed;
	do
	{
		//cout<<count<<" ";
		count--;
		getcood(&butt, &xi, &yi);
		delay(0); // FOR SLOWER SPEED, MAKE DELAY 1 or 2.
		if((butt & 1)==1)
		{
			line(xi-2,yi-4,xi-10,yi-8);
			line(xi-2,yi-2,xi-6,yi-4);
			line(xi-2,yi,xi-4,yi);
			line(xi-2,yi+2,xi-6,yi+4);
			line(xi-2,yi+4,xi-10,yi+8);
			sound(10);

			delay(100);

			nosound();
			hidemptr();
			setcolor(0);
			line(xi-2,yi-4,xi-10,yi-8);
			line(xi-2,yi-2,xi-6,yi-4);
			line(xi-2,yi,xi-4,yi);
			line(xi-2,yi+2,xi-6,yi+4);
			line(xi-2,yi+4,xi-10,yi+8);
			showmptr();


			getcood(&butt, &xi, &yi);

			if((butt & 1)==0)
			{
			    if((xi>x-20)&&(xi<x+20)&&(yi>y-20)&&(yi<y+20))
			    {
				//cout<<" HIT";
				hit++;
				if((xi==x)&&(yi==y))
					hit+=19;
				setfillstyle(1,0);
				setcolor(10);
				bar(300,50,400,100);
				bar(10,10,150,60);
				outtextxy(300,50,"HIT");
				sprintf(counter,"HIT :  %d",hit);
				outtextxy(10,10,counter);
				speed/=2;
				continue;
			    }
			    else
			    {
				//cout<<" MISS";
				miss++;
				setfillstyle(1,0);
				setcolor(20);
				bar(300,50,350,100);
				bar(50,60,150,110);
				outtextxy(300,50,"MISS");
				sprintf(counter,"MISS :  %d",miss);
				outtextxy(10,60,counter);

				if(miss==3)
				{
					flag=1;
					break;
				}
			   }
			}

		}
	}while(count>0);
		if(x>getmaxx()+30)
		{
			x=10;
			lh[0][0]=x;
			rh[0][0]=x;
			ll[0][0]=x;
			rl[0][0]=x;
			t[0][0]=x;
			lh[1][0]=x-30;
			ll[1][0]=x-30;
			rh[1][0]=x+30;
			rl[1][0]=x+30;
			t[1][0]=x;
		}
		if(flag==1)
		{	break;
			//getch();
			//getch();
		}
		//delay(speed);
	}
	while(!kbhit());
	if(flag==0)
		getch();
	//delay(1000);
	//clrscr();
	//setactivepage(1);
	for(j=0; j<=getmaxy(); j++)
		for(k=0; k<=getmaxx(); k++)
			putpixel(k,j,0);
 }
	//delay(500);
	hidemptr();
	setfillstyle(6,200);
	bar(0,0,getmaxx(),getmaxy());
	setcolor(87);
	settextstyle(4,0,8);
	outtextxy(50,100," GAME OVER");
	settextstyle(0,0,2);
	setcolor(76);
	char score[20];
	sprintf(score,"Your Score:  %d",(hit-3*miss));
	outtextxy(190,220,score);
	settextstyle(3,0,8);
	setcolor(10);
	outtextxy(180,300,"MIND IT!");
	settextstyle(1,0,1);
	setcolor(WHITE);
	outtextxy(170,450,"Press any key to contiue.");
	//setvisualpage(1);
	getch();
}
