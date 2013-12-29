// BeWake
// Copyright 2003 Mattia Tristo
// Copyright 2009 Mattia Tristo and Gabriele Biffi

#ifndef CONFIGVIEW_H
#define CONFIGVIEW_H

// Haiku headers
#include <View.h>

// Haiku classes
class BCheckBox;
class BMenu;
class BMenuField;

// Project classes
class BeWake;
class HueSlider;

class ConfigView : public BView
{
private:
	void			_ShowColorSlider(bool show);

    // The screensaver object
    BeWake *		fScreenSaver;

    // Options
    BMenuField *	fColorField;
   	BMenu *			fColorMenu;
   	HueSlider *		fColorSlider;
  	//BCheckBox *		chkUseWaitForRetrace;

public:
    		/*ConfigView(BRect frame, BeWake *screenSaver, float color,
    			bool cycleColors, bool randomColor, bool waitForRetrace);*/
    		ConfigView(BRect frame, BeWake *screenSaver, float color,
				bool cycleColors, bool randomColor);

    // BView anchors
    void	AttachedToWindow(void);
    void	MessageReceived(BMessage *msg);
};

#endif // CONFIGVIEW_H
