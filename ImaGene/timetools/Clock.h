/** @file Clock.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : Clock.h
//
// Creation : 2000/??/??
//
// Version : 2000/??/??
//
// Author : JOL
//
// Summary :
//
// History :
//	2000/??/?? : ?Name? : ?What?
//
// Rcs Id : "@(#)class Clock declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(Clock_RECURSES)
#error Recursive header files inclusion detected in Clock.h
#else // defined(Clock_RECURSES)
#define Clock_RECURSES

#if !defined Clock_h
#define Clock_h

//////////////////////////////////////////////////////////////////////////////
//                                                                          //

#include <iostream>


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class Clock
///////////////////////////////////////////////////////////////////////////////
/**
 * Description of class 'Clock'
 * Aim: To provide functions to start and stop a timer. Is useful to get
 * performance of algorithms.
 */
class Clock
{
  
public :
  
  // ------------------------- Standard services ------------------------------
  // -------------------------- timing services -------------------------------
public:
  /**
   * Starts a clock.
   */
  static void startClock();
  /**
   * @return the time (in ms) since the last 'startClock()'.
   */
  static long stopClock();

  // ------------------------- Interface --------------------------------------
  /**
   * Writes/Displays the object on an output stream.
   * @param that_stream the output stream where the object is written.
   */
  void selfDisplay( std::ostream& that_stream ) const;
  /**
   * Checks the validity/consistency of the object.
   * @return 'true' if the object is valid, 'false' otherwise.
   */
  bool OK() const;
  
private :
  
  // ------------------------- Datas ------------------------------------------
  
  // ------------------------- Hidden services --------------------------------
  
  inline Clock();
  inline Clock(const Clock & other);
  inline Clock & operator=(const Clock & other);
  
  // ------------------------- Internals --------------------------------------
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'Clock'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'Clock' to write.
 * @return the output stream after the writing.
 */
inline std::ostream&
operator<<( std::ostream& that_stream, const Clock& that_object_to_display );

std::ostream&
operator<<( std::ostream& that_stream, const Clock& that_object_to_display )
{
  that_object_to_display.selfDisplay(that_stream);
  return that_stream;
}

} // namespace ImaGene

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined Clock_h

#undef Clock_RECURSES
#endif // else defined(Clock_RECURSES)
