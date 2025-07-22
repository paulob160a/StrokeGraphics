/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/
/*                                                                            */
/* StrokeGraphics001.cpp :                                                    */
/* Paul O'Brien                                                               */
/* 28.05.25                                                                   */
/*                                                                            */
/* 2D GUI to create stroke/vector graphic alphanumeric set                    */
/******************************************************************************/

#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <errno.h>
#include "framework.h"
#include <objidl.h>
#include <gdiplus.h>
#include <time.h>
#include <shellapi.h>
#include <errno.h>
//#include "StrokeGraphics001.h"
#include "StrokeGraphicsAlphabet.h"
#include "StrokeGraphicsListHandlers.h"
#include "StrokeGraphicsUtilities.h"
#include "StrokeGraphicsCaption.h"
#include "ar19937.h"

/******************************************************************************/

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

/******************************************************************************/

extern "C" 
  {

  }

/******************************************************************************/

// Global Variables:
HINSTANCE                   hInst                          = nullptr;                  // current instance
WCHAR                       appTitle[MAX_LOADSTRING]       = { STROKE_GRAPHICS_TITLE }; // The title bar text
WCHAR                       appWindowClass[MAX_LOADSTRING] = { STROKE_GRAPHICS_CLASS }; // the main window class name
                            
GRAPHICS_CHAR               commandLineParameters[STROKE_GRAPHICS_ARGUMENTS][MAX_LOADSTRING] = { "", "", "" };
                            
HBRUSH                      strokeFrameBackgroundColour    = CreateSolidBrush(COLORREF(STROKE_FRAME_BACKGROUND_COLOUR)); // CreateSolidBrush(RGB(255,255,255)); // CreateSolidBrush(COLORREF(STROKE_FRAME_BACKGROUND_COLOUR));
                            
BYTE                        keyboardState[KEYBOARD_STATES] = { 0 };
translatedCharacterBuffer_t translatedCharacters           = { ((GRAPHICS_SHORT)0) };
translatedCharacterState_t  translatedCharacterState       = { false, false };

GRAPHICS_USHORT             actualCode                     = ((GRAPHICS_USHORT)0);
                                                           
GRAPHICS_BOOLEAN            characterActive                = false,
                            characterShiftFlag             = false,
                            cacheExtantFlag                = false;
                            
GRAPHICS_UINT               alphabetSaveCounter            = ((GRAPHICS_UINT)0),
                            copyCharacterBufferIndex       = ((GRAPHICS_UINT)0);
                            
GRAPHICS_USHORT             copyCharacterBuffer[COPY_CHARACTER_BUFFER_LENGTH] = { ((GRAPHICS_USHORT)0), ((GRAPHICS_USHORT)0) };
                            
strokeGraphPointBase_t      strokeGraphPointBase = {
                                                   ((GRAPHICS_UINT)0),
                                                   ((GRAPHICS_UINT)0),
                                                   nullptr,
                                                   BRIGHT_YELLOW_PEN,
                                                   LIGHT_BLUE_PEN,
                                                   RED_PEN,
                                                   STROKE_LINE_WIDTH
                                                   };
                              
strokeGraphToggleState_t      strokeGraphToggleState = STROKE_GRAPH_TOGGLE_STATE_SELECTED;

/******************************************************************************/

// Forward declarations of functions included in this code module:
ATOM             RegisterGraphicsClass(HINSTANCE hInstance);
BOOL             InitInstance(HINSTANCE, RECT *requiredWindowSize, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

/******************************************************************************/

int APIENTRY wWinMain(_In_     HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_     LPWSTR    lpCmdLine,
                      _In_     int       nCmdShow)
  {
/******************************************************************************/

  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  GdiplusStartupInput gdiPlusStartupInput      = { 0 };
  ULONG_PTR           gdiplusToken             = NULL;
                                               
  __time32_t          theCurrentTime           = 0;
                                               
  HACCEL              hAccelTable              = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_STROKEGRAPHICS001));
  MSG                 msg                      = { 0 };
                                               
  RECT                requiredWindowSize       = {
                                                  GDI_PLUS_WINDOW_RECTANGLE_X_LEFT,
                                                  GDI_PLUS_WINDOW_RECTANGLE_Y_TOP,
                                                  GDI_PLUS_WINDOW_RECTANGLE_X_BOTTOM,
                                                  GDI_PLUS_WINDOW_RECTANGLE_Y_BOTTOM
                                                  };

  RECT                adjustedWindowSize        = {
                                                  ((LONG)0),
                                                  ((LONG)0),
                                                  ((LONG)0),
                                                  ((LONG)0)
                                                  },
                      deltaWindowSize           = {
                                                  ((LONG)0),
                                                  ((LONG)0),
                                                  ((LONG)0),
                                                  ((LONG)0)
                                                  };

  LPWSTR             *commandLineList           = nullptr;
  int                 commandLineArgumentNumber = ((int)0),
                      listIndex                 = ((int)0);
  LPWSTR              commandLineArguments      = nullptr;

  size_t              commandStringLength       = ((size_t)0);
  errno_t             commandStringError        = NO_ERROR;
  int                 commandStringIndex        = ((int)0);

  graphicsError_t     graphicsErrorState        = GRAPHICS_NO_ERROR;

  GRAPHICS_UINT       wideStringLength          = ((GRAPHICS_UINT)0); // for 'wcstod()'
  GRAPHICS_WCHAR_PTR  wideStringEnd             = nullptr;

  GRAPHICS_REAL       commandLineWcharToReal[STROKE_GRAPHICS_ARGUMENTS] = { 
                                                                            ((GRAPHICS_REAL)0.0),
                                                                            ((GRAPHICS_REAL)0.0),
                                                                            ((GRAPHICS_REAL)0.0),
                                                                            ((GRAPHICS_ULONG)0),
                                                                            ((GRAPHICS_ULONG)0),
                                                                            false
                                                                          };

  GRAPHICS_REAL       commandLineDefaults[STROKE_GRAPHICS_ARGUMENTS]    = { 
                                                                            ((GRAPHICS_REAL)0.0), 
                                                                            STROKE_FRAME_X_AXIS_POINTS,
                                                                            STROKE_FRAME_Y_AXIS_POINTS,
                                                                            STROKE_GRAPHICS_ALPHABET_NONE,
                                                                            STROKE_GRAPHICS_ALPHABET_NONE,
                                                                            STROKE_GRAPHICS_MODIFICATION_FLAG_N
                                                                          };

  singlePoint_t       characterFrameDimensions                          = { 
                                                                          ((GRAPHICS_REAL)0.0),
                                                                          ((GRAPHICS_REAL)0.0) 
                                                                          };

  // Convert the 'real' x- and y-axes sizes for indices
  GRAPHICS_UINT       cellSizeXIndex                                    = ((GRAPHICS_UINT)0),
                      cellSizeYIndex                                    = ((GRAPHICS_UINT)0),
                      cellSizeX                                         = ((GRAPHICS_UINT)0),
                      cellSizeY                                         = ((GRAPHICS_UINT)0),
                      cellIndex                                         = ((GRAPHICS_UINT)0),
                      alphabetIndex                                     = ((GRAPHICS_UINT)0);

  graphicsError_t     objectError                                       = GRAPHICS_NO_ERROR;

