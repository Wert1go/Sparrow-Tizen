/*
 * SettingsForm.cpp
 *
 *  Created on: Nov 4, 2013
 *      Author: developer
 */

#include "SettingsForm.h"
#include <FGraphics.h>
#include <FMedia.h>
#include <FApp.h>
#include <FBase.h>
#include <FNet.h>
#include <FWeb.h>
#include "SceneRegister.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Ui;
using namespace Tizen::Io;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Graphics;
using namespace Tizen::Media;
using namespace Tizen::Net::Http;
using namespace Tizen::Web::Json;


SettingsForm::SettingsForm() {
	Form::Construct(FORM_STYLE_HEADER);
	SetFormBackEventListener(this);

	Color *pFormBackgroundColor = new (std::nothrow) Color(0, 0, 0, 255);
	this->SetBackgroundColor(*pFormBackgroundColor);

	Color *pHeaderBackgroundColor = new (std::nothrow) Color(65, 97, 137, 255);
	Color *pHeaderTextColor = new (std::nothrow) Color(255, 255, 255, 255);

	Header* pHeader = this->GetHeader();

	pHeader->SetTabEditModeEnabled(false);
	pHeader->SetStyle(HEADER_STYLE_TITLE);
	pHeader->SetColor(*pHeaderBackgroundColor);
	pHeader->SetTitleTextColor(*pHeaderTextColor);
	pHeader->SetTitleText(L"Имя пользователя");

	delete pHeaderBackgroundColor;
	delete pHeaderTextColor;
	delete pFormBackgroundColor;

	SendRequest();
}

SettingsForm::~SettingsForm() {
	// TODO Auto-generated destructor stub
}

void
SettingsForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);
	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_RIGHT));
}

void
SettingsForm::SendRequest() {
	String* pProxyAddr = null;
	String hostAddr = L"https://api.vk.com/";
	String uri = L"https://api.vk.com/method/users.get?user_ids=30143161";

	HttpHeader* pHeader = null;
	HttpTransaction* pHttpTransaction = null;
	__pHttpSession = new HttpSession();
	__pHttpSession->Construct(NET_HTTP_SESSION_MODE_NORMAL, pProxyAddr, hostAddr, null);

	pHttpTransaction = __pHttpSession->OpenTransactionN();
	pHttpTransaction->AddHttpTransactionListener(*this);

	HttpRequest* pHttpRequest = pHttpTransaction->GetRequest();

	pHttpRequest->SetMethod(NET_HTTP_METHOD_GET);
	pHttpRequest->SetUri(uri);
	pHeader = pHttpRequest->GetHeader();
	pHeader->AddField(L"Accept", L"application/json");

	pHttpTransaction->Submit();
}

void
SettingsForm::OnTransactionReadyToRead(HttpSession& httpSession, HttpTransaction& httpTransaction, int availableBodyLen)
{
	AppLog("OnTransactionReadyToRead");

	HttpResponse* pHttpResponse = httpTransaction.GetResponse();
	if (pHttpResponse->GetHttpStatusCode() == HTTP_STATUS_OK)
	{
		HttpHeader* pHttpHeader = pHttpResponse->GetHeader();
		if (pHttpHeader != null)
		{
			String* tempHeaderString = pHttpHeader->GetRawHeaderN();
			ByteBuffer* pBuffer = pHttpResponse->ReadBodyN();

			AppLog("OnTransactionReadyToRead::JSON START");

			IJsonValue* pJson = JsonParser::ParseN(*pBuffer);
			JsonObject* pObject = static_cast< JsonObject* >(pJson);

			if (pJson != null) {
				JsonString* pKeyResponse = new JsonString(L"response");
				IJsonValue* pValResponseArray = null;
				pObject->GetValue(pKeyResponse, pValResponseArray);

				JsonArray *pArray = static_cast< JsonArray* >(pValResponseArray);
				AppLog("OnTransactionReadyToRead::JSON START ARRAY");
				if (pArray->GetCount() > 0) {
					IJsonValue* pUserObjectValue = null;
					pArray->GetAt(0, pUserObjectValue);

					JsonObject* pUserObject = static_cast< JsonObject* >(pUserObjectValue);


					JsonString* pKeyFirstName = new JsonString(L"first_name");
					JsonString* pKeyLastName = new JsonString(L"last_name");

					IJsonValue* pValFirstName = null;
					IJsonValue* pValLastName = null;

					pUserObject->GetValue(pKeyFirstName, pValFirstName);
					pUserObject->GetValue(pKeyLastName, pValLastName);

					JsonString *firstName = static_cast< JsonString* >(pValFirstName);
					JsonString *lastName = static_cast< JsonString* >(pValLastName);

					if (firstName != null && lastName != null) {

						String title(L"");

						title.Append(firstName->GetPointer());
						title.Append(L" ");
						title.Append(lastName->GetPointer());

						this->GetHeader()->SetTitleText(title);

						Draw();
					}

				}
			}




			Draw();

			delete tempHeaderString;
			delete pBuffer;
		}
	}
}

void
SettingsForm::OnTransactionAborted(HttpSession& httpSession, HttpTransaction& httpTransaction, result r)
{
	AppLog("OnTransactionAborted(%s)", GetErrorMessage(r));

	delete &httpTransaction;
}

void
SettingsForm::OnTransactionReadyToWrite(HttpSession& httpSession, HttpTransaction& httpTransaction, int recommendedChunkSize)
{
	AppLog("OnTransactionReadyToWrite");
}

void
SettingsForm::OnTransactionHeaderCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction, int headerLen, bool authRequired)
{
	AppLog("OnTransactionHeaderCompleted");
}

void
SettingsForm::OnTransactionCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction)
{
	AppLog("OnTransactionCompleted");

	delete &httpTransaction;
}

void
SettingsForm::OnTransactionCertVerificationRequiredN(HttpSession& httpSession, HttpTransaction& httpTransaction, Tizen::Base::String* pCert)
{
	AppLog("OnTransactionCertVerificationRequiredN");

	httpTransaction.Resume();

	delete pCert;
}
