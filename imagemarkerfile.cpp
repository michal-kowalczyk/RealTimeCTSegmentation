/**
  This file is an effect of work of Michal Kowalczyk, student of University of Lodz (Poland), doing his master 2 as a erasmus student at Universite de Lorraine (France) - former Universite Henri Poincare during his intership in Loria (France).
  */

#include "imagemarkerfile.h"

#include <QFile>
#include <QImage>
#include <QDebug>
#include <QColor>

unsigned int ImageMarkerFile::markerColour = QColor (255, 0, 0, 255).rgba();
unsigned int ImageMarkerFile::groundTruthColour = QColor (0, 255, 0, 255).rgba();


void ImageMarkerFile::Read (QFile &file, QImage &image, QImage &marker, QImage &groundThruth){
    if (!file.exists()){
        qDebug () << "No file!";
        return;
    }

    file.open (QIODevice::ReadOnly);
    QDataStream data (&file);

    unsigned int readCount = 0;
    unsigned short startImageData, startImageDescription, startMarkerDescription, startAdditionalDescription;
    unsigned short width;
    unsigned short height;
    data >> startImageData;
    if (startImageData > 0){
        data >> width;
        readCount += sizeof (width);
    }
    if (startImageData > 2){
        data >> height;
        readCount += sizeof (height);
    }
    if (image.width () != width || image.height () != height)
        image = QImage (width, height, QImage::Format_ARGB32_Premultiplied);
    if (marker.width () != width || marker.height () != height)
        marker = QImage (width, height, QImage::Format_ARGB32_Premultiplied);
    if (startImageData > 4){
        data >> startImageDescription;
        readCount += sizeof (startImageDescription);
    }
    if (startImageData > 6){
        data >> startMarkerDescription;
        readCount += sizeof (startMarkerDescription);
    }
    if (startImageData > 8){
        data >> startAdditionalDescription;
        readCount += sizeof (startAdditionalDescription);
        qDebug () << startAdditionalDescription;
    }
    bool readGroundTruth = false;
    qDebug () << readCount;
    if (readCount == startAdditionalDescription){
        unsigned char bitSettings;
        data >> bitSettings;
        if (bitSettings - ((bitSettings >> 1) << 1))
            readGroundTruth = true;
        qDebug () << readGroundTruth;
    }
    unsigned int size = width * height;
    unsigned char r, g, b;
    unsigned int *imagePointer = (unsigned int *)image.bits ();
    for (unsigned int i = 0; i < size; i++){
        data >> r;
        data >> g;
        data >> b;
        *imagePointer = qRgb (r, g, b);
        imagePointer++;
    }

    unsigned int *markerPointer = (unsigned int *)marker.bits ();
    unsigned int *endMarkerPointer = markerPointer + size;
    unsigned char code, nextCode;
    for (unsigned int i = 0; i < size; i += 8){
        data >> code;
        for (unsigned j = 0; j < 8 && markerPointer < endMarkerPointer; j++){
            nextCode = code >> 1;
            if (code - (nextCode << 1))
                *markerPointer = markerColour;
            else
                *markerPointer = 0;
            code = nextCode;
            markerPointer++;
        }
    }

    if (readGroundTruth){
        if (groundThruth.width () != width || groundThruth.height () != height)
            groundThruth = QImage (width, height, QImage::Format_ARGB32_Premultiplied);
        unsigned int *groundTruthPointer = (unsigned int *)groundThruth.bits ();
        unsigned int *endGroundTruthPointer = groundTruthPointer + size;
        unsigned char code, nextCode;
        for (unsigned int i = 0; i < size; i += 8){
            data >> code;
            for (unsigned j = 0; j < 8 && groundTruthPointer < endGroundTruthPointer; j++){
                nextCode = code >> 1;
                if (code - (nextCode << 1))
                    *groundTruthPointer = groundTruthColour;
                else
                    *groundTruthPointer = 0;
                code = nextCode;
                groundTruthPointer++;
            }
        }
    }

    file.close ();
}


void ImageMarkerFile::Write (QFile &file, QImage &image, QImage &marker, QImage &groundThruth){
    file.open (QIODevice::WriteOnly);
    QDataStream data (&file);

    data << (unsigned short)11;//start of image data
    data << (unsigned short)image.width ();
    data << (unsigned short)image.height ();

    data << (unsigned short)0;//startImageDescription
    data << (unsigned short)0;//startMarkerDescription
    data << (unsigned short)10;//startAdditionalDescription
    unsigned char additionalDescription = 0;

    bool writeGroundTruth = (groundThruth.width () == image.width () && groundThruth.height () == image.height ());
    if (writeGroundTruth)
        additionalDescription += 1;//writeDesiredResult

    data << (unsigned char)additionalDescription;

    unsigned int size = image.width() * image.height ();
    const unsigned int *framePointer = (const unsigned int *)image.constBits ();
    for (unsigned int i = 0; i < size; i++){
        data << (unsigned char)qRed (*framePointer);
        data << (unsigned char)qGreen (*framePointer);
        data << (unsigned char)qBlue (*framePointer);
        framePointer++;
    }

    const unsigned int *markerPointer = (const unsigned int *)marker.constBits ();
    unsigned char code = 0, codingShift = 0;
    for (unsigned int i = 0; i < size; i++){
        code >>= 1;
        if (*markerPointer > 0)
            code += 128;
        markerPointer++;

        codingShift++;
        if (codingShift == 8){
            data << code;
            code = 0;
            codingShift = 0;
        }
    }
    if (codingShift > 0){
        code <<= 8 - codingShift;
        data << code;
    }

    if (writeGroundTruth){
        const unsigned int *groundTruthPointer = (const unsigned int *)groundThruth.constBits ();
        code = 0;
        codingShift = 0;
        for (unsigned int i = 0; i < size; i++){
            code >>= 1;
            if (*groundTruthPointer > 0)
                code += 128;
            groundTruthPointer++;

            codingShift++;
            if (codingShift == 8){
                data << code;
                code = 0;
                codingShift = 0;
            }
        }
        if (codingShift > 0){
            code <<= 8 - codingShift;
            data << code;
        }
    }

    file.close ();
}
