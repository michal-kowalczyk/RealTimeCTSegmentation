///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : StandardArguments.cxx
//
// Creation : 2008/12/17
//
// Version : 2008/12/17
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
//	2008/12/17 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/base/StandardArguments.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/base/StandardArguments.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const StandardArguments_RCS_ID = "@(#)class StandardArguments definition.";

/**
 * The default input stream.
 */
ifstream ImaGene::StandardArguments::m_input_stream;

/**
 * The default output stream.
 */
ofstream ImaGene::StandardArguments::m_output_stream;

///////////////////////////////////////////////////////////////////////////////
// class StandardArguments
///////////////////////////////////////////////////////////////////////////////

/**
 * Add the standard arguments for debug and trace to argument
 * object [args].
 * -debug: debug mode (force some tests).
 * -trace <level>: trace information level (0: no trace, 3: very verbose)
 * -timing <level>: displays some timings (0: no timings, 3: lots).
 *
 * @param args (updated) the argument object to enrich.
 * @param debug if 'true' add "-debug"
 * @param trace if 'true' add "-trace"
 * @param timings if 'true' add "-timing"
 */
void
ImaGene::StandardArguments::addDebugArgs( Arguments & args, 
					  bool debug, 
					  bool trace,
					  bool timings )
{
  if ( debug )
    args.addBooleanOption( "-debug", "-debug: debug mode (force some tests). Do not replace the -DEBUG or Debug configuration." );
  if ( trace )
    args.addOption( "-trace", "-trace <level>: trace information level (0: no trace, 3: very verbose)", "0" );
  if ( timings )
    args.addOption( "-timing", "-timing <level>: displays some timings (0: no timings, 3: lots).", "0" );
}


/**
 * Add the standard arguments for I/O and trace to argument
 * object [args].
 * -input <file_in>: name of input file.
 * -output <file_in>: name of output file.
 *
 * @param args (updated) the argument object to enrich.
 * @param in if 'true' add "-input"
 * @param out if 'true' add "-output"
 * @see openInput, openOutput
 */
void 
ImaGene::StandardArguments::addIOArgs( Arguments & args, bool in, bool out )
{
  if ( in )
    args.addOption( "-input", "-input <file_name>: set input file name as <file_name>, or standard input if <file_name> is '-'", "-" ); 
  if ( out )
    args.addOption( "-output", "-output <file_name>: set output file name as <file_name>, or standard output if <file_name> is '-'", "-" ); 
}


/**
 * Add the standard arguments for digital spaces to argument
 * object [args].
 * -d <dim>: dimension of the space
 * -x <sx>: size of the space along x, 0-axis
 * -y <sy>: size of the space along y, 1-axis
 * -z <sz>: size of the space along z, 2-axis
 * -t <st>: size of the space along t, 3-axis
 * -4567 <s4> <s5> <s6> <s7>: sizes of the space along axes 4 to 7.
 * -r <radius>: define a radius.
 * -ri <first> <last> <incr>: define a sequence of radii.
 * -digital: tells that the surface should be digital.
 *
 * @param args (updated) the argument object to enrich.
 * @param dim the maximum meaningful dimension.
 * @param radius if 'true' add radius parameters
 * @param digital if 'true' add digital parameters
 */
void
ImaGene::StandardArguments::addDigitalArgs( Arguments & args, 
					    uint dim, 
					    bool radius, 
					    bool digital )
{
  if ( dim > 0 )
    args.addOption( "-d", "-d <uint>: sets the dimension of the space", "2" ); 
  if ( dim >= 1 )
    args.addOption( "-x", "-x <sx>: sets the space size along the 0-coordinate", "128" );
  if ( dim >= 2 )
    args.addOption( "-y", "-y <sy>: sets the space size along the 1-coordinate", "128" );
  if ( dim >= 3 )
    args.addOption( "-z", "-z <sz>: sets the space size along the 2-coordinate", "128" );
  if ( dim >= 4 )
    args.addOption( "-t", "-t <st>: sets the space size along the 3-coordinate", "32" );
  if ( dim >= 5 )
    args.addOption( "-4567", "-4567 <s4> <s5> <s6> <s7>: sizes of the space along axes 4 to 7.", "2", "2", "2", "2" ); 
  if ( radius )
    {
      args.addOption( "-r", "-r <float>: specifies the size of a shape (generally radius of sphere)", "10" );
      args.addOption( "-ri", "-ri <first> <last> <incr>: define a sequence of (possibly floating-point) radii.", "10.0", "10.0", "1.0" );
    }
  if ( digital )
    args.addBooleanOption( "-digital", "-digital: specifies the display of surfaces as a subset of the regular cubical grid" );
}



