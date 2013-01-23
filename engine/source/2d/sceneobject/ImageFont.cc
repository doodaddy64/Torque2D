//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _DGL_H_
#include "graphics/dgl.h"
#endif

#include "console/consoleTypes.h"

#include "io/bitStream.h"

#include "string/stringBuffer.h"

#include "ImageFont.h"

// Script bindings.
#include "ImageFont_ScriptBinding.h"

//------------------------------------------------------------------------------

static EnumTable::Enums textAlignmentEnums[] = 
{
    { ImageFont::ALIGN_LEFT,      "Left" },
    { ImageFont::ALIGN_CENTER,    "Center" },
    { ImageFont::ALIGN_RIGHT,     "Right" },
};

static EnumTable gTextAlignmentTable(3, &textAlignmentEnums[0]); 

//-----------------------------------------------------------------------------

ImageFont::TextAlignment ImageFont::getTextAlignmentEnum(const char* label)
{
    // Search for Mnemonic.
    for (U32 i = 0; i < (sizeof(textAlignmentEnums) / sizeof(EnumTable::Enums)); i++)
    {
        if( dStricmp(textAlignmentEnums[i].label, label) == 0)
            return (TextAlignment)textAlignmentEnums[i].index;
    }

    // Warn.
    Con::warnf("ImageFont::getTextAlignmentEnum() - Invalid text alignment of '%s'", label );

    return ImageFont::INVALID_ALIGN;
}

//-----------------------------------------------------------------------------

const char* ImageFont::getTextAlignmentDescription(const ImageFont::TextAlignment alignment)
{
    // Search for Mnemonic.
    for (U32 i = 0; i < (sizeof(textAlignmentEnums) / sizeof(EnumTable::Enums)); i++)
    {
        if( textAlignmentEnums[i].index == alignment )
            return textAlignmentEnums[i].label;
    }

    // Warn.
    Con::warnf( "ImageFont::getTextAlignmentDescription() - Invalid text alignment.");

    return StringTable->EmptyString;
}

//------------------------------------------------------------------------------

IMPLEMENT_CONOBJECT(ImageFont);

//-----------------------------------------------------------------------------

ImageFont::ImageFont() :
    mTextAlignment( ImageFont::ALIGN_CENTER ),
    mCharacterSize( 1.0f, 1.0f ),
    mCharacterPadding( 0 ),
    mConsoleBuffer(StringTable->EmptyString)
{
   // Use a static body by default.
   mBodyDefinition.type = b2_staticBody;

   // All the font characters are batch isolated.
   setBatchIsolated( true );
}

//-----------------------------------------------------------------------------

ImageFont::~ImageFont()
{
}

//-----------------------------------------------------------------------------

void ImageFont::initPersistFields()
{    
    // Call parent.
    Parent::initPersistFields();

    addProtectedField("image", TypeImageAssetPtr, Offset(mImageAsset, ImageFont), &setImage, &getImage, &writeImage, "");
    addProtectedField("text", TypeString, Offset( mConsoleBuffer, ImageFont ), setText, getText, &writeText, "The text to be displayed." );  
    addProtectedField("textAlignment", TypeEnum, Offset(mTextAlignment, ImageFont), &setTextAlignment, &defaultProtectedGetFn, &writeTextAlignment, 1, &gTextAlignmentTable, "");
    addProtectedField("characterSize", TypeT2DVector, Offset(mCharacterSize, ImageFont), &setCharacterSize, &defaultProtectedGetFn,&writeCharacterSize, "" );
    addProtectedField("characterPadding", TypeF32, Offset(mCharacterPadding, ImageFont), &setCharacterPadding, &defaultProtectedGetFn, &writeCharacterPadding, "" );
}

//-----------------------------------------------------------------------------

bool ImageFont::onAdd()
{
    // Call Parent.
    if(!Parent::onAdd())
        return false;
    
    // Return Okay.
    return true;
}

//-----------------------------------------------------------------------------

