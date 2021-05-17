#include "filter.h"

#include <QtCore/QCoreApplication>
#include <QImage>
#include <string>
#include <QColor>
#include<iostream>
template <class T>
T Clamp(T value, T max, T min)
{
	if (value > max)
		return max;
	if (value < min)
		return min;
	return value;
}
QImage filter::process(const QImage& img)const
{
	QImage result(img);
	for (int x = 0; x < img.width(); x++)
		for (int y = 0; y < img.height(); y++)
		{
			QColor color = calcNewPixelColor(img, x, y);
			result.setPixelColor(x, y, color);
		}
	return result;
}
QColor InvertFilter::calcNewPixelColor(const QImage& img, int x, int y)const
{
	QColor color = img.pixelColor(x, y);
	color.setRgb(255 - color.red(), 255 - color.green(), 255 - color.blue());
	return color;
}

QColor MatrixFilter::calcNewPixelColor(const QImage& img, int x, int y)const
{
	float returnR = 0;
	float returnG = 0;
	float returnB = 0;
	int size = mKernel.getSize();
	int radius = mKernel.getRadius();
	for (int i = -radius; i <= radius; i++)

		for (int j = -radius; j <= radius; j++)
		{
			int idx = (i + radius) * size + j + radius;
			QColor color = img.pixelColor(Clamp(x + j, img.width() - 1, 0), Clamp(y + 1, img.height() - 1, 0));
			returnR += color.red() * mKernel[idx];
			returnG += color.green() * mKernel[idx];
			returnB += color.blue() * mKernel[idx];
		}
	return QColor(Clamp(returnR, 255.f, 0.f), Clamp(returnG, 255.f, 0.f), Clamp(returnB, 255.f, 0.f));
}
QColor GrayScaleFilter::calcNewPixelColor(const QImage& img, int x, int y)const
{
	QColor color = img.pixelColor(x, y);
	float intensity = 0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue();
	color.setRgb(intensity, intensity, intensity);
	return color;
}
QColor Sepia::calcNewPixelColor(const QImage& img, int x, int y)const
{
	QColor color = img.pixelColor(x, y);
	float intensity = 0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue();

	float returnR = 0;
	float returnG = 0;
	float returnB = 0;
	returnR += intensity + 2 * 10;
	returnG += intensity + 0.5 * 10;
	returnB += intensity - 10;
	return QColor(Clamp(returnR, 255.f, 0.f), Clamp(returnG, 255.f, 0.f), Clamp(returnB, 255.f, 0.f));
}
QColor Brightness::calcNewPixelColor(const QImage& img, int x, int y)const
{
	QColor color = img.pixelColor(x, y);
	float returnR = 0;
	float returnG = 0;
	float returnB = 0;
	int k = 60;
	returnR += color.red() + k;
	returnG += color.green() + k;
	returnB += color.blue() + k;
	return QColor(Clamp(returnR, 255.f, 0.f), Clamp(returnG, 255.f, 0.f), Clamp(returnB, 255.f, 0.f));
}
QColor SharpnessFilter::calcNewPixelColor(const QImage& img, int x, int y)const
{
	{
		float returnR = 0;
		float returnG = 0;
		float returnB = 0;
		int size = mKernel.getSize();
		int radius = mKernel.getRadius();
		for (int i = -radius; i <= radius; i++)
			for (int j = -radius; j <= radius; j++)
			{
				int idx = (i + radius) * size + j + radius;

				QColor color = img.pixelColor(Clamp(x + j, img.width() - 1, 0), Clamp(y + i, img.height() - 1, 0));
				if (mKernel[idx])
				{
					returnR += color.red() * mKernel[idx];
					returnG += color.green() * mKernel[idx];
					returnB += color.blue() * mKernel[idx];
				}
			}
		return QColor(Clamp(abs(returnR), 255.f, 0.f), Clamp(abs(returnG), 255.f, 0.f), Clamp(abs(returnB), 255.f, 0.f));
	}
}
QColor SobelFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
	float returnR = 0;
	float returnG = 0;
	float returnB = 0;
	int size = mKernel.getSize();
	int radius = mKernel.getRadius();
	for (int i = -radius; i <= radius; i++)
		for (int j = -radius; j <= radius; j++)
		{
			int idx = (i + radius) * size + j + radius;

			QColor color = img.pixelColor(Clamp(x + j, img.width() - 1, 0), Clamp(y + i, img.height() - 1, 0));
			if (mKernel[idx])
			{
				returnR += color.red() * mKernel[idx];
				returnG += color.green() * mKernel[idx];
				returnB += color.blue() * mKernel[idx];
			}
		}
	return QColor(Clamp(abs(returnR), 255.f, 0.f), Clamp(abs(returnG), 255.f, 0.f), Clamp(abs(returnB), 255.f, 0.f));
}



