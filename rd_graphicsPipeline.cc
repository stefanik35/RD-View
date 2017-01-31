/***********************************************************************
Class:		CSCI 697 (Graphics Independent Study)
Program:	rd_graphicsPipeline.cc
Author:		Drew Stefanik
Date Due:	??/??/2016

Purpose:	contains implementation of the graphics pipeline

Notes:		part of rd_view.exe
			invoked by rd_direct.cc

***********************************************************************/

#include "rd_graphicsPipeline.h"
#include <cmath>	//M_PI, tan, sqrt
#include <iostream>	//cout, endl
#include <iomanip>	//setw

using std::cout;
using std::endl;
using std::setw;

stack <xform> xformStack;
xform currentXform;
xform normalXform;
xform worldToClip;
xform clipToDevice;

float cameraFOV = 90.0 * M_PI / 180;
float nearClip = 1.0;
float farClip = 1000000000.0;
point cameraEye = {0.0, 0.0, 0.0};
point cameraAt = {0.0, 0.0, -1.0};
vector cameraUp = {0.0, 1.0, 0.0};

int REDirect::rd_translate(const float offset[3])
{
	xform translation = rd_identity();
	
	translation.matrix[0][3] = offset[0];
	translation.matrix[1][3] = offset[1];
	translation.matrix[2][3] = offset[2];
	
	currentXform = rd_xformMult(currentXform, translation);
	
	return RD_OK;
}

int REDirect::rd_scale(const float scale_factor[3])
{
	xform scale = rd_identity();
	
	scale.matrix[0][0] = scale_factor[0];
	scale.matrix[1][1] = scale_factor[1];
	scale.matrix[2][2] = scale_factor[2];
	
	currentXform = rd_xformMult(currentXform, scale);
	normalXform = rd_xformMult(scale, normalXform);
	
	return RD_OK;
}

int REDirect::rd_rotate_xy(float angle)
{
	xform rotation = rd_identity();
	float angleDeg = angle*M_PI/180;
		
	rotation.matrix[0][0] = cos(angleDeg);
	rotation.matrix[0][1] = -1 * sin(angleDeg);
	rotation.matrix[1][0] = sin(angleDeg);
	rotation.matrix[1][1] = cos(angleDeg);
	
	currentXform = rd_xformMult(currentXform, rotation);
	normalXform = rd_xformMult(rotation, normalXform);
	
	return RD_OK;
}

int REDirect::rd_rotate_yz(float angle)
{
	xform rotation = rd_identity();
	float angleDeg = angle*M_PI/180;
	
	rotation.matrix[1][1] = cos(angleDeg);
	rotation.matrix[1][2] = -1 * sin(angleDeg);
	rotation.matrix[2][1] = sin(angleDeg);
	rotation.matrix[2][2] = cos(angleDeg);
	
	currentXform = rd_xformMult(currentXform, rotation);
	normalXform = rd_xformMult(rotation, normalXform);
	
	return RD_OK;
}

int REDirect::rd_rotate_zx(float angle)
{
	xform rotation = rd_identity();
	float angleDeg = angle*M_PI/180;
	
	rotation.matrix[0][0] = cos(angleDeg);
	rotation.matrix[0][2] = sin(angleDeg);
	rotation.matrix[2][0] = -1 * sin(angleDeg);
	rotation.matrix[2][2] = cos(angleDeg);
	
	currentXform = rd_xformMult(currentXform, rotation);
	normalXform = rd_xformMult(rotation, normalXform);
	
	return RD_OK;
}

int REDirect::rd_matrix(const float * mat)
{	
	return RD_OK;
}

int REDirect::rd_camera_eye(const float eyepoint[3])
{
	cameraEye.x = eyepoint[0];
	cameraEye.y = eyepoint[1];
	cameraEye.z = eyepoint[2];
	
	return RD_OK;
}

int REDirect::rd_camera_at(const float atpoint[3])
{
	cameraAt.x = atpoint[0];
	cameraAt.y = atpoint[1];
	cameraAt.z = atpoint[2];
	
	return RD_OK;
}

int REDirect::rd_camera_up(const float up[3])
{
	cameraUp.x = up[0];
	cameraUp.y = up[1];
	cameraUp.z = up[2];
	
	return RD_OK;
}

int REDirect::rd_camera_fov(const float fov)
{
	cameraFOV = fov * M_PI / 180;
	
	return RD_OK;
}

int REDirect::rd_clipping(float znear, float zfar)
{
	nearClip = znear;
	farClip = zfar;
	
	return RD_OK;
}

int REDirect::rd_xform_push(void)
{
	xformStack.push(currentXform);
	xformStack.push(normalXform);
	
	return RD_OK;
}

int REDirect::rd_xform_pop(void)
{
	normalXform = xformStack.top();
	xformStack.pop();
	
	currentXform = xformStack.top();
	xformStack.pop();
	
	return RD_OK;
}

int rd_xformPoint(xform m, pointH & pH)
{
	float x, y, z, w;
	
	x = m.matrix[0][0] * pH.x + m.matrix[0][1] * pH.y + m.matrix[0][2] * pH.z + m.matrix[0][3] * pH.w;
	y = m.matrix[1][0] * pH.x + m.matrix[1][1] * pH.y + m.matrix[1][2] * pH.z + m.matrix[1][3] * pH.w;
	z = m.matrix[2][0] * pH.x + m.matrix[2][1] * pH.y + m.matrix[2][2] * pH.z + m.matrix[2][3] * pH.w;
	w = m.matrix[3][0] * pH.x + m.matrix[3][1] * pH.y + m.matrix[3][2] * pH.z + m.matrix[3][3] * pH.w;
	
	pH.x = x;
	pH.y = y;
	pH.z = z;
	pH.w = w;
	
	return RD_OK;
}