/******************************************************************************/

  // Initialize GDI+.
  GdiplusStartup(&gdiplusToken, &gdiPlusStartupInput, NULL);

  // Processing the command-line arguments for required number and format 
  // conversion
  commandLineList = CommandLineToArgvW(GetCommandLineW(), &commandLineArgumentNumber);

  if (commandLineList == nullptr)
    {
    exit(0);
    }
  else
    {
    commandLineArguments = (LPWSTR)calloc(((size_t)(commandLineArgumentNumber + 1)), (MAX_LOADSTRING * ((size_t)(sizeof(WCHAR)))));

    // First copy the all the command-line arguments as wide-character strings
    if (commandLineArguments != nullptr)
      {
      for(listIndex = 0; listIndex < commandLineArgumentNumber; listIndex++) 
        {
        wcscpy_s((commandLineArguments + (listIndex * MAX_LOADSTRING)), ((rsize_t)MAX_LOADSTRING), *(commandLineList + listIndex));
        }
      }
    }

  // Now convert the parameter wide-character command-line arguments to 'wchar' form 
  // - but not the file path and name parameter 0
  if (listIndex == STROKE_GRAPHICS_ARGUMENTS)
    {
    for (commandStringIndex = STROKE_GRAPHICS_ARGUMENT_1; commandStringIndex < STROKE_GRAPHICS_ARGUMENTS; commandStringIndex++)
      {
      if ((commandStringError = wcstombs_s(&commandStringLength,
                                            commandLineParameters[commandStringIndex],
                                            MAX_LOADSTRING,
                                            (commandLineArguments + (commandStringIndex * MAX_LOADSTRING)),
                                            MAX_LOADSTRING)) != NO_ERROR)
        {
        MessageBoxW(NULL, MESSAGE_BOX_ERROR_STATEMENT, MESSAGE_BOX_ERROR_CAPTION, MESSAGE_BOX_ERROR_BEHAVIOUR);
        exit(0);
        }
      }
    }
  else
    {
    MessageBoxW(NULL, MESSAGE_BOX_ERROR_STATEMENT, MESSAGE_BOX_ERROR_CAPTION, MESSAGE_BOX_ERROR_BEHAVIOUR);
    exit(0);
    }

  // If the command-line 'x-points', 'y-points', 'alphabet-number' and 'file-modification' arguments are legal, 
  // convert them to 'GRAPHICS_REAL' and then if necessary to other representations. Otherwise use the defaults
  for (commandStringIndex = STROKE_GRAPHICS_FRAME_X_POINTS; commandStringIndex < STROKE_GRAPHICS_ARGUMENTS; commandStringIndex++)
    {
    wideStringLength = (GRAPHICS_UINT)wcslen((commandLineArguments + (commandStringIndex * MAX_LOADSTRING))); // find the end of the command-line parameter

    commandLineWcharToReal[commandStringIndex] = wcstod((commandLineArguments + (commandStringIndex * MAX_LOADSTRING)), &wideStringEnd);

    if (commandStringIndex == STROKE_GRAPHICS_FRAME_X_POINTS)
      {
      if (!errno)
        { // Force the values to integer...
        characterFrame.strokeXAxisPoints = round(commandLineWcharToReal[STROKE_GRAPHICS_FRAME_X_POINTS]);
        }
      else
        {
        characterFrame.strokeXAxisPoints = commandLineDefaults[STROKE_GRAPHICS_FRAME_X_POINTS];
        }
      }

    if (commandStringIndex == STROKE_GRAPHICS_FRAME_Y_POINTS)
      {
      if (!errno)
        { // Force the values to integer...
        characterFrame.strokeYAxisPoints = round(commandLineWcharToReal[STROKE_GRAPHICS_FRAME_Y_POINTS]);
        }
      else
        {
        characterFrame.strokeYAxisPoints = commandLineDefaults[STROKE_GRAPHICS_FRAME_Y_POINTS];
        }
      }

    // This number is used to reload an existing alphabet. If it is zero then a new 
    // alphabet file will be created and given a version number dependent on the current, 
    // saved and version-numbered alphabet files. If it is already in use then : if the 
    // command-line modification flag is set it will be resaved with the same number. If 
    // the command-line modification flag is unset it will be saved with a new version-
    // number
    if (commandStringIndex == STROKE_GRAPHICS_LOAD_ALPHABET_NUMBER)
      {
      alphabetFileDescriptor.alphabetLoadNumber = (GRAPHICS_ULONG)commandLineWcharToReal[commandStringIndex];

      if (errno)
        {
        alphabetFileDescriptor.alphabetLoadNumber = (GRAPHICS_ULONG)commandLineDefaults[STROKE_GRAPHICS_LOAD_ALPHABET_NUMBER];
        }
      }


    if (commandStringIndex == STROKE_GRAPHICS_SAVE_ALPHABET_NUMBER)
      {
      alphabetFileDescriptor.alphabetSaveNumber = (GRAPHICS_ULONG)commandLineWcharToReal[commandStringIndex];
      
      if (errno)
        {
        alphabetFileDescriptor.alphabetSaveNumber = (GRAPHICS_ULONG)commandLineDefaults[STROKE_GRAPHICS_SAVE_ALPHABET_NUMBER];
        }
      }

    if (commandStringIndex == STROKE_GRAPHICS_MODIFICATION_FLAG)
      {
      alphabetFileDescriptor.alphabetModificationFlag = (alphabetFileModifications_t)commandLineWcharToReal[commandStringIndex];
      
      if (errno)
        {
        alphabetFileDescriptor.alphabetModificationFlag = (alphabetFileModifications_t)commandLineDefaults[STROKE_GRAPHICS_MODIFICATION_FLAG];
        }
      }
    }

/******************************************************************************/
/* Load an existing alphabet file if possible                                 */
/******************************************************************************/

  // Build the character list as a set of line segments per character
  graphicsErrorState = buildCharacterList( ALPHABET_SIZE,
                                          &characterListRoot);

  if (alphabetFileDescriptor.alphabetLoadNumber != (GRAPHICS_ULONG)commandLineDefaults[STROKE_GRAPHICS_LOAD_ALPHABET_NUMBER])
      {
      if ((objectError = readAlphabetFromFile(&alphabetFileDescriptor,
                                               characterListRoot,
                                              &characterFrameDimensions)) != GRAPHICS_NO_ERROR)
        {
        MessageBoxW(NULL, MESSAGE_BOX_FILE_LOAD_ERROR_STATEMENT, MESSAGE_BOX_ERROR_CAPTION, MESSAGE_BOX_ERROR_BEHAVIOUR);
        exit(0);
        }
      else
        {
        // Use the character frame dimensions returned from the file
        cellSizeX = (GRAPHICS_UINT)characterFrameDimensions.xAxisPoint;
        cellSizeY = (GRAPHICS_UINT)characterFrameDimensions.yAxisPoint;
        }
      }
    else
      {
      // Use the character frame dimensions entered on the command line
      cellSizeX = (GRAPHICS_UINT)characterFrame.strokeXAxisPoints;
      cellSizeY = (GRAPHICS_UINT)characterFrame.strokeYAxisPoints;
      }

  characterFrame.strokeXAxisPoints = cellSizeX;
  characterFrame.strokeYAxisPoints = cellSizeY;

/******************************************************************************/
/* Instantiate and initialise the alphanumeric character cell array           */
/******************************************************************************/

  // Build the alphabet array - everything is rooted in the variable 'characterFrame'
  for (alphabetIndex = ((GRAPHICS_UINT)0); alphabetIndex < ALPHABET_SIZE; alphabetIndex++)
    {
    if ((alphabetStrokeCell = (strokeCell_tPtr)calloc(((size_t)1), sizeof(strokeCell_t))) != nullptr)
      {
      // Moor the very first alphabet cell
      if (alphabetIndex == ((GRAPHICS_UINT)0))
        {
        characterFrame.strokeCellArray = alphabetStrokeCell;
        }
      else
        { // Link-up the subsequent cells
        lastStrokeCell->nextStrokeCell = alphabetStrokeCell;
        }

      for (cellSizeXIndex = ((GRAPHICS_UINT)0); cellSizeXIndex <= cellSizeX; cellSizeXIndex++)
        {
        for (cellSizeYIndex = ((GRAPHICS_UINT)0); cellSizeYIndex <= cellSizeY; cellSizeYIndex++)
          {
          if ((cellPoint = (strokeCellPoint_tPtr)calloc(((size_t)1), sizeof(strokeCellPoint_t))) != nullptr)
            {
            // Moor the very first point
            if ((cellSizeXIndex == ((GRAPHICS_UINT)0)) && (cellSizeYIndex == ((GRAPHICS_UINT)0)))
              {
              alphabetStrokeCell->strokeCellPoint = cellPoint;
              }
            else
              {
              lastCellPoint->nextStrokeCellPoint = cellPoint;
              }

            // Mark the line starts' and ends'
            if (cellSizeXIndex == ((GRAPHICS_UINT)0))
              {
              if (cellSizeYIndex == ((GRAPHICS_UINT)0))
                {
                cellPoint->pointType = STROKE_CELL_POINT_TYPE_FIRST_LINE_START;
                }
              else
                {
                if (cellSizeYIndex == cellSizeY)
                  {
                  cellPoint->pointType = STROKE_CELL_POINT_TYPE_LINE_END;
                  }
                else
                  {
                  cellPoint->pointType = STROKE_CELL_POINT_TYPE_LINE_MID;
                  }
                }
              }
            else
              {
              if (cellSizeXIndex == cellSizeX)
                {
                if (cellSizeYIndex == ((GRAPHICS_UINT)0))
                  {
                  cellPoint->pointType = STROKE_CELL_POINT_TYPE_LAST_LINE_START;
                  }
                else
                  {
                  if (cellSizeYIndex == cellSizeY)
                    {
                    cellPoint->pointType = STROKE_CELL_POINT_TYPE_LINE_END;
                    }
                  else
                    {
                    cellPoint->pointType = STROKE_CELL_POINT_TYPE_LINE_MID;
                    }
                  }
                }
              else
                {
                if (cellSizeYIndex == ((GRAPHICS_UINT)0))
                  {
                  cellPoint->pointType = STROKE_CELL_POINT_TYPE_LINE_START;
                  }
                else
                  {
                  if (cellSizeYIndex == cellSizeY)
                    {
                    cellPoint->pointType = STROKE_CELL_POINT_TYPE_LINE_END;
                    }
                  else
                    {
                    cellPoint->pointType = STROKE_CELL_POINT_TYPE_LINE_MID;
                    }
                  }
                }
              }
            }
          else
            {
            graphicsErrorState = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
            return(errno);
            }

          cellPoint->xAxisPoint = (STROKE_FRAME_DELTA_X_NORMAL * ((GRAPHICS_REAL)cellSizeXIndex)) / characterFrame.strokeXAxisPoints;
          cellPoint->yAxisPoint = (STROKE_FRAME_DELTA_Y_NORMAL * ((GRAPHICS_REAL)cellSizeYIndex)) / characterFrame.strokeYAxisPoints;

          lastCellPoint = cellPoint;
          }
        }

      lastStrokeCell = alphabetStrokeCell;
      }
    else
      {
      graphicsErrorState = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
      return(errno);
      }
    }

