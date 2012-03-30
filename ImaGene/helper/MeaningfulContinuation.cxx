///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : MeaningfulContinuation.cxx
//
// Creation : 2011/03/09
//
// Version : 2011/03/09
//
// Author : BK
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
//	2011/03/09 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //


///////////////////////////////////////////////////////////////////////////////

#include "ImaGene/helper/MeaningfulContinuation.h"
#include "ImaGene/mathutils/Mathutils.h"
#include "ImaGene/base/VectorUtils.h"

// Includes inline functions/methods if necessary.
#if !defined(INLINE)
#include "ImaGene/helper/MeaningfulContinuation.ih"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;

const char* const MeaningfulContinuation_RCS_ID = "@(#)class MeaningfulContinuation definition.";







///////////////////////////////////////////////////////////////////////////////
// class EpsilonMeaningfulSegment
///////////////////////////////////////////////////////////////////////////////

      /**
       * Constructor. 
       *
       * @param epsilon: used to determine is the segment is a good continuation (i.e epsilon< NFA (Number of False Alarm))) 
       * @param Nc: parameter given in the NFA computation.
       * @param kappaMax: maximal angle to consider curvature sequence (max PI/2).
       *
       **/


ImaGene::MeaningfulContinuation::EpsilonMeaningfulSegment::EpsilonMeaningfulSegment (double epsilon, uint Nc, double kappaMax){
  m_epsilon= epsilon;
  m_Nc=Nc;
  m_kappaMax = kappaMax;
  m_dThetaMax = 0.0;  
  m_size=0;
  indexFront=-1;
  indexBack=-1;
		 
  
}


ImaGene::MeaningfulContinuation::EpsilonMeaningfulSegment::~EpsilonMeaningfulSegment (){
  
  
}

      /**
       * Check if the point can be added to the front of contour
       * according the new NFA and epsilon.
       *
       * @param pt: the new point
       * @param index: its associated index of the contour.
       * @return: true if point can be added to the segment with a new NFA < epsilon.
       *
       **/

bool 
ImaGene::MeaningfulContinuation::EpsilonMeaningfulSegment::extendFront(const Vector2D &pt, uint index){
  if(m_size <=1){
    m_FrontBackExtremPoints.push_front(pt);
    m_size++;
    indexFront=index;
    if(m_size==1){
      indexBack=index;
    }
    return true;
  }
  
  Vector2D ptA = m_FrontBackExtremPoints.front();
  m_FrontBackExtremPoints.pop_front();
  Vector2D ptB = m_FrontBackExtremPoints.front();
  m_FrontBackExtremPoints.pop_front();
    
  Vector2D ptAPt ( pt.x()- ptA.x(), pt.y()- ptA.y());
  Vector2D ptBPtA ( ptA.x()- ptB.x(), ptA.y()- ptB.y());
  
  VectorUtils::normalize(ptAPt);
  VectorUtils::normalize(ptBPtA);
  
  double newDtheta = abs(acos(VectorUtils::dotProduct(ptBPtA, ptAPt))); 
  if(newDtheta>m_kappaMax){
    double newFalseAlarm=m_Nc;
    if(newFalseAlarm>m_epsilon){
      m_FrontBackExtremPoints.push_front(ptB);
      m_FrontBackExtremPoints.push_front(ptA);
      return false;
    }else{
      m_FrontBackExtremPoints.push_front(ptA);
      m_FrontBackExtremPoints.push_front(pt);
      m_size++;
      indexFront=index;
      return true;     
      
    }
  }


  if(newDtheta>m_dThetaMax){
    double newFalseAlarm = m_Nc*(pow((newDtheta/Mathutils::two_pi_d/2.0), (int)(m_size+1)));
    if(newFalseAlarm>m_epsilon){
      m_FrontBackExtremPoints.push_front(ptB);
      m_FrontBackExtremPoints.push_front(ptA);
      
      return false;
    }else{
      m_FrontBackExtremPoints.push_front(ptA);
      m_FrontBackExtremPoints.push_front(pt);
      m_dThetaMax=newDtheta;
      m_size++;
      indexFront=index;
      return true;
    }
  }else{
    m_FrontBackExtremPoints.push_front(ptA);
    m_FrontBackExtremPoints.push_front(pt);
    m_size++;
    indexFront=index;
    return true;     
  }
  
}




      /**
       * Check if the point can be added to the back of contour
       * according the new NFA and epsilon.
       *
       * @param pt: the new point
       * @param index: its associated index of the contour.
       * @return: true if point can be added to the segment with a new NFA < epsilon.
       *
       **/


