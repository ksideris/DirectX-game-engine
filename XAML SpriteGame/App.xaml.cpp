//
// App.xaml.cpp
// Implementation of the App class.
//

#include "pch.h"
#include "DirectXPage.xaml.h"
#include "App.xaml.h"

using namespace GameEngine;
using namespace Helpers;

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

/// <summary>
/// Initializes the singleton application object.  This is the first line of authored code
/// executed, and as such is the logical equivalent of main() or WinMain().
/// </summary>
App::App()
{
	InitializeComponent();
	Suspending += ref new SuspendingEventHandler(this, &App::OnSuspending);
	Resuming+=ref new Windows::Foundation::EventHandler<Platform::Object ^>(this, &GameEngine::App::OnResuming);
}

/// <summary>
/// Invoked when the application is launched normally by the end user.  Other entry points
/// will be used when the application is launched to open a specific file, to display
/// search results, and so forth.
/// </summary>
/// <param name="args">Details about the launch request and process.</param>
void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ pArgs)
{
	if (pArgs->PreviousExecutionState == ApplicationExecutionState::Terminated)
	{
		//TODO: Load state from previously suspended application
	}

	// Place the frame in the current Window and ensure that it is active
	page =  ref new DirectXPage();
	Window::Current->Content = page;
	Window::Current->Activate();
}

/// <summary>
/// Invoked when the application is being suspended.
/// </summary>
/// <param name="sender">Details about the origin of the event.</param>
/// <param name="args">Details about the suspending event.</param>
void App::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ e)
{
	page->Suspend();
	(void) sender;	// Unused parameter
	(void) e;	// Unused parameter
	  
	//TODO: Save application state and stop any background activity
}

void GameEngine::App::OnResuming(Platform::Object ^sender, Platform::Object ^args)
{
	page->Resume();
}
