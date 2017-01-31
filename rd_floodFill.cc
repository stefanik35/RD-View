/***********************************************************************
Class:		CSCI 697 (Graphics Independent Study)
Program:	rd_floodFill.cc
Author:		Drew Stefanik
Date Due:	09/30/2016

Purpose:	contains functions and variables to implement four-connected
 			flood fill

Notes:		part of rd_view.exe
			invoked by rd_fill in rd_direct.cc

***********************************************************************/

#include "rd_floodFill.h"
#include "rd_direct.h"
#include "rd_display.h"
#include "rd_error.h"

//global variables
float seedColor [3];

//flood fill master function
//finds the seed color and determines if a valid span is at the seed point
//if so, calls rd_fff4
int rd_floodFill(const float seed_point [3])
{	
	//declare variables
	int xStart = (int) seed_point[0];
	int xEnd;
	int yInitial = (int) seed_point[1];
	bool spanFound = false;
	
	//get the seed color
	rd_read_pixel(xStart, yInitial, seedColor);

	//return if seedColor == drawColor
	//DOES NOT WORK AS INTENDED
	//COLORS OUTPUT FROM rd_read_pixel ARE NOT EQUAL TO THOSE USED TO DRAW THEM
	if(rd_compareColor(drawColor) == true)
	{		
		return RD_OK;
	}
	
	//find if there is a span at the seed point
	spanFound = rd_findSpan(xStart, xEnd, yInitial);
	
	//if there is span at the seed point, call rd_fff4 on that span
	if(spanFound == true)
	{
		rd_fff4(xStart, xEnd, yInitial);
	}
	
	return RD_OK;
}

//determines if there is a span starting at inputs (xS, y)
//returns false if there is no span
//returns true if there is a span and modifies inputs xS and xE to indicate the length of the span
bool rd_findSpan(int & xS, int & xE, int y)
{
	//if y is out of the display, return false
	if(y < 0 || y >= display_ySize)
	{
		return false;
	}
	
	//check color of pixel at the start point
	float pixelColor [3];
	rd_read_pixel(xS, y, pixelColor);
		
	//if pixelColor != seedColor, that pixel is a boundary and the span is 0
	if(rd_compareColor(pixelColor) == false)
	{
		return false;
	}
	
	//set the start and end points to be the same
	xE = xS;
	
	//decrement xStart until a boundary is reached or it is out of the display
	//then increment xStart by 1 so that it points to the first pixel to be filled
	while(true)
	{
		xS--;
		
		if(xS < 0)
		{
			xS++;
			
			break;
		}
		
		rd_read_pixel (xS, y, pixelColor);
		
		if(rd_compareColor(pixelColor) == false)
		{
			xS++;
			
			break;
		}
	}
	
	//increment xEnd until a boundary is reached or it is out of the display
	//xEnd will point to the pixel after the last pixel to be filled
	while(true)
	{
		xE++;
		
		if(xE >= display_xSize)
		{
			break;
		}
		
		rd_read_pixel (xE, y, pixelColor);
		
		if(rd_compareColor(pixelColor) == false)
		{
			break;
		}
	}
	
	return true;
}

//compares color to seedColor
//returns true if they are equal, false if not
bool rd_compareColor(float color[3])
{
	if(color[0] == seedColor[0] && color[1] == seedColor[1] && color[2] == seedColor[2])
	{
		return true;
	}
	
	return false;
}

//color each pixel in the input span from (xS, y) to (xE - 1, y) with drawColor
int rd_fillSpan(int xS, int xE, int y)
{
	for(int i = xS; i < xE; i++)
	{
		rd_write_pixel(i, y, drawColor);
	}
	
	return RD_OK;
}

//fast flood fill 4 algorithm
//this is a four-connected algorithm, not eight-connected (does not check for spans diagonally)
//fills the input span (xS0, y) to (xE0 - 1, y)
//finds any other span above or below the filled span and call itself to fill any found spans
int rd_fff4(int xS0, int xE0, int y)
{
	//fill the input span
	rd_fillSpan(xS0, xE0, y);
	
	//declare variables
	int xS;
	int xE;
	int xSprev;
	int xEprev;
	
	bool spanFound = false;
	
	//for each pixel in the input span
	for(int i = xS0; i < xE0; i++)
	{
		xS = i;
		
		//check if there is a span one pixel directly above it
		spanFound = rd_findSpan(xS, xE, y - 1);
		
		xSprev = xS;
		xEprev = xE;
		
		//if there is a span, call rd_fff4 on that span
		//and skip over the length of that span
		if(spanFound == true)
		{
			rd_fff4(xS, xE, y - 1);
			
			i += xEprev - xSprev - 1;
		}
	}
	
	//for each pixel in the input span
	for(int i = xS0; i < xE0; i++)
	{
		xS = i;
		
		//check if there is a span one pixel directly below it
		spanFound = rd_findSpan(xS, xE, y + 1);
		
		xSprev = xS;
		xEprev = xE;
		
		//if there is a span, call rd_fff4 on that span
		//and skip over the length of that span
		if(spanFound == true)
		{
			rd_fff4(xS, xE, y + 1);
			
			i += xEprev - xSprev - 1;
		}
	}
	
	return RD_OK;
}
