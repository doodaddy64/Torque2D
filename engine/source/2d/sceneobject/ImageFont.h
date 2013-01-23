//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _BITMAP_FONT_OBJECT_H_
#define _BITMAP_FONT_OBJECT_H_

#ifndef _STRINGBUFFER_H_
#include "string/stringBuffer.h"
#endif

#ifndef _IMAGE_ASSET_H_
#include "2d/assets/ImageAsset.h"
#endif

#ifndef _SCENE_OBJECT_H_
#include "2d/sceneobject/SceneObject.h"
#endif

#ifndef _ASSET_PTR_H_
#include "assets/assetPtr.h"
#endif

#ifndef _UTILITY_H_
#include "2d/core/utility.h"
#endif

//-----------------------------------------------------------------------------

class ImageFont : public SceneObject
{
    typedef SceneObject          Parent;

public:
    enum TextAlignment
    {
        INVALID_ALIGN,

        ALIGN_LEFT,
        ALIGN_CENTER,
        ALIGN_RIGHT
    };

private:
    AssetPtr<ImageAsset> mImageAsset;
    StringBuffer        mText;
    U32                 mCharacterPadding;
    Vector2           mCharacterSize;
    TextAlignment       mTextAlignment;

    struct TextCell
    {
        char CharValue;
        U32 FrameCell;

        TextCell(char charValue, U32 frameCell)
        {
            CharValue = charValue;
            FrameCell = frameCell;
        }
    };

    const char*         mConsoleBuffer;
    Vector<TextCell>    mTextCells;

private:
    void calculateSpatials( void );

public:
    ImageFont();
    ~ImageFont();

    static void initPersistFields();

    bool onAdd();
    void onRemove();
    void copyTo(SimObject* object);

    virtual void sceneRender( const SceneRenderState* pSceneRenderState, const SceneRenderRequest* pSceneRenderRequest, BatchRender* pBatchRenderer );
    virtual bool canRender( void ) const { return mImageAsset.notNull() && mText.length() > 0; }

    bool setImage( const char* pImageAssetId );
    const char* getImage( void ) const                                  { return mImageAsset.getAssetId(); };
    void setText( const StringBuffer& text );
    inline StringBuffer& getText( void )                                { return mText; }
    void setTextAlignment( const TextAlignment alignment );
    inline TextAlignment getTextAlignment( void ) const                 { return mTextAlignment; }
    void setCharacterSize( const Vector2& size );
    inline Vector2 getCharacterSize( void ) const                       { return mCharacterSize; }
    void setCharacterPadding( const U32 padding );
    inline U32 getCharacterPadding( void ) const                        { return mCharacterPadding; }

    static TextAlignment getTextAlignmentEnum(const char* label);
    static const char* getTextAlignmentDescription(const TextAlignment alignment);

    // Declare Console Object.
    DECLARE_CONOBJECT(ImageFont);

protected:
    static bool setImage(void* obj, const char* data)                   { static_cast<ImageFont*>(obj)->setImage( data ); return false; }
    static const char* getImage(void* obj, const char* data)            { return static_cast<ImageFont*>(obj)->getImage(); }
    static bool writeImage( void* obj, StringTableEntry pFieldName )    { return static_cast<ImageFont*>(obj)->mImageAsset.notNull(); }
    static bool setText( void* obj, const char* data )                  { static_cast<ImageFont*>( obj )->setText( data ); return false; }
    static const char* getText( void* obj, const char* data )           { return static_cast<ImageFont*>( obj )->getText().getPtr8(); }
    static bool writeText( void* obj, StringTableEntry pFieldName )     { return static_cast<ImageFont*>(obj)->mText.length() != 0; }
    static bool setTextAlignment( void* obj, const char* data );
    static bool writeTextAlignment( void* obj, StringTableEntry pFieldName ) {return static_cast<ImageFont*>(obj)->getTextAlignment() != ImageFont::ALIGN_CENTER; }
    static bool setCharacterSize( void* obj, const char* data )         { static_cast<ImageFont*>( obj )->setCharacterSize( Utility::mGetStringElementVector(data) ); return false; }
    static bool writeCharacterSize( void* obj, StringTableEntry pFieldName ) { return static_cast<ImageFont*>(obj)->getCharacterSize().isEqual(Vector2::getOne()); }
    static bool setCharacterPadding( void* obj, const char* data )      { static_cast<ImageFont*>( obj )->setCharacterPadding( dAtoi(data) ); return false; }
    static bool writeCharacterPadding( void* obj, StringTableEntry pFieldName ) { return static_cast<ImageFont*>(obj)->getCharacterPadding() != 0; }
};

#endif // _BITMAP_FONT_OBJECT_H_