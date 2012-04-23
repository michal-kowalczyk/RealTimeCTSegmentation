///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : Arguments.cxx
//
// Creation : 2004/01/21
//
// Version : 2004/01/21
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
//	2004/01/21 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <cstring>
#include "ImaGene/base/Arguments.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/base/Arguments.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const Arguments_RCS_ID = "@(#)class Arguments definition.";



/////////////////////////////////////////////////////////////////////////////
// class Arguments::Option
/////////////////////////////////////////////////////////////////////////////
bool
ImaGene::Arguments::Option::operator==( const std::string & s ) const
{
  //cout << "opt(" << name << ") == (" << s << ") = " << (name == s) << endl;
  return name == s;
}

bool
ImaGene::Arguments::Option::operator!=( const std::string & s ) const
{
  //cout << "opt(" << name << ") != (" << s << ") = " << (name != s) << endl;
  return name != s;
}

bool
ImaGene::Arguments::Option::operator<( const std::string & s ) const
{
  //cout << "opt(" << name << ") < (" << s << ") = " << (name < s) << endl;
  return name < s;
}

std::string 
ImaGene::Arguments::Option::getValue( uint i ) const
{
  if ( i < values.size() )
    return values[ i ];
  return "";
}

int 
ImaGene::Arguments::Option::getIntValue( uint i ) const
{
  return atoi( getValue( i ).c_str() );
}

long long 
ImaGene::Arguments::Option::getLongLongValue( uint i ) const
{
  return atoll( getValue( i ).c_str() );
}

float 
ImaGene::Arguments::Option::getFloatValue( uint i ) const
{
  return atof( getValue( i ).c_str() );
}

double
ImaGene::Arguments::Option::getDoubleValue( uint i ) const
{
  return atof( getValue( i ).c_str() );
}

void 
ImaGene::Arguments::Option::setValue( uint i, const std::string & s ) 
{
  if ( i < values.size() )
    values[ i ] = s;
}



/////////////////////////////////////////////////////////////////////////////
// class Arguments::Options
/////////////////////////////////////////////////////////////////////////////

bool
ImaGene::Arguments::Options::add( const Option & opt ) 
{
  m_options.push_back( opt );
  return true;
}

ImaGene::Arguments::Option*
ImaGene::Arguments::Options::get( string n )
{
  vector<Option>::iterator p;
  p = find( m_options.begin(), m_options.end(), n );
  if ( p != m_options.end() )
    return &( *p );
  return 0;
}

const ImaGene::Arguments::Option*
ImaGene::Arguments::Options::get( string n ) const
{
  vector<Option>::const_iterator p;
  p = find( m_options.begin(), m_options.end(), n );
  if ( p != m_options.end() )
    return &( *p );
  return 0;
}

uint 
ImaGene::Arguments::Options::nb() const
{
  return m_options.size();
}

ImaGene::Arguments::Option*
ImaGene::Arguments::Options::get( uint i )
{
  ASSERT_Arguments( i < nb() );
  return &( m_options[ i ] );
}

const ImaGene::Arguments::Option*
ImaGene::Arguments::Options::get( uint i ) const
{
  ASSERT_Arguments( i < nb() );
  return &( m_options[ i ] );
}

void
ImaGene::Arguments::Options::setOptionPresence( std::string n, bool value )
{
  vector<Option>::iterator p;
  p = find( m_options.begin(), m_options.end(), n );
  if ( p != m_options.end() )
    p->present = value;
}

bool 
ImaGene::Arguments::Options::getOptionPresence( std::string n ) const
{
  const Option* opt = get( n );
  return ( opt != 0 ) && opt->present;
}


/////////////////////////////////////////////////////////////////////////////
// class Arguments
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::Arguments::~Arguments()
{
}

/**
 * Constructor. Initializes every argument by their default value.
 */
ImaGene::Arguments::Arguments()
{
  init();
  resetToDefault();
}


