/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/
/*                                                                            */
/* StrokeGraphicsAlphabet.h :                                                 */
/* Paul O'Brien                                                               */
/* 22.06.25                                                                   */
/*                                                                            */
/* 2D GUI to create stroke/vector graphic alphanumeric set                    */
/******************************************************************************/

#ifndef _STROKE_GRAPHICS_ALPHABET_H_
#define _STROKE_GRAPHICS_ALPHABET_H_

/******************************************************************************/

#include "StrokeGraphicsTypes.h"
#include "StrokeGraphicsUtilities.h"

/******************************************************************************/

// Alphabet file definitions
#define ALPHABET_HOME_DIRECTORY            ((GRAPHICS_WCHAR *)L"C:\\Users\\paulo\\source\\repos\\StrokeGraphics\\")
#define ALPHABET_NUMBER_FILE               ((GRAPHICS_WCHAR *)L"alphabetNumber.txt")
#define ALPHABET_FILE_PREFIX               ((GRAPHICS_WCHAR *)L"alphabetGraphics")
#define ALPHABET_FILE_POSTFIX              ((GRAPHICS_WCHAR *)L".txt")

#define ALPHABET_FILEPATH_MAXIMUM          (4096)

#define ALPHABET_NUMBER_STRING_LENGTH      (16)   // usually three digits plus a null or newline and a null
#define ALPHABET_NUMBER_LINE_LENGTH        (1024) // a pretty long line!

#define ALPHABET_NUMBER_RADIX_10           (10)

// Characters are defined from ASCII(0) to ASCII(127)
#define ALPHABET_SIZE_START                ((GRAPHICS_UINT)0)
#define ALPHABET_SIZE_END                  ((GRAPHICS_UINT)127)
#define ALPHABET_MINIMUM_SIZE              ((GRAPHICS_UINT)1)
#define ALPHABET_SIZE                      (ALPHABET_SIZE_END + ((GRAPHICS_UINT)1))

#define ALPHABET_ASCII_CHARACTER_MASK      ((GRAPHICS_INT)0x0000007F)
#define ALPHABET_ASCII_STRING_TERMINATOR   ((GRAPHICS_INT)0x00)

#define CHARACTER_CACHE_LENGTH             ((GRAPHICS_UINT)1)
#define CHARACTER_CACHE_INDEX              ((GRAPHICS_UINT)0)

typedef enum asciiNumericCharacters_t
  {
  ASCII_NUMERIC_0      = '0',
  ASCII_NUMERIC_1      = (ASCII_NUMERIC_0 + 1),
  ASCII_NUMERIC_2      = (ASCII_NUMERIC_1 + 1),
  ASCII_NUMERIC_3      = (ASCII_NUMERIC_2 + 1),
  ASCII_NUMERIC_4      = (ASCII_NUMERIC_3 + 1),
  ASCII_NUMERIC_5      = (ASCII_NUMERIC_4 + 1),
  ASCII_NUMERIC_6      = (ASCII_NUMERIC_5 + 1),
  ASCII_NUMERIC_7      = (ASCII_NUMERIC_6 + 1),
  ASCII_NUMERIC_8      = (ASCII_NUMERIC_7 + 1),
  ASCII_NUMERIC_9      = (ASCII_NUMERIC_8 + 1),
  ASCII_NUMERIC_VALUES = 10
  } asciiNumericCharacters_t;

#define ASCII_HEXANUMERIC_OFFSET ((GRAPHICS_UINT)0x0a)

