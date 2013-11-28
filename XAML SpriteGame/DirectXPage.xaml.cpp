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
	hudIsClicked = false;
	//init the directX renderer 
	m_renderer = ref new SpriteGame();

	m_renderer->Initialize(
		Window::Current->CoreWindow,
		this,
		DisplayProperties::LogicalDpi
		);
	Paused = false;

	MenuButtonsGrid->Visibility = Windows::UI::Xaml::Visibility::Visible;
	GamePlayGrid->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	LevelButtonsGrid->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

	// wire the event for screen size monitoring. Important for retargeting the renderer and pausing 
	Window::Current->CoreWindow->SizeChanged +=
		ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &DirectXPage::OnWindowSizeChanged);

	//notify renderer of changes in DPI
	DisplayProperties::LogicalDpiChanged +=
		ref new DisplayPropertiesEventHandler(this, &DirectXPage::OnLogicalDpiChanged);

	// main rendering event handler
	m_eventToken = CompositionTarget::Rendering += ref new EventHandler<Object^>(this, &DirectXPage::OnRendering);

	CheckNCreateFile(SAVEFILE);

	m_timer = ref new Timer();

	UpdateWindowSize();
	WireUpUIEvents();
	active_UI = MenuButtonsGrid;

	Windows::Devices::Input::TouchCapabilities^ t =ref new Windows::Devices::Input::TouchCapabilities();
	if (t->TouchPresent == 0)
	{
		ShootButton->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	} 
}

void DirectXPage::WireUpUIEvents()
{

	MenuButtonsGrid->SizeChanged += ref new Windows::UI::Xaml::SizeChangedEventHandler(this, &GameEngine::DirectXPage::OnSizeChanged);
	LevelButtonsGrid->SizeChanged += ref new Windows::UI::Xaml::SizeChangedEventHandler(this, &GameEngine::DirectXPage::OnSizeChanged);
	GamePlayGrid->SizeChanged += ref new Windows::UI::Xaml::SizeChangedEventHandler(this, &GameEngine::DirectXPage::OnSizeChanged);

	PlayButton->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &GameEngine::DirectXPage::OnTapped);
	ShareButton->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &GameEngine::DirectXPage::OnTappedShare);

	tglMusic->Toggled += ref new Windows::UI::Xaml::RoutedEventHandler(this, &GameEngine::DirectXPage::OnToggled);
	sldMusicVolume->ValueChanged += ref new Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, &GameEngine::DirectXPage::OnValueChanged);

	tglSFX->Toggled += ref new Windows::UI::Xaml::RoutedEventHandler(this, &GameEngine::DirectXPage::OnToggled);
	sldSFXVolume->ValueChanged += ref new Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, &GameEngine::DirectXPage::OnValueChanged);

	dismissAudioSettings->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &GameEngine::DirectXPage::OnDismissAudioTapped);
	dismissAbout->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &GameEngine::DirectXPage::OnDismissAboutTapped);
	dismissPrivacy->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &GameEngine::DirectXPage::OnDismissPrivacyTapped);

	PauseButton->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &GameEngine::DirectXPage::OnPauseTapped);
	dismissPaused->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &GameEngine::DirectXPage::OnUnPauseTapped);
	popupPaused->Closed += ref new Windows::Foundation::EventHandler<Platform::Object ^>(this, &GameEngine::DirectXPage::OnClosed);
	UnpauseButton->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &GameEngine::DirectXPage::OnUnPauseTapped);
	SettingsBut->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &GameEngine::DirectXPage::OnSettingsTapped);
	Quit->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &GameEngine::DirectXPage::OnQuitTapped);

	ShootButton->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &GameEngine::DirectXPage::OnShootTapped);
	ShootButton->PointerEntered += ref new Windows::UI::Xaml::Input::PointerEventHandler(this, &GameEngine::DirectXPage::OnShootPointerEntered);
	ShootButton->PointerExited += ref new Windows::UI::Xaml::Input::PointerEventHandler(this, &GameEngine::DirectXPage::OnShootPointerExited);
	//Charm bar events
	auto dataTransferManager = Windows::ApplicationModel::DataTransfer::DataTransferManager::GetForCurrentView();
	dataTransferManager->DataRequested += ref new Windows::Foundation::TypedEventHandler<Windows::ApplicationModel::DataTransfer::DataTransferManager ^, Windows::ApplicationModel::DataTransfer::DataRequestedEventArgs ^>(this, &GameEngine::DirectXPage::OnDataRequested);

	SettingsPane::GetForCurrentView()->CommandsRequested += ref new Windows::Foundation::TypedEventHandler<Windows::UI::ApplicationSettings::SettingsPane ^, Windows::UI::ApplicationSettings::SettingsPaneCommandsRequestedEventArgs ^>(this, &GameEngine::DirectXPage::OnCommandsRequested);

}


