//Computing For Graphics Assignemnt
//Turtle Graphics and Spiralterals 

#include <stdio.h>
#include <SDL2/SDL.h>
//#include <stdlib.h>
//#include<SDL2/SDL_image.h> 
//include "Surfaces.h"  
//include "turtle.h"
#include <math.h>

//pres setting the width and height, it is used later on for window, screen and texture size
#define WIDTH 1000
#define HEIGHT 1000

//created a struct called RGB colour, I use this struct later on for line colour and background colour
typedef struct RGBcolour
{
	Uint8 r;
	Uint8 g;
	Uint8 b;
} 
RGBcolour;

// created a struct called TORT, for the turtle and spirolateral
typedef struct TORT 
{
	double posx;
	double posy;
	float angle;
	int initiallength;
	int newlength;
	int repetitions;
	int segments;
	SDL_bool penstate;
}
TORT;

//The first function we call in the main() function is SDL_Init(). This call of SDL_Init() initializes all the SDL subsystems so we can start using SDL's graphics functions.
//FOR ONE IMAGE   

//These are prototype functions 
// This function draws the spirolateral
int drawLine(SDL_Surface *,int,int,int,int,RGBcolour*); 
// These functions control the creations and destruction of the window, surface and texture
void init(SDL_Window **, SDL_Renderer **, SDL_Surface **, SDL_Texture **, RGBcolour);
void event();
void quit(SDL_Renderer *, SDL_Window *);
// These functions are used to control the turtle
void turn(TORT *,float );
void forward(SDL_Surface *,TORT *,RGBcolour*);
// This function resizes the screen depending on the size of the spirolateral
void resizeScreen(TORT *, float);
//to save the image as an animation, or a single image
void saveAnimation(SDL_Surface *,int);
void saveImage(SDL_Surface *);

void chooseParam(TORT *, RGBcolour *, RGBcolour *);

//check if spirolateral closes
int checkClose(TORT *t,SDL_Surface *surface, SDL_Texture *texture, SDL_Renderer **renderer, RGBcolour *, float alpha);


// This function is used to control the angle in which the turtle turns, the user can change this value
void turn(TORT *t,float a)
{
	t->angle+=a;
}
// This function is used to control the direction in which the turtle moves, it is affected by the angle the user inputs
void forward(SDL_Surface *s,TORT *t,RGBcolour *lineColour)
{
	//float alpha=(t->angle*180)/M_PI; //converting degrees to radians
	double resolvedx=t->newlength*cos(t->angle);
	double resolvedy=t->newlength*sin(t->angle);
	double newx = t->posx + resolvedx;
	double newy= t->posy + resolvedy;
	//printf("%f,%f\t%f,%f\t%f\n",t->posx,t->posy,newx,newy,t->angle);
	drawLine(s,(int)t->posx,(int)t->posy,(int)newx,(int)newy,lineColour);
	t->posx=newx;
	t->posy=newy;
}

// This is the main function, where I create a window, surface, renderer and texture and where I call the function to make the spirolateral
int main( int argc, char* args[] )
{
	TORT turtle; 
	SDL_Surface *surface=NULL;
	SDL_Window *window=NULL; /*= window=SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN)*/
	SDL_Renderer *renderer=NULL;//= SDL_CreateRenderer(window, -1,0);
	SDL_Texture *texture=NULL;

	surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0xff000000, 0x00ff0000, 0x0000ff00,0x000000ff);
	
	//Notice that these are pointers to SDL surfaces. The reason is that 1) we'll be dynamically allocating memory to load images and 2) it's better to reference an image by memory location.
	//in main function I inisalize SDL, create window and renderer and load the image. 	
	
	// This used the struct RGBcolour, I created earlier, I have renamed it bgColour so I can use it in other relevant functions
	RGBcolour bgColour;
	bgColour.r = 255;
	bgColour.g = 255;
	bgColour.b = 0;
	
	// This used the struct lineColour, I created earlier, I have renamed it lineColour so I can use it in other relevant functions
	RGBcolour lineColour;
	lineColour.r=255;
	lineColour.g=56;
	lineColour.b=87;
		
	chooseParam(&turtle, &lineColour, &bgColour);
	init(&window, &renderer, &surface, &texture, lineColour);
	
	
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, bgColour.r, bgColour.g, bgColour.b));

	//have the user change these 		
	float resolvedx, resolvedy, newx, newy;
	turtle.posx=500;
	turtle.posy=500;
	turtle.angle = turtle.angle*(M_PI/180); //converting degrees to radians
	float alpha=turtle.angle;

	checkClose(&turtle, &surface, &texture, &renderer, &lineColour, alpha);
	
	event();
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	quit(renderer, window);
	return 0;

}