void ImageFont::onRemove()
{
    // Call Parent.
    Parent::onRemove();
}

//------------------------------------------------------------------------------

void ImageFont::copyTo(SimObject* object)
{
    // Fetch font object.
    ImageFont* pFontObject = dynamic_cast<ImageFont*>(object);

    // Sanity.
    AssertFatal(pFontObject != NULL, "ImageFont::copyTo() - Object is not the correct type.");

    // Call parent.
    Parent::copyTo(object);

    // Copy.
    pFontObject->setImage( getImage() );
    pFontObject->setText( getText() );
    pFontObject->setTextAlignment( getTextAlignment() );
    pFontObject->setCharacterSize( getCharacterSize() );
    pFontObject->setCharacterPadding( getCharacterPadding() );
}

//------------------------------------------------------------------------------

void ImageFont::sceneRender( const SceneRenderState* pSceneRenderState, const SceneRenderRequest* pSceneRenderRequest, BatchRender* pBatchRenderer )
{
    // Finish if no image asset.
    if ( mImageAsset.isNull() )
        return;

    // Fetch number of characters to render.
    const U32 renderCharacters = mText.length();

    // Ignore if no text to render.
    if( renderCharacters == 0 )
        return;

    // Fetch render OOBB.
    const Vector2& renderOOBB0 = mRenderOOBB[0];
    const Vector2& renderOOBB1 = mRenderOOBB[1];
    const Vector2& renderOOBB3 = mRenderOOBB[3];

    Vector2 characterOOBB0;
    Vector2 characterOOBB1;
    Vector2 characterOOBB2;
    Vector2 characterOOBB3;

    // Calculate the starting render position based upon text alignment.
    switch( mTextAlignment )
    {
        case ALIGN_LEFT:
            {
                // Size is twice the padded text width as we're aligning to the left from the position expanding rightwards.
                characterOOBB0.Set( (renderOOBB0.x + renderOOBB1.x)*0.5f, renderOOBB0.y );
            }
            break;

        case ALIGN_RIGHT:
            {
                // Size is twice the padded text width as we're aligning to the right from the position expanding leftwards.
                characterOOBB0 = renderOOBB0;
            }
            break;

        default:
            {
                // Warn.
                Con::warnf("ImageFont() - Unknown text alignment!");
            }
        case ALIGN_CENTER:
            {
                // Size is the total padded text size as we're simply centered on the position.
                characterOOBB0 = renderOOBB0;
            }
            break;
    }

    // Calculate character width stride.
    Vector2 characterWidthStride = (renderOOBB1 - renderOOBB0);
    characterWidthStride.Normalize( mCharacterSize.x + mCharacterPadding );

    // Calculate character height stride.
    Vector2 characterHeightStride = (renderOOBB3 - renderOOBB0);
    characterHeightStride.Normalize( mCharacterSize.y );

    // Complete character OOBB.
    characterOOBB1 = characterOOBB0 + characterWidthStride;
    characterOOBB2 = characterOOBB1 + characterHeightStride;
    characterOOBB3 = characterOOBB2 - characterWidthStride;

    // Render all the characters.    
    for( U32 characterIndex = 0; characterIndex < renderCharacters; ++characterIndex )
    {
        // Fetch character.
        U32 character = mText.getChar( characterIndex );

        // Set character to "space" if it's out of bounds.
        if ( character < 32 || character > 128 )
            character = 32;

        // Calculate character frame index.
        const U32 characterFrameIndex = character - 32;

        // Fetch current frame area.
        const ImageAsset::FrameArea::TexelArea& texelFrameArea = mImageAsset->getImageFrameArea( characterFrameIndex ).mTexelArea;

        // Fetch lower/upper texture coordinates.
        const Vector2& texLower = texelFrameArea.mTexelLower;
        const Vector2& texUpper = texelFrameArea.mTexelUpper;

        // Submit batched quad.
        pBatchRenderer->SubmitQuad(
            characterOOBB0,
            characterOOBB1,
            characterOOBB2,
            characterOOBB3,
            Vector2( texLower.x, texUpper.y ),
            Vector2( texUpper.x, texUpper.y ),
            Vector2( texUpper.x, texLower.y ),
            Vector2( texLower.x, texLower.y ),
            mImageAsset->getImageTexture() );

        // Translate character OOBB.
        characterOOBB0 += characterWidthStride;
        characterOOBB1 += characterWidthStride;
        characterOOBB2 += characterWidthStride;
        characterOOBB3 += characterWidthStride;
    }
}


