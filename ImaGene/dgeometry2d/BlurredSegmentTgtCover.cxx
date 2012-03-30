///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : BlurredSegmentTgtCover.cxx
//
// Creation : 
//
// Version : 
//
// Author : MS
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
//	2010/8/10 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////
#include "ImaGene/base/Proxy.h"
#include "ImaGene/mathutils/Mathutils.h"
#include "ImaGene/dgeometry2d/C4CGeometry.h"
#include "ImaGene/dgeometry2d/BlurredSegmentTgtCover.h"
#include "ImaGene/base/Vector2i.h"
#include "ImaGene/base/Vector.h"
#include "ImaGene/base/VectorUtils.h"

#include "ImaGene/dgeometry2d/C4CSegment.h"
#include "ImaGene/helper/ShapeHelper.h"
#include "ImaGene/helper/ContourHelper.h"


// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/dgeometry2d/BlurredSegmentTgtCover.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const BlurredSegmentTgtCover_RCS_ID = "@(#)class BlurredSegmentTgtCover definition.";

///////////////////////////////////////////////////////////////////////////////
// class BlurredSegmentTgtCover
///////////////////////////////////////////////////////////////////////////////
// Standard Servics - public:

/**
 * Destructor.
 */
ImaGene::BlurredSegmentTgtCover::~BlurredSegmentTgtCover()
{
}

/**
 * Initialize the Tangential cover.
 */
void
ImaGene::BlurredSegmentTgtCover::init()
{

  //delete &nbpointsCurve;
  //delete &m_Segments;
  
}






/**
 * Initialization by using the FreemanChain c to define the contour.
 *
 *  @param c FreemanChain 
 *  @result initialisation of the contour from the FreemanChain
 *
 */

void 
ImaGene::BlurredSegmentTgtCover::init( const FreemanChain & c )
{
  FreemanChain::const_iterator it = c.begin();
  uint nb = c.chain.size();
  Vector2i fxy( it.get() );
  PointsCurve.push_back( Vector2D( fxy.x() , fxy.y() ) );
  it.nextInLoop();
  for ( uint i = 1; i <= nb; ++i )
    {
      Vector2i nxy( it.get() );
      PointsCurve.push_back( Vector2D( nxy.x() , nxy.y() ) );
      it.nextInLoop();
    }
  nbpointsCurve = PointsCurve.size(); 
}






/**
 * Initialization by using the FreemanChain c to define the contour.
 *
 *  @param c FreemanChain 
 *  @result initialisation of the contour from the FreemanChain
 *
 */

void 
ImaGene::BlurredSegmentTgtCover::init( const vector<Vector2D> & contourVect, bool isClosed )
{
  PointsCurve = contourVect;
  nbpointsCurve = PointsCurve.size(); 
  myIsClosed = isClosed;
}








 





/** MainDiagonal
 * Return the main diagonal width of a convex set
 */
