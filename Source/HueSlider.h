// BeWake
// Copyright 2003 Mattia Tristo
// Copyright 2009 Mattia Tristo and Gabriele Biffi

#ifndef HUESLIDER_H
#define HUESLIDER_H

// Haiku headers
#include <Slider.h>

class HueSlider : public BSlider
{
public:
    		HueSlider(BRect frame, const char *name, const char *label,
    			BMessage *message, int32 resizingMode = B_FOLLOW_LEFT | B_FOLLOW_TOP,
    			int32 flags = B_FRAME_EVENTS | B_WILL_DRAW | B_NAVIGABLE);
    
    // BSlider anchors
    void	DrawBar(void);
};

#endif // HUESLIDER_H
