/** @file DigitalSurfaceUmbrellaComputer.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : DigitalSurfaceUmbrellaComputer.h
//
// Creation : 2005/01/11
//
// Version : 2005/01/11
//
// Author : JOL
//
// Summary : Header file for files DigitalSurfaceUmbrellaComputer.ih and DigitalSurfaceUmbrellaComputer.cxx
//
// History :
//	2005/01/11 : ?Name? : ?What?
//
// Rcs Id : "@(#)class DigitalSurfaceUmbrellaComputer declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(DigitalSurfaceUmbrellaComputer_RECURSES)
#error Recursive header files inclusion detected in DigitalSurfaceUmbrellaComputer.h
#else // defined(DigitalSurfaceUmbrellaComputer_RECURSES)
#define DigitalSurfaceUmbrellaComputer_RECURSES

#if !defined DigitalSurfaceUmbrellaComputer_h
#define DigitalSurfaceUmbrellaComputer_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/DigitalSurfaceTracker.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class DigitalSurfaceUmbrellaComputer
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'DigitalSurfaceUmbrellaComputer' <p> Aim: Useful for
   * computing umbrellas on 'DigitalSurface's, ie set of n-1 cells
   * around a n-3 cell.
   *
   * The current surfel (n-1 cell), or \a face, is denoted by \a s.
   * The n-3 cell is called the \a pivot and denoted by \a +p. It is
   * always a positive cell. There is also a current n-2 cell, called
   * the \a separator and denoted by \a t, which is in the boundary of
   * \a s and whose boundary contains \a +p.
   *
   * Two directions specifies the separator and the pivot, the \a
   * track direction \a j and the separator direction \a k. Moreover
   * an orientation \f$ \epsilon=\pm 1 \f$ specifies one which side is
   * the separator wrt the face. We have \f$ t = \Delta^{\epsilon}_k
   * s, +p = \Delta^{\mu}_{j} t, \f$ where \f$ \mu \f$ is the direct
   * orientation of \a t along \a j.
   *
   * Turning around the pivot means moving the face and the separator
   * once (in the track direction), such that the pivot is the same
   * (ie \a +p), the track and separator directions being
   * updated. Repeating this process a sufficient number of times
   * brings the umbrella back in its original position, except in the
   * case when the DigitalSurface has a boundary touching the pivot.
   *
   * Uses delegation with DigitalSurfaceTracker.
   */
  class DigitalSurfaceUmbrellaComputer
  {
    // ----------------------- Local structures ------------------------------
  public:

    /**
     * Stores (the main part of) the state of an umbrella.
     */
    struct State 
    {
      /**
       * Current face.
       */
      Kn_sid face;

      /**
       * Direction to find separator.
       */
      uint k;

      /**
       * Orientation to find separator.
       */
      uint epsilon;

      /**
       * Track direction.
       */
      uint j;

      /**
       * @param ks the digital space.
       * @return the separator n-2 cell.
       */
      INLINE Kn_sid separator( const KnSpace & ks ) const;

      /**
       * @param ks the digital space.
       * @return the pivot n-3 cell (always positively orientated).
       */
      INLINE Kn_sid pivot( const KnSpace & ks ) const;

      /**
       * @param ks the digital space.
       * @return the track orientation (i.e. \a mu).
       */
      INLINE bool mu( const KnSpace & ks ) const;
    };
    
    

    // ----------------------- Static services ------------------------------
  public:

    /**
     * Computes the complete umbrella from an initialized
     * 'DigitalSurfaceUmbrellaComputer'.
     *
     * @param umbrella an initialized umbrella (with a surface and
     * valid pivot).
     *
     * @param faces (modified) a pointer on a big enough array for
     * storing all visited faces.
     * @param nb (returns) the number of visited faces (in 3D, max is 6).
     *
     * @return 'true' if the umbrella is a loop, 'false' otherwise.
     */
    static bool computeUmbrellaFaces( DigitalSurfaceUmbrellaComputer & umbrella,
				      Kn_sid* faces,
				      uint & nb );
    
    /**
     * Computes the complete umbrella from an initialized
     * 'DigitalSurfaceUmbrellaComputer'.
     *
     * @param umbrella an initialized umbrella (with a surface and
     * valid pivot).
     *
     * @param states (modified) a pointer on a big enough array for
     * storing all umbrella states.
     * @param nb (returns) the number of visited faces (in 3D, max is 6).
     *
     * @return 'true' if the umbrella is a loop, 'false' otherwise.
     */
    static bool computeUmbrella( DigitalSurfaceUmbrellaComputer & umbrella,
				 State* states,
				 uint & nb );

    /**
     * Computes the complete reversed umbrella from an initialized
     * 'DigitalSurfaceUmbrellaComputer'.
     *
     * @param umbrella an initialized umbrella (with a surface and
     * valid pivot).
     *
     * @param states (modified) a pointer on a big enough array for
     * storing all umbrella states.
     * @param nb (returns) the number of visited faces (in 3D, max is 6).
     *
     * @return 'true' if the umbrella is a loop, 'false' otherwise.
     */
    static bool computeReversedUmbrella( DigitalSurfaceUmbrellaComputer & umbrella,
					 State* states,
					 uint & nb );

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~DigitalSurfaceUmbrellaComputer();

    /**
     * Constructor. The object is not valid.
     */
    DigitalSurfaceUmbrellaComputer();

    /**
     * Copy constructor.
     * @param other the object to clone.
     */
    DigitalSurfaceUmbrellaComputer( const DigitalSurfaceUmbrellaComputer & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     */
    DigitalSurfaceUmbrellaComputer & 
    operator=( const DigitalSurfaceUmbrellaComputer & other );
   
    /**
     * Initializes the umbrella with a 'DigitalSurfaceTracker'.
     *
     * @param tracker a tracker on a digital surface with a valid
     * 'current()' surfel. (cloned).
     */
    void init( const DigitalSurfaceTracker & tracker );
    
    /**
     * Tells where is the positive pivot to turn around. @see
     * DigitalSurfaceUmbrellaComputer.
     *
     * @param k the separator direction different from [j] and
     *
     * @param epsilon the orientation where to find the separator
     * pointing at the pivot.
     *
     * @param j the track direction where the next surfel of the umbrella is.
     */
    void setPivot( uint k, bool epsilon, uint j );

    /**
     * Puts the umbrella in the state [state]. The umbrella must have
     * been initialized with a 'DigitalSurfaceTracker' before and the
     * state must be consistent with the tracker. In particular, the
     * face of the state must lie on the same digital surface.
     *
     * @param state a state describing the face, the separator and the pivot.
     */
    void setState( const State & state );

    /**
     * Returns the state of the umbrella in the referenced object [state]. 
     *
     * @param state (returns) the current umbrella state describing
     * the face, the separator and the pivot.
     */
    void getState( State & state ) const;


    /**
     * @return a pointer on the digital space in which the factory is embedded.
     */
    const KnSpace* space() const;
    

    // ----------------------- Accessor services ------------------------------
  public:

    /**
     * @return the current surfel at which the umbrella is pointing.
     */
    INLINE Kn_sid face() const;

    /**
     * @return the separator n-2 cell.
     */
    INLINE Kn_sid separator() const;

    /**
     * @return the pivot n-3 cell (always positively orientated).
     */
    INLINE Kn_sid pivot() const;
    
    /**
     * @return the orthogonal direction to the current face.
     */
    INLINE uint orthDir() const;

    /**
     * @return the track direction.
     */
    INLINE uint trackDir() const;

    /**
     * @return the separator direction.
     */
    INLINE uint separatorDir() const;

    /**
     * @return the separator orientation.
     */
    INLINE bool separatorOrientation() const;

    

    // ----------------------- Pivoting services ------------------------------
  public:

    /**
     * Turns around the current pivot (positive turn).
     *
     * @return 0 if the move was impossible (nothing is updated)
     * otherwise returns the move code of the face.
     */
    uint next();
    
    /**
     * Turns around the current pivot (negative turn).
     *
     * @return 0 if the move was impossible (nothing is updated)
     * otherwise returns the move code of the face.
     */
    uint previous();

    /**
     * Go the adjacent umbrella, ie the one which shares the same
     * faces around the separator but whose pivot is symmetric around
     * the separator.
     *
     * @return 'true' if their was an adjacent umbrella, 'false' otherwise.
     * (the umbrella is in the same state as before the call).
     */
    bool adjacent();
    

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

    /**
     * Tracker used to move on the digital surface.
     */
    DigitalSurfaceTracker* m_tracker;

    /**
     * Digital space.
     */
    const KnSpace* m_space;

    /**
     * The track direction.
     */
    uint m_track_dir;

    /**
     * The separator direction.
     */
    uint m_separator_dir;

    /**
     * The separator orientation.
     */
    bool m_separator_orientation;
    
    

    // ------------------------- Hidden services ------------------------------
  protected:


  private:

  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'DigitalSurfaceUmbrellaComputer'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'DigitalSurfaceUmbrellaComputer' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const DigitalSurfaceUmbrellaComputer & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/DigitalSurfaceUmbrellaComputer.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined DigitalSurfaceUmbrellaComputer_h

#undef DigitalSurfaceUmbrellaComputer_RECURSES
#endif // else defined(DigitalSurfaceUmbrellaComputer_RECURSES)
