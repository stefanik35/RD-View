/***********************************************************************
Class:		CSCI 697 (Graphics Independent Study)
Program:	rd_dataStrctures.h
Author:		Drew Stefanik
Date Due:	09/30/2016

Purpose:	contains all additional data structures for rd_view

Notes:		part of rd_view.exe

***********************************************************************/

#ifndef RD_DATASTRUCTURES_H
#define RD_DATASTRUCTURES_H

struct point
{
	float x;
	float y;
	float z;
};

struct pointH
{
	float x;
	float y;
	float z;
	float w;
};

struct vector
{
	float x;
	float y;
	float z;
};

struct xform
{
	float matrix [4][4];
};

//attributed point
#define ATTR_CONSTANT  4
#define ATTR_R         5
#define ATTR_G         6
#define ATTR_B         7
#define ATTR_NX        8
#define ATTR_NY        9
#define ATTR_NZ       10
#define ATTR_S        11
#define ATTR_T        12
#define ATTR_WORLD_X  13
#define ATTR_WORLD_Y  14
#define ATTR_WORLD_Z  15

#define ATTR_SIZE     16

typedef struct {
  float coord[ATTR_SIZE];
} attrPoint;

struct edge
{
	int yLast;
	attrPoint p;
	attrPoint inc;
	
	edge * next;
};

//lights
struct lightAmbient
{
	float color [3];
};

struct lightFar
{
	float color [3];
	
	vector lightAt;
};

struct lightPoint
{
	float color [3];
	
	point lightPosition;
};

#endif
