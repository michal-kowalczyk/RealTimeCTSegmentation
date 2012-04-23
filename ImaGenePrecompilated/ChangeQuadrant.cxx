///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : ChangeQuadrant.cxx
//
// Creation : 2009/06/30
//
// Version : 2009/06/30
//
// Author : 
//
// email : 
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
//	2009/06/30 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include <string>
#include "ImaGene/base/Proxy.h"
#include "ImaGene/base/WordUtils.h"
#include "ImaGene/mathutils/Mathutils.h"
#include "ImaGene/dgeometry2d/FreemanChain.h"
#include "ImaGene/helper/ChangeQuadrant.h"
// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/helper/ChangeQuadrant.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const ChangeQuadrant_RCS_ID = "@(#)class ChangeQuadrant definition.";



///////////////////////////////////////////////////////////////////////////////
// class ChangeQuadrant
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::ChangeQuadrant::~ChangeQuadrant()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Transform all the characteristics of DSS from the Global Quadrant
 * to Original Quadrant.
 */
void ImaGene::ChangeQuadrant::
TransformGlobalQuad2OriginalQuad( ImaGene::ChangeQuadrant::segmentMLP & GlobalSegmentMLP,                                ImaGene::ChangeQuadrant::segmentMLP & OriginalSegmentMLP, 
int h, int v )
{
  OriginalSegmentMLP.fc.chain = GlobalSegmentMLP.fc.chain;
  int Quadrant = GlobalSegmentMLP.Quadrant;
  int first_code = GlobalSegmentMLP.first_code;
  OriginalSegmentMLP.Quadrant = Quadrant;
  OriginalSegmentMLP.first_code = first_code;
  int a; int b;
  int mu; int mu_sup;
  a = ( GlobalSegmentMLP.a >= 0 ) ? GlobalSegmentMLP.a : - GlobalSegmentMLP.a;
  b = ( GlobalSegmentMLP.b >= 0 ) ? GlobalSegmentMLP.b : - GlobalSegmentMLP.b;
  int x0 = GlobalSegmentMLP.first_point.x();
  int y0 = GlobalSegmentMLP.first_point.y();
  int rh = h - x0 % h - ( x0 - 1 ) % h - 1;
  int rv = v - y0 % v - ( y0 - 1 ) % v - 1;
  if( Quadrant == 0 )
  { 
    OriginalSegmentMLP.first_point.x() = GlobalSegmentMLP.first_point.x(); 
    OriginalSegmentMLP.first_point.y() = GlobalSegmentMLP.first_point.y();
    OriginalSegmentMLP.last_point.x() = GlobalSegmentMLP.last_point.x(); 
    OriginalSegmentMLP.last_point.y() = GlobalSegmentMLP.last_point.y();
    mu = a * OriginalSegmentMLP.first_point.x() - b * OriginalSegmentMLP.first_point.y(); 
    OriginalSegmentMLP.mu = ( first_code == 1 ) ? mu - a - b + 1 : mu;
  }
  else 
   if( Quadrant == 1 )
   { 
     OriginalSegmentMLP.first_point.x() = - GlobalSegmentMLP.first_point.x() - rh;
     OriginalSegmentMLP.first_point.y() = GlobalSegmentMLP.first_point.y();
     OriginalSegmentMLP.last_point.x() = OriginalSegmentMLP.first_point.x() - 
                                       ( GlobalSegmentMLP.first_point.x() - GlobalSegmentMLP.last_point.x() ); 
     OriginalSegmentMLP.last_point.y() = GlobalSegmentMLP.last_point.y();
     mu = a * OriginalSegmentMLP.first_point.x() + b * OriginalSegmentMLP.first_point.y(); 
     OriginalSegmentMLP.mu = ( first_code == 2 ) ? mu - a - b + 1 : mu;
   }
   else 
    if( Quadrant == 2 )
    {                                                      
      OriginalSegmentMLP.first_point.x() = - GlobalSegmentMLP.first_point.x() - rh;
      OriginalSegmentMLP.first_point.y() = - GlobalSegmentMLP.first_point.y() - rv;
      OriginalSegmentMLP.last_point.x() = OriginalSegmentMLP.first_point.x() - 
                                        ( GlobalSegmentMLP.first_point.x() - GlobalSegmentMLP.last_point.x() ); 
      OriginalSegmentMLP.last_point.y() = OriginalSegmentMLP.first_point.y() - 
                                        ( GlobalSegmentMLP.first_point.y() - GlobalSegmentMLP.last_point.y() ); 
      mu = a * OriginalSegmentMLP.first_point.x() - b * OriginalSegmentMLP.first_point.y(); 
      OriginalSegmentMLP.mu = ( first_code == 2 ) ? mu - a - b + 1 : mu;
    }
    else
     { 
       OriginalSegmentMLP.first_point.x() = GlobalSegmentMLP.first_point.x(); 
       OriginalSegmentMLP.first_point.y() = - GlobalSegmentMLP.first_point.y() - rv; 
       OriginalSegmentMLP.last_point.x() = GlobalSegmentMLP.last_point.x(); 
       OriginalSegmentMLP.last_point.y() = OriginalSegmentMLP.first_point.y() - 
                                       ( GlobalSegmentMLP.first_point.y() - GlobalSegmentMLP.last_point.y() ); 
       mu = a * OriginalSegmentMLP.first_point.x() + b * OriginalSegmentMLP.first_point.y(); 
       OriginalSegmentMLP.mu = ( first_code == 3 ) ? mu - a - b + 1 : mu;
     }
  OriginalSegmentMLP.a = GlobalSegmentMLP.a;
  OriginalSegmentMLP.b = GlobalSegmentMLP.b;
  OriginalSegmentMLP.mu_sup = OriginalSegmentMLP.mu + a + b - 1;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Transform the first point, last point and the FreemanChain of DSS 
 * from the Original Quadrant to Original Quadrant (h,v).
 */
void ImaGene::ChangeQuadrant::TransformFCOriginalQuad2OriginalQuadhv ( ImaGene::ChangeQuadrant::segmentMLP & OriginalSegmentMLP, ImaGene::ChangeQuadrant::segmentMLP & OriginalSegmentMLPhv, int h, int v)
{
  FreemanChain fc;
  FreemanChain Originalfchv;
  fc.chain = OriginalSegmentMLP.fc.chain;
  int quadrant = OriginalSegmentMLP.Quadrant;
  OriginalSegmentMLPhv.Quadrant = quadrant;
  uint nb = fc.chain.size();
  Vector2i fxy( OriginalSegmentMLP.first_point );
  int x; int y;
  x = ( quadrant == 0 || quadrant == 3 ) ? fxy.x() : fxy.x() + 1;
  y = ( quadrant == 0 || quadrant == 1 ) ? fxy.y() : fxy.y() + 1;
  Vector2i fXY( x / h , y / v );
  int xi = ( quadrant == 0 || quadrant == 3 ) ? fXY.x() : fXY.x() - 1;
  int yi = ( quadrant == 0 || quadrant == 1 ) ? fXY.y() : fXY.y() - 1;
  Vector2i I_fXY ( xi , yi );
  OriginalSegmentMLPhv.first_point.x() = I_fXY.x();
  OriginalSegmentMLPhv.first_point.y() = I_fXY.y();
  bool first = true;
  int valx, valy;
  valx = fxy.x();
  valy = fxy.y();
  for ( uint i = 0; i < nb; ++i )
  {
    if( fc.code( i ) == 0 ) valx =  valx + 1;
    else if( fc.code( i ) == 2 ) valx =  valx - 1; 
    else if( fc.code( i ) == 1 ) valy =  valy + 1; 
    else valy = valy - 1;
    Vector2i nxy( valx, valy );
    int xn; int yn;
    xn = ( quadrant == 0 || quadrant == 3 ) ? nxy.x() : nxy.x() + 1;
    yn = ( quadrant == 0 || quadrant == 1 ) ? nxy.y() : nxy.y() + 1;
    Vector2i nXY( xn / h , yn / v );
    int xni; int yni;
    xni = ( quadrant == 0 || quadrant == 3 ) ? nXY.x() : nXY.x() - 1;  
    yni = ( quadrant == 0 || quadrant == 1 ) ? nXY.y() : nXY.y() - 1; 
    Vector2i I_nXY( xni , yni );
    if ( ( I_nXY.x() != I_fXY.x() ) || ( I_nXY.y() != I_fXY.y() ) )
    {
      char code;
      if ( I_nXY.x() > I_fXY.x() )       code = '0';
      else if ( I_nXY.x() < I_fXY.x() )  code = '2';
      else if ( I_nXY.y() > I_fXY.y() )  code = '1';
      else                               code = '3';
      I_fXY = I_nXY;
      Originalfchv.chain += code;
      if( first )
      {
        int first_code;
        if( code == '0' ) first_code = 0;
        else if( code == '1' ) first_code = 1;
        else if( code == '2' ) first_code = 2;
        else first_code = 3; 
        OriginalSegmentMLPhv.first_code = first_code;
        first = false; 
      }
      //break;
    } 
    if( i == nb - 1 )
    {
      OriginalSegmentMLPhv.last_point.x() = I_nXY.x();
      OriginalSegmentMLPhv.last_point.y() = I_nXY.y();
    }   
  }
  //Vector2i lxy( OriginalSegmentMLP.last_point );
  //int xl; int yl;
  //xl = ( quadrant == 0 || quadrant == 3 ) ? lxy.x() : lxy.x() + 1;
  //yl = ( quadrant == 0 || quadrant == 1 ) ? lxy.y() : lxy.y() + 1;
  //Vector2i lXY( xl / h , yl / v );
  //int Xl = ( quadrant == 0 || quadrant == 3 ) ? lXY.x() : lXY.x() - 1;
  //int Yl = ( quadrant == 0 || quadrant == 1 ) ? lXY.y() : lXY.y() - 1;
  //Vector2i I_lXY( Xl , Yl );
  //OriginalSegmentMLPhv.last_point.x() = I_lXY.x();
  //OriginalSegmentMLPhv.last_point.y() = I_lXY.y();
  OriginalSegmentMLPhv.fc.chain = Originalfchv.chain;
}  
///////////////////////////////////////////////////////////////////////////////
/**
 * Transform all the characteristics of DSS from the Original Quadrant
 * to First Quadrant.
 */
void ImaGene::ChangeQuadrant::TransformOriginalQuad2FirstQuad( ImaGene::ChangeQuadrant::segmentMLP & FirstSegmentMLPhv, ImaGene::ChangeQuadrant::segmentMLP & OriginalSegmentMLPhv )
{
  int first_code = OriginalSegmentMLPhv.fc.code(0);
  int fpx = OriginalSegmentMLPhv.first_point.x();
  int fpy = OriginalSegmentMLPhv.first_point.y();
  int lpx = OriginalSegmentMLPhv.last_point.x();
  int lpy = OriginalSegmentMLPhv.last_point.y();
  int a = OriginalSegmentMLPhv.a;
  int b = OriginalSegmentMLPhv.b;
  int mu = OriginalSegmentMLPhv.mu;
  int mu_sup = OriginalSegmentMLPhv.mu_sup;
  int quadrant = OriginalSegmentMLPhv.Quadrant;
  if( quadrant == 0 )
  {
    if( first_code == 0 )
    {
      FirstSegmentMLPhv.first_point.x() = fpx;
      FirstSegmentMLPhv.first_point.y() = fpy;
      FirstSegmentMLPhv.last_point.x() = lpx;
      FirstSegmentMLPhv.last_point.y() = lpy;
      FirstSegmentMLPhv.a = a;
      FirstSegmentMLPhv.b = b;
      FirstSegmentMLPhv.mu = mu;
      FirstSegmentMLPhv.mu_sup = mu_sup;
    }
    else
    {
      FirstSegmentMLPhv.first_point.x() = fpx;
      FirstSegmentMLPhv.first_point.y() = fpy;
      FirstSegmentMLPhv.last_point.x() = FirstSegmentMLPhv.first_point.x() + ( lpy - fpy );
      FirstSegmentMLPhv.last_point.y() = FirstSegmentMLPhv.first_point.y() + ( lpx - fpx );
      FirstSegmentMLPhv.a = b;
      FirstSegmentMLPhv.b = a;
      FirstSegmentMLPhv.mu_sup = - mu + ( FirstSegmentMLPhv.a + FirstSegmentMLPhv.b ) * 
                                 ( FirstSegmentMLPhv.first_point.x() - FirstSegmentMLPhv.first_point.y() );
      FirstSegmentMLPhv.mu = FirstSegmentMLPhv.mu_sup - ( FirstSegmentMLPhv.a + FirstSegmentMLPhv.b ) + 1;
    } 
  }
  else
  {
    if( quadrant == 1 )
    {
      if( first_code == 2 )
      {
        FirstSegmentMLPhv.first_point.x() = - fpx;
        FirstSegmentMLPhv.first_point.y() = fpy;
        FirstSegmentMLPhv.last_point.x() = - lpx;
        FirstSegmentMLPhv.last_point.y() = lpy;
        FirstSegmentMLPhv.a = a;
        FirstSegmentMLPhv.b = - b;
        FirstSegmentMLPhv.mu = - mu_sup;
        FirstSegmentMLPhv.mu_sup = - mu;
      }
      else
      {
        FirstSegmentMLPhv.first_point.x() = - fpx;
        FirstSegmentMLPhv.first_point.y() = fpy;
        FirstSegmentMLPhv.last_point.x() = FirstSegmentMLPhv.first_point.x() + ( lpy - fpy );
        FirstSegmentMLPhv.last_point.y() = FirstSegmentMLPhv.first_point.y() - ( lpx - fpx );
        FirstSegmentMLPhv.a = - b;
        FirstSegmentMLPhv.b = a;
        FirstSegmentMLPhv.mu_sup = mu_sup + ( FirstSegmentMLPhv.a + FirstSegmentMLPhv.b ) * 
                                   ( FirstSegmentMLPhv.first_point.x() - FirstSegmentMLPhv.first_point.y() );
        FirstSegmentMLPhv.mu = FirstSegmentMLPhv.mu_sup - ( FirstSegmentMLPhv.a + FirstSegmentMLPhv.b ) + 1;
      }
    }
    else
    {
      if( quadrant == 2 )
      {
        if( first_code == 2 )
        {
          FirstSegmentMLPhv.first_point.x() = - fpx;
          FirstSegmentMLPhv.first_point.y() = - fpy;
          FirstSegmentMLPhv.last_point.x() = - lpx;
          FirstSegmentMLPhv.last_point.y() = - lpy;
          FirstSegmentMLPhv.a = - a;
          FirstSegmentMLPhv.b = - b;
          FirstSegmentMLPhv.mu = - mu_sup;
          FirstSegmentMLPhv.mu_sup = - mu;
        }
        else
        {
          FirstSegmentMLPhv.first_point.x() = - fpx;
          FirstSegmentMLPhv.first_point.y() = - fpy;
          FirstSegmentMLPhv.last_point.x() = FirstSegmentMLPhv.first_point.x() + ( fpy - lpy );
          FirstSegmentMLPhv.last_point.y() = FirstSegmentMLPhv.first_point.y() + ( fpx - lpx );
          FirstSegmentMLPhv.a = - b;
          FirstSegmentMLPhv.b = - a;
          FirstSegmentMLPhv.mu_sup = mu_sup + ( FirstSegmentMLPhv.a + FirstSegmentMLPhv.b ) * 
                                     ( FirstSegmentMLPhv.first_point.x() - FirstSegmentMLPhv.first_point.y() );
          FirstSegmentMLPhv.mu = FirstSegmentMLPhv.mu_sup - ( FirstSegmentMLPhv.a + FirstSegmentMLPhv.b ) + 1;
        }
      }
      else
      {
        if( first_code == 0 )
        {
          FirstSegmentMLPhv.first_point.x() = fpx;
          FirstSegmentMLPhv.first_point.y() = - fpy;
          FirstSegmentMLPhv.last_point.x() = lpx;
          FirstSegmentMLPhv.last_point.y() = - lpy;
          FirstSegmentMLPhv.a = - a;
          FirstSegmentMLPhv.b = b;
          FirstSegmentMLPhv.mu = mu;
          FirstSegmentMLPhv.mu_sup = mu_sup;
        }
        else
        {   
          FirstSegmentMLPhv.first_point.x() = fpx;
          FirstSegmentMLPhv.first_point.y() = - fpy;
          FirstSegmentMLPhv.last_point.x() = FirstSegmentMLPhv.first_point.x() + ( fpy - lpy );
          FirstSegmentMLPhv.last_point.y() = FirstSegmentMLPhv.first_point.y() + ( lpx - fpx );
          FirstSegmentMLPhv.a = b;
          FirstSegmentMLPhv.b = - a;
          FirstSegmentMLPhv.mu_sup = - mu + ( FirstSegmentMLPhv.a + FirstSegmentMLPhv.b ) * 
                                     ( FirstSegmentMLPhv.first_point.x() - FirstSegmentMLPhv.first_point.y() );
          FirstSegmentMLPhv.mu = FirstSegmentMLPhv.mu_sup - ( FirstSegmentMLPhv.a + FirstSegmentMLPhv.b ) + 1;
        }
      }
    }
  }
}
///////////////////////////////////////////////////////////////////////////////
/**
 * Transform all the characteristics of DSS from the First Quadrant
 * to Original Quadrant.
 */
void ImaGene::ChangeQuadrant::TransformFirstQuad2OriginalQuad( ImaGene::ChangeQuadrant::segmentMLP & FirstDSSMLPhv, ImaGene::ChangeQuadrant::segmentMLP & OriginalDSSMLPhv, ImaGene::ChangeQuadrant::segmentMLP & SecondSegmentMLPhv, ImaGene::ChangeQuadrant::segmentMLP & GlobalSegmentMLP )
{
  int first_code = SecondSegmentMLPhv.first_code;
  int Quadrant = GlobalSegmentMLP.Quadrant;
  int a = FirstDSSMLPhv.a;
  int b = FirstDSSMLPhv.b;
  int mu = FirstDSSMLPhv.mu;
  int muI = FirstDSSMLPhv.mu + a + b - 1;
  int x0 = FirstDSSMLPhv.first_point.x();
  int y0 = FirstDSSMLPhv.first_point.y();
  if( Quadrant == 0 )
  {
    OriginalDSSMLPhv.a = ( first_code == 0 ) ? a : b ;
    OriginalDSSMLPhv.b = ( first_code == 0 ) ? b : a ;
    OriginalDSSMLPhv.mu = ( first_code == 0 ) ? mu : - ( mu + a + b - 1 ) + ( a + b ) * ( x0 - y0 );
  }
  else 
   if( Quadrant == 3 )
   {
     OriginalDSSMLPhv.a = ( first_code == 0 ) ? - a : - b;
     OriginalDSSMLPhv.b = ( first_code == 0 ) ? b : a;
     OriginalDSSMLPhv.mu = ( first_code == 0 ) ? mu : - ( mu + a + b - 1 ) + ( a + b ) * ( x0 - y0 );
   }
   else 
    if( Quadrant == 2 )
    {
      OriginalDSSMLPhv.a = ( first_code == 2 ) ? - a : - b;
      OriginalDSSMLPhv.b = ( first_code == 2 ) ? - b : - a;
      OriginalDSSMLPhv.mu = ( first_code == 2 ) ? - muI : mu - ( a + b ) * ( x0 - y0 );
    }
    else
    {
      OriginalDSSMLPhv.a = ( first_code == 2 ) ? a : b;
      OriginalDSSMLPhv.b = ( first_code == 2 ) ? - b : - a;
      OriginalDSSMLPhv.mu = ( first_code == 2 ) ? - muI : mu - ( a + b ) * ( x0 - y0 );
    }

}
//////////////////////////////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
//void 
//ImaGene::SegmentMLP::selfDisplay( ostream& that_stream ) const
//{
//  that_stream << "[SegmnetMLP]";
//}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
