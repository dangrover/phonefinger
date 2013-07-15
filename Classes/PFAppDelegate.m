//
//  IFAppDelegate.m
//  PhoneFinger
//
//  Created by Daniel Grover on 3/25/08.
//  Copyright 2008 Wonder Warp Software LLC. All rights reserved.
//

#import "PFAppDelegate.h"
#import "NSWindow+Transforms.h"
#import "PFFingerprintWindow.h"

#define PFPrefs_ChosenHandModel @"handModel"
#define PFPrefs_ChosenScaleSliderValue @"scaleSliderValue"
#define PFPrefs_MakeSmudges @"makeSmudges"
#define PFPrefs_FirstRun @"firstRun"

@interface PFAppDelegate()
@property(retain) NSMutableArray *fingerprintWindows;
@property(retain) NSDictionary *availableHands;
- (void) _registerDefaultPrefs;
- (void) _reloadHandList;
- (void) _useHand:(PFHand *)theHand;
- (void) _createUserHandsPath;
@end


CGEventRef followEventCallback(CGEventTapProxy proxy, CGEventType type,  CGEventRef event, void *refcon){
	PFAppDelegate *del = [NSApp delegate];
	
	// TODO just check the event against a mask for each type
	
	if ( (type == kCGEventLeftMouseDown) || (type == kCGEventRightMouseDown) || (type == kCGEventOtherMouseDown)){
		[del mouseDown];
	}
	else if ( (type == kCGEventLeftMouseUp) || (type == kCGEventRightMouseUp) || (type == kCGEventOtherMouseUp)){
		[del mouseUp];
	}
	else if((type == kCGEventMouseMoved) || (type == kCGEventLeftMouseDragged) || (type == kCGEventRightMouseDragged)){
		[del follow];
	}
	
    return event;
}


@implementation PFAppDelegate
@synthesize fingerprintWindows, availableHands;

- (void) mouseUp{
	[fingerWindow setImageWithMouseDown:NO];
}

- (void) mouseDown{
	[fingerWindow setImageWithMouseDown:YES];
    
	if ([[NSUserDefaults standardUserDefaults] boolForKey:PFPrefs_MakeSmudges]){
        
		NSPoint loc = [NSEvent mouseLocation];
		for(PFFingerprintWindow *printWindow in self.fingerprintWindows){
			if(NSPointInRect(loc, [printWindow frame])){
				NSPoint windowCoord = [printWindow screenToWindowCoordinates:loc];
				
				[printWindow makePrintAtPoint:NSMakePoint(windowCoord.x,windowCoord.y + fingerWindow.hand.hotspot.y)
										scale: (0.3 + ((rand() / ((double)RAND_MAX + 1)) / 10.0)) * fingerWindow.scaleFactor
									  opacity:(rand() / ((double)RAND_MAX + 1))/5.0
										angle:0];
				return;
			}
		}
	}
}

- (void) follow{
	NSPoint loc = [NSEvent mouseLocation];
    
	[NSCursor hide];
	
	[fingerWindow orientAroundPoint:loc];
	
	lastX = loc.x;
	lastY = loc.y;
}


#pragma mark -

- (void) awakeFromNib{
	[self _registerDefaultPrefs];
	srand(time(NULL));
	[settingsPanel setShowsResizeIndicator:NO];
	[self _reloadHandList];
	
	if([[NSUserDefaults standardUserDefaults] boolForKey:PFPrefs_FirstRun]){
		[self _createUserHandsPath];
		[[NSUserDefaults standardUserDefaults] setBool:NO forKey:PFPrefs_FirstRun];
	}
	
	// Load the settings from the preferences
	NSString *lastUsedHandName = [[NSUserDefaults standardUserDefaults] valueForKey:PFPrefs_ChosenHandModel];
	if([availableHands valueForKey:lastUsedHandName]){
		[handSelector selectItemAtIndex:[[availableHands allKeys] indexOfObject:lastUsedHandName]];
	}
	
	[sizeSelector setFloatValue:[[[NSUserDefaults standardUserDefaults] valueForKey:PFPrefs_ChosenScaleSliderValue] floatValue]];
	
	[self newHandChosen:nil];
	
    
	// Set up fingerprints window for each screen.
	NSArray *screens = [NSScreen screens];
	self.fingerprintWindows = [NSMutableArray array];
	for(NSScreen *screen in screens){
		PFFingerprintWindow *printsWindow = [[PFFingerprintWindow alloc] initWithScreen:screen];
		[printsWindow makeKeyAndOrderFront:self];
		[self.fingerprintWindows addObject:[printsWindow autorelease]];
	}
	
	// Create an event tap to keep track of a bunch of mouse movements
	CFMachPortRef      eventTap;
    CGEventMask        eventMask;
    CFRunLoopSourceRef runLoopSource;
	
    eventMask = kCGEventMaskForAllEvents;
    eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, 0, eventMask, followEventCallback, NULL);
	
	if (!eventTap) {
        fprintf(stderr, "failed to create event tap\n");
		return;
    }
	
	// add the event tap to the run loop
    runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource,kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);
	
	// Let us hide the cursor when in the background
	CFStringRef propertyString = CFStringCreateWithCString(kCFAllocatorDefault,"SetsCursorInBackground",kCFStringEncodingUTF8);
	CFBooleanRef boolVal = kCFBooleanTrue;
	CGSSetConnectionProperty(_CGSDefaultConnection(), _CGSDefaultConnection(), propertyString, boolVal);
	CFRelease(propertyString);
	CFRelease(boolVal);
}


