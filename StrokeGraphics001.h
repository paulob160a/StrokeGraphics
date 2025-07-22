/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/
/*                                                                            */
/* StrokeGraphics001.h :                                                      */
/* Paul O'Brien                                                               */
/* 28.05.25                                                                   */
/*                                                                            */
/* 2D GUI to create stroke/vector graphic alphanumeric set                    */
/******************************************************************************/

#ifndef _STROKE_GRAPHICS_H_
#define _STROKE_GRAPHICS_H_

/******************************************************************************/

#include "resource.h"
#include <Windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include "StrokeGraphicsTypes.h"
//#include "StrokeGraphicsUtilities.h"
//#include "StrokeGraphicsAlphabet.h"
//#include "StrokeGraphicsCaption.h"

/******************************************************************************/

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

/******************************************************************************/

#define MAX_LOADSTRING                     (128)

#define GDI_PLUS_WINDOW_MENU_STATE         ((BOOL)FALSE)

#define KEYBOARD_STATES                    (256)
#define KEY_DOWN_DETECTION_BIT             ((GRAPHICS_SHORT)0x0080)
#define CAPS_LOCK_DETECTION_BIT            ((GRAPHICS_SHORT)0x0001)
#define ASYNC_KEYPRESSED                   ((GRAPHICS_USHORT)0x8000)

#define EXTENDED_ASCII_MASK                ((GRAPHICS_UINT)0x000007F)

#define KEY_PRESS_UNDECODED                ((GRAPHICS_USHORT)0x0000)

#define CANVAS_SIZE_ZERO_X                 ((double)64.0)
#define CANVAS_SIZE_ZERO_Y                 ((double)64.0)
#define CANVAS_SIZE_WIDTH                  ((double)1680.0)
#define CANVAS_SIZE_HEIGHT                 ((double)840.0)

#define GDI_PLUS_WINDOW_RECTANGLE_X_LEFT   ((LONG)CANVAS_SIZE_ZERO_X)
#define GDI_PLUS_WINDOW_RECTANGLE_Y_TOP    ((LONG)CANVAS_SIZE_ZERO_Y)
#define GDI_PLUS_WINDOW_RECTANGLE_X_BOTTOM ((LONG)CANVAS_SIZE_WIDTH)
#define GDI_PLUS_WINDOW_RECTANGLE_Y_BOTTOM ((LONG)CANVAS_SIZE_HEIGHT)

#define STROKE_GRAPHICS_CLASS_NAME          L"StrokeGraphics001"
#define STROKE_GRAPHICS_CAPTION             L"StrokeGraphics001"

#define STROKE_GRAPHICS_TITLE               L"StrokeGraphics001"
#define STROKE_GRAPHICS_CLASS               L"StrokeGraphics001"

#define STROKE_GRAPHICS_ARGUMENT_0          (0)
#define STROKE_GRAPHICS_ARGUMENT_1          (STROKE_GRAPHICS_ARGUMENT_0 + 1)
#define STROKE_GRAPHICS_ARGUMENT_2          (STROKE_GRAPHICS_ARGUMENT_1 + 1)
#define STROKE_GRAPHICS_ARGUMENT_3          (STROKE_GRAPHICS_ARGUMENT_2 + 1)
#define STROKE_GRAPHICS_ARGUMENT_4          (STROKE_GRAPHICS_ARGUMENT_3 + 1)
#define STROKE_GRAPHICS_ARGUMENT_5          (STROKE_GRAPHICS_ARGUMENT_4 + 1)
#define STROKE_GRAPHICS_ARGUMENTS           (STROKE_GRAPHICS_ARGUMENT_5 + 1)

#define STROKE_GRAPHICS_FRAME_X_POINTS       STROKE_GRAPHICS_ARGUMENT_1
#define STROKE_GRAPHICS_FRAME_Y_POINTS       STROKE_GRAPHICS_ARGUMENT_2
#define STROKE_GRAPHICS_LOAD_ALPHABET_NUMBER STROKE_GRAPHICS_ARGUMENT_3
#define STROKE_GRAPHICS_SAVE_ALPHABET_NUMBER STROKE_GRAPHICS_ARGUMENT_4
#define STROKE_GRAPHICS_MODIFICATION_FLAG    STROKE_GRAPHICS_ARGUMENT_5

#define KEY_ESC_EXIT                        (0x1B)

#define GRAPHICS_CHAR_EOS_LENGTH            (1)
#define GRAPHICS_WCHAR_EOS_LENGTH           (2)

#define GRAPHICS_CHAR_LOWER                 (0)
#define GRAPHICS_CHAR_UPPER                 (GRAPHICS_CHAR_LOWER + 1)
#define GRAPHICS_CHAR_LENGTH                (GRAPHICS_CHAR_UPPER + 1)
#define GRAPHICS_WORD_LENGTH                (16)

#define COPY_CHARACTER_BUFFER_OFFSET_0      (0)
#define COPY_CHARACTER_BUFFER_SOURCE        COPY_CHARACTER_BUFFER_OFFSET_0
#define COPY_CHARACTER_BUFFER_OFFSET_1      (COPY_CHARACTER_BUFFER_OFFSET_0 + 1)
#define COPY_CHARACTER_BUFFER_DESTINATION   COPY_CHARACTER_BUFFER_OFFSET_1
#define COPY_CHARACTER_BUFFER_LENGTH        (COPY_CHARACTER_BUFFER_OFFSET_1 + 1)

// Define the alphanumeric and printable ascii code ranges
#define GRAPHICS_NUMERIC_KEY_CODE_0         '0'
#define GRAPHICS_NUMERIC_KEY_CODE_9         '9'
                                            
#define GRAPHICS_ALPHA_KEY_CODE_A           'A'
#define GRAPHICS_ALPHA_KEY_CODE_Z           'Z'
                                            
#define GRAPHICS_ALPHA_KEY_CODE_a           'a'
#define GRAPHICS_ALPHA_KEY_CODE_z           'z'

