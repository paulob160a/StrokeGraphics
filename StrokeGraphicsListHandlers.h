/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/
/*                                                                            */
/* StrokeGraphicsListHandlers.h :                                             */
/* Paul O'Brien                                                               */
/* 28.06.25                                                                   */
/*                                                                            */
/* 2D GUI to create stroke/vector graphic alphanumeric set                    */
/******************************************************************************/

#ifndef _STROKE_GRAPHICS_LIST_HANDLERS_H_
#define _STROKE_GRAPHICS_LIST_HANDLERS_H_

/******************************************************************************/

#include "StrokeGraphicsTypes.h"
#include "StrokeGraphicsAlphabet.h"

/******************************************************************************/

extern graphicsError_t buildCharacterList(const GRAPHICS_UINT            characterListLength,
                                                alphabetCharacters_tPtr *alphabetCharacters);

extern graphicsError_t addLineSegmentToCharacterList(const GRAPHICS_UINT           characterSelected,
                                                     const lineSegment_tPtr        newLineSegment,
                                                           alphabetCharacters_tPtr alphabetCharacters);

extern graphicsError_t normaliseCharacterSegments(const alphabetCharacters_tPtr      characterReference,
                                                       alphabetCharactersReal_tPtr *normalisedReference);

extern graphicsError_t copyCharacterOneToCharacterTwo(const GRAPHICS_UINT           characterOne,
                                                      const GRAPHICS_UINT           characterTwo, 
                                                            alphabetCharacters_tPtr characterListOne,
                                                            alphabetCharacters_tPtr characterListTwo);

extern graphicsError_t fetchCharacterReference(const GRAPHICS_UINT            characterSelected,
                                               const alphabetCharacters_tPtr  alphabetCharacters,
                                                     alphabetCharacters_tPtr *characterReference);

extern graphicsError_t fetchCharacterExtantSegment(const GRAPHICS_INT             segmentNumber,
                                                   const alphabetCharacters_tPtr  alphabetCharacterReference,
                                                         lineSegment_tPtr        *newLineSegment);

extern graphicsError_t deleteCharacter(alphabetCharacters_tPtr selectedCharacter);

extern graphicsError_t deleteCharacterExtantSegment(alphabetCharacters_tPtr alphabetCharacters,
                                                    lineSegment_tPtr        lineSegment);

extern graphicsError_t flipCharacterVertically(alphabetCharacters_tPtr selectedCharacter);
extern graphicsError_t flipCharacterHorizontally(alphabetCharacters_tPtr selectedCharacter);

/******************************************************************************/

#endif

/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/