#pragma once

#include "DirectXPage.g.h"
#include "SpriteGame.h"
#include "Helpers\Timer.h"  
#include "Audio\AudioManager.h"

using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml::Media::Animation;
using namespace Windows::UI::Xaml::Controls;
using namespace Helpers;

namespace GameEngine
{

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DirectXPage sealed
	{
	public:
		DirectXPage();

	private:

		int health;
		bool IsInitialDataLoaded;
		void UpdateWindowSize();


		std::vector<float> framerate;
		std::vector<int> scores;

		Windows::Foundation::EventRegistrationToken	WindowActivationToken;


		void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args);
		void OnWindowActivated(Object^ sender, Windows::UI::Core::WindowActivatedEventArgs^ e);
		void OnLogicalDpiChanged(Platform::Object^ sender);
		void OnRendering(Object^ sender, Object^ args);


		Windows::Foundation::EventRegistrationToken m_eventToken;
		ApplicationViewState PreviousState;


		SpriteGame^ m_renderer;
		Timer^ m_timer;

		void OnLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnKeyDown(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e);

		void LoadSettings();
		
		void XAMLPage_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);


		void OnKeyUp(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e);
		void XAMLPage_PointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void XAMLPage_PointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void XAMLPage_PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void OnDataRequested(Windows::ApplicationModel::DataTransfer::DataTransferManager ^sender, Windows::ApplicationModel::DataTransfer::DataRequestedEventArgs ^args);
		void OnCommandsRequested(Windows::UI::ApplicationSettings::SettingsPane ^sender, Windows::UI::ApplicationSettings::SettingsPaneCommandsRequestedEventArgs ^args);
		void OnSettingsSelected(Windows::UI::Popups::IUICommand^ command);
		void OnTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e);
		void OnLevelTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e);
		void OnTappedShare(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e);


		void writeToText(Platform::String^ Filename,Platform::String^ text);
		void CheckNCreateFile(Platform::String^ Filename); 
		void LoadText(Platform::String^ Filename);
		void OnSizeChanged(Platform::Object ^sender, Windows::UI::Xaml::SizeChangedEventArgs ^e);

		void LoadHighScores(Platform::String^ Filename);
	};
}