void 
ImaGene::BlurredSegmentTgtCover::MainDiagonal( std::deque<int> & D )
{
  /** Strategie is simple:
   *  Do the same thing than for height but with a correction due to 
   *  the diagonal.
   */
  int size = D.size();
  Vector2D P, Q, R, S;
  int q,s; /* [pq] is an edge and s a vertex */
  bool stop = false; /* Default is false */
  double tmpheight, k, dMinus, dPlus;
  
  /* Take the first edge and find the corresponding point of support */
  P = PointsCurve[ D[ 0 ] ];
  q = 1;
  Q = PointsCurve[ D[ q++ ] ];
  /** To find s, start at q+1 and move until angle between the edge at s 
   *  and the line pq is positive.
   */
  s = q;
  S = PointsCurve[ D[ s++ ] ];
  R = PointsCurve[ D[ s++ ] ];
  while( !stop )
  {
    /* Use vectorial product to ckeck angle */
    stop = ( ( R.x() - S.x() ) * ( Q.y() - P.y() ) - ( R.y() - S.y() ) * ( Q.x() - P.x() ) ) >= 0;
    /* If not stop, move forward */
    if( !stop )
    {
      S = R;
      R = PointsCurve[ D[ s++ ] ];
    }
  }
  /* Initialize the diagonal distance */
  k = ( Q.y() - P.y() ) * S.x() - ( Q.x() - P.x() ) * S.y() - ( Q.y() * P.x() - Q.x() * P.y() );
  dMinus = fabs( k / (  ( Q.x() - P.x() ) + ( Q.y() - P.y() ) ) );
  dPlus = fabs( k / (  ( Q.x() - P.x() ) - ( Q.y() - P.y() ) ) );
  height = ( dPlus <= dMinus ) ? dPlus : dMinus;
  edgePh = P; edgeQh = Q; vertexSh = S;
  /* We have found the first (edge,vertex) pair. Go on until cycling. */
  s--;   /* R is untouch */
  P = Q;
  Q = PointsCurve[ D[ q ] ];
  do
  {
    /** Rotate until one edge touch the border
     *  To do this, replicate the border and compute the sign of the angle
     */
    while( ( Q.x() - P.x() ) * ( R.y() - S.y() )  - ( Q.y() - P.y() ) * ( R.x() - S.x() ) >= 0 )
    {
      S = R;
      s = ( s + 1 ) % size;
      R = PointsCurve[ D[ s ] ];
    }
    /* Update height if necessary */
    k = ( Q.y() - P.y() ) * S.x() - ( Q.x() - P.x() ) * S.y() - ( Q.y() * P.x() - Q.x() * P.y() );
    if( ( Q.y() - P.y() ) - ( Q.x() - P.x() ) == 0 )
      dMinus = HUGE_VAL;
    else
      dMinus = fabs( k / (  ( Q.y() - P.y() ) - ( Q.x() - P.x() ) ) );
    if( ( Q.y() - P.y() ) + ( Q.x() - P.x() ) == 0 )
      dPlus = HUGE_VAL;
    else
      dPlus = fabs( k / (  ( Q.y() - P.y() ) + ( Q.x() - P.x() ) ) );
    /* Compute new diagonal distance / sqrt(2) */
    tmpheight = ( dPlus <= dMinus ) ? dPlus : dMinus;
    if( tmpheight < height )
    {
      height = tmpheight;
      edgePh = P; edgeQh = Q; vertexSh = S;
    }
    /* Go to next point */
    P = Q;
    q = ( q + 1 ) % size;
    Q = PointsCurve[ D[ q ] ];
  }while( q != 0 );
  
  /* Fix width to an innaccessible value. */
  width = HUGE_VAL; 
  //cerr << "(height,width)=(" << height << "," << width << ")" << endl;
  //cerr << "============================================================" << endl;
}


/**
 * IsConvexValid
 * Depending on connexity, return if a convex is valid.
 */
bool 
ImaGene::BlurredSegmentTgtCover::IsConvexValid( std::deque<int> & D, float thick, int connexity, int blur  )
{
  /** Strategy is as follows:
   *  If connexity is 0, use MainDiagonal < k * sqrt(2), for k = 1,2,...
   */
  //if( connexity == 0 )
  MainDiagonal( D );
  return ( height < width ? height : width ) < thick * sqrt( 2.0 );
}


/**
 * InitMelkman
 * Initialize Melkman algorithm.
 */
void 
ImaGene::BlurredSegmentTgtCover::InitMelkman( int P0 , int P1 , int P2 )
{
  D.clear();
  if( IsLeft( P0, P1, P2 ) == 0.0 )
    {
      D.push_back(P2);
    D.push_back(P0);
    D.push_back(P2);
  }
  else
  {
    D.push_front(P2);
    if( IsLeft( P0, P1, P2 ) > 0 )      /* ccw vertices are: 2,0,1,2 */
    {
      D.push_back(P0);
      D.push_back(P1); 
    }
    else                                /* ccw vertices are: 2,1,0,2 */
    {
      D.push_back(P1);
      D.push_back(P0); 
    }  
    D.push_back(P2);
  }   
}

/**
 * Melkman
 * Add a point in a convex using one step of Melkman algorithm.
 */