QImage PerfectReflectorFilter::process(const QImage& img) const
{
	float maxR = 0;
	float maxG = 0;
	float maxB = 0;
	QColor color;
	for (int x = 0; x < img.width(); x++)
		for (int y = 0; y < img.height(); y++)
		{
			color = img.pixelColor(x, y);
			if (color.red() > maxR)
				maxR = color.red();
			if (color.green() > maxG)
				maxG = color.green();
			if (color.blue() > maxB)
				maxB = color.blue();
		}
	QImage result(img);

	for (int x = 0; x < img.width(); x++)
		for (int y = 0; y < img.height(); y++)
		{
			QColor color = calcNewPixelColorA(img, x, y, maxR, maxG, maxB);
			result.setPixelColor(x, y, color);
		}
	return result;
}
QColor PerfectReflectorFilter::calcNewPixelColorA(const QImage& img, int x, int y, float maxR, float maxG, float maxB) const
{
	QColor color = img.pixelColor(x, y);
	color.setRgb(color.red() * 255 / maxR, color.green() * 255 / maxG, color.blue() * 255 / maxB);
	return color;
}
QColor HorizontalWaves::calcNewPixelColor(const QImage& img, int x, int y)const
{
	int X = x + 20 * sin((2 * 3.14159265 * y) / 60);
	int Y = y;
	QColor color = img.pixelColor(x, y);
	color.setRgb(img.pixelColor(clamp(X, 0, img.width() - 1), Y).red(), img.pixelColor(clamp(X, 0, img.width() - 1), Y).green(), img.pixelColor(clamp(X, 0, img.width() - 1), Y).blue());
	return color;
}


QColor VerticalWaves::calcNewPixelColor(const QImage& img, int x, int y)const
{
	int X = x + 20 * sin((2 * 3.14159265 * x) / 30);
	int Y = y;
	QColor color = img.pixelColor(x, y);
	color.setRgb(img.pixelColor(clamp(X, 0, img.width() - 1), Y).red(), img.pixelColor(clamp(X, 0, img.width() - 1), Y).green(), img.pixelColor(clamp(X, 0, img.width() - 1), Y).blue());
	return color;
}

QColor GlassEffect::calcNewPixelColor(const QImage& img, int x, int y)const
{
	int X = x + ((double)rand() / RAND_MAX - 0.5) * 10;
	int Y = y + ((double)rand() / RAND_MAX - 0.5) * 10;
	QColor color = img.pixelColor(x, y);
	color.setRgb(img.pixelColor(clamp(X, 0, img.width() - 1), clamp(Y, 0, img.height() - 1)).red(), img.pixelColor(clamp(X, 0, img.width() - 1), clamp(Y, 0, img.height() - 1)).green(), img.pixelColor(clamp(X, 0, img.width() - 1), clamp(Y, 0, img.height() - 1)).blue());
	return color;
}

