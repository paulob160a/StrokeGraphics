/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/
/*                                                                            */
/* StrokeGraphicsUtilities.cpp :                                              */
/* Paul O'Brien                                                               */
/* 28.05.25                                                                   */
/*                                                                            */
/* 2D GUI to create stroke/vector graphic alphanumeric set                    */
/******************************************************************************/

#include <Windows.h>
#include <gdiplusenums.h>
#include <objidl.h>
#include <gdiplus.h>
#include <stdio.h>
#include <math.h>
#include "StrokeGraphicsAlphabet.h"
#include "StrokeGraphicsCaption.h"
#include "StrokeGraphicsListHandlers.h"

/******************************************************************************/

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

/******************************************************************************/

// The screen coordinates are initially set to full HD
globalScreenCoordinates_t globalScreenCoordinates  = {
                                                     GUI_SCREEN_GUI_DEFAULT_LEFT_X,
                                                     GUI_SCREEN_GUI_DEFAULT_TOP_Y,
                                                     GUI_SCREEN_GUI_DEFAULT_RIGHT_X,
                                                     GUI_SCREEN_GUI_DEFAULT_BOTTOM_Y
                                                     };
                                                              
canvasDescriptor_t        canvasSize               = { CANVAS_SIZE_ZERO_X, CANVAS_SIZE_ZERO_Y, CANVAS_SIZE_WIDTH, CANVAS_SIZE_HEIGHT },
                          localCanvasSize          = { CANVAS_SIZE_ZERO_X, CANVAS_SIZE_ZERO_Y, CANVAS_SIZE_WIDTH, CANVAS_SIZE_HEIGHT };
                                                   
bool                      mouseOverObject          = false; // flag when the mouse pointer is in an objects' perimeter
bool                      objectPositionTestFlag   = false;
                                                   
GRAPHICS_SHORT            mouseXPosition           = ((GRAPHICS_SHORT)0);
GRAPHICS_SHORT            mouseYPosition           = ((GRAPHICS_SHORT)0);
                                                   
characterSelectionState_t characterSelectionState  = {
                                                     ((GRAPHICS_UINT)0x00),
                                                     CHARACTER_SELECTION_STATE_NONE,
                                                     CHARACTER_SELECTION_STATE_NONE,
                                                     CHARACTER_SELECTION_STATE_MODIFY_OFF,
                                                     STROKE_CURSOR_TOGGLE_STATE_NOT_SELECTED
                                                     };
                                                   
strokeFrame_t             characterFrame           = { 
                                                       {
                                                       STROKE_FRAME_LEFT_X,
                                                       STROKE_FRAME_TOP_Y,
                                                       STROKE_FRAME_RIGHT_X,
                                                       STROKE_FRAME_BOTTOM_Y 
                                                       },
                                                       {
                                                       STROKE_FRAME_COLOUR
                                                       },
                                                       STROKE_FRAME_COLOUR_CODE,
                                                       {
                                                       MIDDLE_FRAME_COLOUR,
                                                       },
                                                       MIDDLE_FRAME_COLOUR_CODE,
                                                       {
                                                       DESCENT_FRAME_COLOUR
                                                       },
                                                       DESCENT_FRAME_COLOUR_CODE,
                                                     nullptr,
                                                     STROKE_FRAME_X_AXIS_POINTS,
                                                     STROKE_FRAME_Y_AXIS_POINTS
                                                     };

alphabetCharacters_tPtr   characterReference       = nullptr;
                                                   
strokeCellPoint_tPtr      cellPoint                = nullptr,
                          lastCellPoint            = nullptr;
                                                   
strokeCell_tPtr           alphabetStrokeCell       = nullptr,
                          lastStrokeCell           = nullptr;
                                                   
strokeDrawingState_t      strokeDrawingState       = STROKE_DRAWING_STATE_NONE;
                                                   
strokeGraphLinePoint_t    strokeGraphPoints        = { 
                                                     ((GRAPHICS_REAL)0.0),
                                                     ((GRAPHICS_REAL)0.0),
                                                     GRAPH_LINE_POINT_MARKER_NONE,
                                                     nullptr,
                                                     nullptr
                                                     };
                                                   
strokeNavKeyStates_t      strokeActiveCursor       = STROKE_NAV_KEY_STATE_NONE;
                                                   
strokeNavStates_t         strokeNavigationState    = STROKE_NAV_STATE_NONE;

strokeCursor_t            strokeStartCursor        = {
                                                       {
                                                       ((GRAPHICS_UINT)0),
                                                       ((GRAPHICS_UINT)0),
                                                       },
                                                       {
                                                       ((GRAPHICS_UINT)0),
                                                       ((GRAPHICS_UINT)0),
                                                       },
                                                     STROKE_CURSOR_MAXIMUM_CAPTURE_EXTENT,
                                                     { STROKE_START_CURSOR_DEFAULT },
                                                     STROKE_START_CURSOR_DEFAULT_CODE,
                                                     { STROKE_START_CURSOR_LIT },
                                                     STROKE_START_CURSOR_LIT_CODE,
                                                     STROKE_NAV_KEY_STATE_NONE,
                                                     CURSOR_LINE_WIDTH
                                                     },
                                                   
                          strokeEndCursor          = {
                                                       {
                                                       ((GRAPHICS_UINT)0),
                                                       ((GRAPHICS_UINT)0),
                                                       },
                                                       {
                                                       ((GRAPHICS_UINT)0),
                                                       ((GRAPHICS_UINT)0),
                                                       },
                                                     STROKE_CURSOR_MAXIMUM_CAPTURE_EXTENT,
                                                     { STROKE_END_CURSOR_DEFAULT },
                                                     STROKE_END_CURSOR_DEFAULT_CODE,
                                                     { STROKE_END_CURSOR_LIT },
                                                     STROKE_END_CURSOR_LIT_CODE,
                                                     STROKE_NAV_KEY_STATE_NONE,
                                                     CURSOR_LINE_WIDTH
                                                     };
                                                   
rectangleObject_t         lineSegmentOrigin        = {
                                                       {
                                                       GRAPHICS_LINE_ORIGIN_COLOUR
                                                       },
                                                       {
                                                       NO_COLOUR_PEN,
                                                       },
                                                       GRAPHICS_OBJECT_FILL_STATE_EMPTY,
                                                       {
                                                       GRAPHICS_LINE_ORIGIN_LEFT_X,
                                                       GRAPHICS_LINE_ORIGIN_TOP_Y,
                                                       GRAPHICS_LINE_ORIGIN_RIGHT_X,
                                                       GRAPHICS_LINE_ORIGIN_BOTTOM_Y
                                                       },
                                                     };
                                                   
rectangleObject_t         lineSegmentDestination   = {
                                                       {
                                                       GRAPHICS_LINE_DESTINATION_COLOUR
                                                       },
                                                       {
                                                       NO_COLOUR_PEN,
                                                       },
                                                       GRAPHICS_OBJECT_FILL_STATE_EMPTY,
                                                       {
                                                       GRAPHICS_LINE_DESTINATION_LEFT_X,
                                                       GRAPHICS_LINE_DESTINATION_TOP_Y,
                                                       GRAPHICS_LINE_DESTINATION_RIGHT_X,
                                                       GRAPHICS_LINE_DESTINATION_BOTTOM_Y
                                                       },
                                                    };
                                                   
newLineSegment_t          lineSegment             = {
                                                    ((GRAPHICS_UINT)0),
                                                    ((GRAPHICS_UINT)0),
                                                    ((GRAPHICS_UINT)0),
                                                    ((GRAPHICS_UINT)0),
                                                    false
                                                    };
                                                   
lineSegment_tPtr          nextLineSegment         = nullptr;

colourPalette_t           colourPalette[LINE_SEGMENT_COLOURS] = {
                                                                  {
                                                                  BLUE_PEN
                                                                  },
                                                                  {
                                                                  GREEN_PEN
                                                                  },
                                                                  {
                                                                  RED_PEN
                                                                  },
                                                                  {
                                                                  CYAN_PEN
                                                                  },
                                                                  {
                                                                  MAGENTA_PEN
                                                                  },
                                                                  {
                                                                  YELLOW_PEN
                                                                  },
                                                                  {
                                                                  BLACK_PEN_OPAQUE
                                                                  },
                                                                  {
                                                                  WHITE_PEN_OPAQUE
                                                                  },
                                                                  {
                                                                  DARK_GREEN_PEN,
                                                                  },
                                                                  {
                                                                  BRIGHT_YELLOW_PEN,
                                                                  },
                                                                  {
                                                                  LIGHT_BLUE_PEN
                                                                  }
                                                                };

/******************************************************************************/

