/** @file CurveCode.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : CurveCode.h
//
// Creation : 2011/06/23
//
// Version : 2011/06/23
//
// Author : JOL
//
// Summary : Header file for files CurveCode.ih and CurveCode.cxx
//
// History :
//	2011/06/23 : ?Name? : ?What?
//
// Rcs Id : "@(#)class CurveCode declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(CurveCode_RECURSES)
#error Recursive header files inclusion detected in CurveCode.h
#else // defined(CurveCode_RECURSES)
#define CurveCode_RECURSES

#if !defined CurveCode_h
#define CurveCode_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <deque>
#include "ImaGene/base/Vector2i.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class CurveCode
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'CurveCode' <p> Aim: a generalization of
   * freeman chaincodes that handle nicely spikes (like 02 or 13).
   */
  class CurveCode
  {

    // ----------------------- Standard services ------------------------------
  public:
    typedef std::deque<unsigned char> Storage;
    typedef Storage::const_iterator ConstIterator;
    
    /**
     * Destructor. 
     */
    ~CurveCode();

    /**
     * Constructor. Empty chain.
     */
    CurveCode();

    /**
     * Copy constructor.
     * @param other the object to clone.
     */
    CurveCode( const CurveCode & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     */
    CurveCode & operator=( const CurveCode & other );

    /**
       @return the start point of the curve code (pointel if interpixel).
    */
    Vector2i startPoint() const;

    /**
       @return 'true' iff the curve is closed.
    */
    bool isClosed() const;

    /**
       @return 'true' iff the curve is considered clockwise.
    */
    bool isCW() const;

    /**
       @return the freeman code (0-3) of the first step.
    */
    unsigned char startStep() const;

    /**
       @return an iterator on the first code of the curve.
       
       0: go forward
       1: plus
       3: minus
       2: go backward (invalid).
    */
    ConstIterator begin() const;

    /**
       @return an iterator on the first code of the curve.
       
       0: go forward
       1: plus
       3: minus
       2: go backward (invalid).
    */
    ConstIterator end() const;

    /**
       @param pt the coordinates of the first pointel.  @param cw if
       'true', inside is to the right.  @param fchain a valid freeman
       chaincode as a string (e.g. "0001122...").
    */
    void init( const Vector2i & pt, bool cw, const std::string & fchain );
    
    /**
       Computes the inner or outer curve of this curve, assuming this
       is an interpixel curve. These curves lie in the half integer
       plane wrt this one.

       NB: If this curve is closed, then this curve should be shifted
       beforehands so as to start either in a zone with at least two
       '=' (like "[==...]"), or in a zone between a '+' and a '-'
       (like "[=+.....-]").
       @see shiftForInnerOrOuterCurve

       @param inner tells whether we compute the inner curve (inside
       side) or the outer curve (outside side).

       @param twiceDV (returns) twice the displacement vector to go
       from this integer plane to the returned half integer plane.
       
       @return the selected curve.
    */
    CurveCode computeInnerOrOuterCurve( bool inner, 
					Vector2i & twiceDV ) const;

    /**
       Shift once the curve code (advances of one code). Only valid
       for closed curve.
    */
    void shift();

    /**
       Shift the curve so as to start in a zone with at least two '='
       (like "[==...]"), or in a zone between a '+' and a '-' (like
       "[=-.....+]" or "[=+.....-=]" but never like "[=+.....-]").

       @return the number of shifts actually performed.
    */
    unsigned int shiftForInnerOrOuterCurve();

    // ----------------------- Interface --------------------------------------
  public:

    /**
     * Writes/Displays the object on an output stream.
     * @param that_stream the output stream where the object is written.
     */
    void selfDisplay( std::ostream & that_stream ) const;

    /**
     * Checks the validity/consistency of the object.
     * @return 'true' if the object is valid, 'false' otherwise.
     */
    bool OK() const;
  
    // ------------------------- Datas ----------------------------------------
  private:

    Vector2i myStartPt;
    bool myIsClosed;
    bool myCW;
    /**
       first character is start code (0,1,2,3).
    */
    unsigned char myStartStep;
    /**
       Vector of codes
       0: go forward
       1: plus
       3: minus
       2: go backward (invalid).
    */
    Storage myCodes;


    // ------------------------- Hidden services ------------------------------
  protected:

  private:

  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'CurveCode'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'CurveCode' to write.
   * @return the output stream after the writing.
   */
  std::ostream&
  operator<<( std::ostream & that_stream, 
	      const CurveCode & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/dgeometry2d/CurveCode.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined CurveCode_h

#undef CurveCode_RECURSES
#endif // else defined(CurveCode_RECURSES)
