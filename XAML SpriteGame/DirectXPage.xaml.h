#pragma once

#include "DirectXPage.g.h"
#include "SpriteGame.h"
#include "Helpers\Timer.h"  
#include "Audio\AudioManager.h"

#include "IrrXML\irrXML.h"
#define SAVEFILE "highscores.dat"

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
		void Suspend();
		void Resume();


	private:
		LONGLONG time_at_pause;
		int health;
		bool IsInitialDataLoaded;
		void UpdateWindowSize();
		bool hudIsClicked;
		bool Paused;
		std::vector<float> framerate;
		std::deque<int> scores;

		GameState previous_state;

		Windows::Storage::StorageFile^  save_file;

		Windows::Foundation::EventRegistrationToken	WindowActivationToken;


		void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args);
		void OnWindowActivated(Object^ sender, Windows::UI::Core::WindowActivatedEventArgs^ e);

#ifdef W8_1
		void OnDpiChanged(Windows::Graphics::Display::DisplayInformation ^sender, Platform::Object ^args);
#else
		void OnLogicalDpiChanged(Platform::Object^ sender);
#endif
		void OnRendering(Object^ sender, Object^ args);


		Windows::Foundation::EventRegistrationToken m_eventToken;
	 

		Windows::UI::Xaml::Controls::Grid^ active_UI;

		SpriteGame^ m_renderer;
		Timer^ m_timer;

		void OnLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnKeyDown(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e);
		 
		
		void XAMLPage_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);


		void OnKeyUp(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e); 
		void OnDataRequested(Windows::ApplicationModel::DataTransfer::DataTransferManager ^sender, Windows::ApplicationModel::DataTransfer::DataRequestedEventArgs ^args);
		void OnCommandsRequested(Windows::UI::ApplicationSettings::SettingsPane ^sender, Windows::UI::ApplicationSettings::SettingsPaneCommandsRequestedEventArgs ^args);
		void OnSettingsSelected(Windows::UI::Popups::IUICommand^ command);
		void OnTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e);
		void OnShootTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e);
		void OnLevelTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e);
		void OnTappedShare(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e);


		void writeToText(Platform::String^ Filename,Platform::String^ text);
		void CheckNCreateFile(Platform::String^ Filename); 
		void LoadText(Platform::String^ Filename);
		void OnSizeChanged(Platform::Object ^sender, Windows::UI::Xaml::SizeChangedEventArgs ^e);

		void LoadHighScores(Platform::String^ Filename);
		void HandleGameOver();

		void ShowScores(int highLightIndex);
		void Save(Platform::String^ key, Platform::Object^ value);
		Platform::Object^ DirectXPage::Read(Platform::String^ key);
		bool DirectXPage::Exists(Platform::String^ key);
		void OnToggled(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^e);
		void OnValueChanged(Platform::Object ^sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs ^e);
		void LoadSettings();
		void WireUpUIEvents();


		void OnDismissPrivacyTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e);
		void OnDismissAboutTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e);
		void OnDismissAudioTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e);
		void OnContinueTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e);

		void LoadLevels(Platform::String^ levels_xml);

		void OnPauseTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e);
		void OnPointerPressed(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^e);
		void OnPointerMoved(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^e);
		void OnPointerReleased(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^e);
		void OnUnPauseTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e);
		void OnClosed(Platform::Object ^sender, Platform::Object ^args);
		void OnQuitTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e);
		void OnSettingsTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e);


		void UpdateScores();

		void OnShootPointerEntered(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^e);
		void OnShootPointerExited(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^e);

		void Pause();
		void Unpause();
		void OnVisibilityChanged(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::VisibilityChangedEventArgs ^args);
		void OnBackToMenuTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e);
		void OnSuspending(Platform::Object ^sender, Windows::ApplicationModel::SuspendingEventArgs ^e);
};
}
