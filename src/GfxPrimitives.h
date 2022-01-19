#ifndef GFX_PRIMITIVES_H
#define GFX_PRIMITIVES_H

#include <gint/defs/util.h>

// to be added to <gint/defs/util.h>
// another usefull function to be added to common utilities
#define	abs(s) ({			\
	GAUTOTYPE _s = (s);		\
	_s < 0 ? (-1)*_s :	_s;				\
})


#define R_MASK 0b1111100000000000
#define G_MASK 0b0000011111100000
#define B_MASK 0b0000000000011111

/*
    Some usefull conversion routines for manipulating color models
*/

void GfxRGBtoColor( uint8_t R, uint8_t G, uint8_t B, uint16_t *Color );
void GfxColortoRGB( uint16_t Color, uint8_t *R, uint8_t *G, uint8_t *B );
uint16_t GfxColor( uint8_t R, uint8_t G, uint8_t B );

/*
    Internal routines for mixing/blending colors needed by function with Alpha channel
*/

uint16_t alphaBlendRGB565( uint16_t fg, uint16_t bg, uint8_t alpha );

/*
    Drawing routines prototypes using the following convention

    GfxPrototypeColor : base function, using coordinates and Color as a direct RGB565 uint16_t value
    GfxPrototypeColor_Fast : same but with no clipping check (to be used only when you are certain that the whole geometry is contained in the screen
    GfxPrototypeColorA : same but with additional alpha/transparency channel to blend color with current background
    GfxPrototypeColorA_Fast : same with no clipping check

    GfxPrototypeRGB : base function, using coordinates and 3 uint8_t values for individual R, G, B channels
    GfxPrototypeRGB_Fast : same but with no clipping check (to be used only when you are certain that the whole geometry is contained in the screen
    GfxPrototypeRGBA : same but with additional alpha/transparency channel to blend color with current background
    GfxPrototypeRGBA_Fast : same with no clipping check
*/

void GfxGetPixelColor( uint16_t X, uint16_t Y, uint16_t *Color );
void GfxGetPixelColorFast( uint16_t X, uint16_t Y, uint16_t *Color );
void GfxGetPixelRGB( uint16_t X, uint16_t Y, uint8_t *R, uint8_t *G, uint8_t *B );
void GfxGetPixelRGBFast( uint16_t X, uint16_t Y, uint8_t *R, uint8_t *G, uint8_t *B );


void GfxPixelColor( uint16_t X, uint16_t Y, uint16_t Color );
void GfxPixelColorFast( uint16_t X, uint16_t Y, uint16_t Color );
void GfxPixelRGB( uint16_t X, uint16_t Y, uint8_t R, uint8_t G, uint8_t B );
void GfxPixelRGBFast( uint16_t X, uint16_t Y, uint8_t R, uint8_t G, uint8_t B );

void GfxPixelColorA( uint16_t X, uint16_t Y, uint16_t Color, uint8_t Alpha );
void GfxPixelColorAFast( uint16_t X, uint16_t Y, uint16_t Color, uint8_t Alpha );
void GfxPixelRGBA( uint16_t X, uint16_t Y, uint8_t R, uint8_t G, uint8_t B, uint8_t Alpha );
void GfxPixelRGBAFast( uint16_t X, uint16_t Y, uint8_t R, uint8_t G, uint8_t B, uint8_t Alpha );

typedef
 union
  {
   int32_t i;
   struct
    {
     int16_t hi;
     int16_t lo;
    };
  } fixed_point;

void GfxQuickLineColor( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color );

void GfxLineColor( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color );
void GfxLineColorA( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color, uint8_t alpha );
void GfxLineRGB( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t R, uint8_t G, uint8_t B );
void GfxLineRGBA( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t R, uint8_t G, uint8_t B, uint8_t alpha );


void GfxCircleColor(uint16_t centerX, uint16_t centerY, uint16_t radius, uint16_t color );
void GfxCircleColorA(uint16_t centerX, uint16_t centerY, uint16_t radius, uint16_t color, uint8_t alpha );
void GfxCircleRGB(uint16_t centerX, uint16_t centerY, uint16_t radius, uint8_t R, uint8_t G, uint8_t B );
void GfxCircleRGBA(uint16_t centerX, uint16_t centerY, uint16_t radius, uint8_t R, uint8_t G, uint8_t B, uint8_t alpha );


#endif // GFX_PRIMITIVES_H
