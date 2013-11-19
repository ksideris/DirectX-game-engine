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
	health = 100;


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

	CheckNCreateFile("highscore2s.dat");
	//init timer
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
	create_task(ApplicationData::Current->LocalFolder->GetFileAsync(Filename)).then([](StorageFile^  file)
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
			scores.push_back(atoi( (*str_score).c_str() ));
		}

		TextBlock^ t = ref new TextBlock();
		t->FontSize = 40;
		t->Text = "High Scores"; 
		 
		t->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Center;
		HighScoresPanel->Children->Append(t);
		std::sort(scores.begin(), scores.end(), std::greater<int>());
		for (auto score = scores.begin(); score != scores.end(); score++)
		{
			TextBlock^ t = ref new TextBlock();
			t->HorizontalAlignment  = Windows::UI::Xaml::HorizontalAlignment::Left;
			t->FontSize = 60;
			t->Text = (*score).ToString();
			HighScoresPanel->Children->Append(t);
		}
	});


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




#if defined WINDOWS_8_0

	bool visibility = true;
	if (ApplicationView::Value == ApplicationViewState::Snapped)
		visibility = false;

	float margin = (m_renderer->m_renderTargetSize.Width - 768.0f) / 2.0f;
	if (margin < 2.0)
		visibility = false;

	if (visibility)
	{

	}
	else
	{
	}
#else

	bool visibility = true;

	if (ApplicationView::Value == ApplicationViewState::Snapped)
		visibility = false;

	float margin = (m_renderer->m_renderTargetSize.Width - 768.0f) / 2.0f;
	if (margin < 2.0)
		visibility = false;

	if (visibility)
	{
		m_renderer->gamestate = GameState::Menu;
	}
	else
	{
	}
#endif
}

void DirectXPage::OnLogicalDpiChanged(Platform::Object^ sender)
{
	m_renderer->SetDpi(DisplayProperties::LogicalDpi);
}

//handles render loop
void DirectXPage::OnRendering(Platform::Object^ sender, Platform::Object^ args)
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


	if (m_renderer->spaceship->health != health)
		PlayerHitAnimation->Begin();

	HealthBar->Width = max(m_renderer->spaceship->health, 0) * 350 / 100.f;
	health = m_renderer->spaceship->health;

	AudioManager::AudioEngineInstance.Render();

	m_renderer->Update(m_timer->Total, m_timer->Delta);
	m_renderer->Render();
	m_renderer->Present();


}

void DirectXPage::OnLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	IsInitialDataLoaded = true;
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

	String^ newSave = "";
	for (auto score = scores.begin(); score != scores.end();score++)
	{
		newSave += (*score).ToString() + ",";
	}
	writeToText("highscore2s.dat", newSave);


	auto request = args->Request;
	request->Data->Properties->Title = "Share Score";
	request->Data->Properties->Description = "Tell your friends how much you scored in the game engine";
	request->Data->SetText("I just scored ! ");
}
void DirectXPage::OnSettingsSelected(Windows::UI::Popups::IUICommand^ command)
{
	if (command->Id->ToString()		 ==	"musicSfx"		)
	{
		stkMusicSfx->Width = 346.0f;
		grdSubMusicSfx->Height = m_renderer->m_renderTargetSize.Height;
		stkMusicSfx->IsOpen = true;
	}
	else if (command->Id->ToString() == "accelerometer"	)
	{
		stkAccelerometerSettings->Width = 346.0f;
		grdAccelerometerSettings->Height = m_renderer->m_renderTargetSize.Height;
		stkAccelerometerSettings->IsOpen = true;
	}
	else if (command->Id->ToString() == "charMovement"	)
	{
		stkCharacterMovement->Width = 346.0f;
		grdCharacterMovement->Height = m_renderer->m_renderTargetSize.Height;
		stkCharacterMovement->IsOpen = true;
	}
}

void DirectXPage::OnCommandsRequested(Windows::UI::ApplicationSettings::SettingsPane ^sender, Windows::UI::ApplicationSettings::SettingsPaneCommandsRequestedEventArgs ^args)
{

	UICommandInvokedHandler^ handler = ref new UICommandInvokedHandler(this, &DirectXPage::OnSettingsSelected);

	SettingsCommand^ generalCommand = ref new SettingsCommand("musicSfx", "Music & SFX", handler);
	args->Request->ApplicationCommands->Append(generalCommand);

	SettingsCommand^ accelerometerCommand = ref new SettingsCommand("accelerometer", "Accelerometer", handler);
	args->Request->ApplicationCommands->Append(accelerometerCommand);

	SettingsCommand^ charMovementCommand = ref new SettingsCommand("charMovement", "Character Movement", handler);
	args->Request->ApplicationCommands->Append(charMovementCommand);
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


	m_renderer->LoadLevel(   ((Button^)sender)->Tag +".xml");
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
