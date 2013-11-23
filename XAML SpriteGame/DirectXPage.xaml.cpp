//
// BlankPage.xaml.cpp
// Implementation of the BlankPage.xaml class.
//

#include "pch.h"
#include "DirectXPage.xaml.h"  
#include <map>


//related to xml
#include "IrrXML\irrXML.h"
using namespace irr;
using namespace io;
//end xml imports

using namespace GameEngine;
using namespace Helpers;

using namespace AudioEngine;

using namespace Windows::UI::ViewManagement;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Input;
using namespace Windows::UI::Core;
using namespace Windows::System;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::ApplicationSettings;
using namespace Windows::UI::Popups;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;


using namespace concurrency;

DirectXPage::DirectXPage()
{
	InitializeComponent();

	IsInitialDataLoaded = false;

	//init the directX renderer 
	m_renderer = ref new SpriteGame();

	m_renderer->Initialize(
		Window::Current->CoreWindow,
		this,
		DisplayProperties::LogicalDpi
		);
	
	m_renderer->LoadLevel("Menu.xml");

	MenuButtonsGrid->Visibility = Windows::UI::Xaml::Visibility::Visible;
	PlayButton->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &GameEngine::DirectXPage::OnTapped);
	Level1->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &GameEngine::DirectXPage::OnLevelTapped);
	Level2->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &GameEngine::DirectXPage::OnLevelTapped);

	ShareButton->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &GameEngine::DirectXPage::OnTappedShare);
	// wire the event for screen size monitoring. Important for retargeting the renderer and pausing 
	Window::Current->CoreWindow->SizeChanged +=
		ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &DirectXPage::OnWindowSizeChanged);

	//notify renderer of changes in DPI
	DisplayProperties::LogicalDpiChanged +=
		ref new DisplayPropertiesEventHandler(this, &DirectXPage::OnLogicalDpiChanged);

	// wire the event for the share charm
	auto dataTransferManager = Windows::ApplicationModel::DataTransfer::DataTransferManager::GetForCurrentView();
	dataTransferManager->DataRequested += ref new Windows::Foundation::TypedEventHandler<Windows::ApplicationModel::DataTransfer::DataTransferManager ^, Windows::ApplicationModel::DataTransfer::DataRequestedEventArgs ^>(this, &GameEngine::DirectXPage::OnDataRequested);

	SettingsPane::GetForCurrentView()->CommandsRequested += ref new Windows::Foundation::TypedEventHandler<Windows::UI::ApplicationSettings::SettingsPane ^, Windows::UI::ApplicationSettings::SettingsPaneCommandsRequestedEventArgs ^>(this, &GameEngine::DirectXPage::OnCommandsRequested);

	// main rendering event handler
	m_eventToken = CompositionTarget::Rendering += ref new EventHandler<Object^>(this, &DirectXPage::OnRendering);

	MenuButtonsGrid->SizeChanged += ref new Windows::UI::Xaml::SizeChangedEventHandler(this, &GameEngine::DirectXPage::OnSizeChanged);
	LevelButtonsGrid->SizeChanged += ref new Windows::UI::Xaml::SizeChangedEventHandler(this, &GameEngine::DirectXPage::OnSizeChanged);
	GamePlayGrid->SizeChanged += ref new Windows::UI::Xaml::SizeChangedEventHandler(this, &GameEngine::DirectXPage::OnSizeChanged);

	tglMusic->Toggled += ref new Windows::UI::Xaml::RoutedEventHandler(this, &GameEngine::DirectXPage::OnToggled);
	sldMusicVolume->ValueChanged += ref new Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, &GameEngine::DirectXPage::OnValueChanged);

	tglSFX->Toggled += ref new Windows::UI::Xaml::RoutedEventHandler(this, &GameEngine::DirectXPage::OnToggled);
	sldSFXVolume->ValueChanged += ref new Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, &GameEngine::DirectXPage::OnValueChanged);

	dismissAudioSettings->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &GameEngine::DirectXPage::OnDismissAudioTapped);
	dismissAbout->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &GameEngine::DirectXPage::OnDismissAboutTapped);
	dismissPrivacy->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &GameEngine::DirectXPage::OnDismissPrivacyTapped);

	CheckNCreateFile("highscore2s.dat");


	m_timer = ref new Timer();

	UpdateWindowSize();
}