void 
ImaGene::BlurredSegmentTgtCover::Melkman( int P )
{
  /** incrementally add an exterior vertex to the deque hull
   *  get the rightmore tangent at the deque bottom
   */
  if( D.size() == 3 && IsLeft( D[0], D[1], P ) == 0 && IsLeft( D[1], D[2], P ) == 0 )
  {
    D.pop_front();
    D.push_front( P );
    D.pop_back();
    D.push_back( P );
  }
  /* Test if next vertex is inside the deque Hull */
  else if( IsLeft( D[0], D[1], P ) > 0 && IsLeft( D[int(D.size())-2], D[int(D.size())-1], P ) > 0 )
           return;   /* skip an interior vertex */
  else //if( IsLeft( D[0], D[1], P ) < 0 || IsLeft( D[n-2], D[n-1], P ) < 0 )
  {
     while( IsLeft( D[0], D[1], P ) <= 0 )
       D.pop_front();    /* remove bottom of deque */

     D.push_front( P );  /* insert id at bottom of deque */

     while( IsLeft( D[int(D.size())-2], D[int(D.size())-1], P ) <= 0 )
       D.pop_back();     /* pop top of deque */

     D.push_back( P );   /* push id onto top of deque */
  }
}




/**
 * ExtractSegment
 * For all points of the curve, compute the longest DSS starting at the point.
 * Note: write the result to stdout in form
 * Point (x,y) P (x,y) Q (x,y) S (x,y)
 */

