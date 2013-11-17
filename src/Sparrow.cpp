/**
 * Name        : Sparrow
 * Version     : 
 * Vendor      : 
 * Description : 
 */


#include "Sparrow.h"
#include "SparrowFrame.h"

#include "RestRequestOperation.h"
#include "LongPollConnection.h"

#include <FBase.h>

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::System;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;

SparrowApp::SparrowApp(void)
{
}

SparrowApp::~SparrowApp(void)
{
}

UiApp*
SparrowApp::CreateInstance(void)
{
	// Create the instance through the constructor.
	return new SparrowApp();
}

bool
SparrowApp::OnAppInitializing(AppRegistry& appRegistry)
{
	// TODO:
	// Initialize App specific data.
	// The App's permanent data and context can be obtained from the appRegistry.
	//
	// If this method is successful, return true; otherwise, return false.
	// If this method returns false, the App will be terminated.

	// Uncomment the following statement to listen to the screen on/off events.
	//PowerManager::SetScreenEventListener(*this);

	// Create a Frame
	SparrowFrame* pSparrowFrame = new SparrowFrame;
	pSparrowFrame->Construct();
	pSparrowFrame->SetName(L"Sparrow");
	AddFrame(*pSparrowFrame);

	// TODO: Add your initialization code here
	return true;
}

bool
SparrowApp::OnAppInitialized(void)
{
	// TODO:
	// Comment.
	return true;
}

bool
SparrowApp::OnAppWillTerminate(void)
{
	// TODO:
	// Deallocate or release resources that are UI dependent.
	return true;
}

bool
SparrowApp::OnAppTerminating(AppRegistry& appRegistry, bool forcedTermination)
{
	// TODO:
	// Deallocate resources allocated by this App for termination.
	// The App's permanent data and context can be saved via appRegistry.
	return true;
}

void
SparrowApp::OnForeground(void)
{
	// TODO:
	// Start or resume drawing when the application is moved to the foreground.
}

void
SparrowApp::OnBackground(void)
{
	// TODO:
	// Stop drawing when the application is moved to the background.
}

void
SparrowApp::OnLowMemory(void)
{
	// TODO:
	// Free unused resources or close the application.
}

void
SparrowApp::OnBatteryLevelChanged(BatteryLevel batteryLevel)
{
	// TODO:
	// Handle any changes in battery level here.
	// Stop using multimedia features(camera, mp3 etc.) if the battery level is CRITICAL.
}

void
SparrowApp::OnScreenOn(void)
{
	// TODO:
	// Get the released resources or resume the operations that were paused or stopped in OnScreenOff().
}

void
SparrowApp::OnScreenOff(void)
{
	// TODO:
	// Unless there is a strong reason to do otherwise, release resources (such as 3D, media, and sensors) to allow the device
	// to enter the sleep mode to save the battery.
	// Invoking a lengthy asynchronous method within this listener method can be risky, because it is not guaranteed to invoke a
	// callback before the device enters the sleep mode.
	// Similarly, do not perform lengthy operations in this listener method. Any operation must be a quick one.
}

//******************************* DISPATCHER ***************************/

void
SparrowApp::OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs) {
	if (requestId == LONGPOLL_GET_SERVER) {
		LongPollConnection::getInstance().SendRequestToLongPollServer(pArgs);
	} else if (requestId == LONGPOLL_CONNECTION) {
		LongPollConnection::getInstance().GetLongPollServerData();
	}

	delete pArgs;
}