//QColor LinearStretching::calcNewPixelColor(const QImage& img, int x, int y)const
//{
//	int MaxR = 0, MinR = 255;
//	int MaxG = 0, MinG = 255;
//	int MaxB = 0, MinB = 255;
//	
//	
//	for (int i = 0; i < img.width(); i++)
//	{			
//		for (int j = 0; j < img.height(); j++)
//		{
//			QColor color = img.pixelColor(i, j);
//			if (color.red() > MaxR)
//				MaxR = color.red();
//			if (color.red() < MinR)
//				MinR = color.red();
//			if (color.green() > MaxG)
//				MaxG = color.green();
//			if (color.green() < MinG)
//				MinG = color.green();
//			if (color.blue() > MaxB)
//				MaxB = color.blue();
//			if (color.blue() < MinB)
//				MinB = color.blue();
//		}		
//	}
//	QColor color = img.pixelColor(x, y);
//	float returnR = 0;
//	returnR = (img.pixelColor(x, y).red() - MinR) * 255 / (MaxR - MinR);
//	float returnG = 0;
//	returnG = (img.pixelColor(x, y).green() - MinG) * 255 / (MaxG - MinG);
//	float returnB = 0;
//	returnB = (img.pixelColor(x, y).blue() - MinB) * 255 / (MaxB - MinB);
//	color.setRgb(clamp(returnR, 0.f, 255.f), clamp(returnG, 0.f, 255.f), clamp(returnB, 0.f, 255.f));
//	return color;
//}
QColor AutolevelsFilter::calcNewPixelColorA(const QImage& img, int x, int y, float maxR, float maxG, float maxB, float minR, float minG, float minB) const
{
	QColor color = img.pixelColor(x, y);
	color.setRgb(Clamp((color.red() - minR) * 255  / (maxR - minR), 255.f, 0.f), Clamp((color.green() - minG) * 255 / (maxG - minG), 255.f, 0.f), Clamp((color.blue() - minB) * 255  / (maxB - minB), 255.f, 0.f));
	return color;
}