void  DirectXPage::CheckNCreateFile(String^ Filename)
{


	create_task(ApplicationData::Current->LocalFolder->GetFileAsync(Filename)).then([this](task<StorageFile^> getFileTask)
	{
		try
		{
			StorageFile^ s = getFileTask.get();
			return true;
		}
		catch (Platform::Exception^)
		{
			return false;
		}
	}, task_continuation_context::use_arbitrary()).then([this, Filename](bool filefound)
	{
		if (!filefound)
			ApplicationData::Current->LocalFolder->CreateFileAsync(Filename);
	});


}
void DirectXPage::LoadText(String^ Filename)
{
	create_task(ApplicationData::Current->LocalFolder->GetFileAsync(Filename)).then([](StorageFile^  file)
	{
		return FileIO::ReadTextAsync(file);
	}, task_continuation_context::use_arbitrary()).then([this](String^ filecontents)
	{
		//test->Text = filecontents;
	});

}
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}
void DirectXPage::LoadHighScores(String^ Filename)
{
	create_task(ApplicationData::Current->LocalFolder->GetFileAsync(Filename)).then([this](StorageFile^  file)
	{
		return FileIO::ReadTextAsync(file);
	}, task_continuation_context::use_arbitrary()).then([this](String^ filecontents)
	{
		//test->Text = filecontents;

		std::wstring fooW(filecontents->Begin());
		std::string fooA(fooW.begin(), fooW.end());
		auto separated_string = split(fooA, ',');

		for (auto str_score = separated_string.begin(); str_score != separated_string.end(); str_score++)
		{
			scores.push_front(atoi((*str_score).c_str()));
		}

		ShowScores(-1);
	});


}

void DirectXPage::HandleGameOver()
{
	int i = 0;
	for (auto score = scores.begin(); score != scores.end(); score++)
	{
		if (m_renderer->score >= *score)
		{
			scores.insert(scores.begin() + i, m_renderer->score);

			break;
		}
		i++;
	}

	//std::sort(scores.begin(), scores.end(), std::greater<int>());

	if (scores.size() > 5)
		scores.erase(scores.end() - 1);



	String^ newSave = "";
	for (auto score = scores.begin(); score != scores.end(); score++)
	{
		newSave += (*score).ToString() + ",";
	}
	writeToText("highscore2s.dat", newSave);
	ShowScores(i);

	m_renderer->LoadLevel("Menu.xml");
	m_renderer->gamestate = GameState::Menu;

}
void DirectXPage::ShowScores(int highLightIndex)
{
	TextBlock^ t = ref new TextBlock();
	t->FontSize = 40;
	t->Text = "High Scores";
	HighScoresPanel->Children->Clear();

	t->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Center;
	HighScoresPanel->Children->Append(t);

	std::sort(scores.begin(), scores.end(), std::greater<int>());
	int i = 0;
	for (auto score = scores.begin(); score != scores.end(); score++)
	{
		TextBlock^ t = ref new TextBlock();
		t->TextAlignment = Windows::UI::Xaml::TextAlignment::Center;
		t->FontSize = 30;
		if (i == highLightIndex)
			t->Foreground = ref new SolidColorBrush(Windows::UI::Colors::Yellow);
		t->Text = (*score).ToString();
		HighScoresPanel->Children->Append(t);
		i++;
	}
	GamePlayGrid->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	MenuButtonsGrid->Visibility = Windows::UI::Xaml::Visibility::Visible;
	LevelButtonsGrid->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

}
void DirectXPage::writeToText(String^ Filename, String^ text)
{
	if (text != nullptr && !text->IsEmpty())
	{
		create_task(ApplicationData::Current->LocalFolder->GetFileAsync(Filename)).then([this, text](StorageFile^  file)
		{
			return FileIO::WriteTextAsync(file, text);
		}, task_continuation_context::use_arbitrary()).then([this]()
		{
			//test->Text = "The following text was written to '" + test;
		});
	}

}