void 
ImaGene::BlurredSegmentTgtCover::ExtractSegment( float thick, int connexity, int blur )
{
  int M, N, Np1, Np2;
  double pPQ, pS, aPQ, bPQ;
  int LastM = -1;
  m_Segments.clear();
  VectDeque.clear();
  D.clear();
  width=0.0;
  height=0.0;
  
  /* Create the tangential cover structure */
  int nPoints = PointsCurve.size();
  /* Start at the beginning of the curve */
  N = 0;
  
  do 
   {
     Segment Segments;
     Segments.nbElementsAfterInitialisation = 0;

     // Special case for end points when the curve is open.
     if(!myIsClosed && (N==(nPoints-1))){
       break;
     }
     if(!myIsClosed && (N==(nPoints-2))){
       Segments.ptP=PointsCurve[nPoints-1];
       Segments.ptQ=PointsCurve[nPoints-2];
       Segments.ptS=Segments.ptQ;       
       Segments.nbElementsAfterInitialisation=-1;
       Segments.nbElements=2;
       m_Segments.push_back( Segments );
       break;
     }
     
     /* Init for Melkman. Must have a triangle !!! */
     Np1 = (N+1) % nPoints; 
     /* While N, Np1 and Np2 are aligned, we test next */
     Np2 = (Np1 + 1) % nPoints;
     Segments.nbElements=3;
     
     while( IsLeft( N, Np1, Np2 ) == 0 && (myIsClosed  || ( Np2<= nPoints-2 ) ) )
       {
	 Np1 = ( Np1 + 1 ) % nPoints;
	 Np2 = ( Np1 + 1 ) % nPoints;
	 Segments.nbElements++;
       }
     if(!myIsClosed && IsLeft( N, Np1, Np2)==0){
       Segments.ptP=PointsCurve[N];
       Segments.ptQ=PointsCurve[Np1];
       Segments.ptS=Segments.ptQ;       
       Segments.nbElementsAfterInitialisation=-1;
       m_Segments.push_back( Segments );
       break;
     }
     

     
     InitMelkman(N,Np1,Np2);     
     M = Np2;
     std::deque< int > prevEnvConv= D;
     
     /* Test */
     bool stopedByEndPoint=false;
     while (!stopedByEndPoint &&  ( IsConvexValid( D, thick, connexity, blur ) ) && ( M != Np1 ) ) 
       {
	 
	 prevEnvConv=D;
	 Segments.nbElementsAfterInitialisation++;
	 Segments.nbElements++;
	 /* Save previous computation */
       
	 if (height < width) {
	   sav_P = edgePh; sav_Q = edgeQh ; sav_S = vertexSh;
	 } 
	 
	 if(!myIsClosed && (M+1 == nPoints)){
	   stopedByEndPoint=true;
	 }else{
	   /* One point more */
	   M = (M+1)%nPoints;
	   Melkman(M);
	 }
       }
     
     if(!stopedByEndPoint){
       Segments.nbElementsAfterInitialisation--;
       Segments.nbElements--;
     }
     // Modif BK 21/02/2011
     VectDeque.push_back( prevEnvConv );
     //VectDeque.push_back( D );
   
     
     
     /* We have to stop if the whole curve is inside one blurred segment */
     if (M == Np1) 
       {
	 N = 0;
	aPQ = sav_Q.y() - sav_P.y();
	bPQ = sav_Q.x() - sav_P.x();
	pPQ = aPQ * sav_P.x() - bPQ * sav_P.y() ;
	pS  = aPQ * sav_S.x() - bPQ * sav_S.y() ;
	/** Fill tmpTg **/
	Segments.m_first = 0;
	 Segments.m_last = nPoints - 1;
	 Segments.m_a = aPQ; 
         Segments.m_b = bPQ;
	 Segments.m_mu = (pPQ <= pS ? pPQ : pS);
	 Segments.m_omega = (pPQ <= pS ? pS - pPQ : pPQ - pS)+1;
	 // Modif BK 21/02/2011
	 Segments.D= prevEnvConv ;
	 //Segments.D =D;
	 Segments.ptP=sav_P;
	 Segments.ptQ=sav_Q;
	 Segments.ptS=sav_S;
	  if(Segments.nbElementsAfterInitialisation==-1){
	    Segments.ptP=PointsCurve[N%nPoints];
	    Segments.ptQ=PointsCurve[(Np2+nPoints -1)%nPoints];
	    Segments.ptS=Segments.ptQ;
	  }
     
	 
	 m_Segments.push_back( Segments );
	 //Segments++; nTgCover++;
	 continue;
       }
       /*** Output result
        *** Recall that [PQ] is the support edge and S the vertex
        ***/
       if (LastM != ((M + nPoints - 1) % nPoints)) {
             LastM = (M + nPoints - 1) % nPoints;
	    aPQ = sav_Q.y() - sav_P.y();
	    bPQ = sav_Q.x() - sav_P.x();
	    
	    pPQ = aPQ * sav_P.x() - bPQ * sav_P.y() ;
	    pS  = aPQ * sav_S.x() - bPQ * sav_S.y() ;
	    
	    /** Fill tmpTg **/
	    Segments.m_first = N;
	    Segments.m_last = (M + nPoints - 1) % nPoints;
	    Segments.m_a = aPQ; 
            Segments.m_b = bPQ; 
	    Segments.m_mu = (pPQ <= pS ? pPQ : pS);
	    Segments.m_omega = (pPQ <= pS ? pS - pPQ : pPQ - pS)+1;
	    // Modif BK 21/02/2011
	    Segments.D= prevEnvConv ;
	 
	    Segments.ptP=sav_P;
	    Segments.ptQ=sav_Q;
	    Segments.ptS=sav_S;
	     if(Segments.nbElementsAfterInitialisation==-1){
	       Segments.ptP=PointsCurve[N%nPoints];
	       Segments.ptQ=PointsCurve[(Np2+nPoints -1)%nPoints];
	       Segments.ptS=Segments.ptQ;
	     }
     
	    m_Segments.push_back( Segments );
	}
       
       
	/*** Next point
	 ***/
	N = (N+1) % nPoints;
   } while (N != 0);
    /*** Final correction
     *** Since we begin at 0, the first computed DSS might not belong
     *** to the final DSS. To check this, it is sufficient to look at the last one.
     *** If it finishes at the same place than the first one then the first one
     *** is useless.
     ***/
    //if ((nTgCover > 1) && (TgCover[nTgCover - 1].fin == TgCover[0].fin)) {
    //	nTgCover --;
    //	memcpy(TgCover, TgCover + nTgCover, sizeof(Tangente));
    // }
}













/**
 * Print TgCover
 * Print out the computed Tangential Cover.
 */
void 
ImaGene::BlurredSegmentTgtCover::PrintTgCover()
{
  int nTgCover = m_Segments.size();
  std::vector<Segment> s;
  s = m_Segments;
  cout << "Ligne   First   Last   a   b   mu   omega   NbPoints" << endl;
  cout << "===========================================" << endl;
  for(int i = 0; i < nTgCover; ++i )
  {
    cout << i << "    " << s[i].m_first << "    " << s[i].m_last << "    " 
         << s[i].m_a << "    " << s[i].m_b << "    " << s[i].m_mu << "    " 
         << s[i].m_omega << "     " << s[i].m_last - s[i].m_first 
         << endl; 
  }
}








