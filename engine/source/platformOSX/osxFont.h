//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#import "platform/platformFont.h"

//-----------------------------------------------------------------------------

class OSXFont : public PlatformFont
{
private:

    // Font reference.
    CTFontRef       mFontRef;

    // Distance from drawing point to typographic baseline.
    // Think of the drawing point as the upper left corner of a text box.
    // NOTE: 'baseline' is synonymous with 'ascent' in Torque.
    U32             mBaseline;

    // Distance between lines.
    U32             mHeight;

    // Glyph rendering color-space.
    CGColorSpaceRef mColorSpace;
    
public:
    OSXFont();
    virtual ~OSXFont();
    
    /// Look up the requested font, cache style, layout, colorspace, and some metrics.
    virtual bool create( const char* name, U32 size, U32 charset = TGE_ANSI_CHARSET);
    
    /// Determine if the character requested is a drawable character, or if it should be ignored.
    virtual bool isValidChar( const UTF16 character) const;
    virtual bool isValidChar( const UTF8* str) const;
    
    /// Get some vertical data on the font at large. Useful for drawing multiline text, and sizing text boxes.
    virtual U32 getFontHeight() const { return mHeight; }
    virtual U32 getFontBaseLine() const { return mBaseline; }
    
    // Draw the character to a temporary bitmap, and fill the CharInfo with various text metrics.
    virtual PlatformFont::CharInfo &getCharInfo(const UTF16 character) const;
    virtual PlatformFont::CharInfo &getCharInfo(const UTF8 *str) const;
};