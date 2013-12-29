// BeWake
// Copyright 2003 Mattia Tristo
// Copyright 2009 Mattia Tristo and Gabriele Biffi

// Standard C headers
#include <stdio.h>

// Haiku headers
#include <CheckBox.h>
#include <Menu.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <StringView.h>

// Project headers
#include "BeWake.h"
#include "ConfigView.h"
#include "HueSlider.h"
#include "Utils.h"

// Constructors and destructor ------------------------------------------------

/*ConfigView::ConfigView(BRect frame, BeWake *screenSaver, float color,
	bool cycleColors, bool randomColor, bool waitForRetrace) :*/
ConfigView::ConfigView(BRect frame, BeWake *screenSaver, float color,
	bool cycleColors, bool randomColor) :
	BView(frame, "BeWake Configuration Panel", B_FOLLOW_ALL_SIDES, 0),
	fScreenSaver(screenSaver)
{
	BStringView *label;
	BView *last;

  	// Make our background color match the one used by the screensaver
  	// configuration panel
  	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	// Info labels

  	label = new BStringView(BRect(8, 0, 200, 60), "lblTitle", "BeWake");
	label->SetFontSize(48);
	label->ResizeToPreferred();
	last = label;
	AddChild(last);

	label = new BStringView(BRect(10, last->Frame().bottom + 5, 200, 75), "lblAbout", "2009 Biffuz & Mattia");
	label->ResizeToPreferred();
	last = label;
	AddChild(last);

	label = new BStringView(BRect(10, last->Frame().bottom + 5, 200, 87), "lblBy", "by Gabriele Biffi and Mattia Tristo");
	label->ResizeToPreferred();
	last = label;
	AddChild(last);

	label = new BStringView(BRect(10, last->Frame().bottom + 5, 200, 99), "lblEMail", "gb@biffuz.it - mattia.tristo@gmail.com");
	label->ResizeToPreferred();
	last = label;
	AddChild(last);

	// Options

	fColorMenu = new BMenu("fColorMenu");
	fColorMenu->AddItem(new BMenuItem("Cycle colors", new BMessage('clcy')));
	fColorMenu->AddItem(new BMenuItem("Random color", new BMessage('clra')));
	fColorMenu->AddItem(new BMenuItem("Custom color", new BMessage('clcu')));
	fColorMenu->SetRadioMode(true);
	fColorMenu->SetLabelFromMarked(true);
	if (cycleColors)
		fColorMenu->ItemAt(0)->SetMarked(true);
	else if (randomColor)
		fColorMenu->ItemAt(1)->SetMarked(true);
	else
		fColorMenu->ItemAt(2)->SetMarked(true);
	fColorField = new BMenuField(BRect(10, last->Frame().bottom + 5, 200, 120),
		"fColorField", "Color", fColorMenu);
	fColorField->ResizeToPreferred();
	fColorField->SetDivider(fColorField->StringWidth(fColorField->Label()) + 10);
	last = fColorField;
	AddChild(last);

	fColorSlider = new HueSlider(BRect(10, last->Frame().bottom + 5, 200, 200),
		"fColorSlider", "Color", new BMessage('clco'));
	fColorSlider->ResizeToPreferred();
	fColorSlider->SetValue(color * 100);

  	// Wait for retrace - TODO is it supported by Haiku?
  	/*chkUseWaitForRetrace = new BCheckBox(BRect(10, last->Frame().bottom + 5, 200, 148), "chkUseWaitForRetrace","Use WaitForRetrace", new BMessage('cbwr'), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	chkUseWaitForRetrace->ResizeToPreferred();
    chkUseWaitForRetrace->SetValue(waitForRetrace ? B_CONTROL_ON : B_CONTROL_OFF);
    last = chkUseWaitForRetrace;
  	AddChild(last);*/
}

// Private methods ------------------------------------------------------------

void ConfigView::_ShowColorSlider(bool show)
{
	if (show) {
		if (fColorSlider->Parent() == NULL) {
			AddChild(fColorSlider);
			fColorSlider->MoveTo(fColorField->Frame().left, fColorField->Frame().bottom + 10);
			fColorSlider->ResizeToPreferred();
			fColorSlider->Show();
		}
	}
	else {
		if (fColorSlider->Parent() == this)
			RemoveChild(fColorSlider);
	}
}	

// Public methods -------------------------------------------------------------

void ConfigView::AttachedToWindow(void)
{
	BView::AttachedToWindow();

	fColorMenu->SetTargetForItems(this);
	fColorSlider->SetTarget(this);
	//chkUseWaitForRetrace->SetTarget(this);

	if (fColorMenu->ItemAt(2)->IsMarked())
		_ShowColorSlider(true);
}

void ConfigView::MessageReceived(BMessage *msg)
{
	#ifdef __DEBUG__
	printf("ConfigView::MessageReceived(%p, %p, 0x%08X)\n", this, msg, msg->what);
	#endif

	switch (msg->what) {
	case 'clco':
		fScreenSaver->SetColor((float)fColorSlider->Value() / 100.00);
		break;
	case 'clcu':
		fScreenSaver->SetColorFixed();
		_ShowColorSlider(true);
		break;
	case 'clcy':
		fScreenSaver->SetColorCycle();
		_ShowColorSlider(false);
		break;
	case 'clra':
		fScreenSaver->SetColorRandom();
		_ShowColorSlider(false);
		break;
	/*case 'cbwr':
        fScreenSaver->SetWaitForRetrace(chkUseWaitForRetrace->Value() == B_CONTROL_ON);
      	break;*/
	}

	BView::MessageReceived(msg);
}
