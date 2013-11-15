/*
 * Resources.h
 *
 *  Created on: Nov 15, 2013
 *      Author: developer
 */

#ifndef RESOURCES_H_
#define RESOURCES_H_

#include <FGraphics.h>

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

private:
	Bitmap *__pOnlineIndicator;

};

#endif /* RESOURCES_H_ */
