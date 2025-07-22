/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/
/*                                                                            */
/* StrokeGraphicsListHandlers.cpp :                                           */
/* Paul O'Brien                                                               */
/* 09.06.25                                                                   */
/*                                                                            */
/* 2D GUI to create stroke/vector graphic alphanumeric set                    */
/******************************************************************************/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include "StrokeGraphicsAlphabet.h"
#include "StrokeGraphicsCaption.h"
#include "StrokeGraphicsListHandlers.h"

/******************************************************************************/

alphabetCharacters_tPtr characterListRoot = nullptr;
alphabetCharacters_tPtr characterCache    = nullptr;

/******************************************************************************/
/* buildCharacterList() :                                                     */
/******************************************************************************/

graphicsError_t buildCharacterList(const GRAPHICS_UINT            characterListLength,
                                         alphabetCharacters_tPtr *alphabetCharacters)
  {
/******************************************************************************/

  GRAPHICS_UINT           listIndex     = ((GRAPHICS_UINT)0);

  alphabetCharacters_tPtr newCharacter  = nullptr,
                          lastCharacter = nullptr;

  lineSegment_tPtr        newSegment    = nullptr;
                                        
  graphicsError_t         objectError   = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((characterListLength >= ALPHABET_MINIMUM_SIZE) && ( characterListLength <= ALPHABET_SIZE) && 
      (alphabetCharacters  != nullptr)               && (*alphabetCharacters  == nullptr))
    {
    for (listIndex = ((GRAPHICS_UINT)0); listIndex < characterListLength; listIndex++)
      {
      if ((newCharacter = (alphabetCharacters_tPtr)calloc((size_t(1)), ((size_t)sizeof(alphabetCharacters_t)))) != nullptr)
        {
        // The first line segment descriptor is instantiated here
        if ((newSegment = (lineSegment_tPtr)calloc(((size_t)1), sizeof(lineSegment_t))) != nullptr)
          {         
          newCharacter->characterNumber       = listIndex;
          newCharacter->characterState        = ALPHABET_CHARACTER_STATE_NONE;
          newCharacter->characterLineSegments = newSegment;
          newCharacter->numberOfLineSegments  = ((GRAPHICS_UINT)0);
          newCharacter->lineSegmentIndex      = ((GRAPHICS_UINT)0); // used to step through the line indices during modification/editing mode
          newCharacter->lastCharacter         = nullptr;
          newCharacter->nextCharacter         = nullptr;
          
          if (lastCharacter == nullptr)
            {
             lastCharacter      = newCharacter;
            *alphabetCharacters = lastCharacter;
            }
          else
            {
            lastCharacter->nextCharacter = newCharacter;
            newCharacter->lastCharacter  = lastCharacter;
            lastCharacter                = newCharacter;
            }
          }
        else
          {
          objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
          break;
          }
        }
      else
        {
        objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
        break;
        }
      }

    // Complete the loop
    if (objectError == GRAPHICS_NO_ERROR)
      {
       newCharacter->nextCharacter          = *alphabetCharacters;
       (*alphabetCharacters)->lastCharacter =  newCharacter;
      }
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of buildCharacterList                                              */

/******************************************************************************/
/* addLineSegmentToCharacterList() :                                          */
/******************************************************************************/

graphicsError_t addLineSegmentToCharacterList(const GRAPHICS_UINT           characterSelected,
                                              const lineSegment_tPtr        newLineSegment,
                                                    alphabetCharacters_tPtr alphabetCharacters)
  {
/******************************************************************************/

  GRAPHICS_UINT    characterIndex    = ((GRAPHICS_UINT)0);

  lineSegment_tPtr newSegment        = nullptr,
                   characterSegments = nullptr;

  graphicsError_t  objectError       = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((characterSelected < ALPHABET_SIZE) && (newLineSegment != nullptr) && (alphabetCharacters != nullptr))
    {
    if ((newSegment = (lineSegment_tPtr)calloc(((size_t)1), sizeof(lineSegment_t))) != nullptr)
      {
      // Select the current character
      for (characterIndex = ((GRAPHICS_UINT)0); characterIndex < characterSelected; characterIndex++)
        {
        alphabetCharacters = alphabetCharacters->nextCharacter;
        }

      characterSegments = alphabetCharacters->characterLineSegments;

      while (characterSegments->nextLineSegment != nullptr)
        {
        characterSegments = characterSegments->nextLineSegment;
        }

      // The 'current segment' always points to an instantiated line segment descriptor, but there 
      // is no reason to save points ( origin(x,y) == destination(x,y) )
      if ((newLineSegment->lineSegmentOriginX != newLineSegment->lineSegmentDestinationX) || 
          (newLineSegment->lineSegmentOriginY != newLineSegment->lineSegmentDestinationY))
        {
        // Segment is good, save the latest number of segments
        alphabetCharacters->numberOfLineSegments   = alphabetCharacters->numberOfLineSegments + ((GRAPHICS_UINT)1);

        characterSegments->lineSegmentOriginX      = newLineSegment->lineSegmentOriginX;
        characterSegments->lineSegmentOriginY      = newLineSegment->lineSegmentOriginY;
        characterSegments->lineSegmentDestinationX = newLineSegment->lineSegmentDestinationX;
        characterSegments->lineSegmentDestinationY = newLineSegment->lineSegmentDestinationY;
       
        characterSegments->nextLineSegment         = newSegment;
        newSegment->lastLineSegment                = characterSegments;
        }
      else
        {
        objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
        free(newSegment);
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

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of addLineSegmentToCharacterList                                   */

/******************************************************************************/
/* copyCharacterOneToCharacterTwo() :                                         */
/* - if character one's segment definitions exist and character two's segment */
/*   definitions do NOT exist, copy character one's segments to character     */
/*   two. Characters can be copied within the same list or between lists      */
/******************************************************************************/

graphicsError_t copyCharacterOneToCharacterTwo(const GRAPHICS_UINT           characterOne,
                                               const GRAPHICS_UINT           characterTwo, 
                                                     alphabetCharacters_tPtr characterListOne,
                                                     alphabetCharacters_tPtr characterListTwo)
  {
/******************************************************************************/

  GRAPHICS_UINT           characterOneIndex    = ((GRAPHICS_UINT)0),
                          characterTwoIndex    = ((GRAPHICS_UINT)0);

  lineSegment_t           newLineSegment       = { 
                                                 ((GRAPHICS_UINT)0), 
                                                 ((GRAPHICS_UINT)0), 
                                                 ((GRAPHICS_UINT)0), 
                                                 ((GRAPHICS_UINT)0),
                                                 nullptr, 
                                                 nullptr
                                                 };

  lineSegment_tPtr        characterOneSegments = nullptr,
                          characterTwoSegments = nullptr;

  alphabetCharacters_tPtr characterOneOffset   = nullptr,
                          characterTwoOffset   = nullptr;
                                               
  graphicsError_t         objectError          = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((characterListOne != nullptr) && (characterListTwo != nullptr))
    {
    // Characters in the same list cannot have the same number
    if ((characterOne == characterTwo) && (characterListOne == characterListTwo))
      {
      objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
      }
    else
      {
      if ((characterOne  < ALPHABET_SIZE) && (characterTwo     <  ALPHABET_SIZE))
        {
        characterOneOffset = characterListOne;
        characterTwoOffset = characterListTwo;
        
        // Select character one
        for (characterOneIndex = ((GRAPHICS_UINT)0); characterOneIndex < characterOne; characterOneIndex++)
          {
          characterOneOffset = characterOneOffset->nextCharacter;
          }
        
        // Does it have one or more character segments defined ?
        characterOneSegments = characterOneOffset->characterLineSegments;
        
        if (characterOneSegments->nextLineSegment != nullptr)
          {
          // Select character two
          for (characterTwoIndex = ((GRAPHICS_UINT)0); characterTwoIndex < characterTwo; characterTwoIndex++)
            {
            characterTwoOffset = characterTwoOffset->nextCharacter;
            }
        
          // No segments should be defined
          characterTwoSegments = characterTwoOffset->characterLineSegments;
        
          if (characterTwoSegments->nextLineSegment == nullptr)
            {
            while (characterOneSegments->nextLineSegment != nullptr)
              {
              // Copy character one to character two, one segment at a time
              newLineSegment.lineSegmentOriginX      = characterOneSegments->lineSegmentOriginX;
              newLineSegment.lineSegmentOriginY      = characterOneSegments->lineSegmentOriginY;
              newLineSegment.lineSegmentDestinationX = characterOneSegments->lineSegmentDestinationX;
              newLineSegment.lineSegmentDestinationY = characterOneSegments->lineSegmentDestinationY;
        
              if ((objectError = addLineSegmentToCharacterList( characterTwo,
                                                               &newLineSegment,
                                                                characterListTwo)) == GRAPHICS_NO_ERROR)
                {
                characterOneSegments = characterOneSegments->nextLineSegment;
                }
              else
                {
                objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
                break;
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
          objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
          }
        }
      else
        {
        objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
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
  } /* end of copyCharacterOneToCharacterTwo                                  */

/******************************************************************************/
/* fetchCharacterReference() :                                                */
/******************************************************************************/

graphicsError_t fetchCharacterReference(const GRAPHICS_UINT            characterSelected,
                                        const alphabetCharacters_tPtr  alphabetCharacters,
                                              alphabetCharacters_tPtr *characterReference)
  {
/******************************************************************************/

  GRAPHICS_UINT   characterIndex = ((GRAPHICS_UINT)0);

  graphicsError_t objectError    = GRAPHICS_NO_ERROR;

/******************************************************************************/

    if ((characterSelected   < ALPHABET_SIZE) && (alphabetCharacters != nullptr) &&
        (characterReference != nullptr))
      {
      *characterReference = alphabetCharacters;

       // Select the requested character reference
      for (characterIndex = ((GRAPHICS_UINT)0); characterIndex < characterSelected; characterIndex++)
        {
        *characterReference = (*characterReference)->nextCharacter;
        }
      }
    else
      {
      objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
      }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of fetchCharacterReference                                         */

/******************************************************************************/
/* fetchCharacterExtantSegment() :                                            */
/******************************************************************************/

graphicsError_t fetchCharacterExtantSegment(const GRAPHICS_INT             segmentNumber,
                                            const alphabetCharacters_tPtr  alphabetCharacterReference,
                                                  lineSegment_tPtr        *newLineSegment)
  {
/******************************************************************************/

  lineSegment_tPtr segmentReference = nullptr;
  GRAPHICS_UINT    segmentIndex     = ((GRAPHICS_INT)-1); // the first occupied segment index will increment to zero
                                    
  graphicsError_t  objectError      = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((newLineSegment != nullptr) && (alphabetCharacterReference != nullptr))
    {
    segmentReference = alphabetCharacterReference->characterLineSegments;

    // Check if there are any extant line segments - there is always an empty 
    // segment structure at the end of the list that points to nothing
    if (segmentReference->nextLineSegment != nullptr)
      {
      while (segmentReference->nextLineSegment != nullptr)
        {
        // Prepare to return the reference to the segment contents
        *newLineSegment = segmentReference;

         segmentIndex   = segmentIndex + ((GRAPHICS_INT)1);

        // The loop will stop if the list is too short i.e. the segment at index 'n' 
        // doesn't exist. There is no such thing as "too long"!
        if (segmentIndex == segmentNumber)
          {
          break;
          }
        else
          {
          segmentReference = segmentReference->nextLineSegment;
          }
        }

      // If the list is too short...
      if (segmentIndex != segmentNumber)
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

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* fetchCharacterExtantSegment                                            */

/******************************************************************************/
/* deleteCharacter() :                                                        */
/******************************************************************************/

graphicsError_t deleteCharacter(alphabetCharacters_tPtr selectedCharacter)
  {
/******************************************************************************/

  lineSegment_tPtr thislineSegment = nullptr;
  graphicsError_t  objectError      = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((selectedCharacter != nullptr) && (selectedCharacter->characterLineSegments != nullptr))
    {
    if (selectedCharacter->numberOfLineSegments > ((GRAPHICS_UINT)0))
      {
      thislineSegment                     = selectedCharacter->characterLineSegments;
      selectedCharacter->lineSegmentIndex = (GRAPHICS_INT)(selectedCharacter->numberOfLineSegments - ((GRAPHICS_UINT)1));
      
      // Iterate to the segment one place from the end of the list of character line segments
      /*while(thislineSegment->nextLineSegment->nextLineSegment != nullptr)
        {
        selectedCharacter->lineSegmentIndex = selectedCharacter->lineSegmentIndex + ((GRAPHICS_INT)1);
        thislineSegment                     = thislineSegment->nextLineSegment;
        }*/
      
      while ((selectedCharacter->lineSegmentIndex >= ((GRAPHICS_INT)0)) && (selectedCharacter->numberOfLineSegments > ((GRAPHICS_UINT)0)))
        {
        if (fetchCharacterExtantSegment(  selectedCharacter->lineSegmentIndex,
                                          selectedCharacter,
                                         &thislineSegment) == GRAPHICS_NO_ERROR)
          {
          if (deleteCharacterExtantSegment(selectedCharacter,
                                           thislineSegment) != GRAPHICS_NO_ERROR)
            {
            objectError = GRAPHICS_OBJECT_CHARACTER_ERROR;
            break;
            }
          }
        else
          {
          objectError = GRAPHICS_OBJECT_CHARACTER_ERROR;
          break;
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
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of deleteCharacter                                                 */

/******************************************************************************/
/* deleteCharacterExtantSegment() :                                           */
/******************************************************************************/

graphicsError_t deleteCharacterExtantSegment(alphabetCharacters_tPtr alphabetCharacters,
                                             lineSegment_tPtr        lineSegment)
  {
/******************************************************************************/

  graphicsError_t objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((alphabetCharacters != nullptr) && (lineSegment != nullptr))
    {
    if (lineSegment->nextLineSegment != nullptr)
      {
      // Cut the segment out of the list
      if (alphabetCharacters->lineSegmentIndex == ((GRAPHICS_INT)0))
        { // The head of the segment list now points to the next segment descriptor
        alphabetCharacters->characterLineSegments                  = lineSegment->nextLineSegment;
        alphabetCharacters->characterLineSegments->lastLineSegment = nullptr;

        alphabetCharacters->numberOfLineSegments                   = alphabetCharacters->numberOfLineSegments - ((GRAPHICS_UINT)1);
        }
      else
        {
        lineSegment->lastLineSegment->nextLineSegment = lineSegment->nextLineSegment;
        lineSegment->nextLineSegment->lastLineSegment = lineSegment->lastLineSegment;

        alphabetCharacters->numberOfLineSegments      = alphabetCharacters->numberOfLineSegments - ((GRAPHICS_UINT)1);
        alphabetCharacters->lineSegmentIndex          = alphabetCharacters->lineSegmentIndex     - ((GRAPHICS_UINT)1);
        }

      free(lineSegment);
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

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of deleteCharacterExtantSegment                                    */

/******************************************************************************/
/* flipCharacterHorizontally() :                                              */
/******************************************************************************/

graphicsError_t flipCharacterHorizontally(alphabetCharacters_tPtr selectedCharacter)
  {
/******************************************************************************/

  lineSegment_tPtr lineSegment   = nullptr;
                                 
  GRAPHICS_INT     extentTop     = ((GRAPHICS_INT)0),
                   extentBottom  = ((GRAPHICS_INT)0);
                   
  GRAPHICS_FLOAT   midLine       = ((GRAPHICS_FLOAT)0),
                   pointDistance = ((GRAPHICS_FLOAT)0),
                   pointY        = ((GRAPHICS_FLOAT)0);
                   
  graphicsError_t  objectError   = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if (selectedCharacter != nullptr)
    {
    if (selectedCharacter->numberOfLineSegments > ((GRAPHICS_UINT)1))
      {
      selectedCharacter->lineSegmentIndex = selectedCharacter->numberOfLineSegments - ((GRAPHICS_UINT)1);

      extentTop    = selectedCharacter->characterExtents.topLeft.pointY;
      extentBottom = selectedCharacter->characterExtents.bottomRight.pointY;

      if (extentBottom != extentTop) // cannot flip a linear 'shape'!
        {
        if (extentBottom > (extentTop + ((GRAPHICS_INT)1))) // points are seperated by a mid-line
          {
          // A definite mid-line...for an even extent in 'x' the mid-line is an integer but 
          // for an odd extent it will be 'nnn.05'
          midLine     =  ((GRAPHICS_FLOAT)extentTop) + ((((GRAPHICS_FLOAT)extentBottom) - ((GRAPHICS_FLOAT)extentTop)) / ((GRAPHICS_FLOAT)2.0));

          lineSegment = selectedCharacter->characterLineSegments;
       
          while (lineSegment->nextLineSegment != nullptr)
            {
            pointY = ((GRAPHICS_FLOAT)lineSegment->lineSegmentOriginY);

            if (pointY < midLine)
              {
              pointDistance = midLine - pointY;

              lineSegment->lineSegmentOriginY = ((GRAPHICS_INT)(midLine + pointDistance));
              }
            else
              {
              if (pointY > midLine)
                {
                pointDistance = pointY - midLine;

                lineSegment->lineSegmentOriginY = ((GRAPHICS_INT)(midLine - pointDistance));
                }
              else
                {
                lineSegment->lineSegmentOriginY = (GRAPHICS_INT)midLine;
                }
              }

            pointY = ((GRAPHICS_FLOAT)lineSegment->lineSegmentDestinationY);

            if (pointY < midLine)
              {
              pointDistance = midLine - pointY;

              lineSegment->lineSegmentDestinationY = ((GRAPHICS_INT)(midLine + pointDistance));
              }
            else
              {
              if (pointY > midLine)
                {
                pointDistance = pointY - midLine;

                lineSegment->lineSegmentDestinationY = ((GRAPHICS_INT)(midLine - pointDistance));
                }
              else
                {
                lineSegment->lineSegmentDestinationY = (GRAPHICS_INT)midLine;
                }
              }

            lineSegment = lineSegment->nextLineSegment;
            }
          }
        else
          {
          if (extentBottom > extentTop) // all points are seperated by 0 or +1
            {
            lineSegment = selectedCharacter->characterLineSegments;

            while (lineSegment->nextLineSegment != nullptr)
              {
              if (lineSegment->lineSegmentOriginY == extentBottom)
                {
                lineSegment->lineSegmentOriginY = extentTop;
                }
              else
                {
                lineSegment->lineSegmentOriginY = extentBottom;
                }

              if (lineSegment->lineSegmentDestinationY == extentBottom)
                {
                lineSegment->lineSegmentDestinationY = extentTop;
                }
              else
                {
                lineSegment->lineSegmentDestinationY = extentBottom;
                }

              lineSegment = lineSegment->nextLineSegment;
              }
            }
          else
            {
            // What ?
            objectError = GRAPHICS_OBJECT_RANGE_ERROR;
            }
          }
        }
      }
    else
      {
      objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
      }

    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of flipCharacterHorizontally                                       */

/******************************************************************************/
/* flipCharacterVertically :                                                  */
/* - note "left" and "right" imply that "left" coordinate values are smaller  */
/*   than "right coordinate values                                            */
/******************************************************************************/

graphicsError_t flipCharacterVertically(alphabetCharacters_tPtr selectedCharacter)
  {
/******************************************************************************/

  lineSegment_tPtr lineSegment   = nullptr;
                                 
  GRAPHICS_INT     extentLeft    = ((GRAPHICS_INT)0),
                   extentRight   = ((GRAPHICS_INT)0);
                   
  GRAPHICS_FLOAT   midLine       = ((GRAPHICS_FLOAT)0),
                   pointDistance = ((GRAPHICS_FLOAT)0),
                   pointX        = ((GRAPHICS_FLOAT)0);
                   
  graphicsError_t  objectError   = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if (selectedCharacter != nullptr)
    {
    if (selectedCharacter->numberOfLineSegments > ((GRAPHICS_UINT)1))
      {
      selectedCharacter->lineSegmentIndex = selectedCharacter->numberOfLineSegments - ((GRAPHICS_UINT)1);

      extentLeft  = selectedCharacter->characterExtents.topLeft.pointX;
      extentRight = selectedCharacter->characterExtents.bottomRight.pointX;

      if (extentRight != extentLeft) // cannot flip a linear 'shape'!
        {
        if (extentRight > (extentLeft + ((GRAPHICS_INT)1))) // points are seperated by a mid-line
          {
          // A definite mid-line...for an even extent in 'x' the mid-line is an integer but 
          // for an odd extent it will be 'nnn.05'
          midLine     = ((GRAPHICS_FLOAT)extentLeft) + ((((GRAPHICS_FLOAT)extentRight) - ((GRAPHICS_FLOAT)extentLeft)) / ((GRAPHICS_FLOAT)2.0));

          lineSegment = selectedCharacter->characterLineSegments;
       
          while (lineSegment->nextLineSegment != nullptr)
            {
            pointX = ((GRAPHICS_FLOAT)lineSegment->lineSegmentOriginX);

            if (pointX < midLine)
              {
              pointDistance = midLine - pointX;

              lineSegment->lineSegmentOriginX = ((GRAPHICS_INT)(midLine + pointDistance));
              }
            else
              {
              if (pointX > midLine)
                {
                pointDistance = pointX - midLine;

                lineSegment->lineSegmentOriginX = ((GRAPHICS_INT)(midLine - pointDistance));
                }
              else
                {
                lineSegment->lineSegmentOriginX = (GRAPHICS_INT)midLine;
                }
              }

            pointX = ((GRAPHICS_FLOAT)lineSegment->lineSegmentDestinationX);

            if (pointX < midLine)
              {
              pointDistance = midLine - pointX;

              lineSegment->lineSegmentDestinationX = ((GRAPHICS_INT)(midLine + pointDistance));
              }
            else
              {
              if (pointX > midLine)
                {
                pointDistance = pointX - midLine;

                lineSegment->lineSegmentDestinationX = ((GRAPHICS_INT)(midLine - pointDistance));
                }
              else
                {
                lineSegment->lineSegmentDestinationX = (GRAPHICS_INT)midLine;
                }
              }

            lineSegment = lineSegment->nextLineSegment;
            }
          }
        else
          {
          if (extentRight > extentLeft) // all points are seperated by 0 or +1
            {
            lineSegment = selectedCharacter->characterLineSegments;

            while (lineSegment->nextLineSegment != nullptr)
              {
              if (lineSegment->lineSegmentOriginX == extentRight)
                {
                lineSegment->lineSegmentOriginX = extentLeft;
                }
              else
                {
                lineSegment->lineSegmentOriginX = extentRight;
                }

              if (lineSegment->lineSegmentDestinationX == extentRight)
                {
                lineSegment->lineSegmentDestinationX = extentLeft;
                }
              else
                {
                lineSegment->lineSegmentDestinationX = extentRight;
                }

              lineSegment = lineSegment->nextLineSegment;
              }
            }
          else
            {
            // What ?
            objectError = GRAPHICS_OBJECT_RANGE_ERROR;
            }
          }
        }
      }
    else
      {
      objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
      }
    }


/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of flipCharacterVertically                                         */

/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/