/**
 * Adds a boolean non-standard argument.
 * @param name the option name.
 * @param description the option description.
 */
bool
ImaGene::Arguments::addBooleanOption( const std::string & name,
				      const std::string & description )
{
  if ( m_opts.get( name ) != 0)
    return false;
  m_opts.add( Option( name, 0, description ) );
  return true;
}
  

/**
 * Adds a non-standard argument with [nb] parameters.
 * @param name the option name.
 * @param description the option description.
 * @param nb the number of parameters to this option
 * @param default an array storing the default values for the [nb] parameters.
 */
bool
ImaGene::Arguments::addOption( const std::string & name,
			       const std::string & description,
			       uint nb,
			       const std::vector<std::string> & defaultvals )
{
  if ( m_opts.get( name ) != 0)
    return false;
  m_opts.add( Option( name, nb, description, defaultvals ) );
  return true;
}

/**
 * Adds a non-standard argument with 1 parameter.
 * @param name the option name.
 * @param description the option description.
 * @param def1 the default value of the first parameter.
 */
bool 
ImaGene::Arguments::addOption( const std::string & name,
			       const std::string & description,
			       const std::string & def1 )
{
  std::vector<std::string> v( 1 );
  v[ 0 ] = def1;
  return addOption( name, description, 1, v );
}


/**
 * Adds a non-standard argument with 2 parameters.
 * @param name the option name.
 * @param description the option description.
 * @param def1 the default value of the first parameter.
 * @param def2 the default value of the second parameter.
 */
bool 
ImaGene::Arguments::addOption( const std::string & name,
			       const std::string & description,
			       const std::string & def1,
			       const std::string & def2 )
{
  std::vector<std::string> v( 2 );
  v[ 0 ] = def1;
  v[ 1 ] = def2;
  return addOption( name, description, 2, v );
}

/**
 * Adds a non-standard argument with 3 parameters.
 * @param name the option name.
 * @param description the option description.
 * @param def1 the default value of the first parameter.
 * @param def2 the default value of the second parameter.
 * @param def3 the default value of the second parameter.
 */
bool
ImaGene::Arguments::addOption( const std::string & name,
			       const std::string & description,
			       const std::string & def1,
			       const std::string & def2,
			       const std::string & def3 )
{
  std::vector<std::string> v( 3 );
  v[ 0 ] = def1;
  v[ 1 ] = def2;
  v[ 2 ] = def3;
  return addOption( name, description, 3, v );
}


/**
 * Adds a non-standard argument with 4 parameters.
 * @param name the option name.
 * @param description the option description.
 * @param def1 the default value of the first parameter.
 * @param def2 the default value of the second parameter.
 * @param def3 the default value of the third parameter.
 * @param def4 the default value of the fourth parameter.
 */
bool
ImaGene::Arguments::addOption( const std::string & name,
			       const std::string & description,
			       const std::string & def1,
			       const std::string & def2,
			       const std::string & def3,
			       const std::string & def4 )
{
  std::vector<std::string> v( 4 );
  v[ 0 ] = def1;
  v[ 1 ] = def2;
  v[ 2 ] = def3;
  v[ 3 ] = def4;
  return addOption( name, description, 4, v );
}



/**
 * Checks if an option was given in the arguments.
 */
bool
ImaGene::Arguments::check( const std::string & name ) const
{
  return m_opts.getOptionPresence( name );
}

/**
 * @param name an option name.
 * @return the option of name [name]. May be present or not and may have parameters.
 */
const ImaGene::Arguments::Option* 
ImaGene::Arguments::getOption( const std::string & name ) const
{
  return m_opts.get( name );
}


/**
 * Analyses the given arguments and updates the object accordingly.
 * @param argc the number of arguments.
 * @param argv an array of strings storing the arguments.
 * @return 'true' if the arguments were valid, 'false' otherwise.
 */
