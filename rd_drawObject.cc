/***********************************************************************
Class:		CSCI 697 (Graphics Independent Study)
Program:	rd_drawLine.cc
Author:		Drew Stefanik
Date Due:	09/30/2016

Purpose:	contains functions and variables to implement line drawing

Notes:		part of rd_view.exe
			invoked by rd_direct.cc

***********************************************************************/

#include "rd_drawObject.h"
#include <cmath>	//M_PI, sin, cos
#include <iostream>	//cout, endl

using std::cout;
using std::endl;

//global variables
int NSTEPS = 20;		//should be divisible by four
int NSTEPS_SPHERE = 10;

int rd_drawPointSet(const string & vertex_type, int nvertex, const float * vertex)
{
	pointH draw;
	
	for(int i = 0; i < nvertex * 3; i += 3)
	{
		draw.x = vertex[i];
		draw.y = vertex[i + 1];
		draw.z = vertex[i + 2];
		draw.w = 1;
		
		rd_pointPipeline(draw);
	}
	
	return RD_OK;
}

//TEMP
int rd_drawPolySet(const string & vertex_type, int nvertex, const float * vertex, int nface, const int * face)
{
	attrPoint vertices[nvertex];
	attrPoint draw;
	attrPoint start;
	
	int i;
	int j = 0;
	
	for(i = 0; i < nvertex * 3; i += 3)
	{
		vertices[j].coord[0] = vertex[i];
		vertices[j].coord[1] = vertex[i + 1];
		vertices[j].coord[2] = vertex[i + 2];
		vertices[j].coord[3] = 1;
		
		j++;
	}
	
	i = 0;
	j = 0;
	int faceCount = 0;
	
	while(faceCount < nface)
	{	
		if(face[i] == -1)
		{	
			rd_polygonPipeline(start, true);
			
			j = 0;
			
			faceCount++;
		}
		else if(j == 0)
		{
			draw = vertices[face[i]];
			
			start = draw;
			
			rd_polygonPipeline(draw, false);
			
			j++;
		}
		else
		{
			draw = vertices[face[i]];
						
			rd_polygonPipeline(draw, false);
			
			j++;
		}
		
		i++;
	}
	/*pointH vertices[nvertex];
	pointH draw;
	pointH start;
	
	int i;
	int j = 0;
	
	for(i = 0; i < nvertex * 3; i += 3)
	{
		vertices[j].x = vertex[i];
		vertices[j].y = vertex[i + 1];
		vertices[j].z = vertex[i + 2];
		vertices[j].w = 1;
		
		j++;
	}
	
	i = 0;
	j = 0;
	int faceCount = 0;
	
	while(faceCount < nface)
	{	
		if(face[i] == -1)
		{	
			rd_linePipeline(start, true);
			
			j = 0;
			
			faceCount++;
		}
		else if(j == 0)
		{
			draw = vertices[face[i]];
			
			start = draw;
			
			rd_linePipeline(draw, false);
			
			j++;
		}
		else
		{
			draw = vertices[face[i]];
						
			rd_linePipeline(draw, true);
			
			j++;
		}
		
		i++;
	}*/
	
	return RD_OK;
}

