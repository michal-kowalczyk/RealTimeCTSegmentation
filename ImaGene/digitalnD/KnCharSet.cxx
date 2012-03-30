///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : KnCharSet.cxx
//
// Creation : 2002/08/02
//
// Version : 2002/08/02
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
//	2002/08/02 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/KnCharSet.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/KnCharSet.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


namespace ImaGene {

const char* const KnCharSet_RCS_ID = "@(#)class KnCharSet definition.";

///////////////////////////////////////////////////////////////////////////////
// class KnCharSet
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// ------------------------- static public services -------------------------


/**
 * Returns a set (defined with a characteristic function) which may contain
 * any oriented or unoriented cell of dimension [d]. The set is empty or 
 * filled depending on [val].
 * @param ks the space in which the set is defined.
 * @param d the dimension of the cells hold in the returned set.
 * @param oriented when 'true' the set contains oriented cells otherwise the set contains unoriented cells.
 * @param val 0=fills with zeros, otherwise fills with ones.
 * @return a (modifiable) set of [d]-cells (oriented or unoriented).
 */
KnCharSet
KnCharSet::create( const KnSpace & ks,
		   uint d, 
		   bool oriented,
		   int val )
{
  ASSERT_KnCharSet( d <= ks.dim() );
  
  if ( oriented )
    {
      Kn_sid f = ks.sfirstCell( d );
      Kn_sid l = ks.slastCell( d );
      return KnCharSet( ks.getSanitizer
			( oriented, ks.stopology( f ) != ks.stopology( l ) ),
			f, l, val );
    }
  Kn_uid f = ks.ufirstCell( d );
  Kn_uid l = ks.ulastCell( d );
  return KnCharSet( ks.getSanitizer
		    ( oriented, ks.utopology( f ) != ks.utopology( l ) ),
		    f, l, val );
}


/**
 * Returns a set (defined with a characteristic function) which may contain
 * any oriented or unoriented cell of any dimension. The set is empty or 
 * filled depending on [val].
 * @param ks the space in which the set is defined.
 * @param oriented when 'true' the set contains oriented cells otherwise the set contains unoriented cells.
 * @param val 0=fills with zeros, otherwise fills with ones.
 * @return a (modifiable) set of cells (oriented or unoriented).
 */
KnCharSet
KnCharSet::create( const KnSpace & ks,
		   bool oriented,
		   int val )
{
  if ( oriented )
    {
      Kn_sid f = ks.sfirst();
      Kn_sid l = ks.slast();
      return KnCharSet( ks.getSanitizer( oriented, true ), f, l, val );
    }
  Kn_uid f = ks.ufirst();
  Kn_uid l = ks.ulast();
  return KnCharSet( ks.getSanitizer( oriented, true ), f, l, val );
}

/**
 * Returns a set (defined with a characteristic function) which may contain
 * any cell of same topology as [c]. The set is empty or 
 * filled depending on [val].
 * @param ks the space in which the set is defined.
 * @param c all cells in the set will have the same topology as [c].
 * @param val 0=fills with zeros, otherwise fills with ones.
 * @return a (modifiable) set of unoriented cells.
 */
KnCharSet
KnCharSet::ucreate( const KnSpace & ks,
		    Kn_uid c,
		    int val )
{
  return KnCharSet( ks.getSanitizer( false, false ),
 		    ks.ufirst( c ), ks.ulast( c ),
		    val );
}


/**
 * Returns a set (defined with a characteristic function) which may contain
 * any cell of same topology as [c]. The set is empty or 
 * filled depending on [val].
 * @param ks the space in which the set is defined.
 * @param c all cells in the set will have the same topology as [c].
 * @param val 0=fills with zeros, otherwise fills with ones.
 * @return a (modifiable) set of oriented cells.
 */
KnCharSet
KnCharSet::screate( const KnSpace & ks,
		    Kn_sid c,
		    int val )
{
  return KnCharSet( ks.getSanitizer( true, false ),
		    ks.ufirst( ks.toPos( c ) ), ks.ulast( ks.toNeg( c ) ),
		    val );
}








///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Constructor.
 * Builds a set of Kn_uid or Kn_sid, given the element with smallest id [min]
 * and the element with biggest id [max]. The set is empty at initialization
 * if [val] is 0, otherwise the set contains all the elements from [min] to
 * [max]. A sanitizer clears up unused bits and words of the bitset, which 
 * are present when the sizes of the coordinates are not powers of 2.
 * @param sanitizer the sanitizer used to clear unused bits and words of the bitset
 * @param min the smallest possible id.
 * @param max the biggest possible id.
 * @param val 0=fills with zeros, otherwise fills with ones.
 */
KnCharSet::KnCharSet( const Shared<Sanitizer> & sanitizer,
		      Kn_gid min, Kn_gid max, 
		      int val )
  : Shared<Bitset1>( Bitset1( 1 ) ),
    m_sanitizer( sanitizer ),
    m_min( min ),
    m_max1( max + 1 )
{
  this->rw().resize( m_max1 - min, val );
  this->operator[]( min ) = ( val != 0 );
  if ( val )
    doSanitize();
}



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
KnCharSet::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[KnCharSet min=" << m_min
	      << " max=" << m_max1 - 1 << " bitset=[" << endl;
  that_stream << this->ro() << "]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
KnCharSet::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :


/**
 * Copy constructor from BitSet1.
 * @param bitset the bitset to copy
 * @param sanitizer the sanitizer used to clear unused bits and words of the bitset
 * @param min the smallest possible id.
 * @param max the biggest possible id.
 * @param to_sanitize when 'true', the set must be sanitized.
 */
KnCharSet::KnCharSet( const Bitset1 & bitset, 
			  const Shared<Sanitizer> & sanitizer,
			  Kn_gid min, Kn_gid max,
			  bool to_sanitize )
  : Shared<Bitset1>( Bitset1( 1 ) ),
    m_sanitizer( sanitizer ),
    m_min( min ),
    m_max1( max + 1 )
{
  this->rw() = bitset;
  if ( to_sanitize )
    doSanitize();
}


//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene
