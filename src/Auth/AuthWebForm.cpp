/*
 * AuthWebForm.cpp
 *
 *  Created on: Nov 3, 2013
 *      Author: developer
 */

#include "AuthWebForm.h"
#include "AuthManager.h"
#include "SceneRegister.h"
#include "LongPollConnection.h"

using namespace Tizen::App;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Web;
using namespace Tizen::Web::Controls;
using namespace Tizen::Io;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Base::Utility;
using namespace Tizen::Graphics;
using namespace Tizen::Locales;
using namespace Tizen::System;


#define VK_MAIN		L"https://m.vk.com/join"
#define VK_AUTH_URL L"https://oauth.vk.com/authorize"
#define VK_CALLBACK L"https://oauth.vk.com/blank.html"

#define VK_LOGIN_AFTER_REG L"act=login"

#define VK_APP_ID	L"3995016"
#define VK_SCOPE	L"messages,friends,photos,audio,video,notifications,files"

AuthWebForm::AuthWebForm() {
	result r = E_SUCCESS;
	__watingForReqistration = false;
	r = Form::Construct(FORM_STYLE_NORMAL);

	SetFormBackEventListener(this);

	WebSetting setting;
	Rectangle rect = GetBounds();

	__pWeb = new (std::nothrow) Web();
	r = __pWeb->Construct(rect);

	r = AddControl(__pWeb);

	setting.SetInputStyle(INPUT_STYLE_OVERLAY);
	r = __pWeb->SetSetting(setting);

	__pWeb->SetLoadingListener(this);
//	__pWeb->SetWebKeypadEventListener(this);
	__pWeb->SetWebUiEventListener(this);

	__pWeb->SetFocus();
}

AuthWebForm::~AuthWebForm() {

}

Tizen::Base::String*
AuthWebForm::RequestAuthUrl() {
	String query = L"";
	query.Append(L"client_id=");
	query.Append(VK_APP_ID);
	query.Append(L"&scope=");
	query.Append(VK_SCOPE);
	query.Append(L"&redirect_uri=");
	query.Append(VK_CALLBACK);
	query.Append(L"&display=mobile&v=5.2&response_type=token");

	if (AuthManager::getInstance().IsForced()) {
		query.Append(L"&revoke=1");
	}

	Uri uri;
	uri.SetUri(VK_AUTH_URL);
	uri.SetQuery(query);

	String *resultUrl = new (std::nothrow) String(uri.ToString());

	return resultUrl;
}

void
AuthWebForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
								   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs) {

	if (pArgs->GetCount() > 0) {

		String *param = static_cast< String* > (pArgs->GetAt(0));

		if (param != null && param->Equals(L"AUTH", true)) {
			__pWeb->LoadUrl(RequestAuthUrl()->GetPointer());
		} else {
			__pWeb->LoadUrl(VK_MAIN);
		}

		AppLog("%S", param->GetPointer());
	}

}

void
AuthWebForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
									const Tizen::Ui::Scenes::SceneId& nextSceneId) {

}

void
AuthWebForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);
	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));

}

