//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

ConsoleMethod(ImageFont, setImage, bool, 3, 3,   "(imageAssetId) - Sets the image asset to use..\n"
                                                        "@param imageName The image asset to use.\n"
                                                        "@return Returns true on success.")
{
    // Set Image.
    return object->setImage( argv[2] );
}

//-----------------------------------------------------------------------------

ConsoleMethod(ImageFont, getImage, const char*, 2, 2,    "() - Gets current image asset..\n"
                                                                "@return The current image asset.")
{
    // Get Image.
    return object->getImage();
}

//-----------------------------------------------------------------------------

ConsoleMethod(ImageFont, setText, void, 3, 3, "(text) - Set the text to render.\n")
{
    object->setText(argv[2]);
}

//-----------------------------------------------------------------------------

ConsoleMethod(ImageFont, getText, const char*, 2, 2, "() - Gets the text being rendered.\n")
{
    return object->getText().getPtr8();
}

//-----------------------------------------------------------------------------

ConsoleMethod(ImageFont, setTextAlignment, void, 3, 3,    "(alignment) - Set the text alignment to 'left', 'center' or 'right'.\n"
                                                                    "@param alignment The text alignment of 'left', 'center' or 'right'.\n"
                                                                    "@return No return value.")
{

    object->setTextAlignment( ImageFont::getTextAlignmentEnum(argv[2]) );
}

//-----------------------------------------------------------------------------

ConsoleMethod(ImageFont, getTextAlignment, const char*, 2, 2, "() - Gets the text alignment.\n"
                                                                        "@return The text alignment of 'left', 'center' or 'right'.")
{
    return ImageFont::getTextAlignmentDescription(object->getTextAlignment());
}
//-----------------------------------------------------------------------------

ConsoleMethod(ImageFont, setCharacterSize, void, 3, 4,    "(width, height) - Set the size of the rendered characters.\n"
                                                                    "@param width The width of a rendered character.\n"
                                                                    "@param height The height of a rendered character.\n"
                                                                    "@return No return value.")
{
   F32 width, height;

   U32 elementCount = Utility::mGetStringElementCount(argv[2]);

   // ("width height")
   if ((elementCount == 2) && (argc == 3))
   {
      width = dAtof(Utility::mGetStringElement(argv[2], 0));
      height = dAtof(Utility::mGetStringElement(argv[2], 1));
   }

   // (width, [height])
   else if (elementCount == 1)
   {
      width = dAtof(argv[2]);

      if (argc > 3)
         height = dAtof(argv[3]);
      else
         height = width;
   }

   // Invalid
   else
   {
      Con::warnf("SceneObject::setCharacterSize() - Invalid number of parameters!");
      return;
   }

   // Set character size.
   object->setCharacterSize(Vector2(width, height));

}

//-----------------------------------------------------------------------------

ConsoleMethod(ImageFont, getCharacterSize, const char*, 2, 2, "() - Gets the size of the rendered characters.\n"
                                                                        "@return The size of the rendered characters.")
{
    return object->getCharacterSize().scriptThis();
}

//-----------------------------------------------------------------------------

ConsoleMethod(ImageFont, setCharacterPadding, void, 3, 3, "(padding) - Set the character padding.\n"
                                                                    "@param padding The space added in-between characters.\n"
                                                                    "@return No return value.")
{
   // Set character padding.
   object->setCharacterPadding( dAtoi(argv[2]) );

}

//-----------------------------------------------------------------------------

ConsoleMethod(ImageFont, getCharacterPadding, S32, 2, 2, "() - Gets the character padding.\n"
                                                                        "@return The character padding.")
{
    return object->getCharacterPadding();
}