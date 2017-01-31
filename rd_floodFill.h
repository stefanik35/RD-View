/***********************************************************************
Class:		CSCI 697 (Graphics Independent Study)
Program:	rd_floodFill.h
Author:		Drew Stefanik
Date Due:	09/30/2016

Purpose:	header file for rd_floodFill.cc

Notes:		part of rd_view.exe

***********************************************************************/

#ifndef RD_FLOOD_FILL_H
#define RD_FLOOD_FILL_H

#include "rd_direct.h"

//globabl variables
extern float seedColor [3];

//function headers
int rd_floodFill(const float seed_point [3]);

bool rd_findSpan(int & xS, int & xE, int y);

bool rd_compareColor(float pixelColor[3]);

int rd_fillSpan(int xS, int xE, int y);

int rd_fff4(int xS0, int xE0, int y);

#endif