bool
AuthWebForm::ExtractAccessDataAndSave(const Tizen::Base::String& url) {
	String access_token(L"");
	String expires_in(L"");
	String user_id(L"");

	//https://oauth.vk.com/blank.html#access_token=fda4eb8b27d2e796a15239907974c1499c9dace3b3f58f440fc4b0fc50a96c462884e3d281be031e66cd0&expires_in=86400&user_id=30143161

	String redirectedUrl = url;

	String urlDelim(L"#");

	StringTokenizer stringTokenizer(redirectedUrl, urlDelim);

	int count = stringTokenizer.GetTokenCount(); // count == 1

	AppLogDebug("count: %d", count);

	String paramsString;
	while (stringTokenizer.HasMoreTokens())
	{
		stringTokenizer.GetNextToken(paramsString);
	}

	String ampDelimer (L"&");
	String paramDelimer(L"=");

	StringTokenizer pairsStringTokenizer(paramsString, ampDelimer);

	String keyValuePair;

	while (pairsStringTokenizer.HasMoreTokens())
	{

		pairsStringTokenizer.GetNextToken(keyValuePair);

		StringTokenizer paramStringTokenizer(keyValuePair, paramDelimer);
		String paramValue = null;
		String prevValue = null;

		AppLogDebug("param count: %d", paramStringTokenizer.GetTokenCount());

		while(paramStringTokenizer.HasMoreTokens()) {
			paramStringTokenizer.GetNextToken(paramValue);

			if (prevValue != null) {
				if(prevValue.Equals(L"access_token", false)) {
					access_token = paramValue;
				} else if (prevValue.Equals(L"user_id", false)) {
					user_id = paramValue;
				} else if (prevValue.Equals(L"expires_in", false)) {
					expires_in = paramValue;
				}
			}
			prevValue = paramValue;
		}

	}

	if (access_token != null && user_id != null && expires_in != null) {

		AuthManager::getInstance().SaveAccessData(access_token, user_id, expires_in);

		AppLog("access_token %S", access_token.GetPointer());
		AppLog("user_id %S", user_id.GetPointer());
		AppLog("expires_in %S", expires_in.GetPointer());
	}

	return AuthManager::getInstance().IsAuthorized();
}

bool
AuthWebForm::OnHttpAuthenticationRequestedN(const Tizen::Base::String& host, const Tizen::Base::String& realm, const Tizen::Web::Controls::AuthenticationChallenge& authentication)
{
	return false;
}

void
AuthWebForm::OnHttpAuthenticationCanceled(void)
{
}

void
AuthWebForm::OnLoadingStarted(void)
{

}

void
AuthWebForm::OnLoadingCanceled(void)
{

}

void
AuthWebForm::OnLoadingErrorOccurred(LoadingErrorType error, const String& reason)
{
	Tizen::Ui::Controls::MessageBox msgBox;
	int modalresult = 0;
	Tizen::Base::String errReason;

	switch (error)
	{
	case WEB_REQUEST_TIMEOUT:
	{
		errReason = Tizen::Base::String(L"Request timeout");
	}
	break;

	case WEB_NO_CONNECTION:
	{
		errReason = Tizen::Base::String(L"Network is not in service");
	}
	break;

	case WEB_MIME_NOT_SUPPORTED:
	{
		errReason = Tizen::Base::String(L"The content type is not supported");
	}
	break;

	case WEB_BAD_URL:
	{
		errReason = Tizen::Base::String(L"The url is invalid");
	}
	break;

	case WEB_HTTP_RESPONSE:
	{
		errReason = Tizen::Base::String(L"HTTP response");
	}
	break;

	case WEB_OUT_OF_MEMORY:
	{
		errReason = Tizen::Base::String(L"Page Too Large");
	}
	break;

	case WEB_REQUEST_MAX_EXCEEDED:
	{
		errReason = Tizen::Base::String(L"Request Max Exceeded");
	}
	break;

	case WEB_ERROR_UNKNOWN:
	default:
	{
		errReason = Tizen::Base::String(L"An Unknown error");
	}
	break;
	}

	msgBox.Construct(L"LOADING ERROR TYPE", errReason, MSGBOX_STYLE_NONE, 3000);
	msgBox.ShowAndWait(modalresult);
}

