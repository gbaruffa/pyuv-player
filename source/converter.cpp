// Name:    converter.cpp
// Purpose: conversion functions used throughout
// Author:  Giuseppe Baruffa

// Inclusion of the main header file
#include "converter.h"

/*
 *
 * Strange functions
 *
 */

/*
 * draw a grid on the frame
 */
void drawgrid(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height,
			  unsigned int gridsize, unsigned int gridcol)
{

	unsigned int row, col;
	unsigned char Rcol = (unsigned char) (gridcol >> 16);
	unsigned char Gcol = (unsigned char) (gridcol >> 8);
	unsigned char Bcol = (unsigned char) (gridcol >> 0);

	for (row = 0; row < height; row++) {

		for (col = 0; col < width; col++) {

			if ((row % gridsize) && (col % gridsize)) {

				*(dest++) = *(orig++);
				*(dest++) = *(orig++);
				*(dest++) = *(orig++);

			} else {

				*(dest++) = Rcol;
				*(dest++) = Gcol;
				*(dest++) = Bcol;
				orig += 3;
			}
		}
	}
}

/*
 * compute the difference between actual and previous frame
 */
void difference(unsigned char *diff, unsigned char *orig, unsigned int width, unsigned int height)
{

	static unsigned char *ref = NULL, *rref = NULL, tempref;
	unsigned int row, col;

	if (!ref)
		rref = ref = (unsigned char *) calloc(width * height * 3, sizeof(unsigned char)); 

	rref = ref;
	for (row = 0; row < height; row++) {

		for (col = 0; col < width; col++) {

			/* difference in component 1 */
			tempref = *(orig++);
			*(diff++) = tempref - *rref + 128;
			*(rref++) = tempref;

			/* difference in component 2 */
			tempref = *(orig++);
			*(diff++) = tempref - *rref + 128;
			*(rref++) = tempref;

			/* difference in component 3 */
			tempref = *(orig++);
			*(diff++) = tempref - *rref + 128;
			*(rref++) = tempref;
		}
	}
}

/*
 * Bit depth conversion functions
 *
 *
 *
 */

#define FASTLUT
#define	LUTSIZE		65535
static unsigned char scalelut[LUTSIZE], screenlut[256];
static int scalelutstart;
static float scalef[] = {
	0.0F,             /* 0 bits: unused */
	255.0F/1.0F,      /* 1 bit */
	255.0F/3.0F,      /* 2 bits */
	255.0F/7.0F,      /* 3 bits */
	255.0F/15.0F,     /* 4 bits */
	255.0F/31.0F,     /* 5 bits */
	255.0F/63.0F,     /* 6 bits */
	255.0F/127.0F,    /* 7 bits */
	255.0F/255.0F,    /* 8 bits */
	255.0F/511.0F,    /* 9 bits */
	255.0F/1023.0F,   /* 10 bits */
	255.0F/2047.0F,   /* 11 bits */
	255.0F/4095.0F,   /* 12 bits */
	255.0F/8191.0F,   /* 13 bits */
	255.0F/16383.0F,  /* 14 bits */
	255.0F/32767.0F,  /* 15 bits */
	255.0F/65535.0F   /* 16 bits */
};

static float offsetf[] = {
	0.0F,             /* 0 bits: unused */
	1.0F,             /* 1 bit */
	2.0F,             /* 2 bits */
	4.0F,             /* 3 bits */
	8.0F,             /* 4 bits */
	16.0F,            /* 5 bits */
	32.0F,            /* 6 bits */
	64.0F,            /* 7 bits */
	128.0F,           /* 8 bits */
	256.0F,           /* 9 bits */
	512.0F,           /* 10 bits */
	1024.0F,          /* 11 bits */
	2048.0F,          /* 12 bits */
	4096.0F,          /* 13 bits */
	8192.0F,          /* 14 bits */
	16384.0F,         /* 15 bits */
	32768.0F          /* 16 bits */
};

/*
 * Prepare the scaling LUT --> faster?
 *
 *
 *
 */
void
prepare_lut(int bits, int sign, int endian, double gamma)
{
#ifdef FASTLUT
	int n;

	switch (sign) {

	case 0:

		if (bits <= 8) {

			unsigned int i;
			unsigned int start = 0;
			unsigned int stop = (unsigned int) (((int) 1) << bits);
			double bbbb;

			/* Cycle on rows */
			for (i = start; i <= stop; i++) {
				bbbb = powf(((float) i * scalef[bits]) / 255.0F, gamma);
				scalelut[i] = (unsigned char) (0.5 + 255.0 * bbbb);
			}

		} else if (bits > 8) {

			unsigned short temp;
			unsigned int i;
			unsigned int start = 0;
			unsigned int stop = (unsigned int) (((int) 1) << bits);

			/* Cycle on rows */
			for (i = start; i <= stop; i++) {
				temp = endian ? SHORT_SWAP(i) : i;
				//scalelut[i] = (unsigned char) (0.5F + (float) temp * scalef[bits]);
				scalelut[i] = (unsigned char) (0.5 + 255.0 * powf(((float) temp * scalef[bits]) / 255.0F, gamma));
			}

		};
		break;

	case 1:

		if (bits <= 8) {

			int i;
			signed int start = - (signed int) (((int) 1) << (bits - 1));
			signed int stop = (signed int) ((((int) 1) << (bits - 1)) - 1);
			scalelutstart = start;

			/* Cycle on rows */
			for (i = start; i <= stop; i++)
				//scalelut[i - scalelutstart] = (unsigned char) (0.5F + ((float) i + offsetf[bits]) * scalef[bits]);
				scalelut[i] = (unsigned char) (0.5 + 255.0 * powf((((float) i + offsetf[bits]) * scalef[bits]) / 255.0F, gamma));

		} else if (bits > 8) {

			int i;
#if 0
			signed short temp;
#endif
			signed short start = - (signed short) (((int) 1) << (bits - 1));
			signed short stop = (signed short) ((((int) 1) << (bits - 1)) - 1);
			scalelutstart = start;

			/* Cycle on rows */
			for (i = start; i <= stop; i++) {
#if 0
				temp = endian ? SHORT_SWAP(i) : i;
#endif
				//scalelut[i - scalelutstart] = (unsigned char) (0.5F + ((float) i + offsetf[bits]) * scalef[bits]);
				scalelut[i - scalelutstart] = (unsigned char) (0.5 + 255.0 * powf((((float) i + offsetf[bits]) * scalef[bits]) / 255.0F, gamma));
			}
		}
		break;

	default:
		break;
	}

	for (n = 0; n < 256; n++)
		screenlut[n] = (unsigned char) (0.5 + 255.0 * powf((float) n / 255.0F, 1.0 / gamma));
#endif
}