typedef enum asciiHexaNumericNumbers_tTag
  {
  ASCII_NUMERIC_A          = 'A',
  ASCII_NUMERIC_B          = (ASCII_NUMERIC_A + 1),
  ASCII_NUMERIC_C          = (ASCII_NUMERIC_B + 1),
  ASCII_NUMERIC_D          = (ASCII_NUMERIC_C + 1),
  ASCII_NUMERIC_E          = (ASCII_NUMERIC_D + 1),
  ASCII_NUMERIC_F          = (ASCII_NUMERIC_E + 1),
  ASCII_NUMERIC_a          = 'a',
  ASCII_NUMERIC_b          = (ASCII_NUMERIC_a + 1),
  ASCII_NUMERIC_c          = (ASCII_NUMERIC_b + 1),
  ASCII_NUMERIC_d          = (ASCII_NUMERIC_c + 1),
  ASCII_NUMERIC_e          = (ASCII_NUMERIC_d + 1),
  ASCII_NUMERIC_f          = (ASCII_NUMERIC_e + 1),
  ASCII_HEXANUMERIC_VALUES = 12
  } asciiHexaNumericNumbers_t;

// Command-line boolean flags for saving the alphabet characters to a new 
// file or to overwrite the existing file
typedef enum alphabetFileModifications_tTag
  {
  STROKE_GRAPHICS_MODIFICATION_FLAG_N    = 0,
  STROKE_GRAPHICS_MODIFICATION_FLAG_Y    = (STROKE_GRAPHICS_MODIFICATION_FLAG_N + 1),
  STROKE_GRAPHICS_MODIFICATION_FLAG_NONE = (STROKE_GRAPHICS_MODIFICATION_FLAG_Y + 1),
  STROKE_GRAPHICS_MODIFICATION_FLAGS
  } alphabetFileModifications_t;

#define STROKE_GRAPHICS_ALPHABET_NONE       (0x0000)

typedef enum alphabetCharacterState_tTag
  {
  ALPHABET_CHARACTER_STATE_NONE = 0,
  ALPHABET_CHARACTER_STATE_DEFINED,
  ALPHABET_CHARACTER_STATES
  } alphabetCharacterState_t, *alphabetCharacterState_tPtr;

/******************************************************************************/

// Graphics character file delimiters
#define GRAPHICS_CHARACTER_SYMBOL_WIDTH            ((unsigned)(0x01))
#define GRAPHICS_POINT_VALUE_WIDTH                 ((unsigned)(0x08))

#define GRAPHICS_CHARACTER_START_DIMENSIONS_SYMBOL L'<'
#define GRAPHICS_CHARACTER_END_DIMENSIONS_SYMBOL   L'>'

#define GRAPHICS_CHARACTER_START_SYMBOL            L'{'
#define GRAPHICS_CHARACTER_END_SYMBOL              L'}'
                                                   
#define GRAPHICS_SEGMENT_START_SYMBOL              L'['
#define GRAPHICS_SEGMENT_END_SYMBOL                L']'
                                                   
#define GRAPHICS_SEGMENT_END_OF_LINE_SYMBOL        ((wchar_t)0x0a)
                                                   
#define GRAPHICS_SEGMENT_ANY_SYMBOL                ((wchar_t)0xffff) // match any symbol
#define GRAPHICS_SEGMENT_NULL_SYMBOL               ((wchar_t)0x0000) // dummy symbol - the symbol(s) requested is(are) implicit
                                                              // in the requested symbol type

typedef enum graphicsSymbolsTypes_tTag
  {
  GRAPHICS_FILE_SYMBOL_TYPE_NONE = 0,
  GRAPHICS_FILE_SYMBOL_TYPE_SYMBOL,
  GRAPHICS_FILE_SYMBOL_TYPE_DECIMAL,
  GRAPHICS_FILE_SYMBOL_TYPE_HEXADECIMAL,
  GRAPHICS_FILE_SYMBOL_TYPES
  } graphicsSymbolsTypes_t;

// The first character in a decimal number string and the decimal 
// "shift"/divide by 10
#define DECIMAL_CHARACTER_LEADING_WEIGHT     ((GRAPHICS_UINT)100000)
#define DECIMAL_CHARACTER_WEIGHT_SHIFT       ((GRAPHICS_UINT)10)

