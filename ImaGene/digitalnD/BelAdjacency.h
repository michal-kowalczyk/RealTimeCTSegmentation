/** @file BelAdjacency.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : BelAdjacency.h
//
// Creation : 2003/02/06
//
// Version : 2003/02/06
//
// Author : JOL
//
// Summary : Header file for files BelAdjacency.ih and BelAdjacency.cxx
//
// History :
//	2003/02/06 : ?Name? : ?What?
//
// Rcs Id : "@(#)class BelAdjacency declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(BelAdjacency_RECURSES)
#error Recursive header files inclusion detected in BelAdjacency.h
#else // defined(BelAdjacency_RECURSES)
#define BelAdjacency_RECURSES

#if !defined BelAdjacency_h
#define BelAdjacency_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include "ImaGene/digitalnD/KnTypes.h"
#include "ImaGene/base/Vector2i.h"
#include "ImaGene/digitalnD/KnSpace.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

  class KnCharSet;

///////////////////////////////////////////////////////////////////////////////
// class BelAdjacency
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'BelAdjacency' <p>
 * Aim: To represent adjacencies between bel elements. It allows tracking
 * of boundaries and of surfaces.
 */
class BelAdjacency
{
  // ------------------------- public types ------------------------------
public:
  
  class bel_iterator 
  {
    // ------------------------- Standard services ----------------------------
  public:

    /**
     * Destructor. Does nothing.
     */
    INLINE ~bel_iterator();
    
    /**
     * Constructor.
     * @param ks the digital space in which the bel adjacency is defined.
     * @param c the starting bel.
     * @param k the tangent direction to [c] that will be tracked.
     * @param interior when 'true', the bel iterator tracks interior adjacent bels, otherwise exterior ones.
     */
    bel_iterator( const KnSpace & ks, Kn_sid c, uint k, bool interior );

    /**
     * Assignment.
     * @param other the bel iterator to copy.
     * @return a reference on 'this'.
     */
    INLINE bel_iterator& operator=( const bel_iterator & other );
    
    /**
     * Initializer.
     * @param c the new starting bel.
     * @param k the new tangent direction to [c] that will be tracked.
     * @param interior when 'true', the bel iterator tracks interior adjacent bels, otherwise exterior ones.
     */
    void init( Kn_sid c, uint k, bool interior );

    /**
     * @return the current bel.
     */
    INLINE Kn_sid current() const;

//     /**
//      * @return the tangent direction.
//      */
//     INLINE uint tangentDir() const;

    /**
     * @return the current track direction.
     */
    INLINE uint trackDir() const;

    /**
     * @return the current orthogonal direction.
     */
    INLINE uint orthDir() const;

    /**
     * @return 'true' if the track direction follows the positive direction of its axis.
     */
    INLINE bool isTrackDirPositive() const;

    /**
     * @return 'true' if the orthogonal direction follows the positive direction of its axis.
     */
    INLINE bool isOrthDirPositive() const;

    /**
     * @param other any other bel iterator.
     * @return 'true' if the bel iterators are pointing on the same bel and direction, 'false' otherwise.
     */
    INLINE bool operator==( const BelAdjacency::bel_iterator & other ) const;
    
    
    // ------------------------- Follower services ----------------------------
  public:
    
    /**
     * @param i a number between 1 and 3.
     * @return the [i]-th direct follower of 'current()'.
     */ 
    INLINE Kn_sid directFollower( int i ) const;

    /**
     * @return the first direct follower of 'current()'.
     */ 
    INLINE Kn_sid directFollower1() const;
    /**
     * @return the second direct follower of 'current()'.
     */ 
    INLINE Kn_sid directFollower2() const;
    /**
     * @return the third direct follower of 'current()'.
     */ 
    INLINE Kn_sid directFollower3() const;

