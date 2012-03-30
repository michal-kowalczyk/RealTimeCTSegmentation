///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// File name : CurveVariationsHelper.cxx
//
// Creation : 2009/07/22
//
// Version : 2009/07/22
//
// Author : Bertrand Kerautret
//
// email : kerautre@loria.fr
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
//	2009/07/22 : Mr ?Name? : ?What?
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //




#include <stdlib.h>
#include <algorithm>
#include "ImaGene/helper/CurveVariationsHelper.h"



using namespace std;
using namespace ImaGene;







/**
 *  Return the points included in the considered areas (LocalMinMaxArea) 
 *
 */


vector<Vector2i> 
CurveVariationsHelper::getPointsFromArea(const CurveVariationsHelper::LocalMinMaxArea &indicesLocArea, const vector<Vector2i> &vectContour){
  vector<Vector2i> vResult; 
  int tailleContour= vectContour.size();      
  for(int i=indicesLocArea.indiceDebut; (i%tailleContour) !=(indicesLocArea.indiceFin)%tailleContour; i++){
    vResult.push_back(vectContour.at(i%(tailleContour)));
  }
  return vResult;
}




/**
 *  Return the points included in the considered areas (LocalMinMaxArea) 
 *
 */


vector<Vector2i> 
CurveVariationsHelper::getPointsFromArea(const CurveVariationsHelper::PosNegZeroArea &indicesLocArea, const vector<Vector2i> &vectContour ){
  vector<Vector2i> vResult; 
  int tailleContour= vectContour.size();  
  for(int i=indicesLocArea.indiceDebut; (i%tailleContour) !=(indicesLocArea.indiceFin)%tailleContour; i++){
    vResult.push_back(vectContour.at(i%(tailleContour)));
  }
  return vResult;
}
















/*
 *  Get all areas of constant sign or constant zero. Zero is considered neither positif and neither negatif.  
 *  @param vectValues: initial double values
 *  @param quantify : bool value defined to apply or not the quantification of vectValues.
 *  @param epsilon: precision used to define the comparisons (((double)((int)(vectValues.at(i)/epsilon)))*epsilon)
 *  @param isClosed ajust the resulting intervals according the initial values are considered as closed or open
 */

       
