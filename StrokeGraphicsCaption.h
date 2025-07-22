/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/
/*                                                                            */
/* StrokeGraphicsCaption.h :                                                  */
/* Paul O'Brien                                                               */
/* 05.06.25                                                                   */
/*                                                                            */
/* 2D GUI to create stroke/vector graphic alphanumeric set                    */
/******************************************************************************/

#ifndef _STROKE_GRAPHICS_CAPTION_H_
#define _STROKE_GRAPHICS_CAPTION_H_

/******************************************************************************/

#include "StrokeGraphicsTypes.h"
#include "StrokeGraphicsUtilities.h"
#include "StrokeGraphics001.h"

/******************************************************************************/

#define MESSAGE_BOX_ERROR_STATEMENT           L" Error : usage is \n\n  StrokeGraphics.exe\n  <FrameXUnits[5..n]>\n  <FrameYUnits>[5..m]>\n  <AlphabetLoadNumber>[0..inf]>\n  <AlphabetSaveNumber>[0..inf]>\n  <AlphabetModification [0|1]>"
#define MESSAGE_BOX_ERROR_CAPTION             L"StrokeGraphics : "
#define MESSAGE_BOX_ERROR_BEHAVIOUR            (MB_OK)
 
#define MESSAGE_BOX_FILE_LOAD_ERROR_STATEMENT L" Error : the requested load file index is not recognised"
#define MESSAGE_BOX_FILE_SAVE_ERROR_STATEMENT L" Error : the requested save file index has not been generated"

#define MESSAGE_BOX_INTERNAL_MEMORY_ERROR     L" Error : an internal memory operation has failed"

// The NOMINAL main caption frame coordinates
#define MAIN_CAPTION_WIDTH_OFFSET             ((GRAPHICS_REAL)0.05)
#define MAIN_CAPTION_WIDTH                    ((GRAPHICS_REAL)0.25)
#define MAIN_CAPTION_DEPTH                    STROKE_FRAME_DEPTH
                                              
#define MAIN_CAPTION_FRAME_LEFT_X             (STROKE_FRAME_RIGHT_X      + ((GRAPHICS_REAL)0.05))
#define MAIN_CAPTION_FRAME_TOP_Y              STROKE_FRAME_TOP_Y
#define MAIN_CAPTION_FRAME_RIGHT_X            (MAIN_CAPTION_FRAME_LEFT_X + MAIN_CAPTION_WIDTH)
#define MAIN_CAPTION_FRAME_BOTTOM_Y           (STROKE_FRAME_TOP_Y        + MAIN_CAPTION_DEPTH)
                                              
#define CAPTION_MAIN_TEXT                     ((GRAPHICS_WCHAR *)L" Stroke Graphics : ")
#define CAPTION_MAIN_TEXT_HIGHLIGHT           ((GRAPHICS_WCHAR *)L" --------------------------")
#define CAPTION_NEXT_LINE                     ((GRAPHICS_WCHAR *)L"\n")
                                              
#define CAPTION_LOAD_ALPHABET_NUMBER          ((GRAPHICS_WCHAR *)L" Load Alphabet Number : ")
#define CAPTION_SAVE_ALPHABET_NUMBER          ((GRAPHICS_WCHAR *)L" Save Alphabet Number : ")
                                              
#define CAPTION_EDIT_STATE                    ((GRAPHICS_WCHAR *)L" Character Edit State [ F5-On | F6-Off ] : ")
#define CAPTION_EDIT_STATE_OFF                ((GRAPHICS_WCHAR *)L"Off")
#define CAPTION_EDIT_STATE_ON_DRAWING         ((GRAPHICS_WCHAR *)L"On-Drawing")
#define CAPTION_EDIT_STATE_ON_MOVING          ((GRAPHICS_WCHAR *)L"On-Moving")
                                              
#define CAPTION_SELECTED_CHARACTER            ((GRAPHICS_WCHAR *)L" Selected Character : ")
                                              
#define CAPTION_CURSOR_PAIRING                ((GRAPHICS_WCHAR *)L" Cursor Endpoint Pairing Toggle [ F1 ] : ")
#define CAPTION_CURSOR_PAIRED                 ((GRAPHICS_WCHAR *)L"Paired")
#define CAPTION_CURSOR_SEPERATE               ((GRAPHICS_WCHAR *)L"Seperate")

