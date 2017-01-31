/***********************************************************************
Class:		CSCI 697 (Graphics Independent Study)
Program:	pnm_display.h
Author:		Drew Stefanik
Date Due:	09/30/2016

Purpose:	header file for pnm_display.cc

Notes:		part of rd_view.exe

***********************************************************************/

#ifndef PNM_DISPLAY_H
#define PNM_DISPLAY_H

#if defined (__cplusplus)
extern "C"
{
#endif

  //global variables
  extern float * imageArray;
  
  extern int frameNumber;
  
  extern float PNMintensity;
  
  extern float PNMdrawColor [3];
  
  extern float PNMbackgroundColor[3];

  //function headers
  int pnm_init_display(void);

  int pnm_end_display(void);

  int pnm_init_frame(int);

  int pnm_end_frame(void);

  int pnm_write_pixel(int x, int y, const float rgb []);

  int pnm_read_pixel(int x, int y, float rgb []);

  int pnm_set_background(const float rgb []);

  int pnm_clear(void);

#if defined (__cplusplus)
}
#endif

#endif /* PNM_DISPLAY_H */