void  DirectXPage::CheckNCreateFile(String^ Filename)
{

	if (!Exists(L"ApplicationInit"))
	{
		ApplicationData::Current->LocalFolder->CreateFileAsync(Filename);
		writeToText(Filename, "0,0,0,0,0");

		Save(L"ApplicationInit", true);
	}


}
//deprecated will be removed
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

	if (m_renderer->gamestate == GameState::GameOver)
	{
		AudioManager::SetGameOverMusic();
		m_renderer->gamestate = GameState::LevelLost;

	}
	else if (m_renderer->gamestate == GameState::LevelComplete)
	{
		UpdateScores();
		m_renderer->gamestate = GameState::LevelWon;
	}

	GamePlayGrid->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	MenuButtonsGrid->Visibility = Windows::UI::Xaml::Visibility::Visible;


}
void DirectXPage::UpdateScores()
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

	if (scores.size() > 5)
		scores.erase(scores.end() - 1);

	String^ newSave = "";
	for (auto score = scores.begin(); score != scores.end(); score++)
	{
		newSave += (*score).ToString() + ",";
	}
	writeToText(SAVEFILE, newSave);
	ShowScores(i);
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
	//active_UI = MenuButtonsGrid;

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
			Unpause();
			active_UI->Visibility		= Windows::UI::Xaml::Visibility::Visible;
			PausedScreen->Visibility	= Windows::UI::Xaml::Visibility::Collapsed;
		}
	}
	else
	{
		active_UI->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		PausedScreen->Visibility = Windows::UI::Xaml::Visibility::Visible;
		Pause(); 		 
	}


}

void DirectXPage::OnLogicalDpiChanged(Platform::Object^ sender)
{
	m_renderer->SetDpi(DisplayProperties::LogicalDpi);
}
//handles render loop
void DirectXPage::OnRendering(Platform::Object^ sender, Platform::Object^ args)
{

	if (!Paused)
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
		Clock->Text = (((int) m_timer->Total) / 60).ToString();
		Clock->Text += ":" + (((int) m_timer->Total) % 60).ToString();
		Score->Text = m_renderer->score.ToString();

		if (m_renderer->gamestate == GameState::GameOver || m_renderer->gamestate == GameState::LevelComplete)
		{
			HandleGameOver();
		}


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

	m_renderer->LoadLevel("Menu.xml");
	LoadSettings();
	LoadHighScores(SAVEFILE);
	LoadLevels(L"Level Data\\Levels.xml");
	IsInitialDataLoaded = true;
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

		//if (e->Key == VirtualKey::A)
		//	m_renderer->spaceShipLight->m_currentEffect = m_renderer->spaceShipLight->m_pointSpecularEffect;

		//if (e->Key == VirtualKey::B)
		//	m_renderer->spaceShipLight->m_currentEffect = m_renderer->spaceShipLight->m_spotSpecularEffect;
	}
}

void  DirectXPage::OnKeyUp(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e)
{
	if (m_renderer->gamestate == Playing)
	{
		m_renderer->spaceship->ProcessKeyUp(e);
	}
}



void DirectXPage::OnDataRequested(Windows::ApplicationModel::DataTransfer::DataTransferManager ^sender, Windows::ApplicationModel::DataTransfer::DataRequestedEventArgs ^args)
{
	auto request = args->Request;
	request->Data->Properties->Title = "Share your Score";
	request->Data->Properties->Description = "Tell your friends how much you scored in the game engine";
	request->Data->SetText("I just scored ! " + scores[0]);
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
	active_UI = LevelButtonsGrid;
}

