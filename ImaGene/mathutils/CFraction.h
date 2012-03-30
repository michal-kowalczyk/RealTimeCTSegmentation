/** @file CFraction.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : CFraction.h
//
// Creation : 2009/10/22
//
// Version : 2009/10/22
//
// Author : JOL
//
// Summary : Header file for files CFraction.ih and CFraction.cxx
//
// History :
//	2009/10/22 : ?Name? : ?What?
//
// Rcs Id : "@(#)class CFraction declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(CFraction_RECURSES)
#error Recursive header files inclusion detected in CFraction.h
#else // defined(CFraction_RECURSES)
#define CFraction_RECURSES

#if !defined CFraction_h
#define CFraction_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include "ImaGene/base/BasicTypes.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class CFraction
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'CFraction' <p> Aim: represents a fraction
   * as a simple continued fraction. The fraction must be in
   * [0,1]. Stores also its partial quotients.
   */
  class CFraction
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~CFraction();

    /**
     * Constructor. The fraction is not valid.
     */
    INLINE CFraction();

    /**
     * Constructor. Initializes the fraction p/q.
     *
     * @pre p <= q
     * @param p the numerator.
     * @param q the denominator.
     */
    INLINE CFraction( uint p, uint q );

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE CFraction( const CFraction & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE CFraction & operator=( const CFraction & other );

    /**
     * Initializes a fraction p/q.
     *
     * @pre p <= q
     * @param p the numerator.
     * @param q the denominator.
     * @return the gcd of p and q.
     */
    uint init( uint p, uint q );

    // ----------------------- Accessors --------------------------------------
  public:

    /**
     * @return the numerator of the fraction.
     */
    INLINE uint p() const;

    /**
     * @return the denominator of the fraction.
     */
    INLINE uint q() const;

    /**
     * @return the depth or complexity of the simple continued
     * fraction (-1 means empty fraction).
     */
    INLINE int depth() const;

    /**
     * @return 'true' if the depth or complexity of the simple continued
     * fraction is even.
     */
    INLINE bool even() const;

    /**
     * @return 'true' if the depth or complexity of the simple continued
     * fraction is odd.
     */
    INLINE bool odd() const;

    /**
     * @param k the index of the partial coefficient (between 0 and \ref depth())
     * @return the k-th partial coefficient.
     */
    INLINE uint uk( int k ) const;

    /**
     * @param k the index of the partial quotient (between 0 and \ref depth())
     * @return the numerator of the k-th partial quotient.
     */
    INLINE uint pk( int k ) const;

    /**
     * @param k the index of the partial quotient (between 0 and \ref depth())
     * @return the numerator of the k-th partial quotient.
     */
    INLINE uint qk( int k ) const;


    // ----------------------- Mediant services --------------------------------
  public:

    /** 
     * Splits the fraction into two fractions following Berstel's formula:
     * - E(z0) = 0
     * - E(z1) = 0^u1 1
     * - E(z2n)= E(z2n-2) E(z2n-1)^u2n
     * - E(z2n+1)= E(z2n)^u2n+1 E(z2n-1)
     *
     * @param zleft (returns) the left pattern, of the decomposition.
     * @param nleft (returns) the number of times the left pattern is repeated.
     * @param zright (returns) the right pattern, of the decomposition.
     * @param nright (returns) the number of times the right pattern is repeated.
     * @return 'true' if the decomposition was proper (non zero fraction).
     */
    bool splitBerstel( CFraction & zleft, uint & nleft, 
		       CFraction & zright, uint & nright ) const;

    /**
     * Computes the mediant of this fraction and [other]. If one of
     * the two is an ancestor of the other in the Stern-Brocot tree,
     * then the computation is O(1). Otherwise, the continued fraction
     * is recomputed in O(log (p+q)).
     *
     * @param other any other fraction.
     * @return the mediant of 'this' and [other].
     */
    CFraction mediant( const CFraction & other ) const;

    /**
     * @param other any other fraction.
     * @return the determinant of the vector (q,p) with (other.q,other.p).
     */
    int det( const CFraction & other ) const;

    /**
     * @param other any other fraction.
     *
     * @return true if 'this' is the direct descendent of [other] in the
     * Stern-Brocot tree.
     */
    bool isDirectDescendent( const CFraction & other ) const;

    /**
     * @param other any other fraction.
     *
     * @return true if 'this' is the indirect descendent of [other] in the
     * Stern-Brocot tree.
     */
    bool isIndirectDescendent( const CFraction & other ) const;


    /**
     * Constructs and returns the fraction p/q= (k1*p1 +
     * k2*p2)/(k1*q1+k2*q2).  To be efficient, at least one of k1 or
     * k2 should be 1, and z1 and z2 should have determinant +1/-1.
     *
     * Note : the return by value imposes a O(prof(p/q)) complexity.
     *
     * @param z1 a fraction p1/q1.
     * @param k1 a positive number (the number of repetition of z1)
     * @param z2 a fraction p2/q2.
     * @param k2 a positive number (the number of repetition of z2)
     * @return the fraction p/q as defined above.
     */
    static CFraction compose( const CFraction & z1, uint k1, 
			      const CFraction & z2, uint k2 );

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
     * Sequences of partial numerators.
     */
    std::vector<uint> m_p;

    /**
     * Sequences of partial numerators.
     */
    std::vector<uint> m_q;

    /**
     * Sequences of partial coefficients;
     */
    std::vector<uint> m_u;


    // ------------------------- Hidden services ------------------------------
  protected:

  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'CFraction'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'CFraction' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const CFraction & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/mathutils/CFraction.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined CFraction_h

#undef CFraction_RECURSES
#endif // else defined(CFraction_RECURSES)
