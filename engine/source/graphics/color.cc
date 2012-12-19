//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "graphics/color.h"
#include "console/console.h"
#include "console/consoleTypes.h"

#ifndef _HASHTABLE_H
#include "collection/hashTable.h"
#endif

//-----------------------------------------------------------------------------

typedef HashMap<StringTableEntry, ColorF> typeNameToColorFHash;
typedef HashMap<StringTableEntry, ColorI> typeNameToColorIHash;
typedef HashMap<ColorF, StringTableEntry> typeColorFToNameHash;
typedef HashMap<ColorI, StringTableEntry> typeColorIToNameHash;

static typeNameToColorFHash    mNameToColorF;
static typeNameToColorIHash    mNameToColorI;
static typeColorFToNameHash    mColorFToName;
static typeColorIToNameHash    mColorIToName;

#define DEFAULT_UNKNOWN_STOCK_COLOR_NAME    "White"

//-----------------------------------------------------------------------------

class StockColorItem
{
private:
    StockColorItem() {}

public:
    StockColorItem( const char* pName, const U8 red, const U8 green, const U8 blue, const U8 alpha = 255 )
    {
        // Sanity!
        AssertFatal( pName != NULL, "Stock color name cannot be NULL." );

        // Set stock color.
        mColorName = StringTable->insert( pName );
        mColorI.set( red, green, blue, alpha );
        mColorF = mColorI;

        // Insert mappings.
        mNameToColorF.insert( mColorName, mColorF );
        mNameToColorI.insert( mColorName, mColorI );
        mColorFToName.insert( mColorF, mColorName );
        mColorIToName.insert( mColorI, mColorName );
    }

    inline StringTableEntry getColorName( void ) const { return mColorName; }
    inline const ColorF&    getColorF( void ) const { return mColorF; }
    inline const ColorI&    getColorI( void ) const { return mColorI; }

private:
    StringTableEntry    mColorName;
    ColorF              mColorF;
    ColorI              mColorI;
};

//-----------------------------------------------------------------------------

