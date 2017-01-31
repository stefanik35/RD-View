/***********************************************************************
Class:		CSCI 697 (Graphics Independent Study)
Program:	pnm_display.cc
Author:		Drew Stefanik
Date Due:	09/30/2016

Purpose:	contains functions and variables to implement ppm output
			from .rd files

Notes:		part of rd_view.exe

***********************************************************************/

#include "pnm_display.h"
#include "rd_direct.h"
#include "rd_display.h"
#include "rd_error.h"
#include <iostream>		//ios
#include <fstream>		//ofstream
#include <string>		//string
#include <sstream>		//ostringstream

using std::ofstream;
using std::ios;
using std::ostringstream;

//using std::cout;
//using std::endl;

//global variables
float * imageArray;

int frameNumber;

float PNMintensity = 255.0;

float PNMdrawColor [3];

float PNMbackgroundColor[3]  = {0.0, 0.0, 0.0};

//creates an image array based on the size of the display
int pnm_init_display(void)
{
	imageArray = new float [display_xSize * display_ySize * 3];
	
	return RD_OK;
}

//clears up the memory allocated to the image array
int pnm_end_display(void)
{
	delete [] imageArray;
	
	return RD_OK;
}

//clears the image array and sets frameNumber equal to input frameNum
int pnm_init_frame(int frameNum)
{
	pnm_clear();
	
	frameNumber = frameNum;
	
	return RD_OK;
}

//writes the image array out to a P6 ppm file
int pnm_end_frame(void)
{
	//declares the output file
	ofstream outFile;
	
	//declares variables needed to append the frame number to the output file name
	string outFileName;
	string frameNumberString;
	ostringstream ss;
	
	//reads the integer frameNumber into a stringstream and the saves it in a string
	ss << frameNumber;
	frameNumberString = ss.str();
	
	//pads the frameNumberString with 0's if necessary (frameNumberString should be four characters long)
	if(frameNumberString.length() < 4)
	{
		frameNumberString.insert(0, 4 - frameNumberString.length(), '0');
	}
	
	//forms the outputfileName
	outFileName = display_name;
	outFileName.append(frameNumberString);
	outFileName.append(".ppm");

	//opens outFile
	outFile.open(outFileName.c_str(), ios::binary);
	
	//output the ppm header
	outFile << "P6\n" << display_xSize << " " << display_ySize << "\n" << PNMintensity << "\n";
	
	//declare a variable to hold the pixel color
	float pixelColor [3];
	
	//output RGB values, pixel by pixel
	//row first, then column
	for(int y = 0; y < display_ySize; y++)
	{
		for(int x = 0; x < display_xSize; x++)
		{
			pnm_read_pixel(x, y, pixelColor);
			
			//adds a small amount to the rgb values so that they scale properly
			outFile.put(pixelColor[0] * (PNMintensity + 0.999));
			outFile.put(pixelColor[1] * (PNMintensity + 0.999));
			outFile.put(pixelColor[2] * (PNMintensity + 0.999));
		}
	}
	
	//closes outFile
	outFile.close();
	
	return RD_OK;
}

//sets the color in the image array at the input location (x, y) to the input rgb
int pnm_write_pixel(int x, int y, const float rgb [])
{
	imageArray[(y * display_xSize + x)*3] = rgb[0];
	imageArray[(y * display_xSize + x)*3 + 1] = rgb[1];
	imageArray[(y * display_xSize + x)*3 + 2] = rgb[2];
	
	return RD_OK;
}

//reads the color values at the input location (x, y) in the image array into input rgb
int pnm_read_pixel(int x, int y, float rgb [])
{
	rgb[0] = imageArray[(y * display_xSize + x)*3];
	rgb[1] = imageArray[(y * display_xSize + x)*3 + 1];
	rgb[2] = imageArray[(y * display_xSize + x)*3 + 2];
	
	return RD_OK;
}

//sets the PNMbackgroundColor to input rgb
int pnm_set_background(const float rgb [])
{
	PNMbackgroundColor[0] = rgb[0];
	PNMbackgroundColor[1] = rgb[1];
	PNMbackgroundColor[2] = rgb[2];
	
	return RD_OK;
}

//sets the entire image array to be equal to PNMbackgroundColor
int pnm_clear(void)
{
	for(int i = 0; i < display_xSize * display_ySize * 3; i += 3)
	{
		imageArray[i] = PNMbackgroundColor[0];
		imageArray[i+1] = PNMbackgroundColor[1];
		imageArray[i+2] = PNMbackgroundColor[2];
	}
	
	return RD_OK;
}
