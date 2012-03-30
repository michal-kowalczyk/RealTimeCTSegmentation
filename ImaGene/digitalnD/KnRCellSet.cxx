///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : KnRCellSet.cxx
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
#include "ImaGene/digitalnD/KnRCellSet.h"
#include "ImaGene/digitalnD/KnCharSet.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/KnRCellSet.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


namespace ImaGene {

const char* const KnRCellSet_RCS_ID = "@(#)class KnRCellSet definition.";

///////////////////////////////////////////////////////////////////////////////
// class KnRCellSet
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
KnRCellSet
KnRCellSet::create( const KnSpace & ks,
		 uint d, 
		 bool oriented,
		 int val )
{
  return KnRCellSet( ks, ks.getSanitizer( oriented, false ), d, oriented, val );
}


/**
 * Builds a KnCharSet equivalent to [s].
 * @param s any set of cells (oriented or not).
 * @return the corresponding KnCharSet.
 * NB: do not abuse of conversion between types of sets in Kn, since often
 * one representation is better than the other ones.
 * @todo optimize the conversion with word copies.
 */
KnCharSet
KnRCellSet::makeKnCharSet( const KnRCellSet & s )
{
  KnCharSet clone = KnCharSet::create( s.m_space, s.m_cell_dimension,
				       s.m_oriented, 0 );
  KnRCellSet::cell_iterator ip = s.begin();
  KnRCellSet::cell_iterator iq = s.end();
  while ( ip != iq )
    {
      clone += *ip;
      ++ip;
    }
  return clone;
}




///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Constructor.
 * Builds a set of [cell_dimension]-cells, given the space [ks], the 
 * dimension of each cell [cell_dimension]. The cells are oriented depending
 * on [oriented].
 * The set is empty at initialization if [val] is 0, otherwise the set
 * contains all the [cell_dimension]-cells of the space.
 * A sanitizer clears up unused bits and words of the bitset, which 
 * are present when the sizes of the coordinates are not powers of 2.
 * @param ks a reference to a digital space.
 * @param sanitizer the sanitizer used to clear unused bits and words of the bitset
 * @param the dimension of each cell.
 * @param oriented indicates whether it is a set of oriented cells or unoriented cells.
 * @param val 0=fills with zeros, otherwise fills with ones.
 */
KnRCellSet::KnRCellSet( const KnSpace & ks,
		  const Shared<Sanitizer> & sanitizer,
		  uint cell_dimension,
		  bool oriented,
		  int val )
  : Shared<Bitset1>( Bitset1( 1 ) ),
    m_space( ks ),
    m_sanitizer( sanitizer ),
    m_cell_dimension( cell_dimension ),
    m_oriented( oriented )
{
  ASSERT_KnRCellSet( cell_dimension <= ks.dim() );
  if ( oriented )
    this->rw().resize( 1 + ks.srcode( ks.slastCell( cell_dimension ) ), val );
  else
    this->rw().resize( 1 + ks.urcode( ks.ulastCell( cell_dimension ) ), val );
  // JOL: old technique
//   if ( oriented )
//     this->rw().resize( 2 * ks.trueNbCells( cell_dimension ), val );
//   else
//     this->rw().resize( ks.trueNbCells( cell_dimension ), val );
  // NB: was using more space.
  // remove possible uncorrect first bit
  this->operator[]( (Kn_gid) 0x0LL ) = ( val != 0 );
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
KnRCellSet::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[KnRCellSet cell dim=" << m_cell_dimension
	      << " oriented=" << m_oriented << " bitset=[" << endl;
  that_stream << this->ro() << "]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
KnRCellSet::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :


/**
 * Constructor from 'Bitset1'.
 * Builds a set of [cell_dimension]-cells, given the space [ks], the 
 * dimension of each cell [cell_dimension]. The cells are oriented depending
 * on [oriented].
 * The set is created with the elements specified in [bitset].
 * A sanitizer clears up unused bits and words of the bitset, which 
 * are present when the sizes of the coordinates are not powers of 2.
 * @param ks a reference to a digital space.
 * @param sanitizer the sanitizer used to clear unused bits and words of the bitset
 * @param the dimension of each cell.
 * @param oriented indicates whether it is a set of oriented cells or unoriented cells.
 * @param bitset the bitset to copy
 * @param to_sanitize when 'true', the set must be sanitized.
 */
KnRCellSet::KnRCellSet( const KnSpace & ks,
		  const Shared<Sanitizer> & sanitizer,
		  uint cell_dimension,
		  bool oriented,
		  const Bitset1 & bitset,
		  bool to_sanitize )
  : Shared<Bitset1>( Bitset1( 1 ) ),
    m_space( ks ),
    m_sanitizer( sanitizer ),
    m_cell_dimension( cell_dimension ),
    m_oriented( oriented )
{
   this->rw() = bitset;
   if ( to_sanitize )
     doSanitize();
}




//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene
