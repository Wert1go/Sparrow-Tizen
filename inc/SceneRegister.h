/*
 * SceneRegister.h
 *
 *  Created on: Nov 3, 2013
 *      Author: developer
 */

#ifndef SCENEREGISTER_H_
#define SCENEREGISTER_H_

extern const wchar_t* SCENE_AUTH;
extern const wchar_t* SCENE_WEB_AUTH;
extern const wchar_t* SCENE_MAIN;

extern const wchar_t* SCENE_CHAT;

extern const wchar_t* SCENE_MAIN_MESSAGES_TAB;
extern const wchar_t* SCENE_MAIN_USERS_TAB;

extern const wchar_t* SCENE_SETTINGS;

class SceneRegister {
public:
	static void RegisterAllScenes(void);
private:
	SceneRegister();
	virtual ~SceneRegister();
};

#endif /* SCENEREGISTER_H_ */