/*
 * Change the bit depth to 8 bpp per component
 *
 *
 */
void
bitdepth_adjust(unsigned char *dest, void *orig, unsigned int numsamples, int bits, int sign, int endian)
{
	unsigned int sample;
	/*float scale  = scalef[bits];*/
	/*float offset = offsetf[bits];*/

	switch (sign) {

	case 0:

		if (bits <= 8) {

			unsigned char *myorig = (unsigned char *) orig;

			/* Cycle on rows */
			for (sample = 0; sample < numsamples; sample++)
#ifndef FASTLUT
				*(dest++) = (unsigned char) (0.5F + (float) *(myorig++) * scale);
#else
				*(dest++) = scalelut[*(myorig++)]; /* using the scale lut */
#endif

		} else if (bits > 8) {

			unsigned short int *myorig = (unsigned short int *) orig/*, temp*/;

			/* Cycle on rows */
			for (sample = 0; sample < numsamples; sample++) {
#ifndef FASTLUT
				temp = endian ? SHORT_SWAP(*myorig) : *myorig;
				*(dest++) = (unsigned char) (0.5F + (float) temp * scale);
				myorig++;
#else
				*(dest++) = scalelut[*(myorig++)]; /* using the scale lut */
#endif
			}

		};
		break;

	case 1:

		if (bits <= 8) {

			signed char *myorig = (signed char *) orig;

			/* Cycle on rows */
			for (sample = 0; sample < numsamples; sample++)
#ifndef FASTLUT
				*(dest++) = (unsigned char) (0.5F + ((float) *(myorig++) + offset) * scale);
#else
				*(dest++) = scalelut[*(myorig++) - scalelutstart]; /* using the scale lut */
#endif

		} else if (bits > 8) {

			signed short int *myorig = (signed short int *) orig/*, temp*/;

			/* Cycle on rows */
			for (sample = 0; sample < numsamples; sample++) {
#ifndef FASTLUT
				temp = endian ? SHORT_SWAP(*myorig) : *myorig;
				*(dest++) = (unsigned char) (0.5F + ((float) temp + offset) * scale);
				myorig++;
#else
				*(dest++) = scalelut[*(myorig++) - scalelutstart]; /* using the scale lut */
#endif
			}
		};
		break;

	default:
		break;
	}
}


/*
 * Colorspace conversion functions
 * They accept as input an interleaved buffer with
 * three original components, and produce an interleaved buffer
 * with the destination components
 */

/*
 * YCbCr->RGB conversion
 *
 * Rec. BT.601
 *
 */
void
ycbcr601_to_rgb(unsigned char *rgb, unsigned char *ycbcr, unsigned int width, unsigned int height)
{
	int compo;
	int y1, cb1, cr1;
	unsigned int row, col;

	/* Cycle on rows */
	for (row = 0; row < height; row++) {

		/* Cycle on columns */
		for (col = 0; col < width; col++) {

			/* Remove offset from YCbCr components */
			y1  = (int) *(ycbcr++) - 16;
			cb1 = (int) *(ycbcr++) - 128;
			cr1 = (int) *(ycbcr++) - 128;

			/* Red component conversion and clipping */
			compo = (298 * y1 + 409 * cr1) >> 8;
			*(rgb++) = (unsigned char) ((compo < 0) ? 0: ((compo > 255) ? 255: compo));

			/* Green component conversion and clipping */
			compo = (298 * y1 - 100 * cb1 - 208 * cr1) >> 8;
			*(rgb++) = (unsigned char)  ((compo < 0) ? 0: ((compo > 255) ? 255: compo));

			/* Blue component conversion and clipping */
			compo = (298 * y1 + 516 * cb1) >> 8;
			*(rgb++) = (unsigned char) ((compo < 0) ? 0: ((compo > 255) ? 255: compo));
		}
	}
}

/*
 * YCbCr->RGB conversion
 *
 * Rec. BT.709
 *
 */
void
ycbcr709_to_rgb(unsigned char *rgb, unsigned char *ycbcr, unsigned int width, unsigned int height)
{
	int compo;
	int y1, cb1, cr1;
	unsigned int row, col;

	/* Cycle on rows */
	for (row = 0; row < height; row++) {

		/* Cycle on columns */
		for (col = 0; col < width; col++) {

			/* Remove offset from YCbCr components */
			y1  = (int) *(ycbcr++) - 16;
			cb1 = (int) *(ycbcr++) - 128;
			cr1 = (int) *(ycbcr++) - 128;

			/* Red component conversion and clipping */
			compo = (298 * y1 + 459 * cr1) >> 8;
			*(rgb++) = (unsigned char) ((compo < 0) ? 0: ((compo > 255) ? 255: compo));

			/* Green component conversion and clipping */
			compo = (298 * y1 - 55 * cb1 - 136 * cr1) >> 8;
			*(rgb++) = (unsigned char)  ((compo < 0) ? 0: ((compo > 255) ? 255: compo));

			/* Blue component conversion and clipping */
			compo = (298 * y1 + 541 * cb1 + cr1) >> 8;
			*(rgb++) = (unsigned char) ((compo < 0) ? 0: ((compo > 255) ? 255: compo));
		}
	}
}

/*
 * YIQ->RGB conversion
 *
 *
 */
