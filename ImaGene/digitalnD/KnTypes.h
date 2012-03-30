/** @file KnTypes.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : KnTypes.h
//
// Creation : 2002/07/20
//
// Version : 2002/07/20
//
// Author : JOL
//
// Summary :
//
// History :
//	2002/07/20 : ?Name? : ?What?
//
// Rcs Id : "@(#)class KnTypes declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(KnTypes_RECURSES)
#error Recursive header files inclusion detected in KnTypes.h
#else // defined(KnTypes_RECURSES)
/**
 * Useful to avoid recursive inclusion.
 */
#define KnTypes_RECURSES

#if !defined KnTypes_h
#define KnTypes_h

#include "ImaGene/base/BasicTypes.h"


namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// Hardware dependent definition.
///////////////////////////////////////////////////////////////////////////////
#ifdef IMAGENE64
/**
 * Type to store a size or a number of cells in a digital space.
 */
typedef uint64 Kn_size;
/**
 * Signed version of Kn_size.
 */
typedef int64 Kn_ssize;
/**
 * Word storing only the coordinates of a cell (similar to unoriented
 * pointels).
 */
typedef uint64 Kn_coords;
/**
 * unsigned Kn code.
 */
typedef uint64 Kn_uid;
/**
 * signed Kn code.
 */
typedef uint64 Kn_sid;
/**
 * generic Kn code (signed/unsigned). Useful for structures and algorithm
 * who do not care about signed/unsigned cells.
 */
typedef uint64 Kn_gid;
/**
 * unsigned Kn restricted code.
 */
typedef uint64 Kn_urid;
/**
 * signed Kn restricted code.
 */
typedef uint64 Kn_srid;
/**
 * generic Kn restricted code. Useful for structures and algorithm
 * who do not care about signed/unsigned cells.
 */
typedef uint64 Kn_grid;
/**
 * sign of an element.
 */
typedef Kn_sid Kn_sign;

#else // ifdef IMAGENE64

/**
 * Type to store a size or a number of cells in a digital space.
 */
typedef uint32 Kn_size;
/**
 * Signed version of Kn_size.
 */
typedef int32 Kn_ssize;
/**
 * Word storing only the coordinates of a cell (similar to unoriented
 * pointels).
 */
typedef uint32 Kn_coords;
/**
 * unsigned Kn code.
 */
typedef uint32 Kn_uid;
/**
 * signed Kn code.
 */
typedef uint32 Kn_sid;
/**
 * generic Kn code (signed/unsigned). Useful for structures and algorithm
 * who do not care about signed/unsigned cells.
 */
typedef uint32 Kn_gid;
/**
 * unsigned Kn restricted code.
 */
typedef uint32 Kn_urid;
/**
 * signed Kn restricted code.
 */
typedef uint32 Kn_srid;
/**
 * generic Kn restricted code. Useful for structures and algorithm
 * who do not care about signed/unsigned cells.
 */
typedef uint32 Kn_grid;
/**
 * sign of an element.
 */
typedef Kn_sid Kn_sign;

#endif // IMAGENE64

/**
 * Invalid cell code.
 */
static const Kn_gid INVALID_CELL = (Kn_size) ~0x0LL;

/**
 * Invalid cell restricted code.
 */
static const Kn_grid INVALID_RCELL = (Kn_size) ~0x0LL;


///////////////////////////////////////////////////////////////////////////////
// struct mask of KnTypes
///////////////////////////////////////////////////////////////////////////////
/**
 * A field describes a subpart of the code of an (un)signed cell. There is
 * one field per coordinate, one field to store the topology, and a field for
 * the orientation for signed cells.
 * Codes a field mask, its inverse, the shift, and the number of masked bits
 * for the type Kn_uid.
 */
struct Kn_uid_Field 
{
  /**
   * The consecutive bits of the word where the field is defined.
   */
  Kn_uid mask;
  /**
   * The bits of the word not in the field.
   */
  Kn_uid inv_mask;
  /**
   * The shift in the word to reach the field.
   */
  uint shift;
  /**
   * The number of consecutive bits of the field.
   */
  uint nb_bits;
};

/**
 * A field describes a subpart of the code of an (un)signed cell. There is
 * one field per coordinate, one field to store the topology, and a field for
 * the orientation for signed cells.
 * Codes a mask, its inverse, the shift, and the number of masked bits
 * for the type Kn_sid.
 */
struct Kn_sid_Field
{
  /**
   * The consecutive bits of the word where the field is defined.
   */
  Kn_sid mask;
  /**
   * The bits of the word not in the field.
   */
  Kn_sid inv_mask;
  /**
   * The shift in the word to reach the field.
   */
  uint shift;
  /**
   * The number of consecutive bits of the field.
   */
  uint nb_bits;
};

/**
 * Given a field [FIELD], returns the corresponding mask.
 * @see Kn_uid_Field,Kn_sid_Field
 */
#define Field_mask(FIELD)     FIELD.mask
/**
 * Given a field [FIELD], returns the corresponding inversed mask.
 * @see Kn_uid_Field,Kn_sid_Field
 */
#define Field_inv_mask(FIELD) FIELD.inv_mask
/**
 * Given a field [FIELD], returns its shift in the word.
 * @see Kn_uid_Field,Kn_sid_Field
 */
