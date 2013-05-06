//
//  IFFingerWindow.h
//  iphonefinger
//
//  Created by Daniel Grover on 3/25/08.
//  Copyright 2008 Wonder Warp Software SP. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "PFHand.h"
#import "NSImage+ProportionalScaling.h"

@interface PFHandWindow : NSWindow {
	PFHand *hand;
	float scaleFactor;
	NSImage *scaledUpImage;
	NSImage *scaledDownImage;
}

- (void)setImageWithMouseDown:(BOOL)isDown;
- (void) orientAroundPoint:(NSPoint)orientPoint;

@property(retain) PFHand *hand;
@property(assign) float scaleFactor;

@end