StockColorItem StockColorTable[] =
{
    StockColorItem( "InvisibleBlack", 0, 0, 0, 0 ),
    StockColorItem( "TransparentWhite", 255, 255, 255, 0 ),
    StockColorItem( "AliceBlue", 240, 248, 255 ),
    StockColorItem( "AntiqueWhite", 250, 235, 215 ),
    StockColorItem( "Aqua", 0, 255, 255 ),
    StockColorItem( "Aquamarine", 127, 255, 212 ),
    StockColorItem( "Azure", 240, 255, 255 ),
    StockColorItem( "Beige", 245, 245, 220 ),
    StockColorItem( "Bisque", 255, 228, 196 ),
    StockColorItem( "Black", 0, 0, 0, 255 ),
    StockColorItem( "BlanchedAlmond", 255, 235, 205, 255 ),
    StockColorItem( "Blue", 0, 0, 255 ),
    StockColorItem( "BlueViolet", 138, 43, 226 ),
    StockColorItem( "Brown", 165, 42, 42, 255 ),
    StockColorItem( "BurlyWood", 222, 184, 135 ),
    StockColorItem( "CadetBlue", 95, 158, 160 ),
    StockColorItem( "Chartreuse", 127, 255, 0 ),
    StockColorItem( "Chocolate", 210, 105, 30 ),
    StockColorItem( "Coral", 255, 127, 80 ),
    StockColorItem( "CornflowerBlue", 100, 149, 237 ),
    StockColorItem( "Cornsilk", 255, 248, 220 ),
    StockColorItem( "Crimson", 220, 20, 60 ),
    StockColorItem( "Cyan", 0, 255, 255 ),
    StockColorItem( "DarkBlue", 0, 0, 139 ),
    StockColorItem( "DarkCyan", 0, 139, 139 ),
    StockColorItem( "DarkGoldenrod", 184, 134, 11 ),
    StockColorItem( "DarkGray", 169, 169, 169),
    StockColorItem( "DarkGreen", 0, 100, 0 ),
    StockColorItem( "DarkKhaki", 189, 183, 107 ),
    StockColorItem( "DarkMagenta", 139, 0, 139 ),
    StockColorItem( "DarkOliveGreen", 85, 107, 47 ),
    StockColorItem( "DarkOrange", 255, 140, 0 ),
    StockColorItem( "DarkOrchid", 153, 50, 204 ),
    StockColorItem( "DarkRed", 139, 0, 0 ),
    StockColorItem( "DarkSalmon", 233, 150, 122 ),
    StockColorItem( "DarkSeaGreen", 143, 188, 139 ),
    StockColorItem( "DarkSlateBlue", 72, 61, 139 ),
    StockColorItem( "DarkSlateGray", 47, 79, 79 ),
    StockColorItem( "DarkTurquoise", 0, 206, 209 ),
    StockColorItem( "DarkViolet", 148, 0, 211 ),
    StockColorItem( "DeepPink", 255, 20, 147 ),
    StockColorItem( "DeepSkyBlue", 0, 191, 255 ),
    StockColorItem( "DimGray", 105, 105, 105 ),
    StockColorItem( "DodgerBlue", 30, 144, 255 ),
    StockColorItem( "Firebrick", 178, 34, 34 ),
    StockColorItem( "FloralWhite", 255, 250, 240 ),
    StockColorItem( "ForestGreen", 34, 139, 34 ),
    StockColorItem( "Fuchsia", 255, 0, 255 ),
    StockColorItem( "Gainsboro", 220, 220, 220 ),
    StockColorItem( "GhostWhite", 248, 248, 255 ),
    StockColorItem( "Gold", 255, 215, 0 ),
    StockColorItem( "Goldenrod", 218, 165, 32 ),
    StockColorItem( "Gray", 128, 128, 128 ),
    StockColorItem( "Green", 0, 128, 0 ),
    StockColorItem( "GreenYellow", 173, 255, 47 ),
    StockColorItem( "Honeydew", 240, 255, 24 ),
    StockColorItem( "HotPink", 255, 105, 180 ),
    StockColorItem( "IndianRed", 205, 92, 92 ),
    StockColorItem( "Indigo", 75, 0, 130 ),
    StockColorItem( "Ivory", 255, 255, 240 ),
    StockColorItem( "Khaki", 240, 230, 140 ),
    StockColorItem( "Lavender", 230, 230, 250 ),
    StockColorItem( "LavenderBlush", 255, 240, 245 ),
    StockColorItem( "LawnGreen", 124, 252, 0 ),
    StockColorItem( "LemonChiffon", 255, 250, 205 ),
    StockColorItem( "LightBlue", 173, 216, 230 ),
    StockColorItem( "LightCoral", 240, 128, 128 ),
    StockColorItem( "LightCyan", 224, 255, 255),
    StockColorItem( "LightGoldenrodYellow", 250, 250, 210 ),
    StockColorItem( "LightGray", 211, 211, 211),
    StockColorItem( "LightGreen", 144, 238, 144 ),
    StockColorItem( "LightPink", 255, 182, 193 ),
    StockColorItem( "LightSalmon", 255, 160, 122 ),
    StockColorItem( "LightSeaGreen", 32, 178, 170 ),
    StockColorItem( "LightSkyBlue",135, 206, 250 ),
    StockColorItem( "LightSlateGray", 119, 136, 153 ),
    StockColorItem( "LightSteelBlue", 176, 196, 222 ),
    StockColorItem( "LightYellow", 255, 255, 224 ),
    StockColorItem( "Lime", 0, 255, 0 ),
    StockColorItem( "LimeGreen", 50, 205, 50 ),
    StockColorItem( "Linen", 250, 240, 230 ),
    StockColorItem( "Magenta", 255, 0, 255 ),
    StockColorItem( "Maroon", 128, 0, 0 ),
    StockColorItem( "MediumAquamarine", 102, 205, 170 ),
    StockColorItem( "MediumBlue", 0, 0, 205 ),
    StockColorItem( "MediumOrchid", 186, 85, 211 ),
    StockColorItem( "MediumPurple", 147, 112, 219 ),
    StockColorItem( "MediumSeaGreen", 60, 179, 113 ),
    StockColorItem( "MediumSlateBlue", 123, 104, 238 ),
    StockColorItem( "MediumSpringGreen", 0, 250, 154 ),
    StockColorItem( "MediumTurquoise", 72, 209, 204 ),
    StockColorItem( "MediumVioletRed", 199, 21, 133 ),
    StockColorItem( "MidnightBlue", 25, 25, 112 ),
    StockColorItem( "MintCream", 245, 255, 250 ),
    StockColorItem( "MistyRose", 255, 228, 225 ),
    StockColorItem( "Moccasin", 255, 228, 181 ),
    StockColorItem( "NavajoWhite", 255, 222, 173 ),
    StockColorItem( "Navy", 0, 0, 128 ),
    StockColorItem( "OldLace", 253, 245, 230 ),
    StockColorItem( "Olive", 128, 128, 0 ),
    StockColorItem( "OliveDrab", 107, 142, 35 ),
    StockColorItem( "Orange", 255, 165, 0 ),
    StockColorItem( "OrangeRed", 255, 69, 0 ),
    StockColorItem( "Orchid", 218, 112, 214 ),
    StockColorItem( "PaleGoldenrod", 238, 232, 170 ),
    StockColorItem( "PaleGreen", 152, 251, 152 ),
    StockColorItem( "PaleTurquoise", 175, 238, 238 ),
    StockColorItem( "PaleVioletRed", 219, 112, 147 ),
    StockColorItem( "PapayaWhip", 255, 239, 213 ),
    StockColorItem( "PeachPuff", 255, 218, 185 ),
    StockColorItem( "Peru", 205, 133, 63 ),
    StockColorItem( "Pink", 55, 192, 203 ),
    StockColorItem( "Plum", 221, 160, 221 ),
    StockColorItem( "PowderBlue", 176, 224, 230 ),
    StockColorItem( "Purple", 128, 0, 128 ),
    StockColorItem( "Red", 255, 0, 0 ),
    StockColorItem( "RosyBrown", 188, 143, 143 ),
    StockColorItem( "RoyalBlue", 65, 105, 225 ),
    StockColorItem( "SaddleBrown", 139, 69, 19 ),
    StockColorItem( "Salmon", 250, 128, 114 ),
    StockColorItem( "SandyBrown", 244, 164, 96 ),
    StockColorItem( "SeaGreen", 46, 139, 87 ),
    StockColorItem( "SeaShell", 255, 245, 238 ),
    StockColorItem( "Sienna", 160, 82, 45 ),
    StockColorItem( "Silver", 192, 192, 192 ),
    StockColorItem( "SkyBlue", 135, 206, 235 ),
    StockColorItem( "SlateBlue", 106, 90, 205 ),
    StockColorItem( "SlateGray", 112, 128, 144 ),
    StockColorItem( "Snow", 255, 250, 250 ),
    StockColorItem( "SpringGreen", 0, 255, 127 ),
    StockColorItem( "SteelBlue", 70, 130, 180 ),
    StockColorItem( "Tan", 210, 180, 140 ),
    StockColorItem( "Teal", 0, 128, 128 ),
    StockColorItem( "Thistle", 216, 191, 216 ),
    StockColorItem( "Tomato", 255, 99, 71 ),
    StockColorItem( "Turquoise", 64, 224, 208 ),
    StockColorItem( "Violet", 238, 130, 238 ),
    StockColorItem( "Wheat", 245, 222, 179 ),
    StockColorItem( "White", 255, 255, 255 ),
    StockColorItem( "WhiteSmoke", 245, 245, 245 ),
    StockColorItem( "Yellow", 255, 255, 0 ),
    StockColorItem( "YellowGreen", 154, 205, 50 )
};

