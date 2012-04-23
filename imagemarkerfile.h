/**
  This file is an effect of work of Michal Kowalczyk, student of University of Lodz (Poland), doing his master 2 as a erasmus student at Universite de Lorraine (France) - former Universite Henri Poincare during his intership in Loria (France).
  */

#ifndef IMAGEMARKERFILE_H
#define IMAGEMARKERFILE_H

class QFile;
class QImage;

class ImageMarkerFile{
public:
    static void Read (QFile &file, QImage &image, QImage &marker, QImage &groundThruth);
    static void Write (QFile &file, QImage &image, QImage &marker, QImage &groundThruth);
private:
    static unsigned int markerColour;
    static unsigned int groundTruthColour;
};

#endif // IMAGEMARKERFILE_H
