/*
 * Colorspace conversion functions
 * They accept as input an interleaved buffer with
 * three original components, and produce an interleaved buffer
 * with the destination components
 */

/*
 *
 * Strange functions
 *
 */

/*
 * compute the difference between actual and previous frame
 */
void difference(uint8_t *diff, uint8_t *orig, uint32_t width, uint32_t height);

/*
 * draw a grid on the frame
 */
void drawgrid(uint8_t *dest, uint8_t *orig, uint32_t width, uint32_t height, uint32_t gridsize, uint32_t gridcol);

/*
 * Bit depth conversion
 *
 *
 *
 */
void bitdepth_adjust(uint8_t *dest, void *orig, uint32_t numsamples, int bits, int sign, int endian);

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
void ycbcr601_to_rgb(uint8_t *rgb, uint8_t *ycbcr, uint32_t width, uint32_t height);

/*
 * YCbCr->RGB conversion
 *
 * Rec. BT.709
 *
 */
void ycbcr709_to_rgb(uint8_t *rgb, uint8_t *ycbcr, uint32_t width, uint32_t height);

/*
 * XYZ->RGB conversion
 *
 * D65 white point
 *
 */
void xyzD65_to_rgb(uint8_t *rgb, uint8_t *xyz, uint32_t width, uint32_t height);

/*
 * HSV->RGB conversion
 *
 *
 */
void hsv_to_rgb(uint8_t *rgb, uint8_t *hsv, uint32_t width, uint32_t height);

/*
 * YIQ->RGB conversion
 *
 *
 */
void yiq_to_rgb(uint8_t *rgb, uint8_t *yiq, uint32_t width, uint32_t height);

/*
 * YCoCg->RGB conversion
 *
 *
 */
void ycocg_to_rgb(uint8_t *rgb, uint8_t *ycocg, uint32_t width, uint32_t height);

/*
 * YCxCz->RGB conversion
 *
 *
 */
void ycxcz_to_rgb(uint8_t *rgb, uint8_t *ycxcz, uint32_t width, uint32_t height);

/*
 * one selected component->RGB conversion
 *
 *
 */
void whatever_to_rgb(uint8_t *rgb, uint8_t *any, uint32_t width, uint32_t height, int compno);

/*
 * RGB->RGB conversion
 */
void rgb_to_rgb(uint8_t *rgb, uint8_t *ycbcr, uint32_t width, uint32_t height);

/*
 * B/W->RGB conversion
 */
void bw_to_rgb(uint8_t *rgb, uint8_t *y, uint32_t width, uint32_t height);


/*
 * Resampling functions
 */

/*
 * Resample an image by a scale factor
 *
 */
void downsample(uint8_t *dest, uint8_t *orig, uint32_t width, uint32_t height, uint32_t widths, uint32_t heights, uint32_t scale);
void downsample_anamorphic(uint8_t *dest, uint8_t *orig, uint32_t width, uint32_t height, uint32_t widths, uint32_t heights, uint32_t scale);
void upsample(uint8_t *dest, uint8_t *orig, uint32_t width, uint32_t height, uint32_t widths, uint32_t heights, uint32_t scale);
void upsample_anamorphic(uint8_t *dest, uint8_t *orig, uint32_t width, uint32_t height, uint32_t widths, uint32_t heights, uint32_t scale);

/*
 * 4:0:0 planar to 1:1:1 interleaved conversion
 */
void p400_to_i111(uint8_t *dest, uint8_t *orig, uint32_t width, uint32_t height);

/*
 * 4:1:0 planar to 1:1:1 interleaved conversion
 */
void p410_to_i111(uint8_t *dest, uint8_t *orig, uint32_t width, uint32_t height);

/*
 * 4:1:1 planar to 1:1:1 interleaved conversion
 */
void p411_to_i111(uint8_t *dest, uint8_t *orig, uint32_t width, uint32_t height);

/*
 * 4:2:0 planar to 1:1:1 interleaved conversion
 */
void p420_to_i111(uint8_t *dest, uint8_t *orig, uint32_t width, uint32_t height);

/*
 * 4:2:0 planar to 1:1:1 interleaved conversion
 */
void p420_to_i111bis(uint8_t *dest, uint8_t *orig, uint32_t width, uint32_t height);

/*
 * 4:2:2 planar to 1:1:1 interleaved conversion
 */
void p422_to_i111(uint8_t *dest, uint8_t *orig, uint32_t width, uint32_t height);

/*
 * 4:4:4 planar to 1:1:1 interleaved conversion
 */
void p444_123_to_i111(uint8_t *dest, uint8_t *orig, uint32_t width, uint32_t height);
void p444_321_to_i111(uint8_t *dest, uint8_t *orig, uint32_t width, uint32_t height);

/*
 * 4:4:4 planar interlaced to 1:1:1 interleaved conversion
 */
void p444i_to_i111(uint8_t *dest, uint8_t *orig, uint32_t width, uint32_t height);

/*
 * 4:4:4 interleaved to 1:1:1 interleaved conversion
 */
void i444_to_i111(uint8_t *dest, uint8_t *orig, uint32_t width, uint32_t height);

/*
 * 4:2:2 interleaved to 1:1:1 interleaved conversion
 */
void i422_to_i111(uint8_t *dest, uint8_t *orig, uint32_t width, uint32_t height);

/*
 * 4:1:1 interleaved to 1:1:1 interleaved conversion
 */
void i411_to_i111(uint8_t *dest, uint8_t *orig, uint32_t width, uint32_t height);

/*
 * 4:2:0 interleaved to 1:1:1 interleaved conversion
 */
void i420_to_i111(uint8_t *dest, uint8_t *orig, uint32_t width, uint32_t height);

/*
 * 4:1:0 interleaved to 1:1:1 interleaved conversion
 */
void i410_to_i111(uint8_t *dest, uint8_t *orig, uint32_t width, uint32_t height);

/*
 * deinterlace video
 */
void deinterlace(uint8_t *dest, uint8_t *orig, uint32_t width, uint32_t height);

/*
 * 4:2:0 UYVY interleaved to 1:1:1 interleaved conversion
 */
void iuyvy422_to_i111(uint8_t *dest, uint8_t *orig, uint32_t width, uint32_t height);

/*
 * 4:2:2 YUY2 interleaved to 1:1:1 interleaved conversion
 */
void iyuy2422_to_i111(uint8_t *dest, uint8_t *orig, uint32_t width, uint32_t height);

/*
 * 4:2:2 YVYU interleaved to 1:1:1 interleaved conversion
 */
void iyvyu422_to_i111(uint8_t *dest, uint8_t *orig, uint32_t width, uint32_t height);

void prepare_screen(uint8_t *dest, uint8_t *orig, uint32_t width, uint32_t height);
