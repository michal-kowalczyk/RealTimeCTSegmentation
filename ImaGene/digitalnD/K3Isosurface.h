/** @file K3Isosurface.h */
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// File name : K3Isosurface.h
//
// Creation : 2002/08/11
//
// Version : 2002/08/11
//
// Author : JOL
//
// Summary : Header file for files K3Isosurface.ih and K3Isosurface.cxx
//
// History :
//	2002/08/11 : ?Name? : ?What?
//
// Rcs Id : "@(#)class K3Isosurface declaration."
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#if defined(K3Isosurface_RECURSES)
#error Recursive header files inclusion detected in K3Isosurface.h
#else // defined(K3Isosurface_RECURSES)
#define K3Isosurface_RECURSES

#if !defined K3Isosurface_h
#define K3Isosurface_h

//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include "ImaGene/digitalnD/KnTypes.h"
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Forces inline if nothing is provided by the compiler.
#ifndef INLINE
#define INLINE inline
#endif


namespace ImaGene {

  class KnSpace;
  class KnRCellSet;
  class KnCharSet;

///////////////////////////////////////////////////////////////////////////////
// class K3Isosurface
///////////////////////////////////////////////////////////////////////////////
/** 
 * Description of class 'K3Isosurface' <p>
 * Aim: Allows the fast extraction of triangulated isosurfaces in Z3 using
 * a marching-cube like method.
 */
class K3Isosurface
{
  // ------------------------- Local classes ----------------------------------
public:

  /**
   * Structure to store a vertex.
   */
  typedef Kn_sid vertex;

  /**
   * Structure to store a triangle.
   */
  struct triangle
  {
    /**
     * A triangle is delineated by three vertices.
     */
    vertex v[ 3 ];
  };

  /**
   * Structure to store a set of triangles.
   */
  typedef std::vector<triangle> triangle_set;


  // ------------------- configuration table Local classes --------------------
public:

  /**
   * Datatype used in the predefined configuration table.
   * A vertex is two integer number between 0 and 7.
   */
  typedef int sommet[2];
  /**
   * Datatype used in the predefined configuration table.
   * A facette is an array of three vertices (ie a triangle).
   */
  typedef sommet facette[3];
  /**
   * Datatype used in the predefined configuration table.
   * A local surface patch is made of at most six facets.
   */
  typedef facette surfaceinter[6];
  /**
   * Datatype used in the predefined configuration table.
   * A configuration is made of a given number of facets.
   */
  typedef struct general2
  {
    /**
     * The number of facets of this configuration.
     */
    int nb_facettes;
    /**
     * The facets of this configuration.
     */
    surfaceinter tab_facettes;
  } tous_general2;

  // ------------------------- Private Local classes --------------------------
private:

  /**
   * Structure to store the vertices of a configuration.
   */
  struct cfg_vertices 
  {
    /**
     * The number of (iso)vertices of this configuration.
     */
    uint nb_vtx;       
    /**
     * The vertices of this configuration (max is 12).
     */
    vertex vtx[ 12 ];  

    /**
     * Default constructor.
     */
    cfg_vertices() 
      : nb_vtx( 0 ) {}

    /**
     * Add a (iso)vertex to this configuration.
     */
    void add( Kn_sid v ) 
    {
      for ( uint i = 0; i < nb_vtx; ++i )
	if ( vtx[ i ] == v ) return;
      vtx[ nb_vtx++ ] = v;
    }
  };

  /**
   * Structure to store the triangles of a configuration.
   */
  struct cfg_triangles 
  {
    /**
     * The number of triangles for this configuration.
     */
    uint nb_tris;       
    /**
     * The triangles of this configuration (max is 6).
     */
    triangle tris[ 6 ]; 

    /**
     * Default constructor.
     */
    cfg_triangles() 
      : nb_tris( 0 ) {}

