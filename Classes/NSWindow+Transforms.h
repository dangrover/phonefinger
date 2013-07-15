//
//  NSWindow+Transforms.h
//  TypePad-Uploader
//
//  Created by Nicholas Gerakines on 4/28/06.
//  Copyright 2006 Wonder Warp Software LLC. All rights reserved.
//

#import "CoreGraphicsServices.h"
#import <AppKit/NSWindow.h>


@interface NSWindow (Transforms)

- (NSPoint)windowToScreenCoordinates:(NSPoint)point;
- (NSPoint)screenToWindowCoordinates:(NSPoint)point;

- (void)rotate:(double)radians;
- (void)rotate:(double)radians about:(NSPoint)point;

- (void)scaleX:(double)x Y:(double)y;
- (void)scaleX:(double)x Y:(double)y about:(NSPoint)point;

- (void)shearX:(double)x Y:(double)y;
- (void)shearX:(double)x Y:(double)y about:(NSPoint)point;

- (void)flipXAbout:(NSPoint)point;
- (void)flipYAbout:(NSPoint)point;

- (void)reset;


- (void) applyTransform:(CGAffineTransform)transform;
- (CGAffineTransform) rotationTransformForRadians:(double)radians;
- (CGAffineTransform) rotationTransformForPoint:(NSPoint)point radians:(double)radians;

@end
