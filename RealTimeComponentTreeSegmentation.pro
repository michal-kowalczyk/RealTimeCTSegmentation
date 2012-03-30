# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
DEPLOYMENTFOLDERS = # file1 dir1

symbian:TARGET.UID3 = 0xEACB23CA

# Smart Installer package's UID
# This UID is from the protected range 
# and therefore the package will fail to install if self-signed
# By default qmake uses the unprotected range value if unprotected UID is defined for the application
# and 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
# CONFIG += mobility
# MOBILITY +=

QT_GRAPHICSSYSTEM = opengl

#LIBS += -L"/Users/michalkowalczyk/RealTimeComponentTreeSegmentation" -hello-jni #$$PWD

QMAKE_CXXFLAGS_RELEASE+= -O2
QMAKE_CXXFLAGS += -fexceptions

SOURCES += main.cpp mainwindow.cpp \
    camerasupport.cpp \
    JavaClassesLoader.cpp \
    ImaGene/3d/ImplicitSurfaces3D.cxx \
    ImaGene/3d/ImplicitSurfaces.cxx \
    ImaGene/3d/ImplicitSurfaceProjector.cxx \
    ImaGene/3d/ImplicitSurfaceHelper.cxx \
    ImaGene/3d/ImplicitSurfaceEmbedder.cxx \
    ImaGene/3d/ImplicitSurface.cxx \
    ImaGene/3d/ImplicitExtension.cxx \
    ImaGene/3d/ImplicitDigitalSurfaceTracker.cxx \
    ImaGene/3d/ImplicitDigitalSurface.cxx \
    ImaGene/3d/C4CIteratorOnImplicitDigitalSurface.cxx \
    ImaGene/base/WordUtils.cxx \
    ImaGene/base/VectorUtils.cxx \
    ImaGene/base/Vector2i.cxx \
    ImaGene/base/Vector.cxx \
    ImaGene/base/UndoableList.cxx \
    ImaGene/base/StaticHashTable.cxx \
    ImaGene/base/StandardHashTable.cxx \
    ImaGene/base/StandardArguments.cxx \
    ImaGene/base/SmartSet.cxx \
    ImaGene/base/Signal.cxx \
    ImaGene/base/Shapes.cxx \
    ImaGene/base/RnMaps.cxx \
    ImaGene/base/Proxy.cxx \
    ImaGene/base/OrderedAlphabet.cxx \
    ImaGene/base/Matrix.cxx \
    ImaGene/base/HashTable.cxx \
    ImaGene/base/Arguments.cxx \
    ImaGene/dgeometry2d/OctantChange.cxx \
    ImaGene/dgeometry2d/MultiscaleFreemanChain.cxx \
    ImaGene/dgeometry2d/GridCurve.cxx \
    ImaGene/dgeometry2d/GridContour.cxx \
    ImaGene/dgeometry2d/FreemanFrame2D.cxx \
    ImaGene/dgeometry2d/FreemanChainTransform.cxx \
    ImaGene/dgeometry2d/FreemanChain.cxx \
    ImaGene/dgeometry2d/EuclideanGeometry.cxx \
    ImaGene/dgeometry2d/DMLPContour.cxx \
    ImaGene/dgeometry2d/DLine.cxx \
    ImaGene/dgeometry2d/CurveCode.cxx \
    ImaGene/dgeometry2d/C4CTangentialCover.cxx \
    ImaGene/dgeometry2d/C4CSegmentPencil.cxx \
    ImaGene/dgeometry2d/C4CSegment.cxx \
    ImaGene/dgeometry2d/C4CIteratorOnFreemanChain.cxx \
    ImaGene/dgeometry2d/C4CIterator.cxx \
    ImaGene/dgeometry2d/C4CGeometry.cxx \
    ImaGene/dgeometry2d/C4CELength.cxx \
    ImaGene/dgeometry2d/BlurredSegmentTgtCover.cxx \
    ImaGene/digitalnD/SurfelNeighborhood.cxx \
    ImaGene/digitalnD/SmoothingEmbedder.cxx \
    ImaGene/digitalnD/Sanitizer.cxx \
    ImaGene/digitalnD/ObjectBoundaryTracker.cxx \
    ImaGene/digitalnD/ObjectBoundary.cxx \
    ImaGene/digitalnD/NormalOnSurfaceComputerByUmbrellas.cxx \
    ImaGene/digitalnD/MeanCurvatureOnSurfaceComputerByDG.cxx \
    ImaGene/digitalnD/KnUtils.cxx \
    ImaGene/digitalnD/KnTypes.cxx \
    ImaGene/digitalnD/KnTools.cxx \
    ImaGene/digitalnD/KnSpaceScanner.cxx \
    ImaGene/digitalnD/KnSpaceCoordScanner.cxx \
    ImaGene/digitalnD/KnSpace.cxx \
    ImaGene/digitalnD/KnShapes.cxx \
    ImaGene/digitalnD/KnRCellVector.cxx \
    ImaGene/digitalnD/KnRCellSet.cxx \
    ImaGene/digitalnD/KnCharSet.cxx \
    ImaGene/digitalnD/K3Shapes.cxx \
    ImaGene/digitalnD/K3Isosurface.cxx \
    ImaGene/digitalnD/K3Isosurface_k6_l18.cxx \
    ImaGene/digitalnD/K2Space.cxx \
    ImaGene/digitalnD/GridEmbedder.cxx \
    ImaGene/digitalnD/GeometryComputerByContour4.cxx \
    ImaGene/digitalnD/Frame2D.cxx \
    ImaGene/digitalnD/ExplicitDigitalSurfaceTracker.cxx \
    ImaGene/digitalnD/ExplicitDigitalSurface.cxx \
    ImaGene/digitalnD/Embedder.cxx \
    ImaGene/digitalnD/DigitalSurfaceUmbrellaComputer.cxx \
    ImaGene/digitalnD/DigitalSurfaceUmbrella.cxx \
    ImaGene/digitalnD/DigitalSurfaceGeometry.cxx \
    ImaGene/digitalnD/DigitalSurface.cxx \
    ImaGene/digitalnD/CubicalSpace.cxx \
    ImaGene/digitalnD/CubicalComplex.cxx \
    ImaGene/digitalnD/CubicalCell.cxx \
    ImaGene/digitalnD/CentroidEmbedder.cxx \
    ImaGene/digitalnD/C4CIteratorOnFreemanChainSurface.cxx \
    ImaGene/digitalnD/C4CIteratorOnExplicitDigitalSurface.cxx \
    ImaGene/digitalnD/C4CIteratorOnBdry.cxx \
    ImaGene/digitalnD/BoxEmbedder.cxx \
    ImaGene/digitalnD/Bitset1.cxx \
    ImaGene/digitalnD/BelAdjacency.cxx \
    ImaGene/digitalnD/AreaOnSurfaceComputerByEuler.cxx \
    ImaGene/helper/VisualCurvature.cxx \
    ImaGene/helper/ShapeHelper.cxx \
    ImaGene/helper/ScaleProfile.cxx \
    ImaGene/helper/MultiscaleProfile.cxx \
    ImaGene/helper/MeaningfulContinuation.cxx \
    ImaGene/helper/LinearMinimizer.cxx \
    ImaGene/helper/ImageMultiscaleAnalysis.cxx \
    ImaGene/helper/GlobalC4CGeometry.cxx \
    ImaGene/helper/DrawingXFIG.cxx \
    ImaGene/helper/DigitalElevationMapStats.cxx \
    ImaGene/helper/DigitalElevationMap.cxx \
    ImaGene/helper/CurveVariationsHelper.cxx \
    ImaGene/helper/ContourHelper.cxx \
    ImaGene/helper/Color.cxx \
    ImaGene/helper/CharacteristicPolygon.cxx \
    ImaGene/helper/ChangeQuadrant.cxx \
    ImaGene/helper/C4CTangentialCoverGeometry.cxx \
    ImaGene/image/Pixel.cxx \
    ImaGene/image/PGMFilter.cxx \
    ImaGene/image/Image2DUtils.cxx \
    ImaGene/image/Image2D.cxx \
    ImaGene/mathutils/SternBrocot.cxx \
    ImaGene/mathutils/Statistics.cxx \
    ImaGene/mathutils/Statistic.cxx \
    ImaGene/mathutils/SimpleLinearRegression.cxx \
    ImaGene/mathutils/SampledFunction.cxx \
    ImaGene/mathutils/PWFAffineFunction.cxx \
    ImaGene/mathutils/Polynomial3.cxx \
    ImaGene/mathutils/Polynomial.cxx \
    ImaGene/mathutils/MeasureOfStraightLines.cxx \
    ImaGene/mathutils/Mathutils.cxx \
    ImaGene/mathutils/Line2D.cxx \
    ImaGene/mathutils/Lattice.cxx \
    ImaGene/mathutils/G.cxx \
    ImaGene/mathutils/Functions.cxx \
    ImaGene/mathutils/DG.cxx \
    ImaGene/mathutils/CFraction.cxx \
    ImaGene/timetools/Clock.cxx \
    ImaGene/timetools/Alarm.cxx \
    meaningfulscales.cpp
