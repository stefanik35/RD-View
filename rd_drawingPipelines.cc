/***********************************************************************
Class:		CSCI 697 (Graphics Independent Study)
Program:	rd_drawingPipelines.cc
Author:		Drew Stefanik
Date Due:	??/??/2016

Purpose:	contains the implementation of the point, line, and polygon
 				pipelines

Notes:		part of rd_view.exe
			invoked by rd_direct.cc

***********************************************************************/

#include "rd_drawingPipelines.h"
#include <cmath>	//min, max
#include <iostream>	//cout, endl

using std::min;
using std::max;
using std::cout;
using std::endl;

//global variables
pointH movePoint;

edge * edgeTable = NULL;

int rd_pointPipeline(pointH pH)
{
	pointH drawPoint = pH;
	
	rd_xformPoint(currentXform, drawPoint);
	rd_xformPoint(worldToClip, drawPoint);
	
	if(rd_checkBounds(drawPoint) == true)
	{
		rd_xformPoint(clipToDevice, drawPoint);
		
		point p = {drawPoint.x / drawPoint.w, drawPoint.y / drawPoint.w, drawPoint.z / drawPoint.w};
		
		rd_plotPixel(p, drawColor);
	}
	
	return RD_OK;
}

bool rd_checkBounds(pointH pH)
{
	bool inBounds = false;
	
	if((pH.x / pH.w >= 0 && pH.x / pH.w <= 1) && (pH.y / pH.w >= 0 && pH.y / pH.w <= 1) && (pH.z / pH.w >= 0 && pH.z / pH.w <= 1))
	{
		inBounds = true;
	}
	
	return inBounds;
}

int rd_linePipeline(pointH pH, bool draw)
{
	pointH drawPoint = pH;
	
	rd_xformPoint(currentXform, drawPoint);
	rd_xformPoint(worldToClip, drawPoint);
	
	if(draw == false)
	{
		movePoint = drawPoint;
	}
	else
	{
		rd_lineClipping(drawPoint);
	}
	
	return RD_OK;
}

int rd_lineClipping(pointH drawPoint)
{
	//create new variables to represent the line start and end points
	pointH p0 = movePoint;
	pointH p1 = drawPoint;
	
	//declare clipping algorithm variables
	int code0 = 0, code1 = 0, code;
	float alphaMin = 0, alphaMax = 1, alpha;
	
	//compute boundary coordinates
	float BC0[6] = {p0.x, p0.w - p0.x, p0.y, p0.w - p0.y, p0.z, p0.w - p0.z};
	float BC1[6] = {p1.x, p1.w - p1.x, p1.y, p1.w - p1.y, p1.z, p1.w - p1.z};
	
	//compute boundary codes
	for(int i = 0; i < 6; i++)
	{
		if(BC0[i] < 0)
		{
			code0 |= (1<<i);
		}
		
		if(BC1[i] < 0)
		{
			code1 |= (1<<i);
		}
	}
	
	//trivial reject
	if(code0 & code1)
	{
		//clip
		movePoint = p1;
		
		return RD_OK;
	}
	
	code = code0 | code1;
	
	//trivial accept
	if(code == 0)
	{
		//draw		
		pointH clipDrawPoint = p1;
			
		rd_xformPoint(clipToDevice, p0);
		rd_xformPoint(clipToDevice, p1);
		
		float start[3] = {p0.x / p0.w, p0.y / p0.w, p0.z / p0.w};
		float end[3] = {p1.x / p1.w, p1.y / p1.w, p1.z / p1.w};
		
		rd_drawLine(start, end);
		
		movePoint = clipDrawPoint;
		
		return RD_OK;
	}
	
	//for each boundary plane
	for(int i = 0; i < 6; i++)
	{
		//if the bit at position i is turned on
		//line crosses boundary plane i
		if((code & (1<<i)))
		{
			alpha = BC0[i] / (BC0[i] - BC1[i]);
			
			//line goes from outside to inside
			if(code0 & (1<<i))
			{
				alphaMin = max(alpha, alphaMin);
			}
			//line goes from inside to outside
			else
			{
				alphaMax = min(alpha, alphaMax);
			}
		}
	}
	
	//draw
	if(alphaMin < alphaMax)
	{
		//create new variables for the clipped points
		pointH p0c, p1c;
		
		//determine the clipped points
		p0c.x = p0.x + alphaMin * (p1.x - p0.x);
		p0c.y = p0.y + alphaMin * (p1.y - p0.y);
		p0c.z = p0.z + alphaMin * (p1.z - p0.z);
		p0c.w = p0.w + alphaMin * (p1.w - p0.w);
		
		p1c.x = p0.x + alphaMax * (p1.x - p0.x);
		p1c.y = p0.y + alphaMax * (p1.y - p0.y);
		p1c.z = p0.z + alphaMax * (p1.z - p0.z);
		p1c.w = p0.w + alphaMax * (p1.w - p0.w);
		
		//transform the clipped points to device coordinates
		rd_xformPoint(clipToDevice, p0c);
		rd_xformPoint(clipToDevice, p1c);
		
		//convert the homogeneous end points to cartesian and save them as an array of floats
		float start[3] = {p0c.x / p0c.w, p0c.y / p0c.w, p0c.z / p0c.w};
		float end[3] = {p1c.x / p1c.w, p1c.y / p1c.w, p1c.z / p1c.w};
		
		//pass the end points to the line drawing function
		rd_drawLine(start, end);
		
		//set the current drawPoint to be the next endPoint
		movePoint = p1;
		
		return RD_OK;
	}
	else
	{
		//clip
		movePoint = p1;
		
		return RD_OK;
	}
}

