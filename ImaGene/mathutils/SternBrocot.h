/** @file SternBrocot.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : SternBrocot.h
//
// Creation : 2011/02/28
//
// Version : 2011/02/28
//
// Author : JOL
//
// Summary : Header file for files SternBrocot.ih and SternBrocot.cxx
//
// History :
//	2011/02/28 : ?Name? : ?What?
//
// Rcs Id : "@(#)class SternBrocot declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(SternBrocot_RECURSES)
#error Recursive header files inclusion detected in SternBrocot.h
#else // defined(SternBrocot_RECURSES)
#define SternBrocot_RECURSES

#if !defined SternBrocot_h
#define SternBrocot_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class SternBrocot
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'SternBrocot' <p> Aim: This class is a
   * singleton. It represents the tree of irreducible fractions,
   * commonly called Stern-Brocot tree.
   */
  class SternBrocot
  {
  public:
    struct Node {
      Node( int p1, int q1, int u1, int k1, 
	    Node* ascendant_left1, Node* ascendant_right1, 
	    Node* descendant_left1, Node* descendant_right1,
	    Node* inverse1 );
      int p;
      int q;
      int u;
      int k;
      Node* ascendantLeft;
      Node* ascendantRight;
      Node* descendantLeft;
      Node* descendantRight;
      Node* inverse;
    };

    class SBFraction {
    public:
      /**
	 Default constructor. Null fraction.
      */
      SBFraction( Node* sb_node = 0 );
      bool null() const;
      int p() const;
      int q() const;
      int u() const;
      int k() const;
      SBFraction left() const;
      SBFraction right() const;
      bool even() const; 
      bool odd() const; 
      /**
	 @return the father of this fraction in O(1), ie [u0,...,uk]
	 => [u0,...,uk - 1]
      */
      SBFraction father() const;
      /**
	 @return the father of this fraction in O(1), ie
	 [u0,...,u{k-1},uk] => [u0,...,u{k-1}]
      */
      SBFraction previousPartial() const;
      /**
	 @return the inverse of this fraction in O(1), ie
	 [u0,...,uk] => [0,u0,...,uk]
      */
      SBFraction inverse() const;
      /**
	 @param kp the chosen depth of the partial fraction (kp <= k()).

	 @return the partial fraction of depth kp, ie. [u0,...,uk] =>
	 [u0,...,ukp]
      */
      SBFraction partial( int kp ) const;
      /**
	 @param i a positive integer smaller or equal to k()+2.

	 @return the partial fraction of depth k()-i, ie. [u0,...,uk] =>
	 [u0,...,u{k-i}]
      */
      SBFraction reduced( int i ) const;

      /**
	 Splitting formula, O(1) time complexity. This fraction should
	 not be 0/1 or 1/0. NB: 'this' = [f1] \oplus [f2].

	 @param f1 (returns) the left part of the split.
	 @param f2 (returns) the right part of the split.
      */
      void getSplit( SBFraction & f1, SBFraction & f2 ) const; 

      /**
	 Berstel splitting formula, O(1) time complexity. This
	 fraction should not be 0/1 or 1/0. NB: 'this' = nb1*[f1]
	 \oplus nb2*[f2]. Also, if 'this->k' is even then nb1=1,
	 otherwise nb2=1.

	 @param f1 (returns) the left part of the split (left pattern).
	 @param nb1 (returns) the number of repetition of the left pattern
	 @param f2 (returns) the right part of the split (right pattern).
	 @param nb2 (returns) the number of repetition of the right pattern
      */
      void getSplitBerstel( SBFraction & f1, unsigned int & nb1, 
			    SBFraction & f2, unsigned int & nb2 ) const; 

      /**
	 @param quotients (returns) the coefficients of the continued
	 fraction of 'this'.
      */
      void cfrac( std::vector<unsigned int> & quotients ) const;

      SBFraction mediant( SBFraction other ) const;
      bool equals( unsigned int p, unsigned int q ) const;
      bool lessThan( unsigned int p, unsigned int q ) const;
      bool moreThan( unsigned int p, unsigned int q ) const;
      bool operator==( const SBFraction & other ) const;
      bool operator<( const SBFraction & other ) const;
      bool operator>( const SBFraction & other ) const;

    private:
      Node* myNode; 
    };


    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~SternBrocot();
    
    /**
     * Constructor.
     */
    SternBrocot();

    /** The fraction 0/1 */
    static SBFraction zeroOverOne();
    /** The fraction 1/0 */
    static SBFraction oneOverZero();
    /** 
	Any fraction p/q with gcd(p,q)=1. Complexity is in sum_i u_i,
	where u_i are the partial quotients of p/q.

	@param p the numerator (>=0)
	@param q the denominator (>=0)

	@param ancestor (optionnal) any ancestor of p/q in the tree
	(for speed-up).
	
	@return the corresponding fraction in the Stern-Brocot tree.
    */
    static SBFraction fraction( unsigned int p, unsigned int q,
				SBFraction ancestor = zeroOverOne() );

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

    static Node myVirtualZeroOverOne;
    static Node myZeroOverOne;
    static Node myOneOverZero;
    static Node myOneOverOne;

    // ------------------------- Hidden services ------------------------------
  protected:

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE SternBrocot( const SternBrocot & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE SternBrocot & operator=( const SternBrocot & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'SternBrocot'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'SternBrocot' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const SternBrocot & that_object_to_display );

  /**
   * Overloads 'operator<<' for displaying objects of class 'SternBrocot::SBFraction'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'SternBrocot' to write.
   * @return the output stream after the writing.
   */
  INLINE std::ostream&
  operator<<( std::ostream & that_stream, 
	      const SternBrocot::SBFraction & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/mathutils/SternBrocot.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined SternBrocot_h

#undef SternBrocot_RECURSES
#endif // else defined(SternBrocot_RECURSES)
