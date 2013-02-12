//
//  MSAppDelegate.m
//  MSWeakTimer-SampleProject
//
//  Created by Javier Soto on 2/12/13.
//  Copyright (c) 2013 MindSnacks. All rights reserved.
//

#import "MSAppDelegate.h"

#import "MSSampleViewController.h"

@implementation MSAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];

    MSSampleViewController *vc = [[MSSampleViewController alloc] init];
    self.window.rootViewController = vc;

    [self.window makeKeyAndVisible];

    return YES;
}

@end
