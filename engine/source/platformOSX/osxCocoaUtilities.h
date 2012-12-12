//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------

@interface NSSavePanel (TorqueFileDialogs)
-(void) setFilters:(StringTableEntry)filters;
-(void) setDefaultAttributes;
+(NSArray *) showSavePanel;
@end

@interface NSOpenPanel (TorqueFileDialogs)
-(void) setDefaultAttributes;
+(NSArray *) showOpenPanel;
@end