#define GRAPHICS_ALPHA_KEY_OFFSET           (GRAPHICS_ALPHA_KEY_CODE_a - GRAPHICS_ALPHA_KEY_CODE_A)

#define GRAPHICS_AMPERSAND                  (0x26)

/******************************************************************************/
/* Colour Palette : the definitions and their enumerations must be kept in    */
/*                  lock-step. New colour definitions must be added to the    */
/*                  next numeral in the enumeration list                      */
/******************************************************************************/

// Have to appeal to the power of 'C' to extract the colour values!

#define SELECT_BYTE_0                                       ((GRAPHICS_UINT)0)
#define UNSIGNED_INT_MASK_BYTE_0                            ((GRAPHICS_UINT)0x000000ff)
#define UNSIGNED_INT_SHIFT_BYTE_0                           (0)
    
#define SELECT_BYTE_1                                       ((GRAPHICS_UINT)1)
#define UNSIGNED_INT_MASK_BYTE_1                            ((GRAPHICS_UINT)0x0000ff00)
#define UNSIGNED_INT_SHIFT_BYTE_1                           (8)
                         
#define SELECT_BYTE_2                                       ((GRAPHICS_UINT)2)
#define UNSIGNED_INT_MASK_BYTE_2                            ((GRAPHICS_UINT)0x00ff0000)
#define UNSIGNED_INT_SHIFT_BYTE_2                           (UNSIGNED_INT_SHIFT_BYTE_1 << 1)
                                                            
#define SELECT_BYTE_3                                       ((GRAPHICS_UINT)3)
#define UNSIGNED_INT_MASK_BYTE_3                            ((GRAPHICS_UINT)0xff000000)
#define UNSIGNED_INT_SHIFT_BYTE_3                           ((UNSIGNED_INT_SHIFT_BYTE_1 << 1) + UNSIGNED_INT_SHIFT_BYTE_1)

#define EXTRACT_COLOUR_FIELD_BYTE(colourField,selectedByte,requestedByte) { switch(selectedByte) \
                                                                              { \
                                                                              case SELECT_BYTE_0 : *requestedByte = (GRAPHICS_BYTE)((((GRAPHICS_UINT)colourField) & UNSIGNED_INT_MASK_BYTE_0) >> UNSIGNED_INT_SHIFT_BYTE_0); \
                                                                                                   break; \
                                                                              case SELECT_BYTE_1 : *requestedByte = (GRAPHICS_BYTE)((((GRAPHICS_UINT)colourField) & UNSIGNED_INT_MASK_BYTE_1) >> UNSIGNED_INT_SHIFT_BYTE_1); \
                                                                                                   break; \
                                                                              case SELECT_BYTE_2 : *requestedByte = (GRAPHICS_BYTE)((((GRAPHICS_UINT)colourField) & UNSIGNED_INT_MASK_BYTE_2) >> UNSIGNED_INT_SHIFT_BYTE_2); \
                                                                                                   break; \
                                                                              case SELECT_BYTE_3 : *requestedByte = (GRAPHICS_BYTE)((((GRAPHICS_UINT)colourField) & UNSIGNED_INT_MASK_BYTE_3) >> UNSIGNED_INT_SHIFT_BYTE_3); \
                                                                                                   break; \
                                                                              default            : *requestedByte = ((GRAPHICS_BYTE)0); \
                                                                                                   break; \
                                                                              } \
                                                                          }
#define FULL_COLOUR_CHANNEL    ((BYTE)0xff)
#define NULL_COLOUR_CHANNEL    ((BYTE)0x00)
                               
#define FULL_OPAQUE_CHANNEL    ((BYTE)0xff)
#define NULL_OPAQUE_CHANNEL    ((BYTE)0x00)
                               
#define NO_COLOUR_PEN          ((BYTE)0x00), \
                               ((BYTE)0x00), \
                               ((BYTE)0x00), \
                               ((BYTE)0x00) // transparent pen

#define NO_COLOUR_PEN_CODE     ((GRAPHICS_UINT)((NULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                                 NULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                                 NULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                                 NULL_OPAQUE_CHANNEL << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))

#define DARK_GREEN_PEN         ((BYTE)0xFF), \
                               ((BYTE)0x16), \
                               ((BYTE)0x96), \
                               ((BYTE)0x19) // opaque DARK GREEN pen
                               
#define DARK_GREEN_PEN_CODE    ((GRAPHICS_UINT)((FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                                (((BYTE)0x16)        << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                                (((BYTE)0x96)        << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                                (((BYTE)0x19)        << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))
                               
#define BRIGHT_YELLOW_PEN      ((BYTE)0xFF), \
                               ((BYTE)0xFF), \
                               ((BYTE)0xFF), \
                               ((BYTE)0x00) // opaque BRIGHT YELLOW pen

#define BRIGHT_YELLON_PEN_CODE ((GRAPHICS_UINT)((FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                                (((BYTE)0xFF)        << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                                (((BYTE)0xFF)        << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                                (((BYTE)0x00)        << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))

#define LIGHT_BLUE_PEN         ((BYTE)0xFF), \
                               ((BYTE)0x84), \
                               ((BYTE)0xBC), \
                               ((BYTE)0xEF) // opaque LIGHT BLUE pen
                               
#define LIGHT_BLUE_PEN_CODE    ((GRAPHICS_UINT)((FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                                (((BYTE)0x84)        << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                                (((BYTE)0xBC)        << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                                (((BYTE)0xEF)        << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))
                               
#define BLUE_PEN               FULL_OPAQUE_CHANNEL, \
                               NULL_COLOUR_CHANNEL, \
                               NULL_COLOUR_CHANNEL, \
                               FULL_COLOUR_CHANNEL  // opaque BLUE pen
                               
#define BLUE_PEN_CODE          ((GRAPHICS_UINT)((FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                                (NULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                                (NULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                                (FULL_OPAQUE_CHANNEL << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))
                               
