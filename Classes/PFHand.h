//
//  PFHand.h
//  PhoneFinger
//
//  Created by Daniel Grover on 12/19/08.
//  Copyright 2008 Wonder Warp Software LLC. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface PFHand : NSObject {
	NSImage *upImage;
	NSImage *downImage;
	NSPoint hotspot;
	NSString *name;
	float minScaleFactor;
	float maxScaleFactor;
}

+ (NSDictionary *) availableHands; // returns dictionary of available hands with friendly names and paths
+ (PFHand *)handFromBundleAtPath:(NSString *)pathToBundle;

+ (NSString *) userHandsPath;
+ (NSString *) builtInHandsPath;

@property(retain) NSImage *upImage;
@property(retain) NSImage *downImage;
@property(assign) NSPoint hotspot;
@property(retain) NSString *name;
@property(assign) float minScaleFactor;
@property(assign) float maxScaleFactor;
@end