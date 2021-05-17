#pragma once
#include <QtCore/QCoreApplication>
#include <QImage>
#include <string>
#include <QColor>
#include<cmath>
#include<iostream>
using namespace std;
const int SizeG = 7;
const int SizeB = 5;
const int SizeM = 5;
class filter
{
protected:
	virtual QColor calcNewPixelColor(const QImage& img, int x, int y)const = 0;
public:
	virtual ~filter() = default;
	virtual QImage process(const QImage& img)const;
};
class InvertFilter :public filter
{
	QColor calcNewPixelColor(const QImage& img, int x, int y)const override;
};
class Kernel
{
protected:
	unique_ptr<float[]>data;
	size_t radius;
	size_t getLen() const { return getSize() * getSize(); }
public:
	Kernel(size_t radius) :radius(radius)
	{
		data = make_unique<float[]>(getLen());
	}
	Kernel(const Kernel& other) :Kernel(other.radius)
	{
		copy(other.data.get(), other.data.get() + getLen(), data.get());
	}
	size_t getRadius()const { return radius; }
	size_t getSize()const { return 2 * radius + 1; }
	float operator [](size_t id)const { return data[id]; }
	float& operator [](size_t id) { return data[id]; }
};
class MatrixFilter :public filter
{
protected:
	Kernel mKernel;
	QColor calcNewPixelColor(const QImage& img, int x, int y)const override;
public:
	MatrixFilter(const Kernel& kernel) :mKernel(kernel) {};
	virtual ~MatrixFilter() = default;
};
class BlurKernel :public Kernel
{
public:
	using Kernel::Kernel;
	BlurKernel(size_t radius = 2) :Kernel(radius)
	{
		for (size_t i = 0; i < getLen(); i++)
			data[i] = 1.0f / getLen();
	}
};
class BlurFilter :public MatrixFilter
{
public:
	BlurFilter(size_t radius = 1) :MatrixFilter(BlurKernel(radius)) {}
};
class GaussianKernel :public Kernel
{
public:
	//наследования конструкторов 
	using Kernel::Kernel;
	GaussianKernel(size_t radius = 2, float sigma = 3.f) :Kernel(radius)
	{
		float norm = 0;
		int signed_radius = static_cast<int>(radius);

		// рассчитываем ядро линейного фильтра
		for (int x = -signed_radius; x <= signed_radius; x++)
			for (int y = -signed_radius; y <= signed_radius; y++)
			{
				size_t idx = (x + radius) * getSize() + (y + radius);

				data[idx] = exp(-(x * x + y * y) / (sigma * sigma));
				norm += data[idx];
			}
		//нормируем ядро
		for (size_t i = 0; i < getLen(); i++)
		{
			data[i] /= norm;
		}
	}



};
class GaussianFilter :public MatrixFilter
{
public:
	GaussianFilter(size_t radius = 7) :MatrixFilter(GaussianKernel(radius)) {}
};
class GrayScaleFilter :public filter
{
	QColor calcNewPixelColor(const QImage& img, int x, int y)const override;
};
class Sepia :public filter
{
	QColor calcNewPixelColor(const QImage& img, int x, int y)const override;

};
class Brightness :public filter
{
	QColor calcNewPixelColor(const QImage& img, int x, int y)const override;

};
class SharpnessKernel :public Kernel
{
public:
	using Kernel::Kernel;
	SharpnessKernel(size_t radius = 1) :Kernel(radius)
	{
		data[0] = -1; data[1] = -1; data[2] = 1;
		data[3] = -1; data[4] = 9; data[5] = -1;
		data[6] = -1; data[7] = -1; data[8] = -1;
	}
};
class SharpnessFilter : public MatrixFilter
{
public:
	SharpnessFilter(size_t radius = 1) : MatrixFilter(SharpnessKernel(radius)) {}
	QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
};
class SobelKernel : public Kernel
{
public:
	using Kernel::Kernel;
	SobelKernel(size_t radius = 1) :Kernel(radius)
	{
		data[0] = -1; data[1] = -2; data[2] = -1;
		data[3] = 0; data[4] = 0; data[5] = 0;
		data[6] = 1; data[7] = 2; data[8] = 1;
	}
};
class SobelFilter : public MatrixFilter
{
public:
	SobelFilter(size_t radius = 1) : MatrixFilter(SobelKernel(radius)) {}
	QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
};
//class PerfectReflectorFilter :public filter
//{
//	QColor calcNewPixelColor(const QImage& img, int x, int y)const  override;
//
//
//};

class PerfectReflectorFilter :public filter
{
	QColor calcNewPixelColor(const QImage& img, int x, int y) const override { QColor color = img.pixelColor(x, y); return color; }
	QColor calcNewPixelColorA(const QImage& img, int x, int y, float maxR, float maxG, float maxB) const;
public:
	QImage process(const QImage& img) const override;
};
class HorizontalWaves :public filter
{
	QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
};
class VerticalWaves : public filter
{
	QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
};
class GlassEffect : public filter
{
	QColor calcNewPixelColor(const QImage& img, int x, int y)const  override;
};
//class LinearStretching : public filter
//{
//	QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
//
//	
//};

class AutolevelsFilter :public filter
{
	QColor calcNewPixelColor(const QImage& img, int x, int y) const override { QColor color = img.pixelColor(x, y); return color; }
	QColor calcNewPixelColorA(const QImage& img, int x, int y, float maxR, float maxG, float maxB, float minR, float minG, float minB) const;
public:
	QImage process(const QImage& img) const override;
};

class MedianKernel : public Kernel
{
public:
	using Kernel::Kernel;
	MedianKernel(std::size_t radius = 1) : Kernel(radius)
	{
		for (int i = 0; i < getLen(); i++)
		{
			data[i] = 1;
		}
	}
};

class MedianFilter : public MatrixFilter
{
protected:
	QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
public:
	MedianFilter(std::size_t radius = 1) : MatrixFilter(MedianKernel(radius)) {}
};
class DilationKernel : public Kernel
{
public:
	using Kernel::Kernel;
	DilationKernel(size_t radius = 1) :Kernel(radius)
	{
		data[0] = 0.0f; data[1] = 1.f; data[2] = 0.f;
		data[3] = 0.f; data[4] = 0.0f; data[5] = 0.f;
		data[6] = 0.f; data[7] = 1.f; data[8] = 0.f;
		
	}
};

class DilationFilter : public MatrixFilter
{
public:
	DilationFilter(size_t radius = 1) : MatrixFilter(DilationKernel(radius)) {}
protected:
	QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
};

//------------------------------------------------------------------------------

class ErosionKernel : public Kernel
{
public:
	using Kernel::Kernel;
	ErosionKernel(size_t radius = 1) :Kernel(radius)
	{

		data[0] = 0.0f; data[1] = 1.f; data[2] = 0.f;
		data[3] = 1.f; data[4] = 1.0f; data[5] = 1.f;
		data[6] = 0.f; data[7] = 1.f; data[8] = 0.f;

		
	}
};

class ErosionFilter :public MatrixFilter
{
protected:
	//	ErosionKernel tmpKernel;
	QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
public:
	ErosionFilter(size_t radius = 1) : MatrixFilter(ErosionKernel(radius)) {}
};
class EmbossingKernel : public Kernel
{
public:
	using Kernel::Kernel;
	EmbossingKernel(size_t radius = 1) :Kernel(radius)
	{
		data[0] = 0.0f; data[1] = 1.f; data[2] = 0.f;
		data[3] = 1.f; data[4] = 0.0f; data[5] = -1.f;
		data[6] = 0.f; data[7] = -1.f; data[8] = 0.f;
	}
};

class EmbossingFilter : public MatrixFilter
{
public:
	EmbossingFilter(size_t radius = 1) : MatrixFilter(EmbossingKernel(radius)) {}
};

//------------------------------------------------------------------------------

class TopHadKernel : public Kernel
{
public:
	using Kernel::Kernel;
	TopHadKernel(size_t radius = 1) :Kernel(radius)
	{
		data[0] = 1.0f; data[1] = 0.f; data[2] = 1.f;
		data[3] = 1.f; data[4] = 0.0f; data[5] = 1.f;
		data[6] = 1.f; data[7] = 0.f; data[8] = 1.f;
	}
};

class TopHatFilter : public MatrixFilter
{
public:
	TopHatFilter(size_t radius = 1) : MatrixFilter(TopHadKernel(radius)) {}
	QColor calcNewPixelColor(const QImage& img, int x, int y) const override { return QColor(); };
	QImage process(const QImage& img) const override;
};
class MotionBlurKernel : public Kernel
{
public:
	using Kernel::Kernel;
	MotionBlurKernel(std::size_t radius = 3) : Kernel(radius)
	{
		int counter = 0;
		for (std::size_t i = 0; i < getLen(); i++)
		{
			if (int(i) == counter)
			{
				data[i] = 1.0 / getSize();
				counter += getSize() + 1;
			}
			else
				data[i] = 0;
		}
	}
};

class MotionBlurFilter : public MatrixFilter
{
public:
	MotionBlurFilter(std::size_t radius = 3) : MatrixFilter(MotionBlurKernel(radius)) {}
};
class OpeningKernel : public Kernel
{
public:
	using Kernel::Kernel;
	OpeningKernel(std::size_t radius = 1) : Kernel(radius)
	{

		for (int i = 0; i < getLen(); i++)
		{
			data[i] = 1;
		}
	}
};

class Opening : public MatrixFilter
{
protected:
	QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
public:
	Opening(size_t radius = 1) : MatrixFilter(OpeningKernel(radius)) {}
	Opening(Kernel& k) :MatrixFilter(k) {}
	QImage process(const QImage& img) const;
};

//Closing

class ClosingKernel : public Kernel
{
public:
	
	ClosingKernel(size_t radius = 1) : Kernel(radius)
	{
		for (int i = 0; i < getLen(); i++)
		{
			data[i] = 1;
		}
	}
};

class Closing : public MatrixFilter
{
protected:
	QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
public:
	Closing(std::size_t radius = 1) : MatrixFilter(ClosingKernel(radius)) {}
	Closing(Kernel& k) :MatrixFilter(k) {}
	QImage process(const QImage& img) const;
};