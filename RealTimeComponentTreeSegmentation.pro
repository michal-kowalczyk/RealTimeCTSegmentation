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

# QT -= opengl

QT_GRAPHICSSYSTEM = opengl

QMAKE_CXXFLAGS_RELEASE+= -O2
QMAKE_CXXFLAGS += -fexceptions

SOURCES += main.cpp mainwindow.cpp \
    camerasupport.cpp \
    JavaClassesLoader.cpp \
    segmentationbasedoncomponenttree.cpp \
    meaningfulscales.cpp \
    timer.cpp \
    plot.cpp \
    timeseries/src/tsutil.cpp \
    imagemarkerfile.cpp \
    histogram.cpp \
    grayscaleimagehistogram.cpp \
    subimage.cpp \
    markedpixelsindicators.cpp \
    componenttreeinputgenerator.cpp \
    meaningfulscalesalphaadjust.cpp
HEADERS += mainwindow.h \
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
    meaningfulscales.h \
    segmentationbasedoncomponenttree.h \
    camerasupport.h \
    timer.h \
    plot.h \
    timeseries/src/tsutil.h \
    imagemarkerfile.h \
    histogram.h \
    grayscaleimagehistogram.h \
    subimage.h \
    markedpixelsindicators.h \
    componenttreeinputgenerator.h \
    meaningfulscalesalphaadjust.h
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
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/version.xml \
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
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/version.xml

# Including ImaGene library
LIBS += ImaGenePrecompilated/Alarm.o \
    ImaGenePrecompilated/AreaOnSurfaceComputerByEuler.o \
    ImaGenePrecompilated/Arguments.o \
    ImaGenePrecompilated/BelAdjacency.o \
    ImaGenePrecompilated/Bitset1.o \
    ImaGenePrecompilated/BlurredSegmentTgtCover.o \
    ImaGenePrecompilated/BoxEmbedder.o \
    ImaGenePrecompilated/C4CELength.o \
    ImaGenePrecompilated/C4CGeometry.o \
    ImaGenePrecompilated/C4CIterator.o \
    ImaGenePrecompilated/C4CIteratorOnBdry.o \
    ImaGenePrecompilated/C4CIteratorOnExplicitDigitalSurface.o \
    ImaGenePrecompilated/C4CIteratorOnFreemanChain.o \
    ImaGenePrecompilated/C4CIteratorOnFreemanChainSurface.o \
    ImaGenePrecompilated/C4CSegment.o \
    ImaGenePrecompilated/C4CSegmentPencil.o \
    ImaGenePrecompilated/C4CTangentialCover.o \
    ImaGenePrecompilated/C4CTangentialCoverGeometry.o \
    ImaGenePrecompilated/CFraction.o \
    ImaGenePrecompilated/CentroidEmbedder.o \
    ImaGenePrecompilated/ChangeQuadrant.o \
    ImaGenePrecompilated/CharacteristicPolygon.o \
    ImaGenePrecompilated/Clock.o \
    ImaGenePrecompilated/Color.o \
    ImaGenePrecompilated/ContourHelper.o \
    ImaGenePrecompilated/CubicalCell.o \
    ImaGenePrecompilated/CubicalComplex.o \
    ImaGenePrecompilated/CubicalSpace.o \
    ImaGenePrecompilated/CurveCode.o \
    ImaGenePrecompilated/CurveVariationsHelper.o \
    ImaGenePrecompilated/DG.o \
    ImaGenePrecompilated/DLine.o \
    ImaGenePrecompilated/DMLPContour.o \
    ImaGenePrecompilated/DigitalElevationMap.o \
    ImaGenePrecompilated/DigitalElevationMapStats.o \
    ImaGenePrecompilated/DigitalSurface.o \
    ImaGenePrecompilated/DigitalSurfaceGeometry.o \
    ImaGenePrecompilated/DigitalSurfaceUmbrella.o \
    ImaGenePrecompilated/DigitalSurfaceUmbrellaComputer.o \
    ImaGenePrecompilated/DrawingXFIG.o \
    ImaGenePrecompilated/Embedder.o \
    ImaGenePrecompilated/EuclideanGeometry.o \
    ImaGenePrecompilated/ExplicitDigitalSurface.o \
    ImaGenePrecompilated/ExplicitDigitalSurfaceTracker.o \
    ImaGenePrecompilated/Frame2D.o \
    ImaGenePrecompilated/FreemanChain.o \
    ImaGenePrecompilated/FreemanChainTransform.o \
    ImaGenePrecompilated/FreemanFrame2D.o \
    ImaGenePrecompilated/Functions.o \
    ImaGenePrecompilated/G.o \
    ImaGenePrecompilated/GeometryComputerByContour4.o \
    ImaGenePrecompilated/GlobalC4CGeometry.o \
    ImaGenePrecompilated/GridContour.o \
    ImaGenePrecompilated/GridCurve.o \
    ImaGenePrecompilated/GridEmbedder.o \
    ImaGenePrecompilated/HashTable.o \
    ImaGenePrecompilated/Image2D.o \
    ImaGenePrecompilated/Image2DUtils.o \
    ImaGenePrecompilated/ImageMultiscaleAnalysis.o \
    ImaGenePrecompilated/K2Space.o \
    ImaGenePrecompilated/K3Isosurface.o \
    ImaGenePrecompilated/K3Isosurface_k6_l18.o \
    ImaGenePrecompilated/K3Shapes.o \
    ImaGenePrecompilated/KnCharSet.o \
    ImaGenePrecompilated/KnRCellSet.o \
    ImaGenePrecompilated/KnRCellVector.o \
    ImaGenePrecompilated/KnShapes.o \
    ImaGenePrecompilated/KnSpace.o \
    ImaGenePrecompilated/KnSpaceCoordScanner.o \
    ImaGenePrecompilated/KnSpaceScanner.o \
    ImaGenePrecompilated/KnTools.o \
    ImaGenePrecompilated/KnTypes.o \
    ImaGenePrecompilated/KnUtils.o \
    ImaGenePrecompilated/Lattice.o \
    ImaGenePrecompilated/Line2D.o \
    ImaGenePrecompilated/LinearMinimizer.o \
    ImaGenePrecompilated/Mathutils.o \
    ImaGenePrecompilated/Matrix.o \
    ImaGenePrecompilated/MeanCurvatureOnSurfaceComputerByDG.o \
    ImaGenePrecompilated/MeaningfulContinuation.o \
    ImaGenePrecompilated/MeasureOfStraightLines.o \
    ImaGenePrecompilated/MultiscaleFreemanChain.o \
    ImaGenePrecompilated/MultiscaleProfile.o \
    ImaGenePrecompilated/NormalOnSurfaceComputerByUmbrellas.o \
    ImaGenePrecompilated/ObjectBoundary.o \
    ImaGenePrecompilated/ObjectBoundaryTracker.o \
    ImaGenePrecompilated/OctantChange.o \
    ImaGenePrecompilated/OrderedAlphabet.o \
    ImaGenePrecompilated/PGMFilter.o \
    ImaGenePrecompilated/PWFAffineFunction.o \
    ImaGenePrecompilated/Pixel.o \
    ImaGenePrecompilated/Polynomial.o \
    ImaGenePrecompilated/Polynomial3.o \
    ImaGenePrecompilated/Proxy.o \
    ImaGenePrecompilated/RnMaps.o \
    ImaGenePrecompilated/SampledFunction.o \
    ImaGenePrecompilated/Sanitizer.o \
    ImaGenePrecompilated/ScaleProfile.o \
    ImaGenePrecompilated/ShapeHelper.o \
    ImaGenePrecompilated/Shapes.o \
    ImaGenePrecompilated/Signal.o \
    ImaGenePrecompilated/SimpleLinearRegression.o \
    ImaGenePrecompilated/SmartSet.o \
    ImaGenePrecompilated/SmoothingEmbedder.o \
    ImaGenePrecompilated/StandardArguments.o \
    ImaGenePrecompilated/StandardHashTable.o \
    ImaGenePrecompilated/StaticHashTable.o \
    ImaGenePrecompilated/Statistic.o \
    ImaGenePrecompilated/Statistics.o \
    ImaGenePrecompilated/SternBrocot.o \
    ImaGenePrecompilated/SurfelNeighborhood.o \
    ImaGenePrecompilated/UndoableList.o \
    ImaGenePrecompilated/Vector.o \
    ImaGenePrecompilated/Vector2i.o \
    ImaGenePrecompilated/VectorUtils.o \
    ImaGenePrecompilated/VisualCurvature.o \
    ImaGenePrecompilated/WordUtils.o