vector<CurveVariationsHelper::PosNegZeroArea> 
CurveVariationsHelper::getPosNegZeroAreas(const vector<double> &vectValues, bool quantify, double epsilon, bool isClosed ){
  
  vector<CurveVariationsHelper::PosNegZeroArea> vectResuMinMaxArea ;
  vector<double> vectValuesModif;  
  int tailleVValues = vectValues.size();
    
  
    // quantification
    for(int i =0;i<tailleVValues; i++){
      if(quantify)
	vectValuesModif.push_back( ((double)((int)(vectValues.at(i)/epsilon)))*epsilon);       
      else
	vectValuesModif.push_back(vectValues.at(i));       
    }              
    
  
  // recalage au premier changement.
  int decalage =0;
  while( decalage< tailleVValues  &&  ((vectValuesModif.at(decalage)*vectValuesModif.at(0)>0)||
				       ((vectValuesModif.at(0)==0)&&(vectValuesModif.at(decalage)==0)))){
    decalage++;
  }

  
  if(decalage==tailleVValues){
    PosNegZeroArea locAll;
    locAll.indiceDebut=0;
    locAll.indiceFin=vectValuesModif.size()-1;
    locAll.isPositif = (vectValuesModif.at(0)>0);
    locAll.isNegativ = (vectValuesModif.at(0)<0);
    locAll.isZero = (vectValuesModif.at(0)==0);
    vectResuMinMaxArea.push_back(locAll);
    return vectResuMinMaxArea;    
  }
  

  //Si le contour est ouvert on inclu l'intervalle qui va jusqu'au changement de signe ou passage à 0;
  if(!isClosed ){
    PosNegZeroArea loc;
    loc.indiceDebut = 0;
    loc.indiceFin = (decalage-1%tailleVValues)%tailleVValues;
    loc.isPositif = (vectValues.at(loc.indiceDebut)>0);
    loc.isNegativ = (vectValues.at(loc.indiceDebut)<0);
    loc.isZero = (vectValues.at(loc.indiceDebut)==0);    
    vectResuMinMaxArea.push_back(loc);    
  }
  
  int borneSup= (isClosed)? tailleVValues+decalage: tailleVValues;
  
  PosNegZeroArea locEnCours;
  locEnCours.indiceDebut=-1;
  locEnCours.indiceFin=-1;

  for(int i=decalage; i< borneSup; i++){
    // premiere zone trouvée
    if(locEnCours.indiceDebut==-1){
      locEnCours.indiceDebut=i%tailleVValues;
      locEnCours.indiceFin=i%tailleVValues;
      locEnCours.isPositif=(vectValuesModif.at(locEnCours.indiceDebut%tailleVValues)>0);
      locEnCours.isNegativ=(vectValuesModif.at(locEnCours.indiceDebut%tailleVValues)<0);
      locEnCours.isZero=(vectValuesModif.at(locEnCours.indiceDebut%tailleVValues)==0);
    }else{
      // zone déjà en cours de détection
      // la zone s'arrête
      if( vectValuesModif.at(i%tailleVValues)*vectValuesModif.at(locEnCours.indiceDebut%tailleVValues)<0 || 
	  ((vectValuesModif.at(i%tailleVValues)==0.0) && (vectValuesModif.at((locEnCours.indiceDebut%tailleVValues)%tailleVValues)!=0))||
	  ((vectValuesModif.at(i%tailleVValues)!=0.0) && (vectValuesModif.at((locEnCours.indiceDebut%tailleVValues)%tailleVValues)==0.0))){
       	vectResuMinMaxArea.push_back(locEnCours);
	
	locEnCours.indiceDebut=i%tailleVValues;
	locEnCours.indiceFin=i%tailleVValues;
	locEnCours.isPositif=(vectValuesModif.at(locEnCours.indiceDebut%tailleVValues)>0);
	locEnCours.isNegativ=(vectValuesModif.at(locEnCours.indiceDebut%tailleVValues)<0);
	locEnCours.isZero=(vectValuesModif.at(locEnCours.indiceDebut%tailleVValues)==0);	  
	
      }else{
	locEnCours.indiceFin=i%tailleVValues;	      
      }
    }        
  }
  if(locEnCours.indiceFin!=-1){
    vectResuMinMaxArea.push_back(locEnCours);    
  }
  
  return vectResuMinMaxArea;
}

















/*
 *  Renvoie un vecteur contenant les indices des minimas locaux:
 *  @param vectValues : l'ensemble des valeurs
 *  @param quantify : bool value defined to apply or not the quantification of vectValues.
 *  @param epsilon : valeur de prÃ©cision pour considÃ©rer que deux valeurs sont Ã©gales
 *  @param isClosed: modifie le calcul dans la cas de courbes fermÃ©es.
*/


vector<int> CurveVariationsHelper::getLocalMinimaIndices(const vector<double> &vectValues, bool quantify, double epsilon, bool isClosed){
  vector<int> vResult;
  vector<LocalMinMaxArea> vectAreas = CurveVariationsHelper::getLocalMaxMinArea(vectValues, quantify, epsilon, isClosed);
  vector<LocalMinMaxArea>::iterator it = vectAreas.begin();
  int tailleTab = vectValues.size();
  
  while(it!=vectAreas.end()){
    LocalMinMaxArea area = *it;
    if(!area.isMaxArea){      
      int deb = area.indiceDebut;
      int fin = area.indiceFin;
      int longueurInterval= (deb<=fin)?(fin-deb):((fin+tailleTab)-deb);
      int milieu = (deb+(longueurInterval/2))%tailleTab;    
      vResult.push_back(milieu);        
    }
    ++it;
  }
  return vResult;
}







/*
 *  Renvoie un vecteur contenant les indices des minimas locaux:
 *  @param vectValues : l'ensemble des valeurs
 *  @param quantify : bool value defined to apply or not the quantification of vectValues.
 *  @param epsilon : valeur de prÃ©cision pour considÃ©rer que deux valeurs sont Ã©gales
 *  @param isClosed: modifie le calcul dans la cas de courbes fermÃ©es.
*/