void
AuthWebForm::OnLoadingCompleted(void)
{
	String *url = new (std::nothrow) String(__pWeb->GetUrl().GetPointer());
	AppLog("OnLoadingCompleted %S", url->GetPointer());

	if (this->__watingForReqistration) {
		__watingForReqistration = false;
		__pWeb->LoadUrl(RequestAuthUrl()->GetPointer());
		return;
	}

	if(url->StartsWith(VK_CALLBACK, 0)) {
		if (ExtractAccessDataAndSave(*url)) {

			try {
				AuthManager::getInstance().SendRequest();
			} catch (...) {

			}

			if(AuthManager::getInstance().IsForced()) {
				AuthManager::getInstance().SetForced(false);
			}

			LongPollConnection::getInstance().Run();
			SceneManager* pSceneManager = SceneManager::GetInstance();
			AppAssert(pSceneManager);
			pSceneManager->GoForward(ForwardSceneTransition(SCENE_MAIN_MESSAGES_TAB, SCENE_TRANSITION_ANIMATION_TYPE_LEFT));

			try {
				AuthManager::getInstance().SetUserOnline();
			} catch(...) {

			}

		} else {
			SceneManager* pSceneManager = SceneManager::GetInstance();
			AppAssert(pSceneManager);
			pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));
		}
	}
}


void
AuthWebForm::OnEstimatedProgress(int progress)
{
}

void
AuthWebForm::OnPageTitleReceived(const Tizen::Base::String& title)
{
}

bool
AuthWebForm::OnLoadingRequested(const Tizen::Base::String& url, WebNavigationType type)
{

	AppLog("%S",url.GetPointer());

	if(url.Contains(VK_LOGIN_AFTER_REG) && url.Contains(L"join_code")) {
		//Reg process:

		//Requested
		//Complited

		//https://m.vk.com/join
		//https://m.vk.com/join

		//https://m.vk.com/join?act=start
		//https://m.vk.com/join?act=finish

		//https://login.vk.com/?act=login&_origin=https://m.vk.com&ip_h=26569ad0e14f34c684&role=pda&join_code=45917&join_hash=4918def96a5cb8497d6269a130333e4b&to=am9pbj9hY3Q9ZG9uZQ--
		//https://m.vk.com/id229874355

		__watingForReqistration = true;
	}

	return false;
}

DecisionPolicy
AuthWebForm::OnWebDataReceived(const Tizen::Base::String& mime, const Tizen::Net::Http::HttpHeader& header)
{
	return WEB_DECISION_CONTINUE;
}

//void
//WebViewer::OnWebKeypadWillOpen(Tizen::Web::Controls::Web& source)
//{
//	Rectangle bound = __pMainForm->GetClientAreaBounds();
//	AppLog("OnWebKeypadWillOpen.");
//
//	if (source.IsVisible())
//	{
//		source.SetSize(bound.width, bound.height - EDIT_LINE_HEIGHT - EDIT_LINE_BOTTOM_MARGIN);
//	}
//}
//
//void
//WebViewer::OnWebKeypadOpened(Tizen::Web::Controls::Web& source)
//{
//	AppLog("OnWebKeypadOpened.");
//}
//
//void
//WebViewer::OnWebKeypadClosed(Tizen::Web::Controls::Web& source)
//{
//	AppLog("OnWebKeypadClosed.");
//	Rectangle bound = __pMainForm->GetClientAreaBounds();
//	source.SetSize(bound.width, bound.height - EDIT_LINE_HEIGHT - EDIT_LINE_BOTTOM_MARGIN);
//}

void
AuthWebForm::OnWebPageBlockSelected(Tizen::Web::Controls::Web &source, Tizen::Graphics::Point &startPoint, Tizen::Graphics::Point &endPoint)
{
}

void
AuthWebForm::OnWebPageShowRequested(Tizen::Web::Controls::Web &source)
{
}

void
AuthWebForm::OnWebWindowCloseRequested(Tizen::Web::Controls::Web &source)
{
}

Tizen::Web::Controls::Web*
AuthWebForm::OnWebWindowCreateRequested(void)
{
	MessageBox messageBox;
	messageBox.Construct(L"", L"WebViewer doesn't include new window creation.", MSGBOX_STYLE_NONE, 3000);

	int modalResult = 0;
	messageBox.ShowAndWait(modalResult);

	return null;
}