void yiq_to_rgb(unsigned char *rgb, unsigned char *yiq, unsigned int width, unsigned int height)
{
	int compo;
	int y, i, q;
	unsigned int row, col;

	/* Cycle on rows */
	for (row = 0; row < height; row++) {

		/* Cycle on columns */
		for (col = 0; col < width; col++) {

			/* Extend precision */
			y = (int) *(yiq++);
			i = (int) *(yiq++);
			q = (int) *(yiq++);

			/* Red component conversion and clipping */
			compo = (256 * y + 245 * i + 159 * q) >> 8;
			*(rgb++) = (unsigned char) ((compo < 0) ? 0: ((compo > 255) ? 255: compo));

			/* Green component conversion and clipping */
			compo = (256 * y - 70 * i - 166 * q) >> 8;
			*(rgb++) = (unsigned char)  ((compo < 0) ? 0: ((compo > 255) ? 255: compo));

			/* Blue component conversion and clipping */
			compo = (256 * y - 283 * i + 436 * q) >> 8;
			*(rgb++) = (unsigned char) ((compo < 0) ? 0: ((compo > 255) ? 255: compo));
		}
	}
}

/*
 * YCoCg->RGB conversion
 *
 *
 */
void ycocg_to_rgb(unsigned char *rgb, unsigned char *ycocg, unsigned int width, unsigned int height)
{
	int compo;
	int y, co, cg, tmp;
	unsigned int row, col;

	/* Cycle on rows */
	for (row = 0; row < height; row++) {

		/* Cycle on columns */
		for (col = 0; col < width; col++) {

			/* Extend precision */
			y  = (int) *(ycocg++);
			co = (int) *(ycocg++) - 127;
			cg = (int) *(ycocg++) - 128;
			tmp = y - cg;

			/* Red component conversion and clipping */
			compo = tmp + co;
			*(rgb++) = (unsigned char) ((compo < 0) ? 0: ((compo > 255) ? 255: compo));

			/* Green component conversion and clipping */
			compo = y + cg;
			*(rgb++) = (unsigned char)  ((compo < 0) ? 0: ((compo > 255) ? 255: compo));

			/* Blue component conversion and clipping */
			compo = tmp - co;
			*(rgb++) = (unsigned char) ((compo < 0) ? 0: ((compo > 255) ? 255: compo));
		}
	}
}

/*
 * YCxCz->RGB conversion
 *
 */
void
ycxcz_to_rgb(unsigned char *rgb, unsigned char *ycxcz, unsigned int width, unsigned int height)
{
	int compo;
	int y, cx, cz;
	unsigned int row, col;

	/* Cycle on rows */
	for (row = 0; row < height; row++) {

		/* Cycle on columns */
		for (col = 0; col < width; col++) {

			/* Remove offset from YCxCz components */
			y  = (int) *(ycxcz++);
			cx = (int) *(ycxcz++) - 128;
			cz = (int) *(ycxcz++) - 128;

			/* Red component conversion and clipping */
			compo = (350 * y + 1503 * cx - 91 * cz) >> 8;
			*(rgb++) = (unsigned char) ((compo < 0) ? 0: ((compo > 255) ? 255: compo));

			/* Green component conversion and clipping */
			compo = (243 * y - 691 * cx - 146 * cz) >> 8;
			*(rgb++) = (unsigned char)  ((compo < 0) ? 0: ((compo > 255) ? 255: compo));

			/* Blue component conversion and clipping */
			compo = (233 * y + 57 * cx + 498 * cz) >> 8;
			*(rgb++) = (unsigned char) ((compo < 0) ? 0: ((compo > 255) ? 255: compo));
		}
	}
}


/*
 * one selected component->RGB conversion
 *
 *
 */
void whatever_to_rgb(unsigned char *rgb, unsigned char *any, unsigned int width, unsigned int height, int compno)
{
	unsigned int row, col;
	unsigned char a;

	/* Cycle on rows */
	for (row = 0; row < height; row++) {

		/* Cycle on columns */
		for (col = 0; col < width; col++) {

			// Assume inputs
			switch (compno) {

			case (1):
				a = *any;
				any += 3;
				break;

			case (2):
				any += 1;
				a = *any;
				any += 2;
				break;

			case (3):
				any += 2;
				a = *any;
				any += 1;
				break;

			default:
				return;
				break;

			};

			// Assign outputs
			*(rgb++) = a;
			*(rgb++) = a;
			*(rgb++) = a;
		}
	}
}


/*
 * XYZ->RGB conversion
 *
 * D65 white point
 *
 */
void xyzD65_to_rgb(unsigned char *rgb, unsigned char *xyz, unsigned int width, unsigned int height)
{

	int compo;
	int x, y, z;
	unsigned int row, col;

	/* Cycle on rows */
	for (row = 0; row < height; row++) {

		/* Cycle on columns */
		for (col = 0; col < width; col++) {

			/* Expand dynamics */
			x = (int) *(xyz++);
			y = (int) *(xyz++);
			z = (int) *(xyz++);

			/* Red component conversion and clipping */
			compo = (830 * x - 394 * y - 128 * z) >> 8;
			*(rgb++) = (unsigned char) ((compo < 0) ? 0: ((compo > 255) ? 255: compo));

			/* Green component conversion and clipping */
			compo = (-248 * x + 480 * y + 11 * z) >> 8;
			*(rgb++) = (unsigned char)  ((compo < 0) ? 0: ((compo > 255) ? 255: compo));

			/* Blue component conversion and clipping */
			compo = (14 * x - 52 * y + 271 * z) >> 8;
			*(rgb++) = (unsigned char) ((compo < 0) ? 0: ((compo > 255) ? 255: compo));

		}
	}
}


/*
 * HSV->RGB conversion
 *
 *
 */
