//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function StoryboardWindow::onAdd(%this)
{
   %this.scene = new Scene();
   %this.staticSpriteGroup = new SimGroup();
   %this.height = 10;
   
   %this.setScene(%this.scene);
   //%this.scene.setDebugOn(0);
   %this.UseObjectInputEvents = true;
}

function StoryboardWindow::onRemove(%this)
{
   if (isObject(%this.scene))
      %this.scene.delete();
      
   if (isObject(%this.staticSpriteGroup))
      %this.staticSpriteGroup.delete();
}

function ImageStoryboardWindow::update(%this, %imageAssetId, %frames)
{
    %image = AssetDatabase.acquireAsset(%imageAssetId);

    %this.staticSpriteGroup.deleteContents();

    %count = getWordCount(%frames);
    %spacing = %image.getFrameSize(0) / 4;

    %height = %image.getFrameSize(0);
    %width = %spacing;

    for (%i = 0; %i < %count; %i++)
    {
        %frame = getWord(%frames, %i);
        %sprite = new sprite()
        {
            scene = %this.scene;
            class = "ImageStoryboardSprite";
            image = %imageAssetId;
            frame = %frame;
            size = %image.getFrameSize(0);
            UseInputEvents = true;
            frameNumber = %i;
            window = %this;
        };

        %this.staticSpriteGroup.add(%sprite);

        %spriteWidth = getWord(%sprite.size, 0);
        %sprite.position = %width + (%spriteWidth / 2) SPC "0";
        %width += %spriteWidth + %spacing;

        %newHeight = getWord(%sprite.size, 1);
        if (%newHeight > %height)
            %height = %newHeight;
    }

    %dropSpot = new SceneObject()
    {
        scene = %this.scene;
        size = %image.getFrameSize(0);
    };

    %this.staticSpriteGroup.add(%dropSpot);
    %dropSpot.position = %width + (getWord(%dropSpot.size, 0) / 2) SPC "0";

    %width += %image.getFrameSize(0);

    %this.setCurrentCameraArea(0, 0 - (%height / 2), %width, %height / 2);

    %ar = %width / %height;
    %windowHeight = getWord(%this.extent, 1);
    %windowSize = resolveAspectRatio(%ar, %windowHeight);
    %windowWidth = mFloatLength(getWord(%windowSize, 0), 0);

    %this.setExtent(%windowWidth, %windowHeight);

    %this.objectWidth = getWord(%image.getFrameSize(0), 0);
    %this.spacing = %spacing;

    AssetDatabase.releaseAsset(%imageAssetId);
}

function ImageStoryboardSprite::onTouchDragged(%this, %modifier, %position, %clicks)
{
    %windowPos = %this.window.getWindowPoint(%position);
    %mousePoint = %this.window.getCanvasPoint(%windowPos);

    %position = %mousePoint;
    %halfParentWidth = %this.getSize().x / 2;
    %halfParentHeight = %this.getSize().y / 2;
    %position.x -= %halfParentWidth;
    %position.y -= %halfParentHeight;
    AnimationBuilder.createDraggingControl(%this, %position, %mousePoint, %this.Size);
}