// The first character in a hexadecimal number string and the 
// hexadecimal "shift"/divide by 16
#define HEXADECIMAL_CHARACTER_LEADING_WEIGHT ((GRAPHICS_UINT)0x10000000)
#define HEXADECIMAL_CHARACTER_WEIGHT_SHIFT   ((GRAPHICS_UINT)0x10)

#pragma pack(push,1)
typedef struct alphabetFileDescriptor_tTag
  {
  GRAPHICS_ULONG              alphabetLoadNumber;
  GRAPHICS_ULONG              alphabetSaveNumber;
  alphabetFileModifications_t alphabetModificationFlag;
  GRAPHICS_WCHAR_PTR          alphabetFilePath;
  GRAPHICS_WCHAR_PTR          alphabetLineW;
  } alphabetFileDescriptor_t, *alphabetFileDescriptor_tPtr;
#pragma pack(pop)

// The output from a character session is a list of line segments
typedef enum alphabetFileParseStates_tTag
  {
  ALPHABET_PARSE_FILE_STATE_IDLE = 0,
  ALPHABET_PARSE_FILE_STATE_FILE_BEGINS,
  ALPHABET_FILE_STATE_DIMENSIONS_BEGIN,
  ALPHABET_FILE_STATE_DIMENSION_X,
  ALPHABET_FILE_STATE_DIMENSION_Y,
  ALPHABET_FILE_STATE_DIMENSIONS_END,
  ALPHABET_PARSE_FILE_STATE_CHARACTER_BEGINS,
  ALPHABET_PARSE_FILE_STATE_SEGMENT_BEGINS,
  ALPHABET_PARSE_FILE_STATE_SEGMENT_POINT_0_X,
  ALPHABET_PARSE_FILE_STATE_SEGMENT_POINT_0_Y,
  ALPHABET_PARSE_FILE_STATE_SEGMENT_POINT_1_X,
  ALPHABET_PARSE_FILE_STATE_SEGMENT_POINT_1_Y,
  ALPHABET_PARSE_FILE_STATE_SEGMENT_ENDS,
  ALPHABET_PARSE_FILE_STATE_CHARACTER_ENDS,
  ALPHABET_PARSE_FILE_STATE_FILE_ENDS,
  ALPHABET_PARSE_FILE_STATES
  } alphabetFileParseStates_t;

// Index to an array containing the four points that define a segment
typedef enum segmentPointIndicies_tTag
  {
  LINE_SEGMENT_POINT_ORIGIN_X = 0,
  LINE_SEGMENT_POINT_ORIGIN_Y,
  LINE_SEGMENT_POINT_DESTINATION_X,
  LINE_SEGMENT_POINT_DESTINATION_Y,
  LINE_SEGMENT_POINTS
  } segmentPointIndicies_t;

// Point as signed { x, y } relative to graph { y = 0, x = 0 }
#pragma pack(push,1)
typedef struct characterPoint_t
  {
  GRAPHICS_INT pointX;
  GRAPHICS_INT pointY;
  } characterPoint_t, *characterPoint_tPtr;
#pragma pack(pop)

// A single point
#pragma pack(push,1)
typedef struct singlePoint_tTag
  {
  GRAPHICS_REAL xAxisPoint;
  GRAPHICS_REAL yAxisPoint;
  } singlePoint_t, *singlePoint_tPtr;
#pragma pack(pop)

// Character extent/boundary relative to graph { y = 0, x = 0 }
#pragma pack(push,1)
typedef struct characterExtents_t
  {
  characterPoint_t topLeft;
  characterPoint_t bottomRight;
  } characterExtents_t, *characterExtents_tPtr;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct lineSegment_tTag
  {
  GRAPHICS_INT      lineSegmentOriginX;
  GRAPHICS_INT      lineSegmentOriginY;
  GRAPHICS_INT      lineSegmentDestinationX;
  GRAPHICS_INT      lineSegmentDestinationY;
  lineSegment_tTag *nextLineSegment;
  lineSegment_tTag *lastLineSegment;
  } lineSegment_t, *lineSegment_tPtr;
