//
//  IFFingerWindow.m
//  iphonefinger
//
//  Created by Daniel Grover on 3/25/08.
//  Copyright 2008 Wonder Warp Software SP. All rights reserved.
//

#import "PFHandWindow.h"

@interface PFHandWindow()
@property(retain) NSImage *scaledUpImage;
@property(retain) NSImage *scaledDownImage;
@end

@implementation PFHandWindow
@synthesize scaledUpImage, scaledDownImage;

- (id)initWithContentRect:(NSRect)contentRect styleMask:(unsigned int)styleMask backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag{
    if (self = [super initWithContentRect:contentRect styleMask:0 backing:bufferingType defer:flag]){        
		[self setHasShadow:NO];
		[self setOpaque:NO];
		[self setIgnoresMouseEvents:YES];
		[self setLevel:NSScreenSaverWindowLevel];
		[self setCollectionBehavior:NSWindowCollectionBehaviorCanJoinAllSpaces];
		scaleFactor = 1;
        return self;
    }
	
	return nil;
}


- (PFHand *)hand {
    return hand; 
}

- (void)setHand:(PFHand *)theHand {
    if (hand != theHand) {
        [theHand retain];
        [hand release];
        hand = theHand;
		
		[self setScaleFactor:scaleFactor];
		[self setImageWithMouseDown:NO];
    }
}

- (float) scaleFactor{
	return scaleFactor;
}

- (void) setScaleFactor:(float) newScaleFactor{
	scaleFactor = newScaleFactor;
	[self _reloadImages];
	[self setFrame:NSMakeRect([self frame].origin.x, [self frame].origin.y, [scaledUpImage size].width, [scaledUpImage size].height) display:YES];
	[self setImageWithMouseDown:NO];
}

- (void) orientAroundPoint:(NSPoint)orientPoint{
	[self setFrameTopLeftPoint:NSMakePoint(orientPoint.x - hand.hotspot.x*scaleFactor, orientPoint.y + hand.hotspot.y*scaleFactor)];
}

- (void) _reloadImages{
	self.scaledUpImage = [hand.upImage imageByScalingByFactor:scaleFactor];
	self.scaledDownImage = [hand.downImage imageByScalingByFactor:scaleFactor];
}


- (void)setImageWithMouseDown:(BOOL)isDown{
	[self setBackgroundColor:[NSColor colorWithPatternImage:(isDown ? scaledDownImage : scaledUpImage)]];
	[self setHasShadow:NO];
}

- (void) dealloc{
	[hand release];
	[super dealloc];
}

@end