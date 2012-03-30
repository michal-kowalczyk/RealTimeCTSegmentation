/** @file Signal.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : Signal.h
//
// Creation : 2007/03/10
//
// Version : 2007/03/10
//
// Author : JOL
//
// Summary : Header file for files Signal.ih and Signal.cxx
//
// History :
//	2007/03/10 : ?Name? : ?What?
//
// Rcs Id : "@(#)class Signal declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(Signal_RECURSES)
#error Recursive header files inclusion detected in Signal.h
#else // defined(Signal_RECURSES)
#define Signal_RECURSES

#if !defined Signal_h
#define Signal_h

//////////////////////////////////////////////////////////////////////////////
#include "ImaGene/base/BasicTypes.h"
#include "ImaGene/base/Shared.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{

  template <typename T>
    struct SignalData {
      T* data;
      uint size;
      int zero;
      bool periodic;

      /** 
       * Constructor. The object is not valid.
       * @see init.
       */
      INLINE SignalData();

      /** 
       * Initializer.
       *
       * @param s the number of data in the signal.
       * @param z the index of the zero-th element.
       * @param p 'true' if the signal is periodic.
       * @param def the default value.
       */
      INLINE void init( uint s, int z = 0, bool p = false, 
			const T & def = T( 0 ) );
      /**
       * Initializer.
       *
       * @param t the array containing initial data.
       * @param size the size of the signal.
       * @param z the index of the zero-th element.
       * @param p 'true' if the signal is periodic.
       * @param def the default value.
       */
      INLINE void init( const T* t, uint size, int z = 0, bool p = false, 
			const T & def = T( 0 )  );

      /** 
       * Constructor.  
       *
       * @param s the number of data in the signal.
       * @param z the index of the zero-th element.
       * @param p 'true' if the signal is periodic.
       * @param def the default value.
       */
      INLINE SignalData( uint s, int z = 0, bool p = false, 
			 const T & def = T( 0 ) );

      /**
       * Constructor.
       * @param t the array containing initial data.
       * @param size the size of the signal.
       * @param z the index of the zero-th element.
       * @param p 'true' if the signal is periodic.
       * @param def the default value.
       */
      INLINE SignalData( const T* t, uint size, int z = 0, bool p = false, 
			 const T & def = T( 0 )  );

      /** 
       * Destructor.
       */
      INLINE ~SignalData();

      /** 
       * Copy constructor.
       * @param other the data to clone.
       */
      INLINE SignalData( const SignalData & other );

      /** 
       * Assignment.
       * @param other the data to clone.
       * @return 'this'.
       */
      INLINE SignalData& operator=( const SignalData & other );

      /** 
       * Default value.
       * @return the default value.
       */
      const T& defaut() const;

  };
  
  /////////////////////////////////////////////////////////////////////////////
  // class Signal
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'Signal' <p>
   * Aim: Represents a discrete signal, periodic or not.
   */
  template <typename T>
    class Signal
    {

    public:
      /** 
       * @return the binomial signal of order 2.
       */
      static Signal<T> H2();

      /** 
       * @return the right difference signal.
       */
      static Signal<T> Delta();

      /** 
       * @return the binomial signal of order 2n.
       */
      static Signal<T> H2n( uint n );

      /** 
       * @return the differential operator with binomial weights of order 2n.
       */
      static Signal<T> D2n( uint n );

      // ----------------------- Standard services ----------------------------
    public:
      
      /**
       * Destructor. 
       */
      INLINE ~Signal();
      
      /**
       * Constructor.
       * @param size the size of the signal.
       * @param z the index of the zero-th element.
       * @param periodic 'true' if the signal is periodic.
       * @param def the default value.
       */
      INLINE Signal( uint size, int z, bool periodic, 
		     const T & def = T( 0 )  );

      /**
       * Constructor.
       * @param t the array containing initial data.
       * @param size the size of the signal.
       * @param z the index of the zero-th element.
       * @param periodic 'true' if the signal is periodic.
       * @param def the default value.
       */
      INLINE Signal( const T* t, uint size, int z, bool periodic, 
		     const T & def = T( 0 )  );
      
      
      /**
       * Copy constructor.
       * @param other the object to clone.
       */
      INLINE Signal( const Signal & other );
      
      /**
       * Assignment.
       * @param other the object to copy.
       * @return a reference on 'this'.
       * Forbidden by default.
       */
      INLINE Signal & operator=( const Signal & other );



      // ----------------------- Signal services ----------------------------
    public:

      /** 
       * Protected rw access to value. If index is out of bound, return 0
       * if not periodic or the correct value otherwise.
       * 
       * @param i the index in the signal .
       * 
       * @return the i-th value in the signal.
       */      
      INLINE T & operator[]( int i );

      /** 
       * Protected ro access to value. If index is out of bound, return 0
       * if not periodic or the correct value otherwise.
       * 
       * @param i the index in the signal .
       * 
       * @return the i-th value in the signal.
       */      
      INLINE const T & operator[]( int i ) const;


      /** 
       * The signal becomes a constant signal of value [val].
       * 
       * @param val the value of the whole signal.
       */      
      INLINE void setAll( const T & val = T( 0 ) );


      /** 
       * External product of a signal by a scalar value.
       * 
       * @param l the external value.
       * 
       * @return the signal where each term has been multiplied by [l].
       */
      INLINE void multiply( const T & val );

      /** 
       * Convolution product of two signals (F = this).
       * F*G( a ) = sum F(a-i)G(i) 
       * 
       * @param G the second signal (not periodic)
       * 
       * @return the signal that is the convolution of F and G, of type F.
       */
      Signal<T> operator*( const Signal<T>& G );
     
      // ------------------------- Datas --------------------------------------
    private:
      
      /**
       * the array that stores the data.
       * 
       */
      Shared< SignalData<T> > m_data;
      
      // ------------------------- Hidden services ----------------------------
    protected:
      
      /**
       * Constructor.
       * Forbidden by default (protected to avoid g++ warnings).
       */
      INLINE Signal();
      
    private:
      
      // ------------------------- Internals ----------------------------------
    private:
  
  };

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/base/Signal.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined Signal_h

#undef Signal_RECURSES
#endif // else defined(Signal_RECURSES)