void init(SDL_Window **window, SDL_Renderer **renderer, SDL_Surface **surface, SDL_Texture **texture, RGBcolour lineColour)
{
	SDL_Init(SDL_INIT_EVERYTHING);
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			printf("SDL_Init Error: %s\n", 
			SDL_GetError());
			
		}
	*window=SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (*window==NULL){
			printf("Could not create window: %s\n", SDL_GetError());
			fflush(stdout);
		}
	
	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
	if (*renderer==NULL)
	{
		printf("Could not create renderer: %s\n",SDL_GetError());
			fflush(stdout);
	}
	
	*surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32,0, 0, 0,0);
	if (*surface==NULL)
	{
		printf("Could not create surface: %s\n", SDL_GetError());
			fflush(stdout);
	}
	
	*texture = SDL_CreateTextureFromSurface(*renderer,*surface);
	if (*texture==NULL)
	{
		printf("Could not create texture: %s\n", SDL_GetError());
			fflush(stdout);
	}	
	

}

void event()
{	
	int quit =1;
	while (quit ==1)
	{	
		SDL_Event incomingEvent;
		while(SDL_PollEvent(&incomingEvent)) 
		{
			if(incomingEvent.type == SDL_QUIT)
			{
				quit =0;
				break;
			}
		}
		
		
	} 
}

void quit(SDL_Renderer *renderer, SDL_Window *window)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}                                                                                                                                                                                                                                                                                                         

//-----------------------------------------------------------------------------------------
//Eike's code
int drawLine(SDL_Surface *img,int x0,int y0,int xn,int yn,RGBcolour *col)
{
	int dx = abs(xn-x0), sx = x0<xn ? 1 : -1;
	int dy = abs(yn-y0), sy = y0<yn ? 1 : -1;
	int error = (dx>dy ? dx : -dy)/2, e2;
	 Uint32 pixel = SDL_MapRGB(img->format,col->r,col->g,col->b);
	 Uint32 *pixels = (Uint32*)img->pixels; /* pixel array in the SDL_Surface record */
	while(1)
	{
		/* draw point only if coordinate is valid, i.e. within the pixel array */
		/* x0+y0*img->w is the 1D offset location for the 2D pixel coordinate (x0,y0) */
		if(x0>=0 && x0<img->w && y0>=0 && y0<img->h) pixels[x0+y0*img->w]=pixel;
		if(x0==xn && y0==yn) break;
		e2 = error;
		if(e2 >-dx) { error -= dy; x0 += sx; }
		if(e2 < dy) { error += dx; y0 += sy; }
	}
	return(0);
}

////Eike's code
//-----------------------------------------------------------------------------------------------


void saveImage(SDL_Surface *surface)
{
	SDL_SaveBMP(surface, "Spirolateral.bmp");
}

void saveAnimation(SDL_Surface *surface,int n)
{
	char name[20];
	sprintf(name,"Spirolateral%03d.bmp",n);
	SDL_SaveBMP(surface,name);
}


void resizeScreen(TORT *turtle, float a)
{
	int i,j;
	double resolvedx, resolvedy,newx,newy;
	turtle->posx=0.0;
	turtle->posy=0.0;
	float minX = 0.0;
	float maxX = 0.0;
	float minY = 0.0;
	float maxY = 0.0;
	turtle->newlength=turtle->initiallength;
	printf("posx %f, posy %f\n", turtle->posx, turtle->posy);
	
	for (int j=1; j<= turtle->repetitions; j ++)
	{	
		for (int i = 1; i <= turtle->segments; i++)
		{
			resolvedx=(turtle->initiallength)*cos(turtle->angle)*i;
			resolvedy=(turtle->initiallength)*sin(turtle->angle)*i;
			newx = turtle->posx + resolvedx;
			newy= turtle->posy + resolvedy;
			turtle->posx=newx;
			turtle->posy=newy;
			turtle->angle+=a;
				if(turtle->posx>maxX) maxX = turtle->posx;
				if(turtle->posx<minX) minX = turtle->posx;
				if(turtle->posy>maxY) maxY = turtle->posy;
				if(turtle->posy<minY) minY = turtle->posy;
		}
	}
	
	printf("all min max values %f, %f, %f, %f\n", maxX,maxY,minX, minY);
	turtle->posx=500-((maxX+minX)/2);
	turtle->posy=500-((maxY+minY)/2);
	printf("min max %f, %f\n", (maxX+minX)/2,(maxY+minY)/2);
	printf("posx %f, posy %f\n", turtle->posx, turtle->posy);
}