#define CAPTION_GRAPH_EXTENT_TOP_LEFT_X       ((GRAPHICS_WCHAR *)L" Graph Extent TopLeft     { x : ")
#define CAPTION_GRAPH_EXTENT_TOP_LEFT_Y       ((GRAPHICS_WCHAR *)L", y : ")
#define CAPTION_GRAPH_EXTENT_BOTTOM_RIGHT_X   ((GRAPHICS_WCHAR *)L" Graph Extent BottomRight { x : ")
#define CAPTION_GRAPH_EXTENT_BOTTOM_RIGHT_Y   ((GRAPHICS_WCHAR *)L", y : ")
                                              
#define CAPTION_CHARACTER_EXTENT_TOP_LEFT_X   ((GRAPHICS_WCHAR *)L" Character Extent TopLeft     { x : ")
#define CAPTION_CHARACTER_EXTENT_TOP_LEFT_Y   ((GRAPHICS_WCHAR *)L", y : ")
#define CAPTION_EXTENT_SEPERATOR              ((GRAPHICS_WCHAR *)L", ")
#define CAPTION_EXTENT_BOTTOM_RIGHT_X         ((GRAPHICS_WCHAR *)L" Character Extent BottomRight { x : ")
#define CAPTION_EXTENT_BOTTOM_RIGHT_Y         ((GRAPHICS_WCHAR *)L", y : ")
#define CAPTION_EXTENT_TERMINATOR             ((GRAPHICS_WCHAR *)L" }")
                                              
#define CAPTION_START_CURSOR_X_INDEX          ((GRAPHICS_WCHAR *)L" Start Cursor X Index : ")
#define CAPTION_START_CURSOR_Y_INDEX          ((GRAPHICS_WCHAR *)L" Start Cursor Y Index : ")
#define CAPTION_END_CURSOR_X_INDEX            ((GRAPHICS_WCHAR *)L" End   Cursor X Index : ")
#define CAPTION_END_CURSOR_Y_INDEX            ((GRAPHICS_WCHAR *)L" End   Cursor Y Index : ")
                                           
#define CAPTION_NUMBER_OF_CHARACTER_SEGMENTS  ((GRAPHICS_WCHAR *)L" Number of Segments : ")
#define CAPTION_CURRENT_CHARACTER_SEGMENT     ((GRAPHICS_WCHAR *)L" Current Segment    : ")

#define SEGMENT_MODIFICATION_STATE            ((GRAPHICS_WCHAR *)L" Modification State : [ F7-On | F8-Off ] : ")
#define SEGMENT_MODIFICATION_STATE_NONE       ((GRAPHICS_WCHAR *)L"None")
#define SEGMENT_MODIFICATION_STATE_ON         ((GRAPHICS_WCHAR *)L"On")
#define SEGMENT_MODIFICATION_STATE_OFF        ((GRAPHICS_WCHAR *)L"Off")
#define SEGMENT_MODIFICATION_STATE_UNKNOWN    ((GRAPHICS_WCHAR *)L"Unknown")
                                            
#define CAPTION_LINE_SEGMENT_INSERT_MODIFY_OFF ((GRAPHICS_WCHAR *)L"  State : Off - INS == Line Segment Insert")
#define CAPTION_CHARACTER_INSERT_MODIFY_OFF    ((GRAPHICS_WCHAR *)L"  State : Off - F6  == Recover Character From Cache")

#define CAPTION_LINE_SEGMENT_DELETE_MODIFY_ON ((GRAPHICS_WCHAR *)L"  State : On  - DEL == Line Segment Delete")
#define CAPTION_CHARACTER_DELETE_MODIFY_ON    ((GRAPHICS_WCHAR *)L"  State : On  - F3  == Character Delete")

#define CAPTION_ALPHABET_SAVE_COUNT_PREFIX   ((GRAPHICS_WCHAR *)L" Alphabet Updates This Session : ")
#define CAPTION_ALPHABET_SAVE_COUNT_POSTFIX  ((GRAPHICS_WCHAR *)L"")
                                            
#define CAPTION_COPY_A_TO_B_PREFIX           ((GRAPHICS_WCHAR *)L" Copy Characters A to B [ F9-Copy | F11-Cancel ] : ")
#define CAPTION_COPY_CHARACTERS_SOURCE       ((GRAPHICS_WCHAR *)L"  Copy From ")
#define CAPTION_COPY_CHARACTERS_DESTINATION  ((GRAPHICS_WCHAR *)L" To ")
                                            
#define CAPTION_FLIP_CHARACTER               ((GRAPHICS_WCHAR *)L" Flip Character Around The MidLine : ")
#define CAPTION_FLIP_CHARACTER_VERTICALLY    ((GRAPHICS_WCHAR *)L"  Vertically   : PgUp")
#define CAPTION_FLIP_CHARACTER_HORIZONTALLY  ((GRAPHICS_WCHAR *)L"  Horizontally : PgDown")

