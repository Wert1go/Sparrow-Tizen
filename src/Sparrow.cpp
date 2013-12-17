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
#include "UiUpdateConstants.h"
#include "MDatabaseManager.h"
#include "AuthManager.h"
#include "SceneRegister.h"

#include "UiChatForm.h"

#include <FBase.h>
#include <FShell.h>

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::System;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Shell;
using namespace Tizen::Ui::Scenes;

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
	AppControlProviderManager::GetInstance()->SetAppControlProviderEventListener(this);
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
	AppLog("OnAppInitialized");
	return true;
}

bool
SparrowApp::OnAppWillTerminate(void)
{
	// TODO:
	// Deallocate or release resources that are UI dependent.
	AppLog("OnAppWillTerminate");
	return true;
}

bool
SparrowApp::OnAppTerminating(AppRegistry& appRegistry, bool forcedTermination)
{
	// TODO:
	// Deallocate resources allocated by this App for termination.
	// The App's permanent data and context can be saved via appRegistry.
	AppLog("OnAppTerminating");
	return true;
}

void
SparrowApp::OnForeground(void)
{
	// TODO:
	// Start or resume drawing when the application is moved to the foreground.
	AppLog("OnForeground");

	if (LongPollConnection::getInstance().PendingRestart()) {
		LongPollConnection::getInstance().Reconnect();
	}

	try {
		AuthManager::getInstance().SetUserOnline();

	} catch(...) {

	}


}

void
SparrowApp::OnBackground(void)
{
	// TODO:
	// Stop drawing when the application is moved to the background.
	AppLog("OnBackground");
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
	AppLog("OnScreenOn");
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
	AppLog("OnScreenOff");
}

//******************************* DISPATCHER ***************************/

void
SparrowApp::OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs) {
	if (requestId == LONGPOLL_GET_SERVER) {
		AppLog("TTTTEES");
		LongPollConnection::getInstance().SendRequestToLongPollServer(pArgs);
	} else if (requestId == LONGPOLL_CONNECTION) {
		LongPollConnection::getInstance().Reconnect();
	} else if (requestId == UPDATE_MESSAGE_ARRIVED){
		//PostNotification(new String(L"Пис"));
	} else if (requestId == LOGOUT) {
		this->Logout();
	} else if (requestId == 666) {
		AuthManager::getInstance().UpdateImage();
	} else if (requestId == LONGPOLL_RECONNECTION) {
		LongPollConnection::getInstance().RunTimer();
	} else if (requestId == LONGPOLL_RE_REQUEST_DATA) {
		LongPollConnection::getInstance().GetLongPollServerData();
	}

	delete pArgs;
}

void
SparrowApp::Logout() {
	try {
		AuthManager::getInstance().SetUserOffline();
		LongPollConnection::getInstance().ClearMessages();
	} catch(...) {

	}

	LongPollConnection::getInstance().Stop();
	AuthManager::getInstance().Clear();
	MDatabaseManager::getInstance().Clear();

	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);
	pSceneManager->GoForward(ForwardSceneTransition(SCENE_AUTH));
}

void
SparrowApp::PostNotification(String *text) {
	 int badgeNumber;
	 Tizen::Shell::NotificationManager notiMgr;
	 notiMgr.Construct();
	 badgeNumber = notiMgr.GetBadgeNumber();
	 badgeNumber++;
	 notiMgr.Notify(L"tetrtet erw re ");
}

void
SparrowApp::OnAppControlRequestReceived(
		RequestId reqId,
		const Tizen::Base::String& operationId,
		const Tizen::Base::String* pUriData,
		const Tizen::Base::String* pMimeType,
		const Tizen::Base::Collection::IMap* pExtraData) {

	AppLog("OnAppControlRequestReceived");

	if (!AuthManager::getInstance().IsAuthorized()) {
		return;
	}

	if (pExtraData) {
		AppLog("!OnAppControlRequestReceived");
		String notificationAppMessage(L"http://tizen.org/appcontrol/data/notification");
		const String* pMessage = dynamic_cast<const String*>(pExtraData->GetValue(notificationAppMessage));
		if (pMessage) {
			Tizen::Ui::Controls::Frame* pFrame = Tizen::App::UiApp::GetInstance()->GetAppFrame()->GetFrame();
			UiChatForm* pChatForm = static_cast< UiChatForm* >(pFrame->GetControl("UiChatForm", true));
			AppLog("!!OnAppControlRequestReceived");
			int uid;
			Integer::Parse(pMessage->GetPointer(), uid);
			AppLog(":::::: %d || %S", uid, pMessage->GetPointer());
			if (!pChatForm || (pChatForm && pChatForm->__userId != uid)) {
				AppLog("!!!OnAppControlRequestReceived");
				SceneManager* pSceneManager = SceneManager::GetInstance();
				AppAssert(pSceneManager);

				ArrayList *paramsList = new (std::nothrow) ArrayList();
				paramsList->Construct();

				paramsList->Add(new Integer(uid));
				pSceneManager->GoForward(ForwardSceneTransition(SCENE_CHAT, SCENE_TRANSITION_ANIMATION_TYPE_LEFT), paramsList);
			}

		}
	}

}

void
SparrowApp::OnSettingChanged(Tizen::Base::String& key) {
	Tizen::Ui::Controls::Frame* pFrame = Tizen::App::UiApp::GetInstance()->GetAppFrame()->GetFrame();
	pFrame->Invalidate(true);
}