    /**
     * @param i a number between 1 and 3.
     * @param pos when 'true', indicates to look for the follower along the positive direction of the tracking axis, otherwise along the negative direction.
     * @return the [i]-th follower of 'current()'.
     */ 
    INLINE Kn_sid follower( int i, bool pos ) const;

    /**
     * @param pos when 'true', indicates to look for the follower along the positive direction of the tracking axis, otherwise along the negative direction.
     * @return the first follower of 'current()'.
     */ 
    INLINE Kn_sid follower1( bool pos ) const;

    /**
     * @param pos when 'true', indicates to look for the follower along the positive direction of the tracking axis, otherwise along the negative direction.
     * @return the second follower of 'current()'.
     */ 
    INLINE Kn_sid follower2( bool pos ) const;

    /**
     * @param pos when 'true', indicates to look for the follower along the positive direction of the tracking axis, otherwise along the negative direction.
     * @return the third follower of 'current()'.
     */ 
    INLINE Kn_sid follower3( bool pos ) const;

    
    // ------------------------- Track services ----------------------------
  public:
    
    /**
     * Go to the next direct or indirect adjacent bel on the boundary of [obj].
     * The boundary must be closed (it cannot touch the space borders).
     * Following interior or exterior bel adjacencies depends on [m_interior].
     * @see nextOnAnyBdry,m_interior
     * @param obj a set of unoriented spels. This set must have an empty intersection with the image border.
     * @param indirect when 'false', looks for direct followers, otherwise looks for indirect followers.
     * @return 1 if the move was the first interior, 2 if it was the second interior, 3 if it was the third interior.
     */
    INLINE uint nextOnBdry( const KnCharSet & obj, bool indirect = false );
    
    /**
     * Go to the next direct or indirect adjacent bel on the boundary of [obj].
     * The boundary may be open (it touches the space borders).
     * Following interior or exterior bel adjacencies depends on [m_interior].
     * @see nextOnBdry,m_interior
     * @param obj a set of unoriented spels. This set can have a non-empty intersection with the image border.
     * @param indirect when 'false', looks for direct followers, otherwise looks for indirect followers.
     * @return 0 if the move was impossible (no bels in this direction), 1 if it was the first interior, 2 if it was the second interior, 3 if it was the third interior.
     */
    INLINE uint nextOnAnyBdry( const KnCharSet & obj, bool indirect = false );

    /**
     * Go to the next direct or indirect <b>exterior</b> adjacent bel on the
     * boundary of [obj].
     * @param obj a set of unoriented spels.
     * @param indirect when 'false', looks for direct followers, otherwise looks for indirect followers.
     * @return 0 if the move was impossible (no bels in this direction), 1 if it was the first interior, 2 if it was the second interior, 3 if it was the third interior.
     */
    uint nextExtOnBdry( const KnCharSet & obj, bool indirect );

    /**
     * Go to the next direct or indirect <b>interior</b> adjacent bel on the 
     * boundary of [obj].
     * @param obj a set of unoriented spels.
     * @param indirect when 'false', looks for direct followers, otherwise looks for indirect followers.
     * @return 0 if the move was impossible (no bels in this direction), 1 if it was the first interior, 2 if it was the second interior, 3 if it was the third interior.
     */
    uint nextIntOnBdry( const KnCharSet & obj, bool indirect );


    // ------------------------- Geometry services ----------------------------
  public:

//     /**
//      * Computes the deltas between the 1-low incident cells of [m_bel] along
//      * tangent and orthogonal coordinates (defined at init).
//      * @param delta (returns) the delta along tangent coordinate (+1/-1) for x and along orthogonal coordinate (+1/-1) for y.
//      */
//     INLINE void getDelta( Vector2i & delta ) const;

//     /**
//      * Computes the deltas between the 1-low incident cells of [m_bel] along
//      * tangent and orthogonal coordinates (defined at init).
//      * @param dx (returns) the delta along tangent coordinate (+1/-1).
//      * @param dy (returns) the delta along orthogonal coordinate (+1/-1).
//      */
//     INLINE void getDelta( int & dx, int & dy ) const;
    

