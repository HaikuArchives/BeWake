// BeWake
// Copyright 2003 Mattia Tristo
// Copyright 2009 Mattia Tristo and Gabriele Biffi

// Standard C headers
#include <stdlib.h>
#include <time.h>

// Project headers
#include "Utils.h"

// Macro used by HSLtoRGB
#define __conv(i, o) {					\
	if (i <= 0.00) 						\
		*o = 0;							\
	else {								\
		if (i >= 1.00)					\
			*o = 255;					\
		else							\
			*o = (uint8)(i * 255.00);	\
	}									\
}

// Convert an HSL color to a RGB color
void HSLtoRGB(const float hue, const float sat, const float light, uint8 *red, uint8 *blue, uint8 *green)
{
	float m, v, sv, fract, vsf, mid1, mid2;
	float H, S, L, R, G, B;
	int sextant;
	
	H = (hue < 0.0 ? 0.0 : (hue <= 1.0 ? hue : 1.0));
	S = (sat < 0.0 ? 0.0 : (sat <= 1.0 ? sat : 1.0));
	L = (light < 0.0 ? 0.0 : (light <= 1.0 ? light : 1.0));
	
	R = L;
	G = L;
	B = L;
	
	v = (L <= 0.5) ? (L * (1.0 + S)) : (L + S - L * S);
	
	if (v > 0.0) {
	
		m = L * 2.0 - v;
		sv = (v - m) / v;
		H = (1.0 - H) * 6.0;
		sextant = (int)H;
		fract = H - (float)sextant;
		vsf = v * sv * fract;
		mid1 = m + vsf;
		mid2 = v - vsf;
		
		switch (sextant) {
		case 0:
		case 6:
			R = v;
			G = mid1;
			B = m;
			break;
		
		case 1:
			R = mid2;
			G = v;
			B = m;
			break;
		
		case 2:
			R = m;
			G = v;
			B = mid1;
			break;
		
		case 3:
			R = m;
			G = mid2;
			B = v;
			break;
		
		case 4:
			R = mid1;
			G = m;
			B = v;
			break;
		
		case 5:
			R = v;
			G = m;
			B = mid2;
			break;
		}
	}

	__conv(R, red);
	__conv(G, green);
	__conv(B, blue);
}

// Initialize random number generator
void InitRandom(void)
{
	srand(time(NULL));
	rand();		// Trash first number, it's always the same - Haiku bug?
}

// Returns a random number between min and max included
float MyRand(float min, float max)
{
	if (min >= max)
		return min;
	return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}