#define GREEN_PEN              FULL_OPAQUE_CHANNEL, \
                               NULL_COLOUR_CHANNEL, \
                               FULL_COLOUR_CHANNEL, \
                               NULL_COLOUR_CHANNEL  // opaque GREEN pen 
                               
#define GREEN_PEN_CODE         ((GRAPHICS_UINT)((FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                                (NULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                                (FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                                (NULL_OPAQUE_CHANNEL << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))
                               
#define RED_PEN                FULL_OPAQUE_CHANNEL, \
                               FULL_COLOUR_CHANNEL, \
                               NULL_COLOUR_CHANNEL, \
                               NULL_COLOUR_CHANNEL  // opaque RED pen
                               
#define RED_PEN_CODE           ((GRAPHICS_UINT)((FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                                (FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                                (NULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                                (NULL_OPAQUE_CHANNEL << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))

#define CYAN_PEN               FULL_OPAQUE_CHANNEL, \
                               NULL_COLOUR_CHANNEL, \
                               FULL_COLOUR_CHANNEL, \
                               FULL_COLOUR_CHANNEL  // opaque CYAN pen
                               
#define CYAN_PEN_CODE          ((GRAPHICS_UINT) ((FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                                 (NULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                                 (FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                                 (FULL_OPAQUE_CHANNEL << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))
                               
#define MAGENTA_PEN            FULL_OPAQUE_CHANNEL, \
                               FULL_COLOUR_CHANNEL, \
                               NULL_COLOUR_CHANNEL, \
                               FULL_COLOUR_CHANNEL  // opaque MAGENTA pen
                            
#define MAGENTA_PEN_CODE       ((GRAPHICS_UINT)((FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                                (FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                                (NULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                                (FULL_OPAQUE_CHANNEL << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))
                               
#define YELLOW_PEN              FULL_OPAQUE_CHANNEL, \
                                FULL_COLOUR_CHANNEL, \
                                FULL_COLOUR_CHANNEL, \
                                NULL_COLOUR_CHANNEL  // opaque YELLOW pen
                               
#define YELLOW_PEN_CODE        ((GRAPHICS_UINT)((FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                                (FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                                (FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                                (NULL_OPAQUE_CHANNEL << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))
                               
#define BLACK_PEN_OPAQUE        FULL_OPAQUE_CHANNEL, \
                                NULL_COLOUR_CHANNEL, \
                                NULL_COLOUR_CHANNEL, \
                                NULL_COLOUR_CHANNEL  // opaque BLACK pen
                               
#define BLACK_PEN_CODE         ((GRAPHICS_UINT)((FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                                (NULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                                (NULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                                (NULL_OPAQUE_CHANNEL << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))
                               
#define WHITE_PEN_OPAQUE        FULL_OPAQUE_CHANNEL, \
                                FULL_COLOUR_CHANNEL, \
                                FULL_COLOUR_CHANNEL, \
                                FULL_COLOUR_CHANNEL  // opaque WHITE pen
                               
#define WHITE_PEN_OPAQUE_CODE   ((GRAPHICS_UINT)((FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                                 (FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                                 (FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                                 (FULL_OPAQUE_CHANNEL << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))

typedef enum lineSegmentColours_tTag
  {
  LINE_SEGMENT_COLOUR_BLUE = 0,
  LINE_SEGMENT_COLOUR_GREEN,
  LINE_SEGMENT_COLOUR_RED,
  LINE_SEGMENT_COLOUR_CYAN,
  LINE_SEGMENT_COLOUR_MAGENTA,
  LINE_SEGMENT_COLOUR_YELLOW,
  LINE_SEGMENT_COLOUR_BLACK_OPAQUE,
  LINE_SEGMENT_COLOUR_WHITE_OPAQUE,
  LINE_SEGMENT_COLOUR_DARK_GREEN,
  LINE_SEGMENT_COLOUR_BRIGHT_YELLOW,
  LINE_SEGMENT_COLOUR_LIGHT_BLUE,
  LINE_SEGMENT_COLOURS
  } lineSegmentColours_t, *lineSegmentColours_tPtr;

#pragma pack(push,1)
typedef struct colourPalette_tTag
  {
  BYTE colourOpacity;
  BYTE colourRedComponent;
  BYTE colourGreenComponent;
  BYTE colourBlueComponent;
  } colourPalette_t, *colourPalette_tPtr;
#pragma pack(pop)

/******************************************************************************/
/* Line Segments :                                                            */
/*  line segments are highlighted by rectangles at their start and finish     */
/*  points                                                                    */
/*  - the dimensions are RELATIVE to a local { 0, 0 }                         */
/******************************************************************************/

#define GRAPHICS_LINE_ORIGIN_BASE_X         ((GRAPHICS_REAL)0.0)
#define GRAPHICS_LINE_ORIGIN_BASE_Y         ((GRAPHICS_REAL)0.0)

#define GRAPHICS_LINE_ORIGIN_LEFT_X         ((GRAPHICS_LINE_ORIGIN_BASE_X) + ((GRAPHICS_REAL)-5.0))
#define GRAPHICS_LINE_ORIGIN_TOP_Y          ((GRAPHICS_LINE_ORIGIN_BASE_Y) + ((GRAPHICS_REAL)-5.0))
#define GRAPHICS_LINE_ORIGIN_RIGHT_X        ((GRAPHICS_LINE_ORIGIN_BASE_X) + ((GRAPHICS_REAL) 5.0))
#define GRAPHICS_LINE_ORIGIN_BOTTOM_Y       ((GRAPHICS_LINE_ORIGIN_BASE_Y) + ((GRAPHICS_REAL) 5.0))
                  
#define GRAPHICS_LINE_ORIGIN_COLOUR         RED_PEN

#define GRAPHICS_LINE_DESTINATION_BASE_X    ((GRAPHICS_REAL)0.0)
#define GRAPHICS_LINE_DESTINATION_BASE_Y    ((GRAPHICS_REAL)0.0)