    /**
     * Add a triangle (facet) to this configuration.
     */
    void add( const triangle & t ) 
    {
      tris[ nb_tris ].v[ 0 ] = t.v[ 0 ];
      tris[ nb_tris ].v[ 1 ] = t.v[ 1 ];
      tris[ nb_tris ].v[ 2 ] = t.v[ 2 ];
      nb_tris++;
    }
  };
  

  // ------------------------- Standard services ------------------------------
public:

  /**
   * Destructor. 
   */
  ~K3Isosurface();

  /**
   * Constructor.
   * @param ks this is the space where the isosurface is created.
   * @param lut this is the look-up table giving the triangles for each configuration (0 use default).
   */
  K3Isosurface( const KnSpace & ks, const tous_general2* lut = 0 );

  // ------------------------- Isosurface services ----------------------------
public:

  /**
   * Builds and returns the isosurface between "on" spels and "off" spels by
   * marching over a given subspace.
   * @param spels a set of unoriented spels.
   * @param vertices (returns) the vertices of the isosurface given as a set of oriented surfels.
   * @param all_tris (returns) the set of all the triangles of the isosurface.
   * @param lower_bound the lower bound of the subspace of the image in which the isosurface is computed.
   * @param upper_bound the upper bound of the subspace of the image in which the isosurface is computed.
   */
  void extract( const KnCharSet & spels,
		KnRCellSet & vertices,
		triangle_set & all_tris,
		Kn_uid lower_bound,
		Kn_uid upper_bound ) const;
  
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


  // ------------------------- Datas ------------------------------------------
private:

  /**
   * The space where the 'K3Isosurface" works.
   */
  const KnSpace & m_ks;

  /**
   * Stores the vertices of the 256 possible configurations.
   */
  cfg_vertices m_cfg_vtx[ 256 ];

  /**
   * Stores the triangles of the 256 possible configurations.
   */
  cfg_triangles m_cfg_tris[ 256 ];

  
  // ------------------------- Hidden services --------------------------------
private:
  /**
   * Constructor.
   */
  INLINE K3Isosurface();
  /**
   * Copy constructor.
   * @param other the object to clone.
   * Forbidden.
   */
  INLINE K3Isosurface( const K3Isosurface & other );
  /**
   * Assignment.
   * @param other the object to clone.
   * @return a reference on 'this'.
   * Forbidden.
   */
  INLINE K3Isosurface & operator=( const K3Isosurface & other );
  
  // ------------------------- Internals --------------------------------------
private:

  /**
   * Initializes the configurations tables [cfg_vertices] and [cfg_triangles] 
   * from the big look-up table [lut].
   * @param lut a look-up table containing for each configuration the vertices and triangles. They are typicalled defined in the files cas.h etc.
   * @param c_vtcs (returns) fills this array of array of vertices.
   * @param c_tris (returns) fills this array of array of triangles.
   */
  void init( const tous_general2* lut, 
	     cfg_vertices* c_vtcs, cfg_triangles* c_tris );
  /**
   * Internal method to convert an isovertex defined as a pair [v1],[v2] in
   * a configuration [cfg] into a signed cellcode.
   * @param cfg any configuration (0-255)
   * @param v1 the first vertex (0-7)
   * @param v2 the second vertex (0-7)
   * @return the corresponding signed cellcode (in the first 8-cube of the space).
   */
  Kn_sid codeIsoVertexAsSurfel( uint cfg, uint v1, uint v2 ) const;
  
};

/**
 * Overloads 'operator<<' for displaying objects of class 'K3Isosurface'.
 * @param that_stream the output stream where the object is written.
 * @param that_object_to_display the object of class 'K3Isosurface' to write.
 * @return the output stream after the writing.
 */
INLINE std::ostream&
operator<<( std::ostream & that_stream, const K3Isosurface & that_object_to_display );


} // namespace ImaGene

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "ImaGene/digitalnD/K3Isosurface.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined K3Isosurface_h

#undef K3Isosurface_RECURSES
#endif // else defined(K3Isosurface_RECURSES)
