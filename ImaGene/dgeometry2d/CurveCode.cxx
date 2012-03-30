///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : CurveCode.cxx
//
// Creation : 2011/06/23
//
// Version : 2011/06/23
//
// Author : Jacques-Olivier Lachaud
//
// email : lachaud@labri.fr
//
// Purpose : ??
//
// Distribution :
//
// Use :
//	??
//
// Todo :
//	O ??
//
// History :
//	2011/06/23 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <deque>
#include <algorithm>
#include "ImaGene/dgeometry2d/FreemanChain.h"
#include "ImaGene/dgeometry2d/CurveCode.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/dgeometry2d/CurveCode.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const CurveCode_RCS_ID = "@(#)class CurveCode definition.";



///////////////////////////////////////////////////////////////////////////////
// class CurveCode
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::CurveCode::~CurveCode()
{
}

/**
 * Constructor. Empty chain.
 */
ImaGene::CurveCode::CurveCode()
{
}

/**
   @param pt the coordinates of the first pointel.  @param cw if
   'true', inside is to the right.  @param fchain a valid freeman
   chaincode as a string (e.g. "0001122...").
*/
void 
ImaGene::CurveCode::init
( const Vector2i & pt, bool cw, const std::string & fchain )
{
  myStartPt = pt;
  myCW = cw;
  Vector2i current( pt );
  Vector2i delta;
  myStartStep = (unsigned char) ( fchain[ 0 ] - '0' );
  myIsClosed = false;
  for ( unsigned int i = 0; i < fchain.size(); ++i )
    {
      myCodes.push_back( 0 );
      FreemanChain::displacement( delta.x(), delta.y(), fchain[ i ] - '0' );
      current += delta;
      if ( i == ( fchain.size() - 1 ) ) 
	myIsClosed = current == myStartPt;
      if ( ( i < ( fchain.size() - 1 ) ) || myIsClosed )
	{
	  unsigned int move = 
	    FreemanChain::movement( fchain[ i ] - '0',
				    fchain[ ( i + 1 ) % fchain.size() ] - '0', 
				    ! cw );
	  ASSERT_CurveCode( move != 0 );
	  if ( move != 2 ) myCodes.push_back( move );
	}
    } 
}

/**
   Computes the inner or outer curve of this curve, assuming this is
   an interpixel curve. These curves lie in the half integer plane wrt
   this one.

   @param inner tells whether we compute the inner curve (inside
   side) or the outer curve (outside side).
   
   @param twiceDV (returns) twice the displacement vector to go
   from this integer plane to the returned half integer plane.
   
   @return the selected curve.
*/
ImaGene::CurveCode 
ImaGene::CurveCode::computeInnerOrOuterCurve( bool inner, 
					      Vector2i & twiceDV ) const
{
  // Compute displacement vector (depends on contour orientation and
  // whether we wish to extract the inner contour or the outer
  // contour).
  Vector2i delta1, delta2;
  FreemanChain::displacement( delta1.x(), delta1.y(), myStartStep );
  bool turn_ccw = inner ? ( ! myCW ) : myCW; 
  unsigned char next_code = FreemanChain::turnedCode( myStartStep, 
						      turn_ccw );
  FreemanChain::displacement( delta2.x(), delta2.y(), next_code );
  twiceDV = delta1 + delta2;
  // Compute new curve code.
  CurveCode cc;
  cc.myStartPt = myStartPt;
  cc.myIsClosed = myIsClosed;
  cc.myCW = myCW;
  cc.myStartStep = myStartStep;
  unsigned char previous_turn = 2; // the previous turn, invalid at start.
  for ( std::deque<unsigned char>::const_iterator 
	  it = myCodes.begin(),
	  it_end = myCodes.end(); it != it_end; ++it )
    {
      unsigned char code = *it;
      switch ( code )
	{
	case 0: cc.myCodes.push_back( 0 ); break;
	case 1: 
	  if ( code == previous_turn )
	    {
	      if ( inner ) cc.myCodes.pop_back();
	      else cc.myCodes.push_back( 0 );
	    }
	  else previous_turn = code;
	  cc.myCodes.push_back( code );
	  break;
	case 3: 
	  if ( code == previous_turn )
	    {
	      if ( ! inner ) cc.myCodes.pop_back();
	      else cc.myCodes.push_back( 0 );
	    }
	  else previous_turn = code;
	  cc.myCodes.push_back( code );
	  break;
	case 2: ASSERT_CurveCode( false 
				  && "[CurveCode::computeInnerOrOuterCurve] invalide curve code in input." );
	}
    }
  if ( myIsClosed )
    {
      // previous_turn contains the value of the last turn. We search for the
      // first turn.
      std::deque<unsigned char>::const_iterator it = myCodes.begin();
      while ( *it == 0 ) 
	++it;
      if ( *it == previous_turn )
	{
	  if ( previous_turn == 1 )
	    {
	      if ( inner ) cc.myCodes.pop_front();
	      else cc.myCodes.push_front( 0 );
	    }
	  else
	    {
	      if ( ! inner ) cc.myCodes.pop_front();
	      else cc.myCodes.push_front( 0 );
	    }
	}
    }
  return cc;
}

void 
ImaGene::CurveCode::shift()
{
  if ( ! myIsClosed ) return;
  if ( myCodes.size() == 0 ) return;
  unsigned char code = myCodes.front();
  myCodes.pop_front();
  Vector2i delta;
  switch ( code )
    {
    case 0: // = forward
      delta = FreemanChain::displacement( myStartStep );
      myStartPt += delta;
      break;
    case 1: // + convex
      myStartStep = FreemanChain::turnedCode( myStartStep, ! myCW );
      break;
    case 3: // - concave
      myStartStep = FreemanChain::turnedCode( myStartStep, myCW );
      break;
    }
  myCodes.push_back( code );
}

unsigned int
ImaGene::CurveCode::shiftForInnerOrOuterCurve()
{
  unsigned int nb_shifts = 0;
  if ( ! myIsClosed ) return nb_shifts;
  if ( myCodes.size() <= 4 ) return nb_shifts;
  while ( true )
    {
      while ( ( myCodes.front() != 0 ) || ( myCodes.back() == 3 ) )
	{
	  shift();
	  ++nb_shifts;
	}
      unsigned char next_code = *( ++myCodes.begin() );
      switch ( next_code )
	{
	case 0: return nb_shifts;
	case 1:
	case 3: // prev_code != 3
	  unsigned char prev_code = myCodes.back();
	  if ( ( prev_code != 0 )
	       && ( prev_code != next_code ) )
	    return nb_shifts;
	}
      shift();
      ++nb_shifts;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::CurveCode::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[CurveCode" 
	      << " x=" << myStartPt.x()
	      << " y=" << myStartPt.y() 
	      << ( myIsClosed ? " Closed" : " Open" )
	      << ( myCW ? " CW" : " CCW" )
	      << " step=" << (unsigned char)( myStartStep + '0');
  if ( myCodes.size() > 0 )
    {
      that_stream << " ";
      for ( unsigned int i = 0; i < myCodes.size(); ++i )
	{
	  switch ( myCodes[ i ] )
	    {
	    case 0: that_stream << '='; break;
	    case 1: that_stream << '+'; break;
	    case 2: that_stream << 'e'; break;
	    case 3: that_stream << '-'; break;	    
	    }
	}
    }
  that_stream << "]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::CurveCode::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