void hsv_to_rgb(unsigned char *rgb, unsigned char *hsv, unsigned int width, unsigned int height)
{

	/*int compo;*/
	int h, s, v, r, g, b, i, f, p, q, t;
	unsigned int row, col;

	/* Cycle on rows */
	for (row = 0; row < height; row++) {

		/* Cycle on columns */
		for (col = 0; col < width; col++) {

			/* Fix dynamics */
			h = (int) *(hsv++); 
			s = (int) *(hsv++);
			v = (int) *(hsv++);

			if (s == 0) {
				// achromatic (grey)
				r = g = b = v;

			} else {
				i = h / 43; /* sector is between 0 and 5 */
				f = (h - i * 43) * 6; /* fractional part of h */
				p = (v * (255 - s)) >> 8;
				q = (v * (255 - ((s * f) >> 8))) >> 8;
				t = (v * (255 - ((s * (255 - f)) >> 8))) >> 8;
	
				switch( i ) {
				case 0:
					r = v;
					g = t;
					b = p;
					break;
				case 1:
					r = q;
					g = v;
					b = p;
					break;
				case 2:
					r = p;
					g = v;
					b = t;
					break;
				case 3:
					r = p;
					g = q;
					b = v;
					break;
				case 4:
					r = t;
					g = p;
					b = v;
					break;
				default: /* case 5 */
					r = v;
					g = p;
					b = q;
					break;
				};
	
			};

			/* assign values */
			*(rgb++) = r;
			*(rgb++) = g;
			*(rgb++) = b;
	
		}
	}
}

/*
 * RGB->RGB conversion
 */
void
rgb_to_rgb(unsigned char *rgbout, unsigned char *rgbin, unsigned int width, unsigned int height)
{
	unsigned int row, col;

	/* Cycle on rows */
	for (row = 0; row < height; row++) {

		/* Cycle on columns */
		for (col = 0; col < width; col++) {

			*(rgbout++) = *(rgbin++);

		}
	}
}

/*
 * B/W->RGB conversion
 */
void bw_to_rgb(unsigned char *rgb, unsigned char *y, unsigned int width, unsigned int height)
{

	unsigned int row, col;

	/* Cycle on rows */
	for (row = 0; row < height; row++) {

		/* Cycle on columns */
		for (col = 0; col < width; col++) {

			*(rgb++) = *(y++);
			*(rgb++) = *(y++);
			*(rgb++) = *(y++);

		}
	}
}

/*
 * Resampling functions
 */

/*
 * Downsample an image by a scale factor
 *
 */
void downsample(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height, unsigned int widths, unsigned int heights, unsigned int scale)
{
	unsigned int row, col/*, i*/;
	unsigned char *origrow;
	unsigned int colstep = 3 * (scale - 1) + 1;
	unsigned int rowstep = 3 * width * (scale);
	/*unsigned int subR, subG, subB;*/

	/* Cycle on rows */
	origrow = orig;
	for (row = 0; row < heights; row++) {

		/* Cycle on columns */
		orig = origrow;
		for (col = 0; col < widths; col++) {
			
			// copy values
			*(dest++) = *(orig++);
			*(dest++) = *(orig++);
			*(dest++) = *(orig);
			
			// next good column
			orig += colstep;

		}

		// next good row
		origrow += rowstep;

	}
}

/*
 * Stretch for downsample anamorphic
 */
void downsample_anamorphic(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height, unsigned int widths, unsigned int heights, unsigned int scale)
{
	unsigned int row, col/*, i*/;
	unsigned char *origrow;
	unsigned int colstep = 3 * (scale - 1) + 1;
	unsigned int rowstep = 3 * width * (scale);
	/*unsigned int subR, subG, subB;*/

	/* Cycle on rows */
	origrow = orig;
	for (row = 0; row < heights; row++) {

		/* Cycle on columns */
		orig = origrow;
		for (col = 0; col < widths; col++) {

			// copy values
			*(dest++) = *(orig++);
			*(dest++) = *(orig++);
			*(dest++) = *(orig);
			
			// next good column
			orig += colstep;

		}

		// next good row
		origrow += rowstep;

	}
}

#ifndef ui
#define	ui	unsigned int
#define	uc	unsigned char

/* Upsample */
void upsample(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height, unsigned int widths, unsigned int heights, unsigned int scale)
{
	unsigned int row, col, /*i,*/ j;
	unsigned int rowstep = 3 * width, width1 = width - 1;

	/* Cycle on rows */
	for (row = 0; row < height; row++) {

		for (j = 0; j < scale; j++) {

			/* Cycle on columns */
			for (col = 0; col < width1; col++) {
				
				// double scale: interp
				dest[0] = orig[0];
				dest[1] = orig[1];
				dest[2] = orig[2];
				dest[3] = (uc) (((ui) 128 * (ui) orig[0] + (ui) 128 * (ui) orig[3]) >> 8);
				dest[4] = (uc) (((ui) 128 * (ui) orig[1] + (ui) 128 * (ui) orig[4]) >> 8);
				dest[5] = (uc) (((ui) 128 * (ui) orig[2] + (ui) 128 * (ui) orig[5]) >> 8);

				// next good column
				dest += 2 * 3;
				orig += 1 * 3;

			}

			/* remaining columns */
			for (col = width1; col < width; col++) {
				
				// double scale: interp
				dest[0] = orig[0];
				dest[1] = orig[1];
				dest[2] = orig[2];
				dest[3] = orig[0];
				dest[4] = orig[1];
				dest[5] = orig[2];

				// next good column
				dest += 2 * 3;
				orig += 1 * 3;

			}

			// next good row
			orig -= rowstep;
		}
		
		// good
		orig += rowstep;
	}
}

/*
 * Stretch for upsample anamorphic
 */