int rd_polygonPipeline(attrPoint p, bool draw)
{
	pointH geom, dev;
	pointH norm;

	const int MAX_VERTEX_LIST_SIZE = 50;
	static attrPoint vertex_list[MAX_VERTEX_LIST_SIZE];
	static attrPoint clipped_list[MAX_VERTEX_LIST_SIZE];
	static int n_vertex = 0;

	int i;
	
	//run geometry through current transform
	geom.x = p.coord[0];
	geom.y = p.coord[1];
	geom.z = p.coord[2];
	geom.w = p.coord[3];
	
	rd_xformPoint(currentXform, geom);

	//save world coordinates
	p.coord[ATTR_WORLD_X] = geom.x;
	p.coord[ATTR_WORLD_Y] = geom.y;
	p.coord[ATTR_WORLD_Z] = geom.z;
	
	//run normal through normal transform
	norm.x = p.coord[ATTR_NX];
	norm.y = p.coord[ATTR_NY];
	norm.z = p.coord[ATTR_NZ];
	norm.w = 1.0;
	
	rd_xformPoint(normalXform, norm);
	
	//save world normal
	p.coord[ATTR_NX] = norm.x;
	p.coord[ATTR_NY] = norm.y;
	p.coord[ATTR_NZ] = norm.z;
	
	//set attrPoint constant
	p.coord[ATTR_CONSTANT] = 1.0;

	//run through world to clip
	rd_xformPoint(worldToClip, geom);
	
	p.coord[0] = geom.x;
	p.coord[1] = geom.y;
	p.coord[2] = geom.z;
	p.coord[3] = geom.w;
	
	//store in vertex list
	if(n_vertex == MAX_VERTEX_LIST_SIZE)
	{
		return -1;	//overflow
	}

	vertex_list[n_vertex] = p;
	n_vertex++;

	if(draw == false)	//move along to the next vertex
	{
		return RD_OK;
	}
	
	n_vertex = rd_polygonClipping(n_vertex, vertex_list, clipped_list);
	
	if(n_vertex > 0)
	{
		//there's something left! --- Let's draw it
		//pre process vertex list
		for(i = 0; i < n_vertex; i++)
		{				
			//convert geometry to device coordinates
			dev.x = clipped_list[i].coord[0];
			dev.y = clipped_list[i].coord[1];
			dev.z = clipped_list[i].coord[2];
			dev.w = clipped_list[i].coord[3];
			
			rd_xformPoint(clipToDevice,dev);
			
			clipped_list[i].coord[0] = dev.x;
			clipped_list[i].coord[1] = dev.y;
			
			float w = clipped_list[i].coord[3];
			
			//divide geometry by w			
			for(int j = 0; j < ATTR_SIZE; j++)
			{
				clipped_list[i].coord[j] /= w;
			}
		}
		
		//convert the global normal to world coordinates
		pointH normal = { normalVector.x, normalVector.y, normalVector.z, 1.0 };
		
		rd_xformPoint(normalXform, normal);
		
		//save the world coordinate form of the normal vector
		normalVector.x = normal.x;
		normalVector.y = normal.y;
		normalVector.z = normal.z;
		
		//scan conversion
		rd_scanConversion(clipped_list, n_vertex);
	}
	
	//reset structures for next polygon
	n_vertex = 0;

	return RD_OK;
}

