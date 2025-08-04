/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/
/*                                                                            */
/* StrokeGraphicsUtilities.h :                                                */
/* Paul O'Brien                                                               */
/* 01.07.25                                                                   */
/*                                                                            */
/* 2D GUI to create stroke/vector graphic alphanumeric set                    */
/******************************************************************************/

#ifndef _STROKE_GRAPHICS_UTILITIES_H_
#define _STROKE_GRAPHICS_UTILITIES_H_

/******************************************************************************/

#include "resource.h"
#include <Windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include "StrokeGraphicsTypes.h"

/******************************************************************************/

// Analagous to the stroke cell line types
typedef enum graphLinePointMarkers_tTag
  {
  GRAPH_LINE_POINT_MARKER_NONE = 0,
  GRAPH_LINE_POINT_MARKER_LINE_START,
  GRAPH_LINE_POINT_MARKER_LINE_END,
  GRAPH_LINE_POINT_MARKER_LINE_MID,
  GRAPH_LINE_POINT_MARKER_FIRST_LINE_START,
  GRAPH_LINE_POINT_MARKER_LAST_LINE_START,
  GRAPH_LINE_POINT_MARKERS
  } graphLinePointMarkers_t, *graphLinePointMarkers_tPtr;

#pragma pack(push,1)
typedef struct strokeGraphLinePoint_tTag
  {
  GRAPHICS_REAL              pointX;
  GRAPHICS_REAL              pointY;
  graphLinePointMarkers_t    pointMarker;
  strokeGraphLinePoint_tTag *nextStrokeGraphLine;
  strokeGraphLinePoint_tTag *lastStrokeGraphLine;
  } strokeGraphLinePoint_t, *strokeGraphLinePoint_tPtr;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct objectColour_tTag
  {
  BYTE opacity;
  BYTE red;
  BYTE green;
  BYTE blue;
  } objectColour_t, *objectColour_tPtr;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct strokeGraphPointBase_tTag
  {
  GRAPHICS_UINT             graphRowNumber;
  GRAPHICS_UINT             graphColumnNumber;
  strokeGraphLinePoint_tPtr graphPoints;
  objectColour_t            activeLineStrokeColour;
  objectColour_t            passiveLineStrokeColour;
  objectColour_t            editLineStrokeColour;
  GRAPHICS_REAL             lineWidth;
  GRAPHICS_REAL             descenderRatio; // a value in the range { 0, 1 } used to define the 
                                            // boundary below which line segments are part of a 
                                            // characters' descender
  } strokeGraphPointBase_t, *strokeGraphPointBase_tPtr;
#pragma pack(pop)

/******************************************************************************/

#endif

/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/