/***********************************************************************
Class:		CSCI 697 (Graphics Independent Study)
Program:	rd_drawLine.h
Author:		Drew Stefanik
Date Due:	09/30/2016

Purpose:	header file for rd_drawLine.cc

Notes:		part of rd_view.exe

***********************************************************************/

#ifndef RD_DRAWLINE_H
#define RD_DRAWLINE_H

#include "rd_direct.h"

//global variables
extern float * depthBuffer;

//function headers
int rd_drawLine(const float start[3], const float end[3]);

int rd_swapStart(float lineStart[3], float lineEnd[3]);

int rd_bresenhamLine(float lineStart[3], float lineEnd[3]);

int rd_ddaLine(point p0, point p1);

int rd_plotPixel(point p, float color[3]);

#endif
