#include <gint/display.h>
#include "GfxPrimitives.h"

inline void GfxRGBtoColor( uint8_t R, uint8_t G, uint8_t B, uint16_t *Color )
{
       *Color = (uint16_t) ((R >> 3) << 11 | ((G >> 2) << 5) | (B >> 3));
}

inline uint16_t GfxColor( uint8_t R, uint8_t G, uint8_t B )
{
       return (uint16_t) ((R >> 3) << 11 | ((G >> 2) << 5) | (B >> 3));
}

inline void GfxColortoRGB( uint16_t Color, uint8_t *R, uint8_t *G, uint8_t *B )
{
       *R = (uint8_t) (((Color >> 11) & 0b11111) << 3);
       *G = (uint8_t) (((Color >> 5) & 0b111111) << 2);
       *B = (uint8_t) ((Color  & 0b11111) << 3);
}

inline void GfxPixelColor( uint16_t X, uint16_t Y, uint16_t Color )
{
       if (X>=DWIDTH) return;
       if (Y>=DHEIGHT) return;
       gint_vram[ Y*DWIDTH + X ] = Color;
}

inline void GfxPixelColorFast( uint16_t X, uint16_t Y, uint16_t Color )
{
       gint_vram[ Y*DWIDTH + X ] = Color;
}

inline void GfxPixelRGB( uint16_t X, uint16_t Y, uint8_t R, uint8_t G, uint8_t B )
{
       if (X>=DWIDTH) return;
       if (Y>=DHEIGHT) return;
       gint_vram[ Y*DWIDTH + X ] = ((R >> 3) << 11 | ((G >> 2) << 5) | (B >> 3));
}

inline void GfxPixelRGBFast( uint16_t X, uint16_t Y, uint8_t R, uint8_t G, uint8_t B )
{
       gint_vram[ Y*DWIDTH + X ] = ((R >> 3) << 11 | ((G >> 2) << 5) | (B >> 3));
}

void GfxGetPixelColor( uint16_t X, uint16_t Y, uint16_t *Color )
{
       if (X>=DWIDTH) return;
       if (Y>=DHEIGHT) return;
       *Color = gint_vram[ Y*DWIDTH + X ];
}

void GfxGetPixelColorFast( uint16_t X, uint16_t Y, uint16_t *Color )
{
       *Color = gint_vram[ Y*DWIDTH + X ];
}

void GfxGetPixelRGB( uint16_t X, uint16_t Y, uint8_t *R, uint8_t *G, uint8_t *B )
{
       if (X>=DWIDTH) return;
       if (Y>=DHEIGHT) return;
       uint16_t index = Y*DWIDTH + X;
       uint16_t Color =  (uint16_t)  gint_vram[ index ];
       *R = (((Color >> 11) & 0b11111) << 3);
       *G = (((Color >> 5) & 0b111111) << 2);
       *B = ((Color  & 0b11111) << 3);
}

void GfxGetPixelRGBFast( uint16_t X, uint16_t Y, uint8_t *R, uint8_t *G, uint8_t *B )
{
       uint16_t index = Y*DWIDTH + X;
       uint16_t Color =  (uint16_t)  gint_vram[ index ];
       *R = (((Color >> 11) & 0b11111) << 3);
       *G = (((Color >> 5) & 0b111111) << 2);
       *B = ((Color  & 0b11111) << 3);
}

inline uint16_t alphaBlendRGB565( uint16_t Color, uint16_t Background, uint8_t Alpha )
{
       uint32_t fg = (uint32_t) Color;
       uint32_t bg = (uint32_t) Background;
       uint32_t a = (Alpha+4)>>3;
       bg = (bg | (bg <<16)) & 0b00000111111000001111100000011111;
       fg = (fg | (fg <<16)) & 0b00000111111000001111100000011111;
       uint32_t result = ((((fg-bg)*a)>>5) + bg ) & 0b00000111111000001111100000011111;
       return (uint16_t) ((result >> 16) | result);
}

