// BeWake
// Copyright 2003 Mattia Tristo
// Copyright 2009 Mattia Tristo and Gabriele Biffi

#ifndef UTILS_H
#define UTILS_H

// Haiku headers
#include <support/SupportDefs.h>

// Convert an HSL color to a RGB color
void HSLtoRGB(const float hue, const float sat, const float light, uint8 *red, uint8 *blue, uint8 *green);

// Initialize random number generator
void InitRandom(void);

// Returns a random number between min and max included
float MyRand(float min, float max);

#endif // UTILS_H
