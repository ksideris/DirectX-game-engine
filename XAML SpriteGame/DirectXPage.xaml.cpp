//
// BlankPage.xaml.cpp
// Implementation of the BlankPage.xaml class.
//

#include "pch.h"
#include "DirectXPage.xaml.h"  
#include <map>

using namespace Coding4Fun::FallFury; 
using namespace Coding4Fun::FallFury::Helpers; 

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


	m_renderer = ref new SpriteGame();

	m_renderer->Initialize(
		Window::Current->CoreWindow,
		this,
		DisplayProperties::LogicalDpi
		);

	Window::Current->CoreWindow->SizeChanged +=
		ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &DirectXPage::OnWindowSizeChanged);

	DisplayProperties::LogicalDpiChanged +=
		ref new DisplayPropertiesEventHandler(this, &DirectXPage::OnLogicalDpiChanged);

	m_eventToken = CompositionTarget::Rendering += ref new EventHandler<Object^>(this, &DirectXPage::OnRendering);

	m_timer = ref new Timer();


	UpdateWindowSize();

}


 

void DirectXPage::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
	m_renderer->UpdateForWindowSizeChange();
	UpdateWindowSize();
	 //m_renderer->CurrentGameScreen->UpdateWindowSize();
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

void DirectXPage::OnRendering(Platform::Object^ sender, Platform::Object^ args)
{
	m_timer->Update();



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
}




void  DirectXPage::OnKeyUp(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e)
{

	m_renderer->spaceship->ProcessKeyUp(e);
	if (e->Key == VirtualKey::Space)
		m_renderer->CreateProjectile();
}


void DirectXPage::CheckGameState()
{
	/*if (PreviousGameState != m_renderer->CurrentGameState)
	{
		PreviousGameState = m_renderer->CurrentGameState;
	}*/
}
  

void DirectXPage::XAMLPage_LayoutUpdated(Platform::Object^ sender, Platform::Object^ e)
{ 
}
  


