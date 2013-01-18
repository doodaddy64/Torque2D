//-----------------------------------------------------------------------------
// 3 Step Studio
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------

function SpriteBase::setAsset(%this, %asset)
{
    %assetType = AssetDatabase.getAssetType(%asset);
    
    if (%assetType $= "ImageAsset")
    {
        %this.setImage(%asset);
    }
    else if (%assetType $= "AnimationAsset")
    {
        %this.animation = %asset;
        %this.playAnimation(%asset);
    }
    else
    {
        warn("Sprite::setAsset -- invalid asset: " @ %asset); 
        warn("Asset type is " @ %assetType @ ", must be either ImageAsset or AnimationAsset"); 
        return; 
    }
}

function SpriteBase::getAsset(%this)
{
    if (%this.animation !$= "")
        return strchr(%this.Animation, "{");
    else if (%this.getImage() !$= "")
        return strchr(%this.getImage(), "{");
    else
        warn("Sprite::getAsset -- No asset found on Sprite " @ %this);
}

function Sprite::setSizeFromAsset(%this, %asset, %metersPerPixel)
{
    %sizePixels = "";
    
    %assetType = AssetDatabase.getAssetType(%asset);
    
    if (%assetType $= "ImageAsset")
    {
        %imageAsset = AssetDatabase.acquireAsset(%asset);
        %sizePixels = %imageAsset.getFrameSize(0);
        AssetDatabase.releaseAsset(%imageAsset.getAssetId());
    }
    else if (%assetType $= "AnimationAsset")
    {
        %animationAsset = AssetDatabase.acquireAsset(%asset);
        %animationImageAsset = AssetDatabase.acquireAsset(strchr(%animationAsset.image, "{"));
        %sizePixels = %animationImageAsset.getFrameSize(0);
        AssetDatabase.releaseAsset(%animationImageAsset.getAssetId());
        AssetDatabase.releaseAsset(%animationAsset.getAssetId());
    }
    else
    {
        warn("Sprite::setSizeFromAsset -- invalid asset: " @ %asset); 
        warn("Asset type is " @ %assetType @ ", must be either ImageAsset or AnimationAsset"); 
        return; 
    }   
    
    // Set the size of the sprite by converting the pixel size to world units
    %sizeWorld = Vector2Scale(%sizePixels, %metersPerPixel);
    %this.setSize(%sizeWorld);
}