bool 
ImaGene::Arguments::readArguments( int argc, char* argv[] )
{
  int i;

  for ( i = 1; i < argc; ++i )
    {
      m_opts.setOptionPresence( argv[ i ], true );
      // help "-h"
      if ( strcmp( argv[ i ], "-h" ) == 0 ) return false;
      // // dim "-d"
      // else if ( ( strcmp( argv[ i ], "-d" ) == 0 )
      // 		&& ( ( i + 1 ) < argc ) )
      // 	{
      // 	  dim = atoi( argv[ i + 1 ] );
      // 	  ++i;
      // 	}
      // // radius "-r"
      // else if ( ( strcmp( argv[ i ], "-r" ) == 0 )
      // 		&& ( ( i + 1 ) < argc ) )
      // 	{
      // 	  radius = atof( argv[ i + 1 ] );
      // 	  radius_first = radius;
      // 	  radius_last = radius;
      // 	  ++i;
      // 	}
      // // radius "-ri"
      // else if ( ( strcmp( argv[ i ], "-ri" ) == 0 )
      // 		&& ( ( i + 3 ) < argc ) )
      // 	{ 
      // 	  radius_first = atof( argv[ i + 1 ] );
      // 	  radius_last = atof( argv[ i + 2 ] );
      // 	  radius_incr = atof( argv[ i + 3 ] );
      // 	  radius = radius_first;
      // 	  i += 3;
      // 	}
      // // dim "-x"
      // else if ( ( strcmp( argv[ i ], "-x" ) == 0 )
      // 		&& ( ( i + 1 ) < argc ) )
      // 	{
      // 	  sizes[ 0 ] = atoi( argv[ i + 1 ] );
      // 	  ++i;
      // 	}
      // // dim "-y"
      // else if ( ( strcmp( argv[ i ], "-y" ) == 0 )
      // 		&& ( ( i + 1 ) < argc ) )
      // 	{
      // 	  sizes[ 1 ] = atoi( argv[ i + 1 ] );
      // 	  ++i;
      // 	}
      // // dim "-z"
      // else if ( ( strcmp( argv[ i ], "-z" ) == 0 )
      // 		&& ( ( i + 1 ) < argc ) )
      // 	{
      // 	  sizes[ 2 ] = atoi( argv[ i + 1 ] );
      // 	  ++i;
      // 	}
      // // dim "-t"
      // else if ( ( strcmp( argv[ i ], "-t" ) == 0 )
      // 		&& ( ( i + 1 ) < argc ) )
      // 	{
      // 	  sizes[ 3 ] = atoi( argv[ i + 1 ] );
      // 	  ++i;
      // 	}
      // // option "-curv_symtgt_angle"
      // else if ( strcmp( argv[ i ], "-curv_symtgt_angle" ) == 0 )
      // 	{
      // 	  curv_estimator = CURVATURE_BY_ANGLE_SYMTGT;
      // 	}
      // // option "-curv_circumcircle"
      // else if ( strcmp( argv[ i ], "-curv_circumcircle" ) == 0 )
      // 	{
      // 	  curv_estimator = CURVATURE_BY_CIRCUMSCRIBED_CIRCLE;
      // 	}

      else // non-standard options.
	{
	  //if ( m_opts.get( argv[ i ] ) == 0 ) return false;
	  Option* opt = m_opts.get( argv[ i ] );
	  if ( opt == 0 ) return false;
	  for ( uint j = 0; j < opt->nbparams; ++j )
	    {
	      ++i;
	      if ( i < argc )
		opt->setValue( j, argv[ i ] );
	    }
	}

    } // for ( i = 1; i < argc; ++i )

  return true;
}

/**
 * All arguments are reset to their default value.
 */
void
ImaGene::Arguments::resetToDefault()
{
  // dim = 2;
  // sizes[ 0 ] = 128;
  // sizes[ 1 ] = 128;
  // sizes[ 2 ] = 128;
  // sizes[ 3 ] = 64;
  // radius = 10.0;
  // radius_first = radius;
  // radius_last = radius;
  // radius_incr = 1.0;
  // digital = false;
  // shape = BALL;
  // curv_estimator = NO_CURVATURE_ESTIMATOR;
}


