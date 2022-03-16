#include "Sprite.h"
#include "src/core/SkAutoMalloc.h"



Sprite::Sprite(sk_sp<SkImage> *pSpriteImage)
{
	SpriteImage = *pSpriteImage;
	SetPosition(0, 0);
	SetSize(SpriteImage->width(), SpriteImage->height());
	opacity = 1.0;
	anchor.set(0.5, 0.5);
}

//SkPoint
Sprite::Sprite(const char *pImagePath)
{
	sk_sp<SkData> blob = SkData::MakeFromFileName(pImagePath);
	if (blob == NULL) return;

	SpriteImage = SkImage::MakeFromEncoded(blob);
	SetPosition(0, 0);
	SetSize(SpriteImage->width(), SpriteImage->height());
	opacity = 1.0;
	anchor.set(0.5, 0.5);
}

Sprite::~Sprite()
{
	this->StopAllAction();
	SpriteImage.get()->unref();
	SpriteImage.release();
}

void Sprite::Draw(SkCanvas* canvas)
{
	if (IsVisible() == false)
		return;
	SkPaint paint;
	paint.setAlpha(opacity * 255);

	/*SkColorMatrix matrix;
	matrix.setRotate(SkColorMatrix::kG_Axis, 180);
	paint.setColorFilter(SkColorFilter::MakeMatrixFilterRowMajor255(matrix.fMat));*/

	canvas->save();
	canvas->rotate(GetDegress(), GetBound().left() + GetWidth()*anchor.fX, GetBound().top() + GetHeight()*anchor.fY);
    canvas->drawImageRect(SpriteImage.get(), GetBound(), SkSamplingOptions(), &paint);
	canvas->restore();
	//sp_canvas = canvas;
}

#include "include/core/SkGraphics.h"
#include "include/core/SkImage.h"

#include "include/core/SkSurface.h"

Sprite * Sprite::readPixels(const SkRect& subset, bool bRotated)
{

	//SkImageInfo info = SkImageInfo::MakeN32(GetWidth(), GetHeight(), kOpaque_SkAlphaType);
	//SkPixmap pixmap;
	//SpriteImage->readPixels(info, pixmap.writable_addr(), pixmap.rowBytes(), 100, 100);
	////SkPixmap pixmap(info,0,GetWidth()*GetHeight()*4);
	////SpriteImage->readPixels(pixmap, 100, 100, SkImage::kAllow_CachingHint);

	////SkSurface::MakeRaster

	//SkBitmap bitmap;
	//bitmap.installPixels(pixmap);
	////SkImage::MakeFromBitmap(bitmap);
	size_t rowBytes;
	SkBitmap bm;
	if (bRotated == false)
	{
		rowBytes = 4 * subset.width();
		SkAutoMalloc buffer(rowBytes * subset.height());
		SkImageInfo dstInfo = SkImageInfo::Make(subset.width(), subset.height(), kN32_SkColorType, kPremul_SkAlphaType);
		if (!SpriteImage->readPixels(dstInfo, buffer.get(), rowBytes, subset.left(), subset.top())) {
			SkDebugf("readPixels failed\n");
			return false;
		}
		bm.installPixels(dstInfo, buffer.get(), rowBytes);
	}
	else
	{
        rowBytes = 4 * subset.height();
        SkAutoMalloc buffer(rowBytes * subset.width() );
        SkImageInfo dstInfo = SkImageInfo::Make(subset.height() ,
                                                subset.width() ,
                                                kN32_SkColorType,
                                                kPremul_SkAlphaType);
		if (!SpriteImage->readPixels(dstInfo, buffer.get(), rowBytes, subset.left(), subset.top())) {
			SkDebugf("readPixels failed\n");
			return false;
		}
		bm.installPixels(dstInfo, buffer.get(), rowBytes);


		SkCanvas *pCanvas = new SkCanvas(bm);
        SkImageInfo info = SkImageInfo::MakeN32(subset.width() ,
                                                subset.height(),
                                                kOpaque_SkAlphaType);
        //SkImageInfo info =SkImageInfo::MakeN32(subset.height(), subset.width(), kOpaque_SkAlphaType);
		auto gpuSurface = pCanvas->makeSurface(info);
		auto surfaceCanvas = gpuSurface->getCanvas();
		SkPaint paint;
		//surfaceCanvas->clear(SK_ColorWHITE);
		surfaceCanvas->save();
		surfaceCanvas->translate(0, subset.height());
       surfaceCanvas->rotate(270);
      
       sk_sp<SkImage> skSrc = bm.asImage();
       surfaceCanvas->drawImage(skSrc, 0, 0);
       
        //surfaceCanvas->drawBitmap(bm, 0, 0, &paint);
        
		surfaceCanvas->restore();
		//surfaceCanvas->save();
		//surfaceCanvas->rotate(270);
		//surfaceCanvas->translate(90, 0);
		//surfaceCanvas->clear(SK_ColorWHITE);
		
		//surfaceCanvas->drawBitmap(bm, 0, 0, &paint);
		//surfaceCanvas->rotate(270,0,0);
	//	surfaceCanvas->drawBitmap(bm, 0, 0, &paint);
		//surfaceCanvas->restore();
		//surfaceCanvas->rotate(360);
		sk_sp<SkImage> image(gpuSurface->makeImageSnapshot());
		Sprite *pSprite = new Sprite(&image);
        pSprite->SetDegress(180);
		return pSprite;


		
	}
	
    sk_sp < SkImage> bb=SkImage::MakeFromBitmap(bm);
    Sprite* pSprite = new Sprite(&bb);
	if (bRotated == true)
		pSprite->SetDegress(270);
	return pSprite;
}



void Sprite::SetScale(SkScalar sx, SkScalar sy)
{
	SkImageInfo info = SkImageInfo::MakeN32Premul(SpriteImage->width() * sx, SpriteImage->height() * sy);
	SkAutoPixmapStorage scaled;
	scaled.alloc(info);

	//SpriteImage->scalePixels(scaled, kHigh_SkFilterQuality, SkImage::kDisallow_CachingHint);
    SpriteImage->scalePixels(
            scaled, SkSamplingOptions(SkFilterMode::kLinear), SkImage::kDisallow_CachingHint);
	SpriteImage.get()->unref();
	SpriteImage.release();

	SpriteImage = SkImage::MakeRasterCopy(scaled);
	SetSize(SpriteImage->width(), SpriteImage->height());
}

void Sprite::SetScale(SkScalar scale)
{
	SetScale(scale, scale);
}

void Sprite::SetScaleX(SkScalar sx)
{
	SetScale(sx, 1.0);
}

void Sprite::SetScaleY(SkScalar sy)
{
	SetScale(1.0, sy);
}

void Sprite::SetAnchorPoint(SkPoint po)
{
	anchor = po;
}


void Sprite::OnMouseMove(int x, int y)
{
}

bool  Sprite::OnMouseDown(int x, int y)
{
}

//void draw(SkCanvas* canvas) {
//	std::vector<int32_t> srcPixels;
//	int quarterWidth = image->width() / 16;
//	int rowBytes = quarterWidth * 4;
//	int quarterHeight = image->height() / 16;
//	srcPixels.resize(quarterHeight * rowBytes);
//	SkPixmap pixmap(SkImageInfo::MakeN32Premul(quarterWidth, quarterHeight),
//		&srcPixels.front(), rowBytes);
//	canvas->scale(4, 4);
//	SkFilterQuality qualities[] = { kNone_SkFilterQuality, kLow_SkFilterQuality,
//					 kMedium_SkFilterQuality, kHigh_SkFilterQuality };
//	for (unsigned index = 0; index < SK_ARRAY_COUNT(qualities); ++index) {
//		image->scalePixels(pixmap, qualities[index]);
//		sk_sp<SkImage> filtered = SkImage::MakeFromRaster(pixmap, nullptr, nullptr);
//		canvas->drawImage(filtered, 16 * index, 0);
//	}
//}
//void draw(SkCanvas* canvas) {
//	SkPaint paint;
//	SkPictureRecorder recorder;
//	SkCanvas* recordingCanvas = recorder.beginRecording(50, 50);
//	for (auto color : { SK_ColorRED, SK_ColorBLUE, 0xff007f00 }) {
//		paint.setColor(color);
//		recordingCanvas->drawRect({ 10, 10, 30, 40 }, paint);
//		recordingCanvas->translate(10, 10);
//		recordingCanvas->scale(1.2f, 1.4f);
//	}
//	sk_sp<SkPicture> playback = recorder.finishRecordingAsPicture();
//	int x = 0, y = 0;
//	for (auto alpha : { 70, 140, 210 }) {
//		paint.setAlpha(alpha);
//		auto srgbColorSpace = SkColorSpace::MakeSRGB();
//		sk_sp<SkImage> image = SkImage::MakeFromPicture(playback, { 50, 50 }, nullptr, &paint,
//			SkImage::BitDepth::kU8, srgbColorSpace);
//		canvas->drawImage(image, x, y);
//		x += 70; y += 70;
//	}
//}
