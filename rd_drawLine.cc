/***********************************************************************
Class:		CSCI 697 (Graphics Independent Study)
Program:	rd_drawLine.cc
Author:		Drew Stefanik
Date Due:	09/30/2016

Purpose:	contains functions and variables to implement line drawing

Notes:		part of rd_view.exe
			invoked by rd_line in rd_direct.cc

***********************************************************************/

#include "rd_drawLine.h"
#include <algorithm>	//copy

using std::copy;

//global variables
float * depthBuffer = NULL;

//line drawing master function
//copies start and end into non-const variables
//determines if lineStart and lineEnd need to be swapped
//and then calls rd_bresenhamLine
int rd_drawLine(const float start[3], const float end[3])
{
	/*ORIGINAL IMPLEMENTATION
	float lineStart[3];
	float lineEnd[3];
	
	copy(start, start+3, lineStart);
	copy(end, end+3, lineEnd);
	
	rd_swapStart(lineStart, lineEnd);
	rd_bresenhamLine(lineStart, lineEnd);
	*/
	
	point p0 = {start[0], start[1], start[2]};
	point p1 = {end[0], end[1], end[2]};
	
	rd_ddaLine(p0, p1);
	
	return RD_OK;
}

//swaps lineStart and lineEnd if
//the x-coordinate of the input point lineStart is greater than
//the x-coordinate of the input point lineEnd
int rd_swapStart(float lineStart[3], float lineEnd[3])
{
	float temp[3];
	
	if(lineStart[0] > lineEnd[0])
	{
		copy(lineStart, lineStart+3, temp);
		copy(lineEnd, lineEnd+3, lineStart);
		copy(temp, temp+3, lineEnd);
	}
	
	return RD_OK;
}