/**
 *  Compute and update the Bounds of the BlurredSeg according different parameters.
 * 
 * @param segment (return)
 *
 **/


//  20/02/2011  Adaptation/Fusion code de Mouhammad par rapport à l'amélioration
//  de l'estimation de la boite englobante du segment flou.


void 
ImaGene::BlurredSegmentTgtCover::compSegmentBoundsFromExtremPt( const Vector2D & fp, const Vector2D & lp,
								Segment & aSegment, double minVisibleWidthBounds){
  
  
  // Special case, blurred segment composed only of aligned points. 
  // In this cas the bounding box is restrained to a eplison width rectangle.
  
  if(aSegment.nbElementsAfterInitialisation==-1){
    Vector2D vectPQ (aSegment.ptQ.x()- aSegment.ptP.x(), aSegment.ptQ.y()- aSegment.ptP.y()); 
    double normPQ = sqrt(vectPQ.x()*vectPQ.x()+vectPQ.y()*vectPQ.y());
    Vector2D vectUnitPerpPQ (-vectPQ.y()/normPQ, vectPQ.x()/normPQ);
    aSegment.ptALongestSegment1.x()=aSegment.ptP.x()+vectUnitPerpPQ.x()*minVisibleWidthBounds/2.0;
    aSegment.ptALongestSegment1.y()=aSegment.ptP.y()+vectUnitPerpPQ.y()*minVisibleWidthBounds/2.0;
    
    aSegment.ptBLongestSegment1.x()=aSegment.ptQ.x()+vectUnitPerpPQ.x()*minVisibleWidthBounds/2.0;
    aSegment.ptBLongestSegment1.y()=aSegment.ptQ.y()+vectUnitPerpPQ.y()*minVisibleWidthBounds/2.0;
    
    aSegment.ptCLongestSegment2.x()= aSegment.ptQ.x()-vectUnitPerpPQ.x()*minVisibleWidthBounds/2.0;
    aSegment.ptCLongestSegment2.y()= aSegment.ptQ.y()-vectUnitPerpPQ.y()*minVisibleWidthBounds/2.0;
    
    aSegment.ptDLongestSegment2.x()= aSegment.ptP.x()-vectUnitPerpPQ.x()*minVisibleWidthBounds/2.0;
    aSegment.ptDLongestSegment2.y()= aSegment.ptP.y()-vectUnitPerpPQ.y()*minVisibleWidthBounds/2.0;
    
  }else{
    
    Vector2D projF, projL, projS ;
    ContourHelper::projetOnStraightLine(aSegment.ptP, aSegment.ptQ, fp, projF);
    ContourHelper::projetOnStraightLine(aSegment.ptP, aSegment.ptQ, lp, projL);
    ContourHelper::projetOnStraightLine(aSegment.ptP, aSegment.ptQ, aSegment.ptS, projS);
    
    // Shift of the projection of PQ to define the other part of bounds.
    Vector2D dep (aSegment.ptS.x()-projS.x(), aSegment.ptS.y()-projS.y());
    Vector2D projF2(projF.x()+dep.x(), projF.y()+dep.y() );
    Vector2D projL2(projL.x()+dep.x(), projL.y()+dep.y() );
    
    aSegment.ptALongestSegment1.x()=projF.x();
    aSegment.ptALongestSegment1.y()=projF.y();
    
    aSegment.ptBLongestSegment1.x()=projL.x();
    aSegment.ptBLongestSegment1.y()=projL.y();
    
    aSegment.ptCLongestSegment2.x()=projL2.x();
    aSegment.ptCLongestSegment2.y()=projL2.y();
    
    aSegment.ptDLongestSegment2.x()=projF2.x();
    aSegment.ptDLongestSegment2.y()=projF2.y();      
  }
}





/**
 * Compute the basic Bounds of Blurred Segmente by updating the values of segment bounding points 
 *  ptALongestSegment1, ptBLongestSegment ptCLongestSegment2 and ptDLongestSegment2
 * 
 * @param aSegment (return).
 */

