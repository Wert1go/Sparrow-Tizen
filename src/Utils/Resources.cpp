/*
 * Resources.cpp
 *
 *  Created on: Nov 15, 2013
 *      Author: developer
 */

#include "Resources.h"

#include <FApp.h>
#include <FBase.h>
#include <FGraphics.h>
#include <FMedia.h>

#include "MUserDao.h"
#include "ImageCache.h"
#include "Helper.h"

using namespace Tizen::Io;
using namespace Tizen::Media;
using namespace Tizen::App;
using namespace Tizen::Base;

Resources::Resources() {
	// TODO Auto-generated constructor stub
	__pUserAvatar = null;
	__pDeliveredIcon = null;
	__pOnlineIndicator = null;
	__pNormalRoundImageForm = null;
	__pNormalUnreadRoundImageForm = null;
	__pSelectedRoundImageForm = null;
	__pGroupNormalIcon = null;
	__pGroupPressedIcon = null;

	__pVideoPlayIcon = null;

	__pAudioPlayIcon = null;
	__pAudioPlayPressedIcon = null;
	__pAudioPauseIcon = null;
	__pAudioPausePressedIcon = null;

	__pNoUserAvatar = null;
	__pNoGroupAvatar = null;
}

Resources::~Resources() {

	if (__pUserAvatar) {
		delete __pUserAvatar;
		__pUserAvatar = null;
	}

	if (__pDeliveredIcon) {
		delete __pDeliveredIcon;
		__pDeliveredIcon = null;
	}

	if (__pOnlineIndicator) {
		delete __pOnlineIndicator;
		__pOnlineIndicator = null;
	}

	if (__pNormalRoundImageForm) {
		delete __pNormalRoundImageForm;
		__pNormalRoundImageForm = null;
	}

	if (__pNormalUnreadRoundImageForm) {
		delete __pNormalUnreadRoundImageForm;
		__pNormalUnreadRoundImageForm = null;
	}

	if (__pSelectedRoundImageForm) {
		delete __pSelectedRoundImageForm;
		__pSelectedRoundImageForm = null;
	}

	if (__pGroupNormalIcon) {
		delete __pGroupNormalIcon;
		__pGroupNormalIcon = null;
	}

	if (__pGroupPressedIcon) {
		delete __pGroupPressedIcon;
		__pGroupPressedIcon = null;
	}

	if (__pAudioPlayPressedIcon) {
		delete __pAudioPlayPressedIcon;
		__pAudioPlayPressedIcon = null;
	}

	if (__pAudioPlayIcon) {
		delete __pAudioPlayIcon;
		__pAudioPlayIcon = null;
	}

	if (__pAudioPauseIcon) {
		delete __pAudioPauseIcon;
		__pAudioPauseIcon = null;
	}

	if (__pAudioPausePressedIcon) {
		delete __pAudioPausePressedIcon;
		__pAudioPausePressedIcon = null;
	}

	SAFE_DELETE(__pNoUserAvatar);
	SAFE_DELETE(__pNoGroupAvatar);

}

Bitmap *
Resources::GetOnlineIndicator() {
	if (!this->__pOnlineIndicator) {
		this->__pOnlineIndicator = this->LoadBitmapNamed("online_list.png");
	}

	return this->__pOnlineIndicator;
}

Bitmap *
Resources::GetNormalRoundImageForm() {
	if (!__pNormalRoundImageForm) {
		this->__pNormalRoundImageForm = this->LoadBitmapNamed("thumbnail_list.png");
	}

	return __pNormalRoundImageForm;
}

Bitmap *
Resources::GetNormalUnreadRoundImageForm() {
	if (!__pNormalUnreadRoundImageForm) {
		this->__pNormalUnreadRoundImageForm = this->LoadBitmapNamed("thumbnail_list_unread.png");
	}

	return __pNormalUnreadRoundImageForm;
}

Bitmap *
Resources::GetSelectedRoundImageForm() {
	if (!__pSelectedRoundImageForm) {
		this->__pSelectedRoundImageForm = this->LoadBitmapNamed("thumbnail_list_active.png");
	}

	return __pSelectedRoundImageForm;
}

Bitmap *
Resources::GetDeliveredIcon() {
	if (!__pDeliveredIcon) {
		this->__pDeliveredIcon = this->LoadBitmapNamed( L"check_on.png");
	}

	return __pDeliveredIcon;
}

Bitmap *
Resources::GetGroupNormalIcon() {
	if (!__pGroupPressedIcon) {
		this->__pGroupPressedIcon = this->LoadBitmapNamed(L"list_icon_group.png");
	}
	return __pGroupPressedIcon;
}

Bitmap *
Resources::GetGroupPressedIcon() {
	if (!__pGroupPressedIcon) {
		this->__pGroupPressedIcon = this->LoadBitmapNamed(L"list_icon_group.png");
	}
	return __pGroupPressedIcon;
}

Bitmap *
Resources::GetVideoPlayIcon() {
	if (!__pVideoPlayIcon) {
		this->__pVideoPlayIcon = this->LoadBitmapNamed(L"btn_play_video.png");
	}
	return __pVideoPlayIcon;
}

Bitmap *
Resources::GetDocumentIcon() {
	if (!__pDocumentIcon) {
		this->__pDocumentIcon = this->LoadBitmapNamed(L"icon_document.png");
	}
	return __pDocumentIcon;
}

Bitmap *
Resources::GetAudioPlayIcon() {
	if (!__pAudioPlayIcon) {
		this->__pAudioPlayIcon = this->LoadBitmapNamed(L"btn_play_normal.png");
	}
	return __pAudioPlayIcon;
}

Bitmap *
Resources::GetAudioPlayPressedIcon() {
	if (!__pAudioPlayPressedIcon) {
		this->__pAudioPlayPressedIcon = this->LoadBitmapNamed(L"btn_play_press.png");
	}
	return __pAudioPlayPressedIcon;
}

Bitmap *
Resources::GetAudioPauseIcon() {
	if (!__pAudioPauseIcon) {
		this->__pAudioPauseIcon = this->LoadBitmapNamed(L"btn_pause_normal.png");
	}
	return __pAudioPauseIcon;
}

Bitmap *
Resources::GetAudioPausePressedIcon() {
	if (!__pAudioPausePressedIcon) {
		this->__pAudioPausePressedIcon = this->LoadBitmapNamed(L"btn_pause_press.png");
	}
	return __pAudioPausePressedIcon;
}

/************************** UTILS ***************************/



Bitmap *
Resources::LoadBitmapNamed(String name) {
	Image image;
	image.Construct();
	String filepath = App::GetInstance()->GetAppResourcePath() + L"Images/" + name;

	return image.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);
}

Bitmap *
Resources::GetUserAvatar() {
	if (!this->__pUserAvatar) {
		MUser *user = MUserDao::getInstance().GetCurrentUserN();
		if (user) {
			Bitmap *avatar = ImageCache::LoadFromCacheForKeyN(user->GetPhoto());
			if (avatar) {
				__pUserAvatar = avatar;
			}
		}
	}

	return __pUserAvatar;
}


Bitmap *
Resources::GetNoUserAvatar() {
	if (!__pNoUserAvatar) {
		this->__pNoUserAvatar = this->LoadBitmapNamed(L"icon_no_photo_user.png");
	}
	return __pNoUserAvatar;
}

Bitmap *
Resources::GetNoGroupAvatar() {
	if (!__pNoGroupAvatar) {
		this->__pNoGroupAvatar = this->LoadBitmapNamed(L"icon_no_photo_group.png");
	}
	return __pNoGroupAvatar;
}
