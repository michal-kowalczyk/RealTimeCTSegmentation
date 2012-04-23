#include "meaningfulscales.h"
#include <QImage>
#include <QPainter>
#include <QDebug>

#include "ImaGene/base/Proxy.h"
#include "ImaGene/base/Vector.h"
#include "ImaGene/base/Shapes.h"
#include "ImaGene/digitalnD/GridEmbedder.h"
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/digitalnD/KnCharSet.h"
#include "ImaGene/digitalnD/KnRCellSet.h"
#include "ImaGene/digitalnD/KnShapes.h"
#include "ImaGene/digitalnD/C4CIteratorOnBdry.h"
#include "ImaGene/digitalnD/BelAdjacency.h"
#include "ImaGene/digitalnD/ObjectBoundary.h"
#include "ImaGene/digitalnD/Frame2D.h"
#include "ImaGene/helper/ContourHelper.h"
#include "ImaGene/helper/ShapeHelper.h"

#include <algorithm>
#include "ImaGene/dgeometry2d/C4CGeometry.h"
#include "ImaGene/dgeometry2d/C4CSegmentPencil.h"
#include "ImaGene/base/Vector2i.h"
#include "ImaGene/mathutils/Mathutils.h"
#include "ImaGene/mathutils/Statistics.h"
#include "ImaGene/mathutils/SimpleLinearRegression.h"
#include "ImaGene/dgeometry2d/C4CIteratorOnFreemanChain.h"
#include "ImaGene/dgeometry2d/C4CTangentialCover.h"
#include "ImaGene/dgeometry2d/FreemanChain.h"
#include "ImaGene/dgeometry2d/FreemanChainTransform.h"
#include "ImaGene/digitalnD/C4CIteratorOnFreemanChainSurface.h"
#include "ImaGene/digitalnD/C4CIteratorOnSurface.h"
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/helper/MultiscaleProfile.h"
#include "ImaGene/helper/C4CTangentialCoverGeometry.h"
#include "ImaGene/helper/CurveVariationsHelper.h"
#include "ImaGene/helper/DrawingXFIG.h"

#include <istream>
#include <sstream>

using namespace ImaGene;


uint estimMaxSamplingSize (FreemanChain fc){//MeaningfulScales::
  int minX = 0;
  int minY = 0;
  int maxX = 0;
  int maxY = 0;

  fc.computeBoundingBox(minX, minY, maxX, maxY);
  int largeur = maxX-minX;
  int hauteur = maxY-minY;

  return min( largeur/4, hauteur/4);
}