int rd_scanConversion(attrPoint clipped_list[], int n_vertex)
{
	edge AET;
	
	if(!(rd_buildEdgeList(clipped_list, n_vertex)))
	{
		return RD_OK;
	}
	
	AET.next = NULL;
	
	for(int scan = 0; scan < display_ySize; scan++)
	{
		if(edgeTable[scan].next != NULL)
		{	
			rd_addActiveList(scan, &AET);
		}
		
		if(AET.next != NULL)
		{
			rd_fillBtwnEdges(scan, &AET);
			rd_updateAET(scan, &AET);
			rd_resortAET(&AET);
		}
	}
	
	return RD_OK;
}

bool rd_buildEdgeList(attrPoint clipped_list[], int n_vertex)
{
	int v1, v2;
	bool scanlineCrossed = false;
	
	v1 = n_vertex - 1;
	
	for(v2 = 0; v2 < n_vertex; v2++)
	{			
		if(ceil(clipped_list[v1].coord[1]) != ceil(clipped_list[v2].coord[1]))
		{			
			scanlineCrossed = true;
			
			if(clipped_list[v1].coord[1] < clipped_list[v2].coord[1])
			{
				rd_makeEdgeRec(clipped_list[v1], clipped_list[v2]);
			}
			else
			{
				rd_makeEdgeRec(clipped_list[v2], clipped_list[v1]);
			}
		}
		
		v1 = v2;
	}
	
	return scanlineCrossed;
}

int rd_makeEdgeRec(attrPoint lower, attrPoint upper)
{	
	float dy, factor;
	
	dy = upper.coord[1] - lower.coord[1];
	
	edge* e = new edge;
	
	attrPoint a;
	
	for(int i = 0; i < ATTR_SIZE; i++)
	{
		a.coord[i] = (upper.coord[i] - lower.coord[i]) / dy;
	}
	
	e->inc = a;
	
	factor =  ceil(lower.coord[1]) - lower.coord[1];	
	
	for(int i = 0; i < ATTR_SIZE; i++)
	{
		a.coord[i] = lower.coord[i] + factor * e->inc.coord[i];
	}
	
	e->p = a;
	
	e->yLast = ceil(upper.coord[1]) - 1;
	
	int scan = ceil(lower.coord[1]);
	
	e->next = edgeTable[scan].next;
	edgeTable[scan].next = e;
	
	return RD_OK;
}

int rd_addActiveList(int scan, edge * AET)
{
	edge * p, * q;

	p = edgeTable[scan].next; // Get the edges starting on this scan line

	while(p)
	{
		q = p->next;  // Hold the rest of the list
		rd_insertEdge(AET, p);
		p = q;
	}

	// Keep edge table clean -- edges have been transfered
	edgeTable[scan].next = 0;
	
	return RD_OK;
}

int rd_insertEdge(edge * list, edge * e)
{
	edge * p, * q = list;

	// p leads
	p = q->next;

	while(p != 0 && (e->p.coord[0] > p->p.coord[0]))
	{
		// Step to the next edge
		q = p;
		p = p->next;
	}

	// Link the edge into the list after q
	e->next = q->next;
	q->next = e;
	
	return RD_OK;
}

int rd_updateAET(int scanline, edge * active)
{
	edge *q = active, *p = active->next;
	// p leads

	while(p)
	{
		if(scanline == p->yLast)  // This is the last scanline for this edge
		{
			p = p->next;    // Move p along
			rd_deleteAfter(q);  // get rid of old node
		}
		else
		{
			// Update the attribute values
			
			for(int i = 0; i < ATTR_SIZE; i++)
			{
				p->p.coord[i] += p->inc.coord[i];
			}
			
			q = p;
			p = p->next;
		}
	}
	 
	return RD_OK;
}

