//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "graphics/color.h"
#include "console/console.h"
#include "console/consoleTypes.h"


//////////////////////////////////////////////////////////////////////////
// TypeColorF
//////////////////////////////////////////////////////////////////////////
ConsoleType( ColorF, TypeColorF, sizeof(ColorF), "" )

ConsoleGetType( TypeColorF )
{
   ColorF * color = (ColorF*)dptr;
   char* returnBuffer = Con::getReturnBuffer(256);
   dSprintf(returnBuffer, 256, "%g %g %g %g", color->red, color->green, color->blue, color->alpha);
   return(returnBuffer);
}

ConsoleSetType( TypeColorF )
{
   ColorF *tmpColor = (ColorF *) dptr;
   if(argc == 1)
   {
      tmpColor->set(0, 0, 0, 1);
      F32 r,g,b,a;
      S32 args = dSscanf(argv[0], "%g %g %g %g", &r, &g, &b, &a);
      tmpColor->red = r;
      tmpColor->green = g;
      tmpColor->blue = b;
      if (args == 4)
         tmpColor->alpha = a;
   }
   else if(argc == 3)
   {
      tmpColor->red    = dAtof(argv[0]);
      tmpColor->green  = dAtof(argv[1]);
      tmpColor->blue   = dAtof(argv[2]);
      tmpColor->alpha  = 1.f;
   }
   else if(argc == 4)
   {
      tmpColor->red    = dAtof(argv[0]);
      tmpColor->green  = dAtof(argv[1]);
      tmpColor->blue   = dAtof(argv[2]);
      tmpColor->alpha  = dAtof(argv[3]);
   }
   else
      Con::printf("Color must be set as { r, g, b [,a] }");
}

//////////////////////////////////////////////////////////////////////////
// TypeColorI
//////////////////////////////////////////////////////////////////////////
ConsoleType( ColorI, TypeColorI, sizeof(ColorI), "" )

ConsoleGetType( TypeColorI )
{
   ColorI *color = (ColorI *) dptr;
   char* returnBuffer = Con::getReturnBuffer(256);
   dSprintf(returnBuffer, 256, "%d %d %d %d", color->red, color->green, color->blue, color->alpha);
   return returnBuffer;
}

ConsoleSetType( TypeColorI )
{
   ColorI *tmpColor = (ColorI *) dptr;
   if(argc == 1)
   {
      tmpColor->set(0, 0, 0, 255);
      S32 r,g,b,a;
      S32 args = dSscanf(argv[0], "%d %d %d %d", &r, &g, &b, &a);
      tmpColor->red = r;
      tmpColor->green = g;
      tmpColor->blue = b;
      if (args == 4)
         tmpColor->alpha = a;
   }
   else if(argc == 3)
   {
      tmpColor->red    = dAtoi(argv[0]);
      tmpColor->green  = dAtoi(argv[1]);
      tmpColor->blue   = dAtoi(argv[2]);
      tmpColor->alpha  = 255;
   }
   else if(argc == 4)
   {
      tmpColor->red    = dAtoi(argv[0]);
      tmpColor->green  = dAtoi(argv[1]);
      tmpColor->blue   = dAtoi(argv[2]);
      tmpColor->alpha  = dAtoi(argv[3]);
   }
   else
      Con::printf("Color must be set as { r, g, b [,a] }");
}