//-----------------------------------------------------------------------------

bool StockColor::isColor( const char* pStockColorName )
{
    // Find if color name exists or not.
    return mNameToColorF.find( pStockColorName ) != mNameToColorF.end();
}

//-----------------------------------------------------------------------------

const ColorF& StockColor::colorF( const char* pStockColorName )
{
    // Sanity!
    AssertFatal( pStockColorName != NULL, "Cannot fetch a NULL stock color name." );

    // Find stock color.
    typeNameToColorFHash::iterator colorItr = mNameToColorF.find( pStockColorName );

    // Return color if found.
    if ( colorItr != mNameToColorF.end() )
        return colorItr->value;

    // Warn.
    Con::warnf( "Could not find stock color name '%s'.", pStockColorName );

    // Return default stock color.
    return mNameToColorF.find( DEFAULT_UNKNOWN_STOCK_COLOR_NAME )->value;          
}

//-----------------------------------------------------------------------------

const ColorI& StockColor::colorI( const char* pStockColorName )
{
    // Sanity!
    AssertFatal( pStockColorName != NULL, "Cannot fetch a NULL stock color name." );

    // Find stock color.
    typeNameToColorIHash::iterator colorItr = mNameToColorI.find( pStockColorName );

    // Return color if found.
    if ( colorItr != mNameToColorI.end() )
        return colorItr->value;

    // Warn.
    Con::warnf( "Could not find stock color name '%s'.", pStockColorName );

    // Return default stock color.
    return mNameToColorI.find( DEFAULT_UNKNOWN_STOCK_COLOR_NAME )->value; 
}