#define GRAPHICS_LINE_DESTINATION_LEFT_X    ((GRAPHICS_LINE_DESTINATION_BASE_X) + ((GRAPHICS_REAL)-5.0))
#define GRAPHICS_LINE_DESTINATION_TOP_Y     ((GRAPHICS_LINE_DESTINATION_BASE_Y) + ((GRAPHICS_REAL)-5.0))
#define GRAPHICS_LINE_DESTINATION_RIGHT_X   ((GRAPHICS_LINE_DESTINATION_BASE_X) + ((GRAPHICS_REAL) 5.0))
#define GRAPHICS_LINE_DESTINATION_BOTTOM_Y  ((GRAPHICS_LINE_DESTINATION_BASE_Y) + ((GRAPHICS_REAL) 5.0))

#define GRAPHICS_LINE_DESTINATION_COLOUR    GREEN_PEN

#define GRAPHICS_LINE_SEGMENT_COLOUR        RED_PEN

/******************************************************************************/


#define STROKE_GRAPHICS_DEFAULT_MENU_CODE      ((GRAPHICS_UINT)0)
#define STROKE_GRAPHICS_DEFAULT_KEYBOARD_CODE  ((HKL)0x00000452)

#define CHARACTER_DELETE_KEY             VK_F3
#define CHARACTER_SELECT_KEY             VK_F5
#define CHARACTER_RECOVER_FROM_CACHE_KEY VK_F6
#define LINE_SEGMENT_MODIFY_ON_KEY       VK_F7
#define LINE_SEGMENT_MODIFY_OFF_KEY      VK_F8
#define CHARACTER_COPY_A_TO_B_KEY        VK_F9
#define CANCEL_COPY_A_TO_B_KEY           VK_F11
#define STROKE_GRAPH_TOGGLE              VK_BACK
#define STROKE_CURSOR_KEY_TOGGLE         VK_END
#define ALPHABET_SAVE_KEY                VK_HOME
#define LINE_SEGMENT_MODIFY_DELETE_KEY   VK_DELETE
#define CHARACTER_LINE_SEGMENT_SAVE_KEY  VK_INSERT
                                         
#define CHARACTER_FLIP_VERTICAL          VK_PRIOR
#define CHARACTER_FLIP_HORIZONTAL        VK_NEXT

typedef struct translatedCharacterState_t
  {
  bool keydownState;
  bool receivedCodeState;
  } translatedCharacterState_tTag;

typedef union translatedCharacterBuffer_tTag
  {
  GRAPHICS_USHORT translatedCharacterPair;
  BYTE            translatedCharactersA[GRAPHICS_CHAR_LENGTH];
  GRAPHICS_WCHAR  translatedCharactersW[GRAPHICS_WORD_LENGTH];
  } translatedCharacterBuffer_t;

typedef enum strokeGraphToggleState_tTag
  {
  STROKE_GRAPH_TOGGLE_STATE_NONE = 0,
  STROKE_GRAPH_TOGGLE_STATE_SELECTED,
  STROKE_GRAPH_TOGGLE_STATE_NOT_SELECTED,
  STROKE_GRAPH_TOGGLE_STATES
  } strokeGraphToggleState_t;

typedef enum strokeCursorToggleState_tTag
  {
  STROKE_CURSOR_TOGGLE_STATE_NONE = 0,
  STROKE_CURSOR_TOGGLE_STATE_SELECTED,
  STROKE_CURSOR_TOGGLE_STATE_NOT_SELECTED,
  STROKE_CURSOR_TOGGLE_STATES
  } strokeCursorToggleState_t;

typedef enum characterSelectionStates_tTag
  {
  CHARACTER_SELECTION_STATE_NONE = 0,
  CHARACTER_SELECTION_STATE_SELECTED,
  CHARACTER_SELECTION_STATE_DESELECTED,
  CHARACTER_SELECTION_STATE_MODIFY_NONE,
  CHARACTER_SELECTION_STATE_MODIFY_ON,
  CHARACTER_SELECTION_STATE_MODIFY_OFF,
  CHARACTER_SELECTION_STATES
  } characterSelectionStates_t, *characterSelectionStates_tPtr;

#pragma pack(push,1)
typedef struct characterSelectionState_tTag
  {
  GRAPHICS_UINT                selectedCharacter;
  characterSelectionStates_t   lastSelectionState;
  characterSelectionStates_t   thisSelectionState;
  characterSelectionStates_t   modificationState;
  strokeCursorToggleState_t    strokeCursorToggleState;
  } characterSelectionState_t, *characterSelectionState_tPtr;
#pragma pack(pop)

typedef enum strokeDrawingState_tTag
  {
  STROKE_DRAWING_STATE_NONE   = 0,
  STROKE_DRAWING_STATE_REDRAW,
  STROKE_DRAWING_STATES
  } strokeDrawingState_t, *strokeDrawingState_tPtr;

#pragma pack(push,1)
typedef struct rectangleDimensions_tTag
  {
  GRAPHICS_REAL leftX;
  GRAPHICS_REAL topY;
  GRAPHICS_REAL rightX;
  GRAPHICS_REAL bottomY;
  GRAPHICS_UINT lineWidth;
  } rectangleDimensions_t, *rectangleDimensions_tPtr;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct canvasDescriptor_tTag
  {
  GRAPHICS_REAL left;
  GRAPHICS_REAL top;
  GRAPHICS_REAL right;
  GRAPHICS_REAL bottom;
  GRAPHICS_REAL width;
  GRAPHICS_REAL height;
  GRAPHICS_REAL centreX;
  GRAPHICS_REAL centreY;
  } canvasDescriptor_t, *canvasDescriptor_tPtr;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct captionDescriptor_tTag
  {
  GRAPHICS_REAL captionFrameLeftX;
  GRAPHICS_REAL captionFrameTopY;
  GRAPHICS_REAL captionFrameRightX;
  GRAPHICS_REAL captionFrameBottomY;
  } captionDescriptor_t, *captionDescriptor_tPtr;
#pragma pack(pop)