inline void GfxPixelColorA( uint16_t X, uint16_t Y, uint16_t Color, uint8_t Alpha )
{
       if (X>=DWIDTH) return;
       if (Y>=DHEIGHT) return;
       uint16_t bg;
       GfxGetPixelColorFast( X, Y, &bg );
       uint16_t newcolor = alphaBlendRGB565( Color, bg, Alpha );
       GfxPixelColorFast( X, Y, newcolor );
}

inline void GfxPixelColorAFast( uint16_t X, uint16_t Y, uint16_t Color, uint8_t Alpha )
{
       uint16_t bg;
       GfxGetPixelColorFast( X, Y, &bg );
       uint16_t newcolor = alphaBlendRGB565( Color, bg, Alpha );
       GfxPixelColorFast( X, Y, newcolor );
}

inline void GfxPixelRGBA( uint16_t X, uint16_t Y, uint8_t R, uint8_t G, uint8_t B, uint8_t Alpha )
{
       if (X>=DWIDTH) return;
       if (Y>=DHEIGHT) return;
       uint16_t bg, fg;
       GfxGetPixelColorFast( X, Y, &bg );
       GfxRGBtoColor( R, G, B, &fg );
       uint16_t newcolor = alphaBlendRGB565( fg, bg, Alpha );
       GfxPixelColorFast( X, Y, newcolor );
}

inline void GfxPixelRGBAFast( uint16_t X, uint16_t Y, uint8_t R, uint8_t G, uint8_t B, uint8_t Alpha )
{
       uint16_t bg, fg;
       GfxGetPixelColorFast( X, Y, &bg );
       GfxRGBtoColor( R, G, B, &fg );
       uint16_t newcolor = alphaBlendRGB565( fg, bg, Alpha );
       GfxPixelColorFast( X, Y, newcolor );
}


void GfxLineColor( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color )
{
       int dx = x1 - x0;
       int dy = y1 - y0;
       int incX = sgn(dx);
       int incY = sgn(dy);

       dx = (dx >= 0 ? dx : -dx);
       dy = (dy >= 0 ? dy : -dy);

       if (dy == 0)
       {
              // horizontal line
              for (int x = x0; x != x1 + incX; x += incX)
                     GfxPixelColor( x, y0, color );
       }
       else if (dx == 0)
       {
              // vertical line
              for (int y = y0; y != y1 + incY; y += incY)
                     GfxPixelColor( x0, y, color );
       }
       else if (dx >= dy)
       {
              // more horizontal than vertical
              int slope = 2 * dy;
              int error = -dx;
              int errorInc = -2 * dx;
              int y = y0;

              for (int x = x0; x != x1 + incX; x += incX)
              {
                     GfxPixelColor( x, y, color );
                     error += slope;

                     if (error >= 0)
                     {
                            y += incY;
                            error += errorInc;
                     }
              }
       }
       else
       {
              // more vertical than horizontal
              int slope = 2 * dx;
              int error = -dy;
              int errorInc = -2 * dy;
              int x = x0;

              for (int y = y0; y != y1 + incY; y += incY)
              {
                     GfxPixelColor( x, y, color );
                     error += slope;

                     if (error >= 0)
                     {
                            x += incX;
                            error += errorInc;
                     }
              }
       }
}

void GfxLineColorA( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color, uint8_t alpha )
{
       int dx = x1 - x0;
       int dy = y1 - y0;
       int incX = sgn(dx);
       int incY = sgn(dy);
       dx = abs(dx);
       dy = abs(dy);

       if (dy == 0)
       {
              // horizontal line
              for (int x = x0; x != x1 + incX; x += incX)
                     GfxPixelColorA( x, y0, color, alpha );
       }
       else if (dx == 0)
       {
              // vertical line
              for (int y = y0; y != y1 + incY; y += incY)
                     GfxPixelColorA( x0, y, color, alpha );
       }
       else if (dx >= dy)
       {
              // more horizontal than vertical
              int slope = 2 * dy;
              int error = -dx;
              int errorInc = -2 * dx;
              int y = y0;

              for (int x = x0; x != x1 + incX; x += incX)
              {
                     GfxPixelColorA( x, y, color, alpha );
                     error += slope;

                     if (error >= 0)
                     {
                            y += incY;
                            error += errorInc;
                     }
              }
       }
       else
       {
              // more vertical than horizontal
              int slope = 2 * dx;
              int error = -dy;
              int errorInc = -2 * dy;
              int x = x0;

              for (int y = y0; y != y1 + incY; y += incY)
              {
                     GfxPixelColorA( x, y, color, alpha );
                     error += slope;

                     if (error >= 0)
                     {
                            x += incX;
                            error += errorInc;
                     }
              }
       }
}