void upsample_anamorphic(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height,
						 unsigned int widths, unsigned int heights, unsigned int scale)
{
	unsigned int row, col, /*i,*/ j/*, k*/;
	unsigned int rowstep = 3 * widths;
	unsigned char *destrow;
	unsigned int width9 = 9 * (unsigned int) floor((float) width / 9.0F);

	/* Cycle on rows */
	destrow = dest;
	for (row = 0; row < height; row++) {

		/* Cycle on columns */
		dest = destrow;
		for (col = 0; col < width9; col += 9) {
			
			// intersect values
			// matlab generated this!
			dest[0] = orig[0];
			dest[1] = orig[1];
			dest[2] = orig[2];
			dest[3] = (uc) (((ui) 199 * (ui) orig[0] + (ui) 57 * (ui) orig[3]) >> 8);
			dest[4] = (uc) (((ui) 199 * (ui) orig[1] + (ui) 57 * (ui) orig[4]) >> 8);
			dest[5] = (uc) (((ui) 199 * (ui) orig[2] + (ui) 57 * (ui) orig[5]) >> 8);
			dest[6] = orig[3];
			dest[7] = orig[4];
			dest[8] = orig[5];
			dest[9] = (uc) (((ui) 142 * (ui) orig[3] + (ui) 114 * (ui) orig[6]) >> 8);
			dest[10] = (uc) (((ui) 142 * (ui) orig[4] + (ui) 114 * (ui) orig[7]) >> 8);
			dest[11] = (uc) (((ui) 142 * (ui) orig[5] + (ui) 114 * (ui) orig[8]) >> 8);
			dest[12] = orig[6];
			dest[13] = orig[7];
			dest[14] = orig[8];
			dest[15] = (uc) (((ui) 85 * (ui) orig[6] + (ui) 171 * (ui) orig[9]) >> 8);
			dest[16] = (uc) (((ui) 85 * (ui) orig[7] + (ui) 171 * (ui) orig[10]) >> 8);
			dest[17] = (uc) (((ui) 85 * (ui) orig[8] + (ui) 171 * (ui) orig[11]) >> 8);
			dest[18] = orig[9];
			dest[19] = orig[10];
			dest[20] = orig[11];
			dest[21] = (uc) (((ui) 28 * (ui) orig[9] + (ui) 228 * (ui) orig[12]) >> 8);
			dest[22] = (uc) (((ui) 28 * (ui) orig[10] + (ui) 228 * (ui) orig[13]) >> 8);
			dest[23] = (uc) (((ui) 28 * (ui) orig[11] + (ui) 228 * (ui) orig[14]) >> 8);
			dest[24] = (uc) (((ui) 228 * (ui) orig[12] + (ui) 28 * (ui) orig[15]) >> 8);
			dest[25] = (uc) (((ui) 228 * (ui) orig[13] + (ui) 28 * (ui) orig[16]) >> 8);
			dest[26] = (uc) (((ui) 228 * (ui) orig[14] + (ui) 28 * (ui) orig[17]) >> 8);
			dest[27] = orig[15];
			dest[28] = orig[16];
			dest[29] = orig[17];
			dest[30] = (uc) (((ui) 171 * (ui) orig[15] + (ui) 85 * (ui) orig[18]) >> 8);
			dest[31] = (uc) (((ui) 171 * (ui) orig[16] + (ui) 85 * (ui) orig[19]) >> 8);
			dest[32] = (uc) (((ui) 171 * (ui) orig[17] + (ui) 85 * (ui) orig[20]) >> 8);
			dest[33] = orig[18];
			dest[34] = orig[19];
			dest[35] = orig[20];
			dest[36] = (uc) (((ui) 114 * (ui) orig[18] + (ui) 142 * (ui) orig[21]) >> 8);
			dest[37] = (uc) (((ui) 114 * (ui) orig[19] + (ui) 142 * (ui) orig[22]) >> 8);
			dest[38] = (uc) (((ui) 114 * (ui) orig[20] + (ui) 142 * (ui) orig[23]) >> 8);
			dest[39] = orig[21];
			dest[40] = orig[22];
			dest[41] = orig[23];
			dest[42] = (uc) (((ui) 57 * (ui) orig[21] + (ui) 199 * (ui) orig[24]) >> 8);
			dest[43] = (uc) (((ui) 57 * (ui) orig[22] + (ui) 199 * (ui) orig[25]) >> 8);
			dest[44] = (uc) (((ui) 57 * (ui) orig[23] + (ui) 199 * (ui) orig[26]) >> 8);
			dest[45] = orig[24];
			dest[46] = orig[25];
			dest[47] = orig[26];

			// advance dest of 16
			dest += 16 * 3 ;

			// advance orig of 9
			orig += 9 * 3;

		}

		// we don't scale the final pixels
		for (j = width9; j < width; j++) {
			*(dest++) = *(orig++);
			*(dest++) = *(orig++);
			*(dest++) = *(orig++);
		}

		// advance row
		destrow += rowstep;
	}
}

#undef ui
#else
#error Could not define "ui"
#endif

/*
 * 4:0:0 planar to 1:1:1 interleaved conversion
 */
void p400_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height)
{
	unsigned int row, col;

	/* Cycle on rows */
	for (row = 0; row < height; row++) {

		/* Cycle on columns */
		for (col = 0; col < width; col++) {

			*(dest++) = *(orig);
			*(dest++) = *(orig);
			*(dest++) = *(orig++);
		}
	}
}

/*
 * 4:1:0 planar to 1:1:1 interleaved conversion, version 2
 */
void
p410_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height)
{
	unsigned int width4 = width >> 2;
	unsigned int height2 = height >> 1;
	unsigned int area = width * height;
	unsigned int row, col;

	/* Pointers to planes */
	unsigned char *orig1 = orig;
	unsigned char *orig2 = orig + area;
	unsigned char *orig3 = orig2 + (area >> 3);

	/* Cycle on rows */
	for (row = 0; row < height2; row++) {

		/* Even upsampled row */
		/* Cycle on columns */
		for (col = 0; col < width4; col++) {

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *(orig1++);

			/* Component 2 @ QuartHoriz-HalfVert */
			*(dest++) = *orig2;

			/* Component 3 @ QuartHoriz-HalfVert */
			*(dest++) = *orig3;

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *(orig1++);

			/* Component 2 @ QuartHoriz-HalfVert */
			*(dest++) = *orig2;

			/* Component 3 @ QuartHoriz-HalfVert */
			*(dest++) = *orig3;

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *(orig1++);

			/* Component 2 @ QuartHoriz-HalfVert */
			*(dest++) = *orig2;

			/* Component 3 @ QuartHoriz-HalfVert */
			*(dest++) = *orig3;

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *(orig1++);

			/* Component 2 @ QuartHoriz-HalfVert */
			*(dest++) = *(orig2++);

			/* Component 3 @ QuartHoriz-HalfVert */
			*(dest++) = *(orig3++);
		};

		/* Rewind one row in vertically half-sampled compos */
		orig2 -= width4;
		orig3 -= width4;

		/* Odd upsampled row */
		/* Cycle on columns */
		for (col = 0; col < width4; col++) {

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *(orig1++);

			/* Component 2 @ QuartHoriz-HalfVert */
			*(dest++) = *orig2;

			/* Component 3 @ QuartHoriz-HalfVert */
			*(dest++) = *orig3;

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *(orig1++);

			/* Component 2 @ QuartHoriz-HalfVert */
			*(dest++) = *orig2;

			/* Component 3 @ QuartHoriz-HalfVert */
			*(dest++) = *orig3;

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *(orig1++);

			/* Component 2 @ QuartHoriz-HalfVert */
			*(dest++) = *orig2;

			/* Component 3 @ QuartHoriz-HalfVert */
			*(dest++) = *orig3;

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *(orig1++);

			/* Component 2 @ QuartHoriz-HalfVert */
			*(dest++) = *(orig2++);

			/* Component 3 @ QuartHoriz-HalfVert */
			*(dest++) = *(orig3++);
		}
	}
}