/******************************************************************************/

  // Initialise the Mersenne Twister random number generator
  _time32(&theCurrentTime);
  init_genrand((GRAPHICS_ULONG)theCurrentTime);

  // Initialize global strings
  LoadStringW(hInstance, IDS_APP_TITLE, appTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_STROKEGRAPHICS001, appWindowClass, MAX_LOADSTRING);

  RegisterGraphicsClass(hInstance);

  // Correct the frame to our requirements!
  adjustedWindowSize = requiredWindowSize;

  AdjustWindowRect(&requiredWindowSize, WS_CAPTION, GDI_PLUS_WINDOW_MENU_STATE);

  deltaWindowSize.left   = adjustedWindowSize.left   - requiredWindowSize.left;
  deltaWindowSize.right  = requiredWindowSize.right  - adjustedWindowSize.right;
  deltaWindowSize.top    = adjustedWindowSize.top    - requiredWindowSize.top;
  deltaWindowSize.bottom = requiredWindowSize.bottom - adjustedWindowSize.bottom;

  requiredWindowSize.left   = deltaWindowSize.left   + adjustedWindowSize.left;
  requiredWindowSize.right  = deltaWindowSize.right  + adjustedWindowSize.right;
  requiredWindowSize.top    = deltaWindowSize.top    + adjustedWindowSize.top;
  requiredWindowSize.bottom = deltaWindowSize.bottom + adjustedWindowSize.bottom;

  AdjustWindowRect(&requiredWindowSize, WS_CAPTION, GDI_PLUS_WINDOW_MENU_STATE);

  // Perform application initialization:
  if (!InitInstance(hInstance, &requiredWindowSize, nCmdShow))
    {
    return(FALSE);
    }

  // Main message loop :
  while (GetMessage(&msg, nullptr, 0, 0))
    {
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
      {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
      }
    }

  GdiplusShutdown(gdiplusToken);

  LocalFree(commandLineList);

  free(commandLineArguments);

/******************************************************************************/

  return((int)msg.wParam);

/******************************************************************************/
  } /* end of WinMain                                                         */

/******************************************************************************/
/*                                                                            */
/* RegisterGraphicsClass() :                                                  */
/*                                                                            */
/*  - registers the window class.                                             */
/******************************************************************************/

ATOM RegisterGraphicsClass(HINSTANCE hInstance)
  {
/******************************************************************************/

  WNDCLASSEXW wcex = { 0 };

/******************************************************************************/

  wcex.cbSize         = sizeof(WNDCLASSEX);

  wcex.style          = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc    = WndProc;
  wcex.cbClsExtra     = 0;
  wcex.cbWndExtra     = 0;
  wcex.hInstance      = hInstance;
  wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_STROKEGRAPHICS001));
  wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground  = strokeFrameBackgroundColour; // HBRUSH)(COLORREF(STROKE_FRAME_BACKGROUND_COLOUR));
  wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_STROKEGRAPHICS001);
  wcex.lpszClassName  = appWindowClass;
  wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

/******************************************************************************/

  return(RegisterClassExW(&wcex));

/******************************************************************************/
  } /* end of RegisterGraphicsClass                                           */

/******************************************************************************/
/*                                                                            */
/* InitInstance(HINSTANCE, RECT *, int) :                                     */
/*                                                                            */ 
/* - saves the instance handle and creates the main window                    */
/******************************************************************************/

BOOL InitInstance(HINSTANCE hInstance, RECT *requiredWindowSize, int nCmdShow)
  {
/******************************************************************************/

  HINSTANCE hInst = hInstance; // Store instance handle in our global variable

  HWND      hWnd = CreateWindowW(STROKE_GRAPHICS_CLASS_NAME, // window class name
                                 STROKE_GRAPHICS_CAPTION,    // window caption
                                 WS_OVERLAPPEDWINDOW,        // window style
                                 requiredWindowSize->left,   // initial x position
                                 requiredWindowSize->top,    // initial y position
                                 requiredWindowSize->right,  // initial x size
                                 requiredWindowSize->bottom, // initial y size
                                 nullptr,
                                 nullptr,
                                 hInstance,
                                 nullptr);

/******************************************************************************/

  if (!hWnd)
    {
    return(FALSE);
    }

/******************************************************************************/

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

/******************************************************************************/

  return(TRUE);

/******************************************************************************/
  } /* end of InitInstance                                                    */