QImage AutolevelsFilter::process(const QImage& img) const
{
	float maxR = 0;
	float maxG = 0;
	float maxB = 0;
	float minR = 255;
	float minG = 255;
	float minB = 255;
	QColor color;
	for (int x = 0; x < img.width(); x++)
		for (int y = 0; y < img.height(); y++)
		{
			color = img.pixelColor(x, y);
			if (color.red() > maxR)
				maxR = color.red();
			if (color.green() > maxG)
				maxG = color.green();
			if (color.blue() > maxB)
				maxB = color.blue();
			if (color.red() < minR)
				minR = color.red();
			if (color.green() < minG)
				minG = color.green();
			if (color.blue() < minB)
				minB = color.blue();
		}
	QImage result(img);

	for (int x = 0; x < img.width(); x++)
		for (int y = 0; y < img.height(); y++)
		{
			QColor color = calcNewPixelColorA(img, x, y, maxR, maxG, maxB, minR, minG, minB);
			result.setPixelColor(x, y, color);
		}
	return result;
}
QColor MedianFilter::calcNewPixelColor(const QImage& img, int x, int y)const
{
	const int KernelSize = SizeM * SizeM;
	int red[KernelSize], green[KernelSize], blue[KernelSize];
	int X = clamp(x - SizeM / 2, 0, img.width() - 1);
	int Y = clamp(y - SizeM / 2, 0, img.height() - 1);
	int a = 0;
	for (int i = 0; i < SizeM; i++)
	{
		for (int j = 0; j < SizeM; j++)
		{
			red[a] = img.pixelColor(clamp(x - SizeM / 2 + j, 0, img.width() - 1), clamp(y - SizeM / 2 + i, 0, img.height() - 1)).red();
			green[a] = img.pixelColor(clamp(x - SizeM / 2 + j, 0, img.width() - 1), clamp(y - SizeM / 2 + i, 0, img.height() - 1)).green();
			blue[a] = img.pixelColor(clamp(x - SizeM / 2 + j, 0, img.width() - 1), clamp(y - SizeM / 2 + i, 0, img.height() - 1)).blue();
			a++;
		}
	}
	sort(red, red + KernelSize);
	sort(green, green + KernelSize);
	sort(blue, blue + KernelSize);
	QColor color = img.pixelColor(x, y);
	color.setRgb(red[KernelSize / 2], green[KernelSize / 2], blue[KernelSize / 2]);
	return color;
}
QColor ErosionFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
	float returnR = 255;
	float returnG = 255;
	float returnB = 255;
	int size = mKernel.getSize();
	int radius = mKernel.getRadius();
	for (int i = -radius; i <= radius; i++)
		for (int j = -radius; j <= radius; j++)
		{
			int idx = (i + radius) * size + j + radius;

			QColor color = img.pixelColor(Clamp(x + j, img.width() - 1, 0), Clamp(y + i, img.height() - 1, 0));
			if (mKernel[idx])
			{
				if (color.red() < returnR)
					returnR = color.red();
				if (color.green() < returnG)
					returnG = color.green();
				if (color.blue() < returnB)
					returnB = color.blue();
			}
		}
	return QColor(Clamp(returnR, 255.f, 0.f), Clamp(returnG, 255.f, 0.f), Clamp(returnB, 255.f, 0.f));
}
QColor DilationFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
	float returnR = 0;
	float returnG = 0;
	float returnB = 0;
	int size = mKernel.getSize();
	int radius = mKernel.getRadius();
	for (int i = -radius; i <= radius; i++)
		for (int j = -radius; j <= radius; j++)
		{
			int idx = (i + radius) * size + j + radius;

			QColor color = img.pixelColor(Clamp(x + j, img.width() - 1, 0), Clamp(y + i, img.height() - 1, 0));
			if (mKernel[idx])
			{
				if (color.red() > returnR)
					returnR = color.red();
				if (color.green() > returnG)
					returnG = color.green();
				if (color.blue() > returnB)
					returnB = color.blue();
			}
		}
	return QColor(Clamp(returnR, 255.f, 0.f), Clamp(returnG, 255.f, 0.f), Clamp(returnB, 255.f, 0.f));
}
QImage TopHatFilter::process(const QImage& img) const
{
	{
		DilationFilter Dilation;
		QImage img1 = Dilation.process(img);
		ErosionFilter Eros;
		img1 = Eros.process(img1);

		QImage result(img);
		for (int x = 0; x < img.width(); x++)
			for (int y = 0; y < img.height(); y++)
			{
				QColor color = img.pixelColor(x, y);;
				QColor color1 = img1.pixelColor(x, y);
				float returnR = color.red() - color1.red();
				float returnG = color.green() - color1.green();
				float returnB = color.blue() - color1.blue();
				QColor colorResult;
				colorResult.setRgb(Clamp(returnR, 255.f, 0.f), Clamp(returnG, 255.f, 0.f), Clamp(returnB, 255.f, 0.f));
				result.setPixelColor(x, y, colorResult);
			}
		return result;
	}
}
QImage Opening::process(const QImage& img) const
{
	QImage returnQ;
	int radius = mKernel.getRadius();
	DilationFilter dilation(radius);
	returnQ = dilation.process(img);
	ErosionFilter erosion(radius);
	returnQ = erosion.process(returnQ);
	return returnQ;
}

QColor Opening::calcNewPixelColor(const QImage& img, int x, int y) const
{
	QColor QC;
	return QC;
}

QImage Closing::process(const QImage& img) const
{
	QImage returnQ;
	int radius = mKernel.getRadius();
	ErosionFilter erosion(radius);
	returnQ = erosion.process(img);
	DilationFilter dilation(radius);
	returnQ = dilation.process(returnQ);
	return returnQ;
}


QColor Closing::calcNewPixelColor(const QImage& img, int x, int y) const
{
	QColor QC;
	return QC;
}