/**
 * @return the possible options.
 */
ImaGene::Arguments::Options &
ImaGene::Arguments::getOptions()
{
  return m_opts;
}

/**
 * @return the possible options.
 */
const ImaGene::Arguments::Options &
ImaGene::Arguments::getOptions() const
{
  return m_opts;
}

/**
 * Builds an usage from the command name and a list of options
 * separated by whitespaces.
 * @param command the name of the command.
 * @param text a description of the command usage.
 * @param options a list of options separated by whitespaces (e.g, "-d -x -y").
 * @return a string listing the descriptions of the options specified in [options].
 */
string
ImaGene::Arguments::usage( const string & command, 
			   const std::string & text,
			   const string & options )
{
  string u;
  if ( options != "" )
    {
      u = "Usage: " + command + " " + options + '\n';
      u += "\n" + text + "\n\nAvailable options:\n"; 
      size_t pos = 0;
      size_t pos_prev = 0;
      while ( pos_prev != string::npos )
	{
	  //cerr << u << endl;
	  pos = options.find( ' ', pos_prev );
	  string opt = options.substr( pos_prev, pos - pos_prev );
	  //cout << pos << " " << pos_prev << " (" << opt << ")" << endl;
	  const Option* opt_found = getOptions().get( opt );
	  if ( opt_found != 0 )
	    u += "\t" + opt_found->description + '\n';
	  if ( pos != string::npos )
	    pos_prev = pos + 1;
	  else 
	    pos_prev = string::npos;
	}
    }
  else
    {
      u = "Usage: " + command + '\n';
      u += "\n" + text + "\n\nAvailable options:\n"; 
      for ( uint p = 0; p < getOptions().nb(); ++p )
	{
	    u += "\t" + getOptions().get( p )->description + '\n';
	}

    }
  return u;
}



/////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::Arguments::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[Arguments]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::Arguments::OK() const
{
  return true;
}

/////////////////////////////////////////////////////////////////////////////
// Internals - private :

/**
 * Initializes the options.
 */
void
ImaGene::Arguments::init()
{
  // m_opts.add
  //   ( Option( "-h", 0, "-h: display usage" ) ); 
  // m_opts.add
  //   ( Option( "-d", 1, "-d <uint>: sets the dimension of the space" ) ); 
  // m_opts.add
  //   ( Option( "-x", 1, "-x <uint>: sets the space size along the 0-coordinate" ) ); 
  // m_opts.add
  //   ( Option( "-y", 1, "-y <uint>: sets the space size along the 1-coordinate" ) ); 
  // m_opts.add
  //   ( Option( "-z", 1, "-z <uint>: sets the space size along the 2-coordinate" ) ); 
  // m_opts.add
  //   ( Option( "-t", 1, "-t <uint>: sets the space size along the 3-coordinate" ) ); 
  // m_opts.add
  //   ( Option( "-r", 1, "-r <float>: specifies the size of a shape (generally radius of sphere)" ) ); 
  // m_opts.add
  //   ( Option( "-ri", 3, "-ri <float_begin> <float_end> <float_incr>: specifies a range of sizes for shapes (generally radius of spheres)" ) ); 
  // m_opts.add
  //   ( Option( "-digital", 0, "-digital: specifies the display of surfaces as a subset of the regular cubical grid" ) );
  // m_opts.add
  //   ( Option( "-curv_symtgt_angle", 0, "-curv_symtgt_angle: choose the angle variation of symmetric tangents as curvature estimator" ) );
  // m_opts.add
  //   ( Option( "-curv_circumcircle", 0, "-curv_circumcircle: choose the curvature of the circumcircle to endpoints of halftangents as curvature estimator" ) );

}
  

//                                                                         //
/////////////////////////////////////////////////////////////////////////////

