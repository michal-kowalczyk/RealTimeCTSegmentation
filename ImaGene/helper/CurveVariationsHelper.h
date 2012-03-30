#ifndef CURVEVARIATIONSHELPER_H
#define CURVEVARIATIONSHELPER_H


#include <vector>
#include <stdlib.h>
#include <iostream>
#include "ImaGene/base/Vector.h"
#include "ImaGene/base/Vector2i.h"
#include "ImaGene/dgeometry2d/FreemanChain.h"




namespace ImaGene {


class CurveVariationsHelper{


  struct LocalMinMaxArea{
    int indiceDebut;
    int indiceFin;    
    bool isMaxArea;
  };


  struct PosNegZeroArea{
    int indiceDebut;
    int indiceFin;    
    bool isPositif;
    bool isNegativ;
    bool isZero;
  };

  

  
  
public:


  /**
   *   Include header 
   */  
 



/*
 *  Get all areas of constant sign or constant zero. Zero is considered neither positif and neither negatif.  
 *  @param vectValues: initial double values
 *  @param quantify : bool value defined to apply or not the quantification of vectValues.
 *  @param epsilon: precision used to define the comparisons (((double)((int)(vectValues.at(i)/epsilon)))*epsilon)
 *  @param isClosed ajust the resulting intervals according the initial values are considered as closed or open
 */
 
  static std::vector<PosNegZeroArea> 
  getPosNegZeroAreas(const std::vector<double> &vectValues, bool quantify, double epsilon, bool isClosed );


  

  /**
   * Compute the maxima/minima areas given with the precision epsilon.
   * @param vectValues is the vector of initial data.
   * @param quantify : bool value defined to apply or not the quantification of vectValues.
   * @param epsilon represent the prevision for determining if x=y  
   * @param isClosed used to consider the end point.
   * @return a vector containing all the LocalMinMaxArea.
   */
  static std::vector<LocalMinMaxArea> getLocalMaxMinArea(const std::vector<double> &vectValues, bool quantify, 
							 double epsilon, bool isClosed);  


  


  /**
   * Compute the maxima/minima areas extended according the following criteria:   
   * First step: compute all maxima/minima 
   * Second step: extend all previous areas by choosing for a given
   * point the area having a value (the value of the constant max/min
   * area) the nearest to the considered point and having the same
   * sign.
   *  @param quantify : bool value defined to apply or not the quantification of vectValues.
   * @param vectValues is the vector of initial data.
   * @param epsilon represent the prevision for determining if x=y  
   * @param isClosed used to consider the end point.
   * @return a vector containing all the LocalMinMaxArea.
   */

  static std::vector<LocalMinMaxArea> getLocalMaxMinAreaExtended(const std::vector<double> &vectValues, 
								 bool quantify, double epsilon, bool isClosed, 
								 bool extendOnZerocurvature);



  /**
   *  Return the points included in the considered areas (LocalMinMaxArea) 
   *
   */
  
  static std::vector<ImaGene::Vector2i> getPointsFromArea(const CurveVariationsHelper::LocalMinMaxArea &indicesLocArea, 
						 const std::vector<Vector2i> &vectContour );


  
  /**
   *  Return the points included in the considered areas (PosNegZeroArea) 
   *
   */
  
  static std::vector<ImaGene::Vector2i> getPointsFromArea(const CurveVariationsHelper::PosNegZeroArea &indicesLocArea, 
							  const std::vector<Vector2i> &vectContour );    
  


/*
 *  Renvoie un vecteur contenant les indices des minimas locaux:
 *  @param vectValues : l'ensemble des valeurs
 *  @param quantify : bool value defined to apply or not the quantification of vectValues.
 *  @param epsilon : valeur de précision pour considérer que deux valeurs sont égales
 *  @param isClosed: modifie le calcul dans la cas de courbes fermées.
*/
  
  static std::vector<int> getLocalMinimaIndices(const std::vector<double> &vectValues, bool quantify,
						double epsilon, bool isClosed);


/*
 *  Renvoie un vecteur contenant les indices des maximas locaux:
 *  @param vectValues : l'ensemble des valeurs
 *  @param quantify : bool value defined to apply or not the quantification of vectValues.
 *  @param epsilon : valeur de précision pour considérer que deux valeurs sont égales
 *  @param isClosed: modifie le calcul dans la cas de courbes fermées.
*/

  static std::vector<int> getLocalMaximaIndices(const std::vector<double> &vectValues, 
						bool quantify, double epsilon, bool isClosed);

  
  

  /**
   * Transform the initial curve such that it contains no variations
   * larger than ninVariations. The curve is transformed iteratively
   * from each local maximal areas. 
   * @param curve : the initial curve;
   * @param minVariations : the minimal variation authorized between two consecutive points.
   * @param isClosed: permits to take into accounds the type of curve.
   **/
  
  static std::vector<double> getCurveWithLowVariationsFromMax(const std::vector<double> & curve,
							      double minVariations, bool isClosed );
  
  






private:









};
}
#endif












