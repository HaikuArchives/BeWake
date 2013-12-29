// BeWake
// Copyright 2003 Mattia Tristo
// Copyright 2009 Mattia Tristo and Gabriele Biffi

#ifndef BEWAKE_H
#define BEWAKE_H

// Haiku headers
#include <Locker.h>
#include <ScreenSaver.h>

// Haiku classees
class BBitmap;

// Project classes
class ConfigView;

struct WakeLine
{
	int		x1;					// x-coordinate on screen of point 1
	int		y1;					// y-coordinate on screen of point 1
	int		x2;					// x-coordinate on screen of point 2
	int     y2;					// y-coordinate on screen of point 2
	float	hue;				// Line color
	float	sat;
	float	light;
};

class BeWake : public BScreenSaver
{
private:

	void		DrawLines(BView *view);
	void		Init(BView *view);
	void		MoveLines(BView *view);

	BBitmap *	fBackBitmap;
	BView *		fBackView;
    ConfigView *fConfigView;
    BLocker		fLocker;

	int			dx1;				// Increment on X axis of point 1
	int			dx2;				// Increment on X axis of point 2
	int			dy1;				// Increment on Y axis of point 1
	int			dy2;				// Increment on Y axis of point 2

	WakeLine *	fLines;				// Lines
	unsigned	fNumLines;			// Number of lines

	float		fColor;				// Color (hue)
	bool		fCycleColors;		// Cycle colors
	bool		fRandomColor;		// Random color

  	//bool		fWaitForRetrace;	// Wait for retrace

	float		fHuePerStep;		// Hue variation per step
	float		fLightPerStep;		// Light variation of each line per step
	float		fNextHue;			// Hue of next line

public:
				BeWake(BMessage *message, image_id image);
				~BeWake(void);

	// Set properties

	void		SetColor(float hue);
	void		SetColorCycle(void);
	void		SetColorFixed(void);
	void		SetColorRandom(void);

	//void		SetWaitForRetrace(bool waitForRetrace);

	// BScreenSaver anchors

	void		Draw(BView *view, int32 frame);
	status_t 	SaveState(BMessage *into) const;
	void		StartConfig(BView *view);
	status_t	StartSaver(BView *v, bool preview);
	void 		StopConfig(void);
	void        StopSaver(void);
};

#endif // BEWAKE_H
