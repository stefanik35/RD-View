/***********************************************************************
Class:		CSCI 697 (Graphics Independent Study)
Program:	rd_drawObject.h
Author:		Drew Stefanik
Date Due:	09/30/2016

Purpose:	header file for rd_drawObject.cc

Notes:		part of rd_view.exe

***********************************************************************/

#ifndef RD_DRAWOBJECT_H
#define RD_DRAWOBJECT_H

#include "rd_direct.h"

//global variables

//function headers
int rd_drawPointSet(const string & vertex_type, int nvertex, const float * vertex);
int rd_drawPolySet(const string & vertex_type, int nvertex, const float * vertex, int nface, const int * face);

int rd_drawCube(void);
int rd_drawSphere(float radius, float zmin, float zmax, float thetamax);
int rd_drawCone(float height, float radius, float thetamax);
int rd_drawDisk(float height, float radius, float thetamax);
int rd_drawCylinder(float radius, float zmin, float zmax, float thetamax);

#endif