typedef enum objectFillState_t
  {
  GRAPHICS_OBJECT_FILL_STATE_NONE = 0,
  GRAPHICS_OBJECT_FILL_STATE_FULL,
  GRAPHICS_OBJECT_FILL_STATE_EMPTY,
  GRAPHICS_OBJECT_FILL_STATES
  } objectFillState_t, *objectFillState_tPtr;

#pragma pack(push,1)
typedef struct rectangleObject_tTag
  {
  objectColour_t        rectangleColour;
  objectColour_t        rectangleBackgroundColour;
  objectFillState_t     rectangleFillState;
  rectangleDimensions_t rectangleDimensions;
  } rectangleObject_t,  *rectangleObject_tPtr;
#pragma pack(pop)

// Default screen/canvas dimensions
#define GUI_SCREEN_GUI_DEFAULT_LEFT_X   ((GRAPHICS_REAL)0.0)
#define GUI_SCREEN_GUI_DEFAULT_TOP_Y    ((GRAPHICS_REAL)0.0)
#define GUI_SCREEN_GUI_DEFAULT_RIGHT_X  ((GRAPHICS_REAL)1920.0)
#define GUI_SCREEN_GUI_DEFAULT_BOTTOM_Y ((GRAPHICS_REAL)1280.0)

#pragma pack(push,1)
typedef struct globalScreensCoordinates_tTag
  {
  GRAPHICS_REAL screenLeftX;
  GRAPHICS_REAL screenTopY;
  GRAPHICS_REAL screenRightX;
  GRAPHICS_REAL screenBottomY;
  } globalScreenCoordinates_t, *globalScreenCoordinates_tPtr;
#pragma pack(pop)

// The normalised stroke frame
#define STROKE_FRAME_LEFT_X_NORMAL   ((GRAPHICS_REAL)0.0)
#define STROKE_FRAME_TOP_Y_NORMAL    ((GRAPHICS_REAL)0.0)
#define STROKE_FRAME_RIGHT_X_NORMAL  ((GRAPHICS_REAL)1.0)
#define STROKE_FRAME_BOTTOM_Y_NORMAL ((GRAPHICS_REAL)1.0)
#define STROKE_FRAME_DELTA_X_NORMAL  (STROKE_FRAME_RIGHT_X_NORMAL  - STROKE_FRAME_LEFT_X_NORMAL)
#define STROKE_FRAME_DELTA_Y_NORMAL  (STROKE_FRAME_BOTTOM_Y_NORMAL - STROKE_FRAME_TOP_Y_NORMAL)

// The NOMINAL stroke frame coordinates
#define STROKE_FRAME_WIDTH           ((GRAPHICS_REAL)0.6)
#define STROKE_FRAME_DEPTH           ((GRAPHICS_REAL)0.8)
                                    
#define STROKE_FRAME_LEFT_X          ((GRAPHICS_REAL)0.05)
#define STROKE_FRAME_TOP_Y           ((GRAPHICS_REAL)0.05)
#define STROKE_FRAME_RIGHT_X         (STROKE_FRAME_LEFT_X + STROKE_FRAME_WIDTH)
#define STROKE_FRAME_BOTTOM_Y        (STROKE_FRAME_TOP_Y  + STROKE_FRAME_DEPTH)

#define STROKE_FRAME_DETECT_ODD_EVEN ((GRAPHICS_UINT)0x00000001)

#pragma pack(push,1)
typedef struct strokeFrameCoordinates_tTag
  {
  GRAPHICS_REAL frameLeftX;
  GRAPHICS_REAL frameTopY;
  GRAPHICS_REAL frameRightX;
  GRAPHICS_REAL frameBottomY;
  } strokeFrameCoordinates_t, *strokeFrameCoordinates_tPtr;
#pragma pack(pop)

typedef enum strokeCellPointType_tTag
  {
  STROKE_CELL_POINT_TYPE_NONE = 0,
  STROKE_CELL_POINT_TYPE_LINE_START,
  STROKE_CELL_POINT_TYPE_LINE_END,
  STROKE_CELL_POINT_TYPE_LINE_MID,
  STROKE_CELL_POINT_TYPE_FIRST_LINE_START,
  STROKE_CELL_POINT_TYPE_LAST_LINE_START,
  STROKE_CELL_POINT_TYPES
  } strokeCellPointType_t, *strokeCellPointType_tPtr;

// Stroke cell points SHALL be stored as row-wise i.e. { p(0,0) .. p(0,[M-1]) } .. { p([N-1],0) .. p([N-1],[M-1]) }
#pragma pack(push,1)
typedef struct strokeCellPoint_tTag
  {
  GRAPHICS_REAL          xAxisPoint;
  GRAPHICS_REAL          yAxisPoint;
  strokeCellPointType_t  pointType;
  strokeCellPoint_tTag  *nextStrokeCellPoint;
  strokeCellPoint_tTag  *lastStrokeCellPoint;
  } strokeCellPoint_t, *strokeCellPoint_tPtr;
#pragma pack(pop)

// A single index, integer and floating-point
#pragma pack(push,1)
typedef struct singleIndex_tTag
  {
  GRAPHICS_UINT xAxisIndex;
  GRAPHICS_UINT yAxisIndex;
  } singleIndex_t, *singleIndex_tPtr;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct singleIndexReal_tTag
  {
  GRAPHICS_REAL xAxisIndex;
  GRAPHICS_REAL yAxisIndex;
  } singleIndexReal_t, *singleIndexReal_tPtr;
#pragma pack(pop)

#define STROKE_START_CURSOR_DEFAULT          GREEN_PEN
#define STROKE_START_CURSOR_LIT              CYAN_PEN
                                             
#define STROKE_START_CURSOR_DEFAULT_CODE     GREEN_PEN_CODE
#define STROKE_START_CURSOR_LIT_CODE         CYAN_PEN_CODE
                                             
#define STROKE_END_CURSOR_DEFAULT            LIGHT_BLUE_PEN
#define STROKE_END_CURSOR_LIT                MAGENTA_PEN
                                             
