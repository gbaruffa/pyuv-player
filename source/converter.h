/*
 * Colorspace conversion functions
 * They accept as input an interleaved buffer with
 * three original components, and produce an interleaved buffer
 * with the destination components
 */

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

/*
 *
 * Strange functions
 *
 */

/*
 * compute the difference between actual and previous frame
 */
void difference(unsigned char *diff, unsigned char *orig, unsigned int width, unsigned int height);

/*
 * draw a grid on the frame
 */
void drawgrid(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height,
			  unsigned int gridsize, unsigned int gridcol);

/*
 * Bit depth conversion
 *
 *
 *
 */
void bitdepth_adjust(unsigned char *dest, void *orig, unsigned int numsamples, int bits, int sign, int endian);

/*
 * Prepare the scaling LUT --> faster
 *
 *
 *
 */
void prepare_lut(int bits, int sign, int endian, double gamma);

#define SHORT_SWAP(B) (((B) >> 8) | ((B) << 8))

/*
 * YCbCr->RGB conversion
 *
 * Rec. BT.601
 *
 */
void ycbcr601_to_rgb(unsigned char *rgb, unsigned char *ycbcr, unsigned int width, unsigned int height);

/*
 * YCbCr->RGB conversion
 *
 * Rec. BT.709
 *
 */
void ycbcr709_to_rgb(unsigned char *rgb, unsigned char *ycbcr, unsigned int width, unsigned int height);

/*
 * XYZ->RGB conversion
 *
 * D65 white point
 *
 */
void xyzD65_to_rgb(unsigned char *rgb, unsigned char *xyz, unsigned int width, unsigned int height);

/*
 * HSV->RGB conversion
 *
 *
 */
void hsv_to_rgb(unsigned char *rgb, unsigned char *hsv, unsigned int width, unsigned int height);

/*
 * YIQ->RGB conversion
 *
 *
 */
void yiq_to_rgb(unsigned char *rgb, unsigned char *yiq, unsigned int width, unsigned int height);

/*
 * YCoCg->RGB conversion
 *
 *
 */
void ycocg_to_rgb(unsigned char *rgb, unsigned char *ycocg, unsigned int width, unsigned int height);

/*
 * YCxCz->RGB conversion
 *
 *
 */
void ycxcz_to_rgb(unsigned char *rgb, unsigned char *ycxcz, unsigned int width, unsigned int height);

/*
 * one selected component->RGB conversion
 *
 *
 */
void whatever_to_rgb(unsigned char *rgb, unsigned char *any, unsigned int width, unsigned int height, int compno);

/*
 * RGB->RGB conversion
 */
void rgb_to_rgb(unsigned char *rgb, unsigned char *ycbcr, unsigned int width, unsigned int height);

/*
 * B/W->RGB conversion
 */
void bw_to_rgb(unsigned char *rgb, unsigned char *y, unsigned int width, unsigned int height);


/*
 * Resampling functions
 */

/*
 * Resample an image by a scale factor
 *
 */
void downsample(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height, unsigned int widths, unsigned int heights, unsigned int scale);
void downsample_anamorphic(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height, unsigned int widths, unsigned int heights, unsigned int scale);
void upsample(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height, unsigned int widths, unsigned int heights, unsigned int scale);
void upsample_anamorphic(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height, unsigned int widths, unsigned int heights, unsigned int scale);

/*
 * 4:0:0 planar to 1:1:1 interleaved conversion
 */
void p400_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height);

/*
 * 4:1:0 planar to 1:1:1 interleaved conversion
 */
void p410_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height);

/*
 * 4:1:1 planar to 1:1:1 interleaved conversion
 */
void p411_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height);

/*
 * 4:2:0 planar to 1:1:1 interleaved conversion
 */
void p420_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height);

/*
 * 4:2:0 planar to 1:1:1 interleaved conversion
 */
void p420_to_i111bis(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height);

/*
 * 4:2:2 planar to 1:1:1 interleaved conversion
 */
void p422_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height);

/*
 * 4:4:4 planar to 1:1:1 interleaved conversion
 */
void p444_123_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height);
void p444_321_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height);

/*
 * 4:4:4 planar interlaced to 1:1:1 interleaved conversion
 */
void p444i_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height);

/*
 * 4:4:4 interleaved to 1:1:1 interleaved conversion
 */
void i444_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height);

/*
 * 4:2:2 interleaved to 1:1:1 interleaved conversion
 */
void i422_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height);

/*
 * 4:1:1 interleaved to 1:1:1 interleaved conversion
 */
void i411_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height);

/*
 * 4:2:0 interleaved to 1:1:1 interleaved conversion
 */
void i420_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height);

/*
 * 4:1:0 interleaved to 1:1:1 interleaved conversion
 */
void i410_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height);

/*
 * deinterlace video
 */
void deinterlace(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height);

/*
 * 4:2:0 UYVY interleaved to 1:1:1 interleaved conversion
 */
void iuyvy422_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height);

/*
 * 4:2:2 YUY2 interleaved to 1:1:1 interleaved conversion
 */
void iyuy2422_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height);

/*
 * 4:2:2 YVYU interleaved to 1:1:1 interleaved conversion
 */
void iyvyu422_to_i111(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height);

void prepare_screen(unsigned char *dest, unsigned char *orig, unsigned int width, unsigned int height);