/*
 * 4:1:1 planar to 1:1:1 interleaved conversion
 */
void
p411_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height)
{
	unsigned int width4 = width >> 2;
	/*unsigned int height4 = height >> 1;*/
	unsigned int area = width * height;
	unsigned int row, col;

	/* Pointers to planes */
	unsigned char *orig1 = orig;
	unsigned char *orig2 = orig + area;
	unsigned char *orig3 = orig2 + (area >> 2);

	/* Cycle on rows */
	for (row = 0; row < height; row++) {

		/* Cycle on columns */
		for (col = 0; col < width4; col++) {

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *(orig1++);

			/* Component 2 @ QuartHoriz-HalfVert */
			*(dest++) = *orig2;

			/* Component 3 @ QuartHoriz-HalfVert */
			*(dest++) = *orig3;

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *(orig1++);

			/* Component 2 @ QuartHoriz-HalfVert */
			*(dest++) = *orig2;

			/* Component 3 @ QuartHoriz-HalfVert */
			*(dest++) = *orig3;

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *(orig1++);

			/* Component 2 @ QuartHoriz-HalfVert */
			*(dest++) = *orig2;

			/* Component 3 @ QuartHoriz-HalfVert */
			*(dest++) = *orig3;

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *(orig1++);

			/* Component 2 @ QuartHoriz-HalfVert */
			*(dest++) = *(orig2++);

			/* Component 3 @ QuartHoriz-HalfVert */
			*(dest++) = *(orig3++);
		}
	}
}

/*
 * 4:2:0 planar to 1:1:1 interleaved conversion, version 2
 */
void
p420_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height)
{
	unsigned int width2 = width >> 1;
	unsigned int height2 = height >> 1;
	unsigned int area = width * height;
	unsigned int row, col;

	/* Pointers to planes */
	unsigned char *orig1 = orig;
	unsigned char *orig2 = orig + area;
	unsigned char *orig3 = orig2 + (area >> 2);

	/* Cycle on rows */
	for (row = 0; row < height2; row++) {

		/* Even upsampled row */
		/* Cycle on columns */
		for (col = 0; col < width2; col++) {

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *(orig1++);

			/* Component 2 @ HalfHoriz-HalfVert */
			*(dest++) = *orig2;

			/* Component 3 @ HalfHoriz-HalfVert */
			*(dest++) = *orig3;

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *(orig1++);

			/* Component 2 @ HalfHoriz-HalfVert */
			*(dest++) = *(orig2++);

			/* Component 3 @ HalfHoriz-HalfVert */
			*(dest++) = *(orig3++);
		};

		/* Rewind one row in vertically half-sampled compos */
		orig2 -= width2;
		orig3 -= width2;

		/* Odd upsampled row */
		/* Cycle on columns */
		for (col = 0; col < width2; col++) {

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *(orig1++);

			/* Component 2 @ HalfHoriz-HalfVert */
			*(dest++) = *orig2;

			/* Component 3 @ HalfHoriz-HalfVert */
			*(dest++) = *orig3;

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *(orig1++);

			/* Component 2 @ HalfHoriz-HalfVert */
			*(dest++) = *(orig2++);

			/* Component 3 @ HalfHoriz-HalfVert */
			*(dest++) = *(orig3++);
		}
	}
}

/*
 * 4:2:2 planar to 1:1:1 interleaved conversion
 */
void
p422_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height)
{
	unsigned int width2 = width >> 1;
	/*unsigned int height2 = height >> 1;*/
	unsigned int area = width * height;
	unsigned int row, col;

	/* Pointers to planes */
	unsigned char *orig1 = orig;
	unsigned char *orig2 = orig + area;
	unsigned char *orig3 = orig2 + (area >> 1);

	/* Cycle on rows */
	for (row = 0; row < height; row++) {

		/* Cycle on columns */
		for (col = 0; col < width2; col++) {

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *(orig1++);

			/* Component 2 @ HalfHoriz-HalfVert */
			*(dest++) = *orig2;

			/* Component 3 @ HalfHoriz-HalfVert */
			*(dest++) = *orig3;

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *(orig1++);

			/* Component 2 @ HalfHoriz-HalfVert */
			*(dest++) = *(orig2++);

			/* Component 3 @ HalfHoriz-HalfVert */
			*(dest++) = *(orig3++);
		}
	}
}

/*
 * 4:4:4 planar to 1:1:1 interleaved conversion
 */
void
p444_123_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height)
{
	unsigned int area = width * height;
	unsigned int row, col;

	/* Pointers to planes */
	unsigned char *orig1 = orig;
	unsigned char *orig2 = orig + area;
	unsigned char *orig3 = orig2 + area;

	/* Cycle on rows */
	for (row = 0; row < height; row++) {

		/* Cycle on columns */
		for (col = 0; col < width; col++) {

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *(orig1++);

			/* Component 2 @ FullHoriz-FullVert */
			*(dest++) = *(orig2++);

			/* Component 3 @ FullHoriz-FullVert */
			*(dest++) = *(orig3++);
		}
	}
}