#define CAPTION_TEXT_LENGTH(a)               ((size_t)(wcslen(a) + 1))
                                             
#define MAXIMUM_CAPTION_TEXT_LENGTH          (4096)
#define MAXIMUM_CAPTION_SUBTEXT_LENGTH       (64)
                                             
#define CAPTION_COLOUR_RED_SHIFT             (0)
#define CAPTION_COLOUR_GREEN_SHIFT           (8)
#define CAPTION_COLOUR_BLUE_SHIFT            (CAPTION_COLOUR_GREEN_SHIFT << 1)
#define CAPTION_COLOUR_OPACITY_SHIFT         (CAPTION_COLOUR_BLUE_SHIFT  + CAPTION_COLOUR_GREEN_SHIFT)

/* The caption colour is a shade of purple, constants derived from MS Paint   */
/* "Edit Colo(u)rs"                                                           */
#define CAPTION_COLOUR_OPACITY             (255)
#define CAPTION_COLOUR_RED                 (105)
#define CAPTION_COLOUR_GREEN               ( 36)
#define CAPTION_COLOUR_BLUE                (219)
                                          
#define CAPTION_COLOUR                     ((((GRAPHICS_UINT)CAPTION_COLOUR_RED)   << CAPTION_COLOUR_RED_SHIFT)     | \
                                            (((GRAPHICS_UINT)CAPTION_COLOUR_GREEN) << CAPTION_COLOUR_GREEN_SHIFT)   | \
                                            (((GRAPHICS_UINT)CAPTION_COLOUR_BLUE)  << CAPTION_COLOUR_BLUE_SHIFT))

#define CAPTION_FOREGROUND_COLOUR_PEN      ((BYTE)CAPTION_COLOUR_OPACITY),\
                                           ((BYTE)CAPTION_COLOUR_RED),    \
                                           ((BYTE)CAPTION_COLOUR_GREEN),  \
                                           ((BYTE)CAPTION_COLOUR_BLUE)

/* The caption background colour is a light shade of blue */
#define CAPTION_BACKGROUND_COLOUR_RED      (187)
#define CAPTION_BACKGROUND_COLOUR_GREEN    (224)
#define CAPTION_BACKGROUND_COLOUR_BLUE     (249)

#define CAPTION_BACKGROUND_COLOUR          ((((GRAPHICS_UINT)CAPTION_BACKGROUND_COLOUR_RED)   << CAPTION_COLOUR_RED_SHIFT)     | \
                                            (((GRAPHICS_UINT)CAPTION_BACKGROUND_COLOUR_GREEN) << CAPTION_COLOUR_GREEN_SHIFT)   | \
                                            (((GRAPHICS_UINT)CAPTION_BACKGROUND_COLOUR_BLUE)  << CAPTION_COLOUR_BLUE_SHIFT))

#define CAPTION_BACKGROUND_COLOUR_PEN      ((BYTE)CAPTION_COLOUR_OPACITY), \
                                           ((BYTE)CAPTION_BACKGROUND_COLOUR_RED),     \
                                           ((BYTE)CAPTION_BACKGROUND_COLOUR_GREEN),   \
                                           ((BYTE)CAPTION_BACKGROUND_COLOUR_BLUE) 

/******************************************************************************/

extern GRAPHICS_USHORT actualCode;
extern GRAPHICS_WCHAR  captionText[MAXIMUM_CAPTION_TEXT_LENGTH];
extern GRAPHICS_WCHAR  localText[MAXIMUM_CAPTION_SUBTEXT_LENGTH];

/******************************************************************************/

extern graphicsError_t mainCaption(      HDC                          hdc,
                                         GRAPHICS_WCHAR_PTR           captionMainText,
                                         GRAPHICS_WCHAR_PTR           captionLocalText,
                                   const strokeGraphPointBase_tPtr    strokeGraphBase,
                                   const strokeNavKeyStates_tPtr      strokeActiveCursor,
                                   const strokeCursor_tPtr            strokeStartCursor,
                                   const strokeCursor_tPtr            strokeEndCursor,
                                   const canvasDescriptor_tPtr        canvasSize,
                                   const alphabetCharacters_tPtr      selectedCharacter,
                                   const characterSelectionState_tPtr characterSelectionState,
                                   const alphabetFileDescriptor_tPtr  alphabetFileDescriptor,
                                   const GRAPHICS_UINT                alphabetSaveCounter,
                                   const captionDescriptor_tPtr       caption);

/******************************************************************************/

#endif

/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/
