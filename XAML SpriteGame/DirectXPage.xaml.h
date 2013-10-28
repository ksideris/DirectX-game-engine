#pragma once

#include "DirectXPage.g.h"
#include "SpriteGame.h"
#include "Helpers\Timer.h"  

using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml::Media::Animation;
using namespace Windows::UI::Xaml::Controls;

namespace Coding4Fun
{
	namespace FallFury
	{
		[Windows::Foundation::Metadata::WebHostHidden]
		public ref class DirectXPage sealed
		{
		public:
			DirectXPage();




		private:
			bool IsInitialDataLoaded;


			void UpdateWindowSize();
			 



			Windows::Foundation::EventRegistrationToken	WindowActivationToken;


			void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args);
			void OnWindowActivated(Object^ sender, Windows::UI::Core::WindowActivatedEventArgs^ e);
			void OnLogicalDpiChanged(Platform::Object^ sender);
			void OnRendering(Object^ sender, Object^ args);


			Windows::Foundation::EventRegistrationToken m_eventToken;
			ApplicationViewState PreviousState;


			SpriteGame^ m_renderer;


			Coding4Fun::FallFury::Helpers::Timer^ m_timer;

			void OnLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
			void OnKeyDown(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e);

			void LoadSettings();
			void CheckGameState();




			void SwitchGameState();



			void XAMLPage_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
			void XAMLPage_LayoutUpdated(Platform::Object^ sender, Platform::Object^ e);


			void OnKeyUp(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e);
		};
	}
}
