//
//  PFHand.m
//  PhoneFinger
//
//  Created by Daniel Grover on 12/19/08.
//  Copyright 2008 Wonder Warp Software LLC. All rights reserved.
//

#import "PFHand.h"


#define PFHand_HandsFolderName @"Hands"

#define PFHand_DefaultMinScaleFactor 0.5
#define PFHand_DefaultMaxScaleFactor 1.0

#define PFHand_Attribute_Name @"PFHandName"
#define PFHand_Attribute_Author @"PFHandAuthor"
#define PFHand_Attribute_AuthorWebsite @"PFHandAuthorWebsite"
#define PFHand_Attribute_Description @"PFHandDescription"
#define PFHand_Attribute_UpImage @"PFHandUpImage"
#define PFHand_Attribute_DownImage @"PFHandDownImage"
#define PFHand_Attribute_HotspotX @"PFHandHotspotX"
#define PFHand_Attribute_HotspotY @"PFHandHotspotY"
#define PFHand_Attribute_MinScaleFactor @"PFHandMinScaleFactor"
#define PFHand_Attribute_MaxScaleFactor @"PFHandMaxScaleFactor"

@interface PFHand (Private)
+ (NSDictionary *)  _availableHandsWithinSearchPath:(NSString *)dirPath;
@end


@implementation PFHand
@synthesize name, upImage, downImage, hotspot, minScaleFactor, maxScaleFactor;

+ (NSDictionary *) availableHands{
	NSMutableDictionary *result = [NSMutableDictionary dictionary];
	[result setValuesForKeysWithDictionary:[PFHand _availableHandsWithinSearchPath:[PFHand builtInHandsPath]]]; 
	[result setValuesForKeysWithDictionary:[PFHand _availableHandsWithinSearchPath:[PFHand userHandsPath]]]; // override default hands
	return result;
}

+ (NSString *) userHandsPath{
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
    return [[[paths objectAtIndex:0] stringByAppendingPathComponent:@"PhoneFinger"] stringByAppendingPathComponent:PFHand_HandsFolderName];
	
}

+ (NSString *)builtInHandsPath{
	return [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:PFHand_HandsFolderName];
}

+ (NSDictionary *)  _availableHandsWithinSearchPath:(NSString *)dirPath{
	NSError *error = nil;
	NSMutableDictionary *available = [NSMutableDictionary dictionary];
	NSFileManager *fileManager = [NSFileManager defaultManager];
	
	// Verify dir exists.
	BOOL isDir = NO;
	if (![fileManager fileExistsAtPath:dirPath isDirectory:&isDir] || !isDir){
		return available;
	}
	
	// Grab all the paths to the .hands in the path.
	NSMutableArray *dirContents = [NSMutableArray arrayWithArray:[fileManager contentsOfDirectoryAtPath:dirPath error:&error]];
	if(error){ 
		NSLog(@"Error loading custom .hands: %@", error);
		return available;
	}
	
	[dirContents filterUsingPredicate:[NSPredicate predicateWithFormat:@"SELF endswith '.hand'"]];
	
	for(NSString *handName in dirContents){
		NSString *handPath = [dirPath stringByAppendingPathComponent:handName];
		NSString *infoPlistPath = [handPath stringByAppendingPathComponent:@"Info.plist"];
		if ([fileManager fileExistsAtPath:infoPlistPath]){
			NSDictionary *properties = [[NSString stringWithContentsOfFile:infoPlistPath] propertyList];
            
			NSString *handName;
			if(properties && (handName = [properties valueForKey:PFHand_Attribute_Name])){
				[available setObject:handPath forKey:handName];
			}
		}
	}
	
	return available;
}


+ (PFHand *)handFromBundleAtPath:(NSString *)pathToBundle{
	BOOL isDir = NO;
	NSFileManager *fileManager = [NSFileManager defaultManager];
	
	if (![fileManager fileExistsAtPath:pathToBundle isDirectory:&isDir] || !isDir){
		NSLog(@"Could not load hand at %@.",pathToBundle);
		return nil;
	}
	
	NSString *infoPlistPath = [pathToBundle stringByAppendingPathComponent:@"Info.plist"];
	if(![fileManager fileExistsAtPath:infoPlistPath]){
		NSLog(@"Info.plist expected at %@",infoPlistPath);
		return nil;
	}
	
	NSString *plistContents = [NSString stringWithContentsOfFile:infoPlistPath];
	NSDictionary *plist     = [plistContents propertyList];
	
	if(!plist){
		NSLog(@"Corrupt Info.plist at %@",infoPlistPath);
		return nil;
	}
	
	PFHand *hand = [[[PFHand alloc] init] autorelease];
	
	[hand setName:[plist valueForKey:PFHand_Attribute_Name]];
	[hand setUpImage:[[[NSImage alloc] initWithContentsOfFile:[pathToBundle stringByAppendingPathComponent:[plist valueForKey:PFHand_Attribute_UpImage]]] autorelease]];
	[hand setDownImage:[[[NSImage alloc] initWithContentsOfFile:[pathToBundle stringByAppendingPathComponent:[plist valueForKey:PFHand_Attribute_DownImage]]] autorelease]];
	
	NSPoint hotspot;
	hotspot.x = [[plist valueForKey:PFHand_Attribute_HotspotX] floatValue];
	hotspot.y = [[plist valueForKey:PFHand_Attribute_HotspotY] floatValue];
	[hand setHotspot:hotspot];
	
	// Optional custom scale factor
	if([plist objectForKey:PFHand_Attribute_MinScaleFactor] && [plist objectForKey:PFHand_Attribute_MaxScaleFactor]){
		hand.minScaleFactor = [[plist valueForKey:PFHand_Attribute_MinScaleFactor] floatValue];
		hand.maxScaleFactor = [[plist valueForKey:PFHand_Attribute_MaxScaleFactor] floatValue];
	}

	return hand;
}

- (id) init{
	if(self = [super init]){
		minScaleFactor = PFHand_DefaultMinScaleFactor;
		maxScaleFactor = PFHand_DefaultMaxScaleFactor;
	}
	
	return self;
}

- (void)dealloc {
    [upImage release];
    [downImage release];
    [name release];
    [super dealloc];
}

@end