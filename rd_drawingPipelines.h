/***********************************************************************
Class:		CSCI 697 (Graphics Independent Study)
Program:	rd_drawingPipelines.h
Author:		Drew Stefanik
Date Due:	??/??/2016

Purpose:	header file for rd_drawingPipelines.cc

Notes:		part of rd_view.exe

***********************************************************************/

#ifndef RD_DRAWINGPIPELINES_H
#define RD_DRAWINGPIPELINES_H

#include "rd_direct.h"

//global variables
extern pointH movePoint;

extern edge * edgeTable;

//function headers
int rd_pointPipeline(pointH pH);
bool rd_checkBounds(pointH pH);

int rd_linePipeline(pointH pH, bool draw);
int rd_lineClipping(pointH drawPoint);

int rd_polygonPipeline(attrPoint p, bool draw);

int rd_scanConversion(attrPoint clipped_list[], int n_vertex);
bool rd_buildEdgeList(attrPoint clipped_list[], int n_vertex);
int rd_makeEdgeRec(attrPoint upper, attrPoint lower);
int rd_addActiveList(int scan, edge * AET);
int rd_insertEdge(edge * list, edge * e);
int rd_updateAET(int scanline, edge * active);
int rd_deleteAfter(edge * q);
int rd_resortAET(edge * active);
int rd_fillBtwnEdges(int scan, edge * active);

int rd_polygonClipping(int n_vertex, attrPoint vertex_list[], attrPoint clipped_list[]);
int rd_clipPoint(attrPoint p, int b, int & count, attrPoint clipped_list[], attrPoint firstSeen[], attrPoint lastSeen[], bool flagList[]);
int rd_clipLastPoint(int & count, attrPoint clipped_list[], attrPoint firstSeen[], attrPoint lastSeen[], bool flagList[]);
attrPoint rd_intersectionPoint(attrPoint p0, attrPoint p1, int b);
bool rd_crossBoundary(attrPoint p0, attrPoint p1, int b);
bool rd_insideBoundary(attrPoint p, int b);

void rd_printPointA(attrPoint a);

#endif
