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
}

Resources::~Resources() {
	// TODO Auto-generated destructor stub
}

Bitmap *
Resources::GetOnlineIndicator() {
	if (!this->__pOnlineIndicator) {
		Image image;
		image.Construct();
		String filepath = App::GetInstance()->GetAppResourcePath() + L"Images/online_list.png";
		this->__pOnlineIndicator = image.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);
	}

	return this->__pOnlineIndicator;
}

Bitmap *
Resources::GetNormalRoundImageForm() {
	if (!__pNormalRoundImageForm) {
		Image image;
		image.Construct();
		String filepath = App::GetInstance()->GetAppResourcePath() + L"Images/thumbnail_list.png";
		this->__pNormalRoundImageForm = image.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);
	}

	return __pNormalRoundImageForm;
}

Bitmap *
Resources::GetNormalUnreadRoundImageForm() {
	if (!__pNormalUnreadRoundImageForm) {
		Image image;
		image.Construct();
		String filepath = App::GetInstance()->GetAppResourcePath() + L"Images/thumbnail_list_unread.png";
		this->__pNormalUnreadRoundImageForm = image.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);
	}

	return __pNormalUnreadRoundImageForm;
}

Bitmap *
Resources::GetSelectedRoundImageForm() {
	if (!__pSelectedRoundImageForm) {
		Image image;
		image.Construct();
		String filepath = App::GetInstance()->GetAppResourcePath() + L"Images/thumbnail_list_active.png";
		this->__pSelectedRoundImageForm = image.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);
	}

	return __pSelectedRoundImageForm;
}

Bitmap *
Resources::GetDeliveredIcon() {
	if (!__pDeliveredIcon) {
		Image image;
		image.Construct();
		String filepath = App::GetInstance()->GetAppResourcePath() + L"Images/check_on.png";
		this->__pDeliveredIcon = image.DecodeN(filepath, BITMAP_PIXEL_FORMAT_ARGB8888);
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