int rd_drawCube(void)
{
	vertexColorFlag = false;
	vertexNormalFlag = true;
	vertexTextureFlag = false;
	
	attrPoint vertex0 = {-1, -1, 1, 1};
	attrPoint vertex1 = {1, -1, 1, 1};
	attrPoint vertex2 = {1, -1, -1, 1};
	attrPoint vertex3 = {-1, -1, -1, 1};
	attrPoint vertex4 = {-1, 1, 1, 1};
	attrPoint vertex5 = {1, 1, 1, 1};
	attrPoint vertex6 = {1, 1, -1, 1};
	attrPoint vertex7 = {-1, 1, -1, 1};
	
	//front face
	vertex0.coord[ATTR_NX] = 0.0;
	vertex0.coord[ATTR_NY] = -1.0;
	vertex0.coord[ATTR_NZ] = 0.0;
	rd_polygonPipeline(vertex0, false);
	
	vertex1.coord[ATTR_NX] = 0.0;
	vertex1.coord[ATTR_NY] = -1.0;
	vertex1.coord[ATTR_NZ] = 0.0;
	rd_polygonPipeline(vertex1, false);
	
	vertex5.coord[ATTR_NX] = 0.0;
	vertex5.coord[ATTR_NY] = -1.0;
	vertex5.coord[ATTR_NZ] = 0.0;
	rd_polygonPipeline(vertex5, false);
	
	vertex4.coord[ATTR_NX] = 0.0;
	vertex4.coord[ATTR_NY] = -1.0;
	vertex4.coord[ATTR_NZ] = 0.0;
	
	normalVector.x = 0.0;
	normalVector.y = -1.0;
	normalVector.z = 0.0;
	
	rd_polygonPipeline(vertex4, true);
	
	//back face
	vertex2.coord[ATTR_NX] = 0.0;
	vertex2.coord[ATTR_NY] = 1.0;
	vertex2.coord[ATTR_NZ] = 0.0;
	rd_polygonPipeline(vertex2, false);
	
	vertex3.coord[ATTR_NX] = 0.0;
	vertex3.coord[ATTR_NY] = 1.0;
	vertex3.coord[ATTR_NZ] = 0.0;
	rd_polygonPipeline(vertex3, false);
	
	vertex7.coord[ATTR_NX] = 0.0;
	vertex7.coord[ATTR_NY] = 1.0;
	vertex7.coord[ATTR_NZ] = 0.0;
	rd_polygonPipeline(vertex7, false);
	
	vertex6.coord[ATTR_NX] = 0.0;
	vertex6.coord[ATTR_NY] = 1.0;
	vertex6.coord[ATTR_NZ] = 0.0;
	
	normalVector.x = 0.0;
	normalVector.y = 1.0;
	normalVector.z = 0.0;
	
	rd_polygonPipeline(vertex6, true);
	
	//right face
	vertex1.coord[ATTR_NX] = 1.0;
	vertex1.coord[ATTR_NY] = 0.0;
	vertex1.coord[ATTR_NZ] = 0.0;
	rd_polygonPipeline(vertex1, false);
	
	vertex2.coord[ATTR_NX] = 1.0;
	vertex2.coord[ATTR_NY] = 0.0;
	vertex2.coord[ATTR_NZ] = 0.0;
	rd_polygonPipeline(vertex2, false);
	
	vertex6.coord[ATTR_NX] = 1.0;
	vertex6.coord[ATTR_NY] = 0.0;
	vertex6.coord[ATTR_NZ] = 0.0;
	rd_polygonPipeline(vertex6, false);
	
	vertex5.coord[ATTR_NX] = 1.0;
	vertex5.coord[ATTR_NY] = 0.0;
	vertex5.coord[ATTR_NZ] = 0.0;
	
	normalVector.x = 1.0;
	normalVector.y = 0.0;
	normalVector.z = 0.0;
	
	rd_polygonPipeline(vertex5, true);
	
	//left face
	vertex0.coord[ATTR_NX] = -1.0;
	vertex0.coord[ATTR_NY] = 0.0;
	vertex0.coord[ATTR_NZ] = 0.0;
	rd_polygonPipeline(vertex0, false);
	
	vertex4.coord[ATTR_NX] = -1.0;
	vertex4.coord[ATTR_NY] = 0.0;
	vertex4.coord[ATTR_NZ] = 0.0;
	rd_polygonPipeline(vertex4, false);
	
	vertex7.coord[ATTR_NX] = -1.0;
	vertex7.coord[ATTR_NY] = 0.0;
	vertex7.coord[ATTR_NZ] = 0.0;
	rd_polygonPipeline(vertex7, false);
	
	vertex3.coord[ATTR_NX] = -1.0;
	vertex3.coord[ATTR_NY] = 0.0;
	vertex3.coord[ATTR_NZ] = 0.0;
	
	normalVector.x = -1.0;
	normalVector.y = 0.0;
	normalVector.z = 0.0;
	
	rd_polygonPipeline(vertex3, true);
	
	//top face
	vertex4.coord[ATTR_NX] = 0.0;
	vertex4.coord[ATTR_NY] = 0.0;
	vertex4.coord[ATTR_NZ] = 1.0;
	rd_polygonPipeline(vertex4, false);
	
	vertex5.coord[ATTR_NX] = 0.0;
	vertex5.coord[ATTR_NY] = 0.0;
	vertex5.coord[ATTR_NZ] = 1.0;
	rd_polygonPipeline(vertex5, false);
	
	vertex6.coord[ATTR_NX] = 0.0;
	vertex6.coord[ATTR_NY] = 0.0;
	vertex6.coord[ATTR_NZ] = 1.0;
	rd_polygonPipeline(vertex6, false);
	
	vertex7.coord[ATTR_NX] = 0.0;
	vertex7.coord[ATTR_NY] = 0.0;
	vertex7.coord[ATTR_NZ] = 1.0;
	
	normalVector.x = 0.0;
	normalVector.y = 0.0;
	normalVector.z = 1.0;
	
	rd_polygonPipeline(vertex7, true);
	
	//bottom face 
	vertex3.coord[ATTR_NX] = 0.0;
	vertex3.coord[ATTR_NY] = 0.0;
	vertex3.coord[ATTR_NZ] = -1.0;
	rd_polygonPipeline(vertex3, false);
	
	vertex2.coord[ATTR_NX] = 0.0;
	vertex2.coord[ATTR_NY] = 0.0;
	vertex2.coord[ATTR_NZ] = -1.0;
	rd_polygonPipeline(vertex2, false);
	
	vertex1.coord[ATTR_NX] = 0.0;
	vertex1.coord[ATTR_NY] = 0.0;
	vertex1.coord[ATTR_NZ] = -1.0;
	rd_polygonPipeline(vertex1, false);
	
	vertex0.coord[ATTR_NX] = 0.0;
	vertex0.coord[ATTR_NY] = 0.0;
	vertex0.coord[ATTR_NZ] = -1.0;
	
	normalVector.x = 0.0;
	normalVector.y = 0.0;
	normalVector.z = -1.0;
	
	rd_polygonPipeline(vertex0, true);
	
	return RD_OK;
}

