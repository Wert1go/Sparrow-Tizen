/*
 * Resources.h
 *
 *  Created on: Nov 15, 2013
 *      Author: developer
 */

#ifndef RESOURCES_H_
#define RESOURCES_H_

#include <FGraphics.h>

using namespace Tizen::Base;
using namespace Tizen::Graphics;

class Resources {
public:
	static Resources& getInstance()
	{
		static Resources	instance; // Guaranteed to be destroyed.
								  // Instantiated on first use.
		return instance;
	}

private:
	Resources();
	virtual ~Resources();
	Resources(Resources const&);              // Don't Implement
	void operator = (Resources const&);

public:
	Bitmap *GetOnlineIndicator();
	Bitmap *GetNormalRoundImageForm();
	Bitmap *GetNormalUnreadRoundImageForm();
	Bitmap *GetSelectedRoundImageForm();
	Bitmap *GetDeliveredIcon();

	Bitmap *GetGroupNormalIcon();
	Bitmap *GetGroupPressedIcon();

	Bitmap *GetVideoPlayIcon();
	Bitmap *GetDocumentIcon();

	Bitmap *GetAudioPlayIcon();
	Bitmap *GetAudioPlayPressedIcon();

	Bitmap *GetAudioPauseIcon();
	Bitmap *GetAudioPausePressedIcon();

	Bitmap *GetUserAvatar();

	Bitmap *GetNoUserAvatar();
	Bitmap *GetNoGroupAvatar();

	Bitmap *LoadBitmapNamed(String name);

private:
	Bitmap *__pOnlineIndicator;
	Bitmap *__pNormalRoundImageForm;
	Bitmap *__pNormalUnreadRoundImageForm;
	Bitmap *__pSelectedRoundImageForm;
	Bitmap *__pDeliveredIcon;

	Bitmap *__pGroupNormalIcon;
	Bitmap *__pGroupPressedIcon;

	Bitmap *__pVideoPlayIcon;

	Bitmap *__pDocumentIcon;

	Bitmap *__pUserAvatar;
	Bitmap *__pNoUserAvatar;
	Bitmap *__pNoGroupAvatar;

	Bitmap *__pAudioPlayIcon;
	Bitmap *__pAudioPlayPressedIcon;

	Bitmap *__pAudioPauseIcon;
	Bitmap *__pAudioPausePressedIcon;

};

#endif /* RESOURCES_H_ */