#define STROKE_END_CURSOR_DEFAULT_CODE       LIGHT_BLUE_PEN_CODE
#define STROKE_END_CURSOR_LIT_CODE           MAGENTA_PEN_CODE

#define STROKE_CURSOR_MINIMUM_CAPTURE_EXTENT ((GRAPHICS_REAL)0.0)
#define STROKE_CURSOR_MAXIMUM_CAPTURE_EXTENT ((GRAPHICS_REAL)5.0)

// Navigation key assignments : 
#define STROKE_NAV_KEY_LEFT                  VK_LEFT
#define STROKE_NAV_KEY_RIGHT                 VK_RIGHT
#define STROKE_NAV_KEY_DOWN                  VK_DOWN
#define STROKE_NAV_KEY_UP                    VK_UP
                                             
#define STROKE_NAV_KEY_CURSOR_PAIR           VK_F1
#define STROKE_NAV_KEY_CURSOR_DESELECT       VK_F3
#define STROKE_NAV_KEY_CURSOR_SWAP           VK_F4         

typedef enum strokeNavStates_tTag
  {
  STROKE_NAV_STATE_NONE,
  STROKE_NAV_STATE_CURSOR_LEFT,
  STROKE_NAV_STATE_CURSOR_RIGHT,
  STROKE_NAV_STATE_CURSOR_UP,
  STROKE_NAV_STATE_CURSOR_DOWN,
  STROKE_NAV_STATE_CHARACTER_LEFT,
  STROKE_NAV_STATE_CHARACTER_RIGHT,
  STROKE_NAV_STATE_CHARACTER_UP,
  STROKE_NAV_STATE_CHARACTER_DOWN,
  STROKE_NAV_STATE_SAVE_SEGMENT,
  STROKE_NAV_STATE_DELETE_SEGMENT,
  STROKE_NAV_STATES
  } strokeNavStates_t, *strokeNavStates_tPtr;

typedef enum strokeNavKeyStates_tTag
  {
  STROKE_NAV_KEY_STATE_NONE                         = 0,
  STROKE_NAV_KEY_STATE_LINE_START_CURSOR_DESELECTED = 1,
  STROKE_NAV_KEY_STATE_LINE_START_CURSOR_SELECTED   = (STROKE_NAV_KEY_STATE_LINE_START_CURSOR_DESELECTED << 1),
  STROKE_NAV_KEY_STATE_LINE_END_CURSOR_DESELECTED   = (STROKE_NAV_KEY_STATE_LINE_START_CURSOR_SELECTED   << 1),
  STROKE_NAV_KEY_STATE_LINE_END_CURSOR_SELECTED     = (STROKE_NAV_KEY_STATE_LINE_END_CURSOR_DESELECTED   << 1),
  STROKE_NAV_KEY_STATE_LINE_BOTH_CURSORS_SELECTED   = (STROKE_NAV_KEY_STATE_LINE_END_CURSOR_SELECTED     << 1),
  STROKE_NAV_KEY_STATE_LINE_BOTH_START_LEADING      = (STROKE_NAV_KEY_STATE_LINE_BOTH_CURSORS_SELECTED   << 1),
  STROKE_NAV_KEY_STATE_LINE_BOTH_END_LEADING        = (STROKE_NAV_KEY_STATE_LINE_BOTH_START_LEADING      << 1),
  STROKE_NAV_KEY_STATES                             = 8
  } strokeNavKeyStates_t, *strokeNavKeyStates_tPtr;

#pragma pack(push,1)
typedef struct strokeCursor_tTag
  {
  singleIndex_t        strokeCursorIndexDefault;      // cursor index to the stroke graph (i.e. 0..[n - 1])
  singleIndex_t        strokeCursorIndexLit;          // cursor index to the stroke graph (i.e. 0..[m - 1])
  GRAPHICS_REAL        strokeCursorDetectionExtent;   // normalised delta for x- and y-axis coordinates detection box
  objectColour_t       strokeCursorColourDefault;     // cursor colour at rest
  GRAPHICS_UINT        strokeCursorColourDefaultCode;
  objectColour_t       strokeCursorColourLit;         // cursor colour in motion
  GRAPHICS_UINT        strokeCursorColourLitCode;
  strokeNavKeyStates_t strokeCursorState;
  GRAPHICS_REAL        lineWidth;
  } strokeCursor_t, *strokeCursor_tPtr;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct strokeCell_tTag
  {
  strokeCellPoint_tPtr  strokeCellPoint;
  GRAPHICS_REAL         strokeXAxisPoints; // the number of stroke lines in 'x'
  GRAPHICS_REAL         strokeYAxisPoints; // the number of stroke lines in 'y'
  strokeCell_tTag      *nextStrokeCell;
  strokeCell_tTag      *lastStrokeCell;
  } strokeCell_t, *strokeCell_tPtr;
#pragma pack(pop)

typedef enum lineSegmentMode_tTag
  {
  LINE_SEGMENT_MODE_NONE = 0,
  LINE_SEGMENT_MODE_PASSIVE,
  LINE_SEGMENT_MODE_ACTIVE,
  LINE_SEGMENT_MODE_EDIT,
  LINE_SEGMENT_MODES
  } lineSegmentMode_t, *lineSegmentMode_tPtr;

#define CURSOR_LINE_WIDTH   ((GRAPHICS_REAL)3)
#define STROKE_LINE_WIDTH   ((GRAPHICS_REAL)5)

// The anchor-point of the stroke graph
#define STROKE_FRAME_X_MINIMUM             ((GRAPHICS_UINT)0)
#define STROKE_FRAME_Y_MINIMUM             ((GRAPHICS_UINT)0)

// The MINIMAL space division of a character frame
#define STROKE_FRAME_X_MINIMUM_POINTS_UINT ((GRAPHICS_UINT)3)
#define STROKE_FRAME_Y_MINIMUM_POINTS_UINT ((GRAPHICS_UINT)3) 
#define STROKE_FRAME_X_MINIMUM_POINTS_REAL ((GRAPHICS_REAL)3.0)
#define STROKE_FRAME_Y_MINIMUM_POINTS_REAL ((GRAPHICS_REAL)3.0) 

