/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/
/*                                                                            */
/* StrokeGraphicsCaption.cpp :                                                */
/* Paul O'Brien                                                               */
/* 05.06.25                                                                   */
/*                                                                            */
/* 2D GUI to create stroke/vector graphic alphanumeric set                    */
/******************************************************************************/

#include <Windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <stdio.h>
#include <strsafe.h>
#include <math.h>
#include "StrokeGraphicsAlphabet.h"
#include "StrokeGraphicsCaption.h"
#include "StrokeGraphicsListHandlers.h"

/******************************************************************************/

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

/******************************************************************************/

GRAPHICS_WCHAR           captionText[MAXIMUM_CAPTION_TEXT_LENGTH]    = { L"" },
                         localText[MAXIMUM_CAPTION_SUBTEXT_LENGTH]   = { L"" };
                         
alphabetFileDescriptor_t alphabetFileDescriptor                      = {
                                                                        ((GRAPHICS_ULONG)0),
                                                                        ((GRAPHICS_ULONG)0),
                                                                        STROKE_GRAPHICS_MODIFICATION_FLAG_NONE,
                                                                       &alphabetFilePath[0],
                                                                       &alphabetLineW[0]
                                                                       };

GRAPHICS_WCHAR           alphabetFilePath[ALPHABET_FILEPATH_MAXIMUM] = { L"" };
                         
captionDescriptor_t      mainCaptionFrame                            = {
                                                                       MAIN_CAPTION_FRAME_LEFT_X,
                                                                       MAIN_CAPTION_FRAME_TOP_Y,
                                                                       MAIN_CAPTION_FRAME_RIGHT_X,
                                                                       MAIN_CAPTION_FRAME_BOTTOM_Y
                                                                       };

/******************************************************************************/
/*  mainCaption() :                                                           */
/******************************************************************************/

