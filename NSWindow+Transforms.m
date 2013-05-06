//
//  NSWindow+Transforms.m
//  TypePad-Uploader
//
//  Created by Nicholas Gerakines on 4/28/06.
//  Copyright 2006 Wonder Warp Software LLC. All rights reserved.
//

#import "NSWindow+Transforms.h"


@implementation NSWindow (Transforms)

// Need to test these methods on a dual-display config - I suspect they're very very wrong.  I don't think each screen has a unique co-ordinate system - we *should* actually be returning the absolute co-ords, not adjusting them for whatever screen we're on... maybe.

- (NSPoint)windowToScreenCoordinates:(NSPoint)point {
    NSPoint result;
    NSRect screenFrame = [[self screen] frame];
    
    //result = [self convertBaseToScreen:point]; // Doesn't work... it looks like the y co-ordinate is not inverted as necessary
    
    result.x = screenFrame.origin.x + _frame.origin.x + point.x;
    result.y = screenFrame.origin.y + screenFrame.size.height - (_frame.origin.y + point.y);
    
    return result;
}

- (NSPoint)screenToWindowCoordinates:(NSPoint)point { // Doesn't work with the transform stuff
    NSPoint result;
    NSRect screenFrame = [[self screen] frame];
    CGPoint a, b;
    
    result.x = point.x - _frame.origin.x - screenFrame.origin.x;
    result.y = _frame.origin.y + _frame.size.height - point.y - screenFrame.origin.y;
    
    a.x = point.x, a.y = point.y;
    
    CGAffineTransform original;
    CGSGetWindowTransform(_CGSDefaultConnection(), _windowNum, &original);
    
    b = CGPointApplyAffineTransform(a, original);
    
    return NSMakePoint(b.x, b.y);
    //return result;
}

- (void)rotate:(double)radians {
    [self applyTransform: [self rotationTransformForRadians:radians]];
}

- (void)rotate:(double)radians about:(NSPoint)point {
	[self applyTransform: [self rotationTransformForPoint:point radians:radians]];
}

- (void) applyTransform:(CGAffineTransform)transform{
	CGSSetWindowTransform(_CGSDefaultConnection(), _windowNum, transform);
}


- (CGAffineTransform) rotationTransformForRadians:(double)radians{
	return [self rotationTransformForPoint:NSMakePoint(_frame.size.width / 2.0, _frame.size.height / 2.0) radians:radians];
}



- (CGAffineTransform) rotationTransformForPoint:(NSPoint)point radians:(double)radians{
	CGAffineTransform original;
    NSPoint rotatePoint = [self windowToScreenCoordinates:point];
	
    CGSGetWindowTransform(_CGSDefaultConnection(), _windowNum, &original);
    
    original = CGAffineTransformTranslate(original, rotatePoint.x, rotatePoint.y);
    original = CGAffineTransformRotate(original, -radians);
    original = CGAffineTransformTranslate(original, -rotatePoint.x, -rotatePoint.y);
	
	return original;
}


- (void)scaleX:(double)x Y:(double)y {
    [self scaleX:x Y:y about:NSMakePoint(_frame.size.width / 2.0, _frame.size.height / 2.0)];
}

- (void)scaleX:(double)x Y:(double)y about:(NSPoint)point {
    CGAffineTransform original;
    NSPoint scalePoint = [self windowToScreenCoordinates:point];
    
    CGSGetWindowTransform(_CGSDefaultConnection(), _windowNum, &original);
    
    original = CGAffineTransformTranslate(original, scalePoint.x, scalePoint.y);
    original = CGAffineTransformScale(original, 1.0 / x, 1.0 / y);
    original = CGAffineTransformTranslate(original, -scalePoint.x, -scalePoint.y);
    
    CGSSetWindowTransform(_CGSDefaultConnection(), _windowNum, original);
}

- (void)shearX:(double)x Y:(double)y {
    [self shearX:x Y:y about:NSMakePoint(_frame.size.width / 2.0, _frame.size.height / 2.0)];
}

- (void)shearX:(double)x Y:(double)y about:(NSPoint)point {
    CGAffineTransform original, shear;
    NSPoint shearPoint = [self windowToScreenCoordinates:point];
    
    CGSGetWindowTransform(_CGSDefaultConnection(), _windowNum, &original);
    
    original = CGAffineTransformTranslate(original, shearPoint.x, shearPoint.y);
    
    shear.a = 1.0;
    shear.b = tan(y);
    shear.c = -tan(x);
    shear.d = 1.0;
    shear.tx = 0.0;
    shear.ty = 0.0;
    
    original = CGAffineTransformConcat(CGAffineTransformInvert(shear), original);
	
    original = CGAffineTransformTranslate(original, -shearPoint.x, -shearPoint.y);
    
    CGSSetWindowTransform(_CGSDefaultConnection(), _windowNum, original);
}

- (void)flipXAbout:(NSPoint)point {
    CGAffineTransform original;
    NSPoint shearPoint = [self windowToScreenCoordinates:point];
    
    CGSGetWindowTransform(_CGSDefaultConnection(), _windowNum, &original);
    
    original = CGAffineTransformTranslate(original, shearPoint.x, shearPoint.y);
    original.a = -original.a;
    original = CGAffineTransformTranslate(original, -shearPoint.x, -shearPoint.y);
    
    CGSSetWindowTransform(_CGSDefaultConnection(), _windowNum, original);
}

- (void)flipYAbout:(NSPoint)point {
    CGAffineTransform original;
    NSPoint shearPoint = [self windowToScreenCoordinates:point];
    
    CGSGetWindowTransform(_CGSDefaultConnection(), _windowNum, &original);
    
    original = CGAffineTransformTranslate(original, shearPoint.x, shearPoint.y);
    original.d = -original.d;
    original = CGAffineTransformTranslate(original, -shearPoint.x, -shearPoint.y);
    
    CGSSetWindowTransform(_CGSDefaultConnection(), _windowNum, original);
}

- (void)reset {
    // Note that this is not quite perfect... if you transform the window enough it may end up anywhere on the screen, but resetting it plonks it back where it started, which may correspond to it's most-logical position at that point in time.  Really what needs to be done is to reset the current transform matrix, in all places except it's translation, such that it stays roughly where it currently is.
	
    NSPoint point = [self windowToScreenCoordinates:NSMakePoint(0.0, _frame.size.height)]; // Get the screen position of the top left corner, by which our window is positioned
    
    CGSSetWindowTransform(_CGSDefaultConnection(), _windowNum, CGAffineTransformMakeTranslation(-point.x, -point.y));
}

@end