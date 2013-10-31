#include "pch.h"
#include "SpriteGame.h"
#include <DirectXMath.h>    
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace DirectX;
using namespace Windows::Graphics::Display;
using namespace Coding4Fun::FallFury::DXCore;

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}
SpriteGame::SpriteGame()
{
}

void SpriteGame::CreateDeviceIndependentResources()
{
	DirectXBase::CreateDeviceIndependentResources();

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




}

void SpriteGame::CreateProjectile()
{
	ParticleSystem data;
	data.pos = spaceship->pos;
	data.vel = float2(1000.0f* cos(spaceship->rot), -1000.0f*sin(spaceship->rot));
	data.scale = 1.0f;
	data.SetTexture(m_particle);
	data.SetWindowSize(m_windowBounds);
	m_particleData.push_back(data);
}

void SpriteGame::CreateDeviceResources()
{
	DirectXBase::CreateDeviceResources();

	m_spriteBatch = ref new SpriteBatch();
	unsigned int capacity = 10000;

	m_spriteBatch->Initialize(
		m_d3dDevice.Get(),
		capacity
		);




	// Load the sprite textures.

	BasicLoader^ loader = ref new BasicLoader(m_d3dDevice.Get(), m_wicFactory.Get());

	loader->LoadTexture(
		"Assets\\GameObjects\\m31.png",
		&m_background,
		nullptr
		);

	m_spriteBatch->AddTexture(m_background.Get());


	loader->LoadTexture(
		"Assets\\GameObjects\\spotlight.png",
		&m_player_spot,
		nullptr
		);

	m_spriteBatch->AddTexture(m_player_spot.Get());

	loader->LoadTexture(
		"Assets\\GameObjects\\ida.dds",
		&m_asteroid,
		nullptr
		);
	m_spriteBatch->AddTexture(m_asteroid.Get());

	loader->LoadTexture(
		"Assets\\GameObjects\\SpaceShip.png",
		&m_player,
		nullptr
		);
	m_spriteBatch->AddTexture(m_player.Get());



	loader->LoadTexture(
		"Assets\\GameObjects\\particle.png",
		&m_particle,
		nullptr
		);
	m_spriteBatch->AddTexture(m_particle.Get());

	// Create the Gaussian Blur Effect
 

	// Create a bitmap source effect and bind the WIC format converter to it.
	m_d2dContext->CreateEffect(CLSID_D2D1BitmapSource, &m_bitmapSourceEffect);
	ThrowIfFailed(m_bitmapSourceEffect->SetValue(D2D1_BITMAPSOURCE_PROP_WIC_BITMAP_SOURCE, m_wicConverter.Get()));

	// Because the image will not be changing, we should cache the effect for performance reasons.
	ThrowIfFailed(m_bitmapSourceEffect->SetValue(D2D1_PROPERTY_CACHED, FALSE));

	// Create effects and set their input to the BitmapSource effect. These lighting effects use the alpha channel
	// of the inputed image as a heightmap - higher values represent higher elevations off the surface of the image.
	ThrowIfFailed(m_d2dContext->CreateEffect(CLSID_D2D1PointSpecular, &m_pointSpecularEffect));
	m_pointSpecularEffect->SetInputEffect(0, m_bitmapSourceEffect.Get());

	ThrowIfFailed(m_d2dContext->CreateEffect(CLSID_D2D1SpotSpecular, &m_spotSpecularEffect));
	m_spotSpecularEffect->SetInputEffect(0, m_bitmapSourceEffect.Get());

	ThrowIfFailed(m_d2dContext->CreateEffect(CLSID_D2D1DistantSpecular, &m_distantSpecularEffect));
	m_distantSpecularEffect->SetInputEffect(0, m_bitmapSourceEffect.Get());

	ThrowIfFailed(m_d2dContext->CreateEffect(CLSID_D2D1PointDiffuse, &m_pointDiffuseEffect));
	m_pointDiffuseEffect->SetInputEffect(0, m_bitmapSourceEffect.Get());

	ThrowIfFailed(m_d2dContext->CreateEffect(CLSID_D2D1SpotDiffuse, &m_spotDiffuseEffect));
	m_spotDiffuseEffect->SetInputEffect(0, m_bitmapSourceEffect.Get());

	ThrowIfFailed(m_d2dContext->CreateEffect(CLSID_D2D1DistantDiffuse, &m_distantDiffuseEffect));
	m_distantDiffuseEffect->SetInputEffect(0, m_bitmapSourceEffect.Get());

	// m_currentEffect represents the current effect being rendered in the Render() method. At startup,
	// we default to m_pointSpecularEffect.
	m_currentEffect = m_spotSpecularEffect;
}