int rd_drawSphere(float radius, float zmin, float zmax, float thetamax)
{
	//steps used in creating the sphere
	const int longSteps = 40;
	const int latSteps = 20;

	double theta1, phi1, phi2, theta2;

	attrPoint circle = {0, 0, 0, 1};

	//step from south pole to north pole
	for(int i = 0; i < latSteps; i++)
	{
		//calculate phi1
		phi1 = M_PI * ((float) i / (float) latSteps) - (M_PI/2);
		//calculate phi2
		phi2 = M_PI * ((float) (i+1) / (float) latSteps) - (M_PI/2);

		//start from left to right
		for(int j = 0; j < longSteps; j++)
		{
			//calculate theta1
			theta1 = ((float) j * ((2 * M_PI) / (float) longSteps));

			//calculate theta2
			theta2 = ((float) (j + 1) * ((2 * M_PI) / (float) longSteps));

			//draw to the first point
			circle.coord[0] =(radius * cos(theta1) * cos(phi2));
			circle.coord[1] =(radius * sin(theta1) * cos(phi2));
			circle.coord[2] =(radius * sin(phi2));

			rd_polygonPipeline(circle, false);

			//draw to the previous point in phi
			circle.coord[0] = (radius * cos(theta1) * cos(phi1));
			circle.coord[1] = (radius * sin(theta1) * cos(phi1));
			circle.coord[2] = (radius * sin(phi1));
			
			rd_polygonPipeline(circle, false);

			//draw to the next point in theta
			circle.coord[0] = (radius * cos(theta2) * cos(phi1));
			circle.coord[1] = (radius * sin(theta2) * cos(phi1));
			circle.coord[2] = (radius * sin(phi1));

			rd_polygonPipeline(circle, false);
			
			//draw to the next point in phi
			circle.coord[0] = (radius * cos(theta2) * cos(phi2));
			circle.coord[1] = (radius * sin(theta2) * cos(phi2));
			circle.coord[2] = (radius * sin(phi2));
			
			rd_polygonPipeline(circle, true);
		}
	}
	/*
	attrPoint circle = {0, 0, 0, 1};
	attrPoint next;
	
	float phi = -1 * M_PI / 2, theta = 0;
	
	for(int i = 0; i <= NSTEPS_SPHERE; )
	{		
		circle.coord[0] = radius * cos(theta) * cos(phi);
		circle.coord[1] = radius * sin(theta) * cos(phi);
		circle.coord[2] = radius * sin(phi);
		
		rd_polygonPipeline(circle, false);
		
		for(int j = 1; j <= (2 * NSTEPS_SPHERE); j++)
		{
			theta = ((float) j / (2 * NSTEPS_SPHERE)) * (2 * M_PI);
			
			circle.coord[0] = radius * cos(theta) * cos(phi);
			circle.coord[1] = radius * sin(theta) * cos(phi);
			circle.coord[2] = radius * sin(phi);
			
			rd_polygonPipeline(circle, false);
			
			next = circle;
			
			phi = ((float) (i + 1) / NSTEPS_SPHERE) * (M_PI) - (M_PI / 2);
			
			circle.coord[0] = radius * cos(theta) * cos(phi);
			circle.coord[1] = radius * sin(theta) * cos(phi);
			circle.coord[2] = radius * sin(phi);
			
			rd_polygonPipeline(circle, false);
			
			theta = ((float) (j - 1) / (2 * NSTEPS_SPHERE)) * (2 * M_PI);
			
			circle.coord[0] = radius * cos(theta) * cos(phi);
			circle.coord[1] = radius * sin(theta) * cos(phi);
			circle.coord[2] = radius * sin(phi);
			
			rd_polygonPipeline(circle, true);
			
			phi = ((float) (i) / NSTEPS_SPHERE * (M_PI)) - (M_PI / 2);
			
			circle = next;
			
			if(j < (2 * NSTEPS_SPHERE))
			{
				rd_polygonPipeline(circle, false);
			}
		}
		
		i++;
		phi = ((float) i / NSTEPS_SPHERE * (M_PI)) - (M_PI / 2);
		theta = 0;
	}
	*/
	return RD_OK;
}

