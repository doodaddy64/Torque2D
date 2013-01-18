//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef TORQUE_PLAYER


#include "console/console.h"
#include "editor/LevelBuilderBitmapFontTool.h"

// Implement Our Console Object
IMPLEMENT_CONOBJECT(LevelBuilderBitmapFontTool);

LevelBuilderBitmapFontTool::LevelBuilderBitmapFontTool() : LevelBuilderCreateTool(),
    mImageName(NULL),
    mText("")
{
    // Set our tool name
    mToolName      = StringTable->insert("Bitmap Font Tool");
}

LevelBuilderBitmapFontTool::~LevelBuilderBitmapFontTool()
{
}

SceneObject* LevelBuilderBitmapFontTool::createObject()
{
    BitmapFontObject* bitmapFontObject = dynamic_cast<BitmapFontObject*>(ConsoleObject::create("BitmapFontObject"));

    if (bitmapFontObject)
    {
        bitmapFontObject->setImage(mImageName);
    }

    return bitmapFontObject;
}

Point2I LevelBuilderBitmapFontTool::getPixelSize()
{
    BitmapFontObject* bitmapFontObject = dynamic_cast<BitmapFontObject*>(mCreatedObject);
    if (bitmapFontObject)
    {
        ImageAsset* image = dynamic_cast<ImageAsset*>(Sim::findObject(bitmapFontObject->getImage()));
        if (image)
        {
            const ImageAsset::FrameArea::PixelArea& pixelArea = image->getImageFrameArea(0).mPixelArea;
            return Point2I(pixelArea.mPixelWidth, pixelArea.mPixelHeight);
        }
    }

    return Parent::getPixelSize();
}

ConsoleMethod(LevelBuilderBitmapFontTool, setImage, void, 3, 3, "Sets the image map for the created bitmap font object.")
{
    ImageAsset* image = dynamic_cast<ImageAsset*>(Sim::findObject(argv[2]));
    if (image)
    {
        object->setImageName(argv[2]);
    }
    else
        Con::warnf("LevelBuilderBitmapFontTool::setImage - Invalid image map: %s", argv[2]);
}

void LevelBuilderBitmapFontTool::setImageName( const char* name )
{
    mImageName = StringTable->insert(name); 
};

ConsoleMethod(LevelBuilderBitmapFontTool, setBitmapText, void, 3, 3, "Sets the text for a bitmap font object.")
{
    object->setText(argv[2]);
}

void LevelBuilderBitmapFontTool::setText( const char* text )
{
    mText.set(text);
};



#endif // TORQUE_TOOLS
