//
//  AppDelegate.h
//  iTorque2D
//
//  Created by QA Mac1 on 2/2/12.
//  Copyright (c) 2012 GarageGames. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

@interface T2DAppDelegate : NSObject <UIApplicationDelegate, GLKViewDelegate, GLKViewControllerDelegate> {
	IBOutlet UIWindow *window;
}

@property (nonatomic, retain) UIWindow *window;

@end
