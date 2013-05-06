//
//  NSImage+ProportionalScaling.h
//  PhoneFinger
//
//  Created by Daniel Grover on 12/20/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface NSImage (ProportionalScaling)
- (NSImage *)imageByScalingProportionallyToSize:(NSSize)targetSize;
- (NSImage *)imageByScalingByFactor:(float)factor;
@end