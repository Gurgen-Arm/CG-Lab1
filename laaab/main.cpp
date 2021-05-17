#include <QtCore/QCoreApplication>
#include <QImage>
#include <string>
#include "filter.h"
#include <QColor>
#include<iostream>
using namespace std;
void main(int argc, char* argv[])
{
    string s;
    QImage img;
    for (int i = 0; i < argc; i++)
        if (!strcmp(argv[i], "-p") && (i + 1 < argc))
        {
            s = argv[i + 1];
        }
    img.load(QString(s.c_str()));
    img.save("D:/Img/Source3.png");
    InvertFilter invert;
    invert.process(img).save("D:/Img/Invert3.png");
    BlurFilter blur;
    blur.process(img).save("D:/Img/Blur3.png");
    GaussianFilter gauss;
    gauss.process(img).save("D:/Img/Gauss3.png");
    GrayScaleFilter gray;
    gray.process(img).save("D:/Img/GrayScale3.png");
    Sepia sepia;
    sepia.process(img).save("D:/Img/Sepia3.png");
    Brightness brightness;
    brightness.process(img).save("D:/Img/Brightness3.png");
    SharpnessFilter sharpness;
    sharpness.process(img).save("D:/Img/Sharpness3.png");
    SobelFilter sobel;
    sobel .process(img).save("D:/Img/Sobel3.png");
    PerfectReflectorFilter PRF;
    PRF.process(img).save("D:/Img/PRF3.png");
    HorizontalWaves hor;
    hor.process(img).save("D:/Img/hor3.png");
    VerticalWaves ver;
    ver.process(img).save("D:/Img/ver3.png");
    GlassEffect gla;
    gla.process(img).save("D:/Img/gla3.png");
  
    AutolevelsFilter au;
    au.process(img).save("D:/Img/au3.png");
    MedianFilter med;
    med.process(img).save("D:/Img/med3.png");
    ErosionFilter ero;
    ero.process(img).save("D:/Img/ero3.png");
    DilationFilter dil;
    dil.process(img).save("D:/Img/dil3.png");
    EmbossingFilter emb;
    emb.process(img).save("D:/Img/emb3.png");
    TopHatFilter top;
    top.process(img).save("D:/Img/top3.png");
    MotionBlurFilter moto;
    moto.process(img).save("D:/Img/moto3.png");
    Opening op;
    op.process(img).save("D:/Img/op3.png");
    Closing cl;
    cl.process(img).save("D:/Img/cl3.png");
}