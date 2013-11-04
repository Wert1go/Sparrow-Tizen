#ifndef _SPARROWFRAME_H_
#define _SPARROWFRAME_H_

#include <FBase.h>
#include <FUi.h>

class SparrowFrame
	: public Tizen::Ui::Controls::Frame
{
public:
	SparrowFrame(void);
	virtual ~SparrowFrame(void);

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
};

#endif	//_SPARROWFRAME_H_
