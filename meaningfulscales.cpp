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
#include "ImaGene/helper/MultiscaleProfile.h"
#include "ImaGene/helper/ShapeHelper.h"

#include <algorithm>
#include "ImaGene/dgeometry2d/C4CGeometry.h"
#include "ImaGene/dgeometry2d/C4CSegmentPencil.h"
#include "ImaGene/base/Vector2i.h"
#include "ImaGene/mathutils/Mathutils.h"
#include "ImaGene/mathutils/SimpleLinearRegression.h"
#include "ImaGene/dgeometry2d/C4CIteratorOnFreemanChain.h"
#include "ImaGene/dgeometry2d/C4CTangentialCover.h"
#include "ImaGene/dgeometry2d/FreemanChain.h"
#include "ImaGene/dgeometry2d/FreemanChainTransform.h"
#include "ImaGene/digitalnD/C4CIteratorOnFreemanChainSurface.h"
#include "ImaGene/digitalnD/C4CIteratorOnSurface.h"
#include "ImaGene/digitalnD/KnSpace.h"
#include "ImaGene/helper/C4CTangentialCoverGeometry.h"
#include "ImaGene/helper/CurveVariationsHelper.h"
#include "ImaGene/helper/DrawingXFIG.h"

#include <istream>
#include <sstream>

using namespace ImaGene;


MeaningfulScales::MeaningfulScales (QImage &image) : input (image){
    samplingSizeMax = 7;

    inputWidth = input.width ();
    inputHeight = input.height ();
    getFreemanChain ();

    FreemanChain::read (freemanChainStream, freemanChain);
    if (!freemanChainStream.good ()){
        qDebug () << "Error reading Freeman chain code.";
        return;
    }

    //Computing the noise level for each pixel:

    int nbIterationSpikes = 3;

    FreemanChainSubsample fcsub( 1, 1, 0, 0 );
    FreemanChainCleanSpikesCCW fccs( nbIterationSpikes );
    FreemanChainCompose fcomp( fccs, fcsub );
    FreemanChainTransform* ptr_fct = &fcomp;
    FreemanChainSubsample* ptr_fcsub = &fcsub;

    MP.chooseSubsampler( *ptr_fct, *ptr_fcsub );

    //qDebug () << "Contour size: " << freemanChain.chain.size () << " surfels";
    //qDebug () << "Sampling size max used: " << samplingSizeMax;
    if (freemanChain.chain.size() < MinimalFreemanChainSize)
        return;

    MP.init (freemanChain, samplingSizeMax);
}


void MeaningfulScales::getFreemanChain (){
    int theLongestLength = 0, currentLength;
    string theLongestFreemanChain = "";

    bool yInverted = false;//args.check("-invertVerticalAxis");
    KnSpace* ks;
    KnCharSet* voxset;
    uint threshold = 127;
    getPGMStream (RED);
    if ( ! ShapeHelper::importFromPGM (pgmFile, ks, voxset, threshold, 0, true)){
        qDebug () << "Error reading PGM file.";
        return;
    }

    Vector2i ptReference;
    double distanceMax=0.0;

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

    freemanChainStream << theLongestFreemanChain;
}


void MeaningfulScales::getPGMStream (int channel = BLUE){
    const unsigned char* imagePointer = input.constBits ();
    switch (channel){
        case GREEN:
            imagePointer += 1;
            break;
        case BLUE:
            imagePointer += 2;
    }
    const unsigned char* imageEndPointer = imagePointer + inputWidth * inputHeight * 4;

    int maxValue = 255;
    pgmFile << "P5\n" << inputWidth << " " << inputHeight << "\n" << maxValue << "\n";
    unsigned int i = 0;
    while (imagePointer < imageEndPointer){
        pgmFile << (unsigned char)*imagePointer;
        imagePointer += 4;
        i++;
    }
    pgmFile << "\n";
}


QImage MeaningfulScales::GetResult (){
    QPen littleNoise (QColor (0, 255, 0, 50));
    QPen someNoise (QColor (255, 255, 0, 50));
    QPen outOfScaleNoise (QColor (255, 0, 0, 50));

    boxes = QImage (inputWidth, inputHeight, QImage::Format_ARGB32_Premultiplied);
    boxes.fill (QColor (0, 0, 0, 0));
    QPainter painter;
    painter.begin (&boxes);
    painter.setBrush (QBrush (Qt::NoBrush));

    unsigned int i = 0;
    for (FreemanChain::const_iterator it = freemanChain.begin (); it != freemanChain.end (); ++it){
        uint noiseLevel = MP.noiseLevel (i, mscales_min_size, mscales_max_slope);
        if (noiseLevel == 0){
            painter.setPen (outOfScaleNoise);
            painter.drawRect ((*it).x () - samplingSizeMax, (*it).y () - samplingSizeMax, samplingSizeMax * 2, samplingSizeMax * 2);
        } else if (noiseLevel == 1){
            painter.setPen(littleNoise);
            painter.drawRect ((*it).x () - noiseLevel, (*it).y () - noiseLevel, noiseLevel * 2, noiseLevel * 2);
        } else if (noiseLevel >= 2){
            painter.setPen(someNoise);
            painter.drawRect ((*it).x () - noiseLevel, (*it).y () - noiseLevel, noiseLevel * 2, noiseLevel * 2);
        }
        i++;
    }
    painter.end ();

    return boxes;
}


unsigned int MeaningfulScales::GetFreemanChainSize (){
    return freemanChain.size ();
}


unsigned int MeaningfulScales::GetSumOfNoise (){
    unsigned int result = 0;
    unsigned int freemanChainSize = freemanChain.size ();
    for (unsigned int i = 0; i < freemanChainSize; i++){
        uint noiseLevel = MP.noiseLevel (i, mscales_min_size, mscales_max_slope);
        if (noiseLevel == 0)
            result += samplingSizeMax;
        else
            result += noiseLevel;
    }
    return result;
}