#pragma pack(pop)

// For line segment normalisation these structures are the 'REAL' counterparts 
// of the original character definition
#pragma pack(push,1)
typedef struct lineSegmentReal_tTag
  {
  GRAPHICS_REAL         lineSegmentOriginX;
  GRAPHICS_REAL         lineSegmentOriginY;
  GRAPHICS_REAL         lineSegmentDestinationX;
  GRAPHICS_REAL         lineSegmentDestinationY;
  lineSegmentReal_tTag *nextLineSegment;
  lineSegmentReal_tTag *lastLineSegment;
  } lineSegmentReal_t, *lineSegmentReal_tPtr;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct alphabetCharacters_tTag
  {
  GRAPHICS_UINT             characterNumber;
  alphabetCharacterState_t  characterState;
  lineSegment_tPtr          characterLineSegments;
  characterExtents_t        characterExtents;
  GRAPHICS_UINT             numberOfLineSegments;
  GRAPHICS_INT              lineSegmentIndex;
  alphabetCharacters_tTag  *nextCharacter;
  alphabetCharacters_tTag  *lastCharacter;
  } alphabetCharacters_t, *alphabetCharacters_tPtr;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct alphabetCharactersReal_tTag
  {
  GRAPHICS_UINT             characterNumber;
  alphabetCharacterState_t  characterState;
  lineSegmentReal_tPtr      characterLineSegments;
  characterExtents_t        characterExtents;
  GRAPHICS_UINT             numberOfLineSegments;
  GRAPHICS_INT              lineSegmentIndex;
  alphabetCharacters_tTag  *nextCharacter;
  alphabetCharacters_tTag  *lastCharacter;
  } alphabetCharactersReal_t, *alphabetCharactersReal_tPtr;
#pragma pack(pop)

/******************************************************************************/

extern alphabetCharacters_tPtr  characterListRoot;
extern alphabetCharacters_tPtr  characterCache;

extern alphabetFileDescriptor_t alphabetFileDescriptor;
extern GRAPHICS_WCHAR           alphabetFilePath[ALPHABET_FILEPATH_MAXIMUM];
extern GRAPHICS_WCHAR           alphabetLineW[ALPHABET_NUMBER_LINE_LENGTH];

/******************************************************************************/

extern graphicsError_t writeAlphabetToFile(      alphabetFileDescriptor_tPtr alphabetFileDescriptor,
                                           const alphabetCharacters_tPtr     characterListRoot,
                                           const strokeGraphPointBase_tPtr   strokeGraphBase);

extern graphicsError_t readAlphabetFromFile(const alphabetFileDescriptor_tPtr alphabetFileDescriptor,
                                                  alphabetCharacters_tPtr     characterListRoot,
                                                  singlePoint_tPtr            characterFrameDimensions);

extern graphicsError_t advanceSymbolIndex(const GRAPHICS_WCHAR_PTR     segmentLine,
                                          const graphicsSymbolsTypes_t symbolType, 
                                          const GRAPHICS_WCHAR         symbol,
                                          const GRAPHICS_UINT          symbolWidth,
                                                GRAPHICS_VOID_PTR      decodedSymbol,
                                                GRAPHICS_UINT_PTR      symbolIndex);

extern graphicsError_t computeCharacterExtents(const GRAPHICS_UINT             characterSelected,
                                               const alphabetCharacters_tPtr   alphabetCharacters,
                                               const strokeGraphPointBase_tPtr strokeGraphicBase);

extern graphicsError_t findNextWideTokenFile(FILE                  **wideCharacterFile, 
                                             GRAPHICS_WIDE_INT       wideCharacterTokenRequested, 
                                             GRAPHICS_WIDE_INT_PTR   wideCharacterTokenFound);

extern graphicsError_t isnum(GRAPHICS_INT character);

/******************************************************************************/

#endif

/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/