MeaningfulScales::MeaningfulScales(QImage image){
    stringstream stream;
    GetFreemanChain(stream, image);

    static int samplingSizeMax = 6;//10;

    uint mscales_min_size = 1;///args.getOption( "-meaningfulScale" )->getIntValue( 0 );
    double mscales_max_slope = 0;//args.getOption( "-meaningfulScale" )->getDoubleValue( 1 );

    // -------------------------------------------------------------------------
    // Read Freeman chain and creates space/contour.
    FreemanChain fc;
    //istream & in_str = StandardArguments::openInput( args );
    FreemanChain::read( stream, fc );
    if ( ! stream.good() )
      {
        qDebug () << "Error reading Freeman chain code.";
        return;//2
      }

    //uint samplingSizeMaxEstim = estimMaxSamplingSize(fc);

    /*if (samplingSizeMaxEstim < samplingSizeMax)
      samplingSizeMax= samplingSizeMaxEstim;*/

    /*if(args.check("-drawContourSRC")){
      uint color = args.getOption("-drawContourSRC")->getIntValue(0);
      uint linewidth = args.getOption("-drawContourSRC")->getIntValue(1);
      DrawingXFIG::setFillIntensity(100);
      DrawingXFIG::drawContour(args.check("-setFileNameFigure")? ofFig :cout, fc, color, linewidth,0,0 , 2);
    }*/

    //Computing the noise level for each pixel:

    int nbIterationSpikes = 3;

    FreemanChainSubsample fcsub( 1, 1, 0, 0 );
    FreemanChainCleanSpikesCCW fccs( nbIterationSpikes );
    FreemanChainCompose fcomp( fccs, fcsub );
    FreemanChainTransform* ptr_fct = &fcomp;
    FreemanChainSubsample* ptr_fcsub = &fcsub;

    MultiscaleProfile MP;
    MP.chooseSubsampler( *ptr_fct, *ptr_fcsub );

    qDebug () << "Contour size: " << fc.chain.size() << " surfels";
    qDebug () << "Sampling size max used: " << samplingSizeMax;
    if (fc.chain.size() < 300)
        return;


    MP.init( fc, samplingSizeMax );

    QPen littleNoise (QColor (0, 255, 0, 50));
    QPen someNoise (QColor (255, 255, 0, 50));
    QPen outOfScaleNoise (QColor(255, 0, 0, 50));


    boxes = QImage (image.width(), image.height(), QImage::Format_ARGB32_Premultiplied);
    boxes.fill(QColor(0, 0, 0, 0));
    QPainter painter;
    painter.begin(&boxes);
    painter.setBrush(QBrush(Qt::NoBrush));
    unsigned int i = 0;

    unsigned int minNoiseLevel = 5, maxNoiseLevel = 0;

    for (FreemanChain::const_iterator it = fc.begin () ; it != fc.end (); ++it){
      uint noiseLevel = MP.noiseLevel(i, mscales_min_size, mscales_max_slope);
      if (minNoiseLevel > noiseLevel)
          minNoiseLevel = noiseLevel;
      if (maxNoiseLevel < noiseLevel)
          maxNoiseLevel = noiseLevel;

      if (noiseLevel == 0){
          painter.setPen (outOfScaleNoise);
          painter.drawRect ((*it).x() - samplingSizeMax, (*it).y() - samplingSizeMax, samplingSizeMax * 2, samplingSizeMax * 2);
      } else if (noiseLevel == 1){
          painter.setPen(littleNoise);
          painter.drawRect ((*it).x() - noiseLevel, (*it).y() - noiseLevel, noiseLevel * 2, noiseLevel * 2);
      } else if (noiseLevel >= 2){
          painter.setPen(someNoise);
          painter.drawRect ((*it).x() - noiseLevel, (*it).y() - noiseLevel, noiseLevel * 2, noiseLevel * 2);
      }
        i++;
    }
    painter.end();
    qDebug () << minNoiseLevel << maxNoiseLevel;
}


void MeaningfulScales::GetFreemanChain (ostream &result, QImage image){
    stringstream pgm (ios_base::in | ios_base::out);

    int theLongestLength = 0, currentLength;
    string theLongestFreemanChain = "";


    bool yInverted = false;//args.check("-invertVerticalAxis");
    KnSpace* ks;
    KnCharSet* voxset;
    uint threshold = 127;//(uint) args.getOption( "-threshold" )->getIntValue( 0 );
    GetPGMStream(pgm, image, RED);
    if ( ! ShapeHelper::importFromPGM (pgm, ks, voxset, threshold, 0, true)){
      qDebug () << "Error reading PGM file.";
      return;
//      throw exception ();//PGM file corrupted!
    }

    Vector2i ptReference;
    double distanceMax=0.0;
    //Rajout (BK)
    /*if(args.check("-selectContour")){
      ptReference.x()= args.getOption("-selectContour")->getIntValue(0);
      ptReference.y()= args.getOption("-selectContour")->getIntValue(1);
      distanceMax= args.getOption("-selectContour")->getIntValue(2);
    }*/

    bool interior = 0;//args.getOption( "-badj" )->getIntValue( 0 ) == 0;
    uint min_size = 0;//args.getOption( "-min_size" )->getIntValue( 0 );
    BelAdjacency badj( *ks, interior );
    KnRCellSet bdry = KnShapes::smakeBoundary( *ks, *voxset );
    KnRCellSet not_visited( bdry );
    uint num_contour = 0;
    for ( KnRCellSet::cell_iterator cell_it = bdry.begin();
          cell_it != bdry.end();
          ++cell_it )
      {
        Kn_sid bel = *cell_it;
        uint k = *( ks->sbegin_dirs( bel ) );
        C4CIteratorOnBdry c4c_it( badj, bel, k, *voxset );
        bool is_open;
        uint nb_surfels = C4CIterator::size( c4c_it, is_open );
        if ( nb_surfels >= min_size )
          {
            Proxy<C4CIteratorOnSurface> cp
              ( (C4CIteratorOnSurface*) c4c_it.clone() );
/*            if(!args.check("-selectContour")){
            {
              stringstream stringStream (stringstream::in | stringstream::out);
              ContourHelper::displayFreemanChain( stringStream, ks, cp, 0, 1, yInverted );
//              string result123;
//              stringStream >> result123;
              qDebug () << stringStream;//result123;
            }*/
//            }else{
              //Rajout option (BK 29/07/09)
              Frame2D frame;
              frame.init( ks, 0, 1 );
              Kn_sid sbel = cp->current();
              frame.setSurfelFrame( sbel, cp->trackDir() );
              Vector2i p1( frame.transformPoint( Vector2i( 0, 0 ) ) );
              double distance = sqrt((p1.x() - ptReference.x())*(p1.x() - ptReference.x())+
                                     (p1.y() - ptReference.y())*(p1.y() - ptReference.y()));
       //      if(distance< distanceMax){
                stringstream currentFreemanChain (stringstream::in | stringstream::out);
                ContourHelper::displayFreemanChain( currentFreemanChain, ks, cp, 0, 1, yInverted );
                string freemanChain = currentFreemanChain.str();
                currentLength = freemanChain.size();
                if (theLongestLength < currentLength){
                    theLongestFreemanChain = freemanChain;
                    theLongestLength = currentLength;
                }
         //     }
//            }*/
          }
        // Clear contour from set of bels.
        bel = c4c_it.current();
        Kn_sid sbel = bel;
        do
          {
            bdry[ bel ] = false;
            if ( c4c_it.next() == 0 ) break;
            bel = c4c_it.current();
          }
        while ( bel != sbel );

        num_contour++;
    }

    result << theLongestFreemanChain;
}


