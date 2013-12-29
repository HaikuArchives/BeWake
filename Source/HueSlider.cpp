// BeWake
// Copyright 2003 Mattia Tristo
// Copyright 2009 Mattia Tristo and Gabriele Biffi

// Project headers
#include "HueSlider.h"
#include "Utils.h"

// Constructors and destructor ------------------------------------------------

HueSlider::HueSlider(BRect frame, const char *name, const char *label, BMessage *message,
	int32 resizingMode, int32 flags) :
	BSlider(frame, name, label, message, 0, 99, B_HORIZONTAL, B_TRIANGLE_THUMB, resizingMode, flags)
{
}

// Public methods -------------------------------------------------------------

void HueSlider::DrawBar(void)
{
	float huePerPixel;
	uint8 r, g, b;
	BView *view;
	BRect rect;
	int i;

	view = OffscreenView();
	rect = BarFrame();
	huePerPixel = 1.00 / (rect.Width() + 1.00);

	for (int i = (int)rect.left; i <= (int)rect.right; i++) {
		HSLtoRGB(huePerPixel * (float)i, 1.0, 0.5, &r, &g, &b);
		view->SetHighColor(r, g, b);
		view->StrokeLine(BPoint(i, rect.top), BPoint(i, rect.bottom), B_SOLID_HIGH);
	}

	view->SetHighColor(118, 118, 118);
	view->StrokeRect(rect, B_SOLID_HIGH);
}