    // ------------------------- Datas ----------------------------------------
  private:
    /**
     * Digital space where bel iterator is evolving.
     */
    const KnSpace & m_ks;
    /**
     * Current bel.
     */
    Kn_sid m_bel;
//     /**
//      * Tangent direction at beginning (useful for some geometry computation).
//      */
//     uint m_tangent_dir;
    /**
     * Current orthogonal direction (to [m_bel]).
     */
    uint m_orth_dir;
    /**
     * Current track direction (to [m_bel]).
     */
    uint m_track_dir;
    /**
     * Current interior spel (to [m_bel]).
     */
    Kn_uid m_vox_int;
    /**
     * Current exterior spel (to [m_bel]).
     */
    Kn_uid m_vox_ext;
    /** 
     * 'true' if bel adjacency is interior, 'false' otherwise.
     */
    bool m_interior;
    /**
     * 'true' if going positive along current orthogonal direction.
     */
    bool m_orth_direct;
    /**
     * 'true' if going positive along current tracking direction.
     */
    bool m_track_direct;
    
  };
  
  


  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. 
   */
  ~BelAdjacency();

  /**
   * Constructor.
   * @param ks the digital space in which the bel adjacency is defined.
   * @param interior when 'true', the bel adjacency is interior for any coordinate pair, when 'false', it is exterior for any one.
   */
  BelAdjacency( const KnSpace & ks, bool interior = true );

  /**
   * @return a const reference to the space where the BelAdjacency is defined.
   */
  INLINE const KnSpace & space() const;

  /**
   * Set a coordinate pair as interior or exterior in the bel adjacency.
   * @param i first coordinate.
   * @param j second coordinate ('j != i').
   * @param interior when 'true', the bel adjacency is interior for the coordinate pair '(i,j)', when 'false', it is exterior.
   */
  void setAdjacency( uint i, uint j, bool interior );

  /**
   * Returns whether a coordinate pair is interior (true) or exterior (false) 
   * in the bel adjacency.
   * @param i first coordinate.
   * @param j second coordinate ('j != i').
   * @return 'true' if the bel adjacency is interior for the coordinate pair '(i,j)', 'false' when it is exterior.
   */
  bool getAdjacency( uint i, uint j ) const;
    
  /**
   * Constructs a bel iterator starting at surfel [c] with tangent 
   * direction [k].
   * @param c any oriented surfel.
   * @param k any coordinate ('k != orthDir( c )')
   * @return a bel iterator.
   */
  bel_iterator sstart( Kn_sid c, uint k ) const;
  

  // ------------------------- Interface --------------------------------------
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
  
  // ------------------------- Datas ------------------------------------------
private:

  /**
   * Digital space where bel adjacency are defined.
   */
  const KnSpace & m_ks;

  /**
   * Memorizes if bel adjacency is interior or exterior for any pair (i,j).
   * The index of the pair (i,j) is 'i * m_ks.dim() + j'.
   */
  std::vector<bool> m_interior;
  

  // ------------------------- Hidden services --------------------------------
protected:
  /**
   * Constructor.
   * Forbidden (protected to avoid g++ warnings).
   */
  INLINE BelAdjacency();
private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden.
   */
  INLINE BelAdjacency( const BelAdjacency & other );
  /**
   * Assignment.
   * @param other the object to clone.
   * @return a reference on 'this'.
   * Forbidden.
   */
  INLINE BelAdjacency & operator=( const BelAdjacency & other );
  
  // ------------------------- Internals --------------------------------------
private:
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'BelAdjacency'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'BelAdjacency' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const BelAdjacency & that_object_to_display );


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/BelAdjacency.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined BelAdjacency_h

#undef BelAdjacency_RECURSES
#endif // else defined(BelAdjacency_RECURSES)