graphicsError_t mainCaption(      HDC                          hdc,
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
                            const captionDescriptor_tPtr       caption)
  {
/******************************************************************************/

  RECT              captionRectangle    = { ((LONG)0), ((LONG)0), ((LONG)0), ((LONG)0) };

  rectangleObject_t backgroundRectangle = { 
                                            {
                                            CAPTION_FOREGROUND_COLOUR_PEN,
                                            },
                                            {
                                            CAPTION_BACKGROUND_COLOUR_PEN
                                            },
                                            GRAPHICS_OBJECT_FILL_STATE_FULL,
                                            {
                                            ((GRAPHICS_REAL)0.0),
                                            ((GRAPHICS_REAL)0.0),
                                            ((GRAPHICS_REAL)0.0),
                                            ((GRAPHICS_REAL)0.0),
                                            ((GRAPHICS_UINT)1)
                                            }
                                          };

  GRAPHICS_INT      midCharacterFrameX  = ((GRAPHICS_INT)0),
                    midCharacterFrameY  = ((GRAPHICS_INT)0);

  graphicsError_t   objectError         = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((strokeStartCursor != nullptr) && (strokeEndCursor != nullptr) && 
      (canvasSize        != nullptr) && (caption         != nullptr) &&
      (strokeGraphBase   != nullptr))
    {
    if ((canvasSize->left  != canvasSize->right)   && (canvasSize->top    != canvasSize->bottom) &&
        (canvasSize->width != ((GRAPHICS_REAL)0.0) && (canvasSize->height != ((GRAPHICS_REAL)0.0))))
      {
      if ((caption->captionFrameLeftX != caption->captionFrameRightX) && 
          (caption->captionFrameTopY  != caption->captionFrameBottomY))
        {
        midCharacterFrameX  = strokeGraphBase->graphRowNumber    >> 1;
        midCharacterFrameY  = strokeGraphBase->graphColumnNumber >> 1;

        if ((midCharacterFrameX > ((GRAPHICS_INT)0)) && (midCharacterFrameY > ((GRAPHICS_INT)0)))
          {
          swprintf_s(captionText, L"%s", CAPTION_NEXT_LINE);
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_MAIN_TEXT);
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);
          
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_MAIN_TEXT_HIGHLIGHT);
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);
          
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_LOAD_ALPHABET_NUMBER);
          wsprintf(captionLocalText, L"%03d", alphabetFileDescriptor->alphabetLoadNumber);
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), captionLocalText);
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);
          
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_SAVE_ALPHABET_NUMBER);
          wsprintf(captionLocalText, L"%03d", alphabetFileDescriptor->alphabetSaveNumber);
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), captionLocalText);
          
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);

          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_GRAPH_EXTENT_TOP_LEFT_X);
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), L"000");
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_GRAPH_EXTENT_TOP_LEFT_Y);
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), L"000");
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_EXTENT_TERMINATOR);

          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);

          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_GRAPH_EXTENT_BOTTOM_RIGHT_X);
          wsprintf(captionLocalText, L"%03d", strokeGraphBase->graphRowNumber);
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), captionLocalText);
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_GRAPH_EXTENT_BOTTOM_RIGHT_Y);
          wsprintf(captionLocalText, L"%03d", strokeGraphBase->graphColumnNumber);
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), captionLocalText);
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_EXTENT_TERMINATOR);

          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);

          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_EDIT_STATE);

          if ((characterSelectionState->lastSelectionState == CHARACTER_SELECTION_STATE_SELECTED) && 
              (characterSelectionState->thisSelectionState == CHARACTER_SELECTION_STATE_SELECTED))
            {
            if (characterSelectionState->strokeCursorToggleState == STROKE_CURSOR_TOGGLE_STATE_NOT_SELECTED)
              {
              wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_EDIT_STATE_ON_DRAWING);
              }
            else
              {
              wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_EDIT_STATE_ON_MOVING);
              }
            }
          else
            {
            wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_EDIT_STATE_OFF);
            }

          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);

          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_SELECTED_CHARACTER);

          if (characterSelectionState->selectedCharacter == GRAPHICS_AMPERSAND)
            {
            wsprintf(captionLocalText, L"0x%02x --> &&", (GRAPHICS_USHORT)characterSelectionState->selectedCharacter);
            }
          else
            {
            wsprintf(captionLocalText, L"0x%02x --> %c", (GRAPHICS_USHORT)characterSelectionState->selectedCharacter, (GRAPHICS_WCHAR)characterSelectionState->selectedCharacter);
            }

          //StringCchPrintfW(captionLocalText, ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), L"0x%02x --> %C", (GRAPHICS_USHORT)characterSelectionState->selectedCharacter, (GRAPHICS_WCHAR)characterSelectionState->selectedCharacter);

          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), captionLocalText);

          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE); 
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE); 

          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_CURSOR_PAIRING);  

          if ((strokeNavKeyStates_t)((GRAPHICS_UINT)(*strokeActiveCursor) & ((GRAPHICS_UINT)STROKE_NAV_KEY_STATE_LINE_BOTH_START_LEADING)))
            {
            wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_CURSOR_PAIRED); 
            }
          else
            {
            wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_CURSOR_SEPERATE);
            }
  
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE); 
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE); 

          if (selectedCharacter != nullptr)
            {
            wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_CHARACTER_EXTENT_TOP_LEFT_X);
            wsprintf(captionLocalText, L"%03d", selectedCharacter->characterExtents.topLeft.pointX);
            wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), captionLocalText);
            wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_CHARACTER_EXTENT_TOP_LEFT_Y);
            wsprintf(captionLocalText, L"%03d", selectedCharacter->characterExtents.topLeft.pointY);
            wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), captionLocalText);
            wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_EXTENT_TERMINATOR);
            }
          else
            {
            wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_CHARACTER_EXTENT_TOP_LEFT_X);
            wsprintf(captionLocalText, L"000");
            wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), captionLocalText);
            wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_CHARACTER_EXTENT_TOP_LEFT_Y);
            wsprintf(captionLocalText, L"000");
            wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), captionLocalText);
            wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_EXTENT_TERMINATOR);
            }
          
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);
          
          if (selectedCharacter != nullptr)
            {
            wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_EXTENT_BOTTOM_RIGHT_X);
            wsprintf(captionLocalText, L"%03d", selectedCharacter->characterExtents.bottomRight.pointX);
            wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), captionLocalText);
            wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_EXTENT_BOTTOM_RIGHT_Y);
            wsprintf(captionLocalText, L"%03d", selectedCharacter->characterExtents.bottomRight.pointY);
            wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), captionLocalText);
            wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_EXTENT_TERMINATOR);
            }
          else
            {
            wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_EXTENT_BOTTOM_RIGHT_X);
            wsprintf(captionLocalText, L"000");
            wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), captionLocalText);
            wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_EXTENT_BOTTOM_RIGHT_Y);
            wsprintf(captionLocalText, L"000");
            wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), captionLocalText);
            wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_EXTENT_TERMINATOR);
            }
          
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);
          
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_START_CURSOR_X_INDEX);
          wsprintf(captionLocalText, L"%03d", strokeStartCursor->strokeCursorIndexLit.xAxisIndex);
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), captionLocalText);
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);
          
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_START_CURSOR_Y_INDEX);
          wsprintf(captionLocalText, L"%03d", strokeStartCursor->strokeCursorIndexLit.yAxisIndex);
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), captionLocalText);
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE); 
          
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_END_CURSOR_X_INDEX);
          wsprintf(captionLocalText, L"%03d", strokeEndCursor->strokeCursorIndexLit.xAxisIndex);
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), captionLocalText);
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);
          
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_END_CURSOR_Y_INDEX);
          wsprintf(captionLocalText, L"%03d", strokeEndCursor->strokeCursorIndexLit.yAxisIndex);
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), captionLocalText);
          
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE); 
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);
          
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NUMBER_OF_CHARACTER_SEGMENTS);
          
          if (selectedCharacter == nullptr)
            {
            wsprintf(captionLocalText, L"000");
            }
          else
            {
            wsprintf(captionLocalText, L"%03d", selectedCharacter->numberOfLineSegments);
            }
                      
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), captionLocalText);
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);
          
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_CURRENT_CHARACTER_SEGMENT);
          
          if (selectedCharacter == nullptr)
            {
            wsprintf(captionLocalText, L"000");
            }
          else
            {
            wsprintf(captionLocalText, L"%03d", selectedCharacter->lineSegmentIndex);
            }
          
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), captionLocalText);
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE); 
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE); 
          
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), SEGMENT_MODIFICATION_STATE);
          
          switch(characterSelectionState->modificationState)
            {
            case CHARACTER_SELECTION_STATE_MODIFY_NONE : wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), SEGMENT_MODIFICATION_STATE_OFF);
                                                         break;
            case CHARACTER_SELECTION_STATE_MODIFY_ON   : wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), SEGMENT_MODIFICATION_STATE_ON);
                                                         break;
            case CHARACTER_SELECTION_STATE_MODIFY_OFF  : wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), SEGMENT_MODIFICATION_STATE_OFF);
                                                         break;
            default                                    : wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), SEGMENT_MODIFICATION_STATE_UNKNOWN);
                                                         break;
            }

          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);

          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_LINE_SEGMENT_INSERT_MODIFY_OFF);
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_CHARACTER_INSERT_MODIFY_OFF);

          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);

          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_LINE_SEGMENT_DELETE_MODIFY_ON);
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_CHARACTER_DELETE_MODIFY_ON);

          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);
          
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_ALPHABET_SAVE_COUNT_PREFIX);
          wsprintf(captionLocalText, L"x%03d", alphabetSaveCounter);
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), captionLocalText);

          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);
          
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_COPY_A_TO_B_PREFIX);
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_COPY_CHARACTERS_SOURCE);

          if (copyCharacterBuffer[COPY_CHARACTER_BUFFER_SOURCE] == GRAPHICS_AMPERSAND)
            {
            wsprintf(captionLocalText, L"  &&");
            }
          else
            {
            wsprintf(captionLocalText, L"  %c", (GRAPHICS_USHORT)copyCharacterBuffer[COPY_CHARACTER_BUFFER_SOURCE]);
            }

          // wsprintf(captionLocalText, L"  %c", copyCharacterBuffer[COPY_CHARACTER_BUFFER_SOURCE]);

          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), captionLocalText);
          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_COPY_CHARACTERS_DESTINATION);


          if (copyCharacterBuffer[COPY_CHARACTER_BUFFER_DESTINATION] == GRAPHICS_AMPERSAND)
            {
            wsprintf(captionLocalText, L"  &&");
            }
          else
            {
            wsprintf(captionLocalText, L"  %c", (GRAPHICS_USHORT)copyCharacterBuffer[COPY_CHARACTER_BUFFER_DESTINATION]);
            }

          // wsprintf(captionLocalText, L"  %0c", copyCharacterBuffer[COPY_CHARACTER_BUFFER_DESTINATION]);

          wcscat_s(&captionText[0],  ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), captionLocalText);
          
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);

          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_FLIP_CHARACTER);
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_FLIP_CHARACTER_VERTICALLY);
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);
          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_FLIP_CHARACTER_HORIZONTALLY);

          wcscat_s(&captionText[0], ((size_t)MAXIMUM_CAPTION_TEXT_LENGTH), CAPTION_NEXT_LINE);

          // Refresh the screen caption
          SetBkMode(hdc, OPAQUE);
          SetBkColor(hdc, CAPTION_BACKGROUND_COLOUR);
          //SetDCBrushColor(hdc, CAPTION_BACKGROUND_COLOUR);
          
          SetTextAlign(hdc, TA_TOP | TA_LEFT | TA_NOUPDATECP);
          
          SetTextColor(hdc, CAPTION_COLOUR);
          
          captionRectangle.left   = (LONG)(caption->captionFrameLeftX   * canvasSize->width);
          captionRectangle.top    = (LONG)(caption->captionFrameTopY    * canvasSize->height);
          captionRectangle.right  = (LONG)(caption->captionFrameRightX  * canvasSize->width);
          captionRectangle.bottom = (LONG)(caption->captionFrameBottomY * canvasSize->height);
          
          backgroundRectangle.rectangleDimensions.leftX   = (GRAPHICS_REAL)captionRectangle.left;
          backgroundRectangle.rectangleDimensions.topY    = (GRAPHICS_REAL)captionRectangle.top;
          backgroundRectangle.rectangleDimensions.rightX  = (GRAPHICS_REAL)captionRectangle.right;
          backgroundRectangle.rectangleDimensions.bottomY = (GRAPHICS_REAL)captionRectangle.bottom;
          
          objectError = drawRectangle( hdc,
                                      &backgroundRectangle,
                                       canvasSize);
          
          DrawText( hdc,
                    captionText,
                    (GRAPHICS_INT)CAPTION_TEXT_LENGTH(captionText),
                   &captionRectangle,
                    DT_LEFT | DT_INTERNAL);
          }
        else
          {
          objectError = GRAPHICS_OBJECT_RANGE_ERROR;
          }
        }
      else
        {
        objectError = GRAPHICS_OBJECT_RANGE_ERROR;
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
  } /* end of mainCaption                                                     */

/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/