/*
 * Bean.h
 *
 *  Created on: Nov 5, 2013
 *      Author: developer
 */

#ifndef BEAN_H_
#define BEAN_H_

#include <FWeb.h>

using namespace Tizen::Base;

class Bean {
public:
	Bean();
	virtual ~Bean();
	static Bean* CreateFromJsonN(const Tizen::Web::Json::JsonObject &jsonObject);
	static String& TableDescription();
};

#endif /* BEAN_H_ */
