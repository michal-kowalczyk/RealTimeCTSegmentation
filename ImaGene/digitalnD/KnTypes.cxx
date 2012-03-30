///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : KnTypes.cxx
//
// Creation : 2002/07/20
//
// Version : 2002/07/20
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
//	2002/07/20 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //

#include <cstdlib>
#include <iostream>
#include "ImaGene/digitalnD/KnTypes.h"

using namespace std;

#if defined(NO_DEBUG)
#define ASSERT_KnTypes(x)
#define DEBUGCMD_KnTypes(x)
#else //defined(NO_DEBUG)
#define ASSERT_KnTypes(x) if(!(x)) \
    { cerr << "Assertion failed : (" << #x << ')' << endl \
    << "In file : " << __FILE__ << " at line #" << __LINE__ << endl \
    << "Compiled the " << __DATE__ << " at " << __TIME__ << endl; abort();}
#define DEBUGCMD_KnTypes(x) x
#endif // else defined(NO_DEBUG)


namespace ImaGene {

const char* const KnTypes_RCS_ID = "@(#)class KnTypes definition.";

///////////////////////////////////////////////////////////////////////////////
// class KnTypes
///////////////////////////////////////////////////////////////////////////////

const Kn_sign KnTypes::POS;
const Kn_sign KnTypes::NEG;
const Kn_size KnTypes::ZERO;
const Kn_size KnTypes::ONE;
const Kn_size KnTypes::MAX;

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * (NB: generally 32 is enough).
 * @return the number of bits of the type "Kn_uid".
 */
uint
KnTypes::nbBitsKn_uid()
{
  Kn_uid id = 1;
  uint nb_bits = 0;
  while ( id != 0 )
    {
      nb_bits++;
      id <<= 1;
    }
  return nb_bits;
}

/**
 * (NB: generally 32 is enough).
 * @return the number of bits of the type "Kn_sid".
 */
uint
KnTypes::nbBitsKn_sid()
{
  Kn_sid id = 1;
  uint nb_bits = 0;
  while ( id != 0 )
    {
      nb_bits++;
      id <<= 1;
    }
  return nb_bits;
}

/**
 * @param size the size of the space in a given direction.
 * @return the minimum number of bits necessary to code all size coordinates.
 */
uint
KnTypes::nbBitsCoord( Kn_size size )
{
  long nb_bits = 0;
  size--;
  while ( size != 0 )
    {
      size >>= 1;
      nb_bits++;
    }
  return nb_bits;
}

/**
 * @return 'true' if the base types have the expected size.
 */
bool
KnTypes::checkBaseTypesSize()
{
  return ( sizeof( uint8 ) == 1 )
    && ( sizeof( uint16 ) == 2 )
    && ( sizeof( uint32 ) == 4 )
    && ( sizeof( uint64 ) == 8 );
}

/**
 * Displays the size in bytes of base types.
 */
void
KnTypes::displayBaseTypesSize()
{
  cout << "        Exp Size" << endl;
  cout << "uint     ?   " << sizeof( uint ) << endl;
  cout << "uint8    1   " << sizeof( uint8 ) << endl;
  cout << "uint16   2   " << sizeof( uint16 ) << endl;
  cout << "uint32   4   " << sizeof( uint32 ) << endl;
  cout << "uint64   8   " << sizeof( uint64 ) << endl;
  cout << "Kn_*    4/8  " << sizeof( Kn_size ) << endl;
}


///////////////////////////////////////////////////////////////////////////////
// Interface - public :

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene
