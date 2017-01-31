/***********************************************************************
Class:		CSCI 697 (Graphics Independent Study)
Program:	rd_direct.cc
Author:		Drew Stefanik
Date Due:	09/30/2016

Purpose:	invokes rendering functions

Notes:		part of rd_view.exe

***********************************************************************/

#include "rd_direct.h"

#include <cmath>	//M_PI, sin, cos

//global variables
int frameNum;

float drawColor [3]  = {1.0, 1.0, 1.0};
float backgroundColor[3]  = {0.0, 0.0, 0.0};

int REDirect::rd_display(const string & name, const string & type, const string & mode)
{
	return RD_OK;
}

int REDirect::rd_format(int xresolution, int yresolution)
{
	return RD_OK;
}

int REDirect::rd_frame_begin(int frame_no)
{
	frameNum = frame_no;
	
	return RD_OK;
}

int REDirect::rd_world_begin(void)
{	
	rd_disp_init_frame(frameNum);
	
	if(depthBuffer == NULL)
	{
		depthBuffer = new float [display_xSize * display_ySize];
	}
	
	for(int i = 0; i < (display_xSize * display_ySize); i++)
	{
		depthBuffer[i] = 1;
	}
	
	if(edgeTable == NULL)
	{
		edgeTable = new edge [display_ySize];
	}
	
	currentXform = rd_identity();
	normalXform = rd_identity();
	
	xform worldToCamera = rd_worldToCamera();
	xform cameraToClip = rd_cameraToClip();
	
	worldToClip = rd_xformMult(cameraToClip, worldToCamera);
	clipToDevice = rd_clipToDevice();
	
	return RD_OK;
}

int REDirect::rd_frame_end(void)
{
	rd_disp_end_frame();
	
	return RD_OK;
}

int REDirect::rd_world_end(void)
{
	rd_disp_end_frame();
	
	return RD_OK;
}

int REDirect::rd_render_cleanup(void)
{	
	delete [] depthBuffer;
	depthBuffer = NULL;
	
	delete [] edgeTable;
	edgeTable = NULL;
	
	return RD_OK;
}

//set drawColor to the input color
int REDirect::rd_color(const float color[])
{
	drawColor[0] = color[0];
	drawColor[1] = color[1];
	drawColor[2] = color[2];

	colorSurface[0] = color[0];
	colorSurface[1] = color[1];
	colorSurface[2] = color[2];
		
	return RD_OK;
}

//set backgroundColor to the input color
int REDirect::rd_background(const float color[])
{
	backgroundColor[0] = color[0];
	backgroundColor[1] = color[1];
	backgroundColor[2] = color[2];
	
	rd_set_background(backgroundColor);
	
	return RD_OK;
}

//set the pixel at the input point to drawColor
int REDirect::rd_point(const float point[3])
{
	pointH drawPoint = rd_convertPoint(point);
	
	rd_pointPipeline(drawPoint);
	
	//rd_write_pixel((int) point[0], (int) point[1], drawColor);
	
	return RD_OK;
}

//call a function to draw a line from the input start point to the input end point
int REDirect::rd_line(const float start[3], const float end[3])
{
	pointH lineStart = rd_convertPoint(start);
	pointH lineEnd = rd_convertPoint(end);
	
	rd_linePipeline(lineStart, false);
	rd_linePipeline(lineEnd, true);
	
	//rd_drawLine(start, end);
	
	return RD_OK;
}

//call a function to draw a circle at the input center point with the input radius
int REDirect::rd_circle(const float center[3], float radius)
{
	rd_drawCircle(center, radius);
	
	return RD_OK;
}

//call a function to fill starting at the input seed_point
int REDirect::rd_fill(const float seed_point[3])
{
	rd_floodFill(seed_point);
	
	return RD_OK;
}

//call object drawing functions
int REDirect::rd_pointset(const string & vertex_type, int nvertex, const float * vertex)
{
	rd_drawPointSet(vertex_type, nvertex, vertex);
	
	return RD_OK;
}

int REDirect::rd_polyset(const string & vertex_type, int nvertex, const float * vertex, int nface, const int * face)
{
	rd_drawPolySet(vertex_type, nvertex, vertex, nface, face);
	
	return RD_OK;
}

int REDirect::rd_cube(void)
{
	rd_drawCube();
		
	return RD_OK;
}

int REDirect::rd_sphere(float radius, float zmin, float zmax, float thetamax)
{
	rd_drawSphere(radius, zmin, zmax, thetamax);
		
	return RD_OK;
}

int REDirect::rd_cone(float height, float radius, float thetamax)
{
	rd_drawCone(height, radius, thetamax);
	
	return RD_OK;
}

int REDirect::rd_disk(float height, float radius, float thetamax)
{
	rd_drawDisk(height, radius, thetamax);
	
	return RD_OK;
}

int REDirect::rd_cylinder(float radius, float zmin, float zmax, float thetamax)
{
	rd_drawCylinder(radius, zmin, zmax, thetamax);
	
	return RD_OK;
}
