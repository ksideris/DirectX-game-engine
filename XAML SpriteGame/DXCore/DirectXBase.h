#pragma once

#include "DirectXLinker.h"
#include <agile.h>

#pragma warning (disable: 4449)

namespace Coding4Fun
{
	namespace FallFury
	{
		namespace DXCore
		{
			// Helper class that initializes DirectX APIs
			ref class DirectXBase abstract
			{
			internal:
				DirectXBase();

			public:
				virtual void Initialize(Windows::UI::Core::CoreWindow^ window, Windows::UI::Xaml::Controls::SwapChainBackgroundPanel^ panel, float dpi);
				virtual void CreateDeviceIndependentResources();
				virtual void CreateDeviceResources();
				virtual void SetDpi(float dpi);
				virtual void CreateWindowSizeDependentResources();
				virtual void UpdateForWindowSizeChange();
				virtual void Render() = 0;
				virtual void Present();
				virtual float ConvertDipsToPixels(float dips);

				virtual void OnPointerMoved(Windows::UI::Core::CoreWindow^ window,Windows::UI::Core::PointerEventArgs^ pArgs);
				virtual void OnPointerReleased(Windows::UI::Core::CoreWindow^ window,Windows::UI::Core::PointerEventArgs^ pArgs);
				virtual void OnPointerPressed(Windows::UI::Core::CoreWindow^ window,Windows::UI::Core::PointerEventArgs^ pArgs);

			internal:
				Platform::Agile<Windows::UI::Core::CoreWindow>         m_window;
				Windows::UI::Xaml::Controls::SwapChainBackgroundPanel^ m_panel;

				// Direct2D Objects
				Microsoft::WRL::ComPtr<ID2D1Factory1>                  m_d2dFactory;
				Microsoft::WRL::ComPtr<ID2D1Device>                    m_d2dDevice;
				Microsoft::WRL::ComPtr<ID2D1DeviceContext>             m_d2dContext;
				Microsoft::WRL::ComPtr<ID2D1Bitmap1>                   m_d2dTargetBitmap;

				// DirectWrite & Windows Imaging Component Objects
				Microsoft::WRL::ComPtr<IDWriteFactory1>                m_dwriteFactory;
				Microsoft::WRL::ComPtr<IWICImagingFactory2>            m_wicFactory;

				// Direct3D Objects
				Microsoft::WRL::ComPtr<ID3D11Device1>                  m_d3dDevice;
				Microsoft::WRL::ComPtr<ID3D11DeviceContext1>           m_d3dContext;
				Microsoft::WRL::ComPtr<IDXGISwapChain1>                m_swapChain;
				Microsoft::WRL::ComPtr<ID3D11RenderTargetView>         m_renderTargetView;
				Microsoft::WRL::ComPtr<ID3D11DepthStencilView>         m_depthStencilView;

				D3D_FEATURE_LEVEL                                      m_featureLevel;
				Windows::Foundation::Size                              m_renderTargetSize;
				Windows::Foundation::Rect                              m_windowBounds;
				float                                                  m_dpi;
			};
		}
	}
}
#pragma warning (default: 4449)