//  20/02/2011  Adaptation/Fusion code de Mouhammad par rapport à l'amélioration
//  de l'estimation de la boite englobante du segment flou.


void
ImaGene::BlurredSegmentTgtCover::computeBasicBounds( BlurredSegmentTgtCover::Segment & aSegment, double minVisibleWidthBounds){ 
  Vector2D fp ( PointsCurve[aSegment.m_first].x() , PointsCurve[aSegment.m_first].y() );
  Vector2D lp ( PointsCurve[aSegment.m_last].x() , PointsCurve[aSegment.m_last].y() );
  compSegmentBoundsFromExtremPt(fp,lp,aSegment, minVisibleWidthBounds);
  aSegment.boxLength = VectorUtils::norm(Vector2D(aSegment.ptBLongestSegment1.x()-aSegment.ptALongestSegment1.x(),
						  aSegment.ptBLongestSegment1.y()-aSegment.ptALongestSegment1.y()));
 
}






/**
 * Compute the Bounds of Blurred Segmente by taking into account
 *  correctly the extremity even by change of direction for the end
 *  points.  The values of segment bounding points are updated (
 *  ptALongestSegment1, ptBLongestSegment ptCLongestSegment2 and
 *  ptDLongestSegment2)
 * 
 * @param aSegment (return).
 */

//  20/02/2011  Adaptation/Fusion code de Mouhammad par rapport à l'amélioration
//  de l'estimation de la boite englobante du segment flou.



void 
ImaGene::BlurredSegmentTgtCover::computeRealBounds( BlurredSegmentTgtCover::Segment & aSegment, double minVisibleWidthBounds )
{
  computeBasicBounds(aSegment, minVisibleWidthBounds);
  if(aSegment.nbElementsAfterInitialisation==-1){
    return;
  }
  Vector2D extremF ( PointsCurve[aSegment.m_first].x() , PointsCurve[aSegment.m_first].y() );
  Vector2D extremL ( PointsCurve[aSegment.m_last].x() , PointsCurve[aSegment.m_last].y() );
  Vector2D projExtremF;
  Vector2D projExtremL;
  
  ContourHelper::projetOnStraightLine(aSegment.ptP, aSegment.ptQ, extremF, projExtremF);
  ContourHelper::projetOnStraightLine(aSegment.ptP, aSegment.ptQ, extremL, projExtremL);
  
  
  
  std::deque<int> Dsegment = aSegment.D;
  int nb = Dsegment.size();
  for( uint p = 0 ; p < nb ; ++p)
    {
      Vector2D pointEnvConv (  PointsCurve[ Dsegment[p] ].x() ,  PointsCurve[ Dsegment[p] ].y() );
      Vector2D projPointEnvConv;
      bool isNewExtrema = !ContourHelper::projetOnStraightLine(projExtremF, projExtremL, pointEnvConv, projPointEnvConv); 
      if(isNewExtrema){
	double distFirst = VectorUtils::distance(projPointEnvConv, projExtremF);  
	double distLast = VectorUtils::distance(projPointEnvConv, projExtremL);  
	if(distFirst> distLast){
	  extremL = pointEnvConv;
	  projExtremL = projPointEnvConv;	  
	}else{
	  extremF = pointEnvConv;
	  projExtremF = projPointEnvConv;	 
	}
      }
      
    }
  
  compSegmentBoundsFromExtremPt(extremF, extremL, aSegment, minVisibleWidthBounds);
  aSegment.boxLength = VectorUtils::norm(Vector2D(aSegment.ptBLongestSegment1.x()-aSegment.ptALongestSegment1.x(),
						  aSegment.ptBLongestSegment1.y()-aSegment.ptALongestSegment1.y())); 
 
}

















/**
 *
 * Get statistics for each point of the contour
 * 
 * @return a dyn. alloc. statistics object storing the length of maximal segments for each contour point. 
 * Contains as many statistics variables as the number of surfels of [fc]. 
 * 
 **/
// Ajout BK 18/02/2011