#define Field_shift(FIELD)    FIELD.shift
/**
 * Given a field [FIELD], returns its number of bits.
 * @see Kn_uid_Field,Kn_sid_Field
 */
#define Field_nb_bits(FIELD)  FIELD.nb_bits

/**
 * Given a cell code [CODE] and a field [FIELD], returns the part of the code
 * corresponding to the given field.
 * @see Kn_uid_Field,Kn_sid_Field
 */
#define Field_select(CODE, FIELD) \
  ( ( CODE ) & ( FIELD.mask ) )
/**
 * Given a cell code [CODE] and a field [FIELD], returns the code without the
 * part corresponding to the given field.
 * @see Kn_uid_Field,Kn_sid_Field
 */
#define Field_unselect(CODE, FIELD) \
  ( ( CODE ) & ( FIELD.inv_mask ) )
/**
 * Given a cell code [CODE] and a field [FIELD], modifies the code to remove
 * the part corresponding to the given field.
 * @see Kn_uid_Field,Kn_sid_Field
 */
#define Field_reset(CODE, FIELD) \
  CODE &= ( FIELD.inv_mask )
/**
 * Given a cell code [CODE], a field [FIELD] and a value [val], modifies the
 * code to set the given value in the given field.
 * @see Kn_uid_Field,Kn_sid_Field
 */
#define Field_setValue(CODE, VAL, FIELD) \
  CODE = ( ( CODE ) & ( FIELD.inv_mask ) ) | ( ( VAL ) << ( FIELD.shift ) )
/**
 * Given a cell code [CODE], a field [FIELD] and a coded value [FCODE], 
 * returns the code with the given coded value in the given field.
 * @see Kn_uid_Field,Kn_sid_Field
 */
#define Field_getCode(CODE, FCODE, FIELD) \
  ( ( ( CODE ) & ( FIELD.inv_mask ) ) | ( FCODE ) )
/**
 * Given a cell code [CODE], a field [FIELD] and a coded value [FCODE], 
 * modifies the code to set the given coded value in the given field.
 * @see Kn_uid_Field,Kn_sid_Field
 */
#define Field_setCode(CODE, FCODE, FIELD) \
  CODE = ( ( CODE ) & ( FIELD.inv_mask ) ) | ( FCODE )
/**
 * Given a field [FIELD] and a value [VAL], returns the coded value.
 * @see Kn_uid_Field,Kn_sid_Field
 */
#define Field_codeOf(VAL, FIELD) \
  ( ( VAL ) << ( FIELD.shift ) )
/**
 * Given a field [FIELD] and a cell code [CODE], returns the value of this 
 * field.
 * @see Kn_uid_Field,Kn_sid_Field
 */
#define Field_valueOf(CODE, FIELD) \
  ( ( ( CODE ) & ( FIELD.mask ) ) >> ( FIELD.shift ) )
/**
 * Given a field [FIELD], a cell code [CODE] and a bit index [BIT] in the 
 * field, returns true/false depending if this bit is set in the specified
 * field of the code.
 * @see Kn_uid_Field,Kn_sid_Field
 */
#define Field_selectBit(CODE, BIT, FIELD) \
  ( ( CODE ) & ( KnTypes::ONE << ( ( BIT ) + ( FIELD.shift ) ) ) )
/**
 * Given a field [FIELD] and a bit index [BIT] in the 
 * field, returns the mask to test in a cell code if this bit is set.
 * @see Kn_uid_Field,Kn_sid_Field
 */
#define Field_selectorBit(BIT, FIELD) \
  ( KnTypes::ONE << ( ( BIT ) + ( FIELD.shift ) ) )

///////////////////////////////////////////////////////////////////////////////
// class KnTypes
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'KnTypes' <p>
 * Aim: To collect several low-level functions for manipulating bits and
 * 'Kn_uid' and 'Kn_sid' codes.
 */
class KnTypes
{
  // ------------------------- Static services ------------------------------
public:
  /**
   * By convention, positive is zero.
   */
  static const Kn_sign POS = (Kn_sign) 0x0LL;
  /**
   * By convention, negative is non zero.
   */
  static const Kn_sign NEG = (Kn_sign) ~0x0LL;
  
 public:
  /**
   * The value 0, either in 32 or 64 bits.
   */
  static const Kn_size ZERO = (Kn_size) 0x0LL;

  /**
   * The value 1, either in 32 or 64 bits.
   */
  static const Kn_size ONE = (Kn_size) 0x1LL;

  /**
   * The value 0xfff...fff, either in 32 or 64 bits.
   */
  static const Kn_size MAX = (Kn_size) ~0x0LL;

  /**
   * (NB: generally 32 is enough).
   * @return the number of bits of the type "Kn_uid".
   */
  static uint nbBitsKn_uid();

  /**
   * (NB: generally 32 is enough).
   * @return the number of bits of the type "Kn_sid".
   */
  static uint nbBitsKn_sid();

  /**
   * @param size the size of the space in a given direction.
   * @return the minimum number of bits necessary to code all size coordinates.
   */
  static uint nbBitsCoord( Kn_size size );
  /**
   * @return 'true' if the base types have the expected size.
   */
  static bool checkBaseTypesSize();
  /**
   * Displays the size in bytes of base types.
   */
  static void displayBaseTypesSize();

};


} // namespace ImaGene

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined KnTypes_h

#undef KnTypes_RECURSES
#endif // else defined(KnTypes_RECURSES)
