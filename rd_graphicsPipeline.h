/***********************************************************************
Class:		CSCI 697 (Graphics Independent Study)
Program:	rd_graphicsPipeline.h
Author:		Drew Stefanik
Date Due:	??/??/2016

Purpose:	header file for rd_graphicsPipeline.cc

Notes:		part of rd_view.exe

***********************************************************************/

#ifndef RD_GRAPHICSPIPELINE_H
#define RD_GRAPHICSPIPELINE_H

#include "rd_direct.h"
#include <stack>

using std::stack;

//global variables
extern stack <xform> xformStack;
extern xform currentXform;
extern xform normalXform;
extern xform worldToClip;
extern xform clipToDevice;

extern float cameraFOV;
extern float nearClip;
extern float farClip;
extern point cameraEye;
extern point cameraAt;
extern vector cameraUp;

//function headers
int rd_xformPoint(xform m, pointH & pH);
xform rd_xformMult(xform m1, xform m2);
xform rd_identity();
xform rd_worldToCamera();
xform rd_cameraToClip();
xform rd_clipToDevice();

pointH rd_convertPoint(const float point[3]);
float rd_magnitudeSquared(vector v);
vector rd_normalize(vector v);
vector rd_subtractPoint(point p1, point p2);
float rd_dotProduct(vector v1, vector v2);
vector rd_crossProduct(vector v1, vector v2);

void rd_printPoint(point p);
void rd_printPointH(pointH pH);
void rd_printVector(vector v);
void rd_printXform(xform m);

#endif
