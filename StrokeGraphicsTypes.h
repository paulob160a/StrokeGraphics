/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/
/*                                                                            */
/* StrokeGraphicsTypes.h :                                                    */
/* Paul O'Brien                                                               */
/* 22.06.25                                                                   */
/*                                                                            */
/* 2D GUI to create stroke/vector graphic alphanumeric set                    */
/******************************************************************************/

#ifndef _STROKE_GRAPHICS_TYPES_H_
#define _STROKE_GRAPHICS_TYPES_H_

/******************************************************************************/

typedef          void         GRAPHICS_VOID,     *GRAPHICS_VOID_PTR;
typedef          bool         GRAPHICS_BOOLEAN,  *GRAPHICS_BOOLEAN_PTR;
typedef          char         GRAPHICS_CHAR,     *GRAPHICS_CHAR_PTR;
typedef unsigned char         GRAPHICS_UCHAR,    *GRAPHICS_UCHAR_PTR;
typedef unsigned char         GRAPHICS_BYTE,     *GRAPHICS_BYTE_PTR;
typedef          wchar_t      GRAPHICS_WCHAR,    *GRAPHICS_WCHAR_PTR;
typedef          short        GRAPHICS_SHORT,    *GRAPHICS_SHORT_PTR;
typedef unsigned short        GRAPHICS_USHORT,   *GRAPHICS_USHORT_PTR;
typedef          int          GRAPHICS_INT,      *GRAPHICS_INT_PTR;
typedef unsigned int          GRAPHICS_UINT,     *GRAPHICS_UINT_PTR;
typedef wint_t                GRAPHICS_WIDE_INT, *GRAPHICS_WIDE_INT_PTR;
typedef          long         GRAPHICS_LONG,     *GRAPHICS_LONG_PTR;
typedef unsigned long         GRAPHICS_ULONG,    *GRAPHICS_ULONG_PTR;
typedef          float        GRAPHICS_FLOAT,    *GRAPHICS_FLOAT_PTR;
typedef          double       GRAPHICS_DOUBLE,   *GRAPHICS_DOUBLE_PTR;
typedef          double       GRAPHICS_REAL,     *GRAPHICS_REAL_PTR;

/******************************************************************************/


typedef enum graphicsError_tTag
  {
  GRAPHICS_NO_ERROR = 0,
  GRAPHICS_OBJECT_PARAMETER_ERROR,
  GRAPHICS_OBJECT_INSTANTIATION_ERROR,
  GRAPHICS_OBJECT_FUNCTION_ERROR,
  GRAPHICS_OBJECT_RANGE_ERROR,
  GRAPHICS_OBJECT_TYPE_ERROR,
  GRAPHICS_OBJECT_SYMBOL_ERROR,
  GRAPHICS_OBJECT_FILE_ERROR,
  GRAPHICS_OBJECT_END_OF_FILE_ERROR,
  GRAPHICS_OBJECT_CHARACTER_ERROR,
  GRAPHICS_ERRORS
  } graphicsError_t;

/******************************************************************************/

#endif

/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/