HEADERS += mainwindow.h\
    include/ImageIterators.h \
    include/ImageIO.hxx \
    include/ImageIO_InrGz.hxx \
    include/Image.hxx \
    include/Image.h \
    include/FlatSE.hxx \
    include/FlatSE.h \
    include/ComponentTree.hxx \
    include/ComponentTree.h \
    include/Morphology.hxx \
    include/Morphology.h \
    include/OrderedQueue.h \
    camerasupport.h \
    ImaGene/3d/ImplicitSurfaces3D.ih \
    ImaGene/3d/ImplicitSurfaces3D.h \
    ImaGene/3d/ImplicitSurfaces.ih \
    ImaGene/3d/ImplicitSurfaces.h \
    ImaGene/3d/ImplicitSurfaceProjector.ih \
    ImaGene/3d/ImplicitSurfaceProjector.h \
    ImaGene/3d/ImplicitSurfaceHelper.ih \
    ImaGene/3d/ImplicitSurfaceHelper.h \
    ImaGene/3d/ImplicitSurfaceEmbedder.ih \
    ImaGene/3d/ImplicitSurfaceEmbedder.h \
    ImaGene/3d/ImplicitSurface.ih \
    ImaGene/3d/ImplicitSurface.h \
    ImaGene/3d/ImplicitExtension.ih \
    ImaGene/3d/ImplicitExtension.h \
    ImaGene/3d/ImplicitDigitalSurfaceTracker.ih \
    ImaGene/3d/ImplicitDigitalSurfaceTracker.h \
    ImaGene/3d/ImplicitDigitalSurface.ih \
    ImaGene/3d/ImplicitDigitalSurface.h \
    ImaGene/3d/C4CIteratorOnImplicitDigitalSurface.ih \
    ImaGene/3d/C4CIteratorOnImplicitDigitalSurface.h \
    ImaGene/base/WordUtils.ih \
    ImaGene/base/WordUtils.h \
    ImaGene/base/VectorUtils.ih \
    ImaGene/base/VectorUtils.h \
    ImaGene/base/Vector2i.ih \
    ImaGene/base/Vector2i.h \
    ImaGene/base/Vector.ih \
    ImaGene/base/Vector.h \
    ImaGene/base/UndoableList.ih \
    ImaGene/base/UndoableList.h \
    ImaGene/base/TypedObject.h \
    ImaGene/base/StaticHashTable.ih \
    ImaGene/base/StaticHashTable.h \
    ImaGene/base/StandardHashTable.ih \
    ImaGene/base/StandardHashTable.h \
    ImaGene/base/StandardArguments.ih \
    ImaGene/base/StandardArguments.h \
    ImaGene/base/SmartSet.ih \
    ImaGene/base/SmartSet.h \
    ImaGene/base/Signal.ih \
    ImaGene/base/Signal.h \
    ImaGene/base/Shared.h \
    ImaGene/base/Shapes.ih \
    ImaGene/base/Shapes.h \
    ImaGene/base/RnMaps.ih \
    ImaGene/base/RnMaps.h \
    ImaGene/base/Proxy.ih \
    ImaGene/base/Proxy.h \
    ImaGene/base/OrderedAlphabet.ih \
    ImaGene/base/OrderedAlphabet.h \
    ImaGene/base/Matrix.ih \
    ImaGene/base/Matrix.h \
    ImaGene/base/HashTable.ih \
    ImaGene/base/HashTable.h \
    ImaGene/base/CowPtr.ih \
    ImaGene/base/CowPtr.h \
    ImaGene/base/CountedPtr.ih \
    ImaGene/base/CountedPtr.h \
    ImaGene/base/BasicTypes.h \
    ImaGene/base/Arguments.ih \
    ImaGene/base/Arguments.h \
    ImaGene/dgeometry2d/OctantChange.ih \
    ImaGene/dgeometry2d/OctantChange.h \
    ImaGene/dgeometry2d/MultiscaleFreemanChain.ih \
    ImaGene/dgeometry2d/MultiscaleFreemanChain.h \
    ImaGene/dgeometry2d/GridCurve.ih \
    ImaGene/dgeometry2d/GridCurve.h \
    ImaGene/dgeometry2d/GridContour.ih \
    ImaGene/dgeometry2d/GridContour.h \
    ImaGene/dgeometry2d/FreemanFrame2D.ih \
    ImaGene/dgeometry2d/FreemanFrame2D.h \
    ImaGene/dgeometry2d/FreemanChainTransform.ih \
    ImaGene/dgeometry2d/FreemanChainTransform.h \
    ImaGene/dgeometry2d/FreemanChain.ih \
    ImaGene/dgeometry2d/FreemanChain.h \
    ImaGene/dgeometry2d/EuclideanGeometry.ih \
    ImaGene/dgeometry2d/EuclideanGeometry.h \
    ImaGene/dgeometry2d/DMLPContour.ih \
    ImaGene/dgeometry2d/DMLPContour.h \
    ImaGene/dgeometry2d/DLine.ih \
    ImaGene/dgeometry2d/DLine.h \
    ImaGene/dgeometry2d/CurveCode.ih \
    ImaGene/dgeometry2d/CurveCode.h \
    ImaGene/dgeometry2d/C4CTangentialCover.ih \
    ImaGene/dgeometry2d/C4CTangentialCover.h \
    ImaGene/dgeometry2d/C4CSegmentPencil.ih \
    ImaGene/dgeometry2d/C4CSegmentPencil.h \
    ImaGene/dgeometry2d/C4CSegment.ih \
    ImaGene/dgeometry2d/C4CSegment.h \
    ImaGene/dgeometry2d/C4CIteratorOnFreemanChain.ih \
    ImaGene/dgeometry2d/C4CIteratorOnFreemanChain.h \
    ImaGene/dgeometry2d/C4CIterator.ih \
    ImaGene/dgeometry2d/C4CIterator.h \
    ImaGene/dgeometry2d/C4CGeometry.ih \
    ImaGene/dgeometry2d/C4CGeometry.h \
    ImaGene/dgeometry2d/C4CELength.ih \
    ImaGene/dgeometry2d/C4CELength.h \
    ImaGene/dgeometry2d/BlurredSegmentTgtCover.ih \
    ImaGene/dgeometry2d/BlurredSegmentTgtCover.h \
    ImaGene/digitalnD/SurfelNeighborhood.ih \
    ImaGene/digitalnD/SurfelNeighborhood.h \
    ImaGene/digitalnD/SmoothingEmbedder.ih \
    ImaGene/digitalnD/SmoothingEmbedder.h \
    ImaGene/digitalnD/Sanitizer.ih \
    ImaGene/digitalnD/Sanitizer.h \
    ImaGene/digitalnD/ObjectBoundaryTracker.ih \
    ImaGene/digitalnD/ObjectBoundaryTracker.h \
    ImaGene/digitalnD/ObjectBoundary.ih \
    ImaGene/digitalnD/ObjectBoundary.h \
    ImaGene/digitalnD/NormalOnSurfaceComputerByUmbrellas.ih \
    ImaGene/digitalnD/NormalOnSurfaceComputerByUmbrellas.h \
    ImaGene/digitalnD/MeanCurvatureOnSurfaceComputerByDG.ih \
    ImaGene/digitalnD/MeanCurvatureOnSurfaceComputerByDG.h \
    ImaGene/digitalnD/KnUtils.ih \
    ImaGene/digitalnD/KnUtils.h \
    ImaGene/digitalnD/KnTypes.h \
    ImaGene/digitalnD/KnTools.ih \
    ImaGene/digitalnD/KnTools.h \
    ImaGene/digitalnD/KnSpaceScanner.ih \
    ImaGene/digitalnD/KnSpaceScanner.h \
    ImaGene/digitalnD/KnSpaceCoordScanner.ih \
    ImaGene/digitalnD/KnSpaceCoordScanner.h \
    ImaGene/digitalnD/KnSpace.ih \
    ImaGene/digitalnD/KnSpace.h \
    ImaGene/digitalnD/KnShapes.ih \
    ImaGene/digitalnD/KnShapes.h \
    ImaGene/digitalnD/KnRCellVector.ih \
    ImaGene/digitalnD/KnRCellVector.h \
    ImaGene/digitalnD/KnRCellSet.ih \
    ImaGene/digitalnD/KnRCellSet.h \
    ImaGene/digitalnD/KnCharSet.ih \
    ImaGene/digitalnD/KnCharSet.h \
    ImaGene/digitalnD/K3Shapes.ih \
    ImaGene/digitalnD/K3Shapes.h \
    ImaGene/digitalnD/K3Isosurface.ih \
    ImaGene/digitalnD/K3Isosurface.h \
    ImaGene/digitalnD/K3Isosurface_k6_l18.ih \
    ImaGene/digitalnD/K3Isosurface_k6_l18.h \
    ImaGene/digitalnD/K2Space.ih \
    ImaGene/digitalnD/K2Space.h \
    ImaGene/digitalnD/GridEmbedder.ih \
    ImaGene/digitalnD/GridEmbedder.h \
    ImaGene/digitalnD/GeometryComputerByContour4.ih \
    ImaGene/digitalnD/GeometryComputerByContour4.h \
    ImaGene/digitalnD/Frame2D.ih \
    ImaGene/digitalnD/Frame2D.h \
    ImaGene/digitalnD/ExplicitDigitalSurfaceTracker.ih \
    ImaGene/digitalnD/ExplicitDigitalSurfaceTracker.h \
    ImaGene/digitalnD/ExplicitDigitalSurface.ih \
    ImaGene/digitalnD/ExplicitDigitalSurface.h \
    ImaGene/digitalnD/Embedder.h \
    ImaGene/digitalnD/DigitalSurfaceUmbrellaComputer.ih \
    ImaGene/digitalnD/DigitalSurfaceUmbrellaComputer.h \
    ImaGene/digitalnD/DigitalSurfaceUmbrella.ih \
    ImaGene/digitalnD/DigitalSurfaceUmbrella.h \
    ImaGene/digitalnD/DigitalSurfaceTracker.h \
    ImaGene/digitalnD/DigitalSurfaceGeometry.ih \
    ImaGene/digitalnD/DigitalSurfaceGeometry.h \
    ImaGene/digitalnD/DigitalSurface.ih \
    ImaGene/digitalnD/DigitalSurface.h \
    ImaGene/digitalnD/CubicalSpace.ih \
    ImaGene/digitalnD/CubicalSpace.h \
    ImaGene/digitalnD/CubicalComplex.ih \
    ImaGene/digitalnD/CubicalComplex.h \
    ImaGene/digitalnD/CubicalCell.ih \
    ImaGene/digitalnD/CubicalCell.h \
    ImaGene/digitalnD/CentroidEmbedder.ih \
    ImaGene/digitalnD/CentroidEmbedder.h \
    ImaGene/digitalnD/cas.h \
    ImaGene/digitalnD/cas_k26_l6.h \
    ImaGene/digitalnD/cas_k18_l6.h \
    ImaGene/digitalnD/cas_k6_l26.h \
    ImaGene/digitalnD/cas_k6_l18.h \
    ImaGene/digitalnD/C4CIteratorOnSurface.h \
    ImaGene/digitalnD/C4CIteratorOnFreemanChainSurface.ih \
    ImaGene/digitalnD/C4CIteratorOnFreemanChainSurface.h \
    ImaGene/digitalnD/C4CIteratorOnExplicitDigitalSurface.ih \
    ImaGene/digitalnD/C4CIteratorOnExplicitDigitalSurface.h \
    ImaGene/digitalnD/C4CIteratorOnBdry.ih \
    ImaGene/digitalnD/C4CIteratorOnBdry.h \
    ImaGene/digitalnD/BoxEmbedder.ih \
    ImaGene/digitalnD/BoxEmbedder.h \
    ImaGene/digitalnD/Bitset1.ih \
    ImaGene/digitalnD/Bitset1.h \
    ImaGene/digitalnD/BelAdjacency.ih \
    ImaGene/digitalnD/BelAdjacency.h \
    ImaGene/digitalnD/AreaOnSurfaceComputerByEuler.ih \
    ImaGene/digitalnD/AreaOnSurfaceComputerByEuler.h \
    ImaGene/helper/VisualCurvature.ih \
    ImaGene/helper/VisualCurvature.h \
    ImaGene/helper/ShapeHelper.ih \
    ImaGene/helper/ShapeHelper.h \
    ImaGene/helper/ScaleProfile.ih \
    ImaGene/helper/ScaleProfile.h \
    ImaGene/helper/MultiscaleProfile.ih \
    ImaGene/helper/MultiscaleProfile.h \
    ImaGene/helper/MeaningfulContinuation.ih \
    ImaGene/helper/MeaningfulContinuation.h \
    ImaGene/helper/LinearMinimizer.ih \
    ImaGene/helper/LinearMinimizer.h \
    ImaGene/helper/ImageMultiscaleAnalysis.ih \
    ImaGene/helper/ImageMultiscaleAnalysis.h \
    ImaGene/helper/GradientColorMap.ih \
    ImaGene/helper/GradientColorMap.h \
    ImaGene/helper/GlobalC4CGeometry.ih \
    ImaGene/helper/GlobalC4CGeometry.h \
    ImaGene/helper/DrawingXFIG.h \
    ImaGene/helper/DigitalElevationMapStats.ih \
    ImaGene/helper/DigitalElevationMapStats.h \
    ImaGene/helper/DigitalElevationMap.ih \
    ImaGene/helper/DigitalElevationMap.h \
    ImaGene/helper/CurveVariationsHelper.h \
    ImaGene/helper/ContourHelper.ih \
    ImaGene/helper/ContourHelper.h \
    ImaGene/helper/Color.h \
    ImaGene/helper/CharacteristicPolygon.ih \
    ImaGene/helper/CharacteristicPolygon.h \
    ImaGene/helper/ChangeQuadrant.ih \
    ImaGene/helper/ChangeQuadrant.h \
    ImaGene/helper/C4CTangentialCoverGeometry.ih \
    ImaGene/helper/C4CTangentialCoverGeometry.h \
    ImaGene/image/Pixel.ih \
    ImaGene/image/Pixel.h \
    ImaGene/image/PGMFilter.ih \
    ImaGene/image/PGMFilter.h \
    ImaGene/image/Image2DUtils.ih \
    ImaGene/image/Image2DUtils.h \
    ImaGene/image/Image2D.ih \
    ImaGene/image/Image2D.h \
    ImaGene/mathutils/SternBrocot.ih \
    ImaGene/mathutils/SternBrocot.h \
    ImaGene/mathutils/Statistics.ih \
    ImaGene/mathutils/Statistics.h \
    ImaGene/mathutils/Statistic.ih \
    ImaGene/mathutils/Statistic.h \
    ImaGene/mathutils/SimpleLinearRegression.ih \
    ImaGene/mathutils/SimpleLinearRegression.h \
    ImaGene/mathutils/SampledFunction.ih \
    ImaGene/mathutils/SampledFunction.h \
    ImaGene/mathutils/PWFAffineFunction.ih \
    ImaGene/mathutils/PWFAffineFunction.h \
    ImaGene/mathutils/Polynomial3.ih \
    ImaGene/mathutils/Polynomial3.h \
    ImaGene/mathutils/Polynomial.ih \
    ImaGene/mathutils/Polynomial.h \
    ImaGene/mathutils/MeasureOfStraightLines.h \
    ImaGene/mathutils/Mathutils.ih \
    ImaGene/mathutils/Mathutils.h \
    ImaGene/mathutils/Line2D.ih \
    ImaGene/mathutils/Line2D.h \
    ImaGene/mathutils/Lattice.ih \
    ImaGene/mathutils/Lattice.h \
    ImaGene/mathutils/G.ih \
    ImaGene/mathutils/G.h \
    ImaGene/mathutils/Functions.ih \
    ImaGene/mathutils/Functions.h \
    ImaGene/mathutils/DG.ih \
    ImaGene/mathutils/DG.h \
    ImaGene/mathutils/CFraction.ih \
    ImaGene/mathutils/CFraction.h \
    ImaGene/timetools/Clock.h \
    ImaGene/timetools/Alarm.h \
    meaningfulscales.h
FORMS += mainwindow.ui

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/res/drawable/icon.png \
    android/res/drawable/logo.png \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-mdpi/icon.png \
    android/res/layout/splash.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/origo/JavaManager.java \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/src/pl/ekk/mkk/cameratest/ImageManager.java \
    android/version.xml \
    ImaGene/base/TypedObject.ih \
    ImaGene/digitalnD/Embedder.ih \
    ImaGene/digitalnD/DigitalSurfaceTracker.ih










