void GfxLineRGB( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t R, uint8_t G, uint8_t B )
{
       uint16_t color;
       GfxRGBtoColor( R, G, B, &color );

       int dx = x1 - x0;
       int dy = y1 - y0;
       int incX = sgn(dx);
       int incY = sgn(dy);
       dx = abs(dx);
       dy = abs(dy);

       if (dy == 0)
       {
              // horizontal line
              for (int x = x0; x != x1 + incX; x += incX)
                     GfxPixelColor( x, y0, color );
       }
       else if (dx == 0)
       {
              // vertical line
              for (int y = y0; y != y1 + incY; y += incY)
                     GfxPixelColor( x0, y, color );
       }
       else if (dx >= dy)
       {
              // more horizontal than vertical
              int slope = 2 * dy;
              int error = -dx;
              int errorInc = -2 * dx;
              int y = y0;

              for (int x = x0; x != x1 + incX; x += incX)
              {
                     GfxPixelColor( x, y, color );
                     error += slope;

                     if (error >= 0)
                     {
                            y += incY;
                            error += errorInc;
                     }
              }
       }
       else
       {
              // more vertical than horizontal
              int slope = 2 * dx;
              int error = -dy;
              int errorInc = -2 * dy;
              int x = x0;

              for (int y = y0; y != y1 + incY; y += incY)
              {
                     GfxPixelColor( x, y, color );
                     error += slope;

                     if (error >= 0)
                     {
                            x += incX;
                            error += errorInc;
                     }
              }
       }
}

void GfxLineRGBA( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t R, uint8_t G, uint8_t B, uint8_t alpha )
{
       uint16_t color;
       GfxRGBtoColor( R, G, B, &color );

       int dx = x1 - x0;
       int dy = y1 - y0;
       int incX = sgn(dx);
       int incY = sgn(dy);
       dx = abs(dx);
       dy = abs(dy);

       if (dy == 0)
       {
              // horizontal line
              for (int x = x0; x != x1 + incX; x += incX)
                     GfxPixelColorA( x, y0, color, alpha );
       }
       else if (dx == 0)
       {
              // vertical line
              for (int y = y0; y != y1 + incY; y += incY)
                     GfxPixelColorA( x0, y, color, alpha );
       }
       else if (dx >= dy)
       {
              // more horizontal than vertical
              int slope = 2 * dy;
              int error = -dx;
              int errorInc = -2 * dx;
              int y = y0;

              for (int x = x0; x != x1 + incX; x += incX)
              {
                     GfxPixelColorA( x, y, color, alpha );
                     error += slope;

                     if (error >= 0)
                     {
                            y += incY;
                            error += errorInc;
                     }
              }
       }
       else
       {
              // more vertical than horizontal
              int slope = 2 * dx;
              int error = -dy;
              int errorInc = -2 * dy;
              int x = x0;

              for (int y = y0; y != y1 + incY; y += incY)
              {
                     GfxPixelColorA( x, y, color, alpha );
                     error += slope;

                     if (error >= 0)
                     {
                            x += incX;
                            error += errorInc;
                     }
              }
       }
}




void GfxQuickLineColor( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color )
{
       if (x1==x0)
       {
              if (y1<y0) swap(y0,y1);
              for (int k =y0; k<=y1; k++)
                     GfxPixelColor(x0,k,color);
       }
       else if (y1==y0)
       {
              if (x1<x0) swap(x0,x1);
              for (int k =x0; k<=x1; k++)
                     GfxPixelColor(k,y0,color);
       }
       else
       {
              fixed_point f;
              int x;
              int32_t m=((int32_t)(y1-y0)<<16)/(x1-x0);

              f.i=y0<<16;
              for (x=x0; x<=x1; x++,f.i+=m)
              {
                     fixed_point g=f;
                     g.i+=32767;
                     GfxPixelColor(x,g.hi,color);
              }
       }
}