graphicsError_t computeCanvasCentre(canvasDescriptor_tPtr canvasSize,
                                    singleIndexReal_tPtr  canvasCentre)
  {
/******************************************************************************/

  graphicsError_t canvasError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((canvasSize != nullptr) && (canvasCentre != nullptr))
    {
    if ((canvasSize->right > canvasSize->left) && (canvasSize->bottom > canvasSize->top))
      {
      canvasCentre->yAxisIndex = (canvasSize->bottom - canvasSize->top)  / (canvasSize->height * ((GRAPHICS_REAL)2.0));
      canvasCentre->xAxisIndex = (canvasSize->right  - canvasSize->left) / (canvasSize->width  * ((GRAPHICS_REAL)2.0));
      }
    else
      {
      canvasError = GRAPHICS_OBJECT_RANGE_ERROR;
      }
    }
  else
    {
    canvasError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(canvasError);

/******************************************************************************/
  } /* end of computeCanvasCentre                                             */

/******************************************************************************/

graphicsError_t setCanvasCoordinates(canvasDescriptor_tPtr canvasSize,
                                     GRAPHICS_REAL         left,
                                     GRAPHICS_REAL         top,
                                     GRAPHICS_REAL         right,
                                     GRAPHICS_REAL         bottom,
                                     GRAPHICS_REAL         width,
                                     GRAPHICS_REAL         height)
  {
/******************************************************************************/

  graphicsError_t canvasError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  canvasSize->left    = left;
  canvasSize->right   = right;
  canvasSize->top     = top;
  canvasSize->bottom  = bottom;

  canvasSize->width   = width;
  canvasSize->centreX = (GRAPHICS_REAL)ceil((double)(width / ((GRAPHICS_REAL)2.0)));

  canvasSize->height  = height;
  canvasSize->centreY = (GRAPHICS_REAL)ceil((double)(height / ((GRAPHICS_REAL)2.0)));

/******************************************************************************/

  return(canvasError);

/******************************************************************************/
  } /* end of setCanvasCoordinates                                            */

/******************************************************************************/
/* buildCharacterStrokeGraph() :                                              */
/* - create a grid of { 0..n, 0..m} points to describe the matrix of points/  */
/*   lines that will be drawn as the character-creation grid                  */
/*   NOTE : the row-/column-numbers are INCLUSIVE i.e. 0..N = (N + 1) rows/   */
/*          columns                                                           */
/******************************************************************************/

graphicsError_t buildCharacterStrokeGraph(strokeFrame_tPtr          characterFrame,
                                          strokeGraphPointBase_tPtr strokeGraphBase)
    {
/******************************************************************************/

  GRAPHICS_UINT             graphicLineIndexX                   = ((GRAPHICS_UINT)0),
                            graphicLineIndexY                   = ((GRAPHICS_UINT)0),
                            graphicLineLengthX                  = ((GRAPHICS_UINT)0),
                            graphicLineLengthY                  = ((GRAPHICS_UINT)0);
                           
  GRAPHICS_REAL             offsetDeltaX                        = ((GRAPHICS_REAL)0.0),
                            offsetDeltaY                        = ((GRAPHICS_REAL)0.0),
                            pageOffsetX                         = ((GRAPHICS_REAL)0.0),
                            pageOffsetY                         = ((GRAPHICS_REAL)0.0);

  GRAPHICS_UCHAR            characterCode[GRAPHICS_CHAR_LENGTH] = { ((GRAPHICS_UCHAR)ALPHABET_SIZE) };
  size_t                    characterCodeLength                 = ((size_t)0);
  errno_t                   characterError                      = ((errno_t)0);

  strokeGraphLinePoint_tPtr point                               = nullptr,
                            lastPoint                           = nullptr;

  graphicsError_t           objectError                         = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((characterFrame != nullptr) && (strokeGraphBase != nullptr) && (strokeGraphBase->graphPoints == nullptr))
    { // First, construct the points array that describes the character frame
      // How many points in the x- and y-axes ?
      if (((graphicLineLengthX = ((GRAPHICS_UINT)ceil(characterFrame->strokeXAxisPoints))) >= STROKE_FRAME_X_MINIMUM_POINTS_UINT) &&
          ((graphicLineLengthY = ((GRAPHICS_UINT)ceil(characterFrame->strokeYAxisPoints))) >= STROKE_FRAME_Y_MINIMUM_POINTS_UINT))
        {
        // Force the grid dimensions { x, y } to be even
        if ((graphicLineLengthX & STROKE_FRAME_DETECT_ODD_EVEN) == STROKE_FRAME_DETECT_ODD_EVEN)
          {
          graphicLineLengthX = graphicLineLengthX + ((GRAPHICS_UINT)1);
          }

        if ((graphicLineLengthY & STROKE_FRAME_DETECT_ODD_EVEN) == STROKE_FRAME_DETECT_ODD_EVEN)
          {
          graphicLineLengthY = graphicLineLengthY + ((GRAPHICS_UINT)1);
          }

        // The grid has a normalised page-plotting range that is added to each point coordinate
        offsetDeltaX = characterFrame->frameCoordinates.frameRightX  - characterFrame->frameCoordinates.frameLeftX;
        offsetDeltaY = characterFrame->frameCoordinates.frameBottomY - characterFrame->frameCoordinates.frameTopY;
        
        // Instantiate the first point
        if ((point = (strokeGraphLinePoint_tPtr)calloc(((size_t)1), sizeof(strokeGraphLinePoint_t))) != nullptr)
          {
          // Load the 'point' pointer into the base and initialise the row/column values
          strokeGraphBase->graphPoints       = point;
          strokeGraphBase->graphRowNumber    = ((GRAPHICS_UINT)0);
          strokeGraphBase->graphColumnNumber = ((GRAPHICS_UINT)0);

          // Build the basic x- and y-grid in the same format as the alphanumeric cells
          for (graphicLineIndexX = ((GRAPHICS_UINT)0); graphicLineIndexX <= graphicLineLengthX; graphicLineIndexX++)
            {
            if (objectError != GRAPHICS_NO_ERROR)
              {
              break;
              }
  
            for (graphicLineIndexY = ((GRAPHICS_UINT)0); graphicLineIndexY <= graphicLineLengthY; graphicLineIndexY++)
              {
              if (objectError != GRAPHICS_NO_ERROR)
                {
                break;
                }
  
              // Mark the line starts' and ends'
              if (graphicLineIndexX == ((GRAPHICS_UINT)0))
                {
                if (graphicLineIndexY == ((GRAPHICS_UINT)0))
                  {
                  strokeGraphBase->graphRowNumber    = ((GRAPHICS_UINT)0);
                  strokeGraphBase->graphColumnNumber = ((GRAPHICS_UINT)0);

                  point->pointMarker = GRAPH_LINE_POINT_MARKER_FIRST_LINE_START;
                  }
                else
                  {
                  if (graphicLineIndexY == graphicLineLengthY)
                    {
                    strokeGraphBase->graphColumnNumber = strokeGraphBase->graphColumnNumber + ((GRAPHICS_UINT)1);

                    point->pointMarker = GRAPH_LINE_POINT_MARKER_LINE_END;
                    }
                  else
                    {
                    strokeGraphBase->graphColumnNumber = strokeGraphBase->graphColumnNumber + ((GRAPHICS_UINT)1);

                    point->pointMarker = GRAPH_LINE_POINT_MARKER_LINE_MID;
                    }
                  }
                }
              else
                {
                if (graphicLineIndexX == graphicLineLengthX)
                  {
                  if (graphicLineIndexY == ((GRAPHICS_UINT)0))
                    {
                    strokeGraphBase->graphRowNumber    = strokeGraphBase->graphRowNumber + ((GRAPHICS_UINT)1);
                    strokeGraphBase->graphColumnNumber = ((GRAPHICS_UINT)0);

                    point->pointMarker = GRAPH_LINE_POINT_MARKER_LAST_LINE_START;
                    }
                  else
                    {
                    if (graphicLineIndexY == graphicLineLengthY)
                      {
                      strokeGraphBase->graphColumnNumber = strokeGraphBase->graphColumnNumber + ((GRAPHICS_UINT)1);

                      point->pointMarker = GRAPH_LINE_POINT_MARKER_LINE_END;
                      }
                    else
                      {
                      strokeGraphBase->graphColumnNumber = strokeGraphBase->graphColumnNumber + ((GRAPHICS_UINT)1);

                      point->pointMarker = GRAPH_LINE_POINT_MARKER_LINE_MID;
                      }
                    }
                  }
                else
                  {
                  if (graphicLineIndexY == ((GRAPHICS_UINT)0))
                    {
                    strokeGraphBase->graphRowNumber    = strokeGraphBase->graphRowNumber + ((GRAPHICS_UINT)1);
                    strokeGraphBase->graphColumnNumber = ((GRAPHICS_UINT)0);

                    point->pointMarker = GRAPH_LINE_POINT_MARKER_LINE_START;
                    }
                  else
                    {
                    if (graphicLineIndexY == graphicLineLengthY)
                      {
                      strokeGraphBase->graphColumnNumber = strokeGraphBase->graphColumnNumber + ((GRAPHICS_UINT)1);

                      point->pointMarker = GRAPH_LINE_POINT_MARKER_LINE_END;
                      }
                    else
                      {
                      strokeGraphBase->graphColumnNumber = strokeGraphBase->graphColumnNumber + ((GRAPHICS_UINT)1);

                      point->pointMarker = GRAPH_LINE_POINT_MARKER_LINE_MID;
                      }
                    }
                  }
                }
              // NOTE : the point values are first normalised in the range { 0.0..1.0 }
              point->pointX  = (GRAPHICS_REAL)graphicLineIndexX / graphicLineLengthX;
              point->pointY  = (GRAPHICS_REAL)graphicLineIndexY / graphicLineLengthY;
  
              point->pointX = (point->pointX  * offsetDeltaX) + characterFrame->frameCoordinates.frameLeftX; 
              point->pointY = (point->pointY  * offsetDeltaY) + characterFrame->frameCoordinates.frameTopY;
  
              point->nextStrokeGraphLine = nullptr;
  
              if ((lastPoint = (strokeGraphLinePoint_tPtr)calloc(((size_t)1), sizeof(strokeGraphLinePoint_t))) != nullptr)
                {
                point->nextStrokeGraphLine = lastPoint;
                point                      = lastPoint;
                }
              else
                {
                objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
                }
              }
            }
          }
        else
          {
          objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
          }
        }
      else
        {
        objectError = GRAPHICS_OBJECT_RANGE_ERROR;
        }
      }
    else
      {
      objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
      }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
    } /* end of buildCharacterStrokeGraph                                     */

/******************************************************************************/
/* drawCharacterStrokeGraph() :                                               */
/* - the stroke graph points are a set of NORMALISED point values in the      */
/*   in the range { 0.0..1.0 }. The canvas size coordinates are actual screen */
/*   coordinates in the range { { 0, 0 }..{ <screen size x-axis>..            */
/*   <screen size y-axis>. Hence multiplying the graph points by the screen   */
/*   coordinates translates the stroke graph onto the screen.                 */
/******************************************************************************/

graphicsError_t drawCharacterStrokeGraph(HDC                       hdc,
                                         objectColour_tPtr         strokeColour,
                                         objectColour_tPtr         middleColour,
                                         objectColour_tPtr         descentColour,
                                         canvasDescriptor_tPtr     canvasSize,
                                         strokeGraphPointBase_tPtr strokeGraphPoints)
  {
/******************************************************************************/

  Graphics                  graphics(hdc);

  strokeGraphLinePoint_tPtr xAxisPoints   = nullptr,
                            yAxisPoints   = nullptr,
                            xAxisNewPoint = nullptr,
                            yAxisNewPoint = nullptr,
                            graphPoint    = nullptr,

                            xAxisIndex    = nullptr,
                            yAxisIndex    = nullptr;

  GRAPHICS_FLOAT            xDrawingPoint  = ((GRAPHICS_FLOAT)0);
  GRAPHICS_FLOAT            yDrawingPoint  = ((GRAPHICS_FLOAT)0),
                            xStart         = ((GRAPHICS_FLOAT)0),
                            xEnd           = ((GRAPHICS_FLOAT)0),
                            yStart         = ((GRAPHICS_FLOAT)0),
                            yEnd           = ((GRAPHICS_FLOAT)0);

  GRAPHICS_UINT             rowIndex       = ((GRAPHICS_UINT)0),
                            columnIndex    = ((GRAPHICS_UINT)0);
                                           
  graphicsError_t           objectError    = GRAPHICS_NO_ERROR;

  REAL                      dashPattern[DASH_PATTERN_STYLE] = { DASH_PATTERN_FILLED, DASH_PATTERN_EMPTY, DASH_PATTERN_FILLED, DASH_PATTERN_EMPTY };

/******************************************************************************/

  if ((canvasSize != nullptr) && (strokeGraphPoints != nullptr) && (strokeGraphPoints->graphPoints != nullptr))
    {
    if ((strokeGraphPoints->graphRowNumber != ((GRAPHICS_UINT)0)) && (strokeGraphPoints->graphColumnNumber != ((GRAPHICS_UINT)0)))
      {
      // Allocate space for the first x-axis and y-axis points
      if ((xAxisPoints = (strokeGraphLinePoint_tPtr)calloc(((size_t)1), sizeof(strokeGraphLinePoint_t))) != nullptr)
        {
        if ((yAxisPoints = (strokeGraphLinePoint_tPtr)calloc(((size_t)1), sizeof(strokeGraphLinePoint_t))) != nullptr)
          {
          Pen penStroke(Color(((BYTE)strokeColour->opacity),
                              ((BYTE)strokeColour->red),
                              ((BYTE)strokeColour->green),
                              ((BYTE)strokeColour->blue)));

          Pen penMiddle(Color(((BYTE)middleColour->opacity),
                              ((BYTE)middleColour->red),
                              ((BYTE)middleColour->green),
                              ((BYTE)middleColour->blue)),2);

          Pen penDescent(Color(((BYTE)descentColour->opacity),
                               ((BYTE)descentColour->red),
                               ((BYTE)descentColour->green),
                               ((BYTE)descentColour->blue)),2);

          // Initialise the row/column indices
          rowIndex    = ((GRAPHICS_UINT)0);
          columnIndex = ((GRAPHICS_UINT)0);

          graphPoint  = strokeGraphPoints->graphPoints;
          
          // Find the x-axis point values
          xAxisIndex = xAxisPoints;

          for (rowIndex = ((GRAPHICS_UINT)0); rowIndex <= strokeGraphPoints->graphRowNumber; rowIndex++)
            {
            xAxisIndex->pointX = graphPoint->pointX;
            xAxisIndex->pointY = graphPoint->pointY;

            // Get the row start and end values
            if (rowIndex == ((GRAPHICS_UINT)0))
              {
              xStart = (GRAPHICS_FLOAT)xAxisIndex->pointX;
              }

            if (rowIndex == strokeGraphPoints->graphRowNumber)
              {
              xEnd = (GRAPHICS_FLOAT)xAxisIndex->pointX;
              }

            // Skip to the next line start
            if ((xAxisNewPoint = (strokeGraphLinePoint_tPtr)calloc(((size_t)1), sizeof(strokeGraphLinePoint_t))) != nullptr)
              {
              for (columnIndex = ((GRAPHICS_UINT)0); columnIndex <= strokeGraphPoints->graphColumnNumber; columnIndex++)
                {
                graphPoint = graphPoint->nextStrokeGraphLine;
                }

              xAxisIndex->nextStrokeGraphLine = xAxisNewPoint;
              xAxisIndex                      = xAxisNewPoint;
              }
            else
              {
              objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
              break;
              }
            }

          // Initialise the row/column indices
          rowIndex    = ((GRAPHICS_UINT)0);
          columnIndex = ((GRAPHICS_UINT)0);

          graphPoint  = strokeGraphPoints->graphPoints;

          // Find the y-axis point values
          xAxisIndex = xAxisPoints;
          yAxisIndex = yAxisPoints;

          for (columnIndex = ((GRAPHICS_UINT)0); columnIndex <= strokeGraphPoints->graphColumnNumber; columnIndex++)
            {
            if ((yAxisNewPoint = (strokeGraphLinePoint_tPtr)calloc(((size_t)1), sizeof(strokeGraphLinePoint_t))) != nullptr)
              {
              // Record the y-values at each point
              yAxisIndex->pointX = graphPoint->pointX;
              yAxisIndex->pointY = graphPoint->pointY;

              // Get the column start and end values
              if (columnIndex == ((GRAPHICS_UINT)0))
                {
                yStart = ((GRAPHICS_FLOAT)yAxisIndex->pointY);
                }

              if (columnIndex == strokeGraphPoints->graphColumnNumber)
                {
                yEnd = ((GRAPHICS_FLOAT)yAxisIndex->pointY);
                }

              yAxisIndex->nextStrokeGraphLine = yAxisNewPoint;
              yAxisIndex                      = yAxisNewPoint;

              graphPoint                      = graphPoint->nextStrokeGraphLine;
              }
            else
              {
              objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
              break;
              }
            }

          // Good to go - draw the matrix using the x- and y-axis points multiplied 
          // by the canvas width and canvas depth respectively
          // 
          // Initialise the row/column indices
          rowIndex    = ((GRAPHICS_UINT)0);
          columnIndex = ((GRAPHICS_UINT)0);

          xAxisIndex = xAxisPoints;

          for (rowIndex = ((GRAPHICS_UINT)0); rowIndex <= strokeGraphPoints->graphRowNumber; rowIndex++)
            {
            xDrawingPoint = (GRAPHICS_FLOAT)(xAxisIndex->pointX * canvasSize->width);

            // Mark the middle 'x' coordinate of the grid
            if (rowIndex == (strokeGraphPoints->graphRowNumber >> 1))
              {
              penMiddle.SetDashStyle(DashStyleDash);

              graphics.DrawLine((const Pen *)&penMiddle, xDrawingPoint,  (yStart * ((GRAPHICS_FLOAT)canvasSize->height)), xDrawingPoint, (yEnd * ((GRAPHICS_FLOAT)canvasSize->height)));

              penMiddle.SetDashStyle(DashStyleSolid);
              }
            else
              {
              graphics.DrawLine((const Pen *)&penStroke, xDrawingPoint,  (yStart * ((GRAPHICS_FLOAT)canvasSize->height)), xDrawingPoint, (yEnd * ((GRAPHICS_FLOAT)canvasSize->height)));
              }

            xAxisIndex = xAxisIndex->nextStrokeGraphLine;
            }

          yAxisIndex = yAxisPoints;

          for (columnIndex = ((GRAPHICS_UINT)0); columnIndex <= strokeGraphPoints->graphColumnNumber; columnIndex++)
            {
            yDrawingPoint = (GRAPHICS_FLOAT)(yAxisIndex->pointY * canvasSize->height);

            // Mark the middle 'y' coordinate of the grid
            if (columnIndex == (strokeGraphPoints->graphColumnNumber >> 1))
              {
              penMiddle.SetDashStyle(DashStyleDash);

              graphics.DrawLine((const Pen *)&penMiddle, (xStart * ((GRAPHICS_FLOAT)canvasSize->width)), yDrawingPoint, (xEnd * ((GRAPHICS_FLOAT)canvasSize->width)), yDrawingPoint);

              penMiddle.SetDashStyle(DashStyleSolid);
              }
            else
              {
              if (columnIndex == (GRAPHICS_UINT)((GRAPHICS_REAL)strokeGraphPoints->graphColumnNumber * strokeGraphPoints->descenderRatio))
                {
                penDescent.SetDashStyle(DashStyleDash);

                graphics.DrawLine((const Pen *)&penDescent,  (xStart * ((GRAPHICS_FLOAT)canvasSize->width)), yDrawingPoint, (xEnd * ((GRAPHICS_FLOAT)canvasSize->width)), yDrawingPoint);

                penDescent.SetDashStyle(DashStyleSolid);
                }
              else
                {
                graphics.DrawLine((const Pen *)&penStroke, (xStart * ((GRAPHICS_FLOAT)canvasSize->width)), yDrawingPoint, (xEnd * ((GRAPHICS_FLOAT)canvasSize->width)), yDrawingPoint);
                }
              }

            yAxisIndex = yAxisIndex->nextStrokeGraphLine;
            }
          }
        else
          {
          objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
          }
        }
      else
        {
        objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
        }
      }
    else
      {
      objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
      }

    // Scan the points array and determine the graph-enclosing x- and y-points
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of drawCharacterStrokeGraph                                        */

/******************************************************************************/
/* drawLineSegment() :                                                        */
/******************************************************************************/

graphicsError_t drawLineSegment(      HDC                       hdc,
                                const lineSegment_tPtr          newLineSegment,
                                const lineSegmentMode_t         newLineSegmentMode,
                                const strokeFrame_tPtr          characterFrame,
                                const canvasDescriptor_tPtr     canvasSize,
                                const strokeGraphPointBase_tPtr strokeGraphPoints)
  {
/******************************************************************************/

  Graphics        graphics(hdc);

  GRAPHICS_FLOAT  point0XCentre   = ((GRAPHICS_FLOAT)0.0),
                  point0YCentre   = ((GRAPHICS_FLOAT)0.0),
                  point1XCentre   = ((GRAPHICS_FLOAT)0.0),
                  point1YCentre   = ((GRAPHICS_FLOAT)0.0);

  GRAPHICS_REAL   xAxisPointDelta = ((GRAPHICS_REAL)0.0),
                  yAxisPointDelta = ((GRAPHICS_REAL)0.0);

  objectColour_t  lineColour      = { 0 };

  graphicsError_t objectError     = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((newLineSegment != nullptr) && (canvasSize != nullptr) && (strokeGraphPoints != nullptr))
    {
    // The grid has a normalised page-plotting range that is added to each point coordinate
    xAxisPointDelta = (characterFrame->frameCoordinates.frameRightX  - characterFrame->frameCoordinates.frameLeftX) / ((GRAPHICS_REAL)strokeGraphPoints->graphRowNumber);
    yAxisPointDelta = (characterFrame->frameCoordinates.frameBottomY - characterFrame->frameCoordinates.frameTopY)  / ((GRAPHICS_REAL)strokeGraphPoints->graphColumnNumber);

    // Find the line endpoints' centre coordinates
    point0XCentre = (GRAPHICS_FLOAT)(strokeGraphPoints->graphPoints->pointX + (((GRAPHICS_REAL)newLineSegment->lineSegmentOriginX) * xAxisPointDelta));
    point0YCentre = (GRAPHICS_FLOAT)(strokeGraphPoints->graphPoints->pointY + (((GRAPHICS_REAL)newLineSegment->lineSegmentOriginY) * yAxisPointDelta));

    point0XCentre = point0XCentre * ((GRAPHICS_FLOAT)(canvasSize->width));
    point0YCentre = point0YCentre * ((GRAPHICS_FLOAT)(canvasSize->height));

    point1XCentre = (GRAPHICS_FLOAT)(strokeGraphPoints->graphPoints->pointX + (((GRAPHICS_REAL)newLineSegment->lineSegmentDestinationX) * xAxisPointDelta));
    point1YCentre = (GRAPHICS_FLOAT)(strokeGraphPoints->graphPoints->pointY + (((GRAPHICS_REAL)newLineSegment->lineSegmentDestinationY) * yAxisPointDelta));

    point1XCentre = point1XCentre * ((GRAPHICS_FLOAT)(canvasSize->width));
    point1YCentre = point1YCentre * ((GRAPHICS_FLOAT)(canvasSize->height));

    switch(newLineSegmentMode)
      {
      case LINE_SEGMENT_MODE_PASSIVE : lineColour = strokeGraphPoints->passiveLineStrokeColour;
                                       break;
      case LINE_SEGMENT_MODE_ACTIVE  : lineColour = strokeGraphPoints->activeLineStrokeColour;
                                       break;
      case LINE_SEGMENT_MODE_EDIT    : lineColour = strokeGraphPoints->editLineStrokeColour;
                                       break;
      default                        : objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
                                       [[fallthrough]]; // warning C26819 fallthrough is explicit
      case LINE_SEGMENT_MODE_NONE    : 
                                       break;
      }

    if (objectError == GRAPHICS_NO_ERROR)
      {
      Pen pen(Color((BYTE)((lineColour).opacity),
                    (BYTE)((lineColour).red),
                    (BYTE)((lineColour).green),
                    (BYTE)((lineColour).blue)));

      pen.SetWidth((REAL)strokeGraphPoints->lineWidth);

      pen.SetStartCap(LineCapRound);
      pen.SetEndCap(LineCapRound);

      graphics.DrawLine((const Pen *)&pen, point0XCentre, point0YCentre, point1XCentre, point1YCentre);
      }
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of drawLineSegment                                                 */

/******************************************************************************/
/* detectStrokeGraphicCharacter() :                                           */
/* - check the range of printable alphanumeric characters for the stroke      */
/*   graphic character selection. Also for printability in the caption        */
/******************************************************************************/

graphicsError_t detectStrokeGraphicCharacter(GRAPHICS_UCHAR keyCode)
  {
/******************************************************************************/

  graphicsError_t objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if (!(((keyCode >= GRAPHICS_NUMERIC_KEY_CODE_0) && (keyCode <= GRAPHICS_NUMERIC_KEY_CODE_9)) ||
        ((keyCode >= GRAPHICS_ALPHA_KEY_CODE_A)   && (keyCode <= GRAPHICS_ALPHA_KEY_CODE_Z))   ||
        ((keyCode >= GRAPHICS_ALPHA_KEY_CODE_a)   && (keyCode <= GRAPHICS_ALPHA_KEY_CODE_z))))
    {
    objectError = GRAPHICS_OBJECT_RANGE_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of detectStrokeGraphicCharacter                                    */ 

/******************************************************************************/
/* decodeCharacterSelection() :                                               */
/******************************************************************************/

graphicsError_t decodeCharacterSelection(characterSelectionState_tPtr characterSelectionState)
  {
/******************************************************************************/

  graphicsError_t objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if (characterSelectionState != nullptr)
    {
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* decodeCharacterSelection                                               */

/******************************************************************************/
/* moveStrokeCursors() :                                                      */
/* - the stroke cursors can only navigate to- and from- the row/column        */
/*   coordinates of the stroke graph i.e. { X : 0..[n - 1] },                 */
/*                                        { Y : 0..[m - 1] }                  */
/* - in other words, all navigation movement is just the addition/subtraction */
/*   of integer '1'                                                           */
/*                                                                            */
/* - NOTE : the x- and y-axes versus indices are REVERSED (yes, the orignal   */
/*          relationships were back-to-front...my mistake)                    */
/*                                                                            */
/******************************************************************************/

graphicsError_t handleStrokeCursors(      strokeNavStates_tPtr         strokeNavState,
                                    const strokeDrawingState_tPtr      strokeDrawingState,
                                          characterSelectionState_tPtr selectedState,
                                          alphabetCharacters_tPtr      characterList,
                                    const strokeNavKeyStates_tPtr      strokeActiveCursor,
                                          strokeCursor_tPtr            strokeStartCursor,
                                          strokeCursor_tPtr            strokeEndCursor,
                                    const strokeGraphPointBase_tPtr    strokeGraphPoints,
                                          newLineSegment_tPtr          lineSegment)
  {
/******************************************************************************/

  graphicsError_t objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((strokeNavState    != nullptr) && (strokeActiveCursor != nullptr) &&
      (strokeStartCursor != nullptr) && (strokeEndCursor    != nullptr))
    {
    if (*strokeDrawingState == STROKE_DRAWING_STATE_REDRAW) // if the stroke cursor navigation is off, do no harm!
      {
      if (selectedState->modificationState == CHARACTER_SELECTION_STATE_MODIFY_ON)
        { // Segment walk and/or deletion
        if (*strokeNavState != STROKE_NAV_STATE_NONE)
          {
          // Point at the current character
          if ((objectError = fetchCharacterReference( characterSelectionState.selectedCharacter,
                                                      characterListRoot,
                                                     &characterReference)) == GRAPHICS_NO_ERROR)
            {
            if (characterReference->numberOfLineSegments > ((GRAPHICS_UINT)0))
              {
              switch(*strokeNavState)
                {
                case STROKE_NAV_STATE_CURSOR_LEFT    : if (characterReference->lineSegmentIndex > ((GRAPHICS_INT)0))
                                                         {
                                                         characterReference->lineSegmentIndex = characterReference->lineSegmentIndex - ((GRAPHICS_INT)1);
                                                         }
                                                       break;                                                  
                                                     
                case STROKE_NAV_STATE_CURSOR_RIGHT   : if (characterReference->lineSegmentIndex < (((GRAPHICS_INT)characterReference->numberOfLineSegments) - ((GRAPHICS_INT)1)))
                                                         {
                                                         characterReference->lineSegmentIndex = characterReference->lineSegmentIndex + ((GRAPHICS_INT)1);
                                                         }
                                                       break; 
              
                case STROKE_NAV_STATE_DELETE_SEGMENT : if ((objectError = fetchCharacterExtantSegment( characterReference->lineSegmentIndex,
                                                                                                       characterReference,
                                                                                                      &nextLineSegment)) == GRAPHICS_NO_ERROR)
                                                         {
                                                         objectError = deleteCharacterExtantSegment(characterReference,
                                                                                                    nextLineSegment);
                                                         }
                                                       break;
              
                default                              :
                                                       break;
                }
              }
            }
          }
        }
      else
        { // Segment creation
        if (*strokeNavState != STROKE_NAV_STATE_NONE)
          {
          if (*strokeActiveCursor & STROKE_NAV_KEY_STATE_LINE_START_CURSOR_SELECTED)
            {
            switch(*strokeNavState)
              {
              case STROKE_NAV_STATE_CURSOR_LEFT    : if (strokeStartCursor->strokeCursorIndexLit.xAxisIndex > STROKE_FRAME_X_MINIMUM)
                                                       {
                                                       strokeStartCursor->strokeCursorIndexLit.xAxisIndex = strokeStartCursor->strokeCursorIndexLit.xAxisIndex - ((GRAPHICS_UINT)1);
                                                       }
                                                     break;
  
              case STROKE_NAV_STATE_CURSOR_RIGHT   : if (strokeStartCursor->strokeCursorIndexLit.xAxisIndex < strokeGraphPoints->graphRowNumber)
                                                       {
                                                       strokeStartCursor->strokeCursorIndexLit.xAxisIndex = strokeStartCursor->strokeCursorIndexLit.xAxisIndex + ((GRAPHICS_UINT)1);
                                                       }
                                                     break;
  
              case STROKE_NAV_STATE_CURSOR_UP      : if (strokeStartCursor->strokeCursorIndexLit.yAxisIndex > STROKE_FRAME_Y_MINIMUM)
                                                       {
                                                       strokeStartCursor->strokeCursorIndexLit.yAxisIndex = strokeStartCursor->strokeCursorIndexLit.yAxisIndex - ((GRAPHICS_UINT)1);
                                                       }
                                                     break;
  
              case STROKE_NAV_STATE_CURSOR_DOWN    : if (strokeStartCursor->strokeCursorIndexLit.yAxisIndex < strokeGraphPoints->graphColumnNumber)
                                                       {
                                                       strokeStartCursor->strokeCursorIndexLit.yAxisIndex = strokeStartCursor->strokeCursorIndexLit.yAxisIndex + ((GRAPHICS_UINT)1);
                                                       }
                                                     break;
  
              case STROKE_NAV_STATE_SAVE_SEGMENT   : if ((objectError = addLineSegmentToCharacterList(selectedState->selectedCharacter,
                                                                                                      (lineSegment_tPtr)lineSegment,
                                                                                                      characterList)) == GRAPHICS_NO_ERROR)
                                                       {                                                      
                                                        strokeStartCursor->strokeCursorIndexDefault.xAxisIndex = lineSegment->lineSegmentDestinationX;
                                                        strokeStartCursor->strokeCursorIndexDefault.yAxisIndex = lineSegment->lineSegmentDestinationY;
                                                        strokeStartCursor->strokeCursorIndexLit.xAxisIndex     = lineSegment->lineSegmentDestinationX;
                                                        strokeStartCursor->strokeCursorIndexLit.yAxisIndex     = lineSegment->lineSegmentDestinationY;
                                                                                                                                        
                                                        strokeEndCursor->strokeCursorIndexDefault.xAxisIndex   = lineSegment->lineSegmentDestinationX;
                                                        strokeEndCursor->strokeCursorIndexDefault.yAxisIndex   = lineSegment->lineSegmentDestinationY;
                                                        strokeEndCursor->strokeCursorIndexLit.xAxisIndex       = lineSegment->lineSegmentDestinationX;
                                                        strokeEndCursor->strokeCursorIndexLit.yAxisIndex       = lineSegment->lineSegmentDestinationY;

                                                       *strokeActiveCursor                                     = STROKE_NAV_KEY_STATE_LINE_START_CURSOR_SELECTED;
                                                       }
                                                     else
                                                       {
                                                       objectError = GRAPHICS_OBJECT_FUNCTION_ERROR;
                                                       }
                                                     break;
  
              default                              : objectError = GRAPHICS_OBJECT_TYPE_ERROR;
                                                     break;
              }
            }
          else
            {
            if (*strokeActiveCursor & STROKE_NAV_KEY_STATE_LINE_END_CURSOR_SELECTED)
              {
              switch(*strokeNavState)
                {
                case STROKE_NAV_STATE_CURSOR_LEFT     : if (strokeEndCursor->strokeCursorIndexLit.xAxisIndex > STROKE_FRAME_X_MINIMUM)
                                                          {
                                                          strokeEndCursor->strokeCursorIndexLit.xAxisIndex = strokeEndCursor->strokeCursorIndexLit.xAxisIndex - ((GRAPHICS_UINT)1);
                                                          }
                                                        break;
                case STROKE_NAV_STATE_CURSOR_RIGHT    : if (strokeEndCursor->strokeCursorIndexLit.xAxisIndex < strokeGraphPoints->graphRowNumber)
                                                          {
                                                          strokeEndCursor->strokeCursorIndexLit.xAxisIndex = strokeEndCursor->strokeCursorIndexLit.xAxisIndex + ((GRAPHICS_UINT)1);
                                                          }
                                                        break;
                case STROKE_NAV_STATE_CURSOR_UP       : if (strokeEndCursor->strokeCursorIndexLit.yAxisIndex > STROKE_FRAME_Y_MINIMUM)
                                                          {
                                                          strokeEndCursor->strokeCursorIndexLit.yAxisIndex = strokeEndCursor->strokeCursorIndexLit.yAxisIndex - ((GRAPHICS_UINT)1);
                                                          }
                                                        break;
                case STROKE_NAV_STATE_CURSOR_DOWN     : if (strokeEndCursor->strokeCursorIndexLit.yAxisIndex < strokeGraphPoints->graphColumnNumber)
                                                          {
                                                          strokeEndCursor->strokeCursorIndexLit.yAxisIndex = strokeEndCursor->strokeCursorIndexLit.yAxisIndex + ((GRAPHICS_UINT)1);
                                                          }
                                                        break;
                case STROKE_NAV_STATE_SAVE_SEGMENT    : if ((objectError = addLineSegmentToCharacterList(selectedState->selectedCharacter,
                                                                                                         (lineSegment_tPtr)lineSegment,
                                                                                                         characterList)) == GRAPHICS_NO_ERROR)
                                                         {                                                                                                                 
                                                          strokeStartCursor->strokeCursorIndexDefault.xAxisIndex = lineSegment->lineSegmentOriginX;
                                                          strokeStartCursor->strokeCursorIndexDefault.yAxisIndex = lineSegment->lineSegmentOriginY;
                                                          strokeStartCursor->strokeCursorIndexLit.xAxisIndex     = lineSegment->lineSegmentOriginX;
                                                          strokeStartCursor->strokeCursorIndexLit.yAxisIndex     = lineSegment->lineSegmentOriginY;
                                                                                                                 
                                                          strokeEndCursor->strokeCursorIndexDefault.xAxisIndex   = lineSegment->lineSegmentOriginX;
                                                          strokeEndCursor->strokeCursorIndexDefault.yAxisIndex   = lineSegment->lineSegmentOriginY;
                                                          strokeEndCursor->strokeCursorIndexLit.xAxisIndex       = lineSegment->lineSegmentOriginX;
                                                          strokeEndCursor->strokeCursorIndexLit.yAxisIndex       = lineSegment->lineSegmentOriginY;

                                                         *strokeActiveCursor                                     = STROKE_NAV_KEY_STATE_LINE_START_CURSOR_SELECTED;
                                                         }
                                                       else
                                                         {
                                                         objectError = GRAPHICS_OBJECT_FUNCTION_ERROR;
                                                         }
                                                       break;
  
                default                               : objectError = GRAPHICS_OBJECT_TYPE_ERROR;
                                                        break;
                }
              }
            else
              {
              if (!((*strokeActiveCursor | STROKE_NAV_KEY_STATE_LINE_START_CURSOR_DESELECTED) || 
                    (*strokeActiveCursor | STROKE_NAV_KEY_STATE_LINE_END_CURSOR_DESELECTED)))
                {
                objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
                }
              }
            }
          }
        }
      }
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of moveStrokeCursors                                               */

/******************************************************************************/
/* drawStrokeCursors() :                                                      */
/******************************************************************************/

graphicsError_t drawStrokeCursors(      HDC                       hdc,
                                        strokeNavStates_tPtr      strokeNavState,
                                  const strokeFrame_tPtr          characterFrame,
                                  const strokeDrawingState_tPtr   strokeDrawingState,
                                  const strokeNavKeyStates_tPtr   strokeActiveCursor,
                                        strokeCursor_tPtr         strokeStartCursor,
                                        strokeCursor_tPtr         strokeEndCursor,
                                  const canvasDescriptor_tPtr     canvasSize,
                                  const strokeGraphPointBase_tPtr strokeGraphPoints,
                                        newLineSegment_tPtr       lineSegment)
  {
/******************************************************************************/

  Graphics        graphics(hdc);

  GRAPHICS_FLOAT  xCentre           = ((GRAPHICS_FLOAT)0.0),
                  yCentre           = ((GRAPHICS_FLOAT)0.0),
                  xCentreUnselected = ((GRAPHICS_FLOAT)0.0),
                  yCentreUnselected = ((GRAPHICS_FLOAT)0.0),

                  xLeft             = ((GRAPHICS_FLOAT)0.0),
                  yTop              = ((GRAPHICS_FLOAT)0.0),
                  xRight            = ((GRAPHICS_FLOAT)0.0),
                  yBottom           = ((GRAPHICS_FLOAT)0.0);

  GRAPHICS_REAL   xAxisPointDelta   = ((GRAPHICS_REAL)0.0),
                  yAxisPointDelta   = ((GRAPHICS_REAL)0.0);

  GRAPHICS_BYTE   requestedByte     = ((GRAPHICS_BYTE)0);
                                    
  graphicsError_t objectError       = GRAPHICS_NO_ERROR;
                                    
  singlePoint_t  lastStrokeStart    = { ((GRAPHICS_REAL)0.0), ((GRAPHICS_REAL)0.0) };

/******************************************************************************/

  if ((strokeDrawingState != nullptr) && (strokeActiveCursor != nullptr) && 
      (strokeStartCursor  != nullptr) && (strokeEndCursor    != nullptr) && 
      (canvasSize         != nullptr) && (strokeGraphPoints  != nullptr))
    {
    if (*strokeDrawingState == STROKE_DRAWING_STATE_REDRAW) // if the stroke cursor navigation is off, do no harm!
      {
      // Check the surrounding detection limits makes sense
      if (strokeStartCursor->strokeCursorDetectionExtent < STROKE_CURSOR_MINIMUM_CAPTURE_EXTENT)
        {
        strokeStartCursor->strokeCursorDetectionExtent = STROKE_CURSOR_MINIMUM_CAPTURE_EXTENT;
        }
      else
        {
        if (strokeStartCursor->strokeCursorDetectionExtent > STROKE_CURSOR_MAXIMUM_CAPTURE_EXTENT)
          {
          strokeStartCursor->strokeCursorDetectionExtent  = STROKE_CURSOR_MAXIMUM_CAPTURE_EXTENT;
          }
        }

      if (strokeEndCursor->strokeCursorDetectionExtent < STROKE_CURSOR_MINIMUM_CAPTURE_EXTENT)
        {
        strokeEndCursor->strokeCursorDetectionExtent = STROKE_CURSOR_MINIMUM_CAPTURE_EXTENT;
        }
      else
        {
        if (strokeEndCursor->strokeCursorDetectionExtent > STROKE_CURSOR_MAXIMUM_CAPTURE_EXTENT)
          {
          strokeEndCursor->strokeCursorDetectionExtent  = STROKE_CURSOR_MAXIMUM_CAPTURE_EXTENT;
          }
        }

      if (*strokeActiveCursor & STROKE_NAV_KEY_STATE_LINE_START_CURSOR_SELECTED)
        {
        // Detect movement of the stroke start cursor - requires the calling function to call at least once 
        // after the movement has completed
        if ((strokeStartCursor->strokeCursorIndexDefault.xAxisIndex != strokeStartCursor->strokeCursorIndexLit.xAxisIndex) ||
            (strokeStartCursor->strokeCursorIndexDefault.yAxisIndex != strokeStartCursor->strokeCursorIndexLit.yAxisIndex))
          {       
          EXTRACT_COLOUR_FIELD_BYTE(strokeStartCursor->strokeCursorColourLitCode,SELECT_BYTE_0,&requestedByte)
          strokeStartCursor->strokeCursorColourLit.opacity = requestedByte;
          EXTRACT_COLOUR_FIELD_BYTE(strokeStartCursor->strokeCursorColourLitCode,SELECT_BYTE_1,&requestedByte)
          strokeStartCursor->strokeCursorColourLit.red     = requestedByte;
          EXTRACT_COLOUR_FIELD_BYTE(strokeStartCursor->strokeCursorColourLitCode,SELECT_BYTE_2,&requestedByte)
          strokeStartCursor->strokeCursorColourLit.green   = requestedByte;
          EXTRACT_COLOUR_FIELD_BYTE(strokeStartCursor->strokeCursorColourLitCode,SELECT_BYTE_3,&requestedByte)
          strokeStartCursor->strokeCursorColourLit.blue    = requestedByte;
          }
        else
          {
          EXTRACT_COLOUR_FIELD_BYTE(STROKE_START_CURSOR_DEFAULT_CODE,SELECT_BYTE_0,&requestedByte)
          strokeStartCursor->strokeCursorColourDefault.opacity = requestedByte;
          EXTRACT_COLOUR_FIELD_BYTE(STROKE_START_CURSOR_DEFAULT_CODE,SELECT_BYTE_1,&requestedByte)
          strokeStartCursor->strokeCursorColourDefault.red     = requestedByte;
          EXTRACT_COLOUR_FIELD_BYTE(STROKE_START_CURSOR_DEFAULT_CODE,SELECT_BYTE_2,&requestedByte)
          strokeStartCursor->strokeCursorColourDefault.green   = requestedByte;
          EXTRACT_COLOUR_FIELD_BYTE(STROKE_START_CURSOR_DEFAULT_CODE,SELECT_BYTE_3,&requestedByte)
          strokeStartCursor->strokeCursorColourDefault.blue    = requestedByte;
          }
        }
      else
        {
        if (*strokeActiveCursor & STROKE_NAV_KEY_STATE_LINE_END_CURSOR_SELECTED)
          {            
          if ((strokeEndCursor->strokeCursorIndexDefault.xAxisIndex != strokeEndCursor->strokeCursorIndexLit.xAxisIndex) ||
              (strokeEndCursor->strokeCursorIndexDefault.yAxisIndex != strokeEndCursor->strokeCursorIndexLit.yAxisIndex))
            {          
            EXTRACT_COLOUR_FIELD_BYTE(STROKE_END_CURSOR_LIT_CODE,SELECT_BYTE_0,&requestedByte)
            strokeEndCursor->strokeCursorColourLit.opacity = requestedByte;
            EXTRACT_COLOUR_FIELD_BYTE(STROKE_END_CURSOR_LIT_CODE,SELECT_BYTE_1,&requestedByte)
            strokeEndCursor->strokeCursorColourLit.red     = requestedByte;
            EXTRACT_COLOUR_FIELD_BYTE(STROKE_END_CURSOR_LIT_CODE,SELECT_BYTE_2,&requestedByte)
            strokeEndCursor->strokeCursorColourLit.green   = requestedByte;
            EXTRACT_COLOUR_FIELD_BYTE(STROKE_END_CURSOR_LIT_CODE,SELECT_BYTE_3,&requestedByte)
            strokeEndCursor->strokeCursorColourLit.blue    = requestedByte;
            }
          else
            {
            EXTRACT_COLOUR_FIELD_BYTE(STROKE_END_CURSOR_DEFAULT_CODE,SELECT_BYTE_0,&requestedByte)
            strokeEndCursor->strokeCursorColourDefault.opacity = requestedByte;
            EXTRACT_COLOUR_FIELD_BYTE(STROKE_END_CURSOR_DEFAULT_CODE,SELECT_BYTE_1,&requestedByte)
            strokeEndCursor->strokeCursorColourDefault.red     = requestedByte;
            EXTRACT_COLOUR_FIELD_BYTE(STROKE_END_CURSOR_DEFAULT_CODE,SELECT_BYTE_2,&requestedByte)
            strokeEndCursor->strokeCursorColourDefault.green   = requestedByte;
            EXTRACT_COLOUR_FIELD_BYTE(STROKE_END_CURSOR_DEFAULT_CODE,SELECT_BYTE_3,&requestedByte)
            strokeEndCursor->strokeCursorColourDefault.blue    = requestedByte;
            }
          }
        else
          {
          objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
          }
        }

      if (*strokeActiveCursor & STROKE_NAV_KEY_STATE_LINE_BOTH_START_LEADING)
        { // Force the end cursor to follow the start cursor
        strokeEndCursor->strokeCursorIndexDefault.xAxisIndex = strokeStartCursor->strokeCursorIndexDefault.xAxisIndex;
        strokeEndCursor->strokeCursorIndexLit.xAxisIndex     = strokeStartCursor->strokeCursorIndexLit.xAxisIndex;
        strokeEndCursor->strokeCursorIndexDefault.yAxisIndex = strokeStartCursor->strokeCursorIndexDefault.yAxisIndex; 
        strokeEndCursor->strokeCursorIndexLit.yAxisIndex     = strokeStartCursor->strokeCursorIndexLit.yAxisIndex;
        }
      else
        {
        if (*strokeActiveCursor & STROKE_NAV_KEY_STATE_LINE_BOTH_END_LEADING)
          { // Force the start cursor to follow the end cursor
          strokeStartCursor->strokeCursorIndexDefault.xAxisIndex = strokeEndCursor->strokeCursorIndexDefault.xAxisIndex;
          strokeStartCursor->strokeCursorIndexLit.xAxisIndex     = strokeEndCursor->strokeCursorIndexLit.xAxisIndex;
          strokeStartCursor->strokeCursorIndexDefault.yAxisIndex = strokeEndCursor->strokeCursorIndexDefault.yAxisIndex; 
          strokeStartCursor->strokeCursorIndexLit.yAxisIndex     = strokeEndCursor->strokeCursorIndexLit.yAxisIndex;
          }
        }

      // The grid has a normalised page-plotting range that is added to each point coordinate
      xAxisPointDelta = (characterFrame->frameCoordinates.frameRightX  - characterFrame->frameCoordinates.frameLeftX) / ((GRAPHICS_REAL)strokeGraphPoints->graphRowNumber);
      yAxisPointDelta = (characterFrame->frameCoordinates.frameBottomY - characterFrame->frameCoordinates.frameTopY)  / ((GRAPHICS_REAL)strokeGraphPoints->graphColumnNumber);

      // Draw the active cursor
      *strokeDrawingState = STROKE_DRAWING_STATE_NONE;

      if (*strokeActiveCursor & STROKE_NAV_KEY_STATE_LINE_START_CURSOR_SELECTED)
        {
        // Draw the selected cursor first
        xCentre = (GRAPHICS_FLOAT)(strokeGraphPoints->graphPoints->pointX + (((GRAPHICS_REAL)strokeEndCursor->strokeCursorIndexLit.xAxisIndex) * xAxisPointDelta));
        yCentre = (GRAPHICS_FLOAT)(strokeGraphPoints->graphPoints->pointY + (((GRAPHICS_REAL)strokeEndCursor->strokeCursorIndexLit.yAxisIndex) * yAxisPointDelta));

        xCentre = xCentre * ((GRAPHICS_FLOAT)(canvasSize->width));
        yCentre = yCentre * ((GRAPHICS_FLOAT)(canvasSize->height));

        // Save the unselected cursor centre as one end of a line-segment
        xCentreUnselected = xCentre;
        yCentreUnselected = yCentre;

        // Now add the extent to find the rectangle 'corners'...
        xLeft   = xCentre - ((GRAPHICS_FLOAT)(strokeEndCursor->strokeCursorDetectionExtent));
        yTop    = yCentre - ((GRAPHICS_FLOAT)(strokeEndCursor->strokeCursorDetectionExtent));

        xRight  = xCentre + ((GRAPHICS_FLOAT)(strokeEndCursor->strokeCursorDetectionExtent));
        yBottom = yCentre + ((GRAPHICS_FLOAT)(strokeEndCursor->strokeCursorDetectionExtent));

        Pen pen(Color( (BYTE)((strokeEndCursor->strokeCursorColourDefault).opacity),
                       (BYTE)((strokeEndCursor->strokeCursorColourDefault).red),
                       (BYTE)((strokeEndCursor->strokeCursorColourDefault).green),
                       (BYTE)((strokeEndCursor->strokeCursorColourDefault).blue)),
                       (REAL)strokeEndCursor->lineWidth);

        pen.SetWidth((REAL)strokeGraphPoints->lineWidth);

        graphics.DrawLine((const Pen *)&pen, xLeft,  yTop,    xRight, yTop);
        graphics.DrawLine((const Pen *)&pen, xLeft,  yBottom, xRight, yBottom);
        graphics.DrawLine((const Pen *)&pen, xLeft,  yTop,    xLeft,  yBottom);
        graphics.DrawLine((const Pen *)&pen, xRight, yTop,    xRight, yBottom);

        // Then draw the unselected cursor
        xCentre = (GRAPHICS_FLOAT)(strokeGraphPoints->graphPoints->pointX + (((GRAPHICS_REAL)strokeStartCursor->strokeCursorIndexLit.xAxisIndex) * xAxisPointDelta));
        yCentre = (GRAPHICS_FLOAT)(strokeGraphPoints->graphPoints->pointY + (((GRAPHICS_REAL)strokeStartCursor->strokeCursorIndexLit.yAxisIndex) * yAxisPointDelta));

        xCentre = xCentre * ((GRAPHICS_FLOAT)(canvasSize->width));
        yCentre = yCentre * ((GRAPHICS_FLOAT)(canvasSize->height));

        // Now add the extent to find the rectangle 'corners'...
        xLeft   = xCentre - ((GRAPHICS_FLOAT)(strokeStartCursor->strokeCursorDetectionExtent));
        yTop    = yCentre - ((GRAPHICS_FLOAT)(strokeStartCursor->strokeCursorDetectionExtent));

        xRight  = xCentre + ((GRAPHICS_FLOAT)(strokeStartCursor->strokeCursorDetectionExtent));
        yBottom = yCentre + ((GRAPHICS_FLOAT)(strokeStartCursor->strokeCursorDetectionExtent));

        Pen pen2(Color( (BYTE)((strokeStartCursor->strokeCursorColourDefault).opacity),
                        (BYTE)((strokeStartCursor->strokeCursorColourDefault).red),
                        (BYTE)((strokeStartCursor->strokeCursorColourDefault).green),
                        (BYTE)((strokeStartCursor->strokeCursorColourDefault).blue)),
                        (REAL)strokeStartCursor->lineWidth);

        graphics.DrawLine((const Pen *)&pen2, xLeft,  yTop,    xRight, yTop);
        graphics.DrawLine((const Pen *)&pen2, xLeft,  yBottom, xRight, yBottom);
        graphics.DrawLine((const Pen *)&pen2, xLeft,  yTop,    xLeft,  yBottom);
        graphics.DrawLine((const Pen *)&pen2, xRight, yTop,    xRight, yBottom);

        // Draw the line-segment
        Pen pen3(Color( (BYTE)((strokeGraphPoints->activeLineStrokeColour).opacity),
                        (BYTE)((strokeGraphPoints->activeLineStrokeColour).red),
                        (BYTE)((strokeGraphPoints->activeLineStrokeColour).green),
                        (BYTE)((strokeGraphPoints->activeLineStrokeColour).blue)),
                        (REAL)strokeGraphPoints->lineWidth);

        graphics.DrawLine((const Pen *)&pen3, xCentre, yCentre, xCentreUnselected, yCentreUnselected);

        // Export the line-segment end-points
        lineSegment->lineSegmentOriginX      = strokeEndCursor->strokeCursorIndexLit.xAxisIndex;
        lineSegment->lineSegmentOriginY      = strokeEndCursor->strokeCursorIndexLit.yAxisIndex;
        lineSegment->lineSegmentDestinationX = strokeStartCursor->strokeCursorIndexLit.xAxisIndex;
        lineSegment->lineSegmentDestinationY = strokeStartCursor->strokeCursorIndexLit.yAxisIndex;

        if ((xCentre == xCentreUnselected) && (yCentre == yCentreUnselected))
          {
          lineSegment->lineSegmentChanged = false;
          }
        else
          {
          lineSegment->lineSegmentChanged = true;
          }
        }
      else
        {
        if (*strokeActiveCursor & STROKE_NAV_KEY_STATE_LINE_END_CURSOR_SELECTED)
          {
          // Draw the unselected cursor outline first - find the cursor centre
          xCentre = (GRAPHICS_FLOAT)(strokeGraphPoints->graphPoints->pointX + (((GRAPHICS_REAL)strokeStartCursor->strokeCursorIndexLit.xAxisIndex) * xAxisPointDelta));
          yCentre = (GRAPHICS_FLOAT)(strokeGraphPoints->graphPoints->pointY + (((GRAPHICS_REAL)strokeStartCursor->strokeCursorIndexLit.yAxisIndex) * yAxisPointDelta));

          xCentre = xCentre * ((GRAPHICS_FLOAT)(canvasSize->width));
          yCentre = yCentre * ((GRAPHICS_FLOAT)(canvasSize->height));

          // Save the unselected cursor centre as one end of a line-segment
          xCentreUnselected = xCentre;
          yCentreUnselected = yCentre;

          // Now add the extent to find the rectangle 'corners'...
          xLeft   = xCentre - ((GRAPHICS_FLOAT)(strokeStartCursor->strokeCursorDetectionExtent));
          yTop    = yCentre - ((GRAPHICS_FLOAT)(strokeStartCursor->strokeCursorDetectionExtent));

          xRight  = xCentre + ((GRAPHICS_FLOAT)(strokeStartCursor->strokeCursorDetectionExtent));
          yBottom = yCentre + ((GRAPHICS_FLOAT)(strokeStartCursor->strokeCursorDetectionExtent));

          Pen pen(Color( (BYTE)((strokeStartCursor->strokeCursorColourDefault).opacity),
                         (BYTE)((strokeStartCursor->strokeCursorColourDefault).red),
                         (BYTE)((strokeStartCursor->strokeCursorColourDefault).green),
                         (BYTE)((strokeStartCursor->strokeCursorColourDefault).blue)),
                         (REAL)strokeStartCursor->lineWidth);

          graphics.DrawLine((const Pen *)&pen, xLeft,  yTop,    xRight, yTop);
          graphics.DrawLine((const Pen *)&pen, xLeft,  yBottom, xRight, yBottom);
          graphics.DrawLine((const Pen *)&pen, xLeft,  yTop,    xLeft,  yBottom);
          graphics.DrawLine((const Pen *)&pen, xRight, yTop,    xRight, yBottom);
          
          // Then draw the selected cursor
          xCentre = (GRAPHICS_FLOAT)(strokeGraphPoints->graphPoints->pointX + (((GRAPHICS_REAL)strokeEndCursor->strokeCursorIndexLit.xAxisIndex) * xAxisPointDelta));
          yCentre = (GRAPHICS_FLOAT)(strokeGraphPoints->graphPoints->pointY + (((GRAPHICS_REAL)strokeEndCursor->strokeCursorIndexLit.yAxisIndex) * yAxisPointDelta));

          xCentre = xCentre * ((GRAPHICS_FLOAT)(canvasSize->width));
          yCentre = yCentre * ((GRAPHICS_FLOAT)(canvasSize->height));
          
          // Now add the extent to find the rectangle 'corners'...
          xLeft   = xCentre - ((GRAPHICS_FLOAT)(strokeEndCursor->strokeCursorDetectionExtent));
          yTop    = yCentre - ((GRAPHICS_FLOAT)(strokeEndCursor->strokeCursorDetectionExtent));
          
          xRight  = xCentre + ((GRAPHICS_FLOAT)(strokeEndCursor->strokeCursorDetectionExtent));
          yBottom = yCentre + ((GRAPHICS_FLOAT)(strokeEndCursor->strokeCursorDetectionExtent));
          
          Pen pen2(Color( (BYTE)((strokeEndCursor->strokeCursorColourDefault).opacity),
                          (BYTE)((strokeEndCursor->strokeCursorColourDefault).red),
                          (BYTE)((strokeEndCursor->strokeCursorColourDefault).green),
                          (BYTE)((strokeEndCursor->strokeCursorColourDefault).blue)),
                          (REAL)strokeStartCursor->lineWidth);
          
          graphics.DrawLine((const Pen *)&pen2, xLeft,  yTop,    xRight, yTop);
          graphics.DrawLine((const Pen *)&pen2, xLeft,  yBottom, xRight, yBottom);
          graphics.DrawLine((const Pen *)&pen2, xLeft,  yTop,    xLeft,  yBottom);
          graphics.DrawLine((const Pen *)&pen2, xRight, yTop,    xRight, yBottom);

          // Draw the line-segment
          Pen pen3(Color( (BYTE)((strokeGraphPoints->activeLineStrokeColour).opacity),
                          (BYTE)((strokeGraphPoints->activeLineStrokeColour).red),
                          (BYTE)((strokeGraphPoints->activeLineStrokeColour).green),
                          (BYTE)((strokeGraphPoints->activeLineStrokeColour).blue)),
                          (REAL)strokeGraphPoints->lineWidth);

          graphics.DrawLine((const Pen *)&pen3, xCentre, yCentre, xCentreUnselected, yCentreUnselected);

          // Export the line-segment end-points
          lineSegment->lineSegmentOriginX      = strokeStartCursor->strokeCursorIndexLit.xAxisIndex;
          lineSegment->lineSegmentOriginY      = strokeStartCursor->strokeCursorIndexLit.yAxisIndex;
          lineSegment->lineSegmentDestinationX = strokeEndCursor->strokeCursorIndexLit.xAxisIndex;
          lineSegment->lineSegmentDestinationY = strokeEndCursor->strokeCursorIndexLit.yAxisIndex;


          if ((xCentre == xCentreUnselected) && (yCentre == yCentreUnselected))
            {
            lineSegment->lineSegmentChanged = false;
            }
          else
            {
            lineSegment->lineSegmentChanged = true;
            }
          }
        }
      }
    else
      { // Only one cursor can be 'selected then 'deselected' so test them both anyway
      if ((*strokeActiveCursor & STROKE_NAV_KEY_STATE_LINE_START_CURSOR_DESELECTED) || (*strokeActiveCursor & STROKE_NAV_KEY_STATE_LINE_END_CURSOR_DESELECTED))
        {
         // Cancel the requested movement
         lineSegment->lineSegmentOriginX      = ((GRAPHICS_UINT)0);
         lineSegment->lineSegmentOriginY      = ((GRAPHICS_UINT)0);
         lineSegment->lineSegmentDestinationX = ((GRAPHICS_UINT)0);
         lineSegment->lineSegmentDestinationY = ((GRAPHICS_UINT)0);
         lineSegment->lineSegmentChanged      = false;

        *strokeActiveCursor                   = STROKE_NAV_KEY_STATE_NONE;
        }
      }

    strokeStartCursor->strokeCursorIndexDefault.xAxisIndex = strokeStartCursor->strokeCursorIndexLit.xAxisIndex;
    strokeStartCursor->strokeCursorIndexDefault.yAxisIndex = strokeStartCursor->strokeCursorIndexLit.yAxisIndex;

    strokeEndCursor->strokeCursorIndexDefault.xAxisIndex   = strokeEndCursor->strokeCursorIndexLit.xAxisIndex;
    strokeEndCursor->strokeCursorIndexDefault.yAxisIndex   = strokeEndCursor->strokeCursorIndexLit.yAxisIndex;
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of drawStrokeCursors                                               */

/******************************************************************************/
/* buildStrokeFrame() :                                                       */
/* - constructs the x,y matrix of normalised points for the columns and rows  */
/*   of the character graph                                                   */
/******************************************************************************/

graphicsError_t buildStrokeFrame(strokeFrame_tPtr strokeFrame)
  {
/******************************************************************************/

  strokeCell_tPtr       strokeCell     = nullptr;
  strokeCellPoint_tPtr  strokePoint    = nullptr;

  GRAPHICS_UINT         cellNumber     = ((GRAPHICS_UINT)0),
                        newlineXNumber = ((GRAPHICS_UINT)0),
                        oldlineXNumber = ((GRAPHICS_UINT)0);

  GRAPHICS_UINT         xLines         = ((GRAPHICS_UINT)0);
  GRAPHICS_UINT         yLines         = ((GRAPHICS_UINT)0);

  graphicsError_t       objectError    = GRAPHICS_NO_ERROR;
                                      
  FILE                 *alphabet       = nullptr;

  errno_t               alphabetError  = ((errno_t)0);

/******************************************************************************/
  
  if (strokeFrame != nullptr)
    {
    if ((alphabetError = fopen_s(&alphabet, "c:\\temp\\alphabetsoup.txt", "wb")) == ((errno_t)0))
      {
      xLines = (GRAPHICS_UINT)ceil(strokeFrame->strokeXAxisPoints);
      yLines = (GRAPHICS_UINT)ceil(strokeFrame->strokeYAxisPoints);

      strokeCell = strokeFrame->strokeCellArray;
      
      while (strokeCell != nullptr)
        {
        fprintf(alphabet, "\n Cell Number = %03d\n", cellNumber);

        if (strokeCell->strokeCellPoint != nullptr)
          {
          strokePoint    = strokeCell->strokeCellPoint;
          oldlineXNumber = (GRAPHICS_UINT)(strokePoint->xAxisPoint * strokeFrame->strokeXAxisPoints);
          
          fprintf(alphabet, "\n Line Number = %03d\n", oldlineXNumber);
          
          while (strokePoint != nullptr)
            {
            newlineXNumber = (GRAPHICS_UINT)(strokePoint->xAxisPoint * strokeFrame->strokeXAxisPoints);
          
            if (newlineXNumber != oldlineXNumber)
              {
              fprintf(alphabet, "\n Line Number = %03d\n", newlineXNumber);
          
              oldlineXNumber = newlineXNumber;
              }
          
            fprintf(alphabet, "( X = %lf Y = %lf }", strokePoint->xAxisPoint, strokePoint->yAxisPoint);

            switch(strokePoint->pointType)
              {
              case STROKE_CELL_POINT_TYPE_LINE_START       : fprintf(alphabet, "LINE_START");
                                                             break;
              case STROKE_CELL_POINT_TYPE_LINE_END         : fprintf(alphabet, "LINE_END");
                                                             break;
              case STROKE_CELL_POINT_TYPE_LINE_MID         : fprintf(alphabet, "LINE_MID");
                                                             break;
              case STROKE_CELL_POINT_TYPE_FIRST_LINE_START : fprintf(alphabet, "FIRST_LINE_START");
                                                             break;
              case STROKE_CELL_POINT_TYPE_LAST_LINE_START  : fprintf(alphabet, "LAST_LINE_START");
                                                             break;
              case STROKE_CELL_POINT_TYPE_NONE             : fprintf(alphabet, "UNKNOWN TOKEN");
                                                             [[fallthrough]]; // warning C26819 fallthrough is explicit
              default                                      : break;
              }
          
            strokePoint = strokePoint->nextStrokeCellPoint;
            }
          }
        else
          {
          }
      
        fprintf(alphabet, "\n");

        cellNumber = cellNumber + ((GRAPHICS_UINT)1);
        strokeCell = strokeCell->nextStrokeCell;
        }
      }
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

  if (alphabet != 0)
    {
    fclose(alphabet);
    }

  exit(0);

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of buildStrokeFrame                                                */

/******************************************************************************/
/* computeCharacterExtents() :                                                */
/* - scans the characters' line segments and determines the maximum { x, y }  */
/*   coordinates at each "corner" of the character                            */
/******************************************************************************/

graphicsError_t computeCharacterExtents(const GRAPHICS_UINT             characterSelected,
                                        const alphabetCharacters_tPtr   alphabetCharacters,
                                        const strokeGraphPointBase_tPtr strokeGraphicBase)
  {
/******************************************************************************/

  alphabetCharacters_tPtr characterReference = nullptr;

  lineSegment_tPtr        nextSegment        = nullptr;

  graphicsError_t         objectError        = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((alphabetCharacters != nullptr) && (strokeGraphicBase != nullptr) && (characterSelected < ALPHABET_SIZE))
    {
    if ((objectError = fetchCharacterReference( characterSelected,
                                                alphabetCharacters,
                                               &characterReference)) == GRAPHICS_NO_ERROR)
      {
      characterReference->characterExtents.topLeft.pointX     = ((GRAPHICS_INT)0);
      characterReference->characterExtents.topLeft.pointY     = ((GRAPHICS_INT)0);
      characterReference->characterExtents.bottomRight.pointX = ((GRAPHICS_INT)0);
      characterReference->characterExtents.bottomRight.pointY = ((GRAPHICS_INT)0);

      nextSegment = characterReference->characterLineSegments;

      if (nextSegment->nextLineSegment == nullptr)     
        {
        // May or may not be an error per se - could be the character hasn't been built yet
        objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
        }
      else
        {
        characterReference->characterExtents.topLeft.pointX     = ((GRAPHICS_INT)strokeGraphicBase->graphRowNumber);
        characterReference->characterExtents.topLeft.pointY     = ((GRAPHICS_INT)strokeGraphicBase->graphColumnNumber);
        characterReference->characterExtents.bottomRight.pointX = ((GRAPHICS_INT)0);
        characterReference->characterExtents.bottomRight.pointY = ((GRAPHICS_INT)0);

        while (nextSegment->nextLineSegment != nullptr)
          {      
          // "Box" the character's extents
          // TopLeft X : 
          if (nextSegment->lineSegmentOriginX <= nextSegment->lineSegmentDestinationX)
            {
            if (nextSegment->lineSegmentOriginX <= characterReference->characterExtents.topLeft.pointX)
              {
              characterReference->characterExtents.topLeft.pointX = nextSegment->lineSegmentOriginX;
              }
            }
          else
            {
            if (nextSegment->lineSegmentDestinationX <= characterReference->characterExtents.topLeft.pointX)
              {
              characterReference->characterExtents.topLeft.pointX = nextSegment->lineSegmentDestinationX;
              }
            }
       
          // TopLeft Y : 
          if (nextSegment->lineSegmentOriginY <= nextSegment->lineSegmentDestinationY)
            {
            if (nextSegment->lineSegmentOriginY <= characterReference->characterExtents.topLeft.pointY)
              {
              characterReference->characterExtents.topLeft.pointY = nextSegment->lineSegmentOriginY;
              }
            }
          else
            {
            if (nextSegment->lineSegmentDestinationY <= characterReference->characterExtents.topLeft.pointY)
              {
              characterReference->characterExtents.topLeft.pointY = nextSegment->lineSegmentDestinationY;
              }
            }

          // BottomRight X : 
          if (nextSegment->lineSegmentOriginX >= nextSegment->lineSegmentDestinationX)
            {
            if (nextSegment->lineSegmentOriginX >= characterReference->characterExtents.bottomRight.pointX)
              {
              characterReference->characterExtents.bottomRight.pointX = nextSegment->lineSegmentOriginX;
              }
            }
          else
            {
            if (nextSegment->lineSegmentDestinationX >= characterReference->characterExtents.bottomRight.pointX)
              {
              characterReference->characterExtents.bottomRight.pointX = nextSegment->lineSegmentDestinationX;
              }
            }

          // BottomRight Y : 
          if (nextSegment->lineSegmentOriginY >= nextSegment->lineSegmentDestinationY)
            {
            if (nextSegment->lineSegmentOriginY >= characterReference->characterExtents.bottomRight.pointY)
              {
              characterReference->characterExtents.bottomRight.pointY = nextSegment->lineSegmentOriginY;
              }
            }
          else
            {
            if (nextSegment->lineSegmentDestinationY >= characterReference->characterExtents.bottomRight.pointY)
              {
              characterReference->characterExtents.bottomRight.pointY = nextSegment->lineSegmentDestinationY;
              }
            }

          nextSegment = nextSegment->nextLineSegment;
          }
        }
      }
    else
      {
      objectError = GRAPHICS_OBJECT_CHARACTER_ERROR;
      }
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of computeCharacterExtents                                         */

/******************************************************************************/
/* drawRectangle() :                                                          */
/* - draw line start- and end-rectangles                                      */
/******************************************************************************/

graphicsError_t drawRectangle(HDC                   hdc,
                              rectangleObject_tPtr  rectangleObject,
                              canvasDescriptor_t   *canvasSize)
  {
/******************************************************************************/

  Graphics        graphics(hdc);

  graphicsError_t objectError    = GRAPHICS_NO_ERROR;
                                 
  GRAPHICS_FLOAT  leftX          = ((GRAPHICS_FLOAT)0.0),
                  leftXo         = ((GRAPHICS_FLOAT)0.0),
                  topY           = ((GRAPHICS_FLOAT)0.0),
                  topYo          = ((GRAPHICS_FLOAT)0.0),
                  rightX         = ((GRAPHICS_FLOAT)0.0),
                  rightXo        = ((GRAPHICS_FLOAT)0.0),
                  bottomY        = ((GRAPHICS_FLOAT)0.0),
                  bottomYo       = ((GRAPHICS_FLOAT)0.0),
                  deltaX         = ((GRAPHICS_FLOAT)0.0),
                  deltaY         = ((GRAPHICS_FLOAT)0.0);

  RECT            fillFrame      = {
                                   ((LONG)0),
                                   ((LONG)0),
                                   ((LONG)0),
                                   ((LONG)0)
                                   };

  GRAPHICS_UINT   lineWidthIndex = ((GRAPHICS_UINT)0);

/******************************************************************************/

  // Drawing is in 'canvas' coordinates. gui coordinates are normalised to 
  // ((double)0.0)...((double)1.0) in the x- and y-axes. The final number to 
  // use is gui((double)x.y) * canvas((double)x.y) cast to 'float'
  if ((rectangleObject != nullptr) && (canvasSize != nullptr))
    {
    Pen        pen(Color((BYTE)((rectangleObject)->rectangleColour.opacity),
                         (BYTE)((rectangleObject)->rectangleColour.red),
                         (BYTE)((rectangleObject)->rectangleColour.green),
                         (BYTE)((rectangleObject)->rectangleColour.blue)));
    
    leftXo   = (GRAPHICS_FLOAT)((rectangleObject)->rectangleDimensions.leftX);
    topYo    = (GRAPHICS_FLOAT)((rectangleObject)->rectangleDimensions.topY);
    rightXo  = (GRAPHICS_FLOAT)((rectangleObject)->rectangleDimensions.rightX);
    bottomYo = (GRAPHICS_FLOAT)((rectangleObject)->rectangleDimensions.bottomY);

    if (rectangleObject->rectangleFillState == GRAPHICS_OBJECT_FILL_STATE_FULL)
      {    
      SolidBrush backgroundPen(Color((BYTE)((rectangleObject)->rectangleBackgroundColour.opacity),
                                     (BYTE)((rectangleObject)->rectangleBackgroundColour.red),
                                     (BYTE)((rectangleObject)->rectangleBackgroundColour.green),
                                     (BYTE)((rectangleObject)->rectangleBackgroundColour.blue)));

      RectF frameF(leftXo,
                   topYo,
                   rightXo,
                   bottomYo);

      // Fill in the enclosed rectangle (same colour as the enclosing box for now...)
      graphics.FillRectangle(&backgroundPen, frameF);
      }

    deltaX  = ((GRAPHICS_FLOAT)(canvasSize->right))  - ((GRAPHICS_FLOAT)(canvasSize->left));
    deltaY  = ((GRAPHICS_FLOAT)(canvasSize->bottom)) - ((GRAPHICS_FLOAT)(canvasSize->top));
    
    // Draw a set of nested rectangles to build the requested linewidth
    for (lineWidthIndex = ((GRAPHICS_UINT)0); lineWidthIndex < (GRAPHICS_UINT)(((rectangleObject_tPtr)(rectangleObject))->rectangleDimensions.lineWidth); lineWidthIndex++)
      {
      GRAPHICS_FLOAT lineWidthX = ((GRAPHICS_FLOAT)lineWidthIndex) / deltaX,
                     lineWidthY = ((GRAPHICS_FLOAT)lineWidthIndex) / deltaY;
    
      leftX   = ceilf((leftXo   - lineWidthX) * deltaX);
      topY    = ceilf((topYo    - lineWidthY) * deltaY);
      rightX  = ceilf((rightXo  + lineWidthX) * deltaX);
      bottomY = ceilf((bottomYo + lineWidthY) * deltaY);
      
      if (leftX < ((GRAPHICS_FLOAT)0.0))
        {
        leftX = ((GRAPHICS_FLOAT)0.0);
        }
    
      if (topY < ((GRAPHICS_FLOAT)0.0))
        {
        topY = ((GRAPHICS_FLOAT)0.0);
        }
    
      if (rightX > ((GRAPHICS_FLOAT)(canvasSize->right)))
        {
        rightX = ((GRAPHICS_FLOAT)(canvasSize->right));
        }
    
      if (bottomY > ((GRAPHICS_FLOAT)(canvasSize->bottom)))
        {
        bottomY = ((GRAPHICS_FLOAT)(canvasSize->bottom));
        }
    
      graphics.DrawLine((const Pen *)&pen, leftX,  topY,    rightX, topY);
      graphics.DrawLine((const Pen *)&pen, leftX,  bottomY, rightX, bottomY);
      graphics.DrawLine((const Pen *)&pen, leftX,  topY,    leftX,  bottomY);
      graphics.DrawLine((const Pen *)&pen, rightX, topY,    rightX, bottomY);
      }
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of drawRectangle                                                   */

/******************************************************************************/
/* moveCharacterInFrame() :                                                   */
/******************************************************************************/

graphicsError_t moveCharacterInFrame(      alphabetCharacters_tPtr  *selectedCharacter,
                                           alphabetCharacters_tPtr   alphabetCharacters,
                                           strokeNavStates_tPtr      navigationKeyState,
                                     const strokeGraphPointBase_tPtr strokeGraphBase)
  {
/******************************************************************************/

  lineSegment_tPtr thisLineSegment = nullptr;

  GRAPHICS_UINT    segmentIndex    = ((GRAPHICS_UINT)0);
                                   
  GRAPHICS_INT     leftX           = ((GRAPHICS_INT)0),
                   topY            = ((GRAPHICS_INT)0),
                   rightX          = ((GRAPHICS_INT)0),
                   bottomY         = ((GRAPHICS_INT)0);
                                   
  graphicsError_t  objectError     = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((selectedCharacter  != nullptr) && ((*selectedCharacter) != nullptr) &&
      (alphabetCharacters != nullptr) &&
      (navigationKeyState != nullptr) && (strokeGraphBase      != nullptr))
    {
    switch(*navigationKeyState)
      {
      case STROKE_NAV_STATE_CHARACTER_LEFT  : leftX   = ((GRAPHICS_INT)1);
                                              break;  
                                                      
      case STROKE_NAV_STATE_CHARACTER_RIGHT : rightX  = ((GRAPHICS_INT)1);
                                              break;  
                                                      
      case STROKE_NAV_STATE_CHARACTER_UP    : topY    = ((GRAPHICS_INT)1);
                                              break;  
                                                      
      case STROKE_NAV_STATE_CHARACTER_DOWN  : bottomY = ((GRAPHICS_INT)1);
                                              break;

      default                               : objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
                                              break;
      }

    if (objectError == GRAPHICS_NO_ERROR)
      {
      // One-by-one, point at the already extant line segments
      while ((objectError = fetchCharacterExtantSegment( segmentIndex,
                                                        *selectedCharacter,
                                                        &thisLineSegment)) == GRAPHICS_NO_ERROR)
        {
        // Move the character to the left ?
        if (((*selectedCharacter)->characterExtents.topLeft.pointX - leftX) >= ((GRAPHICS_INT)0))
          {
          thisLineSegment->lineSegmentOriginX      = thisLineSegment->lineSegmentOriginX      - leftX;
          thisLineSegment->lineSegmentDestinationX = thisLineSegment->lineSegmentDestinationX - leftX;
          }
      
        // Move the character to the right ?
        if (((*selectedCharacter)->characterExtents.bottomRight.pointX + rightX) <= ((GRAPHICS_INT)strokeGraphBase->graphRowNumber))
          {
          thisLineSegment->lineSegmentOriginX      = thisLineSegment->lineSegmentOriginX      + rightX;
          thisLineSegment->lineSegmentDestinationX = thisLineSegment->lineSegmentDestinationX + rightX;
          }
      
        // Move the character up ?
        if (((*selectedCharacter)->characterExtents.topLeft.pointY - topY) >= ((GRAPHICS_INT)0))
          {
          thisLineSegment->lineSegmentOriginY      = thisLineSegment->lineSegmentOriginY      - topY;
          thisLineSegment->lineSegmentDestinationY = thisLineSegment->lineSegmentDestinationY - topY;
          }
      
        // Move the character down ?
        if (((*selectedCharacter)->characterExtents.bottomRight.pointY + bottomY) <= ((GRAPHICS_INT)strokeGraphBase->graphColumnNumber))
          {
          thisLineSegment->lineSegmentOriginY      = thisLineSegment->lineSegmentOriginY      + bottomY;
          thisLineSegment->lineSegmentDestinationY = thisLineSegment->lineSegmentDestinationY + bottomY;
          }

        segmentIndex = segmentIndex + ((GRAPHICS_UINT)1);
        }
      }

    // Compute the new character extents
    objectError = computeCharacterExtents(((const GRAPHICS_UINT)(*selectedCharacter)->characterNumber),
                                          alphabetCharacters,
                                          (const strokeGraphPointBase_tPtr)strokeGraphBase);
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of moveCharacterInFrame                                            */

/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/
