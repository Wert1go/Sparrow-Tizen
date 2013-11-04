/*
 * AuthForm.cpp
 *
 *  Created on: Nov 3, 2013
 *      Author: developer
 */

#include <new>
#include "AuthForm.h"
#include <FApp.h>
#include <FBase.h>
#include <FGraphics.h>
#include "FMedia.h"
#include "SceneRegister.h"

#define ID_BUTTON_LOGIN 			101
#define ID_BUTTON_REGISTRATION	 	102

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Io;
using namespace Tizen::Media;
using namespace Tizen::Graphics;

AuthForm::AuthForm() {
	// TODO Auto-generated constructor stub
	Form::Construct(FORM_STYLE_NORMAL);


	result r = E_SUCCESS;

	Rectangle rect = GetBounds();
	int buttonWidth = 0;
	int yOffset = 0;

	buttonWidth = (rect.width * 84)/100;
	yOffset = (rect.height * 48)/100;

	Color *textNormalColor = new (std::nothrow) Color(255, 255, 255, 255);
	Color *textSelectedColor = new (std::nothrow) Color(0, 0, 0, 255);
	Color *buttonNormalColor = new (std::nothrow) Color(52, 87, 119, 255);
	Color *buttonSelectedColor = new (std::nothrow) Color(255, 255, 255, 255);

	pAuthButton = new (std::nothrow) Button();
	pAuthButton->Construct(Rectangle(rect.width/2 - buttonWidth/2, yOffset, buttonWidth, 80));
	pAuthButton->SetText(L"Войти через VK");
	pAuthButton->SetActionId(ID_BUTTON_LOGIN);
	pAuthButton->AddActionEventListener(*this);
	pAuthButton->SetTextColor(*textNormalColor);
	pAuthButton->SetHighlightedTextColor(*textSelectedColor);
	pAuthButton->SetColor(BUTTON_STATUS_NORMAL, *buttonNormalColor);
	pAuthButton->SetColor(BUTTON_STATUS_PRESSED, *buttonSelectedColor);

	r = AddControl(pAuthButton);

	delete textNormalColor;
	delete textSelectedColor;
	delete buttonNormalColor;
	delete buttonSelectedColor;

	buttonWidth = (rect.width * 58)/100;
	yOffset = (rect.height * 75)/100;

	textNormalColor = new (std::nothrow) Color(60, 117, 186, 255);
	textSelectedColor = new (std::nothrow) Color(255, 255, 255, 255);
	buttonNormalColor = new (std::nothrow) Color(0, 0, 0, 0);
	buttonSelectedColor = new (std::nothrow) Color(0, 0, 0, 0);

	pRegButton = new (std::nothrow) Button();
	pRegButton->Construct(Rectangle(rect.width/2 - buttonWidth/2, yOffset, buttonWidth, 70));
	pRegButton->SetText(L"Зарегистрироваться");
	pRegButton->SetActionId(ID_BUTTON_REGISTRATION);
	pRegButton->AddActionEventListener(*this);
	pRegButton->SetTextColor(*textNormalColor);
	pRegButton->SetTextSize(40);
	pRegButton->SetHighlightedTextColor(*textSelectedColor);

	Image image;
	r = image.Construct();
	String filepath = App::GetInstance()->GetAppResourcePath() + L"empty_bg.png";
	Bitmap *pBackgroundBitmap = image.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);

	pRegButton->SetNormalBackgroundBitmap(*pBackgroundBitmap);
	pRegButton->SetHighlightedBackgroundBitmap(*pBackgroundBitmap);
	pRegButton->SetPressedBackgroundBitmap(*pBackgroundBitmap);

	r = AddControl(pRegButton);

	delete textNormalColor;
	delete textSelectedColor;
	delete buttonNormalColor;
	delete buttonSelectedColor;

	Color *labelTextColor = new (std::nothrow) Color(86, 86, 93, 255);

	yOffset = (rect.height * 82)/100;
	buttonWidth = (rect.width * 82)/100;

	pInfoLabel = new (std::nothrow) TextBox();
	pInfoLabel->Construct(Rectangle(rect.width/2 - buttonWidth/2, yOffset, buttonWidth, 160), TEXT_BOX_BORDER_NONE);
	pInfoLabel->SetText(L"Вы сможете бесплатно обмениваться мгновенными сообщениями с друзьями");
	pInfoLabel->SetTextAlignment(ALIGNMENT_CENTER);
	pInfoLabel->SetTextColor(TEXT_BOX_TEXT_COLOR_NORMAL, *labelTextColor);
	pInfoLabel->SetTextSize(28);
	pInfoLabel->SetFocusable(false);

	pInfoLabel->SetBackgroundBitmap(TEXT_BOX_STATUS_NORMAL, *pBackgroundBitmap);
	pInfoLabel->SetBackgroundBitmap(TEXT_BOX_STATUS_HIGHLIGHTED, *pBackgroundBitmap);
	pInfoLabel->SetBackgroundBitmap(TEXT_BOX_STATUS_DISABLED, *pBackgroundBitmap);

	r = AddControl(pInfoLabel);

	delete pBackgroundBitmap;
	delete labelTextColor;

}

AuthForm::~AuthForm() {
	// TODO Auto-generated destructor stub
}


result AuthForm::OnDraw(void) {
	result r = E_SUCCESS;

	Image image;
	r = image.Construct();
	String filepath = App::GetInstance()->GetAppResourcePath() + L"Main.png";

	Bitmap *pBackgroundBitmap = image.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);

	Canvas* pCanvas = GetCanvasN();

	if (pCanvas != null && pBackgroundBitmap != null)
	{
		Rectangle rect = GetBounds();
		r = pCanvas->DrawBitmap(rect, *pBackgroundBitmap);
	}

	delete pCanvas;
	return r;
}

void
AuthForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	ArrayList *paramsList = new (std::nothrow) ArrayList();
	paramsList->Construct();

	switch(actionId)
	{
	case ID_BUTTON_LOGIN:
		{
			String *key = new String(L"AUTH");

			paramsList->Add(*key);
			pSceneManager->GoForward(ForwardSceneTransition(SCENE_WEB_AUTH, SCENE_TRANSITION_ANIMATION_TYPE_LEFT), paramsList);
			AppLog("Button is pressed! \n");
		}
		break;

	case ID_BUTTON_REGISTRATION:
		{
			String *key = new String(L"REG");

			paramsList->Add(*key);
			pSceneManager->GoForward(ForwardSceneTransition(SCENE_WEB_AUTH, SCENE_TRANSITION_ANIMATION_TYPE_LEFT), paramsList);
			AppLog("Button is pressed! \n");
		}
		break;
	}

	Invalidate(true);
}
