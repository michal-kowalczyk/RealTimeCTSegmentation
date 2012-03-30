/** @file Proxy.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : Proxy.h
//
// Creation : 2007/02/21
//
// Version : 2007/02/21
//
// Author : JOL
//
// Summary : Header file for files Proxy.ih and Proxy.cxx
//
// History :
//	2007/02/21 : ?Name? : ?What?
//
// Rcs Id : "@(#)class Proxy declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(Proxy_RECURSES)
#error Recursive header files inclusion detected in Proxy.h
#else // defined(Proxy_RECURSES)
#define Proxy_RECURSES

#if !defined Proxy_h
#define Proxy_h

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class Proxy
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'Proxy' <p> Aim: a template class
   * implemented the pattern 'Proxy'. The Proxy is a substitute for
   * another object that has all its characteristics.
   */
  template <typename T>
    class Proxy 
    {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    INLINE ~Proxy();

    /** 
     * Constructs a proxy around dyn. alloc. pointer [ptr_t]. The
     * proxy possesses now this instance and will delete it at his end
     * of life.
     * 
     * @param ptr_t a dyn. alloc. pointer on a 'T'.
     */
    INLINE Proxy( T* ptr_t );

    /** 
     * cast operator.
     * 
     * @return the referenced object.
     */
    INLINE operator T*();

    /** 
     * Dereferencing operator.
     * 
     * @return the referenced object.
     */
    INLINE T& operator*();

    /** 
     * Const dereferencing operator.
     * 
     * @return the referenced object.
     */
    INLINE const T& operator*() const;

    /** 
     * Dereferencing operator.
     * 
     * @return the referenced object.
     */
    INLINE T* operator->();

    /** 
     * Const dereferencing operator.
     * 
     * @return the referenced object.
     */
    INLINE const T* operator->() const;

    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    INLINE Proxy();

  private:
    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE Proxy( const Proxy & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE Proxy & operator=( const Proxy & other );

    // ----------------------- Interface --------------------------------------
  public:

    /**
     * Checks the validity/consistency of the object.
     * @return 'true' if the object is valid, 'false' otherwise.
     */
    INLINE bool OK() const;
  

    // ------------------------- Datas ----------------------------------------
  private:

    /**
     * The object that is hidden by the proxy.
     */
    T* m_t;

 
    // ------------------------- Internals ------------------------------------
  private:
  
  };

} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/base/Proxy.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined Proxy_h

#undef Proxy_RECURSES
#endif // else defined(Proxy_RECURSES)
