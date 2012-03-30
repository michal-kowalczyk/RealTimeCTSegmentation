/** @file DigitalSurfaceUmbrella.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : DigitalSurfaceUmbrella.h
//
// Creation : 2011/04/29
//
// Version : 2011/04/29
//
// Author : JOL
//
// Summary : Header file for files DigitalSurfaceUmbrella.ih and DigitalSurfaceUmbrella.cxx
//
// History :
//	2011/04/29 : ?Name? : ?What?
//
// Rcs Id : "@(#)class DigitalSurfaceUmbrella declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(DigitalSurfaceUmbrella_RECURSES)
#error Recursive header files inclusion detected in DigitalSurfaceUmbrella.h
#else // defined(DigitalSurfaceUmbrella_RECURSES)
#define DigitalSurfaceUmbrella_RECURSES

#if !defined DigitalSurfaceUmbrella_h
#define DigitalSurfaceUmbrella_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/DigitalSurfaceUmbrellaComputer.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class DigitalSurfaceUmbrella
  /////////////////////////////////////////////////////////////////////////////
  /** 
      Description of class 'DigitalSurfaceUmbrella' <p> Aim: This
      class represents an umbrella over a digital surface, i.e. a set
      of n-1-cells looping around a positive n-3-cell called pivot. In
      3D, an umbrella may be seen as a vertex of a digital surface, if
      one considers the surfels to be the 2-faces of surface and the
      adjacencies between surfels as the 1-faces. In 3D also, an
      umbrella may be seen as a coface (2-face of the dual surface),
      while surfels are the vertices of the dual surface.

      An umbrella is indirectly related to some digital surface.

      An umbrella is circulable. Each element is a state composed of a
      surfel, a linel (the separator), a pointel (the pivot).
  
      @see DigitalSurfaceUmbrellaComputer
   */
  class DigitalSurfaceUmbrella
  {
    // ----------------------- Public types ------------------------------
  public:

    typedef DigitalSurfaceUmbrellaComputer CirculatorValue;

    struct Circulator {
      typedef CirculatorValue Value;
      Circulator();
      ~Circulator();
      Circulator( const Circulator & other);
      Circulator& operator=( const Circulator & other);
      /// Post-increment operation. The circulator is moved forward (next).
      Circulator& operator++();
      /// Post-decrement operation. The circulator is moved backward (previous).
      Circulator& operator--();
      /// Read-only access. @return the current value.
      const Value* operator->() const;
      /// Read-only access. @return the current value.
      const Value& operator*() const;
      bool operator==( const Circulator & other ) const;
      bool operator<( const Circulator & other ) const;
      bool operator!=( const Circulator & other ) const;
      
      DigitalSurfaceUmbrellaComputer m_computer;
    };

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~DigitalSurfaceUmbrella();

    /**
     * Constructor. The umbrella is not valid.
     */
    DigitalSurfaceUmbrella();

    /**
     * Copy constructor.
     * @param other the object to clone.
     */
    DigitalSurfaceUmbrella( const DigitalSurfaceUmbrella & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     */
    DigitalSurfaceUmbrella & operator=( const DigitalSurfaceUmbrella & other );

    /**
       Initializes the umbrella with a 'DigitalSurfaceTracker'.  Tells
       where is the positive pivot to turn around. @see
       DigitalSurfaceUmbrellaComputer.

       @param tracker a tracker on a digital surface with a valid
       'current()' surfel. (cloned).
       
       @param k the separator direction different from [j] and
       
       @param epsilon the orientation where to find the separator
       pointing at the pivot.
       
       @param j the track direction where the next surfel of the umbrella is.

       @return 'true' if this is a valid umbrella, or 'false' if the
       umbrella is open (and thus not valid).
     */
    bool init( const DigitalSurfaceTracker & tracker,
	       uint k, bool epsilon, uint j );
    
    /**
       Get the starting point of the circulator to circulate all
       elements of the umbrella. To get all element, you should write
       something like:
       @code
       Circulator start = umbrella.start();
       Circulator p = start;
       do {
         CirculatorValue v = *p; 
	 ... 
       } while ( ++p != start );
       @endcode
    */
    const Circulator & start() const;

    /**
       Less-than operator. Useful to store umbrellas in a set.
       @param other the object to compare with.
       @return 'true' if this umbrella is before the other one.
     */
    bool operator<( const DigitalSurfaceUmbrella & other ) const;

    /**
       Equality operator. 
       @param other the object to compare with.
       @return 'true' if this umbrella is the same as the other one.
     */
    bool operator==( const DigitalSurfaceUmbrella & other ) const;

    /**
       Difference operator. 
       @param other the object to compare with.
       @return 'true' if this umbrella is the same as the other one.
     */
    bool operator!=( const DigitalSurfaceUmbrella & other ) const;
    
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

    /// Start circulator if valid.
    Circulator m_start;

    // ------------------------- Hidden services ------------------------------
  protected:


  private:

  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'DigitalSurfaceUmbrella'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'DigitalSurfaceUmbrella' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const DigitalSurfaceUmbrella & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/DigitalSurfaceUmbrella.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined DigitalSurfaceUmbrella_h

#undef DigitalSurfaceUmbrella_RECURSES
#endif // else defined(DigitalSurfaceUmbrella_RECURSES)