/******************************************************************************/
/* WndProc(HWND, UINT, WPARAM, LPARAM) :                                      */
/* - processes messages for the main window.                                  */
/*                                                                            */
/*  WM_COMMAND  - process the application menu                                */
/*  WM_PAINT    - Paint the main window                                       */
/*  WM_DESTROY  - post a quit message and return                              */
/******************************************************************************/

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
  {
/******************************************************************************/

  PAINTSTRUCT     ps                 = { 0 };
  HDC             hdc                = 0,
                  swapHdc            = 0;
  RECT            thisRectangle      = { 0 };

  GRAPHICS_USHORT receivedCode       = ((GRAPHICS_USHORT)0);
  GRAPHICS_INT    numberOfCodes      = (((GRAPHICS_INT)0));
  GRAPHICS_SHORT  capsControlState   = ((GRAPHICS_SHORT)0);
  GRAPHICS_UINT   controlKeyIndex    = ((GRAPHICS_UINT)0);

  GRAPHICS_UINT   segmentIndex       = ((GRAPHICS_UINT)0);

  GRAPHICS_SHORT  controlKeyState[(VK_RMENU - VK_LSHIFT) + 1]  = { 0 };

  bool            eachKeyState        = false;

  graphicsError_t objectError         = GRAPHICS_NO_ERROR;

/******************************************************************************/

   // BUG : all lower-case characters are identified first in WM_KEYDOWN as 
   //       UPPER-case characters followed by WM_CHAR receiving the lower-case 
   //       character. All upper-case characters are identified first in 
   //       WM_KEYDOWN as a SHIFT character followed by WM_CHAR receiving the 
   //       upper-case character.
   switch (message)
     {
     case WM_CHAR    : receivedCode                               = (GRAPHICS_WCHAR)wParam;
                       translatedCharacterState.receivedCodeState = true;
                       [[fallthrough]]; // break;

     case WM_KEYDOWN : // Check if the mode is 'system' or 'edit'

                       // Unpick the order of character reception. First note 
                       // the occurence of "WM_KEYDOWN"
                       if (translatedCharacterState.keydownState == false)
                         {
                         translatedCharacterState.keydownState = true;
                         }

                       eachKeyState = GetKeyboardState(&keyboardState[0]);

                       // ToAsciiEx((GRAPHICS_UINT)(wParam),
                       //           (GRAPHICS_UINT)(lParam),
                       //           &keyboardState[0],
                       //           &translatedCharacters.translatedCharacterPair,
                       //            STROKE_GRAPHICS_DEFAULT_MENU_CODE,
                       //            STROKE_GRAPHICS_DEFAULT_KEYBOARD_CODE);

                       numberOfCodes = ToUnicodeEx( (GRAPHICS_UINT)(wParam),
                                                    (GRAPHICS_UINT)(lParam),
                                                   &keyboardState[0],
                                                   &translatedCharacters.translatedCharactersW[0],
                                                    GRAPHICS_WORD_LENGTH,
                                                    STROKE_GRAPHICS_DEFAULT_MENU_CODE,
                                                    STROKE_GRAPHICS_DEFAULT_KEYBOARD_CODE);

                       if ((characterSelectionState.thisSelectionState == CHARACTER_SELECTION_STATE_NONE) && 
                           (characterSelectionState.lastSelectionState == CHARACTER_SELECTION_STATE_NONE) &&
                           (translatedCharacterState.receivedCodeState == true)                           &&
                           (translatedCharacterState.keydownState      == true)                           &&

                           //(!((wParam                                  == CHARACTER_SELECT_KEY)           &&
                           //   (characterActive                         == true)))                         &&

                              (wParam                                  != KEY_ESC_EXIT))
                         {
                         characterSelectionState.selectedCharacter = (GRAPHICS_UINT)wParam;
                         actualCode                                = (GRAPHICS_UINT)wParam;

                         //characterSelectionState.selectedCharacter = receivedCode & EXTENDED_ASCII_MASK;
                         //actualCode                                = receivedCode & EXTENDED_ASCII_MASK;

                         characterActive                           = true;

                         // The new input code is now active, cancel the signals
                         translatedCharacterState.receivedCodeState = false;
                         translatedCharacterState.keydownState      = false;

                         InvalidateRect(hWnd, NULL, TRUE);
                         
                         return(0);
                         }
                       else
                         {
                         // Edit mode :
                         if ((receivedCode != ((GRAPHICS_USHORT)0)) && ((wParam & EXTENDED_ASCII_MASK) != KEY_ESC_EXIT))
                           {
                           actualCode = receivedCode & EXTENDED_ASCII_MASK;

                           if (copyCharacterBufferIndex != COPY_CHARACTER_BUFFER_LENGTH)
                             {
                             copyCharacterBuffer[copyCharacterBufferIndex] = actualCode;
                             copyCharacterBufferIndex                      = copyCharacterBufferIndex + ((GRAPHICS_INT)1);
                             }

                           translatedCharacterState.keydownState = false;
                           }
                         else
                           {
                           switch(wParam)
                             {
                             // Cursor navigation is by offsets into the stroke graph i.e. { X : 0..[n - 1] }, { X : 0..[m - 1] }
                             case STROKE_NAV_KEY_LEFT            : if (characterSelectionState.thisSelectionState == CHARACTER_SELECTION_STATE_SELECTED)
                                                                      {
                                                                      if ((strokeActiveCursor & STROKE_NAV_KEY_STATE_LINE_START_CURSOR_SELECTED) ||
                                                                          (strokeActiveCursor & STROKE_NAV_KEY_STATE_LINE_END_CURSOR_SELECTED))
                                                                        {
                                                                        if (characterSelectionState.strokeCursorToggleState == STROKE_CURSOR_TOGGLE_STATE_NOT_SELECTED)
                                                                          {
                                                                          strokeNavigationState = STROKE_NAV_STATE_CURSOR_LEFT;
                                                                          }
                                                                        else
                                                                          {
                                                                          strokeNavigationState = STROKE_NAV_STATE_CHARACTER_LEFT;
                                                                          }
                                                                        }
                                                                      }
                                                                    break;
                                                                  
                             case STROKE_NAV_KEY_RIGHT            : if (characterSelectionState.thisSelectionState == CHARACTER_SELECTION_STATE_SELECTED)
                                                                       {
                                                                       if ((strokeActiveCursor & STROKE_NAV_KEY_STATE_LINE_START_CURSOR_SELECTED) ||
                                                                           (strokeActiveCursor & STROKE_NAV_KEY_STATE_LINE_END_CURSOR_SELECTED))
                                                                         {
                                                                         if (characterSelectionState.strokeCursorToggleState == STROKE_CURSOR_TOGGLE_STATE_NOT_SELECTED)
                                                                           {
                                                                           strokeNavigationState = STROKE_NAV_STATE_CURSOR_RIGHT;
                                                                           }
                                                                         else
                                                                           {
                                                                           strokeNavigationState = STROKE_NAV_STATE_CHARACTER_RIGHT;
                                                                           }
                                                                         }
                                                                       }
                                                                    break;
                                                                  
                             case STROKE_NAV_KEY_UP               : if (characterSelectionState.thisSelectionState == CHARACTER_SELECTION_STATE_SELECTED)
                                                                       {
                                                                       if ((strokeActiveCursor & STROKE_NAV_KEY_STATE_LINE_START_CURSOR_SELECTED) ||
                                                                           (strokeActiveCursor & STROKE_NAV_KEY_STATE_LINE_END_CURSOR_SELECTED))
                                                                         {
                                                                         if (characterSelectionState.strokeCursorToggleState == STROKE_CURSOR_TOGGLE_STATE_NOT_SELECTED)
                                                                           {
                                                                           strokeNavigationState = STROKE_NAV_STATE_CURSOR_UP;
                                                                           }
                                                                         else
                                                                           {
                                                                           strokeNavigationState = STROKE_NAV_STATE_CHARACTER_UP;
                                                                           }
                                                                         }
                                                                       }
                                                                    break;
                                                                  
                             case STROKE_NAV_KEY_DOWN             : if (characterSelectionState.thisSelectionState == CHARACTER_SELECTION_STATE_SELECTED)
                                                                       {
                                                                       if ((strokeActiveCursor & STROKE_NAV_KEY_STATE_LINE_START_CURSOR_SELECTED) ||
                                                                           (strokeActiveCursor & STROKE_NAV_KEY_STATE_LINE_END_CURSOR_SELECTED))
                                                                         {
                                                                         if (characterSelectionState.strokeCursorToggleState == STROKE_CURSOR_TOGGLE_STATE_NOT_SELECTED)
                                                                           {
                                                                           strokeNavigationState = STROKE_NAV_STATE_CURSOR_DOWN;
                                                                           }
                                                                         else
                                                                           {
                                                                           strokeNavigationState = STROKE_NAV_STATE_CHARACTER_DOWN;
                                                                           }
                                                                         }
                                                                       }
                                                                    break;
                                                                 
                             case STROKE_NAV_KEY_CURSOR_PAIR      : if (characterSelectionState.thisSelectionState == CHARACTER_SELECTION_STATE_SELECTED)
                                                                      {
                                                                      if (strokeActiveCursor != STROKE_NAV_KEY_STATE_NONE) 
                                                                        {
                                                                        // Toggle the cursor pairing state
                                                                        if ((GRAPHICS_UINT)strokeActiveCursor & ((GRAPHICS_UINT)STROKE_NAV_KEY_STATE_LINE_BOTH_START_LEADING))
                                                                          {
                                                                          strokeActiveCursor = (strokeNavKeyStates_t)((GRAPHICS_UINT)strokeActiveCursor & (~(GRAPHICS_UINT)STROKE_NAV_KEY_STATE_LINE_BOTH_START_LEADING));
                                                                          }
                                                                        else
                                                                          {
                                                                          strokeActiveCursor = (strokeNavKeyStates_t)((GRAPHICS_UINT)strokeActiveCursor | (GRAPHICS_UINT)STROKE_NAV_KEY_STATE_LINE_BOTH_START_LEADING);
                                                                          }
 
                                                                        strokeNavigationState = STROKE_NAV_STATE_NONE;
                                                                        }
                                                                      }
                                                                 
                                                                    break;
                                                                 
                             case STROKE_NAV_KEY_CURSOR_SWAP      : if (characterSelectionState.thisSelectionState == CHARACTER_SELECTION_STATE_SELECTED)
                                                                      {
                                                                      if (strokeActiveCursor != STROKE_NAV_KEY_STATE_NONE)
                                                                        {
                                                                        if (strokeActiveCursor & STROKE_NAV_KEY_STATE_LINE_START_CURSOR_SELECTED)
                                                                          {
                                                                          strokeActiveCursor = (strokeNavKeyStates_t)((GRAPHICS_UINT)strokeActiveCursor & (~(GRAPHICS_UINT)STROKE_NAV_KEY_STATE_LINE_START_CURSOR_SELECTED));
                                                                          strokeActiveCursor = (strokeNavKeyStates_t)((GRAPHICS_UINT)strokeActiveCursor | (GRAPHICS_UINT)STROKE_NAV_KEY_STATE_LINE_END_CURSOR_SELECTED);
                                                                          }
                                                                        else
                                                                          {
                                                                          if (strokeActiveCursor & STROKE_NAV_KEY_STATE_LINE_END_CURSOR_SELECTED)
                                                                            {
                                                                            strokeActiveCursor = (strokeNavKeyStates_t)((GRAPHICS_UINT)strokeActiveCursor & (~(GRAPHICS_UINT)STROKE_NAV_KEY_STATE_LINE_END_CURSOR_SELECTED));
                                                                            strokeActiveCursor = (strokeNavKeyStates_t)((GRAPHICS_UINT)strokeActiveCursor | (GRAPHICS_UINT)STROKE_NAV_KEY_STATE_LINE_START_CURSOR_SELECTED);
                                                                            }
                                                                          }
                                                                        }
                                                                      }
                                                                    break;
                             
                             case CHARACTER_LINE_SEGMENT_SAVE_KEY : // Add the current line-segment to the character stroke list OR 
                                                                    // if the segment list is empty, reinstate the original character from the cache
                                                                    if (characterSelectionState.thisSelectionState == CHARACTER_SELECTION_STATE_SELECTED)
                                                                      {
                                                                      if (characterSelectionState.modificationState == CHARACTER_SELECTION_STATE_MODIFY_OFF)
                                                                        {
                                                                        strokeNavigationState = STROKE_NAV_STATE_SAVE_SEGMENT;
                                                                        }
                                                                      }

                                                                    break;

                             case CHARACTER_RECOVER_FROM_CACHE_KEY : if (characterSelectionState.thisSelectionState == CHARACTER_SELECTION_STATE_SELECTED)
                                                                      {
                                                                      if (characterSelectionState.modificationState == CHARACTER_SELECTION_STATE_MODIFY_OFF)
                                                                        {
                                                                        objectError = copyCharacterOneToCharacterTwo(CHARACTER_CACHE_INDEX,
                                                                                                                     characterSelectionState.selectedCharacter,
                                                                                                                     characterCache,
                                                                                                                     characterListRoot);

                                                                        objectError = computeCharacterExtents( characterSelectionState.selectedCharacter,
                                                                                                               characterListRoot,
                                                                                                              &strokeGraphPointBase);
                                                                        }
                                                                      }
                                                                    break;
                             
                             case STROKE_GRAPH_TOGGLE             : // Turn the background graph on and off
                                                                    if (characterSelectionState.thisSelectionState == CHARACTER_SELECTION_STATE_SELECTED)
                                                                      {
                                                                      if (strokeGraphToggleState == STROKE_GRAPH_TOGGLE_STATE_SELECTED)
                                                                        {
                                                                        strokeGraphToggleState = STROKE_GRAPH_TOGGLE_STATE_NOT_SELECTED;
                                                                        }
                                                                      else
                                                                        {
                                                                        strokeGraphToggleState = STROKE_GRAPH_TOGGLE_STATE_SELECTED;
                                                                        }
                                                                      }

                                                                    break;

                             case STROKE_CURSOR_KEY_TOGGLE        : if (characterSelectionState.thisSelectionState == CHARACTER_SELECTION_STATE_SELECTED)
                                                                      {
                                                                      if (characterSelectionState.strokeCursorToggleState == STROKE_CURSOR_TOGGLE_STATE_SELECTED)
                                                                        {
                                                                        characterSelectionState.strokeCursorToggleState = STROKE_CURSOR_TOGGLE_STATE_NOT_SELECTED;
                                                                        }
                                                                      else
                                                                        {
                                                                        characterSelectionState.strokeCursorToggleState = STROKE_CURSOR_TOGGLE_STATE_SELECTED;
                                                                        }
                                                                      }

                                                                    break;

                            case CHARACTER_FLIP_VERTICAL          : if (characterSelectionState.thisSelectionState == CHARACTER_SELECTION_STATE_SELECTED)
                                                                      {
                                                                      fetchCharacterReference( characterSelectionState.selectedCharacter,
                                                                                               characterListRoot,
                                                                                              &characterReference);

                                                                      flipCharacterVertically(characterReference);
                                                                      }

                                                                    break;

                            case CHARACTER_FLIP_HORIZONTAL        : if (characterSelectionState.thisSelectionState == CHARACTER_SELECTION_STATE_SELECTED)
                                                                      {
                                                                      fetchCharacterReference( characterSelectionState.selectedCharacter,
                                                                                               characterListRoot,
                                                                                              &characterReference);

                                                                      flipCharacterHorizontally(characterReference);
                                                                      }

                                                                    break;

                             case ALPHABET_SAVE_KEY               : if ((objectError = writeAlphabetToFile(&alphabetFileDescriptor,
                                                                                                            characterListRoot,
                                                                                                           &strokeGraphPointBase)) == GRAPHICS_NO_ERROR)
                                                                      {
                                                                      alphabetSaveCounter = alphabetSaveCounter + ((GRAPHICS_UINT)1);
                                                                      }
                                                                    else
                                                                      {
                                                                      MessageBoxW(NULL, MESSAGE_BOX_FILE_SAVE_ERROR_STATEMENT, MESSAGE_BOX_ERROR_CAPTION, MESSAGE_BOX_ERROR_BEHAVIOUR);
                                                                      exit(0);
                                                                      }
                                                                    break;
                           
                             case LINE_SEGMENT_MODIFY_ON_KEY      : if (characterSelectionState.thisSelectionState == CHARACTER_SELECTION_STATE_SELECTED)
                                                                      {
                                                                      if (characterListRoot != nullptr)
                                                                        { // Point at the current character
                                                                        if ((objectError = fetchCharacterReference( characterSelectionState.selectedCharacter,
                                                                                                                    characterListRoot,
                                                                                                                   &characterReference)) == GRAPHICS_NO_ERROR)
                                                                          { // Check there are any line segments for this character
                                                                          if ((objectError = fetchCharacterExtantSegment( segmentIndex,
                                                                                                                          characterReference,
                                                                                                                         &nextLineSegment)) == GRAPHICS_NO_ERROR)
                                                                            {
                                                                            characterReference->lineSegmentIndex      = ((GRAPHICS_UINT)0); // highlight selected line segment for editing
                                                                            characterSelectionState.modificationState = CHARACTER_SELECTION_STATE_MODIFY_ON;
                                                                            
                                                                            // Force the cursor format
                                                                            strokeActiveCursor                        = (strokeNavKeyStates_t)((GRAPHICS_UINT)STROKE_NAV_KEY_STATE_LINE_START_CURSOR_SELECTED | (GRAPHICS_UINT)STROKE_NAV_KEY_STATE_LINE_BOTH_START_LEADING);
                                                                            }
                                                                          }
                                                                        }
                                                                      else
                                                                        {
                                                                        objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
                                                                        }
                                                                      }
                                                                    break;
                           
                             case LINE_SEGMENT_MODIFY_OFF_KEY     : if (characterSelectionState.thisSelectionState == CHARACTER_SELECTION_STATE_SELECTED)
                                                                      {
                                                                      characterSelectionState.modificationState = CHARACTER_SELECTION_STATE_MODIFY_OFF;
                           
                                                                      // Release the cursor format
                                                                      strokeActiveCursor = (strokeNavKeyStates_t)(STROKE_NAV_KEY_STATE_LINE_START_CURSOR_SELECTED | STROKE_NAV_KEY_STATE_LINE_BOTH_START_LEADING);
                                                                      }
                                                                    break;
                           
                             case LINE_SEGMENT_MODIFY_DELETE_KEY  : if (characterSelectionState.thisSelectionState == CHARACTER_SELECTION_STATE_SELECTED)
                                                                      {
                                                                      if (characterSelectionState.modificationState == CHARACTER_SELECTION_STATE_MODIFY_ON)
                                                                        {
                                                                        strokeNavigationState = STROKE_NAV_STATE_DELETE_SEGMENT;
                                                                        }
                                                                      }

                                                                      break;

                             case CHARACTER_DELETE_KEY            : if (characterSelectionState.thisSelectionState == CHARACTER_SELECTION_STATE_SELECTED)
                                                                      {
                                                                      if (characterSelectionState.modificationState == CHARACTER_SELECTION_STATE_MODIFY_ON)
                                                                        {
                                                                        fetchCharacterReference( characterSelectionState.selectedCharacter,
                                                                                                 characterListRoot,
                                                                                                &characterReference);

                                                                        deleteCharacter(characterReference);
                                                                        }
                                                                      }

                                                                    break;
                           
                             case CHARACTER_SELECT_KEY            : // NOTE : the order of activation is 'CHARACTER_SELECT_KEY' followed by 
                                                                    //        'STROKE_NAV_KEY_CURSOR_SELECT'
                                                                    if (characterSelectionState.thisSelectionState == CHARACTER_SELECTION_STATE_NONE)
                                                                      {
                                                                      characterSelectionState.thisSelectionState = CHARACTER_SELECTION_STATE_SELECTED;
                                                                      strokeActiveCursor                         = STROKE_NAV_KEY_STATE_LINE_START_CURSOR_DESELECTED;
                                                                      strokeNavigationState                      = STROKE_NAV_STATE_NONE;

                                                                      if (fetchCharacterReference( characterSelectionState.selectedCharacter,
                                                                                                   characterListRoot,
                                                                                                   &characterReference) == GRAPHICS_NO_ERROR)
                                                                        {
                                                                        // Create a character cache of length one character
                                                                        if (buildCharacterList( CHARACTER_CACHE_LENGTH,
                                                                                               &characterCache) == GRAPHICS_NO_ERROR)
                                                                          {
                                                                          objectError = copyCharacterOneToCharacterTwo(characterSelectionState.selectedCharacter,
                                                                                                                       CHARACTER_CACHE_INDEX,
                                                                                                                       characterListRoot,
                                                                                                                       characterCache);

                                                                          objectError = computeCharacterExtents( CHARACTER_CACHE_INDEX, // characterSelectionState.selectedCharacter,
                                                                                                                 characterCache,
                                                                                                                &strokeGraphPointBase);

                                                                          cacheExtantFlag = true;
                                                                          }
                                                                        else
                                                                          {
                                                                          MessageBoxW(NULL, MESSAGE_BOX_INTERNAL_MEMORY_ERROR, MESSAGE_BOX_ERROR_CAPTION, MESSAGE_BOX_ERROR_BEHAVIOUR);
                                                                          exit(0);
                                                                          }
                                                                        }
                                                                      else
                                                                        {
                                                                        MessageBoxW(NULL, MESSAGE_BOX_INTERNAL_MEMORY_ERROR, MESSAGE_BOX_ERROR_CAPTION, MESSAGE_BOX_ERROR_BEHAVIOUR);
                                                                        exit(0);
                                                                        }
                                                                      }
                                                                    else
                                                                      {
                                                                      if (characterSelectionState.thisSelectionState == CHARACTER_SELECTION_STATE_SELECTED)
                                                                        {
                                                                        characterShiftFlag                                  = false;
                                                                        strokeGraphToggleState                              = STROKE_GRAPH_TOGGLE_STATE_SELECTED;
                                                                        characterSelectionState.strokeCursorToggleState     = STROKE_CURSOR_TOGGLE_STATE_NOT_SELECTED;
  
                                                                        copyCharacterBufferIndex                            = ((GRAPHICS_UINT)0);
                                                                        copyCharacterBuffer[COPY_CHARACTER_BUFFER_OFFSET_0] = ((GRAPHICS_USHORT)0);
                                                                        copyCharacterBuffer[COPY_CHARACTER_BUFFER_OFFSET_1] = ((GRAPHICS_USHORT)0);
  
                                                                        characterActive                                     = false;
                                                                        characterSelectionState.thisSelectionState          = CHARACTER_SELECTION_STATE_DESELECTED;
                                                                        characterSelectionState.modificationState           = CHARACTER_SELECTION_STATE_MODIFY_OFF;
                                                                        strokeActiveCursor                                  = STROKE_NAV_KEY_STATE_LINE_START_CURSOR_DESELECTED;
                                                                        strokeNavigationState                               = STROKE_NAV_STATE_NONE;
  
                                                                        characterSelectionState.selectedCharacter           = ((GRAPHICS_UINT)0); // NUL is never used ?
  
                                                                        if (cacheExtantFlag == true)
                                                                          {
                                                                          if (fetchCharacterReference( characterSelectionState.selectedCharacter,
                                                                                                       characterCache,
                                                                                                       &characterCache) == GRAPHICS_NO_ERROR)
                                                                            {
                                                                            objectError = deleteCharacter(characterCache);
                                                                          
                                                                            if ((objectError == GRAPHICS_NO_ERROR) || (objectError == GRAPHICS_OBJECT_INSTANTIATION_ERROR))
                                                                              {
                                                                              free(characterCache);
                                                                              
                                                                              characterCache  = nullptr;
                                                                          
                                                                              cacheExtantFlag = false;
                                                                              }
                                                                            }
                                                                          }
                                                                        else
                                                                          {
                                                                          MessageBoxW(NULL, MESSAGE_BOX_INTERNAL_MEMORY_ERROR, MESSAGE_BOX_ERROR_CAPTION, MESSAGE_BOX_ERROR_BEHAVIOUR);
                                                                          exit(0);
                                                                          }
  
                                                                        if ((objectError = fetchCharacterReference( characterSelectionState.selectedCharacter,
                                                                                                                    characterListRoot,
                                                                                                                   &characterReference)) == GRAPHICS_NO_ERROR)
                                                                          {
                                                                          characterReference->characterExtents.topLeft.pointX     = ((GRAPHICS_INT)0);
                                                                          characterReference->characterExtents.topLeft.pointY     = ((GRAPHICS_INT)0);
                                                                          characterReference->characterExtents.bottomRight.pointX = ((GRAPHICS_INT)0);
                                                                          characterReference->characterExtents.bottomRight.pointY = ((GRAPHICS_INT)0);
                                                                          }
                                                                        }
                                                                      }

                                                                    break;
                           
                             case CHARACTER_COPY_A_TO_B_KEY       : if (copyCharacterBufferIndex == COPY_CHARACTER_BUFFER_LENGTH)
                                                                      {
                                                                      objectError = copyCharacterOneToCharacterTwo(copyCharacterBuffer[COPY_CHARACTER_BUFFER_SOURCE],
                                                                                                                   copyCharacterBuffer[COPY_CHARACTER_BUFFER_DESTINATION],
                                                                                                                   characterListRoot,
                                                                                                                   characterListRoot );
                                                           
                                                                      copyCharacterBufferIndex                            = ((GRAPHICS_UINT)0);
                                                                      copyCharacterBuffer[COPY_CHARACTER_BUFFER_OFFSET_0] = ((GRAPHICS_USHORT)0);
                                                                      copyCharacterBuffer[COPY_CHARACTER_BUFFER_OFFSET_1] = ((GRAPHICS_USHORT)0);
                                                                      }
                                                                    break;
                                                                                                             
                             case CANCEL_COPY_A_TO_B_KEY          : copyCharacterBufferIndex                            = ((GRAPHICS_UINT)0);
                                                                    copyCharacterBuffer[COPY_CHARACTER_BUFFER_OFFSET_0] = ((GRAPHICS_USHORT)0);
                                                                    copyCharacterBuffer[COPY_CHARACTER_BUFFER_OFFSET_1] = ((GRAPHICS_USHORT)0);

                                                                    break;
                           
                             case KEY_ESC_EXIT                    :
                                                                    PostMessage(hWnd, WM_CLOSE, 0, 0);
                                                                    [[fallthrough]]; // warning C26819 fallthrough is explicit
                             
                             default                              : 
                                                                    break;
                             };

                           translatedCharacterState.keydownState = false;
                           }
                         }

      if (wParam != VK_SHIFT)
        {
        InvalidateRect(hWnd, NULL, TRUE);
        }

      return(0);

      break;

      case WM_COMMAND:
        {
        int wmId = LOWORD(wParam);

        // Parse the menu selections:
        switch (wmId)
          {
          case IDM_ABOUT:
              DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
              break;
          case IDM_EXIT:
              DestroyWindow(hWnd);
              break;
          default:
              return(DefWindowProc(hWnd, message, wParam, lParam));
          }
        }
      break;

      case WM_MOUSEMOVE :
        *(&mouseXPosition) = GET_X_LPARAM(lParam);
        *(&mouseYPosition) = GET_Y_LPARAM(lParam); 

        break;

      case WM_MBUTTONDOWN : objectPositionTestFlag = true;
      
                            InvalidateRect(hWnd, NULL, TRUE);
        
                            return(0);
                            break;

      InvalidateRect(hWnd, NULL, TRUE);

      return(0);

      case WM_PAINT:
        {
        // Dynamically resize the window
        GetWindowRect(hWnd, &thisRectangle);

        setCanvasCoordinates(&canvasSize,
                              (GRAPHICS_REAL)thisRectangle.left,
                              (GRAPHICS_REAL)thisRectangle.top,
                              (GRAPHICS_REAL)thisRectangle.right,
                              (GRAPHICS_REAL)thisRectangle.bottom,
                              (GRAPHICS_REAL)(thisRectangle.right  - thisRectangle.left),
                              (GRAPHICS_REAL)(thisRectangle.bottom - thisRectangle.top));

        hdc     = BeginPaint(hWnd, &ps);   // the ultimate destination for the new image

        swapHdc = CreateCompatibleDC(hdc); // the hidden drawing plane for the new image

        // Create a "window" bitmap for 'hdc' and a "source" bitmap for 'swapHdc'
        HBITMAP hbmMem = CreateCompatibleBitmap(hdc, (INT)canvasSize.width, (INT)canvasSize.height);

        HBITMAP hbmOld = (HBITMAP)SelectObject(swapHdc, hbmMem);

        // Force a redraw of the line segment stroke cursors
        strokeDrawingState = STROKE_DRAWING_STATE_REDRAW;

        OnPaint( swapHdc,
                 hWnd,
                &characterSelectionState,
                &canvasSize,
                &localCanvasSize,
                &characterFrame,
                &mainCaptionFrame,
                 characterListRoot,
                 alphabetStrokeCell,
                &strokeDrawingState,
                &mouseXPosition,
                &mouseYPosition,
                &strokeNavigationState,
                &strokeActiveCursor,
                &strokeStartCursor,
                &strokeEndCursor,
                &lineSegment,
                &alphabetFileDescriptor,
                &strokeGraphPointBase);

        if (!StretchBlt(hdc,
                        ((INT)0), // canvasSize.left,
                        ((INT)0), // canvasSize.top,
                        (INT)canvasSize.width,
                        (INT)canvasSize.height,
                        swapHdc,
                        ((INT)localCanvasSize.left), // localCanvasSize.left,
                        ((INT)localCanvasSize.top),  // localCanvasSize.top,
                        (INT)localCanvasSize.width,
                        (INT)localCanvasSize.height,
                        SRCCOPY))
          {
          while(true)
            ;
          }

        SelectObject(swapHdc, hbmOld);

        DeleteObject(swapHdc);
        DeleteDC(swapHdc);

        ReleaseDC(hWnd, hdc);

        EndPaint(hWnd, &ps);
        }
      break;

      case WM_CLOSE:
        DestroyWindow(hWnd);
        return(0);

      case WM_DESTROY:
        PostQuitMessage(0);
        return(0);
      break;

      default:
        return(DefWindowProc(hWnd, message, wParam, lParam));
      }

/******************************************************************************/

  return(0);

/******************************************************************************/
  } /* end of WndProc                                                         */

