//
//  IFAppDelegate.h
//  iphonefinger
//
//  Created by Daniel Grover on 3/25/08.
//  Copyright 2008 Wonder Warp Software SP. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "PFHandWindow.h"
#import "PFHand.h"

@interface PFAppDelegate : NSObject {
	IBOutlet PFHandWindow *fingerWindow;
	IBOutlet NSPanel *settingsPanel;
	IBOutlet NSPopUpButton *handSelector;
	IBOutlet NSSlider *sizeSelector;
	float lastProp;
	float lastX;
	float lastY;
	NSMutableArray *fingerprintWindows;
	NSDictionary *availableHands;
}


- (void) follow;
- (void) mouseUp;
- (void) mouseDown;

- (IBAction) newSizeChosen:(id)sender;
- (IBAction) newHandChosen:(id)sender;
- (IBAction) wipeSmudges:(id)sender;
- (IBAction) donate:(id)sender;
@end