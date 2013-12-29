// BeWake
// Copyright 2003 Mattia Tristo
// Copyright 2009 Mattia Tristo and Gabriele Biffi

// Standard C headers
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Haiku headers
#include <Bitmap.h>
#include <Screen.h>
#include <ScreenSaver.h>

// Project headers
#include "BeWake.h"
#include "ConfigView.h"
#include "Utils.h"

// Main instanctiation function -----------------------------------------------

extern "C" _EXPORT BScreenSaver *instantiate_screen_saver(BMessage *message, image_id image)
{
	return new BeWake(message, image);
}

// Constructors and destructor ------------------------------------------------

BeWake::BeWake(BMessage *message, image_id image) :
	BScreenSaver(message, image),
	dx1(3),
	dx2(4),
	dy1(3),
	dy2(4),
	fBackBitmap(NULL),
	fBackView(NULL),
	fColor(0.66),
	fCycleColors(true),
	fHuePerStep(0.05),
	fLightPerStep(0.01),
	fLines(NULL),
	fNextHue(0.66),
	fNumLines(50)
{
	#ifdef __DEBUG__
	printf("BeWake::BeWake(%p, %p, %d)\n", this, message, image);
	#endif

	// Load options

	// Color
  	if (message->FindBool("CycleColors", &fCycleColors) != B_OK)
  		fCycleColors = true;
	if (message->FindBool("RandomColor", &fRandomColor) != B_OK)
  		fRandomColor = false;
  	if (fCycleColors)
  		fRandomColor = false;
	if (message->FindFloat("Color", &fColor) != B_OK)
		fColor = 0.66;
	if (fColor < 0.00 || fColor >= 1.00)
		fColor = 0.66;

	// Wait for screen retrace
  	/*if (message->FindBool("WaitForRetrace", &fWaitForRetrace) != B_OK)
    	fWaitForRetrace = false;*/
}

BeWake::~BeWake(void)
{
	#ifdef __DEBUG__
	printf("BeWake::~BeWake(%p)\n", this);
	#endif

	// Delete the offscreen bitmap
	if (fBackBitmap != NULL)
		delete fBackBitmap;

	// Delete the lines
	if (fLines != NULL)
		delete[] fLines;
}

// Private methods ------------------------------------------------------------

void BeWake::Init(BView *view)
{
	#ifdef __DEBUG__
	printf("BeWake::Init(%p, %p)\n", this, view);
	#endif

	// Allocate memory for lines
	if (fLines != NULL)
		delete[] fLines;
	fLines = new WakeLine[fNumLines];

	// Initialize color
	fLightPerStep = (0.50 / (float)fNumLines);
	fHuePerStep = 0.01;
	if (fCycleColors)
		fNextHue = MyRand(0.0, 1.0);
	else
		fNextHue = (fRandomColor ? MyRand(0.00, 1.00) : fColor);

	// Set up first line
	fLines[0].x1	= (int)MyRand(0.0, view->Bounds().Width() - 1.0);
	fLines[0].y1	= (int)MyRand(0.0, view->Bounds().Height() - 1.0);
	fLines[0].x2    = (int)MyRand(0.0, view->Bounds().Width() - 1.0);
	fLines[0].y2    = (int)MyRand(0.0, view->Bounds().Height() - 1.0);
	fLines[0].hue	= fNextHue;
	fLines[0].sat   = 1.00;
	fLines[0].light = 0.50;
	
	// Clear the next lines	
	for (int i = 1; i < fNumLines; i++) {
		fLines[i].hue	= fNextHue;
		fLines[i].sat   = 1.00;
		fLines[i].light = 0.00;
	}
}

void BeWake::MoveLines(BView *view)
{
	int x1, y1, x2, y2;
	BRect r;

	r = view->Bounds();

	// Calculate position of next line

	x1 = fLines[0].x1;
	y1 = fLines[0].y1;
	x2 = fLines[0].x2;
	y2 = fLines[0].y2;

	x1 += dx1;
	y1 += dy1;
	if (x1 > r.right || x1 < r.left) {
		dx1 *= -1;
		x1 += 2 * dx1;
	}
	if (y1 > r.bottom || y1 < r.top) {
		dy1 *= -1;
		y1 += 2 * dy1;
	}

	x2 = x2 + dx2;
	y2 = y2 + dy2;
	if (x2 > r.right || x2 < r.left) {
		dx2 *= -1;
		x2 += 2 * dx2;
	}
	if (y2 > r.bottom || y2 < r.top) {
		dy2 *= -1;
		y2 += 2 * dy2;
	}

	// Move the lines
	for (int i = (fNumLines - 1); i > 0; i--) {
		fLines[i].x1    = fLines[i-1].x1;
		fLines[i].y1    = fLines[i-1].y1;
		fLines[i].x2    = fLines[i-1].x2;
		fLines[i].y2    = fLines[i-1].y2;
		fLines[i].hue   = fLines[i-1].hue;
		fLines[i].light = fLines[i-1].light - fLightPerStep; // Darken the line
	}

	// Next line color
	if (fCycleColors) {
		fNextHue += fHuePerStep;
		if (fNextHue >= 1.00)
			fNextHue = 0.00;
	}

	// New line
	fLines[0].x1    = x1;
	fLines[0].y1    = y1;
	fLines[0].x2    = x2;
	fLines[0].y2    = y2;
	fLines[0].hue   = fNextHue;
	fLines[0].sat   = 1.00;
	fLines[0].light = 0.50;
}