void GfxCircleColor(uint16_t centerX, uint16_t centerY, uint16_t radius, uint16_t color )
{
       int x = 0;
       int y = radius;
       int m = 5 - 4 * radius;

       while (x <= y)
       {
              GfxPixelColor( centerX + x, centerY + y, color );
              GfxPixelColor( centerX + x, centerY - y, color );
              GfxPixelColor( centerX - x, centerY + y, color );
              GfxPixelColor( centerX - x, centerY - y, color );
              GfxPixelColor( centerX + y, centerY + x, color );
              GfxPixelColor( centerX + y, centerY - x, color );
              GfxPixelColor( centerX - y, centerY + x, color );
              GfxPixelColor( centerX - y, centerY - x, color );

              if (m > 0)
              {
                     y--;
                     m -= 8 * y;
              }

              x++;

              m += 8 * x + 4;
       }
}

void GfxCircleColorA(uint16_t centerX, uint16_t centerY, uint16_t radius, uint16_t color, uint8_t alpha )
{
       int x = 0;
       int y = radius;
       int m = 5 - 4 * radius;

       while (x <= y)
       {
              GfxPixelColorA( centerX + x, centerY + y, color, alpha );
              GfxPixelColorA( centerX + x, centerY - y, color, alpha );
              GfxPixelColorA( centerX - x, centerY + y, color, alpha );
              GfxPixelColorA( centerX - x, centerY - y, color, alpha );
              GfxPixelColorA( centerX + y, centerY + x, color, alpha );
              GfxPixelColorA( centerX + y, centerY - x, color, alpha );
              GfxPixelColorA( centerX - y, centerY + x, color, alpha );
              GfxPixelColorA( centerX - y, centerY - x, color, alpha );

              if (m > 0)
              {
                     y--;
                     m -= 8 * y;
              }

              x++;

              m += 8 * x + 4;
       }
}

void GfxCircleRGB(uint16_t centerX, uint16_t centerY, uint16_t radius,uint8_t R, uint8_t G, uint8_t B )
{
       uint16_t color;
       GfxRGBtoColor( R, G, B, &color );

       int x = 0;
       int y = radius;
       int m = 5 - 4 * radius;

       while (x <= y)
       {
              GfxPixelColor( centerX + x, centerY + y, color );
              GfxPixelColor( centerX + x, centerY - y, color );
              GfxPixelColor( centerX - x, centerY + y, color );
              GfxPixelColor( centerX - x, centerY - y, color );
              GfxPixelColor( centerX + y, centerY + x, color );
              GfxPixelColor( centerX + y, centerY - x, color );
              GfxPixelColor( centerX - y, centerY + x, color );
              GfxPixelColor( centerX - y, centerY - x, color );

              if (m > 0)
              {
                     y--;
                     m -= 8 * y;
              }

              x++;

              m += 8 * x + 4;
       }
}

void GfxCircleRGBA(uint16_t centerX, uint16_t centerY, uint16_t radius, uint8_t R, uint8_t G, uint8_t B, uint8_t alpha )
{
       uint16_t color;
       GfxRGBtoColor( R, G, B, &color );

       int x = 0;
       int y = radius;
       int m = 5 - 4 * radius;

       while (x <= y)
       {
              GfxPixelColorA( centerX + x, centerY + y, color, alpha );
              GfxPixelColorA( centerX + x, centerY - y, color, alpha );
              GfxPixelColorA( centerX - x, centerY + y, color, alpha );
              GfxPixelColorA( centerX - x, centerY - y, color, alpha );
              GfxPixelColorA( centerX + y, centerY + x, color, alpha );
              GfxPixelColorA( centerX + y, centerY - x, color, alpha );
              GfxPixelColorA( centerX - y, centerY + x, color, alpha );
              GfxPixelColorA( centerX - y, centerY - x, color, alpha );

              if (m > 0)
              {
                     y--;
                     m -= 8 * y;
              }

              x++;

              m += 8 * x + 4;
       }
}