void DirectXPage::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
	m_renderer->UpdateForWindowSizeChange();

	UpdateWindowSize();

}

void DirectXPage::UpdateWindowSize()
{

	bool visibility = true;

	if (ApplicationView::Value == ApplicationViewState::Snapped)
		visibility = false;


	if (m_renderer->m_renderTargetSize.Width < 600)
		visibility = false;
	if (visibility)
	{
		if (m_renderer->gamestate == GameState::Paused)
		{

			m_renderer->gamestate = previous_state;
			PausedScreen->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		}
	}
	else
	{
		previous_state = m_renderer->gamestate;
		m_renderer->gamestate = GameState::Paused;
		PausedScreen->Visibility = Windows::UI::Xaml::Visibility::Visible;

	}


}

void DirectXPage::OnLogicalDpiChanged(Platform::Object^ sender)
{
	m_renderer->SetDpi(DisplayProperties::LogicalDpi);
}

//handles render loop
void DirectXPage::OnRendering(Platform::Object^ sender, Platform::Object^ args)
{

	if (m_renderer->gamestate != GameState::Paused)
	{
		m_timer->Update();

		framerate.push_back(1.f / m_timer->Delta);

		if (framerate.size() > 100)
		while (framerate.size() > 100)
			framerate.erase(framerate.begin());
		float fps = 0;

		for (auto _fps : framerate)
			fps += _fps;

		fps /= framerate.size();
		framerateV->Text = m_timer->Total.ToString();
		Score->Text = m_renderer->score.ToString();

		if (m_renderer->spaceship->health <= 0 && m_renderer->gamestate == GameState::Playing)
		{
			HandleGameOver();
		}
		else
		if (m_renderer->spaceship->health != health)
			PlayerHitAnimation->Begin();

		HealthBar->Width = max(m_renderer->spaceship->health, 0) * 350 / 100.f;
		health = m_renderer->spaceship->health;

		AudioManager::AudioEngineInstance.Render();

		m_renderer->Update(m_timer->Total, m_timer->Delta);
		m_renderer->Render();
		m_renderer->Present();

	}
}

void DirectXPage::OnLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	IsInitialDataLoaded = true;
	LoadSettings();
	LoadText("highscore2s.dat");
	LoadHighScores("highscore2s.dat");
}

void DirectXPage::OnWindowActivated(Object^ sender, Windows::UI::Core::WindowActivatedEventArgs^ e)
{
	if (e->WindowActivationState == Windows::UI::Core::CoreWindowActivationState::Deactivated)
	{
	}
}

void DirectXPage::OnKeyDown(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e)
{
	if (m_renderer->gamestate == Playing)
	{
		m_renderer->spaceship->ProcessKeyDown(e);


		if (e->Key == VirtualKey::A)
			m_renderer->spaceShipLight->m_currentEffect = m_renderer->spaceShipLight->m_pointSpecularEffect;

		if (e->Key == VirtualKey::B)
			m_renderer->spaceShipLight->m_currentEffect = m_renderer->spaceShipLight->m_spotSpecularEffect;
	}
}

void  DirectXPage::OnKeyUp(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e)
{
	if (m_renderer->gamestate == Playing)
	{
		m_renderer->spaceship->ProcessKeyUp(e);
		if (e->Key == VirtualKey::Space)
			m_renderer->CreateProjectile();
	}
}




void  DirectXPage::XAMLPage_PointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	if (m_renderer->gamestate == Playing)
	{
		m_renderer->spaceship->ProcessPointerPressed(e->GetCurrentPoint((DirectXPage^) sender));
	}

}


void  DirectXPage::XAMLPage_PointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	if (m_renderer->gamestate == Playing)
	{
		if (e->Pointer->IsInContact)
			m_renderer->spaceship->ProcessPointerMoved(e->GetCurrentPoint((DirectXPage^) sender));
	}
}


void  DirectXPage::XAMLPage_PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	if (m_renderer->gamestate == Playing)
	{
		m_renderer->spaceship->ProcessPointerReleased(e->GetCurrentPoint((DirectXPage^) sender));
	}
}