vector<int> 
CurveVariationsHelper::getLocalMaximaIndices(const vector<double> &vectValues, bool quantify, double epsilon, bool isClosed){
  vector<int> vResult;
  vector<LocalMinMaxArea> vectAreas = CurveVariationsHelper::getLocalMaxMinArea(vectValues, quantify, epsilon, isClosed);
  vector<LocalMinMaxArea>::iterator it = vectAreas.begin();
  int tailleTab = vectValues.size();
  while(it!=vectAreas.end()){
    CurveVariationsHelper::LocalMinMaxArea area = *it;
    if(area.isMaxArea){      
      int deb = area.indiceDebut;
      int fin = area.indiceFin;
      int longueurInterval= (deb<=fin)?(fin-deb):((fin+tailleTab)-deb);
      int milieu = (deb+(longueurInterval/2))%tailleTab;    
      vResult.push_back(milieu);        
    }
    ++it;
  }
  return vResult;
}






  /**
   * Compute the maxima/minima areas given with the precision epsilon.
   * @param vectValues is the vector of initial data.
   * @param quantify : bool value defined to apply or not the quantification of vectValues.
   * @param epsilon represent the prevision for determining if x=y  
   * @param isClosed used to consider the end point.
   * @return a vector containing all the LocalMinMaxArea.
   */
       
vector<CurveVariationsHelper::LocalMinMaxArea> 
CurveVariationsHelper::getLocalMaxMinArea(const vector<double> &vectValues, bool quantify, double epsilon, bool isClosed ){
  
  vector<CurveVariationsHelper::LocalMinMaxArea> vectResuMinMaxArea ;
 
  
  vector<double> vectValuesModif;
  
  int tailleVValues = vectValues.size();
    
  int decalage =0;
  bool estMontant=true;
  

    for(int i =0;i<tailleVValues; i++){
      if(quantify)
	vectValuesModif.push_back( ((double)((int)(vectValues.at(i)/epsilon)))*epsilon);       
      else
	vectValuesModif.push_back( vectValues.at(i));       
    }
  
  while(decalage<tailleVValues && vectValuesModif.at((tailleVValues+decalage)%tailleVValues)==vectValuesModif.at(0)){
    if(vectValuesModif.at((tailleVValues+decalage)%tailleVValues)-vectValuesModif.at((tailleVValues+decalage+1)%tailleVValues)<0){
      estMontant=true;
      break;
    }
    if(vectValuesModif.at((tailleVValues+decalage)%tailleVValues)-vectValuesModif.at((tailleVValues+decalage+1)%tailleVValues)>0){
      estMontant=false;
      break;
    }
    decalage++;
  }
  
  if(decalage==tailleVValues)
    return vectResuMinMaxArea;
      
  int borneSup= (isClosed)? tailleVValues+decalage+1: tailleVValues-1;

  if(!isClosed){
    CurveVariationsHelper::LocalMinMaxArea lMinMax;
    lMinMax.indiceDebut=0;
    lMinMax.indiceFin=(decalage+tailleVValues)%tailleVValues;
    lMinMax.isMaxArea=!estMontant;
    vectResuMinMaxArea.push_back(lMinMax);
  }
  


  int indiceZonePlat = -1;
  for(int i=decalage; i< borneSup; i++){
    if(vectValuesModif.at((i+tailleVValues)%tailleVValues)!=vectValuesModif.at((i+1)%tailleVValues)){
      indiceZonePlat=i+1;
    }
    if(estMontant){     
      if((vectValuesModif.at((i+1)%tailleVValues)-vectValuesModif.at(i%tailleVValues))<0){
	estMontant=false;	
	int k = i+tailleVValues;
	CurveVariationsHelper::LocalMinMaxArea lMinMax;
	lMinMax.indiceFin=k%tailleVValues;	
       	while(vectValuesModif.at(k%tailleVValues)==vectValuesModif.at(i%tailleVValues)){	  	 
	  k--;
	}
	lMinMax.indiceDebut=(k+1)%tailleVValues;
	lMinMax.isMaxArea=true;
	vectResuMinMaxArea.push_back(lMinMax);
      }
    }else{
      if(vectValuesModif.at((i+1)%tailleVValues)-vectValuesModif.at((i)%tailleVValues)>0){
	estMontant=true;		
	int k = i+tailleVValues;
	CurveVariationsHelper::LocalMinMaxArea lMinMax;
	lMinMax.indiceFin=k%tailleVValues;
	while(vectValuesModif.at(k%tailleVValues)==vectValuesModif.at(i%tailleVValues)){
	  k--;
	}
	lMinMax.indiceDebut=(k+1)%tailleVValues;
	lMinMax.isMaxArea=false;
	vectResuMinMaxArea.push_back(lMinMax);	
      }
    }    
  }
  
  
  if(!isClosed){
    CurveVariationsHelper::LocalMinMaxArea lMinMax;
    lMinMax.indiceDebut=indiceZonePlat;
    lMinMax.indiceFin= tailleVValues-1;
    lMinMax.isMaxArea=estMontant;
    vectResuMinMaxArea.push_back(lMinMax);
  }
  
  return vectResuMinMaxArea;
}



















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


