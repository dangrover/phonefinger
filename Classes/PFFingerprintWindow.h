//
//  IFFingerprintWindow.h
//  PhoneFinger
//
//  Created by Daniel Grover on 6/5/08.
//  Copyright 2008 Wonder Warp Software LLC. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface PFFingerprintWindow : NSWindow {
	NSImage *_slate;
	NSImage *_fingerPrint;
}

- (id)initWithScreen:(NSScreen *)screen;
- (void) makePrintAtPoint:(NSPoint)point scale:(float)scale opacity:(float)opacity angle:(float)angle;
- (void) clearPrints;

@end
