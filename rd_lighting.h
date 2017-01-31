/***********************************************************************
Class:		CSCI 697 (Graphics Independent Study)
Program:	rd_lighting.h
Author:		Drew Stefanik
Date Due:	12/09/2016

Purpose:	header file for rd_lighting.cc

Notes:		part of rd_view.exe

***********************************************************************/

#ifndef RD_LIGHTING_H
#define RD_LIGHTING_H

#include "rd_direct.h"

//global variables
extern lightAmbient globalAmbient;
extern lightFar globalFar [30];
extern int counterFar;
extern lightPoint globalPoint [30];
extern int counterPoint;

extern float coefAmbient;
extern float coefDiffuse;
extern float coefSpecular;

extern float colorSurface [3];
extern float colorSpecular [3];
extern float exponentSpecular;

extern bool vertexColorFlag;
extern bool vertexNormalFlag;
extern bool vertexTextureFlag;
extern bool interpolationFlag;

extern vector viewVector;
extern vector normalVector;

extern attrPoint surfacePointValues;

//void (*surfaceShader)(float surfaceColor [3]);

//function headers
int rd_matteShader(float (&colorPixel) [3]);

float rd_diffuseLight(int colorComponent);

#endif