// The NOMINAL space division of a character frame
#define STROKE_FRAME_X_AXIS_POINTS         ((GRAPHICS_REAL)16.0) // 'x' = 0.0 --> point 0, 'x' = 1.0 --> point 15
#define STROKE_FRAME_Y_AXIS_POINTS         ((GRAPHICS_REAL)16.0) // 'y' = 0.0 --> point 0, 'y' = 1.0 --> point 15

#define STROKE_FRAME_BACKGROUND_RED_SHIFT     (0)
#define STROKE_FRAME_BACKGROUND_GREEN_SHIFT   (8)
#define STROKE_FRAME_BACKGROUND_BLUE_SHIFT    (16)
#define STROKE_FRAME_BACKGROUND_A_SHIFT       (24)

/* The stroke graph frame background is grey... */
#define FRAME_BACKGROUND_COLOUR_RED           (0xa5)
#define FRAME_BACKGROUND_COLOUR_GREEN         (0xa6)
#define FRAME_BACKGROUND_COLOUR_BLUE          (0xa8)
#define FRAME_BACKGROUND_COLOUR_A             (0xff)

#define STROKE_FRAME_BACKGROUND_COLOUR_RED    (FRAME_BACKGROUND_COLOUR_RED   << STROKE_FRAME_BACKGROUND_RED_SHIFT)
#define STROKE_FRAME_BACKGROUND_COLOUR_GREEN  (FRAME_BACKGROUND_COLOUR_GREEN << STROKE_FRAME_BACKGROUND_GREEN_SHIFT)
#define STROKE_FRAME_BACKGROUND_COLOUR_BLUE   (FRAME_BACKGROUND_COLOUR_BLUE  << STROKE_FRAME_BACKGROUND_BLUE_SHIFT)
#define STROKE_FRAME_BACKGROUND_COLOUR_A      (FRAME_BACKGROUND_COLOUR_A     << STROKE_FRAME_BACKGROUND_A_SHIFT)

#define STROKE_FRAME_BACKGROUND_COLOUR        (STROKE_FRAME_BACKGROUND_COLOUR_RED   | \
                                               STROKE_FRAME_BACKGROUND_COLOUR_GREEN | \
                                               STROKE_FRAME_BACKGROUND_COLOUR_BLUE  | \
                                               STROKE_FRAME_BACKGROUND_COLOUR_A)

#define STROKE_FRAME_COLOUR                   BLUE_PEN // DO NOT ENCLOSE THIS MACRO IN BRACKETS '('..')' OR '{'..'}'
#define STROKE_FRAME_COLOUR_CODE              ((GRAPHICS_UINT)BLUE_PEN_CODE)

#define MIDDLE_FRAME_COLOUR                   WHITE_PEN_OPAQUE
#define MIDDLE_FRAME_COLOUR_CODE              ((GRAPHICS_UINT)WHITE_PEN_OPAQUE_CODE)

#define DESCENT_FRAME_COLOUR                  YELLOW_PEN
#define DESCENT_FRAME_COLOUR_CODE             ((GRAPHICS_UINT)YELLOW_PEN_CODE)

#define DESCENT_ROW_PROPORTION                ((GRAPHICS_REAL)0.8)

#define DASH_PATTERN_FILLED                   ((REAL)10) // gdiplus::REAL
#define DASH_PATTERN_EMPTY                    ((REAL)10)
#define DASH_PATTERN_STYLE                    (4) // two elements define the dash pattern for the frame "middle" lines

#pragma pack(push,1)
typedef struct strokeFrame_tTag
  {
  const strokeFrameCoordinates_t frameCoordinates;  // the frame as drawn on the canvas in x- and y-axis multipliers from 0.0..1.0
        objectColour_t           strokeColour;
        GRAPHICS_UINT            strokeColourCode;
        objectColour_t           middleColour;
        GRAPHICS_UINT            middleColourCode;
        objectColour_t           descentColour;
        GRAPHICS_UINT            descentColourCode;
        strokeCell_tPtr          strokeCellArray;
        GRAPHICS_REAL            strokeXAxisPoints; // the number of stroke lines in 'x'
        GRAPHICS_REAL            strokeYAxisPoints; // the number of stroke lines in 'y'
  } strokeFrame_t, *strokeFrame_tPtr;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct newLineSegment_tTag
  {
  GRAPHICS_UINT lineSegmentOriginX;
  GRAPHICS_UINT lineSegmentOriginY;
  GRAPHICS_UINT lineSegmentDestinationX;
  GRAPHICS_UINT lineSegmentDestinationY;
  bool          lineSegmentChanged;
  } newLineSegment_t, *newLineSegment_tPtr;
#pragma pack(pop)

/******************************************************************************/

extern graphicsError_t computeCanvasCentre(canvasDescriptor_tPtr canvasSize,
                                           singleIndexReal_tPtr  canvasCentre);

extern graphicsError_t setCanvasCoordinates(canvasDescriptor_tPtr canvasSize,
                                            GRAPHICS_REAL         left,
                                            GRAPHICS_REAL         top,
                                            GRAPHICS_REAL         right,
                                            GRAPHICS_REAL         bottom,
                                            GRAPHICS_REAL         width,
                                            GRAPHICS_REAL         height);

extern VOID OnPaint(HDC                           hdc,
                    HWND                          hWnd,
                    characterSelectionState_tPtr  characterSelectionState,
                    canvasDescriptor_tPtr         canvasSize,
                    canvasDescriptor_tPtr         localCanvasSize,
                    strokeFrame_tPtr              characterFrame,
                    captionDescriptor_tPtr        caption,
                    alphabetCharacters_tPtr       characterList,
                    strokeCell_tPtr               strokeCells,
                    strokeDrawingState_tPtr       strokeDrawingState,
                    GRAPHICS_SHORT_PTR            mouseXPosition,
                    GRAPHICS_SHORT_PTR            mouseYPosition,
                    strokeNavStates_tPtr          strokeNavigationState,
                    strokeNavKeyStates_tPtr       strokeActiveCursor,
                    strokeCursor_tPtr             strokeStartCursor,
                    strokeCursor_tPtr             strokeEndCursor,
                    newLineSegment_tPtr           lineSegment,
                    alphabetFileDescriptor_tPtr   alphabetFileDescriptor,
                    strokeGraphPointBase_tPtr    strokeGraphBase);