/******************************************************************************/
/* OnPaint() :                                                                */
/* - the business end for drawing and reacting to input                       */
/******************************************************************************/

VOID OnPaint(HDC                          hdc,
             HWND                         hWnd,
             characterSelectionState_tPtr characterSelectionState,
             canvasDescriptor_tPtr        canvasSize,
             canvasDescriptor_tPtr        localCanvasSize,
             strokeFrame_tPtr             characterFrame,
             captionDescriptor_tPtr       caption,
             alphabetCharacters_tPtr      characterList,
             strokeCell_tPtr              strokeCells,
             strokeDrawingState_tPtr      strokeDrawingState,
             GRAPHICS_SHORT_PTR           mouseXPosition,
             GRAPHICS_SHORT_PTR           mouseYPosition,
             strokeNavStates_tPtr         strokeNavigationState,
             strokeNavKeyStates_tPtr      strokeActiveCursor,
             strokeCursor_tPtr            strokeStartCursor,
             strokeCursor_tPtr            strokeEndCursor,
             newLineSegment_tPtr          lineSegment,
             alphabetFileDescriptor_tPtr  alphabetFileDescriptor,
             strokeGraphPointBase_tPtr    strokeGraphBase)
  {           
/******************************************************************************/

  graphicsError_t        objectError = GRAPHICS_NO_ERROR;
                         
  Graphics               graphics(hdc);
                         
  RECT                   localRectangle    = { 0 };
                         
  // singleIndexReal_t      localCanvasCentre = { ((GRAPHICS_REAL)0.0), ((GRAPHICS_REAL)0.0) };

  GRAPHICS_UINT          canvasCentreX = ((GRAPHICS_UINT)0),
                         canvasCentreY = ((GRAPHICS_UINT)0);

  SolidBrush             backgroundPen(Color(FRAME_BACKGROUND_COLOUR_A, FRAME_BACKGROUND_COLOUR_RED, FRAME_BACKGROUND_COLOUR_GREEN, FRAME_BACKGROUND_COLOUR_BLUE));

  HFONT                  hFont             = nullptr,
                         hOldFont          = nullptr;

/******************************************************************************/

  if ((hdc               != nullptr) && (characterSelectionState != nullptr) && (canvasSize         != nullptr) &&
      (characterFrame    != nullptr) && (caption                 != nullptr) && (characterList      != nullptr) &&
      (strokeCells       != nullptr) && (strokeDrawingState      != nullptr) && (mouseXPosition     != nullptr) &&
      (mouseYPosition    != nullptr) && (strokeNavigationState   != nullptr) && (strokeActiveCursor != nullptr) &&
      (strokeStartCursor != nullptr) && (strokeEndCursor         != nullptr))
    {
    RectF frameF(((GRAPHICS_FLOAT)0.0), //canvasSize->left),
                 ((GRAPHICS_FLOAT)0.0), //canvasSize->top),
                 ((GRAPHICS_FLOAT)(canvasSize->right - canvasSize->left)),
                 ((GRAPHICS_FLOAT)canvasSize->bottom));

    graphics.FillRectangle(&backgroundPen, frameF);

    objectError = buildCharacterStrokeGraph(characterFrame,
                                            strokeGraphBase);

    if ((strokeGraphToggleState == STROKE_GRAPH_TOGGLE_STATE_SELECTED) || (characterActive == false))
      {
      objectError = drawCharacterStrokeGraph( hdc,
                                             &characterFrame->strokeColour,
                                             &characterFrame->middleColour,
                                             &characterFrame->descentColour,
                                              canvasSize,
                                              strokeGraphBase);
      }
    
    // Get the canvas size as seen from this HDC swap-buffer
    GetWindowRect(hWnd, &localRectangle);

    setCanvasCoordinates(localCanvasSize,
                         (0.0f), // (GRAPHICS_REAL)localRectangle.left,
                         (0.0f), // (GRAPHICS_REAL)localRectangle.top,
                         (GRAPHICS_REAL)localRectangle.right,
                         (GRAPHICS_REAL)localRectangle.bottom,
                         (GRAPHICS_REAL)(localRectangle.right  - localRectangle.left),
                         (GRAPHICS_REAL)(localRectangle.bottom - localRectangle.top));

    // Initialise the stroke cursors when a new character definition begins. The first point in the stroke graphic 
    // graph is the default for both stroke cursors' coordinates
    if ((characterSelectionState->thisSelectionState == CHARACTER_SELECTION_STATE_SELECTED) && 
        (characterSelectionState->lastSelectionState == CHARACTER_SELECTION_STATE_NONE))
      {
      if (strokeGraphBase->graphPoints != nullptr)
        {
        GRAPHICS_UINT segmentIndex = ((GRAPHICS_UINT)0);
        
        // objectError = computeCanvasCentre( localCanvasSize,
        //                                   &localCanvasCentre);

        canvasCentreX = strokeGraphBase->graphRowNumber    >> 1;
        canvasCentreY = strokeGraphBase->graphColumnNumber >> 1;

        strokeStartCursor->strokeCursorIndexDefault.xAxisIndex = canvasCentreX;
        strokeStartCursor->strokeCursorIndexDefault.yAxisIndex = canvasCentreY;
        strokeEndCursor->strokeCursorIndexDefault.xAxisIndex   = canvasCentreX;
        strokeEndCursor->strokeCursorIndexDefault.yAxisIndex   = canvasCentreY;
        
        strokeStartCursor->strokeCursorIndexLit.xAxisIndex     = canvasCentreX;
        strokeStartCursor->strokeCursorIndexLit.yAxisIndex     = canvasCentreY;
        strokeEndCursor->strokeCursorIndexLit.xAxisIndex       = canvasCentreX;
        strokeEndCursor->strokeCursorIndexLit.yAxisIndex       = canvasCentreY;

        // strokeStartCursor->strokeCursorIndexDefault = localCanvasCentre;
        // Always start with the 'start' cursor selected
        *strokeActiveCursor = (strokeNavKeyStates_t)(STROKE_NAV_KEY_STATE_LINE_START_CURSOR_SELECTED | STROKE_NAV_KEY_STATE_LINE_BOTH_START_LEADING);
    
        // Zero the line segment points
        lineSegment->lineSegmentOriginX      = canvasCentreX;
        lineSegment->lineSegmentOriginY      = canvasCentreY;
        lineSegment->lineSegmentDestinationX = canvasCentreX; 
        lineSegment->lineSegmentDestinationY = canvasCentreY;
        lineSegment->lineSegmentChanged      = false;

        objectError = drawStrokeCursors(hdc,
                                        strokeNavigationState,
                                        (const strokeFrame_tPtr)characterFrame,
                                        (const strokeDrawingState_tPtr)strokeDrawingState,
                                        (const strokeNavKeyStates_tPtr)strokeActiveCursor,
                                        strokeStartCursor,
                                        strokeEndCursor,
                                        (const canvasDescriptor_tPtr)localCanvasSize,
                                        (const strokeGraphPointBase_tPtr)strokeGraphBase,
                                        lineSegment);
    
        // Point at the current character
        objectError = fetchCharacterReference( characterSelectionState->selectedCharacter,
                                               characterList,
                                              &characterReference);

        // One-by-one, point at the already extant line segments
        while ((objectError = fetchCharacterExtantSegment( segmentIndex,
                                                           characterReference,
                                                          &nextLineSegment)) == GRAPHICS_NO_ERROR)
          {
          // Redraw the existing character segments
          objectError = drawLineSegment(hdc,
                                        nextLineSegment,
                                        LINE_SEGMENT_MODE_PASSIVE,
                                        (const strokeFrame_tPtr)characterFrame,
                                        (const canvasDescriptor_tPtr)canvasSize,
                                        (const strokeGraphPointBase_tPtr)strokeGraphBase);

          segmentIndex = segmentIndex + ((GRAPHICS_UINT)1);
          }

        objectError = computeCharacterExtents(characterSelectionState->selectedCharacter,
                                              characterList,
                                              strokeGraphBase);

        characterSelectionState->lastSelectionState = CHARACTER_SELECTION_STATE_SELECTED;
        }
      else
        { // Something has gone wrong!
        characterSelectionState->thisSelectionState = CHARACTER_SELECTION_STATE_NONE;
        characterSelectionState->lastSelectionState = CHARACTER_SELECTION_STATE_NONE;
        }
      }
    else
      { // This is the "create-and-edit" state
      if ((characterSelectionState->thisSelectionState == CHARACTER_SELECTION_STATE_SELECTED) && 
          (characterSelectionState->lastSelectionState == CHARACTER_SELECTION_STATE_SELECTED))
        {
        GRAPHICS_UINT segmentIndex = ((GRAPHICS_UINT)0);

        objectError = handleStrokeCursors(strokeNavigationState,
                                          (const strokeDrawingState_tPtr)strokeDrawingState,
                                          characterSelectionState,
                                          characterList,
                                          (const strokeNavKeyStates_tPtr)strokeActiveCursor,
                                          strokeStartCursor,
                                          strokeEndCursor,
                                          (const strokeGraphPointBase_tPtr)strokeGraphBase,
                                          lineSegment);
    
        objectError = drawStrokeCursors(hdc,
                                        strokeNavigationState,
                                        (const strokeFrame_tPtr)characterFrame,
                                        (const strokeDrawingState_tPtr)strokeDrawingState,
                                        (const strokeNavKeyStates_tPtr)strokeActiveCursor,
                                        strokeStartCursor,
                                        strokeEndCursor,
                                        (const canvasDescriptor_tPtr)localCanvasSize,
                                        (const strokeGraphPointBase_tPtr)strokeGraphBase,
                                        lineSegment);

        // Point at the current character
        objectError = fetchCharacterReference( characterSelectionState->selectedCharacter,
                                               characterList,
                                              &characterReference);

        // Process any character movement
        if (characterSelectionState->strokeCursorToggleState == STROKE_CURSOR_TOGGLE_STATE_SELECTED)
          {
          objectError = moveCharacterInFrame(&characterReference,
                                              characterList,
                                              strokeNavigationState,
                                              (const strokeGraphPointBase_tPtr)strokeGraphBase);
          }

        // One-by-one, point at the already extant line segments
        while ((objectError = fetchCharacterExtantSegment( segmentIndex,
                                                           characterReference,
                                                          &nextLineSegment)) == GRAPHICS_NO_ERROR)
          {
          // Redraw the existing character segments
          if (characterSelectionState->modificationState == CHARACTER_SELECTION_STATE_MODIFY_ON)
            {
            if (characterReference->lineSegmentIndex == ((GRAPHICS_INT)segmentIndex))
              {
              objectError = drawLineSegment(hdc,
                                            nextLineSegment,
                                            LINE_SEGMENT_MODE_EDIT,
                                            (const strokeFrame_tPtr)characterFrame,
                                            (const canvasDescriptor_tPtr)canvasSize,
                                            (const strokeGraphPointBase_tPtr)strokeGraphBase);
              }
            else
              {
              objectError = drawLineSegment(hdc,
                                            nextLineSegment,
                                            LINE_SEGMENT_MODE_PASSIVE,
                                            (const strokeFrame_tPtr)characterFrame,
                                            (const canvasDescriptor_tPtr)canvasSize,
                                            (const strokeGraphPointBase_tPtr)strokeGraphBase);
              }
            }
          else
            {
            objectError = drawLineSegment(hdc,
                                          nextLineSegment,
                                          LINE_SEGMENT_MODE_PASSIVE,
                                          (const strokeFrame_tPtr)characterFrame,
                                          (const canvasDescriptor_tPtr)canvasSize,
                                          (const strokeGraphPointBase_tPtr)strokeGraphBase);
            }

          segmentIndex = segmentIndex + ((GRAPHICS_UINT)1);
          }

        objectError = computeCharacterExtents(characterSelectionState->selectedCharacter,
                                              characterList,
                                              strokeGraphBase);
        }
      else
        {
        // Deselect the character and save
        if ((characterSelectionState->thisSelectionState == CHARACTER_SELECTION_STATE_DESELECTED) && 
            (characterSelectionState->lastSelectionState == CHARACTER_SELECTION_STATE_SELECTED))
          {
          // Save the character definition and clear the canvas
          characterSelectionState->thisSelectionState = CHARACTER_SELECTION_STATE_NONE;
          characterSelectionState->lastSelectionState = CHARACTER_SELECTION_STATE_NONE;
          }
        else
          {
          if ((characterSelectionState->thisSelectionState == CHARACTER_SELECTION_STATE_NONE) && 
              (characterSelectionState->lastSelectionState == CHARACTER_SELECTION_STATE_NONE))
            {
            // General setup and character selection is only allowed out of edit mode
            // objectError = decodeCharacterSelection(characterSelectionState);
            }
          }
        }
      }
    
    // Cancel the last cursor movement request, if any...
    *strokeNavigationState = STROKE_NAV_STATE_NONE;

    hFont    = (HFONT)GetStockObject(ANSI_FIXED_FONT);

    hOldFont = (HFONT)SelectObject(hdc, hFont);

    objectError = mainCaption( hdc,
                              &captionText[0],
                              &localText[0],
                               strokeGraphBase,
                               (const strokeNavKeyStates_tPtr)strokeActiveCursor,
                               strokeStartCursor,
                               strokeEndCursor,
                               localCanvasSize,
                               characterReference,
                               characterSelectionState,
                               alphabetFileDescriptor,
                               alphabetSaveCounter,
                               caption);
    
    // On the flag, test to see if the mouse is in the (composite) objects' detection boundary
    if (objectPositionTestFlag == true)
      {
    
      }
    
    objectPositionTestFlag = false;
    }

/******************************************************************************/
  } /* end of OnPaint                                                         */

/******************************************************************************/
/* About() :                                                                  */
/* - message handler for about box.                                           */
/******************************************************************************/

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
  {
/******************************************************************************/

  UNREFERENCED_PARAMETER(lParam);

/******************************************************************************/

  switch (message)
    {
    case WM_INITDIALOG:
      return((INT_PTR)TRUE);
    
    case WM_COMMAND:
      if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
        EndDialog(hDlg, LOWORD(wParam));
        return((INT_PTR)TRUE);
        }
    break;
    }

/******************************************************************************/

  return((INT_PTR)FALSE);

/******************************************************************************/
  } /* end of About                                                           */

/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/
