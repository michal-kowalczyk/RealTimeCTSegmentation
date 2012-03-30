/** @file Arguments.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : Arguments.h
//
// Creation : 2004/01/21
//
// Version : 2004/01/21
//
// Author : JOL
//
// Summary : Header file for files Arguments.ih and Arguments.cxx
//
// History :
//	2004/01/21 : ?Name? : ?What?
//
// Rcs Id : "@(#)class Arguments declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(Arguments_RECURSES)
#error Recursive header files inclusion detected in Arguments.h
#else // defined(Arguments_RECURSES)
#define Arguments_RECURSES

#if !defined Arguments_h
#define Arguments_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <vector>
#include "ImaGene/base/BasicTypes.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene {

///////////////////////////////////////////////////////////////////////////////
// class Arguments
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'Arguments' <p> Aim: A class to analyse a list
 * of arguments given to a program and to store the arguments. This
 * class is specific to the many test programs in the sense that the
 * expected parameters are specific (but common) to these programs.
 */
class Arguments
{

  // ------------------------- Helper classes ------------------------------
public:

  enum Shape { 
    NO_SHAPE, BALL, PARALLEPIPED
  };

  enum CurvatureEstimator {
    NO_CURVATURE_ESTIMATOR, CURVATURE_BY_ANGLE_SYMTGT, CURVATURE_BY_CIRCUMSCRIBED_CIRCLE
  };

  class Option {
  public:
    std::string name;
    uint nbparams;
    std::string description;
    bool present;
    std::vector<std::string> values;

    INLINE Option( std::string na, uint nb, std::string descr );
    INLINE Option( std::string na, uint nb, std::string descr,
		       std::vector<std::string> defaultval );
    bool operator==( const std::string & s ) const;
    bool operator!=( const std::string & s ) const;
    bool operator<( const std::string & s ) const;
    std::string getValue( uint i ) const;
    int getIntValue( uint i ) const;
    long long getLongLongValue( uint i ) const;
    float getFloatValue( uint i ) const;
    double getDoubleValue( uint i ) const;
    void setValue( uint i, const std::string & s );
    
  };

  class Options {
  private:
    std::vector<Option> m_options; 
  public:
    bool add( const Option & opt );
    Option* get( std::string n );
    const Option* get( std::string n ) const;
    uint nb() const;
    Option* get( uint i );
    const Option* get( uint i ) const;
    void setOptionPresence( std::string n, bool value );
    bool getOptionPresence( std::string n ) const;
  };

  // ------------------------- Standard services ------------------------------
public:
  /**
   * Destructor. Does nothing.
   */
  ~Arguments();

  /**
   * Constructor. Initializes every argument by their default value.
   */
  Arguments();

  /**
   * Adds a boolean non-standard argument.
   * @param name the option name.
   * @param description the option description.
   */
  bool addBooleanOption( const std::string & name,
			 const std::string & description );

  /**
   * Adds a non-standard argument with [nb] parameters.
   * @param name the option name.
   * @param description the option description.
   * @param nb the number of parameters to this option
   * @param default an array storing the default values for the [nb] parameters.
   */
  bool addOption( const std::string & name,
		  const std::string & description,
		  uint nb,
		  const std::vector<std::string> & defaultvals );

  /**
   * Adds a non-standard argument with 1 parameter.
   * @param name the option name.
   * @param description the option description.
   * @param def1 the default value of the first parameter.
   */
  bool addOption( const std::string & name,
		  const std::string & description,
		  const std::string & def1 );

  /**
   * Adds a non-standard argument with 2 parameters.
   * @param name the option name.
   * @param description the option description.
   * @param def1 the default value of the first parameter.
   * @param def2 the default value of the second parameter.
   */
  bool addOption( const std::string & name,
		  const std::string & description,
		  const std::string & def1,
		  const std::string & def2 );

  /**
   * Adds a non-standard argument with 3 parameters.
   * @param name the option name.
   * @param description the option description.
   * @param def1 the default value of the first parameter.
   * @param def2 the default value of the second parameter.
   * @param def3 the default value of the second parameter.
   */
  bool addOption( const std::string & name,
		  const std::string & description,
		  const std::string & def1,
		  const std::string & def2,
		  const std::string & def3 );

  /**
   * Adds a non-standard argument with 4 parameters.
   * @param name the option name.
   * @param description the option description.
   * @param def1 the default value of the first parameter.
   * @param def2 the default value of the second parameter.
   * @param def3 the default value of the third parameter.
   * @param def4 the default value of the fourth parameter.
   */
  bool addOption( const std::string & name,
		  const std::string & description,
		  const std::string & def1,
		  const std::string & def2,
		  const std::string & def3,
		  const std::string & def4 );

  /**
   * Checks if an option was given in the arguments.
   */
  bool check( const std::string & name ) const;

  /**
   * @param name an option name.
   * @return the option of name [name]. May be present or not and may have parameters.
   */
  const Option* getOption( const std::string & name ) const;

  /**
   * Analyses the given arguments and updates the object accordingly.
   * @param argc the number of arguments.
   * @param argv an array of strings storing the arguments.
   * @return 'true' if the arguments were valid, 'false' otherwise.
   */
  bool readArguments( int argc, char* argv[] );

  /**
   * All arguments are reset to their default value.
   */
  void resetToDefault();

  /**
   * @return the possible options.
   */
  Options & getOptions();

  /**
   * @return the possible options (const reference version).
   */
  const Options & getOptions() const;

  /**
   * Builds an usage from the command name and a list of options
   * separated by whitespaces.
   * @param command the name of the command.
   * @param text a description of the command usage.
   * @param options a list of options separated by whitespaces (e.g, "-d -x -y").
   * @return a string listing the descriptions of the options specified in [options].
   */
  std::string usage( const std::string & command, 
		     const std::string & text,
		     const std::string & options );

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
  
  // ------------------------- public Datas -----------------------------------
public:

  /* uint dim; */
  /* uint sizes[ 100 ]; */
  /* float radius; */
  /* float radius_first; */
  /* float radius_last; */
  /* float radius_incr; */
  /* bool digital; */
  /* Shape shape; */
  /* CurvatureEstimator curv_estimator; */

  // ------------------------- Datas ------------------------------------------
private:
  /**
   * List of standard possible options for the arguments.
   */
  Options m_opts;

  // ------------------------- Hidden services --------------------------------
protected:
private:
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden by default.
   */
  INLINE Arguments( const Arguments & other );
  /**
   * Assignment.
   * @param other the object to copy.
   * @return a reference on 'this'.
   * Forbidden by default.
   */
  INLINE Arguments & operator=( const Arguments & other );
  
  // ------------------------- Internals --------------------------------------
private:

  /**
   * Initializes the options.
   */
  void init();
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'Arguments'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'Arguments' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const Arguments & that_object_to_display );

} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "Arguments.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined Arguments_h

#undef Arguments_RECURSES
#endif // else defined(Arguments_RECURSES)
