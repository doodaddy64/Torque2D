//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef TORQUE_PLAYER


#include "console/console.h"
#include "editor/levelBuilderScrollerTool.h"

// Implement Our Console Object
IMPLEMENT_CONOBJECT(LevelBuilderScrollerTool);

LevelBuilderScrollerTool::LevelBuilderScrollerTool() : LevelBuilderCreateTool(),
                                                       mImageName(NULL)
{
   // Set our tool name
   mToolName = StringTable->insert("Scroller Tool");
}

LevelBuilderScrollerTool::~LevelBuilderScrollerTool()
{
}

SceneObject* LevelBuilderScrollerTool::createObject()
{
   Scroller* scroller = dynamic_cast<Scroller*>(ConsoleObject::create("Scroller"));

   if (scroller)
      static_cast<SpriteProxyBase*>(scroller)->setImage(mImageName);

   return scroller;
}

Point2I LevelBuilderScrollerTool::getPixelSize()
{
   Scroller* scroller = dynamic_cast<Scroller*>(mCreatedObject);
   if (scroller)
   {
      // No way to get size from the actual object. We'll do it this way for now.
      ImageAsset* image = dynamic_cast<ImageAsset*>(Sim::findObject(mImageName));
      if (image)
      {
          const ImageAsset::FrameArea::PixelArea& pixelArea = image->getImageFrameArea(0).mPixelArea;
          return Point2I(pixelArea.mPixelWidth, pixelArea.mPixelHeight);
      }
   }

   return Parent::getPixelSize();
}

ConsoleMethod(LevelBuilderScrollerTool, setImage, void, 3, 3, "Sets the image map for the created scrollers.")
{
   ImageAsset* image = dynamic_cast<ImageAsset*>(Sim::findObject(argv[2]));
   if (image)
      object->setImageName(argv[2]);
   else
      Con::warnf("LevelBuilderScrollerTool::setImage - Invalid image map: %s", argv[2]);;
}


#endif // TORQUE_TOOLS