int rd_deleteAfter(edge * q)
{
	edge *p = q->next;

   q->next = p->next;
   delete p;
   
   return RD_OK;
}

int rd_resortAET(edge * active)
{
	edge *q, *p = active->next;
	
	active->next = 0;
	while(p)
	{
		q = p->next;
		rd_insertEdge(active, p);
		p = q;
	}
	
	return RD_OK;
}

int rd_fillBtwnEdges(int scan, edge * active)
{
	edge * p1, * p2;

	p1 = active->next;
	
	attrPoint inc, value;
	
	while (p1)
	{
		p2 = p1->next;  // Get the pair of edges from the AET
		
		if(p1->p.coord[0] != p2->p.coord[0])
		{
			// Calculate the attribute increments along the scanline
			float dx = p2->p.coord[0] - p1->p.coord[0];
			
			for(int i = 0; i < ATTR_SIZE; i++)
			{
				inc.coord[i] = (p2->p.coord[i] - p1->p.coord[i]) / dx;
			}

			// Calculate the starting values for the edge
			float factor = ceil(p1->p.coord[0]) - p1->p.coord[0]; // Gives the fractional position of the first pixel crossing
			
			for(int i = 0; i < ATTR_SIZE; i++)
			{
				value.coord[i] = p1->p.coord[i] + factor * inc.coord[i];
			}
			
			float endx = ceil(p2->p.coord[0]);
			
			while(value.coord[0] < endx)
			{
				//save the current values into the global surface values array
				for(int i = 0; i < ATTR_SIZE; i++)
				{
					surfacePointValues.coord[i] = value.coord[i] / value.coord[ATTR_CONSTANT];
				}
				
				//get the pixel location and color												
				point plot = {value.coord[0], (float) scan, value.coord[2]};
				
				float colorPixel [3];
				rd_matteShader(colorPixel);
				
				//plot the pixel
				rd_plotPixel(plot, colorPixel);

				// Increment the values
				for(int i = 0; i < ATTR_SIZE; i++)
				{
					value.coord[i] += inc.coord[i];
				}
			}
		}
		
		p1 = p2->next;
	}
	
	return RD_OK;
}

int rd_polygonClipping(int n_vertex, attrPoint vertex_list[], attrPoint clipped_list[])
{
	static attrPoint firstSeen[6];
	static attrPoint lastSeen[6];
	static bool flagList[6];	//initialized to false
	int count = 0;
	int b = 0;
	
	for(int i = 0; i < n_vertex; i++)
	{
		rd_clipPoint(vertex_list[i], b, count, clipped_list, firstSeen, lastSeen, flagList);
	}
	
	//cout << "COUNT: " << count << endl;
	
	rd_clipLastPoint(count, clipped_list, firstSeen, lastSeen, flagList);
	
	//cout << "COUNT: " << count << endl;
	
	for(int i = 0; i < 6; i++)
	{
		flagList[i] = false;
	}
	
	return count;
	
	/*//no clipping
	for(int j = 0; j < n_vertex; j++)
	{
		clipped_list[j] = vertex_list[j];
	}
	
	return n_vertex;*/
}

int rd_clipPoint(attrPoint p, int b, int & count, attrPoint clipped_list[], attrPoint firstSeen[], attrPoint lastSeen[], bool flagList[])
{
	
	if(flagList[b] == false)
	{
		firstSeen[b] = p;
		flagList[b] = true;
	}
	else
	{
		if(rd_crossBoundary(p, lastSeen[b], b) == true)
		{
			attrPoint intersection = rd_intersectionPoint(p, lastSeen[b], b);
			
			if(b == 5)
			{
				clipped_list[count] = intersection;
				count++;
			}
			else
			{
				int bNext = b + 1;
				rd_clipPoint(intersection, bNext, count, clipped_list, firstSeen, lastSeen, flagList);
			}
		}
	}
	
	lastSeen[b] = p;
	
	if(rd_insideBoundary(p, b) == true)
	{
		if(b == 5)
		{
			clipped_list[count] = p;
			count++;
		}
		else
		{
			int bNext = b + 1;
			rd_clipPoint(p, bNext, count, clipped_list, firstSeen, lastSeen, flagList);
		}
	}
	
	return RD_OK;
}