//draws a line from lineStart to lineEnd in drawColor
//uses bresenham's line drawing algorithm
//there are four different cases to cover the eight octants
//lines in the -x octants have their start and end points swapped prior to calling this function
//to reduce the number of cases from eight to four
//either x or y will always be incremented based on the slope of the line
//the other will be conditionally incremented based on the value of a decision variable
int rd_bresenhamLine(float lineStart[3], float lineEnd[3])
{
	//calculate slope components based on lineStart and lineEnd
	int deltaX = (int) lineEnd[0] - (int) lineStart[0];
	int deltaY = (int) lineEnd[1] - (int) lineStart[1];
	
	//declare variables
	int p0;
	int pk;
	int initialAlwaysInc;
	int alwaysInc;
	int condInc;
	
	//always increment x, conditionally increment y, positive slope
	//includes +45 degrees and 0 degrees
	if(deltaX >= deltaY && deltaY >= 0)
	{
		//find the initial value of the decision variable
		p0 = 2 * deltaY - deltaX;
		pk = p0;
		
		//set the start point
		initialAlwaysInc = (int) lineStart[0];
		condInc = (int) lineStart[1];
		
		for(alwaysInc = initialAlwaysInc; alwaysInc <= initialAlwaysInc + deltaX; alwaysInc++)
		{
			//write a pixel at the current point
			rd_write_pixel(alwaysInc, condInc, drawColor);
			
			//update the decision variable
			if(pk >= 0)
			{
				pk = pk + 2 * deltaY - 2 * deltaX;
				
				condInc++;
			}
			else
			{
				pk = pk + 2 * deltaY;
			}
		}
	}
	//always increment x, conditionally increment y, negative slope
	//includes -45 degrees
	else if(deltaX >= -deltaY && deltaY < 0)
	{
		//pretend the slope is positive for the purpose of the decision variable
		deltaY = -deltaY;
		
		//find the initial value of the decision variable
		p0 = 2 * deltaY - deltaX;
		pk = p0;
		
		//set the start point
		initialAlwaysInc = (int) lineStart[0];
		condInc = (int) lineStart[1];
		
		for(alwaysInc = initialAlwaysInc; alwaysInc <= initialAlwaysInc + deltaX; alwaysInc++)
		{
			//write a pixel at the current point
			rd_write_pixel(alwaysInc, condInc, drawColor);
			
			//update the decision variable and decrement if appropriate
			if(pk >= 0)
			{
				pk = pk + 2 * deltaY - 2 * deltaX;
				
				condInc--;
			}
			else
			{
				pk = pk + 2 * deltaY;
			}
		}
	}
	//always increment y, conditionally increment x, positive slope
	//includes +90 degrees
	else if(deltaX < deltaY && deltaY > 0 && deltaX >= 0)
	{
		//find the initial value of the decision variable
		p0 = 2 * deltaX - deltaY;
		pk = p0;
		
		//set the start point
		initialAlwaysInc = (int) lineStart[1];
		condInc = (int) lineStart[0];
		
		for(alwaysInc = initialAlwaysInc; alwaysInc <= initialAlwaysInc + deltaY; alwaysInc++)
		{
			//write a pixel at the current point
			rd_write_pixel(condInc, alwaysInc, drawColor);
			
			//update the decision variable and decrement if appropriate
			if(pk >= 0)
			{
				pk = pk + 2 * deltaX - 2 * deltaY;
				
				condInc++;
			}
			else
			{
				pk = pk + 2 * deltaX;
			}
		}
	}
	//always increment y, conditionally increment x, negative slope
	//includes -90 degrees
	else if(deltaX < -deltaY && deltaY < 0 && deltaX >= 0)
	{
		//pretend the slope is positive for the purpose of the decision variable
		deltaY = -deltaY;
		
		//find the initial value of the decision variable
		p0 = 2 * deltaX - deltaY;
		pk = p0;
		
		//set the start point
		initialAlwaysInc = (int) lineStart[1];
		condInc = (int) lineStart[0];
		
		for(alwaysInc = initialAlwaysInc; alwaysInc >= initialAlwaysInc - deltaY; alwaysInc--)
		{
			//write a pixel at the current point
			rd_write_pixel(condInc, alwaysInc, drawColor);
			
			//update the decision variable and decrement if appropriate
			if(pk >= 0)
			{
				pk = pk + 2 * deltaX - 2 * deltaY;
				
				condInc++;
			}
			else
			{
				pk = pk + 2 * deltaX;
			}
		}
	}
	
	return RD_OK;
}

int rd_ddaLine(point p0, point p1)
{
	//calculate deltas based on lineStart and lineEnd
	int deltaX = (int) (p1.x + 0.5) - (int) (p0.x + 0.5);
	int deltaY = (int) (p1.y + 0.5) - (int) (p0.y + 0.5);
	float deltaZ = p1.z - p0.z;
	
	point p = p0;
	
	p.x = (int) (p.x + 0.5);
	p.y = (int) (p.y + 0.5);
	
	int nSteps;
	float dx, dy, dz;
	
	if(abs(deltaX) > abs(deltaY))
	{
		nSteps = abs(deltaX);
	}
	else
	{
		nSteps = abs(deltaY);
	}
	
	dx = (float) deltaX / nSteps;
	dy = (float) deltaY / nSteps;
	dz = deltaZ / nSteps;
	
	for(int i = 0; i <= nSteps; i++)
	{
		rd_plotPixel(p, drawColor);
		
		p.x += dx;
		p.y += dy;
		p.z += dz;
	}
	
	return RD_OK;
}

int rd_plotPixel(point p, float color[3])
{
	if(p.z < depthBuffer[((int) (p.y + 0.5) * display_xSize + (int) (p.x + 0.5))])
	{
		//rd_write_pixel((int) (p.x + 0.5), (int) (p.y + 0.5), drawColor);
		rd_write_pixel((int) (p.x + 0.5), (int) (p.y + 0.5), color);
		depthBuffer[((int) (p.y + 0.5) * display_xSize + (int) (p.x + 0.5))] = p.z;
	}
	
	return RD_OK;
}
