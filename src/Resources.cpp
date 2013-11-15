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
#include <FGraphics.h>

using namespace Tizen::Io;
using namespace Tizen::Media;
using namespace Tizen::App;
using namespace Tizen::Base;

Resources::Resources() {
	// TODO Auto-generated constructor stub

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

