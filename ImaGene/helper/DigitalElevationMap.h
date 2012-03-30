/** @file DigitalElevationMap.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : DigitalElevationMap.h
//
// Creation : 2010/05/21
//
// Version : 2010/05/21
//
// Author : JOL
//
// Summary : Header file for files DigitalElevationMap.ih and DigitalElevationMap.cxx
//
// History :
//	2010/05/21 : ?Name? : ?What?
//
// Rcs Id : "@(#)class DigitalElevationMap declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(DigitalElevationMap_RECURSES)
#error Recursive header files inclusion detected in DigitalElevationMap.h
#else // defined(DigitalElevationMap_RECURSES)
#define DigitalElevationMap_RECURSES

#if !defined DigitalElevationMap_h
#define DigitalElevationMap_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ImaGene/digitalnD/C4CIteratorOnSurface.h"
#include "ImaGene/digitalnD/DigitalSurfaceTracker.h"
#include "ImaGene/digitalnD/KnCharSet.h"
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/ObjectBoundary.h"
#include "ImaGene/image/Pixel.h"
#include "ImaGene/image/Image2D.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif

namespace ImaGene 
{
  
  /////////////////////////////////////////////////////////////////////////////
  // class DigitalElevationMap
  /////////////////////////////////////////////////////////////////////////////
  /** 
   * Description of class 'DigitalElevationMap' <p> Aim: this class
   * models an elevation map as a 3D digital object. It then offers a
   * series of tools to navigate onto it.
   */
  class DigitalElevationMap
  {

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor. 
     */
    ~DigitalElevationMap();

    /**
     * Constructor. The object is not valid.
     */
    DigitalElevationMap();

    /**
     * Clear object. The object is as it was just instantiated.
     */
    void clear();

    /**
     * Initializes the map from the image [image], a class which must
     * provide the types:
     *
     * - pixel_type: type of the pixel (generally ImaGene::Pixel)
     * - value_type: type of the image value (generally integer)
     *
     * and the methods
     * 
     * - pixel_type lowest() : lowest pixel
     * - pixel_type highest() : first pixel outside (width,height)
     * - value_type at( const pixel_type & p ) : value at pixel p
     *
     * @param image an image defining the elevation map.
     */
    template <typename IMAGE>
    void init( const IMAGE & image );

    /**
     * @return the lowest pixel of the image (generally (0,0)).
     */
    Pixel lowest() const;

    /**
     * @return the highest pixel of the image (generally (width,height)).
     */
    Pixel highest() const;
    

    /**
     * @return the (3D) digital space or 0 if non valid object.
     */
    KnSpace* space() const;

    /**
     * @return the (2D) digital space or 0 if non valid object.
     */
    KnSpace* space2D() const;

    /**
     * @return the volume associated with the elevation map.
     */
    KnCharSet object() const;

    /**
     * @return the pointer on the object boundary (the upper surface)
     * associated with the elevation map.
     */
    const ObjectBoundary* objectBoundary() const;

    /**
     * The elevation as an image between (0,0) and (width-1,height-1).
     */
    const Image2D<int> & elevation() const;

    /**
     * @return the surfel on the volume at the given pixel coordinates.
     */
    Kn_sid topSurfel( Pixel p ) const;

    /**
     * @param p any pixel in the original image frame.
     *
     * @return the coded pixel on the digital plane at the given pixel
     * coordinates.
     */
    Kn_uid upixel( Pixel p ) const;

    /**
     * Projects the given cell in 3D space onto the 2D space.
     * @param cell any signed cell.
     * @return the projected signed cell.
     */
    Kn_sid sproject( Kn_sid cell ) const;

    /**
     * Projects the given cell in 3D space onto the 2D space.
     * @param cell any unsigned cell.
     * @return the projected unsigned cell.
     */
    Kn_uid uproject( Kn_uid cell ) const;

    /**
     * @param p any valid pixel in the image.
     *
     * @return a dyn. alloc. iterator moving horizontally on the
     * digital elevation surface (XZ iterator).
     */
    C4CIteratorOnSurface* createHIterator( Pixel p ) const;

    /**
     * @param p any valid pixel in the image.
     *
     * @return a dyn. alloc. iterator moving vertically on the digital
     * elevation surface (YZ iterator).
     */
    C4CIteratorOnSurface* createVIterator( Pixel p ) const;

    /**
     * @param p any valid pixel in the image.
     * @param i a coordinate index (0 or 1).
     *
     * @return a dyn. alloc. iterator moving vertically on the digital
     * elevation surface (XZ or YZ iterator).
     */
    C4CIteratorOnSurface* createIterator( Pixel p, uint i ) const;

    /**
     * @param p any valid pixel in the image.
     *
     * @return a dyn. alloc. tracker that may move arbitrarily from
     * the initial topSurfel on the elevation surface.
     */
    DigitalSurfaceTracker* createTracker( Pixel p ) const;


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
    
    /**
     * Digital space.
     */
    KnSpace* m_space;

    /**
     * 2D Digital space.
     */
    KnSpace* m_space_2d;

    /**
     * 3D digital object associated with the elevation map. Its
     * boundary is topologically an open digital surface.
     */
    ObjectBoundary* m_object_bdry;

    /**
     * Bel adjacency used when tracking boundary. Any one gives the
     * same result.
     */
    BelAdjacency* m_bel_adj;

    /**
     * Lowest point in the image.
     */
    Pixel m_x0;

    /**
     * Highest point in the image.
     */
    Pixel m_x1;

    /**
     * First valid z-coordinate (lowest value in the image minus 1).
     */
    int m_z_low;

    /**
     * Last valid z-coordinate (highest value in the image plus 1).
     */
    int m_z_high;

    /**
     * Stores the elevation map as an image.
     */
    Image2D<int> m_image;

    // ------------------------- Hidden services ------------------------------
  protected:


  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    INLINE DigitalElevationMap( const DigitalElevationMap & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    INLINE DigitalElevationMap & operator=( const DigitalElevationMap & other );
  
    // ------------------------- Internals ------------------------------------
  private:
  
  };

  /**
   * Overloads 'operator<<' for displaying objects of class 'DigitalElevationMap'.
   * @param that_stream the output stream where the object is written.
   * @param that_object_to_display the object of class 'DigitalElevationMap' to write.
   * @return the output stream after the writing.
   */
  std::ostream&
  operator<<( std::ostream & that_stream, 
	      const DigitalElevationMap & that_object_to_display );

  
} // namespace ImaGene


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/helper/DigitalElevationMap.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined DigitalElevationMap_h

#undef DigitalElevationMap_RECURSES
#endif // else defined(DigitalElevationMap_RECURSES)