void
p444_321_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height)
{
	unsigned int area = width * height;
	unsigned int row, col;

	/* Pointers to planes */
	unsigned char *orig3 = orig;
	unsigned char *orig2 = orig + area;
	unsigned char *orig1 = orig2 + area;

	/* Cycle on rows */
	for (row = 0; row < height; row++) {

		/* Cycle on columns */
		for (col = 0; col < width; col++) {

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *(orig1++);

			/* Component 2 @ FullHoriz-FullVert */
			*(dest++) = *(orig2++);

			/* Component 3 @ FullHoriz-FullVert */
			*(dest++) = *(orig3++);
		}
	}
}

/*
 * 4:4:4 planar interlaced to 1:1:1 interleaved conversion
 */
void
p444i_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height)
{
	unsigned int height2 = height / 2, area = width * height2;
	unsigned int row, col;

	/* Pointers to planes */
	unsigned char *orig1 = orig;
	unsigned char *orig2 = orig + area;
	unsigned char *orig3 = orig2 + area;

	/* Even/odd field counter */
	static unsigned int evenfield = 0;

	/* Prepare destination interlacing offset */
	if (!evenfield)
		dest += width * 3;
	evenfield = !evenfield;

	/* Cycle on rows */
	for (row = 0; row < height2; row++) {

		/* Cycle on columns */
		for (col = 0; col < width; col++) {

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *(orig1++);

			/* Component 2 @ FullHoriz-FullVert */
			*(dest++) = *(orig2++);

			/* Component 3 @ FullHoriz-FullVert */
			*(dest++) = *(orig3++);
		}

		/* Skip one destination col */
		dest += width * 3;
	}
}

/*
 * 4:4:4 interleaved to 1:1:1 interleaved conversion
 */
void i444_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height)
{
	unsigned int area3 = width * height * 3;

	/* Direct copy between buffers */
	memcpy(dest, orig, area3);
}

/*
 * 4:2:2 interleaved to 1:1:1 interleaved conversion
 */
void i422_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height)
{
	/*unsigned int area = width * height;*/
	unsigned int width2 = width >> 1;
	unsigned int row, col;

	unsigned char comp2, comp3;

	/* Pointers to compos */
	unsigned char *orig1 = orig;
	unsigned char *orig2 = orig1 + 1;
	unsigned char *orig3 = orig1 + 3;

	/* Cycle on rows */
	for (row = 0; row < height; row++) {

		/* Cycle on columns */
		for (col = 0; col < width2; col++) {

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *orig1;
			orig1 += 2;

			/* Component 2 @ HalfHoriz-FullVert */
			comp2 = *(dest++) = *orig2;
			orig2 += 4;

			/* Component 3 @ HalfHoriz-FullVert */
			comp3 = *(dest++) = *orig3;
			orig3 += 4;

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *orig1;
			orig1 += 2;

			/* Component 2 @ HalfHoriz-FullVert */
			*(dest++) = comp2;

			/* Component 3 @ HalfHoriz-FullVert */
			*(dest++) = comp3;
		}
	}
}

/*
 * 4:1:1 interleaved to 1:1:1 interleaved conversion
 */
void i411_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height)
{
	/*unsigned int area = width * height;*/
	unsigned int width4 = width >> 2;
	unsigned int row, col;

	unsigned char comp2, comp3;

	/* Pointers to compos */
	unsigned char *orig1 = orig;
	unsigned char *orig2 = orig1 + 1;
	unsigned char *orig3 = orig1 + 4;

	/* Cycle on rows */
	for (row = 0; row < height; row++) {

		/* Cycle on columns */
		for (col = 0; col < width4; col++) {

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *orig1;
			orig1 += 2;

			/* Component 2 @ QuartHoriz-FullVert */
			comp2 = *(dest++) = *orig2;
			orig2 += 6;

			/* Component 3 @ QuartHoriz-FullVert */
			comp3 = *(dest++) = *orig3;
			orig3 += 6;

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *(orig1++);

			/* Component 2 @ QuartHoriz-FullVert */
			*(dest++) = comp2;

			/* Component 3 @ QuartHoriz-FullVert */
			*(dest++) = comp3;

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *orig1;
			orig1 += 2;

			/* Component 2 @ QuartHoriz-FullVert */
			*(dest++) = comp2;

			/* Component 3 @ QuartHoriz-FullVert */
			*(dest++) = comp3;

			/* Component 1 @ FullHoriz-FullVert */
			*(dest++) = *(orig1++);

			/* Component 2 @ QuartHoriz-FullVert */
			*(dest++) = comp2;

			/* Component 3 @ QuartHoriz-FullVert */
			*(dest++) = comp3;
		}
	}
}

/*
 * 4:2:0 interleaved to 1:1:1 interleaved conversion
 */
void i420_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height)
{
	/*unsigned int area = width * height;*/
	unsigned int width2 = width >> 1;
	unsigned int height2 = height >> 1;
	unsigned int width3 = 3 * width;
	unsigned int width32 = width + (width >> 1);
	unsigned int row, col;

	unsigned char comp2, comp3;

	/* Pointers to compos */
	unsigned char *orige = orig;
	unsigned char *origo = orig + width32;

	/* Pointers to odd and even rows */
	unsigned char *deste = dest;
	unsigned char *desto = dest + width3;

	/* Cycle on rows */
	for (row = 0; row < height2; row++) {

		/* Cycle on columns */
		for (col = 0; col < width2; col++) {

			/* Component 1e @ FullHoriz-FullVert */
			*(deste++) = *(orige++);

			/* Component 1o @ FullHoriz-FullVert */
			*(desto++) = *(origo++);

			/* Component 2eo @ HalfHoriz-HalfVert */
			comp2 = *(desto++) = *(deste++) = *(orige++);

			/* Component 3oe @ HalfHoriz-HalfVert */
			comp3 = *(deste++) = *(desto++) = *(origo++);

			/* Component 1 @ FullHoriz-FullVert */
			*(deste++) = *(orige++);

			/* Component 1o @ FullHoriz-FullVert */
			*(desto++) = *(origo++);

			/* Component 2eo @ HalfHoriz-HalfVert */
			*(desto++) = *(deste++) = comp2;
			
			/* Component 3oe @ HalfHoriz-HalfVert */
			*(deste++) = *(desto++) = comp3;

		}

		/* Update to next rows */
		deste += width3;
		desto += width3;
		orige += width32;
		origo += width32;
	}
}

