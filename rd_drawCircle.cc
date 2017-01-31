/***********************************************************************
Class:		CSCI 697 (Graphics Independent Study)
Program:	rd_drawCircle.cc
Author:		Drew Stefanik
Date Due:	09/30/2016

Purpose:	contains functions and variables to implement circle drawing

Notes:		part of rd_view.exe
			invoked by rd_circle in rd_direct.cc

***********************************************************************/

#include "rd_drawCircle.h"
#include "rd_direct.h"
#include "rd_display.h"
#include "rd_error.h"

//draws a circle of input radius at input point center in drawColor
//uses bresenham's circle drawing algorithm
//starts at the point (0, radius)
//moves clockwise through the first octant
//always increments x and conditionally increments y based on the value of a decision variable
//uses circular symmetry to find the corresponding points in the other seven octants
int rd_drawCircle(const float center[3], float radius)
{
	//declare variables
	int p0;
	int pk;
	int x;
	int y;
	
	//set the circle center
	int centerX = (int) center[0];
	int centerY = (int) center[1];
	
	//find the initial value of the decision variable
	p0 = 1 - (int) radius;
	pk = p0;
	
	//sets the initial y value to the radius
	y = (int) radius;
	
	//stops at the end of the first octant, at or before x == y
	for(x = 0; x <= y; )
	{
		//write the eight symmetric circle points
		rd_write_pixel(x + centerX, y + centerY, drawColor);
		rd_write_pixel(x + centerX, -y + centerY, drawColor);
		rd_write_pixel(-x + centerX, y + centerY, drawColor);
		rd_write_pixel(-x + centerX, -y + centerY, drawColor);
		rd_write_pixel(y + centerX, x + centerY, drawColor);
		rd_write_pixel(y + centerX, -x + centerY, drawColor);
		rd_write_pixel(-y + centerX, x + centerY, drawColor);
		rd_write_pixel(-y + centerX, -x + centerY, drawColor);
		
		//the new y value is computed based on the next value of x
		//so x is incremented before the calculation
		x++;
		
		//update the decision variable and decrement if appropriate
		if(pk >= 0)
		{
			pk = pk + 2 * x - 2 * y + 2;
			
			y--;
		}
		else
		{
			pk = pk + 2 * x + 1;
		}
	}
	
	return RD_OK;
}