//-----------------------------------------------------------------------------

bool ImageFont::setImage( const char* pImageAssetId )
{
    // Set asset.
    mImageAsset = pImageAssetId;

    // Finish if no image asset.
    if ( mImageAsset.isNull() )
        return false;

    // We need a minimum of 96 frames here.
    if ( mImageAsset->getFrameCount() < 96 )
    {
        // Warn.
        Con::warnf("ImageFont::setImage() - The image needs to have at least 96 frames to be used as a font! (%s)", mImageAsset.getAssetId() );
        mImageAsset.clear();
        return false;
    }
    
    // Return Okay.
    return true;
}

//-----------------------------------------------------------------------------

void ImageFont::setText( const StringBuffer& text )
{
    // Set text.
    mText.set( &text );
    calculateSpatials();   
}

//-----------------------------------------------------------------------------

void ImageFont::setTextAlignment( const TextAlignment alignment )
{
    mTextAlignment = alignment;
    calculateSpatials();
}

//-----------------------------------------------------------------------------

void ImageFont::setCharacterSize( const Vector2& size )
{
    mCharacterSize = size;
    mCharacterSize.clampZero();
    calculateSpatials();
}

//-----------------------------------------------------------------------------

void ImageFont::setCharacterPadding( const U32 padding )
{
    mCharacterPadding = padding;
    calculateSpatials();
}

//-----------------------------------------------------------------------------

void ImageFont::calculateSpatials( void )
{
    // Fetch number of characters to render.
    const U32 renderCharacters = mText.length();

    // Set size as a single character if no text.
    if ( renderCharacters == 0 )
    {
        setSize( mCharacterSize );
        return;
    }

    // Calculate total character padding.
    const U32 totalCharacterPadding = (renderCharacters * mCharacterPadding) - mCharacterPadding;

    // Calculate total character size.
    const Vector2 totalCharacterSize( renderCharacters * mCharacterSize.x, mCharacterSize.y );

    // Calculate total padded text size.
    const Vector2 totalPaddedTextSize( totalCharacterSize.x + totalCharacterPadding, totalCharacterSize.y );

    // Calculate size (AABB) including alignment relative to position.
    // NOTE:    For left/right alignment we have to double the size width as clipping is based upon size and
    //          we're aligning to the position here.  We cannot align to the size AABB itself as that changes
    //          as the text length changes therefore it is not a stable point from which to align from.
    switch( mTextAlignment )
    {
        case ALIGN_LEFT:
            {
                // Size is twice the padded text width as we're aligning to the left from the position expanding rightwards.
                setSize( totalPaddedTextSize * 2.0f );
            }
            break;

        case ALIGN_RIGHT:
            {
                // Size is twice the padded text width as we're aligning to the right from the position expanding leftwards.
                setSize( totalPaddedTextSize * 2.0f );
            }
            break;

        case ALIGN_CENTER:
            {
                // Size is the total padded text size as we're simply centered on the position.
                setSize( totalPaddedTextSize );
            }
            break;

        default:
            {
                // Warn.
                Con::warnf("ImageFont() - Unknown text alignment!");
            }
    }
}

//-----------------------------------------------------------------------------

bool ImageFont::setTextAlignment( void* obj, const char* data )
{
    static_cast<ImageFont*>( obj )->setTextAlignment( getTextAlignmentEnum(data) ); return false;
}