vector<CurveVariationsHelper::LocalMinMaxArea> 
CurveVariationsHelper::getLocalMaxMinAreaExtended(const vector<double> & vectValues, 
						  bool quantify, double epsilon, 
						  bool isClosed, bool  extendOnZerocurvature){
  
  vector<LocalMinMaxArea> vectExtendedAreas = getLocalMaxMinArea(vectValues, quantify,  epsilon,  isClosed); 
  vector<LocalMinMaxArea> vectAreaMinMax =  getLocalMaxMinArea(vectValues, quantify,  epsilon,  isClosed);
  
  vector<LocalMinMaxArea> vectResult;
   
  int tailleContour = vectValues.size();
  int nbAreas = vectExtendedAreas.size();
 

  // quantification   
  vector<double> vectValuesModif;  

  for(int i =0;i<tailleContour; i++){
    if(quantify)
      vectValuesModif.push_back( ((double)((int)(vectValues.at(i)/epsilon)))*epsilon);       
    else
      vectValuesModif.push_back( vectValues.at(i));       
  }      
        

  
  for(int i=0; i< nbAreas; i++){    
    LocalMinMaxArea locSuivante = vectAreaMinMax.at((i+1)%nbAreas);
    LocalMinMaxArea loc = vectAreaMinMax.at(i);
    LocalMinMaxArea locPrec = vectAreaMinMax.at((i-1+nbAreas)%nbAreas);
    
    LocalMinMaxArea locSuivanteExt = vectExtendedAreas.at((i+1)%nbAreas);
    LocalMinMaxArea locExt = vectExtendedAreas.at(i);
    LocalMinMaxArea locPrecExt = vectExtendedAreas.at((i-1+nbAreas)%nbAreas);


    // Extension vers la droite de la zone courante:
    double valSuivante = vectValuesModif.at(locSuivante.indiceDebut%tailleContour);
    double valInitial = vectValuesModif.at(loc.indiceFin%tailleContour);
    int k = (loc.indiceFin+1)%tailleContour;
    bool stopped = false;
    
    while(!stopped){
      double valCourante = vectValuesModif.at(k%tailleContour);
      
      double d0 = abs((float)(valInitial-valCourante));
      double d1 = abs((float)(valSuivante-valCourante));
      if(((valCourante==0) &&(abs(valInitial)>abs(valSuivante)))||((valCourante==0.0)&&(!extendOnZerocurvature))||
	 (d0>d1 && valSuivante*valInitial>0) || valCourante*valInitial<0 || (k%tailleContour)==(locSuivanteExt.indiceDebut%tailleContour)){
	stopped=true;
      }else{
	vectExtendedAreas.at(i%nbAreas).indiceFin=k%tailleContour;
      }
      k++;
    }

    

    // Extension vers la gauche de la zone courante:
    double valPrec = vectValuesModif.at(locPrec.indiceFin%tailleContour);
    valInitial = vectValuesModif.at(loc.indiceDebut%tailleContour);

    k = (loc.indiceDebut+tailleContour)%tailleContour;

    stopped = false;
    while(!stopped){
      double valCourante = vectValuesModif.at((k+tailleContour)%tailleContour);
      double d0 = abs((float)(valInitial-valCourante));
      double d1 = abs((float)(valPrec-valCourante));
      if(((valCourante==0) &&(abs(valInitial)>abs(valPrec)))||((valCourante==0.0)&&(!extendOnZerocurvature))||
	 (d0>d1 && valPrec*valInitial>0) || (valCourante*valInitial<0) ||((k+tailleContour)%tailleContour)==(locPrecExt.indiceFin%tailleContour)){
	  stopped=true;
	}else{
	  vectExtendedAreas.at(i%nbAreas).indiceDebut=(k+tailleContour)%tailleContour;
      }
      
      
      k--;
    }
    // TODO Compléter les zones vides
    
  } 

return vectExtendedAreas;
  //return vectAreaMinMax;
}













  /**
   * Transform the initial curve such that it contains no variations
   * larger than ninVariations. The curve is transformed iteratively
   * from each local maximal areas. 
   * @param curve : the initial curve;
   * @param minVariations : the minimal variation authorized between two consecutive points.
   * @param isClosed: permits to take into accounds the type of curve.
   **/
  