- (void) _reloadHandList{
	self.availableHands = [PFHand availableHands];
	[handSelector removeAllItems];
	for(unsigned i = 0; i < [[availableHands allKeys] count]; i++){
		NSString *handName = [[availableHands allKeys] objectAtIndex:i];
		[handSelector addItemWithTitle:handName];
	}
}

- (void) _useHand:(PFHand *)theHand{
	fingerWindow.hand = theHand;
	[self follow];
}


- (void) _registerDefaultPrefs{
	NSMutableDictionary *defaultPrefs = [NSMutableDictionary dictionary];
	[defaultPrefs setObject:[NSNumber numberWithBool:YES] forKey:PFPrefs_MakeSmudges];
	[defaultPrefs setObject:@"Default" forKey:PFPrefs_ChosenHandModel];
	[defaultPrefs setObject:[NSNumber numberWithFloat:0.5] forKey:PFPrefs_ChosenScaleSliderValue];
	[defaultPrefs setObject:[NSNumber numberWithBool:YES] forKey:PFPrefs_FirstRun];
	[[NSUserDefaults standardUserDefaults] registerDefaults:defaultPrefs];
}

#pragma mark -

- (IBAction) newHandChosen:(id)sender{
	NSString *handName = [[availableHands allKeys] objectAtIndex:[handSelector indexOfSelectedItem]];
	[self _useHand:[PFHand handFromBundleAtPath:[availableHands objectForKey:handName]]];
	[self newSizeChosen:nil];
	[[NSUserDefaults standardUserDefaults] setObject:handName forKey:PFPrefs_ChosenHandModel];
}

- (IBAction) newSizeChosen:(id)sender{
	[fingerWindow setScaleFactor:fingerWindow.hand.minScaleFactor + ([sizeSelector floatValue] * (fingerWindow.hand.maxScaleFactor - fingerWindow.hand.minScaleFactor))];
	[[NSUserDefaults standardUserDefaults] setObject:[NSNumber numberWithFloat:[sizeSelector floatValue]] forKey:PFPrefs_ChosenScaleSliderValue];
}

- (IBAction) wipeSmudges:(id)sender{
	for(PFFingerprintWindow *w in fingerprintWindows){
		[w clearPrints];
	}
}

- (IBAction) donate:(id)sender{
	[[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:@"http://www.wonderwarp.com/phonefinger/donate"]];
}

- (BOOL)application:(NSApplication *)theApplication openFile:(NSString *)filename{
	if([[filename pathExtension] isEqualToString:@"hand"]){
		NSString *name = [[filename pathComponents] objectAtIndex:[[filename pathComponents] count]-1];
		NSFileManager *fileManager = [NSFileManager defaultManager];
		
		NSString *movedPath = [[PFHand userHandsPath] stringByAppendingPathComponent:name];
		if([fileManager fileExistsAtPath:movedPath]){
			NSLog(@"Hand is already installed at %@",movedPath);
			return NO;
		}else{
			[self _createUserHandsPath];
			BOOL worked = [fileManager copyPath:filename toPath:movedPath handler:nil];
			if(worked){
                    [self _reloadHandList];
                    NSString *handName = [[availableHands allKeysForObject:movedPath] objectAtIndex:0];
                    [handSelector selectItemAtIndex:[[availableHands allKeys] indexOfObject:handName]];
                    [self newHandChosen:nil];
                    return YES;
			}else{
                    NSLog(@"Could not move hand: %@",filename);
                    return NO;
			}
		}
	}
	
	return NO;
}

- (void) _createUserHandsPath{
	NSFileManager *fileManager = [NSFileManager defaultManager];
	
	NSString *userHandsFolder    = [PFHand userHandsPath];
	NSString *phoneFingerAppSupportFolder = [userHandsFolder stringByDeletingLastPathComponent];
	
	if(![fileManager createDirectoryAtPath:phoneFingerAppSupportFolder attributes:nil]){
		NSLog(@"Could not create app support subdir at %@",phoneFingerAppSupportFolder);
		return;
	}
	
	if(![fileManager createDirectoryAtPath:userHandsFolder attributes:nil]){
		NSLog(@"Could not create user hands folder at %@",userHandsFolder);
	}
}

@end