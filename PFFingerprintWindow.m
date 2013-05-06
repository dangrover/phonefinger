//
//  IFFingerprintWindow.m
//  iphonefinger
//
//  Created by Daniel Grover on 6/5/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "PFFingerprintWindow.h"

@interface PFFingerprintWindow()
- (void) _update;
@property(retain) NSImage *_fingerPrint;
@property(retain) NSImage *_slate;
@end

@implementation PFFingerprintWindow
@synthesize _slate, _fingerPrint;

- (id)initWithScreen:(NSScreen *)screen {
	int styleMask = 0;
    if (self = [super initWithContentRect:[NSWindow contentRectForFrameRect:[screen visibleFrame] styleMask:styleMask] styleMask:styleMask backing:NSBackingStoreBuffered defer:NO]){        
		
		[self setHasShadow:NO];
		[self setOpaque:NO];
		[self setIgnoresMouseEvents:YES];
		[self setLevel:NSScreenSaverWindowLevel];
		
		[self setCollectionBehavior:NSWindowCollectionBehaviorCanJoinAllSpaces];
		[self clearPrints];
		
		self._fingerPrint = [NSImage imageNamed:@"fingerprint1.png"];
		//[self makePrintAtPoint:NSMakePoint(300,300) scale:0.5 opacity:0.5 angle:25];
		[self _update];
		//NSLog(@"my frame is at %f,%f,%f,%f",[self frame].origin.x,[self frame].origin.y,[self frame].size.width,[self frame].size.height);
        return self;
    }
	
	return nil;
}


- (void) makePrintAtPoint:(NSPoint)point scale:(float)scale opacity:(float)opacity angle:(float)angle{
	[self._slate lockFocus];
	
	/*NSAffineTransform* xform = [NSAffineTransform transform];
	[xform rotateByDegrees:angle];
	[xform concat];*/
	
	NSSize printSize = [_fingerPrint size];
	NSSize scaledPrintSize = printSize;
	scaledPrintSize.width *= scale;
	scaledPrintSize.height *= scale;
	
	[self._fingerPrint drawInRect:NSMakeRect(point.x - (scaledPrintSize.width/2), point.y - (printSize.height/2),scaledPrintSize.width,scaledPrintSize.height)
				fromRect:NSMakeRect(0,0,printSize.width,printSize.height) 
			operation:NSCompositeSourceOver
			fraction:opacity];
	
	[self._slate unlockFocus];
	[self _update];
}

- (void) clearPrints{
	//NSLog(@"clearing prints");
	self._slate = [[[NSImage alloc] initWithSize:[self frame].size] autorelease];
	[_slate lockFocus];
	[[NSColor clearColor] set];
	[NSBezierPath fillRect:NSMakeRect(0,0,[self frame].size.width,[self frame].size.height)];
	[_slate unlockFocus];
	[self _update];
}

#pragma mark -
- (void) _update{
	[self setBackgroundColor:[NSColor colorWithPatternImage:self._slate]];
	[self setHasShadow:NO];
	[self display];
}

@end