//CHOOSING  PARAMETERS
void chooseParam(TORT *t, RGBcolour *lineColour, RGBcolour *bgColour)
{
	int value, presetValue;
	printf("Please select an option by inputing a value from 1-2:\n (1) Preset Spirolateral\n (2) Custom Spirolateral\n");
	scanf("%d", &value);
	
	switch(value)
	{
		case 1:
		{
			printf("Please choose from 4 different preset parameters:\n");
			printf("(1), Angle= 60, Inital length = 50, Repetitions = 6, Segments = 5, Pen Colour = White, Background colour = Black\n");
			printf("(2), Angle= 118, Inital length = 10, Repetitions = 13, Segments = 15, Pen Colour = Dark Blue, Background colour = Cian\n");
			printf("(3), Angle= 20, Inital length = 30, Repetitions = 20, Segments = 5, Pen Colour = Pink, Background colour = Red\n");
			printf("(4), Angle= 140, Inital length = 50, Repetitions = 9, Segments = 8, Pen Colour = Black, Background colour = Green\n");
		
		scanf("%d", &presetValue);
		
		switch(presetValue)
		{
			case 1:
			{
				t->angle = 60;
				t->initiallength = 50;
				t->repetitions = 6;
				t->segments = 5;
				lineColour->r = 255;
				lineColour->b = 255;
				lineColour->g = 255;
				bgColour->r=0;
				bgColour->g=0;
				bgColour->b=0;
				break;
			}
		
			case 2:
			{
				t->angle = 40;
				t->initiallength = 10;
				t->repetitions = 3;
				t->segments = 15;
				lineColour->r = 50;
				lineColour->b = 50;
				lineColour->g = 100;
				bgColour->r=0;
				bgColour->g=255;
				bgColour->b=255;
				break;
			}
			
			case 3:
			{
				t->angle = 118;
				t->initiallength = 10;
				t->repetitions = 13;
				t->segments = 15;
				lineColour->r = 255;
				lineColour->b = 204;
				lineColour->g = 204;
				bgColour->r=209;
				bgColour->g=23;
				bgColour->b=23;
				break;
			}
			
				case 4:
			{
				t->angle = 140;
				t->initiallength = 50;
				t->repetitions = 9;
				t->segments = 8;
				lineColour->r = 255;
				lineColour->b = 255;
				lineColour->g = 255;
				bgColour->r=50;
				bgColour->g=100;
				bgColour->b=50;
				break;
			}

		}
		
		break;

		}
		case 2:
		{
			printf("Please enter custom parameters\n");
			printf("Please enter an angle value, greater than 0:\n");
			scanf("%f", &t->angle);
			printf("angle%f", t->angle);
			printf("Please enter an initial length value:\n");
			scanf("%d", &t->initiallength);
			printf("Please enter a number of segments value less than or equal to 15:\n");
			scanf("%d", &t->segments);
			break;
		}
		
	}
}



int checkClose(TORT *t, SDL_Surface *surface, SDL_Texture *texture, SDL_Renderer *renderer, RGBcolour *lineColour, float alpha)
{
	
	
	for (t->repetitions=2; t->repetitions<360; t->repetitions++)
	{
		int totalAngle;
		totalAngle = ((t->angle)*(t->segments)*(t->repetitions));
		if(totalAngle % 360 == 0)
		{
			for (int j = 0; j < t->repetitions; j++)
				{
				for (int i = 1; i <= t->segments; i++)
					{
						t->newlength=i*(t->initiallength);
						forward(surface,t,lineColour);
						turn(t, alpha);
						
						SDL_UpdateTexture(texture,NULL,surface->pixels,surface->pitch);
						SDL_RenderCopy(renderer, texture, NULL, NULL);
						SDL_RenderPresent(renderer);
						SDL_Delay(10);
						
					}
				}
					
		break;
		}
	}
	return(t->repetitions);
	int i;
	int savetype=1;
	if (savetype==2)
	{
		saveAnimation(surface,(t->repetitions*(t->segments))+i); //actual frame number
	}
	
	if (savetype==1)
	{
		saveImage(surface);
	}
}