/**
 * Open input file or standard input depending on arguments.
 *
 * @param args the argument object.
 * @return an input stream (cin or file)
 * @see addIOArgs
 */
istream & 
ImaGene::StandardArguments::openInput( const Arguments & args )
{
  string fname = args.getOption( "-input" )->getValue( 0 );
  if ( fname == "-" )
    return cin;
  else 
    {
      m_input_stream.open( fname.c_str() );
      return m_input_stream;
    }
}

/**
 * Open output file or standard output depending on arguments. This
 * method is not reentrant.
 *
 * @param args the argument object.
 * @return an output stream (cout or file)
 * @see addIOArgs
 */
std::ostream & 
ImaGene::StandardArguments::openOutput( const Arguments & args )
{
  string fname = args.getOption( "-output" )->getValue( 0 );
  if ( fname == "-" )
    return cout;
  else 
    {
      m_output_stream.open( fname.c_str() );
      return m_output_stream;
    }
}

/**
 * @param args the argument object.
 *
 * @return the dimension of the digital space.
 *
 * @see addDigitalArgs
 */
unsigned int
ImaGene::StandardArguments::dim( const Arguments & args )
{
  return args.getOption( "-d" )->getIntValue( 0 );
}

/**
 * Fills the sizes of the digital space as specified in the
 * argument object.
 *
 * @param args the argument object.
 *
 * @param sizes (modified) a array of size at least the dimension
 * of the space.
 *
 * @see addDigitalArgs
 */
void
ImaGene::StandardArguments::fillSizes( const Arguments & args, 
				       unsigned int* sizes )
{
  uint d = dim( args );
  if ( d > 0 ) sizes[ 0 ] = (unsigned int) args.getOption( "-x" )->getIntValue( 0 );
  if ( d > 1 ) sizes[ 1 ] = (unsigned int) args.getOption( "-y" )->getIntValue( 0 );
  if ( d > 2 ) sizes[ 2 ] = (unsigned int) args.getOption( "-z" )->getIntValue( 0 );
  if ( d > 3 ) sizes[ 3 ] = (unsigned int) args.getOption( "-t" )->getIntValue( 0 );
  if ( d > 4 ) 
    for ( uint i = 0; i < 4; ++i )
      sizes[ 4 + i ] = (unsigned int) args.getOption( "-4567" )->getIntValue( i );
}

/**
 * Fills the sizes of the digital space as specified in the
 * argument object.
 *
 * @param args the argument object.
 *
 * @param sizes (modified) a array of size at least the dimension
 * of the space.
 *
 * @see addDigitalArgs
 */
void 
ImaGene::StandardArguments::fillSizes( const Arguments & args, 
				       unsigned long long* sizes )
{
  uint d = dim( args );
  if ( d > 0 ) sizes[ 0 ] = (unsigned long long) args.getOption( "-x" )->getLongLongValue( 0 );
  if ( d > 1 ) sizes[ 1 ] = (unsigned long long) args.getOption( "-y" )->getLongLongValue( 0 );
  if ( d > 2 ) sizes[ 2 ] = (unsigned long long) args.getOption( "-z" )->getLongLongValue( 0 );
  if ( d > 3 ) sizes[ 3 ] = (unsigned long long) args.getOption( "-t" )->getLongLongValue( 0 );
  if ( d > 4 ) 
    for ( uint i = 0; i < 4; ++i )
      sizes[ 4 + i ] = (unsigned long long) args.getOption( "-4567" )->getLongLongValue( i );
}


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::StandardArguments::~StandardArguments()
{
}



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::StandardArguments::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[StandardArguments]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::StandardArguments::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
