//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

@interface T2DAppDelegate : NSObject <UIApplicationDelegate, GLKViewDelegate, GLKViewControllerDelegate> {
	IBOutlet UIWindow *window;
}

@property (nonatomic, retain) UIWindow *window;

@end