void GameEngine::DirectXPage::OnLevelTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e)
{
	LevelButtonsGrid->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	GamePlayGrid->Visibility = Windows::UI::Xaml::Visibility::Visible;


	active_UI = GamePlayGrid;

	m_renderer->LoadLevel(((Button^) sender)->Tag + ".xml");

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
	else
	{
		MusicEnabled = true;
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
			Save(L"MusicVolume", volume);

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
	else
	{
		SFXEnabled = true;
	}

	if (SFXEnabled)
	{
		tglSFX->IsOn = true;

		if (Exists(L"SFXVolume"))
		{
			sfxvolume = (int) Read(L"SFXVolume");

			sldSFXVolume->Value = sfxvolume;
		}
		else
		{
			sldSFXVolume->Value = 60;
			sfxvolume = 60;

			Save(L"SFXVolume", sfxvolume);

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
	if (IsInitialDataLoaded)
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
				Save(L"MusicVolume", 100);
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
				sldSFXVolume->Value = 60;
				Save(L"SFXVolume", 60);
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
}


void GameEngine::DirectXPage::OnValueChanged(Platform::Object ^sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs ^e)
{
	if (IsInitialDataLoaded)
	{
		int volume = (int) sldMusicVolume->Value;

		AudioEngine::AudioManager::SetMusicVolume(volume);
		Save(L"MusicVolume", volume);

		if (volume == 0)
			tglMusic->IsOn = false;
		else
			tglMusic->IsOn = true;


		int sfxvolume = (int) sldSFXVolume->Value;

		if (sfxvolume == 0){
			Save(L"SFXEnabled", false);
			tglSFX->IsOn = false;
		}
		else{
			Save(L"SFXEnabled", true);
			tglSFX->IsOn = true;
		}

		AudioEngine::AudioManager::SetSFXVolume(sfxvolume);
		Save(L"SFXVolume", sfxvolume);


	}
}

void DirectXPage::LoadLevels(String^ levels_xml)
{
	std::wstring level_xmlW(levels_xml->Begin());
	std::string level_xmlA(level_xmlW.begin(), level_xmlW.end());


	IrrXMLReader* xml = createIrrXMLReader(level_xmlA.c_str());

	// strings for storing the data we want to get out of the file
	std::string stringdata2;
	std::string stringdata;
	std::wstring wstringdata;
	Platform::String^ final_data;


	// parse the file until end reached
	while (xml && xml->read())
	{
		switch (xml->getNodeType())
		{

		case EXN_ELEMENT:
			{
				if (!strcmp("Level", xml->getNodeName()))
				{
					Button^ b = ref new Button();

					stringdata = xml->getAttributeValue("Name");
					std::wstring wsTmp(stringdata.begin(), stringdata.end());

					b->Content = ref new String(wsTmp.c_str());

					stringdata = xml->getAttributeValue("Data");
					std::wstring wsTmp2(stringdata.begin(), stringdata.end());

					b->Tag = ref new String(wsTmp2.c_str());
					b->Background = ref new SolidColorBrush(Windows::UI::Colors::Gray);
					b->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Left;
					b->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Top;
					b->Width = 400;
					b->Height = 60;
					b->Margin = Windows::UI::Xaml::Thickness(0, 10, 0, 0);
					b->FontSize = 23;
					b->Tapped += ref new Windows::UI::Xaml::Input::TappedEventHandler(this, &GameEngine::DirectXPage::OnLevelTapped);
					LevelsPanel->Children->Append(b);
				}
				break;
						}
		}
	}

	// delete the xml parser after usage
	delete xml;

	///end xml testing
}


void GameEngine::DirectXPage::OnPauseTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e)
{
	hudIsClicked = true;

	Pause();
}


void GameEngine::DirectXPage::OnPointerPressed(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^e)
{
	if (m_renderer->gamestate == Playing && !hudIsClicked)
	{
		if (e->Pointer->IsInContact && e->GetCurrentPoint(GamePlayGrid)->Position.Y > HUD->Height)
		{
			m_renderer->spaceship->ProcessPointerPressed(e->GetCurrentPoint((DirectXPage^) sender));
		}

	}

}


void GameEngine::DirectXPage::OnPointerMoved(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^e)
{
	if (m_renderer->gamestate == Playing && !hudIsClicked)
	{

		if (e->Pointer->IsInContact && e->GetCurrentPoint(GamePlayGrid)->Position.Y > HUD->Height)
		{

			m_renderer->spaceship->ProcessPointerMoved(e->GetCurrentPoint((DirectXPage^) sender));
		}

	}
}


void GameEngine::DirectXPage::OnPointerReleased(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^e)
{
	if (m_renderer->gamestate == Playing && !hudIsClicked)
	{
		if (e->Pointer->IsInContact  && e->GetCurrentPoint(GamePlayGrid)->Position.Y > HUD->Height)
		{

			m_renderer->spaceship->ProcessPointerReleased(e->GetCurrentPoint((DirectXPage^) sender));
		}
	}
	hudIsClicked = false;
}


void GameEngine::DirectXPage::OnUnPauseTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e)
{
	Unpause();  
}
void GameEngine::DirectXPage::OnClosed(Platform::Object ^sender, Platform::Object ^args)
{
}
void GameEngine::DirectXPage::OnQuitTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e)
{ 
}
void GameEngine::DirectXPage::OnSettingsTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e)
{
	SettingsPane::GetForCurrentView()->Show();
}
void GameEngine::DirectXPage::OnShootTapped(Platform::Object ^sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs ^e)
{
	m_renderer->spaceship->Shoot();
}
void GameEngine::DirectXPage::OnShootPointerEntered(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^e)
{
	hudIsClicked = true;
}
void GameEngine::DirectXPage::OnShootPointerExited(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^e)
{
	hudIsClicked = false;
}
void  GameEngine::DirectXPage::Pause()
{
	if (!Paused)
	{
		if (PausedScreen->Visibility == Windows::UI::Xaml::Visibility::Collapsed)
		{
			popupPaused->IsOpen = true;
			popupPaused->Width = 346.0f;
			grdPaused->Height = m_renderer->m_renderTargetSize.Height;
		}
		time_at_pause = m_timer->CurrentTime;
		previous_state = m_renderer->gamestate;
		m_renderer->gamestate = GameState::Paused; 
		Paused = true;
	}
	if (PausedScreen->Visibility == Windows::UI::Xaml::Visibility::Visible)
	{
		popupPaused->IsOpen = false; 
	}
}
void  GameEngine::DirectXPage::Unpause()
{
	if (Paused)
	{
		popupPaused->IsOpen = false;
		Paused = false;
		m_renderer->gamestate = previous_state;
		m_timer->Reset(time_at_pause);
	}
}