void DirectXPage::OnDataRequested(Windows::ApplicationModel::DataTransfer::DataTransferManager ^sender, Windows::ApplicationModel::DataTransfer::DataRequestedEventArgs ^args)
{
	auto request = args->Request;
	request->Data->Properties->Title = "Share Score";
	request->Data->Properties->Description = "Tell your friends how much you scored in the game engine";
	request->Data->SetText("I just scored ! ");
}
void DirectXPage::OnSettingsSelected(Windows::UI::Popups::IUICommand^ command)
{
	if (command->Id->ToString() == "musicSfx")
	{
		stkMusicSfx->Width = 346.0f;
		grdSubMusicSfx->Height = m_renderer->m_renderTargetSize.Height;
		stkMusicSfx->IsOpen = true;
	}
	if (command->Id->ToString() == "about")
	{
		popupAbout->Width = 346.0f;
		grdAbout->Height = m_renderer->m_renderTargetSize.Height;
		popupAbout->IsOpen = true;
		
	}
	if (command->Id->ToString() == "privacy")
	{
		popupPrivacy->Width = 346.0f;
		grdPrivacy->Height = m_renderer->m_renderTargetSize.Height;
		popupPrivacy->IsOpen = true;
	}
	WindowActivationToken = Window::Current->Activated += ref new WindowActivatedEventHandler(this, &DirectXPage::OnWindowActivated);

}

void DirectXPage::OnCommandsRequested(Windows::UI::ApplicationSettings::SettingsPane ^sender, Windows::UI::ApplicationSettings::SettingsPaneCommandsRequestedEventArgs ^args)
{

	UICommandInvokedHandler^ handler = ref new UICommandInvokedHandler(this, &DirectXPage::OnSettingsSelected);

	SettingsCommand^ generalCommand = ref new SettingsCommand("musicSfx", "Music & SFX", handler);
	args->Request->ApplicationCommands->Append(generalCommand);
	SettingsCommand^ privacyCommand = ref new SettingsCommand("privacy", "Privacy", handler);
	args->Request->ApplicationCommands->Append(privacyCommand);
	SettingsCommand^ aboutCommand = ref new SettingsCommand("about", "About", handler);
	args->Request->ApplicationCommands->Append(aboutCommand);

}

void GameEngine::DirectXPage::OnDismissAboutTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e)
{
	

	popupAbout->IsOpen = false;
}


void GameEngine::DirectXPage::OnDismissAudioTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e)
{
	stkMusicSfx->IsOpen = false;

}


void GameEngine::DirectXPage::OnDismissPrivacyTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e)
{

	popupPrivacy->IsOpen = false;

}


void GameEngine::DirectXPage::OnTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e)
{
	MenuButtonsGrid->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	LevelButtonsGrid->Visibility = Windows::UI::Xaml::Visibility::Visible;

}

void GameEngine::DirectXPage::OnLevelTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e)
{
	LevelButtonsGrid->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	GamePlayGrid->Visibility = Windows::UI::Xaml::Visibility::Visible;


	m_renderer->LoadLevel(((Button^) sender)->Tag + ".xml");

	health = m_renderer->spaceship->health;
	m_timer->Reset();
	m_renderer->gamestate = Playing;

}

void GameEngine::DirectXPage::OnTappedShare(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e)
{
	Windows::ApplicationModel::DataTransfer::DataTransferManager::GetForCurrentView()->ShowShareUI();

}
void GameEngine::DirectXPage::OnSizeChanged(Platform::Object ^sender, Windows::UI::Xaml::SizeChangedEventArgs ^e)
{

}
void DirectXPage::Save(Platform::String^ key, Platform::Object^ value)
{
	ApplicationDataContainer^ localSettings = ApplicationData::Current->LocalSettings;
	auto values = localSettings->Values;
	values->Insert(key, value);
}

Platform::Object^ DirectXPage::Read(Platform::String^ key)
{
	ApplicationDataContainer^ localSettings = ApplicationData::Current->LocalSettings;
	auto values = localSettings->Values;
	return values->Lookup(key);
}

