/** @file SurfelNeighborhood.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : SurfelNeighborhood.h
//
// Creation : 2005/01/10
//
// Version : 2005/01/10
//
// Author : JOL
//
// Summary : Header file for files SurfelNeighborhood.ih and SurfelNeighborhood.cxx
//
// History :
//	2005/01/10 : ?Name? : ?What?
//
// Rcs Id : "@(#)class SurfelNeighborhood declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(SurfelNeighborhood_RECURSES)
#error Recursive header files inclusion detected in SurfelNeighborhood.h
#else // defined(SurfelNeighborhood_RECURSES)
#define SurfelNeighborhood_RECURSES

#if !defined SurfelNeighborhood_h
#define SurfelNeighborhood_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/BelAdjacency.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class SurfelNeighborhood
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'SurfelNeighborhood' <p> Aim: Represents a
   * surfel and its neighborhood on a surface or on a boundary * The
   * 'SurfelNeighborhood' needs a context precising the space and the
   * bel adjacency used for determining adjacent surfels.
   */
  class SurfelNeighborhood
  {
    // ----------------------- Context services ------------------------------
  public:
    
    /**
     * Sets the space and the bel adjacency for the 'SurfelNeighborhood'.
     *
     * @param ks the digital space.  
     *
     * @param bel_adjacency the bel adjacency (specified for each pair
     * of directions which adjacent surfel is preferred: the inner one
     * or the outer one).
     */
    INLINE void init( const KnSpace* ks, 
		      const BelAdjacency* bel_adjacency );

    /**
     * @return the digital space in which the SurfelNeighborhood is taken.
     */
    INLINE const KnSpace* space() const;

    /**
     * @return the bel adjacency chosen for the SurfelNeighborhood.
     */
    INLINE const BelAdjacency* belAdjacency() const;
    
    

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~SurfelNeighborhood();

    /**
     * Constructor. The object is not valid.
     * @see init
     */
    SurfelNeighborhood();

    /**
     * Moves the object to the signed surfel [s]. Same as 'init'.
     *
     * @param s any signed surfel.
     * @see init
     */
    INLINE void move( Kn_sid s );
    
    /**
     * @return the current surfel.
     */
    INLINE Kn_sid current() const;

    /**
     * @return the orthogonal direction to the current surfel.
     */
    INLINE uint orthDir() const;

    // ----------------------- Orientation ------------------------------
  public:

    /**
     * Returns the direct orientation of surfel 'current()' along
     * direction [dir], meaning the incident cell along direction
     * [dir] and the returned orientation has a positive sign.
     *
     * @param dir any direction. 
     * 
     * @return the direct orientation along the direction [dir] of
     * surfel 'current()'.
     */
    INLINE bool directOrientation( uint dir ) const;
    


    // ----------------------- Followers and link services --------------------
  public:

    
    /**
     * @param track_dir the direction where to look for the follower.
     * @param i a number between 1 and 3.
     * @return the [i]-th direct follower of 'current()'.
     */ 
    INLINE Kn_sid directFollower( uint track_dir, int i ) const;

    /**
     * @param track_dir the direction where to look for the follower.
     * @return the first direct follower of 'current()'.
     */ 
    INLINE Kn_sid directFollower1( uint track_dir ) const;
    /**
     * @param track_dir the direction where to look for the follower.
     * @return the second direct follower of 'current()'.
     */ 
    INLINE Kn_sid directFollower2( uint track_dir ) const;
    /**
     * @param track_dir the direction where to look for the follower.
     * @return the third direct follower of 'current()'.
     */ 
    INLINE Kn_sid directFollower3( uint track_dir ) const;

    /**
     * @param track_dir the direction where to look for the follower.
     * @param i a number between 1 and 3.
     * @param pos when 'true', indicates to look for the follower along the positive direction of the tracking axis, otherwise along the negative direction.
     * @return the [i]-th follower of 'current()'.
     */ 
    INLINE Kn_sid follower( uint track_dir, int i, bool pos ) const;

    /**
     * @param track_dir the direction where to look for the follower.
     * @param pos when 'true', indicates to look for the follower along the positive direction of the tracking axis, otherwise along the negative direction.
     * @return the first follower of 'current()'.
     */ 
    INLINE Kn_sid follower1( uint track_dir, bool pos ) const;

    /**
     * @param track_dir the direction where to look for the follower.
     * @param pos when 'true', indicates to look for the follower along the positive direction of the tracking axis, otherwise along the negative direction.
     * @return the second follower of 'current()'.
     */ 
    INLINE Kn_sid follower2( uint track_dir, bool pos ) const;

    /**
     * @param track_dir the direction where to look for the follower.
     * @param pos when 'true', indicates to look for the follower along the positive direction of the tracking axis, otherwise along the negative direction.
     * @return the third follower of 'current()'.
     */ 
    INLINE Kn_sid follower3( uint track_dir, bool pos ) const;


    /**
     * @param track_dir the direction where to look for the link.
     * @return the direct link of 'current()' in direction [track_dir].
     */ 
    INLINE Kn_sid directLink( uint track_dir ) const;

    
    /**
     * @param track_dir the direction where to look for the link.
     * @param pos when 'true', indicates to look for the link along the positive direction of the tracking axis, otherwise along the negative direction.
     * @return the link of 'current()' in direction [track_dir].
     */ 
    INLINE Kn_sid link( uint track_dir, bool pos ) const;



    // ----------------------- Spels services --------------------
  public:

    /**
     * @return the positively oriented spel touching the surfel 'current()'.
     */
    INLINE Kn_sid innerSpel() const;

    /**
     * @return the negatively oriented spel touching the surfel 'current()'.
     */
    INLINE Kn_sid outerSpel() const;
    
    /**
     * @return the unsigned positively oriented spel touching the
     * surfel 'current()'.
     */
    INLINE Kn_uid uinnerSpel() const;

    /**
     * @return the unsigned negatively oriented spel touching the
     * surfel 'current()'.
     */
    INLINE Kn_uid uouterSpel() const;

    /**
     * @param track_dir the direction where to look for the spel.
     * @param pos when 'true', looks along the positive direction of the tracking axis, otherwise along the negative direction.
     * @return the (positively oriented) spel adjacent to 'innerSpel()' in the direction [track_dir] and orientation [pos].
     */
    INLINE Kn_sid innerAdjSpel( uint track_dir, bool pos ) const;

    /**
     * @param track_dir the direction where to look for the spel.
     * @param pos when 'true', looks along the positive direction of the tracking axis, otherwise along the negative direction.
     * @return the (negatively oriented) spel adjacent to 'outerSpel()' in the direction [track_dir] and orientation [pos].
     */
    INLINE Kn_sid outerAdjSpel( uint track_dir, bool pos ) const;

    /**
     * @param track_dir the direction where to look for the spel.
     * @param pos when 'true', looks along the positive direction of the tracking axis, otherwise along the negative direction.
     * @return the unsigned spel adjacent to 'innerSpel()' in the direction [track_dir] and orientation [pos].
     */
    INLINE Kn_uid uinnerAdjSpel( uint track_dir, bool pos ) const;

    /**
     * @param track_dir the direction where to look for the spel.
     * @param pos when 'true', looks along the positive direction of the tracking axis, otherwise along the negative direction.
     * @return the unsigned spel adjacent to 'outerSpel()' in the direction [track_dir] and orientation [pos].
     */
    INLINE Kn_uid uouterAdjSpel( uint track_dir, bool pos ) const;


    // ----------------------- Adjacency services --------------------
  public:

    /**
     * Go to the next direct or indirect adjacent bel on the boundary of [obj].
     * The boundary may be open (it touches the space borders).
     * Following interior or exterior bel adjacencies depends on [m_bel_adj].
     *
     * @param obj any object which has the 'bool const operator[ Kn_uid ]'.
     * @param track_dir the direction where to look for the spel.
     * @param pos when 'true' look in positive direction along [track_dir] axis, 'false' look in negative direction.
     * @param adj_surfel (returns) the adjcent surfel in direction [track_dir] if there is one.
     *
     * @return 0 if the move was impossible (no bels in this direction), 1 if it was the first interior, 2 if it was the second interior, 3 if it was the third interior.
     */
    template <typename Set>
    INLINE uint adjacentOnAnyBdry( const Set & obj, 
				   uint track_dir,
				   bool pos,
				   Kn_sid & adj_surfel ) const;

    /**
     * Go to the next direct or indirect adjacent bel on the oriented
     * surface [surface].  The boundary may be open.  Following
     * interior or exterior bel adjacencies depends on [m_bel_adj].
     *
     * @param surface any set of signed surfels which has the 'bool const operator[ Kn_sid ]'.
     * @param track_dir the direction where to look for the spel.
     * @param pos when 'true' look in positive direction along [track_dir] axis, 'false' look in negative direction.
     * @param adj_surfel (returns) the adjcent surfel in direction [track_dir] if there is one.
     *
     * @return 0 if the move was impossible (no surfels in this direction), 1 if it was the first interior, 2 if it was the second interior, 3 if it was the third interior.
     */
    template <typename Surface>
    INLINE uint adjacentOnOrientedSurface( const Surface & surface, 
					   uint track_dir,
					   bool pos, 
					   Kn_sid & adj_surfel ) const;


    /**
     * Go to the next direct or indirect adjacent bel on the oriented
     * surface [surface].  The boundary may be open.  Following
     * interior or exterior bel adjacencies depends on [m_bel_adj].
     *
     * @param surface any set of signed surfels which has the 'bool const operator[ Kn_sid ]'.
     * @param track_dir the direction where to look for the spel.
     * @param indirect when 'false', looks for direct followers, otherwise looks for indirect followers.
     * @param adj_surfel (returns) the adjcent surfel in direction [track_dir] if there is one.
     *
     * @return 0 if the move was impossible (no surfels in this direction), 1 if it was the first interior, 2 if it was the second interior, 3 if it was the third interior.
     */
    template <typename Surface>
    INLINE uint adjacentOnOrientedSurfaceDirected( const Surface & surface, 
						   uint track_dir,
						   bool indirect, 
						   Kn_sid & adj_surfel ) const;


    /**
     * Go to the next direct or indirect adjacent bel on the unoriented
     * surface [surface].  The boundary may be open.  Following
     * interior or exterior bel adjacencies depends on [m_bel_adj].
     *
     * @param surface any set of unsigned surfels which has the 'bool const operator[ Kn_uid ]'.
     * @param track_dir the direction where to look for the spel.
     * @param indirect when 'false', looks for direct followers, otherwise looks for indirect followers.
     * @param adj_surfel (returns) the adjcent surfel in direction [track_dir] if there is one.
     *
     * @return 0 if the move was impossible (no surfels in this direction), 1 if it was the first interior, 2 if it was the second interior, 3 if it was the third interior.
     */
    template <typename Surface>
    INLINE uint adjacentOnUnorientedSurface( const Surface & surface, 
					     uint track_dir,
					     bool indirect, 
					     Kn_sid & adj_surfel ) const;

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
     * A pointer to the digital space.
     */
    const KnSpace* m_ks;
    
    /**
     * A pointer to the bel adjacency.
     */
    const BelAdjacency* m_bel_adj;

    /**
     * The current surfel.
     */
    Kn_sid m_surfel;

    /**
     * The orthogonal direction to [m_surfel].
     * @see m_surfel
     */
    uint m_orth_dir;

    /**
     * The direct orientation in the orthogonal direction wrt [m_surfel].
     * @see m_surfel
     */
    bool m_orth_direct_orientation;
    
    

    // ------------------------- Hidden services ------------------------------
  protected:


  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE SurfelNeighborhood( const SurfelNeighborhood & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE SurfelNeighborhood & operator=( const SurfelNeighborhood & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'SurfelNeighborhood'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'SurfelNeighborhood' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const SurfelNeighborhood & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/SurfelNeighborhood.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined SurfelNeighborhood_h

#undef SurfelNeighborhood_RECURSES
#endif // else defined(SurfelNeighborhood_RECURSES)