void SpriteGame::CreateWindowSizeDependentResources()
{
	DirectXBase::CreateWindowSizeDependentResources();



	background = new SlidingBackgroundSprite();

	background->SetTexture(m_background);
	background->SetWindowSize(m_windowBounds);
	background->InitSliding();

	// Randomly generate some non-interactive asteroids to fit the screen.
	m_asteroidData.clear();
	for (int i = 0; i < 100; i++)
	{
		Asteroid data;
		data.pos.x = RandFloat(0.0f, m_windowBounds.Width);
		data.pos.y = RandFloat(0.0f, m_windowBounds.Height);
		float tempRot = RandFloat(-PI_F, PI_F);
		float tempMag = RandFloat(0.0f, 17.0f);
		data.vel.x = tempMag * cosf(tempRot);
		data.vel.y = tempMag * sinf(tempRot);
		data.rot = 0;//RandFloat(-PI_F, PI_F);
		data.scale = RandFloat(0.1f, 1.0f);
		data.rotVel = RandFloat(-PI_F, PI_F) / (7.0f + 3.0f * data.scale);
		data.size = (180, 110);
		data.SetTexture(m_asteroid);
		data.SetWindowSize(m_windowBounds);
		m_asteroidData.push_back(data);
	}


	spaceship = new Player();

	spaceship->pos.x = (float) m_windowBounds.Width / 2.0;
	spaceship->pos.y = (float) m_windowBounds.Height / 2.0;
	float tempRot = RandFloat(-PI_F, PI_F);
	float tempMag = RandFloat(0.0f, 17.0f);
	spaceship->vel.x = 10;
	spaceship->vel.y = 0;
	spaceship->rot = 0;
	spaceship->rotVel =0;
	spaceship->scale = .75;
	spaceship->SetTexture(m_player);
	spaceship->spot_texture = m_player_spot;
	spaceship->SetWindowSize(m_windowBounds);


	rocketFuel = new RocketFire();
	rocketFuel->pos = spaceship->pos;
	rocketFuel->pos.x -= spaceship->textureSize.Width / 2.0*spaceship->scale;
	rocketFuel->SetTexture(m_particle);

	rocketFuel->ParentObject = spaceship;
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

	D2D1_SIZE_F size = m_d2dContext->GetSize();

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

	ThrowIfFailed(m_spotSpecularEffect->SetValue(D2D1_SPOTSPECULAR_PROP_FOCUS, 15.4f));
	ThrowIfFailed(m_spotSpecularEffect->SetValue(D2D1_SPOTSPECULAR_PROP_SPECULAR_CONSTANT, 1.2f));
	ThrowIfFailed(m_spotSpecularEffect->SetValue(D2D1_SPOTSPECULAR_PROP_LIMITING_CONE_ANGLE, 70.0f));
	 

	ThrowIfFailed(m_pointSpecularEffect->SetValue(D2D1_POINTSPECULAR_PROP_SURFACE_SCALE, 10.f));
	ThrowIfFailed(m_pointSpecularEffect->SetValue(D2D1_POINTSPECULAR_PROP_SPECULAR_EXPONENT, 1.2f));
	 
}