bool 
ImaGene::MeaningfulContinuation::EpsilonMeaningfulSegment::extendBack(const Vector2D &pt, uint index){
  if(m_size <=1){
    m_FrontBackExtremPoints.push_back(pt);
    m_size++;
    indexBack=index;
    indexFront=index;
    return true;
  }
  
  Vector2D ptA = m_FrontBackExtremPoints.back();
  m_FrontBackExtremPoints.pop_back();
  Vector2D ptB = m_FrontBackExtremPoints.back();
  m_FrontBackExtremPoints.pop_back();
  
  
  Vector2D ptAPt ( pt.x()- ptA.x(), pt.y()- ptA.y());
  Vector2D ptBPtA ( ptA.x()- ptB.x(), ptA.y()- ptB.y());
  
  VectorUtils::normalize(ptAPt);
  VectorUtils::normalize(ptBPtA);
  
  double newDtheta = abs(acos(VectorUtils::dotProduct(ptBPtA, ptAPt))); 
  if(newDtheta>m_kappaMax){
    double newFalseAlarm=m_Nc;
    if(newFalseAlarm>m_epsilon){
      m_FrontBackExtremPoints.push_back(ptB);
      m_FrontBackExtremPoints.push_back(ptA);
      return false;
    }else{
      m_FrontBackExtremPoints.push_back(ptA);
      m_FrontBackExtremPoints.push_back(pt);
      m_size++;
      indexBack=index;
      return true;     
      
    }
  }
  
 if(newDtheta>m_dThetaMax){
    
    double newFalseAlarm = m_Nc*(pow((newDtheta/Mathutils::two_pi_d/2.0), (int)(m_size+1)));
    if(newFalseAlarm>m_epsilon){
	m_FrontBackExtremPoints.push_back(ptB);
	m_FrontBackExtremPoints.push_back(ptA);
      return false;
    }else{
      m_FrontBackExtremPoints.push_back(ptA);
      m_FrontBackExtremPoints.push_back(pt);
      m_dThetaMax=newDtheta;
      m_size++;
      indexBack=index;
      return true;
    }
  }else{
    m_FrontBackExtremPoints.push_back(ptA);
    m_FrontBackExtremPoints.push_back(pt);
    m_size++;
    indexBack=index;
    return true;     
  }
  
}



      /**
       * Return the index of the last point added to the front. 
       **/

uint 
ImaGene::MeaningfulContinuation::EpsilonMeaningfulSegment::front(){
  return indexFront;
}

      /**
       * Return the index of the last point added to the back. 
       **/

uint 
ImaGene::MeaningfulContinuation::EpsilonMeaningfulSegment::back(){
  return indexBack;
}


      
      /**
       * Return the size = number of points added to the segment. 
       **/
uint 
ImaGene::MeaningfulContinuation::EpsilonMeaningfulSegment::size(){
  return m_size;
}
								 
///////////////////////////////////////////////////////////////////////////////
// class MeaningfulContinuation
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

/**
 * Destructor. 
 */
ImaGene::MeaningfulContinuation::~MeaningfulContinuation()
{
}

/**
 * Constructor. The object is not valid yet.
 * @see init
 */
ImaGene::MeaningfulContinuation::MeaningfulContinuation()
{
}


  
 /** 
     * Initialization from polygon.
     *
     * @param polygon  the coordinates of polygon vertices.
     * @param epsilon: used to determine is the segment is a good continuation (i.e epsilon< NFA (Number of False Alarm))) 
     * @param Nc: parameter given in the NFA computation.
     * @param kappaMax: maximal angle to consider curvature sequence (max PI/2).
     *
     * @param closed when 'true' the polygon is closed, otherwise it
     * is an open polygonal line.
     */
void 
ImaGene::MeaningfulContinuation::init(const std::vector<ImaGene::Vector2D> & polygon, double epsilon,
				      double Nc, double kappaMax,  bool closed ){
  
  m_polygon = polygon;
  m_epsilon = epsilon;
  m_Nc= Nc;
  m_kappaMax = kappaMax;
  m_closed= closed;
  
}




vector< ImaGene::MeaningfulContinuation::EpsilonMeaningfulSegment > 
ImaGene::MeaningfulContinuation::getMeaningfulMaxSegments() {
  // Implementation en ouvert:
  
  vector<EpsilonMeaningfulSegment > vectResult;
  uint pos=0;
  bool noMorePoints= false;
  while(!noMorePoints){
    EpsilonMeaningfulSegment seg(m_epsilon, m_Nc, m_kappaMax);
    bool extending = true;
    while(extending  && pos<m_polygon.size()  ){
      extending = seg.extendFront(m_polygon.at(pos), pos);
      if(extending)
	pos++;
    }    
    vectResult.push_back(seg);
    noMorePoints= (pos==m_polygon.size());
    
  }
  
  return vectResult;
}





///////////////////////////////////////////////////////////////////////////////
// Interface - public :

/**
 * Writes/Displays the object on an output stream.
 * @param that_stream the output stream where the object is written.
 */
void 
ImaGene::MeaningfulContinuation::selfDisplay( ostream& that_stream ) const
{
  that_stream << "[MeaningfulContinuation]";
}

/**
 * Checks the validity/consistency of the object.
 * @return 'true' if the object is valid, 'false' otherwise.
 */
bool 
ImaGene::MeaningfulContinuation::OK() const
{
  return true;
}



///////////////////////////////////////////////////////////////////////////////
// Internals - private :

//                                                                           //
///////////////////////////////////////////////////////////////////////////////