bool DirectXPage::Exists(Platform::String^ key)
{
	ApplicationDataContainer^ localSettings = ApplicationData::Current->LocalSettings;
	auto values = localSettings->Values;
	return values->HasKey(key);
}


void DirectXPage::LoadSettings()
{
	
	bool MusicEnabled = false;
	bool SFXEnabled = false;
	int volume = 0, sfxvolume = 0;

	if (Exists(L"MusicEnabled"))
	{
		MusicEnabled = (bool) Read(L"MusicEnabled");
	}

	if (MusicEnabled)
	{
		tglMusic->IsOn = true;

		if (Exists(L"MusicVolume"))
		{
			volume = (int) Read(L"MusicVolume");

			sldMusicVolume->Value = volume;
		}
		else
		{
			sldMusicVolume->Value = 100;
			volume = 100;
		}

	}
	else
	{
		tglMusic->IsOn = false;

		sldMusicVolume->Value = 0;
		volume = 0;
	}
	if (Exists(L"SFXEnabled"))
	{
		SFXEnabled = (bool) Read(L"SFXEnabled");
	}

	if (SFXEnabled)
	{
		tglSFX->IsOn = true;

		if (Exists(L"MusicVolume"))
		{
			sfxvolume = (int) Read(L"MusicVolume");

			sldSFXVolume->Value = sfxvolume;
		}
		else
		{
			sldSFXVolume->Value = 100;
			sfxvolume = 100;
		}

	}
	else
	{
		tglSFX->IsOn = false;

		sldSFXVolume->Value = 0;
		sfxvolume = 0;
	}

	AudioManager::Initialize();
	AudioManager::SetMainMenuMusic();

	AudioManager::IsSFXStarted = true;
	if (true)
	{
		AudioManager::AudioEngineInstance.StartSFX();
	}
	else
	{
		AudioManager::AudioEngineInstance.SuspendSFX();
	}

	AudioManager::IsMusicStarted = true;

	if (true)
	{
		AudioManager::AudioEngineInstance.StartMusic();
	}
	else
	{
		AudioManager::AudioEngineInstance.SuspendMusic();
	}

	AudioManager::SetMusicVolume(volume);
	AudioManager::SetSFXVolume(sfxvolume);
}

void GameEngine::DirectXPage::OnToggled(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^e)
{
	if (tglMusic->IsOn)
	{
		if (Exists(L"MusicVolume"))
		{
			int volume = (int) Read(L"MusicVolume");

			AudioEngine::AudioManager::SetMusicVolume(volume);
			sldMusicVolume->Value = volume;
		}
		else
		{
			AudioEngine::AudioManager::SetMusicVolume(100);
			sldMusicVolume->Value = 100;
		}
		Save(L"MusicEnabled", true);
	}
	else
	{
		sldMusicVolume->Value = 0;
		AudioEngine::AudioManager::SetMusicVolume(0);
		Save(L"MusicEnabled", false);
	}

	if (tglSFX->IsOn)
	{
		if (Exists(L"SFXEnabled"))
		{
			int volume = (int) Read(L"SFXVolume");

			AudioEngine::AudioManager::SetSFXVolume(volume);
			sldSFXVolume->Value = volume;
		}
		else
		{
			AudioEngine::AudioManager::SetSFXVolume(100);
			sldSFXVolume->Value = 100;
		}
		Save(L"SFXEnabled", true);
	}
	else
	{
		sldSFXVolume->Value = 0;
		AudioEngine::AudioManager::SetSFXVolume(0);
		Save(L"SFXEnabled", false);
	}
}


void GameEngine::DirectXPage::OnValueChanged(Platform::Object ^sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs ^e)
{

	int volume = (int) sldMusicVolume->Value;

	AudioEngine::AudioManager::SetMusicVolume(volume);
	Save(L"MusicVolume", volume);

	if (volume == 0)
		tglMusic->IsOn = false;
	else
		tglMusic->IsOn = true;


	int sfxvolume = (int) sldSFXVolume->Value;

	if (sfxvolume == 0)
		tglSFX->IsOn = false;
	else
		tglSFX->IsOn = true;

	AudioEngine::AudioManager::SetSFXVolume(volume);
	Save(L"SFXVolume", volume);



}