xform rd_xformMult(xform m1, xform m2)
{
	xform result = {};
		
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			result.matrix[i][j] = m1.matrix[i][0] * m2.matrix[0][j] + m1.matrix[i][1] * m2.matrix[1][j] + m1.matrix[i][2] * m2.matrix[2][j] + m1.matrix[i][3] * m2.matrix[3][j];
		}
	}
	
	return result;
}

xform rd_identity()
{
	xform identity = {};
	
	identity.matrix[0][0] = 1;
	identity.matrix[1][1] = 1;
	identity.matrix[2][2] = 1;
	identity.matrix[3][3] = 1;
	
	return identity;
}

xform rd_worldToCamera()
{
	xform worldToCameraRotation = rd_identity();
	xform worldToCameraTranslation = rd_identity();
	xform worldToCamera = rd_identity();
	
	vector A, u, v;
	
	A = rd_subtractPoint(cameraAt, cameraEye);
	A = rd_normalize(A);
	
	u = rd_crossProduct(A, cameraUp);
	u = rd_normalize(u);
	
	v = rd_crossProduct(u, A);
	
	worldToCameraRotation.matrix[0][0] = u.x;
	worldToCameraRotation.matrix[0][1] = u.y;
	worldToCameraRotation.matrix[0][2] = u.z;
	
	worldToCameraRotation.matrix[1][0] = v.x;
	worldToCameraRotation.matrix[1][1] = v.y;
	worldToCameraRotation.matrix[1][2] = v.z;
	
	worldToCameraRotation.matrix[2][0] = A.x;
	worldToCameraRotation.matrix[2][1] = A.y;
	worldToCameraRotation.matrix[2][2] = A.z;
	
	worldToCameraTranslation.matrix[0][3] = -cameraEye.x;
	worldToCameraTranslation.matrix[1][3] = -cameraEye.y;
	worldToCameraTranslation.matrix[2][3] = -cameraEye.z;
	
	worldToCamera = rd_xformMult(worldToCameraRotation, worldToCameraTranslation);
	
	return worldToCamera;
}

xform rd_cameraToClip()
{
	xform cameraToClip = rd_identity();
	xform scale = rd_identity();
	xform translation = rd_identity();
	
	float aspectRatio = (float) display_xSize / display_ySize;
	
	cameraToClip.matrix[0][0] = 1 / (aspectRatio * tan(cameraFOV / 2));
	cameraToClip.matrix[1][1] = 1 / (tan(cameraFOV / 2));
	cameraToClip.matrix[2][2] = farClip / (farClip - nearClip);
	cameraToClip.matrix[2][3] = (-1 * farClip *nearClip) / (farClip - nearClip);
	cameraToClip.matrix[3][2] = 1;
	cameraToClip.matrix[3][3] = 0;
	
	scale.matrix[0][0] = 0.5;
	scale.matrix[1][1] = 0.5;
	translation.matrix[0][3] = 0.5;
	translation.matrix[1][3] = 0.5;
	
	cameraToClip = rd_xformMult(scale, cameraToClip);
	cameraToClip = rd_xformMult(translation, cameraToClip);
	
	return cameraToClip;
}

xform rd_clipToDevice()
{
	xform clipToDevice = rd_identity();
	float epsilon = 0.001;
	
	clipToDevice.matrix[0][0] = display_xSize - epsilon;
	clipToDevice.matrix[1][1] = -1 * (display_ySize - epsilon);
	clipToDevice.matrix[1][3] = display_ySize - epsilon;
	
	//scan conversion adjustments
	clipToDevice.matrix[0][3] = -0.5;
	clipToDevice.matrix[1][3] += -0.5;
	
	return clipToDevice;
}

pointH rd_convertPoint(const float point[3])
{
	pointH result;
	
	result.x = point[0];
	result.y = point[1];
	result.z = point[2];
	result.w = 1;
	
	return result;
}

float rd_magnitudeSquared(vector v)
{
	float result;
	
	result = v.x * v.x + v.y * v.y + v.z * v.z;
	
	return result;
}

vector rd_normalize(vector v)
{
	float magnitude;
	vector result;
	
	magnitude = rd_magnitudeSquared(v);
	
	magnitude = sqrt(magnitude);
	
	result.x = v.x / magnitude;
	result.y = v.y / magnitude;
	result.z = v.z / magnitude;
	
	return result;
}

vector rd_subtractPoint(point p1, point p2)
{
	vector result;
	
	result.x = p1.x - p2.x;
	result.y = p1.y - p2.y;
	result.z = p1.z - p2.z;
	
	return result;
}

float rd_dotProduct(vector v1, vector v2)
{
	float result;
	
	result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	
	return result;
}

vector rd_crossProduct(vector v1, vector v2)
{
	vector result;
	
	result.x = v1.y * v2.z - v2.y * v1.z;
	result.y = -(v1.x * v2.z) + v2.x * v1.z;
	result.z = v1.x * v2.y - v2.x * v1.y;
	
	return result;
}

void rd_printPointH(pointH pH)
{
	cout << endl << "X: " << pH.x;
	cout << endl << "Y: " << pH.y;
	cout << endl << "Z: " << pH.z;
	cout << endl << "W: " << pH.w << endl;
}

void rd_printXform(xform m)
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			cout << setw(10) << m.matrix[i][j] << " ";
		}
		
		cout << endl;
	}
	
	cout << endl;
}

void rd_printPoint(point p)
{
	cout << p.x << " " << p.y << " " << p.z << endl;
}

void rd_printVector(vector v)
{
	cout << v.x << " " << v.y << " " << v.z << endl;
}
