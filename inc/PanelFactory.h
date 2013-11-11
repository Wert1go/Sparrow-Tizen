/*
 * PanelFactory.h
 *
 *  Created on: Nov 10, 2013
 *      Author: developer
 */

#ifndef PANELFACTORY_H_
#define PANELFACTORY_H_

#include <FUi.h>

extern const wchar_t* PANEL_DIALOGS;

class PanelFactory
	: public Tizen::Ui::Scenes::IPanelFactory
{
public:
	PanelFactory(void);
	virtual ~PanelFactory(void);

	virtual Tizen::Ui::Controls::Panel* CreatePanelN(const Tizen::Base::String& panelId, const Tizen::Ui::Scenes::SceneId& sceneId);
};

#endif /* PANELFACTORY_H_ */