/*
 * 4:1:0 interleaved to 1:1:1 interleaved conversion
 */
void i410_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height)
{
	/*unsigned int area = width * height;*/
	unsigned int width4 = width >> 2;
	unsigned int height2 = height >> 1;
	unsigned int width3 = 3 * width;
	unsigned int width54 = width + (width >> 2);
	unsigned int row, col;

	unsigned char comp2, comp3;

	/* Pointers to compos */
	unsigned char *orige = orig;
	unsigned char *origo = orig + width54;

	/* Pointers to odd and even rows */
	unsigned char *deste = dest;
	unsigned char *desto = dest + width3;

	/* Cycle on rows */
	for (row = 0; row < height2; row++) {

		/* Cycle on columns */
		for (col = 0; col < width4; col++) {

			/* Component 1e @ FullHoriz-FullVert */
			*(deste++) = *(orige++);

			/* Component 1o @ FullHoriz-FullVert */
			*(desto++) = *(origo++);

			/* Component 2eo @ HalfHoriz-HalfVert */
			comp2 = *(desto++) = *(deste++) = *(orige++);

			/* Component 3oe @ HalfHoriz-HalfVert */
			comp3 = *(deste++) = *(desto++) = *(origo++);

			/* Component 1 @ FullHoriz-FullVert */
			*(deste++) = *(orige++);

			/* Component 1o @ FullHoriz-FullVert */
			*(desto++) = *(origo++);

			/* Component 2eo @ HalfHoriz-HalfVert */
			*(desto++) = *(deste++) = comp2;
			
			/* Component 3oe @ HalfHoriz-HalfVert */
			*(deste++) = *(desto++) = comp3;

			/* Component 1 @ FullHoriz-FullVert */
			*(deste++) = *(orige++);

			/* Component 1o @ FullHoriz-FullVert */
			*(desto++) = *(origo++);

			/* Component 2eo @ HalfHoriz-HalfVert */
			*(desto++) = *(deste++) = comp2;
			
			/* Component 3oe @ HalfHoriz-HalfVert */
			*(deste++) = *(desto++) = comp3;

			/* Component 1 @ FullHoriz-FullVert */
			*(deste++) = *(orige++);

			/* Component 1o @ FullHoriz-FullVert */
			*(desto++) = *(origo++);

			/* Component 2eo @ HalfHoriz-HalfVert */
			*(desto++) = *(deste++) = comp2;
			
			/* Component 3oe @ HalfHoriz-HalfVert */
			*(deste++) = *(desto++) = comp3;

		}

		/* Update to next rows */
		deste += width3;
		desto += width3;
		orige += width54;
		origo += width54;
	}
}


/*
 * Deinterlace video
 *
 */
void
deinterlace(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height)
{
	/*int compo;*/
	/*int y1, cb1, cr1;*/
	unsigned int row, col, height2 = height / 2;
	unsigned int width3 = 3 * width;

	/* Cycle on rows */
	for (row = 0; row < height2; row++) {

		/* Cycle on columns */
		for (col = 0; col < width3; col++) {

			/* Update destinations */
			/* Average even and odd fields */
			*(dest) = *(dest + width3) = (unsigned char) (((*orig) + (*(orig + width3))) >> 1);

			/* Update orig and dest */
			dest++;
			orig++;
		};

		/* jump one row */
		dest += width3;
		orig += width3;
	}
}


/*
 * 4:2:2 UYVY interleaved to 1:1:1 interleaved conversion
 */
void iuyvy422_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height)
{
	/*unsigned int area = width * height;*/
	unsigned int width2 = width >> 1;
	/*unsigned int height2 = height >> 1;*/
	/*unsigned int height1 = height - 80;*/
	unsigned int row, col;

	/* Cycle on rows */
 	for (row = 0; row < height; row++) {

		/* Cycle on columns */
		for (col = 0; col < width2; col++) {

			/* Map UYVY into YUVs */
			dest[0] = orig[1];
			dest[1] = orig[0];
			dest[2] = orig[2];

			dest[3] = orig[3];
			dest[4] = orig[0];
			dest[5] = orig[2];

			/* Update to next couple cols */
			dest += 3*2;
			orig += 4;

		}
	}
}

/*
 * 4:2:2 YUY2 interleaved to 1:1:1 interleaved conversion
 */
void iyuy2422_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height)
{
	/*unsigned int area = width * height;*/
	unsigned int width2 = width >> 1;
	/*unsigned int height2 = height >> 1;*/
	/*unsigned int height1 = height - 80;*/
	unsigned int row, col;

	/* Cycle on rows */
	for (row = 0; row < height; row++) {

		/* Cycle on columns */
		for (col = 0; col < width2; col++) {

			/* Map UYVY into YUVs */
			dest[0] = orig[0];
			dest[1] = orig[1];
			dest[2] = orig[3];

			dest[3] = orig[2];
			dest[4] = orig[1];
			dest[5] = orig[3];

			/* Update to next couple cols */
			dest += 3*2;
			orig += 4;
		}
	}
}

/*
 * 4:2:2 YVYU interleaved to 1:1:1 interleaved conversion
 */
void iyvyu422_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height)
{
	/*unsigned int area = width * height;*/
	unsigned int width2 = width >> 1;
	/*unsigned int height2 = height >> 1;*/
	/*unsigned int height1 = height - 80;*/
	unsigned int row, col;

	/* Cycle on rows */
	for (row = 0; row < height; row++) {

		/* Cycle on columns */
		for (col = 0; col < width2; col++) {

			/* Map UYVY into YUVs */
			dest[0] = orig[0];
			dest[1] = orig[3];
			dest[2] = orig[1];

			dest[3] = orig[2];
			dest[4] = orig[3];
			dest[5] = orig[1];

			/* Update to next couple cols */
			dest += 3*2;
			orig += 4;
		}
	}
}


void
prepare_screen(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height)
{
	int n, area = width * height * 3;

	for (n = 0; n < area; n++)
#if 0
		*(dest++) = screenlut[*dest];
#else
		*dest = screenlut[*dest];
		dest++;
#endif
}