void MeaningfulScales::GetPGMStream(ostream &stream, QImage image, int channel = BLUE){
    const unsigned char* bits = image.constBits ();
    unsigned int offset, area = image.width () * image.height ();
    if (channel == RED)
        offset = 0;
    else if (channel == GREEN)
        offset = 1;
    else if (channel == BLUE)
        offset = 2;
    else{
        offset = 0;
//        throw exception ();//unknown channel!
    }

    int maxValue = 255;
    //for (unsigned int i = offset; i < bytesNumber; i += 4)
    //    if (maxValue < bits[i])
    //        maxValue = bits[i];

    /*if (maxValue == 0)
        return result;*/

    stream << "P5\n" << image.width () << " " << image.height () << "\n" << maxValue << "\n";
    for (unsigned int i = 0; i < area; i++)
        stream << (unsigned char)bits[i * 4];

    /*stream << "P5" << "\n" << "24" << " " << "7" << "\n" << "15" << "\n";

    int row1[] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};
    for (int i = 0; i < 24; i++)
        stream << (unsigned char)row1[i];
    int row2[] = {0,  3,  3,  3,  3,  0,  0,  7,  7,  7,  7,  0,  0, 11, 11, 11, 11,  0,  0, 15, 15, 15, 15,  0};
    for (int i = 0; i < 24; i++)
        stream << (unsigned char)row2[i];
    int row3[] = {0,  3,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0, 11,  0,  0,  0,  0,  0, 15,  0,  0, 15,  0};
    for (int i = 0; i < 24; i++)
        stream << (unsigned char)row3[i];
    int row4[] = {0,  3,  3,  3,  0,  0,  0,  7,  7,  7,  0,  0,  0, 11, 11, 11,  0,  0,  0, 15, 15, 15, 15,  0};
    for (int i = 0; i < 24; i++)
        stream << (unsigned char)row4[i];
    int row5[] = {0,  3,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0, 11,  0,  0,  0,  0,  0, 15,  0,  0,  0,  0};
    for (int i = 0; i < 24; i++)
        stream << (unsigned char)row5[i];
    int row6[] = {0,  3,  0,  0,  0,  0,  0,  7,  7,  7,  7,  0,  0, 11, 11, 11, 11,  0,  0, 15,  0,  0,  0,  0};
    for (int i = 0; i < 24; i++)
        stream << (unsigned char)row6[i];
    int row7[] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};
    for (int i = 0; i < 24; i++)
        stream << (unsigned char)row7[i];*/

    stream << "\n";
}

QImage MeaningfulScales::GetResult()
{
    return boxes;
}
