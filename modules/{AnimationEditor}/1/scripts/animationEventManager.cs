//-----------------------------------------------------------------------------
// 3 Step Studio
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------

// Builds the event manager and script listener that will be responsible for
// handling important system events, such as a module group finishing a load,
// a patch being available, a download completing, etc
function initializeAnimBuilderEventManager()
{
    if (!isObject(AnimBuilderEventManager))
    {
        $AnimBuilderEventManager = new EventManager(AnimBuilderEventManager)
        { 
            queue = "AnimBuilderEventManager"; 
        };
        
        // Module related signals
        AnimBuilderEventManager.registerEvent("_CellPaneUpdateComplete");
        AnimBuilderEventManager.registerEvent("_TimelineDeleteRequest");
    }
    
    if (!isObject(AnimBuilderListener))
    {
        $AnimBuilderListener = new ScriptMsgListener(AnimBuilderListener) 
        { 
            class = "AnimBuilder"; 
        };
        
        // Module related subscriptions
        AnimBuilderEventManager.subscribe(AnimBuilderListener, "_CellPaneUpdateComplete", "onCellPaneUpdateComplete");
        AnimBuilderEventManager.subscribe(AnimBuilderListener, "_TimelineDeleteRequest", "onTimelineDeleteRequest");
    }
}

// Cleanup the AnimBuilderEventManager
function destroyAnimBuilderEventManager()
{
    if (isObject(AnimBuilderEventManager) && isObject(AnimBuilderListener))
    {
        // Remove all the subscriptions
        AnimBuilderEventManager.remove(AnimBuilderListener, "_StartUpComplete");

        // Delete the actual objects
        AnimBuilderEventManager.delete();
        AnimBuilderListener.delete();
        
        // Clear the global variables, just in case
        $AnimBuilderEventManager = "";
        $AnimBuilderListener = "";
    }
}

function AnimBuilder::onCellPaneUpdateComplete(%this, %msgData)
{
    %obj = getWord(%msgData, 0);
    %count = getWord(%msgData, 1);
    %time = (%count / 10);
    %wait = (%time < 32 ? 32 : %time);
    %obj.schedule(%wait, "update", %count);
}

function AnimBuilder::onCellPaneUpdateComplete(%this, %msgdata)
{
    AnimationBuilder.schedule(64, updateGui);
}