extern graphicsError_t buildCharacterStrokeGraph(strokeFrame_tPtr          characterFrame,
                                                 strokeGraphPointBase_tPtr strokeGraphBase);
extern graphicsError_t drawCharacterStrokeGraph(HDC                       hdc,
                                                objectColour_tPtr         strokeColour,
                                                objectColour_tPtr         middleColour,
                                                objectColour_tPtr         descentColour,
                                                canvasDescriptor_tPtr     canvasSize,
                                                strokeGraphPointBase_tPtr strokeGraphBase);

extern graphicsError_t buildStrokeFrame(strokeFrame_tPtr strokeFrame);

extern graphicsError_t drawRectangle(HDC                   hdc,
                                     rectangleObject_tPtr  graphicsObject,
                                     canvasDescriptor_t   *canvasSize);

extern graphicsError_t detectStrokeGraphicCharacter(GRAPHICS_UCHAR keyCode);

extern graphicsError_t drawLineSegment(      HDC                       hdc,
                                       const lineSegment_tPtr          newLineSegment,
                                       const lineSegmentMode_t         newLineSegmentMode,
                                       const strokeFrame_tPtr          characterFrame,
                                       const canvasDescriptor_tPtr     canvasSize,
                                       const strokeGraphPointBase_tPtr strokeGraphPoints);

extern graphicsError_t handleStrokeCursors(      strokeNavStates_tPtr          strokeNavState,
                                           const strokeDrawingState_tPtr       strokeDrawingState,
                                                 characterSelectionState_tPtr selectedState,
                                                 alphabetCharacters_tPtr      characterList,
                                           const strokeNavKeyStates_tPtr      strokeActiveCursor,
                                                 strokeCursor_tPtr            strokeStartCursor,
                                                 strokeCursor_tPtr            strokeEndCursor,
                                           const strokeGraphPointBase_tPtr    strokeGraphPoints,
                                                 newLineSegment_tPtr          lineSegmen);

extern graphicsError_t drawStrokeCursors(      HDC                       hdc,
                                               strokeNavStates_tPtr      strokeNavState,
                                         const strokeFrame_tPtr          characterFrame,
                                         const strokeDrawingState_tPtr   strokeDrawingState,
                                         const strokeNavKeyStates_tPtr   strokeActiveCursor,
                                               strokeCursor_tPtr         strokeStartCursor,
                                               strokeCursor_tPtr         strokeEndCursor,
                                         const canvasDescriptor_tPtr     canvasSize,
                                         const strokeGraphPointBase_tPtr strokeGraphPoints,
                                               newLineSegment_tPtr       lineSegment);


extern graphicsError_t moveCharacterInFrame(      alphabetCharacters_tPtr  *selectedCharacter,
                                                  alphabetCharacters_tPtr   alphabetCharacters,
                                                  strokeNavStates_tPtr      navigationKeyState,
                                            const strokeGraphPointBase_tPtr strokeGraphBase);

/******************************************************************************/

extern HBRUSH                       strokeFrameBackgroundColour;
                                    
extern BYTE                         keyboardState[KEYBOARD_STATES];
extern translatedCharacterBuffer_t  translatedCharacters;
extern translatedCharacterState_t   translatedCharacterState;

extern GRAPHICS_USHORT              actualCode;
extern GRAPHICS_BOOLEAN             characterActive;
extern GRAPHICS_BOOLEAN             characterShiftFlag;
extern GRAPHICS_BOOLEAN             cacheExtantFlag;
                                    
extern GRAPHICS_UINT                alphabetSaveCounter;
extern GRAPHICS_UINT                copyCharacterBufferIndex;
                                    
extern GRAPHICS_USHORT              copyCharacterBuffer[COPY_CHARACTER_BUFFER_LENGTH];

extern canvasDescriptor_t           canvasSize;
extern canvasDescriptor_t           localCanvasSize;
                                    
extern bool                         mouseOverObject;
extern bool                         objectPositionTestFlag;
extern GRAPHICS_SHORT               mouseXPosition;
extern GRAPHICS_SHORT               mouseYPosition;
                                    
extern characterSelectionState_t    characterSelectionState;
                                    
extern alphabetCharacters_tPtr      characterReference;
                                    
extern strokeCellPoint_tPtr         cellPoint;
extern strokeCellPoint_tPtr         lastCellPoint;
                                    
extern strokeCell_tPtr              alphabetStrokeCell;
extern strokeCell_tPtr              lastStrokeCell;
                                    
extern strokeDrawingState_t         strokeDrawingState;
                                    
extern strokeGraphPointBase_t       strokeGraphPointBase;
                                 
extern strokeGraphToggleState_t     strokeGraphToggleState;

extern captionDescriptor_t          mainCaptionFrame;;
                                    
extern strokeFrame_t                characterFrame;
extern strokeGraphLinePoint_t       strokeGraphPoints;
                                    
extern newLineSegment_t             lineSegment;
extern lineSegment_tPtr             nextLineSegment;
                                    
extern strokeNavKeyStates_t         strokeActiveCursor;
extern strokeNavStates_t            strokeNavigationState;
                                    
extern strokeCursor_t               strokeStartCursor;
extern strokeCursor_t               strokeEndCursor;
                                    
extern rectangleObject_t            lineSegmentOrigin;
extern rectangleObject_t            lineSegmentDestination;
                                    
extern colourPalette_t              colourPalette[LINE_SEGMENT_COLOURS];

/******************************************************************************/

#endif

/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/