int rd_clipLastPoint(int & count, attrPoint clipped_list[], attrPoint firstSeen[], attrPoint lastSeen[], bool flagList[])
{	
	for(int b = 0; b < 6; b++)
	{
		if(flagList[b] == true && (rd_crossBoundary(firstSeen[b], lastSeen[b], b) == true))
		{
			attrPoint intersection = rd_intersectionPoint(lastSeen[b], firstSeen[b], b);
			
			if(b == 5)
			{
				clipped_list[count] = intersection;
				count++;
			}
			else
			{
				rd_clipPoint(intersection, b, count, clipped_list, firstSeen, lastSeen, flagList);
			}
		}
	}
	
	return RD_OK;
}

attrPoint rd_intersectionPoint(attrPoint p0, attrPoint p1, int b)
{
	attrPoint intersection = {0};
	float alpha;
	
	switch(b)
	{
		case 0 :
			alpha = p0.coord[0] / (p0.coord[0] - p1.coord[0]);
			break;
		case 1 :
			alpha = (p0.coord[3] - p0.coord[0]) / ((p0.coord[3] - p0.coord[0]) - (p1.coord[3] - p1.coord[0]));
			break;
		case 2 :
			alpha = p0.coord[1] / (p0.coord[1] - p1.coord[1]);
			break;
		case 3 :
			alpha = (p0.coord[3] - p0.coord[1]) / ((p0.coord[3] - p0.coord[1]) - (p1.coord[3] - p1.coord[1]));
			break;
		case 4 :
			alpha = p0.coord[2] / (p0.coord[2] - p1.coord[2]);
			break;
		case 5 :
			alpha = (p0.coord[3] - p0.coord[2]) / ((p0.coord[3] - p0.coord[2]) - (p1.coord[3] - p1.coord[2]));
			break;
	}
		
	for(int i = 0; i < ATTR_SIZE; i++)
	{
		intersection.coord[i] = p0.coord[i] + alpha * (p1.coord[i] - p0.coord[i]);
	}
	
	for(int i = 0; i < 3; i++)
	{
		if((intersection.coord[i] < 0) && (b == 0 || b == 2 || b == 4))
		{
			intersection.coord[i] = ceil(intersection.coord[i]);
		}/*
		else if((intersection.coord[i] > 1) && (b == 1 || b == 3 || b == 5))
		{
			intersection.coord[i] = floor(intersection.coord[i]);
		}*/
	}
	
	return intersection;
}

bool rd_crossBoundary(attrPoint p0, attrPoint p1, int b)
{
	if(rd_insideBoundary(p0, b) != rd_insideBoundary(p1, b))
	{
		return true;
	}
	
	return false;
}

bool rd_insideBoundary(attrPoint p, int b)
{
	int code = 0;
	
	//compute boundary coordinates
	float BC[6] = {p.coord[0], p.coord[3] - p.coord[0], p.coord[1], p.coord[3] - p.coord[1], p.coord[2], p.coord[3] - p.coord[2]};
	
	//compute boundary codes
	for(int i = 0; i < 6; i++)
	{
		if(BC[i] < 0)
		{
			code |= (1<<i);
		}
	}
	switch(b)
	{
		case 0 :
			if(!(code & (1<<b)))
			{
				return true;
			}
			break;
		case 1 :
			if(!(code & (1<<b)))
			{
				return true;
			}
			break;
		case 2 :
			if(!(code & (1<<b)))
			{
				return true;
			}
			break;
		case 3 :
			if(!(code & (1<<b)))
			{
				return true;
			}
			break;
		case 4 :
			if(!(code & (1<<b)))
			{
				return true;
			}
			break;
		case 5 :
			if(!(code & (1<<b)))
			{
				return true;
			}
			break;
	}
	
	return false;
}

void rd_printPointA(attrPoint a)
{
	cout << a.coord[0] << ", " << a.coord[1] << ", " << a.coord[2] << ", " << a.coord[3] << ", " << endl;
}