#Include Qwt library
LIBS += QwtPrecompilated/moc_qwt_abstract_slider.o \
    QwtPrecompilated/moc_qwt_analog_clock.o \
    QwtPrecompilated/moc_qwt_compass.o \
    QwtPrecompilated/moc_qwt_counter.o \
    QwtPrecompilated/moc_qwt_dial.o \
    QwtPrecompilated/moc_qwt_dyngrid_layout.o \
    QwtPrecompilated/moc_qwt_knob.o \
    QwtPrecompilated/moc_qwt_legend.o \
    QwtPrecompilated/moc_qwt_legend_item.o \
    QwtPrecompilated/moc_qwt_magnifier.o \
    QwtPrecompilated/moc_qwt_panner.o \
    QwtPrecompilated/moc_qwt_picker.o \
    QwtPrecompilated/moc_qwt_plot.o \
    QwtPrecompilated/moc_qwt_plot_canvas.o \
    QwtPrecompilated/moc_qwt_plot_magnifier.o \
    QwtPrecompilated/moc_qwt_plot_panner.o \
    QwtPrecompilated/moc_qwt_plot_picker.o \
    QwtPrecompilated/moc_qwt_plot_renderer.o \
    QwtPrecompilated/moc_qwt_plot_zoomer.o \
    QwtPrecompilated/moc_qwt_sampling_thread.o \
    QwtPrecompilated/moc_qwt_scale_widget.o \
    QwtPrecompilated/moc_qwt_slider.o \
    QwtPrecompilated/moc_qwt_text_label.o \
    QwtPrecompilated/moc_qwt_thermo.o \
    QwtPrecompilated/moc_qwt_wheel.o \
    QwtPrecompilated/qwt_abstract_scale.o \
    QwtPrecompilated/qwt_abstract_scale_draw.o \
    QwtPrecompilated/qwt_abstract_slider.o \
    QwtPrecompilated/qwt_analog_clock.o \
    QwtPrecompilated/qwt_arrow_button.o \
    QwtPrecompilated/qwt_clipper.o \
    QwtPrecompilated/qwt_color_map.o \
    QwtPrecompilated/qwt_column_symbol.o \
    QwtPrecompilated/qwt_compass.o \
    QwtPrecompilated/qwt_compass_rose.o \
    QwtPrecompilated/qwt_counter.o \
    QwtPrecompilated/qwt_curve_fitter.o \
    QwtPrecompilated/qwt_dial.o \
    QwtPrecompilated/qwt_dial_needle.o \
    QwtPrecompilated/qwt_double_range.o \
    QwtPrecompilated/qwt_dyngrid_layout.o \
    QwtPrecompilated/qwt_event_pattern.o \
    QwtPrecompilated/qwt_interval.o \
    QwtPrecompilated/qwt_interval_symbol.o \
    QwtPrecompilated/qwt_knob.o \
    QwtPrecompilated/qwt_legend.o \
    QwtPrecompilated/qwt_legend_item.o \
    QwtPrecompilated/qwt_magnifier.o \
    QwtPrecompilated/qwt_math.o \
    QwtPrecompilated/qwt_matrix_raster_data.o \
    QwtPrecompilated/qwt_null_paintdevice.o \
    QwtPrecompilated/qwt_painter.o \
    QwtPrecompilated/qwt_panner.o \
    QwtPrecompilated/qwt_picker.o \
    QwtPrecompilated/qwt_picker_machine.o \
    QwtPrecompilated/qwt_plot.o \
    QwtPrecompilated/qwt_plot_axis.o \
    QwtPrecompilated/qwt_plot_canvas.o \
    QwtPrecompilated/qwt_plot_curve.o \
    QwtPrecompilated/qwt_plot_dict.o \
    QwtPrecompilated/qwt_plot_directpainter.o \
    QwtPrecompilated/qwt_plot_grid.o \
    QwtPrecompilated/qwt_plot_histogram.o \
    QwtPrecompilated/qwt_plot_intervalcurve.o \
    QwtPrecompilated/qwt_plot_item.o \
    QwtPrecompilated/qwt_plot_layout.o \
    QwtPrecompilated/qwt_plot_magnifier.o \
    QwtPrecompilated/qwt_plot_marker.o \
    QwtPrecompilated/qwt_plot_panner.o \
    QwtPrecompilated/qwt_plot_picker.o \
    QwtPrecompilated/qwt_plot_rasteritem.o \
    QwtPrecompilated/qwt_plot_renderer.o \
    QwtPrecompilated/qwt_plot_rescaler.o \
    QwtPrecompilated/qwt_plot_scaleitem.o \
    QwtPrecompilated/qwt_plot_seriesitem.o \
    QwtPrecompilated/qwt_plot_spectrocurve.o \
    QwtPrecompilated/qwt_plot_spectrogram.o \
    QwtPrecompilated/qwt_plot_xml.o \
    QwtPrecompilated/qwt_plot_zoomer.o \
    QwtPrecompilated/qwt_point_3d.o \
    QwtPrecompilated/qwt_point_polar.o \
    QwtPrecompilated/qwt_raster_data.o \
    QwtPrecompilated/qwt_round_scale_draw.o \
    QwtPrecompilated/qwt_sampling_thread.o \
    QwtPrecompilated/qwt_scale_div.o \
    QwtPrecompilated/qwt_scale_draw.o \
    QwtPrecompilated/qwt_scale_engine.o \
    QwtPrecompilated/qwt_scale_map.o \
    QwtPrecompilated/qwt_scale_widget.o \
    QwtPrecompilated/qwt_series_data.o \
    QwtPrecompilated/qwt_slider.o \
    QwtPrecompilated/qwt_spline.o \
    QwtPrecompilated/qwt_symbol.o \
    QwtPrecompilated/qwt_system_clock.o \
    QwtPrecompilated/qwt_text.o \
    QwtPrecompilated/qwt_text_engine.o \
    QwtPrecompilated/qwt_text_label.o \
    QwtPrecompilated/qwt_thermo.o \
    QwtPrecompilated/qwt_wheel.o




















