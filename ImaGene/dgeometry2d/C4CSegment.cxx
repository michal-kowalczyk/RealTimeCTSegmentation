///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : C4CSegment.cxx
//
// Creation : 2003/11/05
//
// Version : 2003/11/05
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
//	2003/11/05 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/dgeometry2d/C4CSegment.h"

#include "ImaGene/base/Vector2i.h"


// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "C4CSegment.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;


namespace ImaGene {

const char* const C4CSegment_RCS_ID = "@(#)class C4CSegment definition.";

///////////////////////////////////////////////////////////////////////////////
// class C4CSegment
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Copy constructor.
 * @param other the object to clone.
 * Forbidden by default.
 */
C4CSegment::C4CSegment( const C4CSegment & other )
  : m_a( other.m_a ), m_b( other.m_b ), m_mu( other.m_mu ),
    m_up( other.m_up ), m_lp( other.m_lp ), m_u( other.m_u ), m_l( other.m_l ),
    m_cp_n( other.m_cp_n ), m_c_n( other.m_c_n ),
    m_state_cp_n( other.m_state_cp_n ), m_state_c_n( other.m_state_c_n )
{
}

/**
 * Assignment.
 * @param other the object to copy.
 * @return a reference on 'this'.
 * Forbidden by default.
 */
C4CSegment & 
C4CSegment::operator=( const C4CSegment & other )
{
  if ( this != &other )
    {
      m_a = other.m_a;
      m_b = other.m_b;
      m_mu = other.m_mu;
      m_up = other.m_up;
      m_lp = other.m_lp;
      m_u = other.m_u;
      m_l = other.m_l;
      m_cp_n = other.m_cp_n;
      m_c_n = other.m_c_n;
      m_state_cp_n = other.m_state_cp_n;
      m_state_c_n = other.m_state_c_n;
    }
  return *this;
}



/**
 * (Re-)Initializes the line segment as an horizontal edge with
 * first point C_n in (0,0) and second point in (1,0). The
 * "interior" of the contour is to the right (clockwise) or
 * negatively along y axis.
 */
void 
C4CSegment::init()
{
  // initial digital line coefficients.
  m_a = 0;
  m_b = 1;
  m_mu = 0;
  m_up = Vector2i( 1, 0 );// U'
  m_lp = m_up;             // L'
  m_u = Vector2i( 0, 0 ); // U
  m_l = m_u;               // L
  m_cp_n = m_up;           // C'_n
  m_c_n = m_u;             // C_n
  m_state_cp_n = 0;        // 0 : tangent, 4: turned_int, 8: turned_ext
  m_state_c_n = 0;         // 0 : tangent, 4: turned_int, 8: turned_ext
}




///////////////////////////////////////////////////////////////////////////////
// ------------------------- Segment services ------------------------------
/**
 * Tries to transform the discrete line segment in a thiner one but included
 *
 * @param code 0: slim it from upper leaning points, 
 * 1: slim it from lower leaning points
 *
 * @return '1' a thiner segment is computed, '0' if the initial segment is already the thinest
 */
// uint
// C4CSegment::slim( uint code )
// {
//   //check if it is possible to slim the segment with respect to code
//   Vector2i dir = getTangent();
//   cout<<"vecteur directeur : "<<dir<<endl;
//   bool noSlimUp = false;
//   bool noSlimLow = false;
//   int det;
//   
//   if(code==0)
//   {
//     det = dir.x()*m_u.y() - dir.y()*m_u.x();
//     cout<<"determinant de dir et IU : "<<det<<endl;  
//     if(det == 0)
//     {
//       cout<<"impossible d'affiner par le haut"<<endl;
//       noSlimUp = true;
//     }
//     else
//     {
// //       Vector2D dirF = Vector2D(dir.x(),dir.y());
// //       Vector2D bez = VectorUtils::bezoutWithConstraints( dirF, -1,  u(), cp_n().x() );
//       Vector2D dirF = Vector2D(dir.x(),dir.y()); // vecteur directeur float
//       Vector2D bez = VectorUtils::closestBezout( dirF, -1);  //Bezout du vecteur directeur
//      /* while(bez.x()<0) 
//       {
//         cout<<"abscisse Bezout negative, recalcul..."<<endl; 
// 	bez += dirF;
//       }*/
//       int nbUpperLeaningPoints = nbU();
//       cout<<"nb de pts d'appui sup : "<<nbUpperLeaningPoints<<endl;
//       cout<<"le bezout = "<<bez.x()<<" "<<bez.y()<<endl;
// //       Vector2i newP = m_u+bez;
//       Vector2i newP = Vector2i(m_u.x()+bez.x(),m_u.y()+bez.y());
//       if(nbUpperLeaningPoints==1) //un seul pt d'appui sup
//         if(m_u.x()+bez.x()>=m_c_n.x() && m_u.x()+bez.x()<=m_cp_n.x()) //U+bez appartient au segment
// 	{
// 	  //mise a jour de a et b
//           m_a = newP.y()-m_u.y();
// 	  m_b = newP.x()-m_u.x();
// 	  //ajout nouveau pt d'appui sup
//           int fl = floor(m_cp_n.x()-newP.x(),bez.x()).get_si();
// // 	  m_up = newP + product(bez,fl);
//           m_up = newP;
// 	  m_up += Vector2i(bez.x()*fl,bez.y()*fl);
// 	  //mise à jour pt extrémité pas necessaire ds ce cas
// // 	  m_cp_n = newP;
// 	  //mise a jour pts d'appui inf
// 	  Vector2D bezNeg = Vector2D(-bez.x(),-bez.y());
// 	  fl = floor(m_c_n.x()-m_lp.x(),bezNeg.x()).get_si();
// 	  if(m_c_n==m_l) //si l'ancien pt d'appui L correspondait à un pt extremité
// 	  {  
// 	    m_c_n = m_lp;
// 	    m_c_n += Vector2i(bezNeg.x()*fl,bezNeg.y()*fl);
// 	  }
// 	  m_l = m_lp;
// 	  m_l += Vector2i(bezNeg.x()*fl,bezNeg.y()*fl);
// 	}
//     }
//   }
//   else
//   {
//     det = dir.x()*l().y() - dir.y()*l().x();
//     cout<<"determinant de dir et IL : "<<det<<endl; 
//     if(det == 0)
//     {
//       cout<<"impossible d'affiner par le bas"<<endl;
//       noSlimLow = true;
//     }
//     else
//     {
// //       Vector2D dirF = Vector2D(dir.x(),dir.y());
// //       Vector2D bez = VectorUtils::bezoutWithConstraints( dirF, 1,  l(), cp_n().x() );
//       Vector2D dirF = Vector2D(dir.x(),dir.y());
//       Vector2D bez = VectorUtils::closestBezout( dirF, 1);
//       cout<<"le bezout est = "<<bez.x()<<" "<<bez.y()<<endl;
//     }
//   }
// 
//   return 1;
// }

/**
 * Tries to transform the discrete line segment in a thiner one but included
 *
 * @param codeUL 0: slim it from upper leaning points, 
 * 1: slim it from lower leaning points
 * @param dir : the direction vector to consider 
 * @return '1' a thiner segment is computed, '0' the origin lies on the selected border and the segment is not thiner, '2' the computed Bezout vector is not usable
 */
// uint
// C4CSegment::slim( uint codeUL, Vector2i & dir )
// {
//   //check if it is possible to slim the segment with respect to code
//   //Vector2i dir = getTangent();
//   cout<<"vecteur directeur : "<<dir<<endl;
//   int det;
//   uint codePN;
//   
//   // codePN 0: consider director vector in the negative sense, 
//   // 1: in the positive sense
//   if(codeUL==0 && m_u.x()<=0)
//     codePN = 1;
//   else
//     codePN = 0;
//   if(codeUL==1 && m_l.x()<=0)
//     codePN = 1;
//   else
//     codePN = 0;
//   
//   if(codeUL==0) //slim upper border line
//   {
//     det = dir.x()*m_u.y() - dir.y()*m_u.x();
//     cout<<"determinant de dir et IU : "<<det<<endl;  
//     if(det == 0)
//     {
//       cout<<"impossible d'affiner par le haut, origine sur le bord sup!"<<endl;
//       return 0;
//     }
//     else
//     {
//       Vector2D dirF = Vector2D(dir.x(),dir.y()); // vecteur directeur float
//       Vector2D bez = VectorUtils::closestBezout( dirF, -1);  //Bezout du vecteur directeur
//       int nbUpperLeaningPoints = nbU();
//       cout<<"nb de pts d'appui sup : "<<nbUpperLeaningPoints<<endl;
//       cout<<"le bezout = "<<bez.x()<<" "<<bez.y()<<endl;
// 
//       Vector2i newP = Vector2i(m_u.x()+bez.x(),m_u.y()+bez.y());
//       Vector2i newPP = Vector2i(newP.x()-1,newP.y());
//       if(nbUpperLeaningPoints==1) //un seul pt d'appui sup
//       {
//         if( (m_u.x()+bez.x()>m_c_n.x() && m_u.x()+bez.x()<m_cp_n.x()) || (m_a >= 0 && newP.x()==m_cp_n.x() && dir.x()*(newPP.y()-m_u.y()) - dir.y()*(newPP.x()-m_u.x())<=0) || (m_a < 0 && newP.x()==m_cp_n.x()) ) //U+bez appartient bien au segment
// 	{
// 	  //mise a jour de a et b
//           m_a = newP.y()-m_u.y();
// 	  m_b = newP.x()-m_u.x();
// 	  //ajout nouveau pt d'appui sup
//           int fl = floor(m_cp_n.x()-newP.x(),bez.x()).get_si();
// // 	  m_up = newP + product(bez,fl);
//           m_up = newP;
// 	  m_up += Vector2i(bez.x()*fl,bez.y()*fl);
// 	  //mise à jour pt extrémité pas necessaire ds ce cas
// // 	  m_cp_n = newP;
// 	  //mise a jour pts d'appui inf
// 	  Vector2D bezNeg = Vector2D(-bez.x(),-bez.y());
// 	 // fl = floor(m_c_n.x()-m_lp.x(),bezNeg.x()).get_si();
// 	  if(m_c_n==m_l) //si l'ancien pt d'appui L correspondait à un pt extremité
// 	  {  
// 	    m_c_n = m_lp;
// // 	    m_c_n += Vector2i(bezNeg.x()*fl,bezNeg.y()*fl);
//             m_c_n += Vector2i(bezNeg.x(),bezNeg.y());
// 	  }
// 	  m_l = m_lp;
// // 	  m_l += Vector2i(bezNeg.x()*fl,bezNeg.y()*fl);
// 	  m_l += Vector2i(bezNeg.x(),bezNeg.y());
// 	  return 1;
// 	}
// 	dir = Vector2i(bez.x(), bez.y());
// 	return 2;
//       } //if(nbUpperLeaningPoints==1)
//       else //2 upper leaning points
//       {
//         while( (newP.x()-m_u.x())*(-m_u.y())-(newP.y()-m_u.y())*(-m_u.x()) > 0 )
// 	{
// 	  m_u += getTangent();
// 	  newP = Vector2i(m_u.x()+bez.x(),m_u.y()+bez.y());
// 	}
// 	
//       }
//     }
//   } // if(codeUL==0)
// //   else
// //   {
// //     det = dir.x()*l().y() - dir.y()*l().x();
// //     cout<<"determinant de dir et IL : "<<det<<endl; 
// //     if(det == 0)
// //     {
// //       cout<<"impossible d'affiner par le bas"<<endl;
// //       noSlimLow = true;
// //     }
// //     else
// //     {
// //       Vector2D dirF = Vector2D(dir.x(),dir.y());
// //       Vector2D bez = VectorUtils::closestBezout( dirF, 1);
// //       cout<<"le bezout est = "<<bez.x()<<" "<<bez.y()<<endl;
// //     }
// //   }
// 
//   return 1;
// }

// bool
// C4CSegment::belongToSegment(Vector2i & P )
// {
//   //cout<<"fonction belongToSegment avec P = "<<P.x()<<" "<<P.y()<<endl;
//   Vector2i dir = getTangent();
//   Vector2i UP = P;
//   UP -= m_u;
//   Vector2i LP = P;
//   LP -= m_l;
//   if(P.x()<m_c_n.x() || P.x()>m_cp_n.x())
//     return false;
//   if(P.x()>m_c_n.x() && P.x()<m_cp_n.x() && dir.x()*UP.y()-dir.y()*UP.x() <= 0 && dir.x()*LP.y()-dir.y()*LP.x() >= 0)
//     return true;
//   if(P == m_c_n || P == m_cp_n)
//     return true;
//     
//    if(P.x()==m_c_n.x() && dir.x()*UP.y()-dir.y()*UP.x() <= 0 && dir.x()*LP.y()-dir.y()*LP.x() >= 0) //P meme absc que Cn entre les 2 bords
//   {
//       if(m_a>=0) //pente positive
//         if(P.y()<m_c_n.y())
// 	  return false;
// 	else
// 	  return true;
//       else //pente negative
//         if(P.y()>m_c_n.y())
// 	  return false;
// 	else
// 	  return true;
//   }
//    if(P.x()==m_cp_n.x() && dir.x()*UP.y()-dir.y()*UP.x() <= 0 && dir.x()*LP.y()-dir.y()*LP.x() >= 0) //P meme absc que C'n entre les 2 bords
//   {
//       if(m_a>=0) //pente positive
//         if(P.y()>m_cp_n.y())
// 	  return false;
// 	else
// 	  return true;
//       else //pente negative
//         if(P.y()<m_cp_n.y())
// 	  return false;
// 	else
// 	  return true;
//   }
//   return false;
// }

/**
 * Tries to transform the discrete line segment in a thiner one but included
 * @param dir : the direction vector to consider 
 * @return '1' a thiner segment is computed, '0' the origin lies on the selected border and the segment is not thiner, '2' the computed Bezout vector is not usable
 */
// uint
// C4CSegment::slimRight(Vector2i & dir )
// {
//   cout<<"slimRight vecteur direction à considérer : "<<dir<<endl;
//   uint codePN;
//   int fl;
//   
//   int nbUpperLeaningPoints = nbU();
//   int nbLowerLeaningPoints = nbL();
//   cout<<"nb de pts d'appui sup : "<<nbUpperLeaningPoints<<endl;
//   cout<<"nb de pts d'appui inf : "<<nbLowerLeaningPoints<<endl;
//   
//   int det = dir.x()*m_u.y() - dir.y()*m_u.x();
//   cout<<"determinant de dir et IU : "<<det<<endl;
//   if(det == 0)
//     cout<<"origine sur le bord sup!"<<endl;
//     
//   Vector2D dirF = Vector2D(dir.x(),dir.y()); // vecteur direction
//   Vector2D bez = VectorUtils::closestBezout( dirF, -1);  //Bezout du vecteur direction
//   cout<<"le bezout = "<<bez.x()<<" "<<bez.y()<<endl;
//     
//   //test pour savoir si le segment est modifiable dans ce sens (right)
//   /*if(nbUpperLeaningPoints==1) //one upper leaning point
//   {
//     Vector2i v = Vector2i(m_u.x()+bez.x(),m_u.y()+bez.y());
//     if( !belongToSegment(v) ) //U+bez n'appartient pas au segment
//     {
//       Vector2i P = m_lp;
//       if( abs((float(m_a) / (float)m_b)) <= 1)
//       {
//           while( P.x()>=0 )
// 	  {
// 	    P -= getTangent();
// 	  }
// 	  P += getTangent();
//       }
//       else
//       {
//           while( P.y()>=0 )
// 	  {
// 	    P -= getTangent();
// 	  }
// 	  P += getTangent();
//       }
//       Vector2i w = Vector2i(P.x()-bez.x(),P.y()-bez.y());
//       if( !belongToSegment(w) )
//       {
//         dir = Vector2i(bez.x(), bez.y());
// 	cout<<"segment non modifiable dans le sens right"<<endl;
//         return 2;
//       }
//     }
//   } //if(nbUpperLeaningPoints==1)
//   else
//   {
//     
//   }*/
//   
// 
// 
//       if(nbUpperLeaningPoints==1) //one upper leaning point
//       {
//         cout<<"un seul point d'appui supérieur"<<endl;
//         Vector2i v = Vector2i(m_u.x()+bez.x(),m_u.y()+bez.y());
//         if( belongToSegment(v) ) //U+bez appartient bien au segment
// 	{
// 	  //ajout nouveau pt d'appui sup
//           fl = floor(m_cp_n.x()-m_u.x(),bez.x()).get_si();
// 	  cout<<"fl 1 vaut "<<fl<<endl;
//           //m_up = m_u;
// 	  m_up += Vector2i(bez.x()*fl,bez.y()*fl);
// 	  if( !belongToSegment(m_up) )
// 	  {
// 	    m_up -= Vector2i(bez.x(),bez.y());
// 	    cout<<"moins bez"<<endl;
// 	  }	  
//           //mise a jour premier pt extremité Cn
// 	  m_c_n = m_l;
// 	  if(m_a>=0)
//  	    m_c_n += Vector2i(0,1);
// 	  else
// 	    m_c_n += Vector2i(-1,0);
// 	  //mise a jour pts d'appui inf
// 	  fl = ceil(m_l.x()-m_lp.x(),bez.x()).get_si();  //fl is always negative
// 	  cout<<"ceil 2 vaut "<<fl<<endl;
// 	  m_l = m_lp;
//  	  m_l += Vector2i(bez.x()*fl,bez.y()*fl);
// 	  if( !belongToSegment(m_l) )
// 	    m_l += Vector2i(bez.x(),bez.y());
// 	  //mise a jour de a, b et mu
//           m_a = bez.y();
// 	  m_b = bez.x();
// 	  m_mu = m_a*m_u.x() - m_b*m_u.y();
// 	  return 1;
// 	}
// 	dir = Vector2i(bez.x(), bez.y());
// 	cout<<"bezout calculé inexploitable 1"<<endl;
// 	return 2;
//       } //if(nbUpperLeaningPoints==1)
//       else //at least 2 upper leaning points
//       {
//         //compute the upper leaning point located at the left of I (the nearest)
//         Vector2i P = m_u;
// 	if( abs((float(m_a) / (float)m_b)) <= 1)
// 	{
//           while( P.x()<=0 )
// 	  {
// 	    P += getTangent();
// 	  }
// 	  P -= getTangent();
// 	}
// 	else
// 	{
//           while( P.y()<=0 )
// 	  {
// 	    P += getTangent();
// 	  }
// 	  P -= getTangent();
// 	}
//         Vector2i v = Vector2i(P.x()+bez.x(),P.y()+bez.y());
// 	if( belongToSegment(v) ) //P+bez appartient bien au segment
// 	{
// 	  if(P==m_up) fl = floor(m_cp_n.x()-P.x(),bez.x()).get_si();
// 	  else fl = floor(P.x()+dir.x()-P.x(),bez.x()).get_si();
// 	  m_u = P;
// 	  m_up = Vector2i(m_u.x()+bez.x()*fl,m_u.y()+bez.y()*fl);
// 	  if( !belongToSegment(m_up) )
// 	    m_up -= Vector2i(bez.x(),bez.y());
// 	  if(P!=m_up) 
// 	  {
// 	    //mise a jour premier pt extremité Cn'
// 	    m_cp_n = P;
// 	    m_cp_n += dir;
// 	    if(m_a>=0)
//  	      m_cp_n += Vector2i(0,-1);
// 	    else
// 	      m_cp_n += Vector2i(-1,0);
// 	  }
// 	  if(nbLowerLeaningPoints==1) //one lower leaning point
// 	  {
// 	    fl = ceil(m_c_n.x()-m_lp.x(),bez.x()).get_si();
// 	    //m_l = m_lp;
//    	    m_l += Vector2i(bez.x()*fl,bez.y()*fl);
// 	    if( !belongToSegment(m_l) )
// 	      m_l += Vector2i(bez.x(),bez.y());
// 	  } //if(nbLowerLeaningPoints==1)
// 	  else //at least two lower leaning points
// 	  {
// 	     if( abs((float(m_a) / (float)m_b)) <= 1)
// 	     {
// 	       while( m_lp.x()>=0 )
// 	       {
// 	         m_lp -= getTangent();
// 	       }
// 	       m_lp += getTangent();
// 	     }
// 	     else
// 	     {
// 	       while( m_lp.y()>=0 )
// 	       {
// 	         m_lp -= getTangent();
// 	       }
// 	       m_lp += getTangent();
// 	     }
// 	     if(m_lp==m_l) fl = ceil(m_c_n.x()-m_lp.x(),bez.x()).get_si();
// 	     else fl = ceil(m_lp.x()-dir.x()-m_lp.x(),bez.x()).get_si();
// 	     Vector2i v = Vector2i(m_lp.x()-bez.x(),m_lp.y()-bez.y());
// 	     if(belongToSegment(v)) //L'-bez appartient au segment
// 	     {
// 	       m_l = m_lp;
// 	       m_l += Vector2i(bez.x()*fl,bez.y()*fl);
// 	       if( !belongToSegment(m_l) )
// 	         m_l += Vector2i(bez.x(),bez.y());
// 	       if(m_lp!=m_l)
// 	       {
// 	         m_c_n = m_lp;
// 		 m_c_n -= getTangent();
// 		 if(m_a>=0)
//    	           m_c_n += Vector2i(0,1);
// 		 else
// 		   m_c_n += Vector2i(1,0);
// 	       }
// 	     }
// 	  }
// 	  //mise a jour de a et b
//           m_a = bez.y();
// 	  m_b = bez.x();
// 	  m_mu = m_a*m_u.x() - m_b*m_u.y();
// 	} //if(P.x()+bez.x()<=m_cp_n.x())
// 	else
// 	{
// 	  dir = Vector2i(bez.x(), bez.y());
// 	  cout<<"bezout calculé inexploitable 2"<<endl;
// 	  return 2;
// 	}
//       }
//       
//   return 1;
// }
// 
// uint
// C4CSegment::slimLeft(Vector2i & dir )
// {
//   cout<<"slimLeft vecteur direction à considérer : "<<dir<<endl;
//   uint codePN;
//   int fl;
//   
//   int nbUpperLeaningPoints = nbU();
//   int nbLowerLeaningPoints = nbL();
//   cout<<"nb de pts d'appui sup : "<<nbUpperLeaningPoints<<endl;
//   cout<<"nb de pts d'appui inf : "<<nbLowerLeaningPoints<<endl;
//   
//   int det = dir.x()*m_l.y() - dir.y()*m_l.x();
//   cout<<"determinant de dir et IL : "<<det<<endl;
//   if(det == 0)
//   {
//     cout<<"origine sur le bord inf!"<<endl;
//     //return 0;
//   }
//   
//   // codePN 0: consider director vector in the negative sense, 
//   // 1: in the positive sense
// //   if(m_l.x()<=0)
// //     {codePN = 1; cout<<"sens positif"<<endl;}
// //   else
// //     {codePN = 0; cout<<"sens negatif"<<endl;}
// //   
// //   if(codePN == 1) //try to slim lower border line in positive sense
// //   {
//     Vector2D dirF = Vector2D(dir.x(),dir.y()); // vecteur direction
//     Vector2D bez = VectorUtils::closestBezout( dirF, 1);  //Bezout du vecteur direction
//     cout<<"le bezout = "<<bez.x()<<" "<<bez.y()<<endl;
// 
//       if(nbLowerLeaningPoints==1) //one lower leaning point
//       {
//         cout<<"un seul point d'appui inferieur"<<endl;
//         Vector2i v = Vector2i(m_l.x()+bez.x(),m_l.y()+bez.y());
//         if( belongToSegment(v) ) //U+bez appartient bien au segment
// 	{
// 	  //ajout nouveau pt d'appui inf
//           fl = floor(m_cp_n.x()-m_l.x(),bez.x()).get_si();
// 	  cout<<"fl 1 vaut "<<fl<<endl;
//          // m_lp = m_l;
// 	  m_lp += Vector2i(bez.x()*fl,bez.y()*fl);
// 	  if( !belongToSegment(m_lp) )
// 	  {
// 	    m_lp -= Vector2i(bez.x(),bez.y());
// 	    cout<<"moins bez"<<endl;
// 	  }
// 	  //mise a jour de a et b
//           m_a = bez.y();
// 	  m_b = bez.x();
// 	  m_mu = m_a*m_u.x() - m_b*m_u.y();
//           //mise a jour premier pt extremité Cn
// 	  m_c_n = m_u;
// 	  if(m_a>=0)
//  	    m_c_n += Vector2i(1,0);
// 	  else
// 	    m_c_n += Vector2i(0,-1);
// 	  //mise a jour pts d'appui sup
// 	  fl = ceil(m_u.x()-m_up.x(),bez.x()).get_si();  //fl is always negative
// 	  cout<<"ceil 2 vaut "<<fl<<endl;
// 	  m_u = m_up;
//  	  m_u += Vector2i(bez.x()*fl,bez.y()*fl);
// 	  if( !belongToSegment(m_u) )
// 	    m_u += Vector2i(bez.x(),bez.y());
// 	  return 1;
// 	}
// 	dir = Vector2i(bez.x(), bez.y());
// 	cout<<"bezout calculé inexploitable 3"<<endl;
// 	return 2;
//       } //if(nbUpperLeaningPoints==1)
//       else //at least 2 lower leaning points
//       {
//         //compute the lower leaning point located at the left of I (the nearest)
//         Vector2i P = m_l;
// 	if( abs((float(m_a) / (float)m_b)) <= 1)
// 	{
// 	  cout<<"pente <=1"<<endl;
//           while( P.x()<=0 )
// 	  {
// 	    P += getTangent();
// 	  }
// 	  P -= getTangent();
// 	}
// 	else
// 	{
// 	  cout<<"pente >1"<<endl;
//           while( P.y()<=0 )
// 	  {
// 	    P += getTangent();
// 	  }
// 	  P -= getTangent();
// 	}
//         Vector2i v = Vector2i(P.x()+bez.x(),P.y()+bez.y());
// 	if( belongToSegment(v) ) //P+bez appartient bien au segment
// 	{
// 	  if(P==m_lp) fl = floor(m_cp_n.x()-P.x(),bez.x()).get_si();
// 	  else fl = floor(P.x()+dir.x()-P.x(),bez.x()).get_si();
// 	  m_l = P;
// 	  m_lp = Vector2i(m_l.x()+bez.x()*fl,m_l.y()+bez.y()*fl);
// 	  if( !belongToSegment(m_lp) )
// 	    m_lp -= Vector2i(bez.x(),bez.y());
// 	  if(P!=m_lp) 
// 	  {
// 	    //mise a jour premier pt extremité Cn'
// // 	    m_cp_n = m_lp;
// //  	    m_cp_n += Vector2i(-1,0);
//             m_cp_n = m_l;
// 	    m_cp_n += getTangent();
// 	    if(m_a>=0)
//  	      m_cp_n += Vector2i(-1,0);
// 	    else
// 	      m_cp_n += Vector2i(0,1);
// 	  }
// 	  if(nbUpperLeaningPoints==1) //one upper leaning point
// 	  {
// 	    fl = ceil(m_c_n.x()-m_up.x(),bez.x()).get_si();
// 	    //m_l = m_lp;
//    	    m_u += Vector2i(bez.x()*fl,bez.y()*fl);
// 	    if( !belongToSegment(m_u) )
// 	      m_u += Vector2i(bez.x(),bez.y());
// 	  } //if(nbUpperLeaningPoints==1)
// 	  else //at least two upper leaning points
// 	  {
// 	     if( abs((float(m_a) / (float)m_b)) <= 1)
// 	     {
// 	       while( m_up.x()>=0 )
// 	       {
// 	         m_up -= getTangent();
// 	       }
// 	       m_up += getTangent();
// 	     }
// 	     else
// 	     {
// 	       while( m_up.y()>=0 )
// 	       {
// 	         m_up -= getTangent();
// 	       }
// 	       m_up += getTangent();
// 	     }
// 	     if(m_up==m_u) fl = ceil(m_c_n.x()-m_up.x(),bez.x()).get_si();
// 	     else fl = ceil(m_up.x()-dir.x()-m_up.x(),bez.x()).get_si();
// 	     Vector2i v = Vector2i(m_up.x()-bez.x(),m_up.y()-bez.y());
// 	     if(belongToSegment(v)) //U'-bez appartient au segment
// 	     {
// 	       m_u = m_up;
// 	       m_u += Vector2i(bez.x()*fl,bez.y()*fl);
// 	       if( !belongToSegment(m_u) )
// 	         m_u += Vector2i(bez.x(),bez.y());
// 	       if(m_up!=m_u)
// 	       {
// // 	         m_c_n = m_u;
// //    	         m_c_n += Vector2i(0,-1);
// 	         m_c_n = m_up;
// 		 m_c_n -= getTangent();
// 		 if(m_a>=0)
//    	           m_c_n += Vector2i(1,0);
// 		 else
// 		   m_c_n += Vector2i(0,-1);
// 	       }
// 	     }
// 	  }
// 	//mise a jour de a et b
//         m_a = bez.y();
//         m_b = bez.x();
// 	m_mu = m_a*m_u.x() - m_b*m_u.y();
// 	} //if(P.x()+bez.x()<=m_cp_n.x())
// 	else
// 	{
// 	  dir = Vector2i(bez.x(), bez.y());
// 	  cout<<"bezout calculé inexploitable 4"<<endl;
// 	  return 2;
// 	}
//       }
// //  } // if(codePN==1)
//   
// /*  else //try to slim lower border line in negative sense, codePN == 0
//   {
//     Vector2D dirF = Vector2D(-dir.x(),-dir.y()); // vecteur direction
//     Vector2D bez = VectorUtils::closestBezout( dirF, -1);  //Bezout du vecteur direction
//     cout<<"le bezout = "<<bez.x()<<" "<<bez.y()<<endl;
//     
//       if(nbLowerLeaningPoints==1) //one lower leaning point
//       {
//         cout<<"un seul point d'appui inferieur"<<endl;
//         Vector2i v = Vector2i(m_l.x()+bez.x(),m_l.y()+bez.y());
//         if( belongToSegment(v) ) //L+bez appartient bien au segment
// 	{
// 	  //ajout nouveau pt d'appui inf
//           fl = floor(m_c_n.x()-m_l.x(),bez.x()).get_si();
//           //m_up = m_u;
// 	  m_l += Vector2i(bez.x()*fl,bez.y()*fl);
// 	  if( !belongToSegment(m_l) )
// 	         m_l -= Vector2i(bez.x(),bez.y());
// 	  //mise a jour de a et b
//           m_a = -bez.y();
// 	  m_b = -bez.x();
// 	  m_mu = m_a*m_u.x() - m_b*m_u.y();
//           //mise a jour premier pt extremité Cn'
// 	  m_cp_n = m_up;
// 	  if(m_a>=0)
//  	    m_cp_n += Vector2i(0,-1);
// 	  else
// 	    m_cp_n += Vector2i(-1,0);
// 	  //mise a jour pts d'appui sup
// 	  fl = ceil(m_up.x()-m_u.x(),bez.x()).get_si();
// 	  cout<<"fl vaut "<<fl<<endl;
// 	  m_up = m_u;
//  	  m_up += Vector2i(bez.x()*fl,bez.y()*fl);
// 	  if( !belongToSegment(m_up) )
// 	         m_up += Vector2i(bez.x(),bez.y());
// 	  return 1;
// 	}
// 	dir = Vector2i(-bez.x(),-bez.y());
// 	cout<<"bezout calculé inexploitable"<<endl;
// 	return 2;
//       } //if(nbLowerLeaningPoints==1)
//       else //at least 2 lower leaning points
//       {
//         /*Vector2i P = m_lp;
//         while( P.x()>=0 )
// 	{
// 	  P -= getTangent();
// 	}
// 	P += getTangent();*/
// //         Vector2i v = Vector2i(m_l.x()+bez.x(),m_l.y()+bez.y());
// // 	if( belongToSegment(v) ) //P+bez appartient bien au segment
// // 	{
// // 	 /* if(P==m_l) fl = floor(m_c_n.x()-P.x(),bez.x()).get_si();
// // 	  else fl = floor(P.x()-dir.x()-P.x(),bez.x()).get_si();*/
// // 	  fl = floor(m_c_n.x()-m_l.x(),bez.x()).get_si();
// // 	  m_lp = m_l;
// // 	  m_l = Vector2i(m_lp.x()+bez.x()*fl,m_lp.y()+bez.y()*fl);
// // 	  if( !belongToSegment(m_l) )
// // 	         m_l -= Vector2i(bez.x(),bez.y());
// // 	 /* if(P!=m_l) 
// // 	  {
// // 	    //mise a jour premier pt extremité Cn
// // 	    m_c_n = m_l;  //pb cn
// //  	    m_c_n += Vector2i(0,1);
// // 	  }*/
// // 	  if(nbUpperLeaningPoints==1) //one upper leaning point
// // 	  {
// // 	    fl = ceil(m_cp_n.x()-m_up.x(),bez.x()).get_si();
// // 	   // m_l = m_lp;
// //    	    m_up += Vector2i(bez.x()*fl,bez.y()*fl);  //manquerait pas un test d'appartenance?
// // 	    if( !belongToSegment(m_up) )
// // 	         m_up += Vector2i(bez.x(),bez.y());
// // 	  } //if(nbLowerLeaningPoints==1)
// // 	  else //at least two upper leaning points
// // 	  {
// // 	     while( m_u.x()<=0 )
// // 	     {
// // 	       m_u += getTangent();
// // 	     }
// // 	     m_u -= getTangent();
// // 	     if(m_up==m_u) fl = ceil(m_cp_n.x()-m_up.x(),bez.x()).get_si();
// // 	     else fl = ceil(m_u.x()+dir.x()-m_u.x(),bez.x()).get_si();
// // 	     Vector2i v = Vector2i(m_u.x()-bez.x(),m_u.y()-bez.y());
// // 	     if(belongToSegment(v)) //U-bez appartient au segment
// // 	     {
// // 	       m_up = m_u;
// // 	       m_up += Vector2i(bez.x()*fl,bez.y()*fl);
// // 	       if( !belongToSegment(m_up) )
// // 	         m_up += Vector2i(bez.x(),bez.y());
// // 	       if(m_up!=m_u)
// // 	       {
// // 	         /*m_cp_n = m_up;
// //    	         m_cp_n += Vector2i(0,-1);*/
// // 		 m_cp_n = m_u;
// // 		 m_cp_n += getTangent();
// // 		 if(m_a>=0)
// // 		   m_cp_n += Vector2i(0,-1);
// // 		 else
// // 		   m_cp_n += Vector2i(-1,0);
// // 	       }
// // 	     }
// // 	  }
// // 	//mise a jour de a et b
// //         m_a = -bez.y();
// //         m_b = -bez.x();
// // 	m_mu = m_a*m_u.x() - m_b*m_u.y();
// // 	} //if(P.x()+bez.x()<=m_cp_n.x())
// // 	else
// // 	{
// // 	  dir = Vector2i(-bez.x(), -bez.y());
// // 	  return 2;
// // 	}
// //       }
// //   } // else
// 
//   return 1;
// }

// uint
// C4CSegment::slimRight(Vector2i & dir )
// {
//   cout<<"slimRight vecteur direction à considérer : "<<dir<<endl;
//   uint codePN;
//   int fl;
//   
//   int nbUpperLeaningPoints = nbU();
//   int nbLowerLeaningPoints = nbL();
//   cout<<"nb de pts d'appui sup : "<<nbUpperLeaningPoints<<endl;
//   cout<<"nb de pts d'appui inf : "<<nbLowerLeaningPoints<<endl;
//   
//   int det = dir.x()*m_u.y() - dir.y()*m_u.x();
//   cout<<"determinant de dir et IU : "<<det<<endl;
//   if(det == 0)
//     cout<<"origine sur le bord sup!"<<endl;
//     
//   Vector2D dirF = Vector2D(dir.x(),dir.y()); // vecteur direction
//   Vector2D bez = VectorUtils::closestBezout( dirF, -1);  //Bezout du vecteur direction
//   cout<<"le bezout = "<<bez.x()<<" "<<bez.y()<<endl;
//     
//   //test pour savoir si le segment est modifiable dans ce sens (right)
//   bool oneFinalU = false, oneFinalL = false;
//   bool recuperable = false;
//   
//   if(m_up.x()>0)
//     oneFinalU = true;
//   else
//   {
//     Vector2i v = Vector2i(m_u.x()+bez.x(),m_u.y()+bez.y());
//     if(nbUpperLeaningPoints==1 && !belongToSegment(v))
//       {oneFinalU = true;  recuperable = true;}
//   }
//   
//   if(m_lp.x()<0)
//     oneFinalL = true;
//   else
//   {
//     Vector2i v = Vector2i(m_l.x()-bez.x(),m_l.y()-bez.y());
//     if(nbLowerLeaningPoints==1 && !belongToSegment(v))
//       {oneFinalL = true;  recuperable = true;}
//   }
//   
//   if( oneFinalU && oneFinalL )
//   {
//      dir = Vector2i(bez.x(), bez.y());
//      cout<<"segment non modifiable dans le sens right"<<endl;
//      if(recuperable)
//        return 2;
//      else
//        return 0;
//   }
// 
// 
//       if(nbUpperLeaningPoints==1) //one upper leaning point
//       {
//         cout<<"un seul point d'appui supérieur"<<endl;
//         Vector2i v = Vector2i(m_u.x()+bez.x(),m_u.y()+bez.y());
//         if( belongToSegment(v) ) //U+bez appartient bien au segment
// 	{
// 	  //ajout nouveau pt d'appui sup
//           fl = floor(m_cp_n.x()-m_u.x(),bez.x()).get_si();
// 	  cout<<"fl 1 vaut "<<fl<<endl;
//           //m_up = m_u;
// 	  m_up += Vector2i(bez.x()*fl,bez.y()*fl);
// 	  if( !belongToSegment(m_up) )
// 	  {
// 	    m_up -= Vector2i(bez.x(),bez.y());
// 	    cout<<"moins bez"<<endl;
// 	  }
// 	}
//           //mise a jour premier pt extremité Cn
// 	  m_c_n = m_l;
// 	  if(m_a>=0)
//  	    m_c_n += Vector2i(0,1);
// 	  else
// 	    m_c_n += Vector2i(1,0);
// 	  //mise a jour pts d'appui inf
// 	  fl = ceil(m_l.x()-m_lp.x(),bez.x()).get_si();  //fl is always negative
// 	  cout<<"ceil 2 vaut "<<fl<<endl;
// 	  m_l = m_lp;
//  	  m_l += Vector2i(bez.x()*fl,bez.y()*fl);
// 	  if( !belongToSegment(m_l) )
// 	    m_l += Vector2i(bez.x(),bez.y());
// 	  //mise a jour de a, b et mu
//           m_a = bez.y();
// 	  m_b = bez.x();
// 	  m_mu = m_a*m_u.x() - m_b*m_u.y();
// 	  dir = getTangent();
// 	  return 1;
// 	/*}
// 	dir = Vector2i(bez.x(), bez.y());
// 	cout<<"bezout calculé inexploitable 1"<<endl;
// 	return 2;*/
//       } //if(nbUpperLeaningPoints==1)
//       else //at least 2 upper leaning points
//       {
//         //compute the upper leaning point located at the left of I (the nearest)
//         Vector2i P = m_u;
//         while( P.x()<=0 )
// 	{
// 	  P += getTangent();
//         }
//         P -= getTangent();
//         Vector2i v = Vector2i(P.x()+bez.x(),P.y()+bez.y());
//         if( !belongToSegment(v) ) //P+bez n'appartient pas au segment
// 	{
// 	  m_u = P;
// 	  m_up = P;
// 	}
// 	else //P+bez appartient bien au segment
// 	{
// 	  if(P==m_up) fl = floor(m_cp_n.x()-P.x(),bez.x()).get_si();
// 	  else fl = floor(P.x()+dir.x()-P.x(),bez.x()).get_si();
//  	  m_u = P;
// 	  m_up = Vector2i(m_u.x()+bez.x()*fl,m_u.y()+bez.y()*fl);
// 	  if( !belongToSegment(m_up) )
// 	    m_up -= Vector2i(bez.x(),bez.y());
// 	  if(P!=m_up) 
// 	  {
// 	    //mise a jour premier pt extremité Cn'
// 	    m_cp_n = P;
// 	    m_cp_n += dir;
// 	    if(m_a>=0)
//  	      m_cp_n += Vector2i(0,-1);
// 	    else
// 	      m_cp_n += Vector2i(-1,0);
// 	  }
// 	 }
// 	  if(nbLowerLeaningPoints==1) //one lower leaning point
// 	  {
// 	    fl = ceil(m_c_n.x()-m_lp.x(),bez.x()).get_si();
// 	    //m_l = m_lp;
//    	    m_l += Vector2i(bez.x()*fl,bez.y()*fl);
// 	    if( !belongToSegment(m_l) )
// 	      m_l += Vector2i(bez.x(),bez.y());
// 	  } //if(nbLowerLeaningPoints==1)
// 	  else //at least two lower leaning points
// 	  {
// 	       while( m_lp.x()>=0 )
// 	       {
// 	         m_lp -= getTangent();
// 	       }
// 	       m_lp += getTangent();
// 	     if(m_lp==m_l) fl = ceil(m_c_n.x()-m_lp.x(),bez.x()).get_si();
// 	     else fl = ceil(m_lp.x()-dir.x()-m_lp.x(),bez.x()).get_si();
// 	     Vector2i v = Vector2i(m_lp.x()-bez.x(),m_lp.y()-bez.y());
// 	     if(belongToSegment(v)) //L'-bez appartient au segment
// 	     {
// 	       m_l = m_lp;
// 	       m_l += Vector2i(bez.x()*fl,bez.y()*fl);
// 	       if( !belongToSegment(m_l) )
// 	         m_l += Vector2i(bez.x(),bez.y());
// 	       if(m_lp!=m_l)
// 	       {
// 	         m_c_n = m_lp;
// 		 m_c_n -= getTangent();
// 		 if(m_a>=0)
//    	           m_c_n += Vector2i(0,1);
// 		 else
// 		   m_c_n += Vector2i(1,0);
// 	       }
// 	     }
// 	  }
// 	  //mise a jour de a et b
//           m_a = bez.y();
// 	  m_b = bez.x();
// 	  m_mu = m_a*m_u.x() - m_b*m_u.y();
// 	  dir = getTangent();
// 	//} //if(P.x()+bez.x()<=m_cp_n.x())
// 	/*else
// 	{
// 	  dir = Vector2i(bez.x(), bez.y());
// 	  cout<<"bezout calculé inexploitable 2"<<endl;
// 	  return 2;
// 	}*/
//       }
//   dir = getTangent();    
//   return 1;
// }

// uint
// C4CSegment::slimLeft(Vector2i & dir )
// {
//   cout<<"slimLeft vecteur direction à considérer : "<<dir<<endl;
//   uint codePN;
//   int fl;
//   
//   int nbUpperLeaningPoints = nbU();
//   int nbLowerLeaningPoints = nbL();
//   cout<<"nb de pts d'appui sup : "<<nbUpperLeaningPoints<<endl;
//   cout<<"nb de pts d'appui inf : "<<nbLowerLeaningPoints<<endl;
//   
//   int det = dir.x()*m_l.y() - dir.y()*m_l.x();
//   cout<<"determinant de dir et IL : "<<det<<endl;
//   
//   bool recuperable = false;
//   
//   if(det == 0)
//   {
//     cout<<"origine sur le bord inf!"<<endl;
//   }
//   
//     Vector2D dirF = Vector2D(dir.x(),dir.y()); // vecteur direction
//     Vector2D bez = VectorUtils::closestBezout( dirF, 1);  //Bezout du vecteur direction
//     cout<<"le bezout = "<<bez.x()<<" "<<bez.y()<<endl;
//     
//      //test pour savoir si le segment est modifiable dans ce sens (left)
//   bool oneFinalU = false, oneFinalL = false;
//   
//   if(m_up.x()<0)
//     oneFinalU = true;
//   else
//   {
//     Vector2i v = Vector2i(m_u.x()-bez.x(),m_u.y()-bez.y());
//     if(nbUpperLeaningPoints==1 && !belongToSegment(v))
//       {oneFinalU = true; recuperable = true;}
//   }
//   
//   if(m_lp.x()>0)
//     oneFinalL = true;
//   else
//   {
//     Vector2i v = Vector2i(m_l.x()+bez.x(),m_l.y()+bez.y());
//     if(nbLowerLeaningPoints==1 && !belongToSegment(v))
//       {oneFinalL = true;  recuperable = true;}
//   }
//   
//   if( oneFinalU && oneFinalL )
//   {
//      dir = Vector2i(bez.x(), bez.y());
//      cout<<"segment non modifiable dans le sens left"<<endl;
//      if(recuperable)
//        return 2;
//      else
//        return 0;
//   }
//   
// 
//       if(nbLowerLeaningPoints==1) //one lower leaning point
//       {
//         cout<<"un seul point d'appui inferieur"<<endl;
//         Vector2i v = Vector2i(m_l.x()+bez.x(),m_l.y()+bez.y());
//         if( belongToSegment(v) ) //U+bez appartient bien au segment
// 	{
// 	  //ajout nouveau pt d'appui inf
//           fl = floor(m_cp_n.x()-m_l.x(),bez.x()).get_si();
// 	  cout<<"fl 1 vaut "<<fl<<endl;
//          // m_lp = m_l;
// 	  m_lp += Vector2i(bez.x()*fl,bez.y()*fl);
// 	  if( !belongToSegment(m_lp) )
// 	  {
// 	    m_lp -= Vector2i(bez.x(),bez.y());
// 	    cout<<"moins bez"<<endl;
// 	  }
// 	}
// 	  //mise a jour de a et b
//           m_a = bez.y();
// 	  m_b = bez.x();
// 	  m_mu = m_a*m_u.x() - m_b*m_u.y();
//           //mise a jour premier pt extremité Cn
// 	  m_c_n = m_u;
// 	  if(m_a>=0)
//  	    m_c_n += Vector2i(1,0);
// 	  else
// 	    m_c_n += Vector2i(0,-1);
// 	  //mise a jour pts d'appui sup
// 	  fl = ceil(m_u.x()-m_up.x(),bez.x()).get_si();  //fl is always negative
// 	  cout<<"ceil 2 vaut "<<fl<<endl;
// 	  m_u = m_up;
//  	  m_u += Vector2i(bez.x()*fl,bez.y()*fl);
// 	  if( !belongToSegment(m_u) )
// 	    m_u += Vector2i(bez.x(),bez.y());
// 	  dir = getTangent();
// 	  return 1;
// 	//}
// 	/*dir = Vector2i(bez.x(), bez.y());
// 	cout<<"bezout calculé inexploitable 3"<<endl;
// 	return 2;*/
//       } //if(nbUpperLeaningPoints==1)
//       else //at least 2 lower leaning points
//       {
//         //compute the lower leaning point located at the left of I (the nearest)
//         Vector2i P = m_l;
//           while( P.x()<=0 )
// 	  {
// 	    P += getTangent();
// 	  }
// 	  P -= getTangent();
//         Vector2i v = Vector2i(P.x()+bez.x(),P.y()+bez.y());
// 	if( !belongToSegment(v) ) //P+bez n'appartient pas au segment
// 	{
// 	  m_l = P;
// 	  m_lp = P;
// 	}
// 	else //P+bez appartient bien au segment
// 	{
// 	  if(P==m_lp) fl = floor(m_cp_n.x()-P.x(),bez.x()).get_si();
// 	  else fl = floor(P.x()+dir.x()-P.x(),bez.x()).get_si();
// 	  m_l = P;
// 	  m_lp = Vector2i(m_l.x()+bez.x()*fl,m_l.y()+bez.y()*fl);
// 	  if( !belongToSegment(m_lp) )
// 	    m_lp -= Vector2i(bez.x(),bez.y());
// 	  if(P!=m_lp) 
// 	  {
// 	    //mise a jour premier pt extremité Cn'
// // 	    m_cp_n = m_lp;
// //  	    m_cp_n += Vector2i(-1,0);
//             m_cp_n = m_l;
// 	    m_cp_n += getTangent();
// 	    if(m_a>=0)
//  	      m_cp_n += Vector2i(-1,0);
// 	    else
// 	      m_cp_n += Vector2i(0,1);
// 	  }
// 	 }
// 	  if(nbUpperLeaningPoints==1) //one upper leaning point
// 	  {
// 	    fl = ceil(m_c_n.x()-m_up.x(),bez.x()).get_si();
// 	    //m_l = m_lp;
//    	    m_u += Vector2i(bez.x()*fl,bez.y()*fl);
// 	    if( !belongToSegment(m_u) )
// 	      m_u += Vector2i(bez.x(),bez.y());
// 	  } //if(nbUpperLeaningPoints==1)
// 	  else //at least two upper leaning points
// 	  {
// 	       while( m_up.x()>=0 )
// 	       {
// 	         m_up -= getTangent();
// 	       }
// 	       m_up += getTangent();
// 	     if(m_up==m_u) fl = ceil(m_c_n.x()-m_up.x(),bez.x()).get_si();
// 	     else fl = ceil(m_up.x()-dir.x()-m_up.x(),bez.x()).get_si();
// 	     Vector2i v = Vector2i(m_up.x()-bez.x(),m_up.y()-bez.y());
// 	     if(belongToSegment(v)) //U'-bez appartient au segment
// 	     {
// 	       m_u = m_up;
// 	       m_u += Vector2i(bez.x()*fl,bez.y()*fl);
// 	       if( !belongToSegment(m_u) )
// 	         m_u += Vector2i(bez.x(),bez.y());
// 	       if(m_up!=m_u)
// 	       {
// // 	         m_c_n = m_u;
// //    	         m_c_n += Vector2i(0,-1);
// 	         m_c_n = m_up;
// 		 m_c_n -= getTangent();
// 		 if(m_a>=0)
//    	           m_c_n += Vector2i(1,0);
// 		 else
// 		   m_c_n += Vector2i(0,-1);
// 	       }
// 	     }
// 	  }
// 	//mise a jour de a et b
//         m_a = bez.y();
//         m_b = bez.x();
// 	m_mu = m_a*m_u.x() - m_b*m_u.y();
// 	/*} //if(P.x()+bez.x()<=m_cp_n.x())
// 	else
// 	{
// 	  dir = Vector2i(bez.x(), bez.y());
// 	  cout<<"bezout calculé inexploitable 4"<<endl;
// 	  return 2;
// 	}*/
//       }
// //  } // if(codePN==1)
//   
//   dir = getTangent();
//   return 1;
// }
// 


/**
 * Tries to transform the discrete line segment in a thiner one but included
 * @param dir : the direction vector to consider 
 * @return '1' a thiner segment is computed, '0' the origin lies on the selected border and the segment is not thiner, '2' the computed Bezout vector is not usable
 */
// /*uint
// C4CSegment::slimUp(Vector2i & dir )
// {
//   cout<<"slimUp vecteur direction à considérer : "<<dir<<endl;
//   uint codePN;
//   int fl;
//   
//   int nbUpperLeaningPoints = nbU();
//   int nbLowerLeaningPoints = nbL();
//   cout<<"nb de pts d'appui sup : "<<nbUpperLeaningPoints<<endl;
//   cout<<"nb de pts d'appui inf : "<<nbLowerLeaningPoints<<endl;
//   
//   int det = dir.x()*m_u.y() - dir.y()*m_u.x();
//   cout<<"determinant de dir et IU : "<<det<<endl;
//   if(det == 0)
//   {
//     cout<<"impossible d'affiner par le haut, origine sur le bord sup!"<<endl;
//     return 0;
//   }
//   
//   // codePN 0: consider director vector in the negative sense, 
//   // 1: in the positive sense
//   cout<<"u.x : "<<m_u.x()<<endl;
//   if(m_u.x()<=0)
//     {codePN = 1; cout<<"sens positif"<<endl;}
//   else
//     {codePN = 0; cout<<"sens negatif"<<endl;}
//   
//   if(codePN == 1) //try to slim upper border line in positive sense
//   {
//     Vector2D dirF = Vector2D(dir.x(),dir.y()); // vecteur direction
//     Vector2D bez = VectorUtils::closestBezout( dirF, -1);  //Bezout du vecteur direction
//     cout<<"le bezout = "<<bez.x()<<" "<<bez.y()<<endl;
// 
//       if(nbUpperLeaningPoints==1) //one upper leaning point
//       {
//         cout<<"un seul point d'appui supérieur"<<endl;
//         Vector2i v = Vector2i(m_u.x()+bez.x(),m_u.y()+bez.y());
//         if( belongToSegment(v) ) //U+bez appartient bien au segment
// 	{
// 	  //ajout nouveau pt d'appui sup
//           fl = floor(m_cp_n.x()-m_u.x(),bez.x()).get_si();
// 	  cout<<"fl 1 vaut "<<fl<<endl;
//           //m_up = m_u;
// 	  m_up += Vector2i(bez.x()*fl,bez.y()*fl);
// 	  if( !belongToSegment(m_up) )
// 	  {
// 	    m_up -= Vector2i(bez.x(),bez.y());
// 	    cout<<"moins bez"<<endl;
// 	  }	  
//           //mise a jour premier pt extremité Cn
// 	  m_c_n = m_l;
// 	  if(m_a>=0)
//  	    m_c_n += Vector2i(0,1);
// 	  else
// 	    m_c_n += Vector2i(-1,0);
// 	  //mise a jour pts d'appui inf
// 	  fl = ceil(m_l.x()-m_lp.x(),bez.x()).get_si();  //fl is always negative
// 	  cout<<"ceil 2 vaut "<<fl<<endl;
// 	  m_l = m_lp;
//  	  m_l += Vector2i(bez.x()*fl,bez.y()*fl);
// 	  if( !belongToSegment(m_l) )
// 	    m_l += Vector2i(bez.x(),bez.y());
// 	  //mise a jour de a, b et mu
//           m_a = bez.y();
// 	  m_b = bez.x();
// 	  m_mu = m_a*m_u.x() - m_b*m_u.y();
// 	  return 1;
// 	}
// 	dir = Vector2i(bez.x(), bez.y());
// 	cout<<"bezout calculé inexploitable"<<endl;
// 	return 2;
//       } //if(nbUpperLeaningPoints==1)
//       else //at least 2 upper leaning points
//       {
//         //compute the upper leaning point located at the left of I (the nearest)
//         Vector2i P = m_u;
//         while( P.x()<=0 )
// 	{
// 	  P += getTangent();
// 	}
// 	P -= getTangent();
//         Vector2i v = Vector2i(P.x()+bez.x(),P.y()+bez.y());
// 	if( belongToSegment(v) ) //P+bez appartient bien au segment
// 	{
// 	  if(P==m_up) fl = floor(m_cp_n.x()-P.x(),bez.x()).get_si();
// 	  else fl = floor(P.x()+dir.x()-P.x(),bez.x()).get_si();
// 	  m_u = P;
// 	  m_up = Vector2i(m_u.x()+bez.x()*fl,m_u.y()+bez.y()*fl);
// 	  if( !belongToSegment(m_up) )
// 	    m_up -= Vector2i(bez.x(),bez.y());
// 	  if(P!=m_up) 
// 	  {
// 	    //mise a jour premier pt extremité Cn'
// 	    m_cp_n = P;
// 	    m_cp_n += dir;
// 	    if(m_a>=0)
//  	      m_cp_n += Vector2i(0,-1);
// 	    else
// 	      m_cp_n += Vector2i(-1,0);
// 	  }
// 	  if(nbLowerLeaningPoints==1) //one lower leaning point
// 	  {
// 	    fl = ceil(m_c_n.x()-m_lp.x(),bez.x()).get_si();
// 	    //m_l = m_lp;
//    	    m_l += Vector2i(bez.x()*fl,bez.y()*fl);
// 	    if( !belongToSegment(m_l) )
// 	      m_l += Vector2i(bez.x(),bez.y());
// 	  } //if(nbLowerLeaningPoints==1)
// 	  else //at least two lower leaning points
// 	  {
// 	     while( m_lp.x()>=0 )
// 	     {
// 	       m_lp -= getTangent();
// 	     }
// 	     m_lp += getTangent();
// 	     if(m_lp==m_l) fl = ceil(m_c_n.x()-m_lp.x(),bez.x()).get_si();
// 	     else fl = ceil(m_lp.x()-dir.x()-m_lp.x(),bez.x()).get_si();
// 	     Vector2i v = Vector2i(m_lp.x()-bez.x(),m_lp.y()-bez.y());
// 	     if(belongToSegment(v)) //L'-bez appartient au segment
// 	     {
// 	       m_l = m_lp;
// 	       m_l += Vector2i(bez.x()*fl,bez.y()*fl);
// 	       if( !belongToSegment(m_l) )
// 	         m_l += Vector2i(bez.x(),bez.y());
// 	       if(m_lp!=m_l)
// 	       {
// 	         m_c_n = m_lp;
// 		 m_c_n -= getTangent();
// 		 if(m_a>=0)
//    	           m_c_n += Vector2i(0,1);
// 		 else
// 		   m_c_n += Vector2i(1,0);
// 	       }
// 	     }
// 	  }
// 	  //mise a jour de a et b
//           m_a = bez.y();
// 	  m_b = bez.x();
// 	  m_mu = m_a*m_u.x() - m_b*m_u.y();
// 	} //if(P.x()+bez.x()<=m_cp_n.x())
// 	else
// 	{
// 	  dir = Vector2i(bez.x(), bez.y());
// 	  return 2;
// 	}
//       }
//   } // if(codePN==1)
//   
//   else //try to slim upper border line in negative sense, codePN == 0
//   {
//     Vector2D dirF = Vector2D(-dir.x(),-dir.y()); // vecteur direction
//     Vector2D bez = VectorUtils::closestBezout( dirF, 1);  //Bezout du vecteur direction
//     cout<<"le bezout = "<<bez.x()<<" "<<bez.y()<<endl;
//     
//       if(nbUpperLeaningPoints==1) //one upper leaning point
//       {
//         cout<<"un seul point d'appui supérieur"<<endl;
//         Vector2i v = Vector2i(m_u.x()+bez.x(),m_u.y()+bez.y());
//         if( belongToSegment(v) ) //U+bez appartient bien au segment
// 	{
// 	  //ajout nouveau pt d'appui sup
//           fl = floor(m_c_n.x()-m_u.x(),bez.x()).get_si();
//           //m_up = m_u;
// 	  m_u += Vector2i(bez.x()*fl,bez.y()*fl);
// 	  if( !belongToSegment(m_u) )
// 	         m_u -= Vector2i(bez.x(),bez.y());
// 	  //mise a jour de a et b
//           m_a = -bez.y();
// 	  m_b = -bez.x();	
// 	  m_mu = m_a*m_u.x() - m_b*m_u.y();
//           //mise a jour premier pt extremité Cn
// 	  m_cp_n = m_lp;
// 	  if(m_a>=0)
//  	    m_cp_n += Vector2i(-1,0);
// 	  else
// 	    m_cp_n += Vector2i(0,1);
// 	  //mise a jour pts d'appui inf
// 	  fl = ceil(m_lp.x()-m_l.x(),bez.x()).get_si();
// 	  cout<<"fl vaut "<<fl<<endl;
// 	  m_lp = m_l;
//  	  m_lp += Vector2i(bez.x()*fl,bez.y()*fl);
// 	  if( !belongToSegment(m_lp) )
// 	         m_lp += Vector2i(bez.x(),bez.y());
// 	  return 1;
// 	}
// 	dir = Vector2i(-bez.x(),-bez.y());
// 	cout<<"bezout calculé inexploitable"<<endl;
// 	return 2;
//       } //if(nbUpperLeaningPoints==1)
//       else //at least 2 upper leaning points
//       {
//        /* Vector2i P = m_up;
//         while( P.x()>=0 )
// 	{
// 	  P -= getTangent();
// 	}
// 	P += getTangent();*/
//         Vector2i v = Vector2i(m_u.x()+bez.x(),m_u.y()+bez.y());
// 	if( belongToSegment(v) ) //U+bez appartient bien au segment
// 	{
// 	  /*if(P==m_u) fl = floor(m_c_n.x()-P.x(),bez.x()).get_si();
// 	  else fl = floor(P.x()-dir.x()-P.x(),bez.x()).get_si();*/
// 	  fl = floor(m_c_n.x()-m_u.x(),bez.x()).get_si();
// 	  m_up = /*P*/m_u;
// 	  m_u = Vector2i(m_up.x()+bez.x()*fl,m_up.y()+bez.y()*fl);
// 	  if( !belongToSegment(m_u) )
// 	         m_u -= Vector2i(bez.x(),bez.y());
// 	  if(m_up!=m_u) 
// 	  {
// 	    //mise a jour premier pt extremité Cn
// 	   /* m_c_n = m_u;*/
// 	    m_c_n = m_up;
// 	    m_c_n -= getTangent();
// 	    if(m_a>=0)
//  	      m_c_n += Vector2i(1,0);
// 	    else
// 	      m_c_n += Vector2i(0,-1);
// 	  }
// 	  if(nbLowerLeaningPoints==1) //one lower leaning point
// 	  {
// 	    fl = ceil(m_cp_n.x()-m_lp.x(),bez.x()).get_si();
// 	   // m_l = m_lp;
//    	    m_lp += Vector2i(bez.x()*fl,bez.y()*fl);  //manquerait pas un test d'appartenance?
// 	    if( !belongToSegment(m_lp) )
// 	         m_lp += Vector2i(bez.x(),bez.y());
// 	  } //if(nbLowerLeaningPoints==1)
// 	  else //at least two lower leaning points
// 	  {
// 	     while( m_l.x()<=0 )
// 	     {
// 	       m_l += getTangent();
// 	     }
// 	     m_l -= getTangent();
// 	     if(m_lp==m_l) fl = ceil(m_cp_n.x()-m_lp.x(),bez.x()).get_si();
// 	     else fl = ceil(m_l.x()+dir.x()-m_l.x(),bez.x()).get_si();
// 	     Vector2i v = Vector2i(m_l.x()-bez.x(),m_l.y()-bez.y());
// 	     if(belongToSegment(v)) //L'-bez appartient au segment
// 	     {
// 	       m_lp = m_l;
// 	       m_lp += Vector2i(bez.x()*fl,bez.y()*fl);
// 	       if( !belongToSegment(m_lp) )
// 	         m_lp += Vector2i(bez.x(),bez.y());
// 	       if(m_lp!=m_l)
// 	       {
// 	         m_cp_n = m_l;
// 		 m_cp_n += getTangent();
// 		 if(m_a>=0)
//    	           m_cp_n += Vector2i(-1,0);
// 		 else
// 		   m_cp_n += Vector2i(0,1);
// 	       }
// 	     }
// 	  }
// 	//mise a jour de a et b
//         m_a = -bez.y();
// 	m_b = -bez.x();
// 	m_mu = m_a*m_u.x() - m_b*m_u.y();
// 	} //if(P.x()+bez.x()<=m_cp_n.x())
// 	else
// 	{
// 	  dir = Vector2i(-bez.x(), -bez.y());
// 	  return 2;
// 	}
//       }
//   } // else
// 
//   return 1;
// }*/

// uint
// C4CSegment::slimLow(Vector2i & dir )
// {
//   cout<<"slimLow vecteur direction à considérer : "<<dir<<endl;
//   uint codePN;
//   int fl;
//   
//   int nbUpperLeaningPoints = nbU();
//   int nbLowerLeaningPoints = nbL();
//   cout<<"nb de pts d'appui sup : "<<nbUpperLeaningPoints<<endl;
//   cout<<"nb de pts d'appui inf : "<<nbLowerLeaningPoints<<endl;
//   
//   int det = dir.x()*m_l.y() - dir.y()*m_l.x();
//   cout<<"determinant de dir et IL : "<<det<<endl;
//   if(det == 0)
//   {
//     cout<<"impossible d'affiner par le bas, origine sur le bord inf!"<<endl;
//     return 0;
//   }
//   
//   // codePN 0: consider director vector in the negative sense, 
//   // 1: in the positive sense
//   if(m_l.x()<=0)
//     {codePN = 1; cout<<"sens positif"<<endl;}
//   else
//     {codePN = 0; cout<<"sens negatif"<<endl;}
//   
//   if(codePN == 1) //try to slim lower border line in positive sense
//   {
//     Vector2D dirF = Vector2D(dir.x(),dir.y()); // vecteur direction
//     Vector2D bez = VectorUtils::closestBezout( dirF, 1);  //Bezout du vecteur direction
//     cout<<"le bezout = "<<bez.x()<<" "<<bez.y()<<endl;
// 
//       if(nbLowerLeaningPoints==1) //one lower leaning point
//       {
//         cout<<"un seul point d'appui inferieur"<<endl;
//         Vector2i v = Vector2i(m_l.x()+bez.x(),m_l.y()+bez.y());
//         if( belongToSegment(v) ) //U+bez appartient bien au segment
// 	{
// 	  //ajout nouveau pt d'appui inf
//           fl = floor(m_cp_n.x()-m_l.x(),bez.x()).get_si();
// 	  cout<<"fl 1 vaut "<<fl<<endl;
//          // m_lp = m_l;
// 	  m_lp += Vector2i(bez.x()*fl,bez.y()*fl);
// 	  if( !belongToSegment(m_lp) )
// 	  {
// 	    m_lp -= Vector2i(bez.x(),bez.y());
// 	    cout<<"moins bez"<<endl;
// 	  }
// 	  //mise a jour de a et b
//           m_a = bez.y();
// 	  m_b = bez.x();
// 	  m_mu = m_a*m_u.x() - m_b*m_u.y();
//           //mise a jour premier pt extremité Cn
// 	  m_c_n = m_u;
// 	  if(m_a>=0)
//  	    m_c_n += Vector2i(1,0);
// 	  else
// 	    m_c_n += Vector2i(0,-1);
// 	  //mise a jour pts d'appui sup
// 	  fl = ceil(m_u.x()-m_up.x(),bez.x()).get_si();  //fl is always negative
// 	  cout<<"ceil 2 vaut "<<fl<<endl;
// 	  m_u = m_up;
//  	  m_u += Vector2i(bez.x()*fl,bez.y()*fl);
// 	  if( !belongToSegment(m_u) )
// 	    m_u += Vector2i(bez.x(),bez.y());
// 	  return 1;
// 	}
// 	dir = Vector2i(bez.x(), bez.y());
// 	cout<<"bezout calculé inexploitable"<<endl;
// 	return 2;
//       } //if(nbUpperLeaningPoints==1)
//       else //at least 2 lower leaning points
//       {
//         //compute the lower leaning point located at the left of I (the nearest)
//         Vector2i P = m_l;
//         while( P.x()<=0 )
// 	{
// 	  P += getTangent();
// 	}
// 	P -= getTangent();
//         Vector2i v = Vector2i(P.x()+bez.x(),P.y()+bez.y());
// 	if( belongToSegment(v) ) //P+bez appartient bien au segment
// 	{
// 	  if(P==m_lp) fl = floor(m_cp_n.x()-P.x(),bez.x()).get_si();
// 	  else fl = floor(P.x()+dir.x()-P.x(),bez.x()).get_si();
// 	  m_l = P;
// 	  m_lp = Vector2i(m_l.x()+bez.x()*fl,m_l.y()+bez.y()*fl);
// 	  if( !belongToSegment(m_lp) )
// 	    m_lp -= Vector2i(bez.x(),bez.y());
// 	  if(P!=m_lp) 
// 	  {
// 	    //mise a jour premier pt extremité Cn'
// // 	    m_cp_n = m_lp;
// //  	    m_cp_n += Vector2i(-1,0);
//             m_cp_n = m_l;
// 	    m_cp_n += getTangent();
// 	    if(m_a>=0)
//  	      m_cp_n += Vector2i(-1,0);
// 	    else
// 	      m_cp_n += Vector2i(0,1);
// 	  }
// 	  if(nbUpperLeaningPoints==1) //one upper leaning point
// 	  {
// 	    fl = ceil(m_c_n.x()-m_up.x(),bez.x()).get_si();
// 	    //m_l = m_lp;
//    	    m_u += Vector2i(bez.x()*fl,bez.y()*fl);
// 	    if( !belongToSegment(m_u) )
// 	      m_u += Vector2i(bez.x(),bez.y());
// 	  } //if(nbUpperLeaningPoints==1)
// 	  else //at least two upper leaning points
// 	  {
// 	     while( m_up.x()>=0 )
// 	     {
// 	       m_up -= getTangent();
// 	     }
// 	     m_up += getTangent();
// 	     if(m_up==m_u) fl = ceil(m_c_n.x()-m_up.x(),bez.x()).get_si();
// 	     else fl = ceil(m_up.x()-dir.x()-m_up.x(),bez.x()).get_si();
// 	     Vector2i v = Vector2i(m_up.x()-bez.x(),m_up.y()-bez.y());
// 	     if(belongToSegment(v)) //U'-bez appartient au segment
// 	     {
// 	       m_u = m_up;
// 	       m_u += Vector2i(bez.x()*fl,bez.y()*fl);
// 	       if( !belongToSegment(m_u) )
// 	         m_u += Vector2i(bez.x(),bez.y());
// 	       if(m_up!=m_u)
// 	       {
// // 	         m_c_n = m_u;
// //    	         m_c_n += Vector2i(0,-1);
// 	         m_c_n = m_up;
// 		 m_c_n -= getTangent();
// 		 if(m_a>=0)
//    	           m_c_n += Vector2i(1,0);
// 		 else
// 		   m_c_n += Vector2i(0,-1);
// 	       }
// 	     }
// 	  }
// 	//mise a jour de a et b
//         m_a = bez.y();
//         m_b = bez.x();
// 	m_mu = m_a*m_u.x() - m_b*m_u.y();
// 	} //if(P.x()+bez.x()<=m_cp_n.x())
// 	else
// 	{
// 	  dir = Vector2i(bez.x(), bez.y());
// 	  return 2;
// 	}
//       }
//   } // if(codePN==1)
//   
//   else //try to slim lower border line in negative sense, codePN == 0
//   {
//     Vector2D dirF = Vector2D(-dir.x(),-dir.y()); // vecteur direction
//     Vector2D bez = VectorUtils::closestBezout( dirF, -1);  //Bezout du vecteur direction
//     cout<<"le bezout = "<<bez.x()<<" "<<bez.y()<<endl;
//     
//       if(nbLowerLeaningPoints==1) //one lower leaning point
//       {
//         cout<<"un seul point d'appui inferieur"<<endl;
//         Vector2i v = Vector2i(m_l.x()+bez.x(),m_l.y()+bez.y());
//         if( belongToSegment(v) ) //L+bez appartient bien au segment
// 	{
// 	  //ajout nouveau pt d'appui inf
//           fl = floor(m_c_n.x()-m_l.x(),bez.x()).get_si();
//           //m_up = m_u;
// 	  m_l += Vector2i(bez.x()*fl,bez.y()*fl);
// 	  if( !belongToSegment(m_l) )
// 	         m_l -= Vector2i(bez.x(),bez.y());
// 	  //mise a jour de a et b
//           m_a = -bez.y();
// 	  m_b = -bez.x();
// 	  m_mu = m_a*m_u.x() - m_b*m_u.y();
//           //mise a jour premier pt extremité Cn'
// 	  m_cp_n = m_up;
// 	  if(m_a>=0)
//  	    m_cp_n += Vector2i(0,-1);
// 	  else
// 	    m_cp_n += Vector2i(-1,0);
// 	  //mise a jour pts d'appui sup
// 	  fl = ceil(m_up.x()-m_u.x(),bez.x()).get_si();
// 	  cout<<"fl vaut "<<fl<<endl;
// 	  m_up = m_u;
//  	  m_up += Vector2i(bez.x()*fl,bez.y()*fl);
// 	  if( !belongToSegment(m_up) )
// 	         m_up += Vector2i(bez.x(),bez.y());
// 	  return 1;
// 	}
// 	dir = Vector2i(-bez.x(),-bez.y());
// 	cout<<"bezout calculé inexploitable"<<endl;
// 	return 2;
//       } //if(nbLowerLeaningPoints==1)
//       else //at least 2 lower leaning points
//       {
//         /*Vector2i P = m_lp;
//         while( P.x()>=0 )
// 	{
// 	  P -= getTangent();
// 	}
// 	P += getTangent();*/
//         Vector2i v = Vector2i(m_l.x()+bez.x(),m_l.y()+bez.y());
// 	if( belongToSegment(v) ) //P+bez appartient bien au segment
// 	{
// 	 /* if(P==m_l) fl = floor(m_c_n.x()-P.x(),bez.x()).get_si();
// 	  else fl = floor(P.x()-dir.x()-P.x(),bez.x()).get_si();*/
// 	  fl = floor(m_c_n.x()-m_l.x(),bez.x()).get_si();
// 	  m_lp = m_l;
// 	  m_l = Vector2i(m_lp.x()+bez.x()*fl,m_lp.y()+bez.y()*fl);
// 	  if( !belongToSegment(m_l) )
// 	         m_l -= Vector2i(bez.x(),bez.y());
// 	 /* if(P!=m_l) 
// 	  {
// 	    //mise a jour premier pt extremité Cn
// 	    m_c_n = m_l;  //pb cn
//  	    m_c_n += Vector2i(0,1);
// 	  }*/
// 	  if(nbUpperLeaningPoints==1) //one upper leaning point
// 	  {
// 	    fl = ceil(m_cp_n.x()-m_up.x(),bez.x()).get_si();
// 	   // m_l = m_lp;
//    	    m_up += Vector2i(bez.x()*fl,bez.y()*fl);  //manquerait pas un test d'appartenance?
// 	    if( !belongToSegment(m_up) )
// 	         m_up += Vector2i(bez.x(),bez.y());
// 	  } //if(nbLowerLeaningPoints==1)
// 	  else //at least two upper leaning points
// 	  {
// 	     while( m_u.x()<=0 )
// 	     {
// 	       m_u += getTangent();
// 	     }
// 	     m_u -= getTangent();
// 	     if(m_up==m_u) fl = ceil(m_cp_n.x()-m_up.x(),bez.x()).get_si();
// 	     else fl = ceil(m_u.x()+dir.x()-m_u.x(),bez.x()).get_si();
// 	     Vector2i v = Vector2i(m_u.x()-bez.x(),m_u.y()-bez.y());
// 	     if(belongToSegment(v)) //U-bez appartient au segment
// 	     {
// 	       m_up = m_u;
// 	       m_up += Vector2i(bez.x()*fl,bez.y()*fl);
// 	       if( !belongToSegment(m_up) )
// 	         m_up += Vector2i(bez.x(),bez.y());
// 	       if(m_up!=m_u)
// 	       {
// 	         /*m_cp_n = m_up;
//    	         m_cp_n += Vector2i(0,-1);*/
// 		 m_cp_n = m_u;
// 		 m_cp_n += getTangent();
// 		 if(m_a>=0)
// 		   m_cp_n += Vector2i(0,-1);
// 		 else
// 		   m_cp_n += Vector2i(-1,0);
// 	       }
// 	     }
// 	  }
// 	//mise a jour de a et b
//         m_a = -bez.y();
//         m_b = -bez.x();
// 	m_mu = m_a*m_u.x() - m_b*m_u.y();
// 	} //if(P.x()+bez.x()<=m_cp_n.x())
// 	else
// 	{
// 	  dir = Vector2i(-bez.x(), -bez.y());
// 	  return 2;
// 	}
//       }
//   } // else
// 
//   return 1;
// }

/**
 * Tries to extend the discrete line segment to the "front"
 * (positively along the contour).
 *
 * @param code 0: no move, 1: turn right (toward interior), 2: move
 * straight ahead (no turn), 3: turn left (toward exterior).
 *
 * @return '0' if this extension was valid and the discrete line
 * segment has been updated, '1' if the extension was not valid
 * because of code incompatibility, '2' if the extension was not
 * valid because it cannot be a digital straight line. In case '!=0'
 * the discrete line segment is unchanged.
 */
uint
C4CSegment::extendsFront( uint code )
{
  // Memorizes C'_n and state.
  Vector2i old_cp_n( m_cp_n );
  uint8 state = m_state_cp_n;
  
  // [code] indicates where is C'_{n+1}.
  // Depends on the current state and on the move.
  // Some moves are incompatible. Others are compatible but there is no
  // discrete line that includes all these points.
  switch ( m_state_cp_n + code ) {
  case 0: break;
  case 1: 
    if ( m_a <= 0 ) // test compatibility
      { m_cp_n.y()--; m_state_cp_n = 4; } 
    else code = 0;
    break;
  case 2: m_cp_n.x()++; m_state_cp_n = 0; break;
  case 3: 
    if ( m_a >= 0 ) // test compatibility
      { m_cp_n.y()++; m_state_cp_n = 8; }
    else code = 0;
    break;
  case 4: code = 0; break;
  case 5: code = 0; break;
  case 6: m_cp_n.y()--; break;
  case 7: 
    ASSERT_C4CSegment( m_a <= 0 ); // test compatibility
    m_cp_n.x()++; m_state_cp_n = 0;
    break;
  case 8: code = 0; break;
  case 9: 
    ASSERT_C4CSegment( m_a >= 0 ); // test compatibility
    m_cp_n.x()++; m_state_cp_n = 0;
    break;
  case 10: m_cp_n.y()++; break;
  case 11: code = 0; break;
  default: code = 0; break;
  }

  // In this case, the discrete line segment is not updated.
  if ( code == 0 ) return 1;

  // compute new ax-by.
  int c = m_a * m_cp_n.x() - m_b * m_cp_n.y();
  int lower_bound = m_mu - 1;
  int upper_bound = m_mu + ( m_a >= 0 ? m_a : -m_a ) + m_b;
  // start recognition.
  if ( ( c < lower_bound ) || ( c > upper_bound ) )
    { // not a digital line
      m_cp_n = old_cp_n;
      m_state_cp_n = state;
      return 2;
    }
  else if ( c == lower_bound )
    { // new upper critical point.
      m_up = m_cp_n; // U'<- C'_{n+1}
      m_l = m_lp;    // L <- L'
      m_a = m_up.y() - m_u.y();
      m_b = m_up.x() - m_u.x();
      m_mu = m_a * m_up.x() - m_b * m_up.y();
    }
  else if ( c == upper_bound )
    { // new lower critical point.
      m_u = m_up;    // U <- U'
      m_lp = m_cp_n; // L'<- C'_{n+1}
      m_a = m_lp.y() - m_l.y();
      m_b = m_lp.x() - m_l.x();
      m_mu = m_a * m_lp.x() - m_b * m_lp.y() 
	- ( m_a >= 0 ? m_a : -m_a ) 
	- m_b + 1;
    }
  else
    { // still the same digital line.
      // The upper and lower critical point may change.
      if ( c == m_mu ) 
	m_up = m_cp_n; // U' <- C'_{n+1}
      if ( c == ( upper_bound - 1 ) )
	m_lp = m_cp_n; // L' <- C'_{n+1}
    }

  // The discrete line segment has been updated.
  return 0;
}


/**
 * Tries to extend the discrete line segment to the "back"
 * (negatively along the contour).
 *
 * @param code 0: no move, 1: turn left (toward interior), 2: move
 * straight ahead (no turn), 3: turn right (toward exterior).
 *
 * @return '0' if this extension was valid and the discrete line
 * segment has been updated, '1' if the extension was not valid
 * because of code incompatibility, '2' if the extension was not
 * valid because it cannot be a digital straight line. In case '!=0'
 * the discrete line segment is unchanged.
 */
uint
C4CSegment::extendsBack( uint code )
{
  // Memorizes C_n and state.
  Vector2i old_c_n( m_c_n );
  uint8 state = m_state_c_n;

  // [code] indicates where is C'_{n+1}.
  // Depends on the current state and on the move.
  // Some moves are incompatible. Others are compatible but there is no
  // discrete line that includes all these points.
  switch ( m_state_c_n + code ) {
  case 0: break;
  case 1: 	
    if ( m_a >= 0 ) // test compatibility
      { m_c_n.y()--; m_state_c_n = 4; }
    else code = 0;
    break;
  case 2: m_c_n.x()--; m_state_c_n = 0; break;
  case 3: 
    if ( m_a <= 0 ) // test compatibility
      { m_c_n.y()++; m_state_c_n = 8; }
    else code = 0;
    break;
  case 4: code = 0; break;
  case 5: code = 0; break;
  case 6: m_c_n.y()--; break;
  case 7: 
    ASSERT_C4CSegment( m_a >= 0 ); // test compatibility
    m_c_n.x()--; m_state_c_n = 0; break;
  case 8: code = 0; break;
  case 9: 
    ASSERT_C4CSegment( m_a <= 0 ); // test compatibility
    m_c_n.x()--; m_state_c_n = 0; break;
  case 10: m_c_n.y()++; break;
  case 11: code = 0; break;
  default: code = 0; break;
  }

  // In this case, the discrete line segment is not updated.
  if ( code == 0 ) return 1;

  // compute new ax-by.
  int c = m_a * m_c_n.x() - m_b * m_c_n.y();
  int lower_bound = m_mu - 1;
  int upper_bound = m_mu + ( m_a >= 0 ? m_a : -m_a ) + m_b;
  // start recognition.
  if ( ( c < lower_bound ) || ( c > upper_bound ) )
    { // not a digital line
      m_c_n = old_c_n;
      m_state_c_n = state;
      return 2;
    }
  else if ( c == lower_bound )
    { // new upper critical point.
      m_u = m_c_n;   // U <- C_{n+1}
      m_lp = m_l;    // L'<- L
      m_a = m_up.y() - m_u.y();
      m_b = m_up.x() - m_u.x();
      m_mu = m_a * m_u.x() - m_b * m_u.y();
    }
  else if ( c == upper_bound )
    { // new lower critical point.
      m_up = m_u;    // U'<- U
      m_l = m_c_n;   // L <- C_{n+1}
      m_a = m_lp.y() - m_l.y();
      m_b = m_lp.x() - m_l.x();
      m_mu = m_a * m_l.x() - m_b * m_l.y() 
	- ( m_a >= 0 ? m_a : -m_a )
	- m_b + 1;
    }
  else
    { // still the same digital line.
      // The upper and lower critical point may change.
      if ( c == m_mu ) 
	m_u = m_c_n; // U <- C_{n+1}
      if ( c == ( upper_bound - 1 ) )
	m_l = m_c_n; // L <- C_{n+1}
    }

  // The discrete line segment has been updated.
  return 0;
}



/**
 * Tries to retract the discrete line segment to the "front"
 * (positively along the contour).
 *
 * @param code the shape of the retraction going from the "front"
 * extremity: 0: no move, 1: turn left (toward interior), 2: move
 * straight ahead (no turn), 3: turn right (toward exterior).
 *
 * @return 'true' if this retraction was valid and the discrete line
 * segment has been updated, otherwise 'false' and the discrete line
 * segment is unchanged.
 *
 * @pre m_cp_n != (1,0)
 *
 * @todo False version. Do the same as in retractsBack.
 */
// bool
// C4CSegment::retractsFront( uint code )
// {
//   // Cannot retract if reference frame is in the retraction.
//   ASSERT_C4CSegment( ( m_cp_n.x() != 1 ) || ( m_cp_n.y() != 0 ) );
//   
//   Vector2i m( m_cp_n ); // memorizes m.
// 
//   // Takes care of state machine.
//   switch ( m_state_cp_n + code ) {
//   case 0: break;
//   case 1: ASSERT_C4CSegment( m_a > 0 );
//     m_state_cp_n = 8; 
//     --m_cp_n.x();
//     break;
//   case 2: // state unchanged
//     --m_cp_n.x();
//     break;
//   case 3: ASSERT_C4CSegment( m_a < 0 );
//     m_state_cp_n = 4;
//     --m_cp_n.x();
//     break;
//   case 4: break;
//   case 5: ASSERT_C4CSegment( m_a < 0 );
//     m_state_cp_n = 0;
//     ++m_cp_n.y();
//     break;
//   case 6: ASSERT_C4CSegment( m_a < 0 );
//     // state unchanged
//     ++m_cp_n.y();
//     break;
//   case 7: ASSERT_C4CSegment( false ); // Should not arrived
//     code = 0;
//     break;
//   case 8: break;
//   case 9: ASSERT_C4CSegment( false ); // Should not arrived
//     code = 0;
//     break;
//   case 10: ASSERT_C4CSegment( m_a > 0 );
//     // state unchanged
//     --m_cp_n.y();
//     break;
//   case 11: ASSERT_C4CSegment( m_a > 0 );
//     m_state_cp_n = 0;
//     --m_cp_n.y();
//     break;
//   }
// 
//   if ( code == 0 ) return false;
//   
//   // Updates line segment and upper and lower leaning lines depending on the
//   // retracted point.
// 
//   // Case 0: horizontal line.
//   if ( ( m_a == 0 ) && ( m_b == 1 ) )
//     {
//       ASSERT_C4CSegment( ( m == m_up ) && ( m == m_lp ) );
//       --m_up.x();
//       --m_lp.x();
//       ASSERT_C4CSegment( m_u != m_up );
//     }
//   // Case 1: M == U'
//   else if ( m == m_up )
//     { // Two subcases depending on L' == L
//       if ( m_lp == m_l )
// 	{ // Case 1.a: L' == L
// 	  // => M Changed the slope of the line.
// 	  
// 	  // P "is" a lower leaning point.
// 	  // a > 0 => P = M + (1,-1)
// 	  // a < 0 => P = M + (-1,-1)
// 	  Vector2i p( m );
// 	  if ( m_a > 0 )
// 	    { 
// 	      p.x() += 1;
// 	      p.y() -= 1;
// 	    }
// 	  else // ( m_a < 0 )
// 	    { 
// 	      p.x() -= 1;
// 	      p.y() -= 1;
// 	    }
// 	  // Since P and L' defines the slope of the line, we get:
// 	  m_b = p.x() - m_lp.x();
// 	  m_a = p.y() - m_lp.y();
// 	  // Since U belongs to the upper leaning line, we get:
// 	  m_mu = m_a * m_u.x() - m_b * m_u.y();
// 	  // L' and U are not changed.
// 
// 	  // We recompute U' from U and the length of the discrete segment.
// 	  int k = ( m_cp_n.x() - m_u.x() ) / m_b;
// 	  m_up.x() = m_u.x() + k * m_b;
// 	  m_up.y() = m_u.y() + k * m_a;
// 
// 	  // We recompute L from L' and the length of the discrete segment.
// 	  int l = ( p.x() - m_c_n.x() ) / m_b;
// 	  m_l.x() = m_lp.x() - ( l - 1 ) * m_b;
// 	  m_l.y() = m_lp.y() - ( l - 1 ) * m_a;
// 	  
// 	  ASSERT_C4CSegment( ( k >= 0 ) && ( l >= 1 ) );
// 	}
//       else
// 	{ // Case 1.b: L' != L
// 	  // => M did not change the slope of the line, was only on the upper
// 	  // leaning line.
// 	  // U' <- U' - (b,a)
// 	  m_up.x() -= m_b;
// 	  m_up.y() -= m_a;
// 	}
//     } // if ( m == m_up )
//   // Case 2: M == L'
//   else if ( m == m_lp )
//     { // Two subcases depending on U' == U
//       if ( m_up == m_u )
// 	{ // Case 2.a: U' == U 
// 	  // => M Changed the slope of the line.
// 	  
// 	  // P "is" an upper leaning point.
// 	  // a > 0 => P = M + (-1,1)
// 	  // a < 0 => P = M + (1,1)
// 	  Vector2i p( m );
// 	  if ( m_a > 0 )
// 	    { 
// 	      p.x() -= 1;
// 	      p.y() += 1;
// 	    }
// 	  else // ( m_a < 0 )
// 	    { 
// 	      p.x() += 1;
// 	      p.y() += 1;
// 	    }
// 	  // Since P and U' defines the slope of the line, we get:
// 	  m_b = p.x() - m_up.x();
// 	  m_a = p.y() - m_up.y();
// 	  // Since U' belongs to the upper leaning line, we get:
// 	  m_mu = m_a * m_up.x() - m_b * m_up.y();
// 	  // L and U' are not changed.
// 
// 	  // We recompute L' from L and the length of the discrete segment.
// 	  int k = ( m_cp_n.x() - m_l.x() ) / m_b;
// 	  m_lp.x() = m_l.x() + k * m_b;
// 	  m_lp.y() = m_l.y() + k * m_a;
// 
// 	  // We recompute U from U' and the length of the discrete segment.
// 	  int l = ( p.x() - m_c_n.x() ) / m_b;
// 	  m_u.x() = m_up.x() - ( l - 1 ) * m_b;
// 	  m_u.y() = m_up.y() - ( l - 1 ) * m_a;
// 	  
// 	  ASSERT_C4CSegment( ( k >= 0 ) && ( l >= 1 ) );
// 	}
//       else
// 	{ // Case 2.b: U' != U
// 	  // => M did not change the slope of the line, was only on the lower
// 	  // leaning line.
// 	  // L' <- L' - (b,a)
// 	  m_lp.x() -= m_b;
// 	  m_lp.y() -= m_a;
// 	}
//     } // if ( m == m_lp )
//     
//   /** test que tous les pts d'appuis appartiennent bien au segment initial**/
//  /* if(!belongToSegment(m_u) || !belongToSegment(m_up) || !belongToSegment(m_l) || !belongToSegment(m_lp) )
//   {
//     cout<<"un pt d'appui n'appartient pas au segment original!"<<endl;
//     exit(-1);
//   }*/
//   /**fin du test**/
// 
//   // Otherwise, nothing to do.
//   return true;
// 
// }

bool
C4CSegment::retractsFront( uint code )
{
  // Cannot retract if reference frame is in the retraction.
  ASSERT_C4CSegment( ( m_cp_n.x() != 1 ) || ( m_cp_n.y() != 0 ) );
  
  Vector2i m( m_cp_n ); // memorizes m.

  // Takes care of state machine.
  switch ( m_state_cp_n + code ) {
  case 0: break;
  case 1: ASSERT_C4CSegment( m_a > 0 );
    m_state_cp_n = 8; 
    --m_cp_n.x();
    break;
  case 2: // state unchanged
    --m_cp_n.x();
    break;
  case 3: ASSERT_C4CSegment( m_a < 0 );
    m_state_cp_n = 4;
    --m_cp_n.x();
    break;
  case 4: break;
  case 5: ASSERT_C4CSegment( m_a < 0 );
    m_state_cp_n = 0;
    ++m_cp_n.y();
    break;
  case 6: ASSERT_C4CSegment( m_a < 0 );
    // state unchanged
    ++m_cp_n.y();
    break;
  case 7: ASSERT_C4CSegment( false ); // Should not arrived
    code = 0;
    break;
  case 8: break;
  case 9: ASSERT_C4CSegment( false ); // Should not arrived
    code = 0;
    break;
  case 10: ASSERT_C4CSegment( m_a > 0 );
    // state unchanged
    --m_cp_n.y();
    break;
  case 11: ASSERT_C4CSegment( m_a > 0 );
    m_state_cp_n = 0;
    --m_cp_n.y();
    break;
  }

  if ( code == 0 ) return false;
  
  // Updates line segment and upper and lower leaning lines depending on the
  // retracted point.

  // Case 0: horizontal line.
  if ( ( m_a == 0 ) && ( m_b == 1 ) )
    {
      ASSERT_C4CSegment( ( m == m_up ) && ( m == m_lp ) );
      --m_up.x();
      --m_lp.x();
      ASSERT_C4CSegment( m_u != m_up );
    }
  // Case 1: M == U'
  else if ( m == m_up )
    { // Two subcases depending on L' == L
      if ( m_lp == m_l )
	{ // Case 1.a: L' == L
	  // => M Changed the slope of the line.
	  
	  // P "is" a lower leaning point.
	  // a > 0 => P = M + (1,-1)
	  // a < 0 => P = M + (-1,-1)
	  Vector2i p( m );
	  if ( m_a > 0 )
	    { 
	      p.x() += 1;
	      p.y() -= 1;
	    }
	  else // ( m_a < 0 )
	    { 
	      p.x() -= 1;
	      p.y() -= 1;
	    }
	  // Since P and L' defines the slope of the line, we get:
	  m_b = p.x() - m_lp.x();
	  m_a = p.y() - m_lp.y();
	  // Since U belongs to the upper leaning line, we get:
	  m_mu = m_a * m_u.x() - m_b * m_u.y();
	  // L' and U are not changed.
	  
	  //modif à partir d'ici
	  if ( m_a == 0 ) 
	    {
	      m_l = m_u;
	      m_up = m_lp;
	      ASSERT_C4CSegment( ( m_u == m_c_n ) && ( m_lp == m_cp_n ) );
	    }
	  else if ( m_a > 0 )
	    {

	  // We recompute U' from U and the length of the discrete segment.
	  int k = ( m_cp_n.y() - m_u.y() ) / m_a;
	  m_up.x() = m_u.x() + k * m_b;
	  m_up.y() = m_u.y() + k * m_a;

	  // We recompute L from L' and the length of the discrete segment.
	  int l = ( p.y() - m_c_n.y() ) / m_a;
	  m_l.x() = m_lp.x() - ( l - 1 ) * m_b;
	  m_l.y() = m_lp.y() - ( l - 1 ) * m_a;
	  
	  ASSERT_C4CSegment( ( k >= 0 ) && ( l >= 1 ) );
	     }
	   else // case ( m_a < 0 )
	    {
	    	  // We recompute U' from U and the length of the discrete segment.
	  int k = ( m_cp_n.x() - m_u.x() ) / m_b;
	  m_up.x() = m_u.x() + k * m_b;
	  m_up.y() = m_u.y() + k * m_a;

	  // We recompute L from L' and the length of the discrete segment.
	  int l = ( p.x() - m_c_n.x() ) / m_b;
	  m_l.x() = m_lp.x() - ( l - 1 ) * m_b;
	  m_l.y() = m_lp.y() - ( l - 1 ) * m_a;
	  
	  
	  ASSERT_C4CSegment( ( k >= 0 ) && ( l >= 1 ) );   
	    }
	}
      else
	{ // Case 1.b: L' != L
	  // => M did not change the slope of the line, was only on the upper
	  // leaning line.
	  // U' <- U' - (b,a)
	  m_up.x() -= m_b;
	  m_up.y() -= m_a;
	}
    } // if ( m == m_up )
  // Case 2: M == L'
  else if ( m == m_lp )
    { // Two subcases depending on U' == U
      if ( m_up == m_u )
	{ // Case 2.a: U' == U 
	  // => M Changed the slope of the line.
	  
	  // P "is" an upper leaning point.
	  // a > 0 => P = M + (-1,1)
	  // a < 0 => P = M + (1,1)
	  Vector2i p( m );
	  if ( m_a > 0 )
	    { 
	      p.x() -= 1;
	      p.y() += 1;
	    }
	  else // ( m_a < 0 )
	    { 
	      p.x() += 1;
	      p.y() += 1;
	    }
	  // Since P and U' defines the slope of the line, we get:
	  m_b = p.x() - m_up.x();
	  m_a = p.y() - m_up.y();
	  // Since U' belongs to the upper leaning line, we get:
	  m_mu = m_a * m_up.x() - m_b * m_up.y();
	  // L and U' are not changed.
	  
	  //modif à partir de là
	   if ( m_a == 0 ) 
	    {
// 	      m_l = m_u;
// 	      m_up = m_lp;
	      m_u = m_l;
	      m_lp = m_up;
	      ASSERT_C4CSegment( ( m_l == m_c_n ) && ( m_up == m_cp_n ) );
	    }
	  else if ( m_a > 0 )
	    {
	  // We recompute L' from L and the length of the discrete segment.
	  int k = ( m_cp_n.x() - m_l.x() ) / m_b;
	  m_lp.x() = m_l.x() + k * m_b;
	  m_lp.y() = m_l.y() + k * m_a;

	  // We recompute U from U' and the length of the discrete segment.
	  int l = ( p.x() - m_c_n.x() ) / m_b;
	  m_u.x() = m_up.x() - ( l - 1 ) * m_b;
	  m_u.y() = m_up.y() - ( l - 1 ) * m_a;
	  
	  ASSERT_C4CSegment( ( k >= 0 ) && ( l >= 1 ) );
	    }
	  else // case ( m_a < 0 )
	    {
	    	  // We recompute L' from L and the length of the discrete segment.
	  int k = ( m_cp_n.y() - m_l.y() ) / m_a;
	  m_lp.x() = m_l.x() + k * m_b;
	  m_lp.y() = m_l.y() + k * m_a;

	  // We recompute U from U' and the length of the discrete segment.
	  int l = ( p.y() - m_c_n.y() ) / m_a;
	  m_u.x() = m_up.x() - ( l - 1 ) * m_b;
	  m_u.y() = m_up.y() - ( l - 1 ) * m_a;
	  
	  
	  ASSERT_C4CSegment( ( k >= 0 ) && ( l >= 1 ) );
	    }
	}
      else
	{ // Case 2.b: U' != U
	  // => M did not change the slope of the line, was only on the lower
	  // leaning line.
	  // L' <- L' - (b,a)
	  m_lp.x() -= m_b;
	  m_lp.y() -= m_a;
	}
    } // if ( m == m_lp )

  // Otherwise, nothing to do.
  return true;

}


/**
 * Tries to retract the discrete line segment to the "back"
 * (negatively along the contour).
 *
 * @param code the shape of the retraction going from the "back"
 * extremity: 0: no move, 1: turn left (toward interior), 2: move
 * straight ahead (no turn), 3: turn right (toward exterior).
 *
 * @return 'true' if this retraction was valid and the discrete line
 * segment has been updated, otherwise 'false' and the discrete line
 * segment is unchanged.
 *
 * @pre m_c_n != (0,0)
 */
bool
C4CSegment::retractsBack( uint code )
{
  // Cannot retract if reference frame is in the retraction.
  ASSERT_C4CSegment( ( m_c_n.x() != 0 ) || ( m_c_n.y() != 0 ) );
  
  Vector2i m( m_c_n ); // memorizes m.

  // Takes care of state machine.
  switch ( m_state_c_n + code ) {
  case 0: break;
  case 1: ASSERT_C4CSegment( m_a < 0 );
    m_state_c_n = 8; 
    ++m_c_n.x();
    break;
  case 2: // state unchanged
    ++m_c_n.x();
    break;
  case 3: ASSERT_C4CSegment( m_a > 0 );
    m_state_c_n = 4;
    ++m_c_n.x();
    break;
  case 4: break;
  case 5: ASSERT_C4CSegment( m_a > 0 );
    m_state_c_n = 0;
    ++m_c_n.y();
    break;
  case 6: ASSERT_C4CSegment( m_a > 0 );
    // state unchanged
    ++m_c_n.y();
    break;
  case 7: ASSERT_C4CSegment( false ); // Should not arrived
    code = 0;
    break;
  case 8: break;
  case 9: ASSERT_C4CSegment( false ); // Should not arrived
    code = 0;
    break;
  case 10: ASSERT_C4CSegment( m_a < 0 );
    // state unchanged
    --m_c_n.y();
    break;
  case 11: ASSERT_C4CSegment( m_a < 0 );
    m_state_c_n = 0;
    --m_c_n.y();
    break;
  }

  if ( code == 0 ) return false;
  
  // Updates line segment and upper and lower leaning lines depending on the
  // retracted point.

  // Case 0: horizontal line.
  if ( ( m_a == 0 ) && ( m_b == 1 ) )
    {
      ASSERT_C4CSegment( ( m == m_u ) && ( m == m_l ) );
      ++m_u.x();
      ++m_l.x();
      ASSERT_C4CSegment( m_u != m_up );
    }
  // Case 1: M == U
  else if ( m == m_u )
    { // Two subcases depending on L' == L
      if ( m_lp == m_l )
	{ // Case 1.a: L' == L
	  // => M Changed the slope of the line.
	  
	  // P "is" a lower leaning point.
	  // a > 0 => P = M + (1,-1)
	  // a < 0 => P = M + (-1,-1)
	  Vector2i p( m );
	  if ( m_a > 0 )
	    { 
	      p.x() += 1;
	      p.y() -= 1;
	    }
	  else // ( m_a < 0 )
	    { 
	      p.x() -= 1;
	      p.y() -= 1;
	    }
	  // Since P and L defines the slope of the line, we get:
	  m_b = m_l.x() - p.x();
	  m_a = m_l.y() - p.y();
	  // Since U' belongs to the upper leaning line, we get:
	  m_mu = m_a * m_up.x() - m_b * m_up.y();

	  // L and U' are not changed.

	  // JOL 2004/09/07
	  if ( m_a == 0 ) 
	    {
	      m_u = m_l;
	      m_lp = m_up;
	      ASSERT_C4CSegment( ( m_u == m_c_n ) && ( m_lp == m_cp_n ) );
	    }
	  else if ( m_a > 0 )
	    {
	      // We recompute U from U' and the length of the discrete segment.
	      int k = ( m_up.x() - m_c_n.x() ) / m_b;
	      m_u.x() = m_up.x() - k * m_b;
	      m_u.y() = m_up.y() - k * m_a;
	      
	      // We recompute L' from L and the length of the discrete segment.
	      // JOL 2004/09/07 : int l = ( m_cp_n.x() - p.x() ) / m_b;
	      int l = ( m_cp_n.x() - m_c_n.x() ) / m_b;
	      m_lp.x() = m_l.x() + ( l - 1 ) * m_b;
	      m_lp.y() = m_l.y() + ( l - 1 ) * m_a;
	      
	      ASSERT_C4CSegment( ( k >= 0 ) && ( l >= 1 ) );
	    }
	  else // case ( m_a < 0 )
	    {
	      // We recompute U from U' and the length of the discrete segment.
	      int k = ( m_up.y() - m_c_n.y() ) / m_a;
	      m_u.x() = m_up.x() - k * m_b;
	      m_u.y() = m_up.y() - k * m_a;
	      
	      // We recompute L' from L and the length of the discrete segment.
	      // JOL 2004/09/07 : int l = ( m_cp_n.x() - p.x() ) / m_b;
	      int l = ( m_cp_n.y() - m_c_n.y() ) / m_a;
	      m_lp.x() = m_l.x() + ( l - 1 ) * m_b;
	      m_lp.y() = m_l.y() + ( l - 1 ) * m_a;
	      
	      ASSERT_C4CSegment( ( k >= 0 ) && ( l >= 1 ) );
	    }
	}
      else
	{ // Case 1.b: L' != L
	  // => M did not change the slope of the line, was only on the upper
	  // leaning line.
	  // U <- U + (b,a)
	  m_u.x() += m_b;
	  m_u.y() += m_a;
	}
    } // if ( m == m_u )
  // Case 2: M == L
  else if ( m == m_l )
    { // Two subcases depending on U' == U
      if ( m_up == m_u )
	{ // Case 2.a: U' == U 
	  // => M Changed the slope of the line.
	  
	  // P "is" an upper leaning point.
	  // a > 0 => P = M + (-1,1)
	  // a < 0 => P = M + (1,1)
	  Vector2i p( m );
	  if ( m_a > 0 )
	    { 
	      p.x() -= 1;
	      p.y() += 1;
	    }
	  else // ( m_a < 0 )
	    { 
	      p.x() += 1;
	      p.y() += 1;
	    }
	  // Since P and U defines the slope of the line, we get:
	  m_b = m_u.x() - p.x();
	  m_a = m_u.y() - p.y();
	  // Since U belongs to the upper leaning line, we get:
	  m_mu = m_a * m_u.x() - m_b * m_u.y();
	  // L' and U are not changed.

	  // JOL 2004/09/07
	  if ( m_a == 0 ) 
	    {
	      m_l = m_u;
	      m_up = m_lp;
	      ASSERT_C4CSegment( ( m_l == m_c_n ) && ( m_up == m_cp_n ) );
	    }
	  else if ( m_a > 0 )
	    {
	      // We recompute L from L' and the length of the discrete segment.
	      int k = ( m_lp.y() - m_c_n.y() ) / m_a;
	      m_l.x() = m_lp.x() - k * m_b;
	      m_l.y() = m_lp.y() - k * m_a;
	      
	      // We recompute U' from U and the length of the discrete segment.
	      // JOL 2004/09/07 : int l = ( m_cp_n.x() - p.x() ) / m_b;
	      int l = ( m_cp_n.y() - m_c_n.y() ) / m_a;
	      m_up.x() = m_u.x() + ( l - 1 ) * m_b;
	      m_up.y() = m_u.y() + ( l - 1 ) * m_a;
	      
	      ASSERT_C4CSegment( ( k >= 0 ) && ( l >= 1 ) );
	    }
	  else // case ( m_a < 0 )
	    {
	      // We recompute L from L' and the length of the discrete segment.
	      int k = ( m_lp.x() - m_c_n.x() ) / m_b;
	      m_l.x() = m_lp.x() - k * m_b;
	      m_l.y() = m_lp.y() - k * m_a;
	      
	      // We recompute U' from U and the length of the discrete segment.
	      // JOL 2004/09/07 : int l = ( m_cp_n.x() - p.x() ) / m_b;
	      int l = ( m_cp_n.x() - m_c_n.x() ) / m_b;
	      m_up.x() = m_u.x() + ( l - 1 ) * m_b;
	      m_up.y() = m_u.y() + ( l - 1 ) * m_a;
	      
	      ASSERT_C4CSegment( ( k >= 0 ) && ( l >= 1 ) );
	    }
	}
      else
	{ // Case 2.b: U' != U
	  // => M did not change the slope of the line, was only on the lower
	  // leaning line.
	  // L <- L + (b,a)
	  m_l.x() += m_b;
	  m_l.y() += m_a;
	}
    } // if ( m == m_l )

//   cerr << "retractsBack: (" << m_a << "," << m_b << "," << m_mu << ")"
//        << " Cn=" << m_c_n.x() << "," << m_c_n.y()
//        << " C'n=" << m_cp_n.x() << "," << m_cp_n.y()
//        << endl
//        << " U=" << m_u.x() << "," << m_u.y()
//        << " U'=" << m_up.x() << "," << m_up.y()
//        << " L=" << m_l.x() << "," << m_l.y()
//        << " L'=" << m_lp.x() << "," << m_lp.y() 
//        << endl;

  // Otherwise, nothing to do.
  return true;
}

/**
 * Tries to extend the discrete line segment both to the "back"
 * (negatively along the contour) and to the "front" (positively
 * along the contour).
 *
 * @param code_back 0: no move, 1: turn left (toward interior), 2:
 * move straight ahead (no turn), 3: turn right (toward exterior).
 *
 * @param code_front 0: no move, 1: turn right (toward interior), 2:
 * move straight ahead (no turn), 3: turn left (toward exterior).
 *
 * @return 'true' if this symmetric extension was valid and the
 * discrete line segment has been updated, otherwise 'false' and the
 * discrete line segment is unchanged.
 */
bool
C4CSegment::extends( uint code_back, uint code_front )
{
  C4CSegment copy = *this;
  if ( extendsFront( code_front ) == 0 )
    {
      if ( extendsBack( code_back ) == 0 )
	return true;
      *this = copy;
    }
  return false;
}

/**
 * Tries to extend the discrete line segment both to the "back"
 * (negatively along the contour) and to the "front" (positively
 * along the contour).
 * Be careful, faster than 'extends' but the 'C4CSegment' is not valid for
 * further extension/suppression.
 *
 * @param code_back 0: no move, 1: turn left (toward interior), 2:
 * move straight ahead (no turn), 3: turn right (toward exterior).
 *
 * @param code_front 0: no move, 1: turn right (toward interior), 2:
 * move straight ahead (no turn), 3: turn left (toward exterior).
 *
 * @return 'true' if this symmetric extension was valid and the
 * discrete line segment has been updated, otherwise 'false' and the
 * discrete line segment is unchanged.
 */
bool
C4CSegment::extendsUnsecure( uint code_back, uint code_front )
{
  int coef_a = m_a;
  int coef_b = m_b;
  int coef_mu = m_mu;
  
  if ( extendsFront( code_front ) == 0 )
    {
      if ( extendsBack( code_back ) == 0 )
	return true;
      m_a = coef_a;
      m_b = coef_b;
      m_mu = coef_mu;
    }
  return false;
}



/**
 * Tries to displace the reference frame one step forward. Useful
 * for incremental computation of tangents over a contour. The code
 * indicates how the new center is located with respect to the
 * preceding one. Note that the reference frame can be inversed.
 *
 * @param code 0: no move (! => does nothing), 1: turn right
 * (toward interior), 2: move straight ahead (no turn), 3: turn left
 * (toward exterior).
 *
 * @return 'true' in any case, except when the discrete line segment
 * is reduced to two points. Does nothing in this case and return
 * 'false'.
 */
bool
C4CSegment::slidesForward( uint code )
{
  // Test if current discrete line segment is reduced to two points.
  if ( ( m_cp_n.x() - m_c_n.x() == 1 )
       && ( m_cp_n.y() - m_c_n.y() == 0 ) )
    return false;

  // Tests the different cases.
  switch ( code ) {
  case 0: break;
  case 1: // moving forward, turns to right
    moveAlongX( 1 );
    turnRight();
    break;
  case 2: // moving forward, goes ahead
    moveAlongX( 1 );
    break;
  case 3: // moving forward, turns to left
    moveAlongX( 1 );
    turnLeft();
    break;
  }
  return true;
}


/**
 * Moves a vector one step forward along the contour. Useful
 * for incremental computation of tangents over a contour. The code
 * indicates how the new center is located with respect to the
 * preceding one. Note that the reference frame can be inversed.
 *
 * @param code 0: no move (! => does nothing), 1: turn right
 * (toward interior), 2: move straight ahead (no turn), 3: turn left
 * (toward exterior).
 * @param v any vector.
 */
void
C4CSegment::slidesForward( uint code, Vector2i & v )
{
  // Tests the different cases.
  switch ( code ) {
  case 0: 
  case 2: break;
  case 1: // moving forward, turns to right
    v.turnRight();
    break;
  case 3: // moving forward, turns to left
    v.turnLeft();
    break;
  }
}



/**
 * Tries to displace the reference frame one step backward. Useful
 * for incremental computation of tangents over a contour. The code
 * indicates how the new center is located with respect to the
 * preceding one. Note that the reference frame can be inversed.
 *
 * @param code 0: no move (! => does nothing), 1: turn left
 * (toward interior), 2: move straight ahead (no turn), 3: turn right
 * (toward exterior).
 *
 * @return 'true' in any case, except when the discrete line segment
 * is reduced to two points. Does nothing in this case and return
 * 'false'.
 */
bool
C4CSegment::slidesBackward( uint code )
{
  // Test if current discrete line segment is reduced to two points.
  if ( ( m_cp_n.x() - m_c_n.x() == 1 )
       && ( m_cp_n.y() - m_c_n.y() == 0 ) )
    return false;

  // Tests the different cases.
  switch ( code ) {
  case 0: break;
  case 1: // moving backward, turns to left
    turnLeft();
    moveAlongX( -1 );
    break;
  case 2: // moving backward, goes ahead
    moveAlongX( -1 );
    break;
  case 3: // moving backward, turns to right
    turnRight();
    moveAlongX( -1 );
    break;
  }
  return true;
}



/**
 * Moves a vector one step backward along the contour. Useful
 * for incremental computation of tangents over a contour. The code
 * indicates how the new center is located with respect to the
 * preceding one. Note that the reference frame can be inversed.
 *
 * @param code 0: no move (! => does nothing), 1: turn right
 * (toward interior), 2: move straight ahead (no turn), 3: turn left
 * (toward exterior).
 * @param v any vector.
 */
void
C4CSegment::slidesBackward( uint code, Vector2i & v )
{
  // Tests the different cases.
  switch ( code ) {
  case 0:
  case 2: break;
  case 1: // moving backward, turns to left
    v.turnLeft();
    break;
  case 3: // moving backward, turns to right
    v.turnRight();
    break;
  }
}


 
///////////////////////////////////////////////////////////////////////////////
// ------------------------- Reference frame services -----------------------

/**
 * Turn reference frame to the right (clockwise).  /!\ There are
 * some restrictions in changing a reference frame of a 'C4CSegment'.
 */
void
C4CSegment::turnRight()
{
  // (x',y') = (-y,x)
  // @pre m_a < 0
  ASSERT_C4CSegment( m_a < 0 );

  // Turns the line.
  int a = m_a;
  m_a = m_b;
  m_b = -a;

  // Turns the points
  m_up.turnRight();
  m_lp.turnRight();
  m_u.turnRight();
  m_l.turnRight();
  m_cp_n.turnRight();
  m_c_n.turnRight();

  // 8 -> 0, 0 -> 4, 4 incorrect !
  ASSERT_C4CSegment( m_state_c_n != 4 );
  m_state_c_n = ( m_state_c_n + 4 ) % 12;

  // 4 -> 0, 0 -> 8, 8 incorrect !
  ASSERT_C4CSegment( m_state_cp_n != 8 );
  m_state_cp_n = ( m_state_cp_n + 8 ) % 12;
  
}


/**
 * Turn reference frame to the left (counterclokckwise). /!\ There
 * are some restrictions in changing a reference frame of a
 * 'C4CSegment'.
 */
void
C4CSegment::turnLeft()
{
  // (x',y') = (y,-x)
  // @pre m_a > 0
  ASSERT_C4CSegment( m_a > 0 );

  // Turns the line.
  int a = m_a;
  m_a = -m_b;
  m_b = a;

  // Turns the points
  m_up.turnLeft();
  m_lp.turnLeft();
  m_u.turnLeft();
  m_l.turnLeft();
  m_cp_n.turnLeft();
  m_c_n.turnLeft();

  // 4 -> 0, 0 -> 8, 8 incorrect !
  ASSERT_C4CSegment( m_state_c_n != 8 );
  m_state_c_n = ( m_state_c_n + 8 ) % 12;

  // 8 -> 0, 0 -> 4, 4 incorrect !
  ASSERT_C4CSegment( m_state_cp_n != 4 );
  m_state_cp_n = ( m_state_cp_n + 4 ) % 12;

}


/**
 * Move reference frame along X. /!\ There
 * are some restrictions in changing a reference frame of a
 * 'C4CSegment'.
 * @param dx the variation in X.
 * @post m_mu <= 0
 */
void
C4CSegment::moveAlongX( int dx )
{
  m_mu -= m_a * dx;
  ASSERT_C4CSegment( m_mu <= 0 );

  m_up.x() -= dx;
  m_lp.x() -= dx;
  m_u.x() -= dx;
  m_l.x() -= dx;
  m_cp_n.x() -= dx;
  m_c_n.x() -= dx;
  
}

/**
 * Move reference frame along Y. /!\ There
 * are some restrictions in changing a reference frame of a
 * 'C4CSegment'.
 * @param dy the variation in Y.
 * @post m_mu <= 0
 */
void
C4CSegment::moveAlongY( int dy )
{
  m_mu += m_b * dy;
  ASSERT_C4CSegment( m_mu <= 0 );

  m_up.y() -= dy;
  m_lp.y() -= dy;
  m_u.y() -= dy;
  m_l.y() -= dy;
  m_cp_n.y() -= dy;
  m_c_n.y() -= dy;

}




///////////////////////////////////////////////////////////////////////////////
// ------------------------- Other geometric services -----------------------
///////////////////////////////////////////////////////////////////////////////



/**
 * A discrete segment represents the digitization of some true
 * contour that is approximately straight.  For any [x] within the
 * bounds of the segment, there is a given probability function [y]
 * that estimates the probability that the point (x,y) lies on the true
 * contour.
 *
 * @param x a coordinate in the local frame (between 'c_n' and 'cp_n').
 * @return the probability density of the described function
 */
PWFAffineFunction
C4CSegment::yDPFct( float x ) const
{
  float alpha = ( m_cp_n.x() - x ) / ( m_cp_n.x() - m_c_n.x() );
  
  float e_div_2 = 0.5f / m_b;
  int mup = m_mu + ( m_a >= 0 ? m_a : -m_a ) + m_b - 1;
  float y_mid = ( 2*m_a*x - m_mu - mup ) * e_div_2;

  // alpha tunes the density probability function between a perfect uniform
  // law, a trapezoidal law, or a triangular law.
  return PWFAffineFunction::trapezoidalDP( y_mid - e_div_2, 
					   y_mid + e_div_2,
					   alpha );
}


/**
 * A discrete segment represents the digitization of some true
 * contour that is approximately straight. For any [x] within the
 * bounds of the segment, there is a given probability function [y]
 * that estimates the probability that the point (x,y) lies on the true
 * contour. The return value is the mean of this probability function.
 *
 * @param x a coordinate in the local frame (between 'c_n' and 'cp_n').
 * @return the mean Y fct for this [x].
 */
float
C4CSegment::meanY( float x ) const
{
  return yDPFct( x ).data().centroid();
}


/**
 * A discrete segment represents the digitization of some true
 * contour that is approximately straight. For any [y] within the
 * bounds of the segment, there is a given probability function [x]
 * that estimates the probability that the point (x,y) lies on the true
 * contour. The return value is the mean of this probability function.
 *
 * @param y a coordinate in the local frame (between 'c_n' and 'cp_n').
 * @return the mean X fct for this [y].
 * @TODO It is currently an approximation.
 */
float
C4CSegment::meanX( float y ) const
{
  if ( m_a == 0 ) return 0.5f;
  int mup = m_mu + ( m_a >= 0 ? m_a : -m_a ) + m_b - 1;
  // cout << "[a=" << m_a << " b=" << m_b << " mu=" << m_mu << " nu=" << mup << "]";
  
  float est_x = ( m_mu + mup + 2*m_b*y ) / (2*m_a);
  float est_y = meanY( est_x );
//   cout << "[meanX:" << y << "," << est_x << "," << est_y << " segm="
//        << *this << "]" << endl;
  return est_x + m_b*( y - est_y ) / m_a;
}




/**
 * Projects the point [m] onto the average straight line (ie (mu+nu)/2).
 * @param m any point expressed in the local reference frame (may not be part of the segment).
 * @return the projected point.
 */
Vector2D
ImaGene::C4CSegment::project( const Vector2i & m ) const
{
  float alpha = ( m_mu + m_mu + ( m_a >= 0 ? m_a : -m_a ) + m_b - 1 ) / 2.0f;
  float d2 = (float)( m_a * m_a + m_b * m_b );
  float s = m_b * m.x() + m_a * m.y();
  float xp = ( m_b * s + m_a * alpha ) / d2;
  float yp = ( m_a * s - m_b * alpha ) / d2;
  return Vector2D( xp, yp );
}

/**
 * Projects the point [m] onto the straight line whose points have
 * remainder [r].
 *
 * @param m any point expressed in the local reference frame (may not
 * be part of the segment).
 *
 * @param r the remainder (may not be an integer).
 * @return the projected point.
 */
Vector2D
ImaGene::C4CSegment::project( const Vector2i & m, float r ) const
{
  //  float alpha = ( m_mu + m_mu + ( m_a >= 0 ? m_a : -m_a ) + m_b - 1 ) / 2.0f;
  float d2 = (float)( m_a * m_a + m_b * m_b );
  float s = m_b * m.x() + m_a * m.y();
  float xp = ( m_b * s + m_a * r ) / d2;
  float yp = ( m_a * s - m_b * r ) / d2;
  return Vector2D( xp, yp );
}

/**
 * Projects the point [m] onto the straight line going through point [p].
 *
 * @param m any point expressed in the local reference frame (may not
 * be part of the segment).
 *
 * @param p any point expressed in the local reference frame (may not
 * be part of the segment).
 *
 * @return the projected point.
 */
Vector2D
ImaGene::C4CSegment::project( const Vector2i & m, const Vector2i & p ) const
{
  float r = m_a * p.x() - m_b * p.y();
  float d2 = (float)( m_a * m_a + m_b * m_b );
  float s = m_b * m.x() + m_a * m.y();
  float xp = ( m_b * s + m_a * r ) / d2;
  float yp = ( m_a * s - m_b * r ) / d2;
  return Vector2D( xp, yp );
}


/**
 * Defined as: norm( project(cp_n) - project(c_n) )
 * @return the projected length of the segment.
 * @see projectRegularly
 */
float 
ImaGene::C4CSegment::projectedSegmentLength() const
{
  Vector2D v = project( m_cp_n );
  Vector2D u = project( m_c_n );
  v -= u;
  return sqrt( v.x() * v.x() + v.y() * v.y() );
}


/**
 * Defined as: norm1( m - c_n ) / norm1( cp_n - c_n )
 * @param m any point of the segment.
 * @return the eccentricity of m
 * @see projectRegularly
 */
float 
ImaGene::C4CSegment::eccentricity( const Vector2i & m ) const
{
  Vector2i u1( m );
  Vector2i u2( m_cp_n );
  u1 -= m_c_n;
  u2 -= m_c_n;
  return (float) u1.norm1() / (float) u2.norm1();
}


/**
 * Projects the point [m] onto the average straight line (ie
 * (mu+nu)/2). The projection is regular in the sense that any two
 * successive point of the segment are projected at equal distance.
 * Uses the eccentricity of the point and the length of the
 * projected segment.
 *
 * @param m any point of the segment.
 * @return the projected point.
 * @see projectedSegmentLength,eccentricity
 */
Vector2D 
ImaGene::C4CSegment::projectRegularly( const Vector2i & m ) const
{
  return eccentricityPoint( eccentricity( m ) );
}

/**
 * Defined as: norm1( m - c_n ) / norm1( cp_n - c_n )
 * @param m any point of the segment.
 * @return the eccentricity of m
 * @see projectRegularly
 */
float 
ImaGene::C4CSegment::eccentricity( const Vector2D & m ) const
{
  // Vector2D u1( m );
  float x1 = m.x() - m_c_n.x();
  float y1 = m.y() - m_c_n.y();
  Vector2i u2( m_cp_n );
  u2 -= m_c_n;
  return ( fabs( x1 ) + fabs( y1 ) ) / (float) u2.norm1();
}


/**
 * Projects the point [m] onto the average straight line (ie
 * (mu+nu)/2). The projection is regular in the sense that any two
 * successive point of the segment are projected at equal distance.
 * Uses the eccentricity of the point and the length of the
 * projected segment.
 *
 * @param m any point of the segment.
 * @return the projected point.
 * @see projectedSegmentLength,eccentricity
 */
Vector2D 
ImaGene::C4CSegment::projectRegularly( const Vector2D & m ) const
{
  return eccentricityPoint( eccentricity( m ) );
}

/**
 * @param ecc an eccentricity characterising a point on the segment.
 * @return the corresponding regularly projected point.
 * @see projectRegularly,projectedSegmentLength,eccentricity
 */
Vector2D
ImaGene::C4CSegment::eccentricityPoint( float ecc ) const
{
  Vector2D u = project( m_c_n );
  float l = ecc * projectedSegmentLength();
  float d = sqrt( (float)( m_a * m_a + m_b * m_b ) );
  float cost = m_b / d;
  float sint = m_a / d;
  u.x() += l*cost;
  u.y() += l*sint;
  return u;
}



/**
 * This minimum slope is obtained by assuming the digitization is
 * the same for any straight line in-between the leaning points.
 *
 * @return the minimum slope of the straight lines that are digitized as this segment (the dx is in 'x()' and the dy is in 'y()').
 */
ImaGene::Vector2i
ImaGene::C4CSegment::minSlopeLeaningPoints() const
{
  if ( -m_a > m_b )
    {
      int dyl = m_lp.y() - m_l.y();
      int dyu = m_up.y() - m_u.y();
      int dym = dyl <= dyu ? -dyl : -dyu;
      return Vector2i( m_b * dym + m_a, m_a * dym );
    }
  else if ( m_a > m_b )
    {
      int dyl = m_lp.y() - m_l.y();
      int dyu = m_up.y() - m_u.y();
      int dym = dyl >= dyu ? dyl : dyu;
      return Vector2i( m_b * dym + m_a, m_a * dym );
    }
  else
    {
      int dxl = m_lp.x() - m_l.x();
      int dxu = m_up.x() - m_u.x();
      int dxm = dxl >= dxu ? dxl : dxu;
      return Vector2i( m_b * dxm, m_a * dxm - m_b );
    }
}


/**
 * This maximum slope is obtained by assuming the digitization is
 * the same for any straight line in-between the leaning points.
 *
 * @return the maximum slope of the straight lines that are digitized as this segment (the dx is in 'x()' and the dy is in 'y()').
 */
ImaGene::Vector2i
ImaGene::C4CSegment::maxSlopeLeaningPoints() const
{
  if ( -m_a > m_b )
    {
      int dyl = m_lp.y() - m_l.y();
      int dyu = m_up.y() - m_u.y();
      int dym = dyl <= dyu ? -dyl : -dyu;
      return Vector2i( m_b * dym - m_a, m_a * dym );
    }
  else if ( m_a > m_b )
    {
      int dyl = m_lp.y() - m_l.y();
      int dyu = m_up.y() - m_u.y();
      int dym = dyl >= dyu ? dyl : dyu;
      return Vector2i( m_b * dym - m_a, m_a * dym );
    }
  else
    {
      int dxl = m_lp.x() - m_l.x();
      int dxu = m_up.x() - m_u.x();
      int dxm = dxl >= dxu ? dxl : dxu;
      return Vector2i( m_b * dxm, m_a * dxm + m_b );
    }
}

/**
 * This minimum slope is obtained by assuming the digitization is
 * the same for any straight line in-between the end points. The
 * computation is derived from the dual line parameter space.
 *
 * @return the minimum slope of the straight lines that are digitized as this segment (the dx is in 'x()' and the dy is in 'y()').
 */
ImaGene::Vector2i
ImaGene::C4CSegment::minSlopeDual() const
{
  if ( m_a == 0 )
    return Vector2i( m_up.x() - m_l.x(),
 		     m_up.y() - m_l.y() - 1 );
  else if ( m_a > 0 )
    return Vector2i( m_up.x() - m_l.x() + 1,
		     m_up.y() - m_l.y() - 1 );
  else // (m_a < 0 )
    return Vector2i( m_up.x() - m_l.x() - 1,
 		     m_up.y() - m_l.y() - 1 );
}


/**
 * This maximum slope is obtained by assuming the digitization is
 * the same for any straight line in-between the end points. The
 * computation is derived from the dual line parameter space.
 *
 * @return the maximum slope of the straight lines that are digitized as this segment (the dx is in 'x()' and the dy is in 'y()').
 */
ImaGene::Vector2i
ImaGene::C4CSegment::maxSlopeDual() const
{
  if ( m_a == 0 )
    return Vector2i( m_lp.x() - m_u.x(),
 		     m_lp.y() - m_u.y() + 1 );
  else if ( m_a > 0 )
    return Vector2i( m_lp.x() - m_u.x() - 1,
 		     m_lp.y() - m_u.y() + 1 );
  else // (m_a < 0 )
    return Vector2i( m_lp.x() - m_u.x() + 1,
 		     m_lp.y() - m_u.y() + 1 );
}


ImaGene::Vector2D
ImaGene::C4CSegment::rightmostDualVertex() const
{
  ImaGene::Vector2i slope;
  int mu;
  
  slope = maxSlopeDual();
  mu = slope.x() * m_u.y() - slope.y() * m_u.x();
  
  if(abs(m_a)<=abs(m_b))
    return Vector2D((float)slope.y()/(float) slope.x(), (float)mu/(float)slope.x());
  else
    return Vector2D((float)slope.x()/(float) slope.y(), (float)mu/(float)slope.y());
}

ImaGene::Vector2D
ImaGene::C4CSegment::leftmostDualVertex() const
{
  ImaGene::Vector2i slope;
  int mu;
  
  slope = minSlopeDual();
  
  mu = slope.x() * m_up.y() - slope.y() * m_up.x();
  
  
  if(abs(m_a)<=abs(m_b))
    return Vector2D((float)slope.y()/(float)slope.x(), (float) mu/(float)slope.x());
  else
    return Vector2D((float)slope.x()/(float)slope.y(), (float) mu/(float)slope.y()); 
  
}

ImaGene::Vector2D
ImaGene::C4CSegment::middleDownDualVertex() const
{
  if(abs(m_a)<=abs(m_b))
    return Vector2D((float)m_a/m_b, (float) -m_mu/m_b);
  else
    return  Vector2D((float)m_b/m_a, (float) -m_mu/m_a);
}


ImaGene::Vector2D
ImaGene::C4CSegment::middleUpDualVertex() const
{
  if(abs(m_a)<=abs(m_b))
    return Vector2D((float)m_a/m_b, (float) (-m_mu+1)/m_b);
  else
    return Vector2D((float)m_b/m_a, (float) (-m_mu+1)/m_a); 
}


void
ImaGene::C4CSegment::getPreimage(Vector2D &leftmost, Vector2D &middleUp, Vector2D &rightmost, Vector2D &middleDown) const
{
  leftmost = leftmostDualVertex();
  middleUp = middleDownDualVertex();
  rightmost = rightmostDualVertex();
  middleDown = middleDownDualVertex();
}

/**
* @return the current slope of the tangent vector associated to the segment (b,a) using the Measure of straight lines.
*/
double
ImaGene::C4CSegment::getTangentMeasure() const
{
  ImaGene::Vector2D A, B, C, D;
  ImaGene::MeasureOfStraightLines m;
  
  A = leftmostDualVertex();
  B = middleDownDualVertex();
  C = rightmostDualVertex();
  D = middleUpDualVertex();
  
  std::vector<double> a;
  a.push_back(A.x());
  a.push_back(B.x());
  a.push_back(C.x());
  a.push_back(D.x());
  
  std::vector<double> b;
  b.push_back(A.y());
  b.push_back(B.y());
  b.push_back(C.y());
  b.push_back(D.y());
  
  double C_a;
  
  C_a = m.computeCentroidA(a,b);
  
  if(abs(m_a)<=abs(m_b))
    return C_a;
  else
    return 1/C_a;
  
}

/**
* @return the measure of the preimage of the segment (b,a).
*/
double
ImaGene::C4CSegment::getMeasure() const
{
  ImaGene::Vector2D A, B, C, D;
  ImaGene::MeasureOfStraightLines m;
  
  A = leftmostDualVertex();
  B = middleDownDualVertex();
  C = rightmostDualVertex();
  D = middleUpDualVertex();
  
  std::vector<double> a;
  a.push_back(A.x());
  a.push_back(B.x());
  a.push_back(C.x());
  a.push_back(D.x());
  
  std::vector<double> b;
  b.push_back(A.y());
  b.push_back(B.y());
  b.push_back(C.y());
  b.push_back(D.y());
  
  return m.computeMeasure(a,b);
}



///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
C4CSegment::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[C4CSegment (" << m_a << "," << m_b << "," << m_mu << ")"
	      << " C_n=(" << m_c_n.x() << "," << m_c_n.y() << ")"
	      << " C'_n=(" << m_cp_n.x() << "," << m_cp_n.y() << ")"
	      << " ]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
C4CSegment::OK() const
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

} // namespace ImaGene
