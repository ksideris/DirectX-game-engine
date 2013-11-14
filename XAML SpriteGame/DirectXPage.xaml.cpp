//
// BlankPage.xaml.cpp
// Implementation of the BlankPage.xaml class.
//

#include "pch.h"
#include "DirectXPage.xaml.h"  
#include <map>

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

	// wire the event for screen size monitoring. Important for retargeting the renderer and pausing 
	Window::Current->CoreWindow->SizeChanged +=
		ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &DirectXPage::OnWindowSizeChanged);

	//notify renderer of changes in DPI
	DisplayProperties::LogicalDpiChanged +=
		ref new DisplayPropertiesEventHandler(this, &DirectXPage::OnLogicalDpiChanged);

	// wire the event for the share charm
	auto dataTransferManager = Windows::ApplicationModel::DataTransfer::DataTransferManager::GetForCurrentView();
	dataTransferManager->DataRequested+=ref new Windows::Foundation::TypedEventHandler<Windows::ApplicationModel::DataTransfer::DataTransferManager ^, Windows::ApplicationModel::DataTransfer::DataRequestedEventArgs ^>(this, &GameEngine::DirectXPage::OnDataRequested);
	
	
	// main rendering event handler
	m_eventToken = CompositionTarget::Rendering += ref new EventHandler<Object^>(this, &DirectXPage::OnRendering);

	//init timer
	m_timer = ref new Timer();

	
	UpdateWindowSize();
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

	float margin = (m_renderer->m_renderTargetSize.Width - 768.0f) / 2.0f;
	if (margin < 2.0)
		visibility = false;

	if (visibility)
	{

	}
	else
	{
	}
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

	if (framerate.size()>100)
	while (framerate.size() > 100)
		framerate.erase(framerate.begin());
	float fps = 0;

	for (auto _fps : framerate)
		fps += _fps; 

	fps /= framerate.size();
	framerateV->Text = fps.ToString();
	AudioManager::AudioEngineInstance.Render();
	m_renderer->Update(m_timer->Total, m_timer->Delta);
	m_renderer->Render();
	m_renderer->Present();


}

void DirectXPage::OnLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

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
	m_renderer->spaceship->ProcessKeyDown(e);


	if (e->Key == VirtualKey::A  )
		m_renderer->spaceShipLight->m_currentEffect = m_renderer->spaceShipLight->m_pointSpecularEffect;

	if (e->Key == VirtualKey::B )
		m_renderer->spaceShipLight->m_currentEffect = m_renderer->spaceShipLight->m_spotSpecularEffect;
}

void  DirectXPage::OnKeyUp(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e)
{

	m_renderer->spaceship->ProcessKeyUp(e);
	if (e->Key == VirtualKey::Space)
		m_renderer->CreateProjectile();
}

 
 
 
void  DirectXPage::XAMLPage_PointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	 
	m_renderer->spaceship->ProcessPointerPressed(e->GetCurrentPoint((DirectXPage^) sender));
}


void  DirectXPage::XAMLPage_PointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	if (e->Pointer->IsInContact)
	m_renderer->spaceship->ProcessPointerMoved(e->GetCurrentPoint((DirectXPage^) sender));
}


void  DirectXPage::XAMLPage_PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	m_renderer->spaceship->ProcessPointerReleased(e->GetCurrentPoint((DirectXPage^) sender));
}


void DirectXPage::OnDataRequested(Windows::ApplicationModel::DataTransfer::DataTransferManager ^sender, Windows::ApplicationModel::DataTransfer::DataRequestedEventArgs ^args)
{
	auto request = args->Request;
	request->Data->Properties->Title = "Share Score";
	request->Data->Properties->Description = "Tell your friends how much you scored in the game engine";
	request->Data->SetText("I just scored  ! ");
}