void BeWake::DrawLines(BView *view)
{
	uint8 red, green, blue;
	BPoint p1, p2;
		
	for (int i = fNumLines - 1; i >= 0; i--) {
		p1.x = fLines[i].x1;
		p1.y = fLines[i].y1;
		p2.x = fLines[i].x2;
		p2.y = fLines[i].y2;
		HSLtoRGB(fLines[i].hue, fLines[i].sat, fLines[i].light, &red, &green, &blue);
		view->SetHighColor(red, green, blue);
		view->StrokeLine(p1, p2, B_SOLID_HIGH);
	}
}

// Public methods -------------------------------------------------------------

// Draw the screensaver
void BeWake::Draw(BView *view, int32 frame)
{
	fLocker.Lock();

	// If random color, reset the screensaver every 1000 frames
	if (fRandomColor && frame % 1000 == 0)
		Init(view);

	// Draw the lines on the off-screen bitmap
	if (fBackBitmap->Lock()) {
		fBackView->FillRect(fBackView->Bounds(), B_SOLID_LOW);
		MoveLines(fBackView);
		DrawLines(fBackView);
		fBackView->Sync();
		fBackBitmap->Unlock();
	}

    // Used to sync to vertical retrace; this makes for a smoother display on some systems, as
    // drawing will take place during the vertical blank -- and less flickering will be evident
    // TODO is it supported by Haiku?
    /*if (fWaitForRetrace) {
		BScreen screen;
		screen.WaitForRetrace();
	}*/

	// Draw the off-screen bitmap on the target BView
	view->DrawBitmap(fBackBitmap, view->Bounds().LeftTop());

	fLocker.Unlock();
}

// Saves the state of the screensaver (stores config variables)
status_t BeWake::SaveState(BMessage *into) const
{
	#ifdef __DEBUG__
	printf("BeWake::SaveState(%p, %p)\n", this, into);
	#endif

	into->AddBool("CycleColors", fCycleColors);
	into->AddBool("RandomColor", fRandomColor);
	into->AddFloat("Color", fColor);
	//into->AddBool("WaitForRetrace", fWaitForRetrace);

	return B_OK;
}

// Set the color (ignored if cycling or random, but saved)
void BeWake::SetColor(float hue)
{
	#ifdef __DEBUG__
	printf("BeWake::SetColor(%p, %.2f)\n", this, hue);
	#endif

	fLocker.Lock();
	fColor = hue;
	fNextHue = hue;
	fLocker.Unlock();
}

// Set colors cycling
void BeWake::SetColorCycle(void)
{
	#ifdef __DEBUG__
	printf("BeWake::SetColorCycle(%p)\n", this);
	#endif

	fLocker.Lock();
	fCycleColors = true;
	fRandomColor = false;
	fLocker.Unlock();
}

// Set fixed color
void BeWake::SetColorFixed(void)
{
	#ifdef __DEBUG__
	printf("BeWake::SetColorFixed(%p)\n", this);
	#endif

	fLocker.Lock();
	fCycleColors = false;
	fRandomColor = false;
	fLocker.Unlock();
}

// Set random color (changes every 1000 frames)
void BeWake::SetColorRandom(void)
{
	#ifdef __DEBUG__
	printf("BeWake::SetColorRandom(%p)\n", this);
	#endif

	fLocker.Lock();
	fCycleColors = false;
	fRandomColor = true;
	fColor = MyRand(0.00, 0.99);
	fLocker.Unlock();
}

// Set if wait for retrace
/*void BeWake::SetWaitForRetrace(bool waitForRetrace)
{
	#ifdef __DEBUG__
	printf("BeWake::SetWaitForRetrace(%p, %c)\n", this, (waitForRetrace ? 't' : 'f'));
	#endif

	fLocker.Lock();
	fWaitForRetrace = waitForRetrace;
	fLocker.Unlock();
}*/

// The Config panel shown in the screensaver-preference window
void BeWake::StartConfig(BView *view)
{
	#ifdef __DEBUG__
	printf("BeWake::StartConfig(%p, %p)\n", this, view);
	#endif

	/*fConfigView = new ConfigView(view->Bounds(), this, fColor,
		fCycleColors, fRandomColor, fWaitForRetrace);*/
	fConfigView = new ConfigView(view->Bounds(), this, fColor,
		fCycleColors, fRandomColor);
	view->AddChild(fConfigView);
	BScreenSaver::StartConfig(view);
}

// Initialize screensaver
status_t BeWake::StartSaver(BView *view, bool preview)
{
	#ifdef __DEBUG__
	printf("BeWake::StartSaver(%p, %p, %c)\n", this, view, (preview ? 't' : 'f'));
	#endif

	if (view == NULL)
		return B_OK;

	// Initialize data
  	Init(view);

	// Set interval between frames
  	SetTickSize(20000);

	// Set up the bitmap for double buffering
	if (fBackBitmap != NULL)
		delete fBackBitmap;
	fBackBitmap = new BBitmap(view->Bounds(), B_RGBA32, true);
	fBackView = new BView(fBackBitmap->Bounds(), NULL, 0, 0);
	fBackView->SetLowColor(0, 0, 0);
	fBackBitmap->AddChild(fBackView);

  	return BScreenSaver::StartSaver(view, preview);
}

// Close configuration panel
void BeWake::StopConfig(void)
{
	#ifdef __DEBUG__
	printf("BeWake::StopConfig(%p)\n", this);
	#endif

	BScreenSaver::StopConfig();
}

// Stop screensaver
void BeWake::StopSaver(void)
{
	#ifdef __DEBUG__
	printf("BeWake::StopSaver(%p)\n", this);
	#endif

	BScreenSaver::StopSaver();
}
