/***********************************************************************
Class:		CSCI 697 (Graphics Independent Study)
Program:	rd_direct.h
Author:		Drew Stefanik
Date Due:	09/30/2016

Purpose:	header file for rd_direct.cc

Notes:		part of rd_view.exe

***********************************************************************/

#ifndef RD_ENGINE_DIRECT_H
#define RD_ENGINE_DIRECT_H

#include "rd_dataStructures.h"
#include "rd_display.h"
#include "rd_drawLine.h"
#include "rd_drawCircle.h"
#include "rd_drawingPipelines.h"
#include "rd_drawObject.h"
#include "rd_enginebase.h"
#include "rd_error.h"
#include "rd_floodFill.h"
#include "rd_graphicsPipeline.h"
#include "rd_lighting.h"
#include <string>

using std::string;

// This is a rendering engine that renders directly to the image buffer as
// primitives come in.  A depth buffer is obviously needed.  Transparency is
// not properly handled.

//global variables
extern int frameNum;

extern float drawColor [3];
extern float backgroundColor [3];

class REDirect: public RenderEngine
{
 public:

  int rd_display(const string & name, const string & type, 
		 const string & mode);

  int rd_format(int xresolution, int yresolution);

  int rd_frame_begin(int frame_no);
  int rd_frame_end(void);

//  int rd_render_init(void);  /* Initialize renderer */
  int rd_render_cleanup(void);

  int rd_point(const float point[3]);
  int rd_line(const float start[3], const float end[3]);
  int rd_translate(const float offset[3]);
  int rd_scale(const float scale_factor[3]);
  int rd_rotate_xy(float angle);
  int rd_rotate_yz(float angle);
  int rd_rotate_zx(float angle);
  int rd_matrix(const float * mat);
  int rd_camera_eye(const float eyepoint[3]);
  int rd_camera_at(const float atpoint[3]);
  int rd_camera_up(const float up[3]);
  int rd_camera_fov(const float fov);
  int rd_clipping(float znear, float zfar);
  int rd_world_begin(void);
  int rd_world_end(void);
/*
  int rd_bezier_curve(const string & vertex_type,
		      int degree, const float * vertex);
  int rd_bezier_patch(const string & vertex_type,
		      int u_degree, int v_degree, const float * vertex);

  int rd_lineset(const string & vertex_type,
		 int nvertex, const float * vertex,
		 int nseg, const int * seg);
  */int rd_pointset(const string & vertex_type,
		  int nvertex, const float * vertex);
  int rd_polyset(const string & vertex_type, 
		 int nvertex, const float * vertex,
		 int nface,   const int * face);
  int rd_cube(void);
  int rd_sphere(float radius, float zmin, float zmax, float thetamax);
  int rd_cone(float height, float radius, float thetamax);
  int rd_disk(float height, float radius, float thetamax);
  int rd_cylinder(float radius, float zmin, float zmax, float thetamax);
  
  /*int rd_tube(const float start[3], const float end[3], float radius);
  int rd_hyperboloid(const float start[3], const float end[3], float thetamax);
  int rd_paraboloid(float rmax, float zmin, float zmax, float thetamax);
  int rd_torus(float radius1, float radius2, 
	       float phimin, float phimax, float thetamax);

  int rd_sqsphere(float radius, float north, float east, 
		  float zmin, float zmax, float thetamax); 
  int rd_sqtorus(float radius1, float radius2, 
		 float north, float east, float phimin, float phimax, 
		 float thetamax);*/
  
  int rd_background(const float color[]);
  int rd_color(const float color[]);

  int rd_circle(const float center[3], float radius);
  int rd_fill(const float seed_point[3]);

  int rd_xform_push(void);
  int rd_xform_pop(void);

#ifndef NO_SHADING
  int rd_surface(const string & shader_type);

  int rd_point_light(const float pos[3], const float color[], 
		     float intensity);
  int rd_far_light  (const float dir[3], const float color[], 
		     float intensity);
  int rd_ambient_light(const float color[], float intensity);

  //int rd_cone_light(const float pos[3], const float at[3], float theta_min, float theta_max, const float color[], float intensity);

  int rd_specular_color(const float color[], int exponent);

  int rd_k_ambient(float Ka);
  int rd_k_diffuse(float Kd);
  int rd_k_specular(float Ks); 
#endif

/*
#ifndef NO_TEXTURE
  int rd_map_border(const string & map_type, 
		    const string & horizontal, const string & vertical);
  int rd_map_bound(const string & map_type,
		   float s_min, float t_min, float s_max, float t_max);
  int rd_map_load(const string & filename, const string & label);
  int rd_map_sample(const string & map_type,
		    const string & intra_level, 
		    const string & inter_level);
  int rd_map(const string & map_type, const string & name);
#endif *//* NO_TEXTURE */
/*
  int rd_option_bool(const string &, bool flag);
  int rd_option_real(const string &, float value);

  int rd_custom(const string & label);
  */
};

#endif /* RD_ENGINE_DIRECT_H */
