//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------
#import "platformOSX/platformOSX.h"

@implementation NSSavePanel (TorqueFileDialogs)

//-----------------------------------------------------------------------------
#pragma message ("NSSavePanel::setFilters not yet implemented")
-(void)setFilters:(StringTableEntry)filters
{

}

//-----------------------------------------------------------------------------
#pragma message ("NSSavePanel::setDefaultAttributes not yet implemented")
-(void)setDefaultAttributes
{

}

//-----------------------------------------------------------------------------
#pragma message ("NSSavePanel::showSavePanel not yet implemented")
+(NSArray *) showSavePanel
{
    NSMutableArray *array = [NSMutableArray arrayWithCapacity:10];

    return array;
}
@end

@implementation  NSOpenPanel (TorqueFileDialogs)

//-----------------------------------------------------------------------------
#pragma message ("NSOpenPanel::setDefaultAttributes not yet implemented")
-(void) setDefaultAttributes
{

}

//-----------------------------------------------------------------------------
#pragma message ("NSOpenPanel::showOpenPanel not yet implemented")
+(NSArray *) showOpenPanel
{
    NSMutableArray *array = [NSMutableArray arrayWithCapacity:10];

    return array;
}

@end