ImaGene::Statistics* 
ImaGene::BlurredSegmentTgtCover::getStatsMaximalSegments(double scaleWidth){
  Statistics* statsResu = new Statistics(  PointsCurve.size(), false );  
  this->ExtractSegment( scaleWidth, 0, 0 );

  for(int i=0; i<m_Segments.size(); i++){
    Segment seg= m_Segments.at(i);
    computeRealBounds( seg,0.1 );
    double segLength= seg.boxLength; 
    
    for(int k=seg.m_first; k !=((seg.m_last+1)% PointsCurve.size()); k=(k+1)% PointsCurve.size()){
      statsResu-> addValue(k, segLength);
    }
  }
  statsResu->terminate();
  return statsResu;
}






void 
ImaGene::BlurredSegmentTgtCover::computeNoiseAndSlope(std::vector<double> &vectNoise, 
						      std::vector<double> &vectSlope, 
						      std::vector<double> vectScalesWidth, uint minSize, double maxSlope){  
  vector<Statistics *> allStats;
  cerr<< "CurveSize :" << PointsCurve.size()<< endl;
  cerr << "Computing stats to all scales : [" ;
  for(uint i=0; i <vectScalesWidth.size(); i++){
    allStats.push_back(getStatsMaximalSegments(vectScalesWidth.at(i)));
    cerr << ".";
  }
  cerr << "]" << endl;
  // Construction of all the scale profiles:  
  for(int i=0; i<PointsCurve.size(); i++){
    ScaleProfile sp; 
    sp.init(vectScalesWidth.size());
    for(int j = 0; j< vectScalesWidth.size(); j++){
      sp.addValue(j, (allStats.at(j)->mean(i))/(vectScalesWidth.at(j)));      
    }
    vectNoise.push_back(sp.noiseLevel(minSize, maxSlope));
    std::pair<bool ,double> slope= sp.getSlopeFromMeaningfulScales(maxSlope, -1e20, minSize);
    if(!slope.first){
      cerr << "Warning pt [" <<i<< "] : slope defined from all profile points since no meaningful scale found " << endl;
      vectSlope.push_back(1000.0);
    }else{
      vectSlope.push_back(slope.second);
    }
    

    
  }
  for(int i=0;i < allStats.size(); i++){
    delete allStats.at(i);
  }  
  
}
    
    

vector<double> 
ImaGene::BlurredSegmentTgtCover::getNoiseLevels(vector<double>  vectScalesWidth, uint minSize, double maxSlope){
  vector<double> noiseLevels;
  vector<Statistics *> allStats;
  cerr<< "CurveSize :" << PointsCurve.size()<< endl;
  cerr << "Computing stats to all scales : [" ;
  for(uint i=0; i <vectScalesWidth.size(); i++){
    allStats.push_back(getStatsMaximalSegments(vectScalesWidth.at(i)));
    cerr << ".";
  }
  cerr << "]" << endl;
  // Construction of all the scale profiles:  
  for(int i=0; i<PointsCurve.size(); i++){
    ScaleProfile sp; 
    sp.init(vectScalesWidth.size());
    for(int j = 0; j< vectScalesWidth.size(); j++){
      sp.addValue(j, (allStats.at(j)->mean(i))/(vectScalesWidth.at(j)));
    }
    noiseLevels.push_back(sp.noiseLevel(minSize, maxSlope));
  }  
  for(int i=0;i < allStats.size(); i++){
    delete allStats.at(i);
  }  
  return noiseLevels;  
}




void
ImaGene::BlurredSegmentTgtCover::getScaleProfile(vector<double>  vectScalesWidth, uint index, ScaleProfile &sp){
  vector<Statistics *> allStats;
  cerr<< "CurveSize :" << PointsCurve.size()<< endl;
  cerr << "Computing stats to alls scales : [" ;
  for(uint i=0; i <vectScalesWidth.size(); i++){
    allStats.push_back(getStatsMaximalSegments(vectScalesWidth.at(i)));
    cerr << ".";
  }
  cerr << "]" << endl;
  
  sp.init(vectScalesWidth.size());
  for(int j = 0; j< vectScalesWidth.size(); j++){
    sp.addValue(j, (allStats.at(j)->mean(index))/(vectScalesWidth.at(j)));      
  }  
}












///////////////////////////////////////////////////////////////////////////////
// Internals - private :
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
