
#pragma once
#include "GameObject.h"

#include "DXCore\DirectXBase.h" 

class d2dLightEffect : public GameObject
{

	Microsoft::WRL::ComPtr<IWICFormatConverter>             m_wicConverter;
	D2D1_SIZE_U                                             m_imageSize;

	Microsoft::WRL::ComPtr<ID2D1DeviceContext>				_m_d2dContext;
	Microsoft::WRL::ComPtr<IWICImagingFactory2>            _m_wicFactory;

	float                                                   m_lightPositionZ;

public:
	Microsoft::WRL::ComPtr<ID2D1Effect>                     m_bitmapSourceEffect;
	Microsoft::WRL::ComPtr<ID2D1Effect>                     m_currentEffect;
	Microsoft::WRL::ComPtr<ID2D1Effect>                     m_pointSpecularEffect;
	Microsoft::WRL::ComPtr<ID2D1Effect>                     m_spotSpecularEffect;
	Microsoft::WRL::ComPtr<ID2D1Effect>                     m_distantSpecularEffect;
	Microsoft::WRL::ComPtr<ID2D1Effect>                     m_pointDiffuseEffect;
	Microsoft::WRL::ComPtr<ID2D1Effect>                     m_spotDiffuseEffect;
	Microsoft::WRL::ComPtr<ID2D1Effect>                     m_distantDiffuseEffect;
	 

	void initWithImageFromFile(Microsoft::WRL::ComPtr<ID2D1DeviceContext>   m_d2dContext, Microsoft::WRL::ComPtr<IWICImagingFactory2>            m_wicFactory);
	void  InitWindowDependentProperties(Windows::Foundation::Size   m_renderTargetSize);
	void Update(GameObject* parent);

	void Draw();


};