void SpriteGame::Update(float timeTotal, float timeDelta)
{


	background->Update(timeDelta);
	spaceship->Update(timeDelta);

	//D2D1_VECTOR_3F lightPosition = D2D1::Vector3F(spaceship->pos.x+spaceship->textureSize.Width/2.0*spaceship->scale-30, spaceship->pos.y,25);
	D2D1_VECTOR_3F lighttarget = D2D1::Vector3F(spaceship->pos.x + (200  )*cos(spaceship->rot), spaceship->pos.y -(200)*sin(spaceship->rot), 5);//+ tan(spaceship->lightAngle* 3.14 / 180.0) * 100


	D2D1_VECTOR_3F lightPosition = D2D1::Vector3F(spaceship->pos.x + (-70 )*cos(spaceship->rot), spaceship->pos.y - (-70)*sin(spaceship->rot), 25);
	//D2D1_VECTOR_3F lighttarget = D2D1::Vector3F(spaceship->pos.x, spaceship->pos.y + tan(spaceship->lightAngle* 3.14 / 180.0) * 100, 5);
	// The DistantSpecular and DistantDiffuse effects do not have a LightPosition property. 
	ThrowIfFailed(m_pointSpecularEffect->SetValue(D2D1_POINTSPECULAR_PROP_LIGHT_POSITION, lightPosition));
	ThrowIfFailed(m_spotSpecularEffect->SetValue(D2D1_SPOTSPECULAR_PROP_LIGHT_POSITION, lightPosition));
	ThrowIfFailed(m_spotSpecularEffect->SetValue(D2D1_SPOTSPECULAR_PROP_POINTS_AT, lighttarget));
	rocketFuel->pos = float2(spaceship->pos.x - (spaceship->textureSize.Width / 2.0*spaceship->scale  )*cos(spaceship->rot),
		spaceship->pos.y +(spaceship->textureSize.Height / 2.0*spaceship->scale )*sin(spaceship->rot ));


	rocketFuel->Update(timeDelta); 
	for (auto asteroid = m_asteroidData.begin(); asteroid != m_asteroidData.end(); asteroid++)
	{
		asteroid->Update(timeDelta);
		if (spaceship->IsColliding(*asteroid))
		{
			asteroid = m_asteroidData.erase(asteroid);

			if (asteroid == m_asteroidData.end())
				break;
		}

		for (auto particle = m_particleData.begin(); particle != m_particleData.end(); particle++)
		{
			if (particle->IsColliding(*asteroid))
			{
				asteroid->vel = particle->vel;
				particle = m_particleData.erase(particle);

				if (particle == m_particleData.end())
					break;

			}
		}
	}
	for (auto particle = m_particleData.begin(); particle != m_particleData.end(); particle++)
	{
		particle->Update(timeDelta);

		if (particle->IsOutOfVisibleArea())
		{
			particle = m_particleData.erase(particle);

			if (particle == m_particleData.end())
				break;
		}

		 

	}


}

void SpriteGame::Render()
{
	m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);

	m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), reinterpret_cast<float*>(&D2D1::ColorF(D2D1::ColorF::AntiqueWhite)));

	m_spriteBatch->Begin(); 
	for (auto asteroid = m_asteroidData.begin(); asteroid != m_asteroidData.end(); asteroid++)
		asteroid->Draw(m_spriteBatch);
	m_spriteBatch->End();

	m_d2dContext->BeginDraw();

	// Draw the currently selected effect. By using SOURCE_COPY, the screen does
	// not need to be cleared prior to each render since the pixels are completely
	// overwritten, not blended as with SOURCE_OVER.
	m_d2dContext->DrawImage(
		m_currentEffect.Get(),
		D2D1_INTERPOLATION_MODE_LINEAR,

		D2D1_COMPOSITE_MODE_DESTINATION_ATOP
		);

	// We ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
	// is lost. It will be handled during the next call to Present.
	HRESULT hr = m_d2dContext->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		ThrowIfFailed(hr);
	}
	m_spriteBatch->Begin();
	//background->Draw(m_spriteBatch);

	for (auto particle = m_particleData.begin(); particle != m_particleData.end(); particle++)
		particle->Draw(m_spriteBatch);


	rocketFuel->Draw(m_spriteBatch);
	spaceship->Draw(m_spriteBatch);

	m_spriteBatch->End();
}

void SpriteGame::CheckScreenType()
{




}


float SpriteGame::RandFloat(float min, float max)
{
	return (static_cast<float>(rand() % RAND_MAX) / static_cast<float>(RAND_MAX)) * (max - min) + min;
}



bool SpriteGame::IsWithinScreenBoundaries(float2 position)
{
	/*if (position.x < CurrentGameScreen->LoBoundX || position.x > CurrentGameScreen->HiBoundX || position.y < CurrentGameScreen->LoBoundY || position.y > CurrentGameScreen->HiBoundY)
		return false;
		else
		return true;*/return true;

}