int rd_drawCone(float height, float radius, float thetamax)
{
	attrPoint circle = {radius, 0, 0, 1};
	attrPoint start = circle;
	attrPoint next;
	
	float theta;
	
	rd_polygonPipeline(circle, false);
	
	for(int i = 1; i <= NSTEPS; i++)
	{
		theta = ((float) i / NSTEPS) * (2 * M_PI);
		
		circle.coord[0] = radius * cos(theta);
		circle.coord[1] = radius * sin(theta);
		
		rd_polygonPipeline(circle, false);
		
		next = circle;
		
		circle.coord[0] = 0;
		circle.coord[1] = 0;
		circle.coord[2] = height;
		
		rd_polygonPipeline(circle, false);
		rd_polygonPipeline(circle, true);
		
		circle = start;
		
		start = next;
		
		if(i < NSTEPS)
		{
			rd_polygonPipeline(start, false);
		}
	}
	
	return RD_OK;
}

int rd_drawDisk(float height, float radius, float thetamax)
{
	attrPoint circle = {radius, 0, height, 1};
	attrPoint next;
	
	float theta;
	
	rd_polygonPipeline(circle, false);
	
	for(int i = 1; i <= NSTEPS; i++)
	{
		theta = ((float) i / NSTEPS) * (2 * M_PI);
		
		circle.coord[0] = radius * cos(theta);
		circle.coord[1] = radius * sin(theta);
		
		rd_polygonPipeline(circle, false);
		
		next = circle;
		
		circle.coord[0] = 0;
		circle.coord[1] = 0;
		
		rd_polygonPipeline(circle, true);
		
		circle = next;
		
		if(i < NSTEPS)
		{
			rd_polygonPipeline(circle, false);
		}
	}
	
	return RD_OK;
}

int rd_drawCylinder(float radius, float zmin, float zmax, float thetamax)
{
	attrPoint circle = {radius, 0, zmin, 1};
	attrPoint start = circle;
	attrPoint next;
	
	float theta;
	
	rd_polygonPipeline(circle, false);
	
	for(int i = 1; i <= NSTEPS; i++)
	{
		theta = ((float) i / NSTEPS) * (2 * M_PI);
		
		circle.coord[0] = radius * cos(theta);
		circle.coord[1] = radius * sin(theta);
		
		rd_polygonPipeline(circle, false);
		
		next = circle;
		
		circle.coord[2] = zmax;
		
		rd_polygonPipeline(circle, false);
		
		circle = start;
		circle.coord[2] = zmax;
		
		rd_polygonPipeline(circle, true);
		
		circle = start;
		
		start = next;
		
		if(i < NSTEPS)
		{
			rd_polygonPipeline(start, false);
		}
	}
	
	return RD_OK;
}