//-----------------------------------------------------------------------------

StringTableEntry StockColor::name( const ColorF& color )
{
    // Find stock color name.
    typeColorFToNameHash::iterator colorNameItr = mColorFToName.find( color );

    // Return name if found.
    if ( colorNameItr != mColorFToName.end() )
        return colorNameItr->value;

    // Return empty string.
    return StringTable->EmptyString;
}

//-----------------------------------------------------------------------------

StringTableEntry StockColor::name( const ColorI& color )
{
    // Find stock color name.
    typeColorIToNameHash::iterator colorNameItr = mColorIToName.find( color );

    // Return name if found.
    if ( colorNameItr != mColorIToName.end() )
        return colorNameItr->value;

    // Return empty string.
    return StringTable->EmptyString;
}

//-----------------------------------------------------------------------------

ColorF::ColorF( const char* pStockColorName )
{
    // Set stock color.
    *this = StockColor::colorF( pStockColorName );
}

//-----------------------------------------------------------------------------

void ColorF::set( const char* pStockColorName )
{
    // Set stock color.
    *this = StockColor::colorF( pStockColorName );
}

//-----------------------------------------------------------------------------

const ColorF& ColorF::StockColor( const char* pStockColorName )
{
    return StockColor::colorF( pStockColorName );
}

//-----------------------------------------------------------------------------

StringTableEntry ColorF::StockColor( void )
{
    // Return stock color name.
    return StockColor::name( *this );
}

//-----------------------------------------------------------------------------

ColorI::ColorI( const char* pStockColorName )
{
    // Set stock color.
    *this = StockColor::colorI( pStockColorName );
}

//-----------------------------------------------------------------------------

void ColorI::set( const char* pStockColorName )
{
    // Set stock color.
    *this = StockColor::colorI( pStockColorName );
}

//-----------------------------------------------------------------------------

const ColorI& ColorI::StockColor( const char* pStockColorName )
{
    return StockColor::colorI( pStockColorName );
}

//-----------------------------------------------------------------------------

StringTableEntry ColorI::StockColor( void )
{
    // Return stock color name.
    return StockColor::name( *this );
}

//-----------------------------------------------------------------------------

ConsoleType( ColorF, TypeColorF, sizeof(ColorF), "" )

//-----------------------------------------------------------------------------

ConsoleGetType( TypeColorF )
{
   ColorF * color = (ColorF*)dptr;
   char* returnBuffer = Con::getReturnBuffer(256);
   dSprintf(returnBuffer, 256, "%g %g %g %g", color->red, color->green, color->blue, color->alpha);
   return(returnBuffer);
}

//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------

ConsoleType( ColorI, TypeColorI, sizeof(ColorI), "" )

//-----------------------------------------------------------------------------

ConsoleGetType( TypeColorI )
{
   ColorI *color = (ColorI *) dptr;
   char* returnBuffer = Con::getReturnBuffer(256);
   dSprintf(returnBuffer, 256, "%d %d %d %d", color->red, color->green, color->blue, color->alpha);
   return returnBuffer;
}

//-----------------------------------------------------------------------------

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
