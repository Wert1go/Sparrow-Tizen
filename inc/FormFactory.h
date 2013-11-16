/*
 * FormFactory.h
 *
 *  Created on: Nov 3, 2013
 *      Author: developer
 */

#ifndef FORMFACTORY_H_
#define FORMFACTORY_H_

extern const wchar_t* FORM_AUTH;
extern const wchar_t* FORM_WEB_AUTH;
extern const wchar_t* FORM_MAIN;
extern const wchar_t* FORM_SETTINGS;

extern const wchar_t* FORM_CHAT;

extern const wchar_t* FORM_TAB_STYLE_BASE;

#include <FUi.h>

class FormFactory: public Tizen::Ui::Scenes::IFormFactory
{
public:
	FormFactory(void);
	virtual ~FormFactory(void);

	virtual Tizen::Ui::Controls::Form* CreateFormN(const Tizen::Base::String& formId, const Tizen::Ui::Scenes::SceneId& sceneId);
};


#endif /* FORMFACTORY_H_ */