vector<double>  
CurveVariationsHelper::getCurveWithLowVariationsFromMax(const vector<double> & curve, double minVariations, bool isClosed ){
  
  vector <double> curveResult;
  int tailleContour = curve.size();
  bool isChecked [tailleContour];
  
  // Initialisation du vecteur initial
  for(int i=0; i < tailleContour; i++){
    curveResult.push_back(curve.at(i));
    isChecked[i] = false;
  }
  
  // Récupération des indices des max locaux et création des zones initiales. 
  vector<int> vectIndMaxima = getLocalMaximaIndices(curve, false, 0.0, isClosed);
  vector<LocalMinMaxArea> vectInitAreas;
  for(int i=0; i < vectIndMaxima.size(); i++){
    LocalMinMaxArea loc;
    int indiceMax = vectIndMaxima.at(i);
    loc.indiceDebut = indiceMax;
    loc.indiceFin = indiceMax;
    loc.isMaxArea = true;
    vectInitAreas.push_back(loc);
  }
  int nbCheked =0;
  
  // Mise à jour du contour pour vérifier la contrainte
  bool hasChanged = true;
  while(nbCheked< tailleContour){  
    hasChanged=false;
    for(int i =0; i< vectInitAreas.size(); i++){
      LocalMinMaxArea  & loc = vectInitAreas.at(i);
      int indicePrec = loc.indiceDebut;
      int indiceSuiv = loc.indiceFin;
      double valPrec = curveResult.at(indicePrec);
      double valSuiv = curveResult.at(indiceSuiv);
      
      bool precExt = true;
      bool suivExt = true;

      // si la courbe n'est pas fermée on ne continue au delà des extrémités
      if((!isClosed && (indicePrec==0))|| isChecked[indicePrec]){
	precExt=false;
      }
      if((!isClosed && (indiceSuiv==tailleContour-1))|| isChecked[indiceSuiv]){
	suivExt=false;
      }      
      
      if(valPrec>(curveResult.at((indicePrec-1+tailleContour)%tailleContour)+minVariations)&& precExt){
	hasChanged=true;
	curveResult.at((indicePrec-1+tailleContour)%tailleContour)= valPrec-minVariations;
      }
      if(valSuiv>(curveResult.at((indiceSuiv+1)%tailleContour)+minVariations) && suivExt){
	hasChanged=true;
	curveResult.at((indiceSuiv+1)%tailleContour)= valSuiv-minVariations;
      }
      if(precExt){
	loc.indiceDebut = (loc.indiceDebut-1+tailleContour)%tailleContour; 
	if(!isChecked[indicePrec]){
	  isChecked[indicePrec]=true;
	  nbCheked ++;
	}
      }
      if(suivExt){
	loc.indiceFin = (loc.indiceFin+1)%tailleContour; 
	if(!isChecked[indiceSuiv]){
	  isChecked[indiceSuiv]=true;
	  nbCheked ++;
	}
      }



    }    
  }
  

  return curveResult;
}



