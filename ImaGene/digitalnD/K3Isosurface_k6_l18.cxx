///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : K3Isosurface_k6_l18.cxx
//
// Creation : 2002/08/17
//
// Version : 2002/08/17
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
//	2002/08/17 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/digitalnD/K3Isosurface_k6_l18.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/digitalnD/K3Isosurface_k6_l18.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

namespace ImaGene {

const char* const K3Isosurface_k6_l18_RCS_ID = "@(#)class K3Isosurface_k6_l18 definition.";

///////////////////////////////////////////////////////////////////////////////
// class K3Isosurface_k6_l18
///////////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------------*
 * declaration et initialisation de la table permettant -- a partir d'un    *
 * index -- d'acceder a la surface engendree par la configuration traduite  *
 * par l'index. Cette surface est une liste de facettes (au maximum 4). Une *
 * facette est une liste de 3 sommets connus par leur appartenance a une    *
 * arete du cube. Un sommet est un couple de numeros de sommet du cube.	    *
 *--------------------------------------------------------------------------*/
static K3Isosurface::tous_general2 cas_general_k6_l18[256] = 
  {  /* tables des config des cubes */
#include "ImaGene/digitalnD/cas_k6_l18.h" 
  };

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Constructor.
 * Creates an isosurface calculator for 6-18 connectedness couple.
 * @param ks this is the space where the isosurface is created.
 */
K3Isosurface_k6_l18::K3Isosurface_k6_l18( const KnSpace & ks )
  : K3Isosurface( ks, cas_general_k6_l18 )
{
}


///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene
