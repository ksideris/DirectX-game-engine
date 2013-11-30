#include "pch.h"
#include "Direct2DlightEffect.h" 
#include <DirectXMath.h>    

using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core; 
using namespace Windows::Graphics::Display;
using namespace DXCore;



void d2dLightEffect::initWithImageFromFile( ComPtr<ID2D1DeviceContext>   m_d2dContext, ComPtr<IWICImagingFactory2>            m_wicFactory)
{

	_m_d2dContext = m_d2dContext;
	_m_wicFactory = m_wicFactory;
	// Load an image from a Windows Imaging Component decoder.
	ComPtr<IWICBitmapDecoder> decoder;
	ThrowIfFailed(
		m_wicFactory->CreateDecoderFromFilename(
		L"Assets\\GameObjects\\heightmap.png",
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&decoder
		)
		);

	ComPtr<IWICBitmapFrameDecode> frame;
	ThrowIfFailed(
		decoder->GetFrame(0, &frame)
		);

	ThrowIfFailed(
		m_wicFactory->CreateFormatConverter(&m_wicConverter)
		);

	ThrowIfFailed(
		m_wicConverter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom
		)
		);

	// Get the size of the image.
	unsigned int width, height;
	ThrowIfFailed(m_wicConverter->GetSize(&width, &height));
	m_imageSize = D2D1::SizeU(width, height);




	// Create a bitmap source effect and bind the WIC format converter to it.
	m_d2dContext->CreateEffect(CLSID_D2D1BitmapSource, &m_bitmapSourceEffect);
	ThrowIfFailed(m_bitmapSourceEffect->SetValue(D2D1_BITMAPSOURCE_PROP_WIC_BITMAP_SOURCE, m_wicConverter.Get()));

	// Because the image will not be changing, we should cache the effect for performance reasons.
	ThrowIfFailed(m_bitmapSourceEffect->SetValue(D2D1_PROPERTY_CACHED, FALSE));

	// Create effects and set their input to the BitmapSource effect. These lighting effects use the alpha channel
	// of the inputed image as a heightmap - higher values represent higher elevations off the surface of the image.
	ThrowIfFailed(_m_d2dContext->CreateEffect(CLSID_D2D1PointSpecular, &m_pointSpecularEffect));
	m_pointSpecularEffect->SetInputEffect(0, m_bitmapSourceEffect.Get());

	ThrowIfFailed(_m_d2dContext->CreateEffect(CLSID_D2D1SpotSpecular, &m_spotSpecularEffect));
	m_spotSpecularEffect->SetInputEffect(0, m_bitmapSourceEffect.Get());

	ThrowIfFailed(_m_d2dContext->CreateEffect(CLSID_D2D1DistantSpecular, &m_distantSpecularEffect));
	m_distantSpecularEffect->SetInputEffect(0, m_bitmapSourceEffect.Get());

	ThrowIfFailed(_m_d2dContext->CreateEffect(CLSID_D2D1PointDiffuse, &m_pointDiffuseEffect));
	m_pointDiffuseEffect->SetInputEffect(0, m_bitmapSourceEffect.Get());

	ThrowIfFailed(_m_d2dContext->CreateEffect(CLSID_D2D1SpotDiffuse, &m_spotDiffuseEffect));
	m_spotDiffuseEffect->SetInputEffect(0, m_bitmapSourceEffect.Get());

	ThrowIfFailed(_m_d2dContext->CreateEffect(CLSID_D2D1DistantDiffuse, &m_distantDiffuseEffect));
	m_distantDiffuseEffect->SetInputEffect(0, m_bitmapSourceEffect.Get());

	// m_currentEffect represents the current effect being rendered in the Render() method. At startup,
	// we default to m_pointSpecularEffect.
	m_currentEffect = m_spotSpecularEffect;
}


void d2dLightEffect::InitWindowDependentProperties(Size      m_renderTargetSize)
{
	// Resize the image depending on the layout.
	if (m_renderTargetSize.Width < 1024.0)
	{
		// Scale the image to fit vertically.
		D2D1_VECTOR_2F scale = D2D1::Vector2F(
			1.0f,
			m_renderTargetSize.Height / m_imageSize.height
			);

		ThrowIfFailed(
			m_bitmapSourceEffect->SetValue(D2D1_BITMAPSOURCE_PROP_SCALE, scale)
			);
	}
	else
	{
		// Scale the image to fit horizontally and vertically.
		D2D1_VECTOR_2F scale = D2D1::Vector2F(
			m_renderTargetSize.Width / m_imageSize.width,
			m_renderTargetSize.Height / m_imageSize.height
			);

		ThrowIfFailed(
			m_bitmapSourceEffect->SetValue(D2D1_BITMAPSOURCE_PROP_SCALE, scale)
			);
	}

	D2D1_SIZE_F size = _m_d2dContext->GetSize();

	// Set the pointsAt property for the Spot effects to the center of the screen.
	D2D1_VECTOR_3F pointsAt = D2D1::Vector3F(
		size.width / 2.0F,
		size.height / 2.0F,
		0.0f
		);
	m_lightPositionZ = 100;
	// The Z position is manually set by the user.
	ThrowIfFailed(m_spotSpecularEffect->SetValue(D2D1_SPOTSPECULAR_PROP_POINTS_AT, pointsAt));
	ThrowIfFailed(m_spotDiffuseEffect->SetValue(D2D1_SPOTDIFFUSE_PROP_POINTS_AT, pointsAt));
	ThrowIfFailed(m_spotSpecularEffect->SetValue(D2D1_SPOTSPECULAR_PROP_SURFACE_SCALE, 10.f));

	ThrowIfFailed(m_spotSpecularEffect->SetValue(D2D1_SPOTSPECULAR_PROP_FOCUS, 60.4f));
	ThrowIfFailed(m_spotSpecularEffect->SetValue(D2D1_SPOTSPECULAR_PROP_SPECULAR_CONSTANT, 1.2f));
	ThrowIfFailed(m_spotSpecularEffect->SetValue(D2D1_SPOTSPECULAR_PROP_LIMITING_CONE_ANGLE, 45.0f));


	ThrowIfFailed(m_pointSpecularEffect->SetValue(D2D1_POINTSPECULAR_PROP_SURFACE_SCALE, 10.f));
	ThrowIfFailed(m_pointSpecularEffect->SetValue(D2D1_POINTSPECULAR_PROP_SPECULAR_EXPONENT, 1.2f));
}

void d2dLightEffect::Update(GameObject *parent)
{

	//D2D1_VECTOR_3F lightPosition = D2D1::Vector3F(spaceship->pos.x+spaceship->textureSize.Width/2.0*spaceship->scale-30, spaceship->pos.y,25);
	D2D1_VECTOR_3F lighttarget = D2D1::Vector3F(parent->GetPos().x + (200)*cos(parent->GetRot()), parent->GetPos().y - (200)*sin(parent->GetRot()),40 - 10);//+ tan(spaceship->lightAngle* 3.14 / 180.0) * 100


	D2D1_VECTOR_3F lightPosition = D2D1::Vector3F(parent->GetPos().x + (-200)*cos(parent->GetRot()), parent->GetPos().y - (-200)*sin(parent->GetRot()),40);
	 
	// The DistantSpecular and DistantDiffuse effects do not have a LightPosition property. 
	ThrowIfFailed(m_pointSpecularEffect->SetValue(D2D1_POINTSPECULAR_PROP_LIGHT_POSITION, lightPosition));
	ThrowIfFailed(m_spotSpecularEffect->SetValue(D2D1_SPOTSPECULAR_PROP_LIGHT_POSITION, lightPosition));
	ThrowIfFailed(m_spotSpecularEffect->SetValue(D2D1_SPOTSPECULAR_PROP_POINTS_AT, lighttarget)); 
}

void d2dLightEffect::Draw()
{
	_m_d2dContext->BeginDraw();

	// Draw the currently selected effect. By using SOURCE_COPY, the screen does
	// not need to be cleared prior to each render since the pixels are completely
	// overwritten, not blended as with SOURCE_OVER.
	_m_d2dContext->DrawImage(
		m_currentEffect.Get(),
		D2D1_INTERPOLATION_MODE_LINEAR,
		D2D1_COMPOSITE_MODE_DESTINATION_IN
		);

	// We ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
	// is lost. It will be handled during the next call to Present.
	HRESULT hr =_m_d2dContext->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		ThrowIfFailed(hr);
	}
}