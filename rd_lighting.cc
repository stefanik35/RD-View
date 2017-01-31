/***********************************************************************
Class:		CSCI 697 (Graphics Independent Study)
Program:	rd_lighting.cc
Author:		Drew Stefanik
Date Due:	12/09/2016

Purpose:	contains functions and variables to implement lighting and
 			shading

Notes:		part of rd_view.exe

***********************************************************************/

#include "rd_lighting.h"
#include <iostream>

using std::cout;
using std::endl;

lightAmbient globalAmbient = { 1.0, 1.0, 1.0 };
lightFar globalFar [30];
int counterFar = 0;
lightPoint globalPoint [30];
int counterPoint = 0;

float coefAmbient = 1;
float coefDiffuse = 0;
float coefSpecular = 0;

float colorSurface [3] = { 1.0, 1.0, 1.0 };
float colorSpecular [3] = { 1.0, 1.0, 1.0 };
float exponentSpecular = 10.0;

bool vertexColorFlag;
bool vertexNormalFlag;
bool vertexTextureFlag;
bool interpolationFlag = true;

vector viewVector;
vector normalVector;

attrPoint surfacePointValues;

//default shader???

int REDirect::rd_surface(const string & shader_type)
{
	return RD_OK;
}

int REDirect::rd_point_light(const float pos[3], const float color[], float intensity)
{
	lightPoint newPointLight;
	
	point position = { pos[0], pos[1], pos[2] };
	
	newPointLight.lightPosition = position;
	
	newPointLight.color[0] = intensity * color[0];
	newPointLight.color[1] = intensity * color[1];
	newPointLight.color[2] = intensity * color[2];
	
	globalPoint[counterPoint] = newPointLight;
	counterPoint++;
	
	return RD_OK;
}

int REDirect::rd_far_light  (const float dir[3], const float color[], float intensity)
{
	lightFar newFarLight;
	
	vector direction = { dir[0], dir[1], dir[2] };
	
	newFarLight.lightAt = direction;
	
	newFarLight.color[0] = intensity * color[0];
	newFarLight.color[1] = intensity * color[1];
	newFarLight.color[2] = intensity * color[2];
	
	globalFar[counterFar] = newFarLight;
	counterFar++;
	
	return RD_OK;
}

int REDirect::rd_ambient_light(const float color[], float intensity)
{
	globalAmbient.color[0] = intensity * color[0];
	globalAmbient.color[1] = intensity * color[1];
	globalAmbient.color[2] = intensity * color[2];
	
	return RD_OK;
}

int REDirect::rd_specular_color(const float color[], int exponent)
{
	colorSpecular[0] = color[0];
	colorSpecular[1] = color[1];
	colorSpecular[2] = color[2];
	
	exponentSpecular = exponent;
	
	return RD_OK;
}

int REDirect::rd_k_ambient(float Ka)
{
	coefAmbient = Ka;
	
	return RD_OK;
}

int REDirect::rd_k_diffuse(float Kd)
{
	coefDiffuse = Kd;
	
	return RD_OK;
}

int REDirect::rd_k_specular(float Ks)
{
	coefSpecular = Ks;
	
	return RD_OK;
}

int rd_matteShader(float (&colorPixel) [3])
{
	float ambientComponent;
	
	for(int i = 0; i < 3; i++)
	{
		ambientComponent = coefAmbient * colorSurface[i] * globalAmbient.color[i];
		colorPixel[i] =  ambientComponent + rd_diffuseLight(i);
	}
	
	return RD_OK;
}

float rd_diffuseLight(int colorComponent)
{
	float diffuseComponent = 0;
	float dotProduct;
	
	vector L;
	vector N;
	
	for(int i = 0; i < counterPoint; i++)
	{
		L.x = globalPoint[i].lightPosition.x - surfacePointValues.coord[ATTR_WORLD_X];
		L.y = globalPoint[i].lightPosition.y - surfacePointValues.coord[ATTR_WORLD_Y];
		L.z = globalPoint[i].lightPosition.z - surfacePointValues.coord[ATTR_WORLD_Z];
		
		N = normalVector;
		
		L = rd_normalize(L);
		N = rd_normalize(N);
		
		dotProduct = rd_dotProduct(L, N);
		
		if(dotProduct < 0)
		{
			continue;
		}
		
		diffuseComponent += globalPoint[i].color[colorComponent] * dotProduct;
	}
	/*
	for(int i = 0; i < counterFar; i++)
	{
		L.x = -1 * globalFar[i].lightAt.x - surfacePointValues.coord[ATTR_WORLD_X];
		L.y = -1 * globalFar[i].lightAt.y - surfacePointValues.coord[ATTR_WORLD_Y];
		L.z = -1 * globalFar[i].lightAt.z - surfacePointValues.coord[ATTR_WORLD_Z];
		
		N = normalVector;
		
		L = rd_normalize(L);
		N = rd_normalize(N);
		
		dotProduct = rd_dotProduct(L, N);
		
		if(dotProduct < 0)
		{
			continue;
		}
		
		diffuseComponent += globalFar[i].color[colorComponent] * dotProduct;
	}
	*/
	diffuseComponent *= coefDiffuse;
	diffuseComponent *= colorSurface[colorComponent];
	
	return diffuseComponent;
}
