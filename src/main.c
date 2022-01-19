#include <gint/keyboard.h>
#include <libprof.h>
#include <gint/display.h>
#include "GfxPrimitives.h"
#include <math.h>


#define DELTA_INC 5                // set resolution at 5µs
// does not seem to work with lower values
int libproftime[10];

float Tcos[360], Tsin[360];

int radius = 45;

int main(void)
{

       for (int k = 0; k<360; k++)
       {
              Tcos[k]=radius * cos(k * 3.1415927 / 180);
              Tsin[k]=radius * sin(k * 3.1415927 / 180);
       }
       /*
              prof_init();
              prof_t prof = prof_make();

              for (int i=0; i<1; i++)
              {
                     dclear(0xFFFF);
                     prof_enter(prof);

                     for( int k=0; k< 1000; k++ )
                     {
                         GfxLineColor( 0, 0, 395, 223, 0x0000 );
                     }

                     prof_leave(prof);
                     dupdate();
                     libproftime[i] = prof_time(prof);
                     prof_quit();
              }
              getkey();
              dclear(0x0000);
              for (int i=0; i<10; i++)
              {
                     dprint(1, 1+i*10, C_WHITE, "GfxLineColor : %d micro-seconds.", libproftime[i] );
              }
              dupdate();
              getkey();



              prof_init();
              prof = prof_make();
              for (int i=0; i<1; i++)
              {
                     dclear(0xFFFF);
                     prof_enter(prof);

                     for( int k=0; k< 1000; k++ )
                     {
                         GfxLineColorA( 0, 0, 395, 223, 0x0000, 64 );
                     }
                     prof_leave(prof);
                     dupdate();
                     libproftime[i] = prof_time(prof);
                     prof_quit();
              }
              getkey();
              dclear(0x0000);
              for (int i=0; i<10; i++)
              {
                     dprint(1, 1+i*10, C_WHITE, "GfxLineColorA : %d micro-seconds.", libproftime[i] );
              }
              dupdate();
              getkey();



              prof_init();
              prof = prof_make();
              for (int i=0; i<1; i++)
              {
                     dclear(0xFFFF);
                     prof_enter(prof);

                     for( int k=0; k< 1000; k++ )
                     {
                         GfxLineRGB( 0, 0, 395, 223, 0, 0, 0 );
                     }

                     prof_leave(prof);
                     dupdate();
                     libproftime[i] = prof_time(prof);
                     prof_quit();
              }
              getkey();
              dclear(0x0000);
              for (int i=0; i<10; i++)
              {
                     dprint(1, 1+i*10, C_WHITE, "GfxLineRGB : %d micro-seconds.", libproftime[i] );
              }
              dupdate();
              getkey();



              prof_init();
              prof = prof_make();
              for (int i=0; i<1; i++)
              {
                     dclear(0xFFFF);
                     prof_enter(prof);

                     for( int k=0; k< 1000; k++ )
                     {
                         GfxLineRGBA( 0, 0, 395, 223, 0, 0, 0, 64 );
                     }

                     prof_leave(prof);
                     dupdate();
                     libproftime[i] = prof_time(prof);
                     prof_quit();
              }
              getkey();
              dclear(0x0000);
              for (int i=0; i<10; i++)
              {
                     dprint(1, 1+i*10, C_WHITE, "GfxLineRGBA : %d micro-seconds.", libproftime[i] );
              }
              dupdate();
              getkey();
       */
       int nbiter=1;
       int i=0;

       prof_init();

       dclear( 0xFFFF );

       libproftime[i] = prof_exec(
       {
              for( int k=0; k< 360; k++ ) dline( 50, 150, (int) (50+Tcos[k]), (int) (150+Tsin[k]), 0x0000 );
       }
       );

       dprint(1, 1+i*10, C_GREEN, "Reference  dline : %d micro-seconds.", libproftime[i]/nbiter );
       i++;

       /*      libproftime[i] = prof_exec(
             {
                    for( int k=0; k< nbiter; k++ ) GfxLineColor( 1, 0, 395, 223, 0x0000 );
             }
             );

             dprint(1, 1+i*10, C_BLACK, "GfxLineColor  : %d micro-seconds.", libproftime[i]/nbiter );
             i++;
       */
       libproftime[i] = prof_exec(
       {
              for( int k=0; k< 360; k++ ) GfxLineColor( 150, 150, (int) (150+Tcos[k]), (int) (150+Tsin[k]), 0x0000 );
       }
       );

       dprint(1, 1+i*10, C_RED, "GfxLineColor : %d micro-seconds.", libproftime[i]/nbiter );
       i++;

       /*
              libproftime[i] = prof_exec(
              {
                     for( int k=0; k< nbiter; k++ ) GfxLineColorA( 0, 222, 394, 0, 0x0000, 64 );
              }
              );
              dprint(1, 1+i*10, C_BLACK, "GfxLineColorA : %d micro-seconds.", libproftime[i]/nbiter );
              i++;



              libproftime[i] = prof_exec(
              {
                     for( int k=0; k< nbiter; k++ ) GfxLineRGB( 0, 1, 394, 222, 0, 0, 0 );
              }
              );

              dprint(1, 1+i*10, C_BLACK, "GfxLineRGB  : %d micro-seconds.", libproftime[i]/nbiter );
              i++;

              libproftime[i] = prof_exec(
              {
                     for( int k=0; k< nbiter; k++ ) GfxLineRGBA( 1, 222, 395, 1, 0, 0, 0, 64 );
              }
              );
              dprint(1, 1+i*10, C_BLACK, "GfxLineRGBA : %d micro-seconds.", libproftime[i]/nbiter );
              i++;

              libproftime[i] = prof_exec(
              {
                     for( int k=0; k< nbiter; k++ ) GfxCircleColor( 396/2, 224/2, 100, 0x0000 );
              }
              );
              dprint(1, 1+i*10, C_BLACK, "GfxCircleColor : %d micro-seconds.", libproftime[i]/nbiter );
              i++;

              libproftime[i] = prof_exec(
              {
                     for( int k=0; k< nbiter; k++ ) GfxCircleColorA( 396/2, 224/2, 99, 0x0000, 64 );
              }
              );
              dprint(1, 1+i*10, C_BLACK, "GfxCircleColorA : %d micro-seconds.", libproftime[i]/nbiter );
              i++;

              libproftime[i] = prof_exec(
              {
                     for( int k=0; k< nbiter; k++ ) GfxCircleRGB( 396/2, 224/2, 98, 0,0,0 );
              }
              );
              dprint(1, 1+i*10, C_BLACK, "GfxCircleRGB : %d micro-seconds.", libproftime[i]/nbiter );
              i++;

              libproftime[i] = prof_exec(
              {
                     for( int k=0; k< nbiter; k++ ) GfxCircleRGBA( 396/2, 224/2, 97, 0,0,0, 64 );
              }
              );
              dprint(1, 1+i*10, C_BLACK, "GfxCircleRGBA : %d micro-seconds.", libproftime[i]/nbiter );
              i++;

              uint16_t Color = 0x0000;
              uint8_t R,G,B;

              R=255;
              G=0;
              B=0;

              GfxRGBtoColor( R, G, B, &Color );
              dprint(1, 1+i*10, Color, "This text should appear RED.");
              i++;

              Color = 0b0000000000011111;
              GfxColortoRGB( Color, &R, &G, &B );
              dprint(1, 1+i*10, GfxColor(R,G,B), "And this one BLUE.");
              i++;
       */
       dupdate();
       getkey();

       return 1;
}
