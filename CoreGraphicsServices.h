//
//  CoreGraphicsServices.h
//  TypePad-Uploader
//
//  Created by Nicholas Gerakines on 4/28/06.
//  Copyright 2006 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#ifndef CORE_GRAPHICS_SERVICES_H
#define CORE_GRAPHICS_SERVICES_H

#include <Carbon/Carbon.h> /* for ProcessSerialNumber */


enum { // Error codes returned by CoreGraphics functions... implied meanings may be different from actual
    kCGSInvalidWID = 1001,
    kCGSInvalidCID = 1002
};

typedef int CGSWindow; /* Note that CGS can retrieve a particular window's CGSConnection automatically, given a CGSWindow, but many functions do not do this - unless explicitly stated, all CGSConnection arguments must be provided and valid */
typedef int CGSConnection;
typedef void* CGSValue;
typedef void* CGSRegion;
typedef void* CGSBoundingShape;
typedef void* CGSAnimationRef; /* Ref really is a pointer to some other structure.  *ref + 40 = window ID */

typedef struct _CGSMeshCell {
    float unknown1[8]; // Judging by the lineEq function, {0, 1} -> {4, 5}, {2, 1} -> {6, 5}, {3, 2} -> {7, 5}, etc... no sense.
    char unknown2[298];
} CGSMeshCell;

typedef struct _CGSMesh {
    unsigned int numberOfCells;
    unsigned int unknown; // Set to zero in meshAlloc
    CGSMeshCell cells[];
} CGSMesh;

typedef enum _CGSWindowOrderingMode {
    kCGSOrderAbove  =  1,
    kCGSOrderBelow  = -1,
    kCGSOrderOut    =  0
} CGSWindowOrderingMode;

typedef enum _CGSTransitionType {
    CGSNone = 0,    // No transition effect.
    CGSFade,        // Cross-fade.
    CGSZoom,        // Zoom/fade towards us.
    CGSReveal,      // Reveal new desktop under old.
    CGSSlide,       // Slide old out and new in.
    CGSWarpFade,    // Warp old and fade out revealing new.
    CGSSwap,        // Swap desktops over graphically.
    CGSCube,        // The well-known cube effect.
    CGSWarpSwitch   // Warp old, switch and un-warp.
} CGSTransitionType;

typedef enum _CGSTransitionOption {
    CGSDown,                // Old desktop moves down.
    CGSLeft,                // Old desktop moves left.
    CGSRight,               // Old desktop moves right.
    CGSInRight,             // CGSSwap: Old desktop moves into screen, 
                            //                      new comes from right.
    CGSBottomLeft = 5,      // CGSSwap: Old desktop moves to bl,
                            //                      new comes from tr.
    CGSBottomRight,         // Old desktop to br, New from tl.
    CGSDownTopRight,        // CGSSwap: Old desktop moves down, new from tr.
    CGSUp,                  // Old desktop moves up.
    CGSTopLeft,             // Old desktop moves tl.
    
    CGSTopRight,            // CGSSwap: old to tr. new from bl.
    CGSUpBottomRight,       // CGSSwap: old desktop up, new from br.
    CGSInBottom,            // CGSSwap: old in, new from bottom.
    CGSLeftBottomRight,     // CGSSwap: old one moves left, new from br.
    CGSRightBottomLeft,     // CGSSwap: old one moves right, new from bl.
    CGSInBottomRight,       // CGSSwap: onl one in, new from br.
    CGSInOut                // CGSSwap: old in, new out.
} CGSTransitionOption;

typedef enum _CGSWindowTag {
    CGSTagExposeFade    = 0x0002,   // Fade out when Expose activates.
    CGSTagNoShadow      = 0x0008,   // No window shadow.
    CGSTagTransparent   = 0x0200,   // Transparent to mouse clicks.
    CGSTagSticky        = 0x0800,   // Appears on all workspaces.
} CGSWindowTag;


typedef struct _CGSTransitionSpec {
    uint32_t unknown1;
    CGSTransitionType type;
    CGSTransitionOption option;
    CGSWindow wid; /* Can be 0 for full-screen */
    float *backColour; /* Null for black otherwise pointer to 3 float array with RGB value */
} CGSTransitionSpec;


#define kCGSNullConnectionID ((CGSConnection)0)


/*! @function meshAlloc
@abstract Allocates a new mesh structure with a given number of cells.
@discussion No idea what the cells are at this point, nor how these meshes can be used.
@param numberOfCells The number of cells in the mesh.
@result Returns a new mesh if successful, NULL otherwise. */

extern CGSMesh* meshAlloc(unsigned int numberOfCells);

/*! @function meshAllocWithVertexMap
@abstract Allocates a new mesh with cells based on the given vertex map.
@discussion The vertexes is an array of 8*N floats, where each set of 8 floats (32 bytes) is copied into it's own cell within th resulting mesh (after being swapped around in an odd fashion).  The 2nd & 3rd parameters are probably the dimensions of the vertex array (X * Y), although this is only a guess.  It seems like unknown3 must always be greater than 0, while unknown2 may be greater than or equal to 0. */

extern CGSMesh* meshAllocWithVertexMap(float *vertexes, int unknown2, int unknown3);

/*! @function meshCellInitialize
@abstract Initialises a CGSMeshCell (details unknown).
@discussion Calls lineEq twelve times, with different permutations of the cell's initial 8 points... no idea what the logic is.  Does a whole lot more as well, but I haven't even tried to decipher that, as yet. */

extern void meshCellInitialize(CGSMeshCell *cell);

extern void meshTransformShape(int unknown1, int unknown2, int unknown3, int unknown4);

extern OSStatus CGSSetWindowWarp(const CGSConnection cid, const CGSWindow wid /* Cannot be NULL; probably the window ID */, int unknown2 /* Probably X dimension */, int unknown3 /* Probably Y dimension */, void *unknown4 /* If zero then unknown2 & unknown3 are both treated as zero - presumably a pointer to the relevant mesh/vertex/whatever info*/);

/*! @function _CGSConnectionByID
@abstract Returns a pointer to the internal window server structure for a given connection ID.
@discussion The returned struct contains at least the connection ID itself (offset by 4) as well as a pointer to the next such struct in a singly-linked list (offset by 0).  Presumably the rest of the connection info is stored in the rest of the struct, which is at present of unknown size.
@param cid The connection ID who's struct you wish to look up.
@result Returns a pointer to the connection's struct, or NULL if an invalid connection ID is provided. */

extern void* _CGSConnectionByID(const CGSConnection cid);


extern CGSConnection _CGSDefaultConnection(void);

extern CGSValue CGSCreateCStringNoCopy(const char *str);
extern char* CGSCStringValue(CGSValue string);
extern int CGSIntegerValue(CGSValue intVal);

extern void CGSReenableUpdate(const CGSConnection cid);
extern void CGSDisableUpdate(const CGSConnection cid);

extern OSStatus CGSSetWindowTransforms(const CGSConnection cid, CGSWindow *wid, CGAffineTransform *transform, int n);
extern OSStatus CGSSetWindowTransform(const CGSConnection cid, CGSWindow wid, CGAffineTransform transform);

extern OSStatus CGSGetWindowTransform(const CGSConnection, CGSWindow wid, CGAffineTransform *outTransform);

// questionable, partly guessed IIRC.
extern OSStatus CGSSetSharedWindowState(const CGSConnection cid, CGSWindow wid, CGSValue boolean);

extern OSStatus CGSSetWindowAlpha(const CGSConnection cid, CGSWindow wid, float alpha);
extern OSStatus CGSSetWindowListAlpha(const CGSConnection cid, CGSWindow *wids, int count, float alpha);
extern OSStatus CGSGetWindowAlpha(const CGSConnection cid, const CGSWindow wid, float* alpha);

extern OSStatus CGSSetWindowProperty(const CGSConnection cid, CGSWindow wid, CGSValue key, CGSValue value);

extern CGSValue CGSCreateCString(char *string);
extern CGSValue CGSCreateBoolean(Boolean boolValue);
extern void CGSReleaseGenericObj(CGSValue obj);

extern OSStatus CGSOrderWindow(const CGSConnection cid, CGSWindow wid, CGSWindowOrderingMode place, CGSWindow relativeToWindowID /* can be NULL */);

extern void CGSNewRegionWithRect(const CGRect *aRectangle, CGSRegion *outRegionObj);
extern OSStatus CGSReleaseRegion(CGSRegion region);
extern void CGSGetRegionBounds(const CGSRegion aRegion, CGRect *outRect);

extern void CGSSetWindowOpacity(CGSConnection cid, CGSWindow wid, void* opacity /* kCGSFalse, is that a CGSValue or standard char? */);

extern OSStatus CGSNewConnection(void *something /* can be NULL, parent connection? */, CGSConnection *outID);
extern OSStatus CGSReleaseConnection(const CGSConnection cid);
extern void CGSInitialize();

extern OSStatus CGSGetConnectionIDForPSN(const CGSConnection cid, ProcessSerialNumber *psn, CGSConnection *out);

// random hack constants for CGSSetDebugOptions
#define kCGSDebugOptionNormal 0
#define kCGSDebugOptionNoShadows 16384
#define kCGSHDumpWindowInfoToFile (0x8000<<16)|1

extern OSStatus CGSSetDebugOptions(unsigned long);

extern OSStatus CGSSetUniversalOwner(const CGSConnection cid, int); // only works if you kill the dock, then stop dock from relaunching
extern OSStatus CGSSetOtherUniversalConnection(const CGSConnection cid); // apparently a nop

extern OSStatus CGSGetScreenRectForWindow(const CGSConnection cid, CGSWindow wid, CGRect *outRect);
extern OSStatus CGSMoveWindow(const CGSConnection cid, CGSWindow wid, CGPoint *aPoint);

extern OSStatus CGSGetWindowLevel(const CGSConnection cid, CGSWindow wid, CGWindowLevel *level);
extern OSStatus CGSGetWindowBounds(const CGSConnection cid, CGSWindow wid, CGRect *bounds);

extern CGSWindow CGSDesktopWindow(void);

/*! @function CGSGetWindowCount
@abstract Obtain the number of windows of all types.
@discussion This function provides you with the number of windows of any type, whether on or off screen, with whatever attributes, etc.  In this respect it differs notably from the NSWindowCount function, which only returns the number of on-screen windows.
@param cid The connection ID to the window server.  Cannot be NULL.
@param target The connection ID whose number of windows you want.  If NULL, the total number of windows of all connections is returned.
@param count On return the number of windows.
@result Returns 0 if successful, an error code otherwise (1002 = invalid connection ID). */

extern OSStatus CGSGetWindowCount(const CGSConnection cid, CGSConnection target, int *count);

/*! @function CGSGetWindowList
@abstract Returns a list of window ID's for all open windows.
@discussion This function provides a list of the window ID's for every open window, whether on or off screen, and of any other attributes.  In this respect it differs notably from the NSWindowList function, which only returns on-screen windows.
@param cid The caller's connection ID to the window server.  Cannot be NULL.
@param target The connection ID whose list of windows you want.  If NULL, the windows of all connections are returned.
@param listSize The maximum number of elements that may be returned.
@param list A pointer to an array of ints in which the window ID's will be returned.
@param numberOfWindows On return, contains the number of items returned.
@result Returns 0 if successful, an error code otherwise (1002 = invalid connection ID). */

extern OSStatus CGSGetWindowList(const CGSConnection cid, CGSConnection target, int listSize, int *list, int *numberOfWindows);

/*! @function CGSGetOnScreenWindowCount
@abstract Obtain the number of windows visible on screen.
@discussion This function provides you with the number of windows visible on screen.  It has the same behaviour as the NSWindowCount function.
@param cid The connection ID to the window server.  Cannot be NULL.
@param target The connection ID whose number of windows you want.  If NULL, the total number of windows of all connections is returned.
@param count On return the number of windows.
@result Returns 0 if successful, an error code otherwise (1002 = invalid connection ID). */

extern OSStatus CGSGetOnScreenWindowCount(const CGSConnection cid, CGSConnection targetCID, int* outCount);

/*! @function CGSGetOnScreenWindowList
@abstract Returns a list of window ID's for all open windows visible on screen.
@discussion This function provides a list of the window ID's for every open window visible on screen.  It has the same behaviour as the NSWindowList function.
@param cid The caller's connection ID to the window server.  Cannot be NULL.
@param target The connection ID whose list of windows you want.  If NULL, the windows of all connections are returned.
@param listSize The maximum number of elements that may be returned.
@param list A pointer to an array of ints in which the window ID's will be returned.
@param numberOfWindows On return, contains the number of items returned.
@result Returns 0 if successful, an error code otherwise (1002 = invalid connection ID). */

extern OSStatus CGSGetOnScreenWindowList(const CGSConnection cid, CGSConnection targetCID, int count, int* list, int* outCount);

/*! @function CGSGetWorkspaceWindowCount
@abstract Obtain the number of windows in a particular workspace.
@discussion This function provides you with the number of windows in a particular workspace.
@param cid The connection ID to the window server.  Cannot be NULL.
@param workspaceNumber The workspace number.
@param count On return the number of windows.
@result Returns 0 if successful, an error code otherwise (1002 = invalid connection ID). */

extern OSStatus CGSGetWorkspaceWindowCount(const CGSConnection cid, int workspaceNumber, int *outCount);

/*! @function CGSGetWorkspaceWindowList
@abstract Returns a list of window ID's for all open windows in a particular workspace.
@discussion This function provides a list of the window ID's for every open window in a particular workspace.
@param cid The caller's connection ID to the window server.  Cannot be NULL.
@param workspaceNumber The workspace number.
@param listSize The maximum number of elements that may be returned.
@param list A pointer to an array of ints in which the window ID's will be returned.
@param numberOfWindows On return, contains the number of items returned.
@result Returns 0 if successful, an error code otherwise (1002 = invalid connection ID). */

extern OSStatus CGSGetWorkspaceWindowList(const CGSConnection cid, int workspaceNumber, int count, int* list, int* outCount);

/*! @function CGSReleaseWindowList
@abstract Purportedly releases a previously obtained window list.
@discussion The purpose of this function isn't known... there's nothing obvious to release of the list, since it's allocated by the caller, so presumably either (a) there's additional junk in the window server that is allocated on a call to CGSGetWindowList, or (b) this is in fact for releasing a list returned via some other function.

I suspect now that this might be some internal list, as in (b), where the second parameter is a window ID who's 'list' (whatever that is) you want to free.
@param cid The caller's connection ID to the window server.  Cannot be NULL.
@param unknown1 Most likely a pointer to the window list - causes an EXC_BAD_ACCESS if set to some unlikely pointer (e.g. 1).
@param unknown2 Possibly the number of entries in the list.
@result Always seems to return 1001 (if it doesn't crash) - meaning invalid window ID - in all situations I've tested. */

extern OSStatus CGSReleaseWindowList(const CGSConnection cid, int unknown1, int unknown2);

/*! @function CGSGetWindowSharingState
@abstract Supposedly returns whether a given window is "shared".
@discussion Appears to be broken - always returns NO.
@param cid The caller's connection ID to the window server.  Cannot be NULL.
@param unknown1 Presumably the global window number, but could be anything.
@param result On output the shared state of the window.  Cannot be NULL.
@result Returns 0 if successful, an error code otherwise. */

extern OSStatus CGSGetWindowSharingState(const CGSConnection cid, int unknown1, Boolean *result);

/* Retrieve the workspace number associated with the workspace currently
* being shown.
*
* cid -- Current connection.
* workspace -- Pointer to int value to be set to workspace number.
*/
extern OSStatus CGSGetWorkspace(const CGSConnection cid, int *workspace);

/* Show workspace associated with a workspace number.
*
* cid -- Current connection.
* workspace -- Workspace number.
*/
extern OSStatus CGSSetWorkspace(const CGSConnection cid, int workspace);

extern OSStatus CGSSetWorkspaceWithTransition(const CGSConnection cid, int workspaceNumber, CGSTransitionType transition, CGSTransitionOption subtype, float time);

/*! @function CGSGetWindowWorkspace
@abstract Apparently returns whether or not a given window is on screen.
@discussion While "workspace" is taken as a pointer to an int, the only values returned are apparently 0 & 1, indicating a boolean value.  The result corresponds with the "On screen" attribute as shown in Quartz Debug.

Note that it has not yet been conclusively determined whether windows belonging to another user (using Fast User Switching) are "visible" (in the window list) for other users; evidence presently suggests they are not.  Regardless, if they are, they are shown as "on" or "off" screen (i.e. the result of this method) as normal; there is no apparent distinction between users.
@param cid The caller's connection ID to the window server.  Cannot be NULL.
@param wid The window ID who's "workspace" (see discussion) you want.
@param workspace On output the result of the query, if it is successful.
@result Returns 0 if successful, an error code otherwise. */

extern OSStatus CGSGetWindowWorkspace(const CGSConnection cid, CGSWindow wid, int *workspace);

extern OSStatus CGSMoveWorkspaceWindows(const CGSConnection connection, int toWorkspace, int fromWorkspace);
extern OSStatus CGSMoveWorkspaceWindowList(const CGSConnection connection, CGSWindow *wids, int count, int toWorkspace);

extern OSStatus CGSNewTransition(const CGSConnection cid, const CGSTransitionSpec* spec, int *pTransitionHandle);
extern OSStatus CGSInvokeTransition(const CGSConnection cid, int transitionHandle, float duration);
extern OSStatus CGSReleaseTransition(const CGSConnection cid, int transitionHandle);

// thirtyTwo must = 32 for some reason. tags is pointer to 
//array ot ints (size 2?). First entry holds window tags.
// 0x0800 is sticky bit.
extern OSStatus CGSGetWindowTags(const CGSConnection cid, const CGSWindow wid, CGSWindowTag *tags, int thirtyTwo);
extern OSStatus CGSSetWindowTags(const CGSConnection cid, const CGSWindow wid, CGSWindowTag *tags, int thirtyTwo);
extern OSStatus CGSClearWindowTags(const CGSConnection cid, const CGSWindow wid, CGSWindowTag *tags, int thirtyTwo);
extern OSStatus CGSGetWindowEventMask(const CGSConnection cid, const CGSWindow wid, uint32_t *mask);
extern OSStatus CGSSetWindowEventMask(const CGSConnection cid, const CGSWindow wid, uint32_t mask);

extern OSStatus CGSGetWindowProperty(const CGSConnection cid, CGSWindow wid, CGSValue key, CGSValue *outValue);

extern OSStatus CGSUncoverWindow(const CGSConnection cid, const CGSWindow wid);
extern OSStatus CGSFlushWindow(const CGSConnection cid, const CGSWindow wid, int unknown /* 0 works */ );

extern OSStatus CGSSynchronizeWindow(const CGSConnection cid, const CGSWindow wid);

extern OSStatus CGSGetWindowOwner(const CGSConnection cid, const CGSWindow wid, CGSConnection *ownerCid);
extern OSStatus CGSConnectionGetPID(const CGSConnection cid, pid_t *pid, const CGSConnection ownerCid);

/*! @function CGSGetWindowDepthLimit
@abstract Returns something relating to the window's depth limit (gee, really?).
@discussion Usually returns 4, but occasionally returns 3 (seeming to correspond to 16-bit in Quartz Debug, but the inverse is not true - not *all* 16-bit's return 3).  May be some kind of constant, but more likely the number of bytes... either the number used to store the window, or some sort of maximum it can be set to... most likely the former, since the latter doesn't make so much sense.
@param cid The caller's connection ID to the window server.  Cannot be NULL.
@param wid The window id in question.
@param depth On output the "depth limit" of the given window, whatever that is.
@result Returns 0 if successful, an error code otherwise. */

extern OSStatus CGSGetWindowDepthLimit(const CGSConnection cid, CGSWindow wid, int *depth);

/*! @function CGSGetWindowGeometry
@abstract 
@discussion
@param cid The caller's connection ID to the window server.  Cannot be NULL, nor end in 0xffff (no idea why).  Note: appears to be buggy or otherwise "special"; while it checks for NULL and 0xffff, it still continues anyway.  Strange.
@param wid The window id in question.
@param bounds If not NULL, is set to the bounds of the window. (untested)
@param unknown1 Not known... loaded from the window structure itself... possibly the depth limit of the window, based on available evidence.  Ignored if NULL.
@param unknown2 No idea.  Seems to correlate with the on-screen status of the window, based on Quartz Debug, which also seems to be the "workspace" of the window.  Ignored if NULL.
@result Returns 0 if successful, an error code otherwise. */

extern OSStatus CGSGetWindowGeometry(const CGSConnection cid, CGSWindow wid, NSRect *bounds, void *unknown1, Boolean *unknown2);

//extern OSStatus CGSFlushWindowContentRegion(

/*! @function CGSSystemSupportsHardwareCursor
@abstract Returns whether or not the system supports a hardware cursor.
@discussion Yikes, that's a lot of parameters.  No idea what most of them do - they seem to be passed on to various Mach-port-related stuff.  In any case, the function appears to work fine with just the three known parameters (although I suspect at least one of the other parameters is a screen id/number/whatever, since presumably only some of the available screens might support a hardware cursor).
@param cid The window server connection ID to use.  Should be valid.
@param unknown2 No idea.  'result' always seems to be 0 if this is not 0.
@param cursorSize The size of the cursor in question, e.g. {16.0, 16.0}.
@param unknown4 No idea.  'result' always seems to be 0 if this is not 0.
@param unknown5 No idea.  'result' always seems to be 0 if this is not 0.
@param unknown6 No idea.  'result' always seems to be 0 if this is not 0.
@param result A boolean output indicating whether a hardware cursor is supported or not (in testing returns 1 if true, 0 if false).
@result Returns 0 if successful, an error code otherwise. */

extern OSStatus CGSSystemSupportsHardwareCursor(const CGSConnection cid, int unknown2, CGSize *cursorSize, int unknown4, int unknown5, int unknown6, Boolean *result);

/*! @function CGSGetWindowShape
@abstract Returns a CGSRegion describing the shape of a given window.
@discussion At time of writing the functions for manipulating a CGSRegion are sparse, but you can retrieve a CGRect from them, which for a "normal" window will give you it's standard bounds.
@param cid The window server connection ID to use.  Should be valid.
@param wid The window ID in question.
@param shape A pointer to the CGSRegion in which to store the window's region information.  Remember to release the region later using CGSReleaseRegion.
@result Returns 0 if successful, an error code otherwise. */

extern OSStatus CGSGetWindowShape(const CGSConnection cid, const CGSWindow wid, CGSRegion *shape);

extern OSStatus CGSGetWindowClipShape(const CGSConnection cid, const CGSWindow wid, CGSRegion *shape /* Assumed; not verified */);

extern OSStatus CGSCreateGenieWindowAnimation(const CGSConnection cid, const CGSWindow wid1, const CGSWindow wid2, CGSAnimationRef *ref /* Guessed */);

//extern OSStatus /* ??? */ GenieAnimationProgress(CGSAnimationRef ref); /* Does a bunch of region stuff, calls createGenieRow & createGenieMeshRec, and CGSSetWindowWarp */ /* Cannot find symbol if used... ??? */

extern void /* Maybe returns OSStatus? */ GenieAnimationRelease(CGSAnimationRef ref); /* Self-explanatory */

extern OSStatus CGSSetWindowAnimationProgress(CGSAnimationRef ref, float progress /* Guessed.  Suspect value between 0.0 and 1.0 */);

extern OSStatus CGSReleaseWindowAnimation(CGSAnimationRef ref);

extern OSStatus CreateAnimation(int unknown1 /* Not known; CGSCreateGenieWindowAnimation passed 0x50 (80) */,
const CGSConnection cid,
int unknown3 /* Not known; CGSCreateGenieWindowAnimation passes it's second parameter [unmodified] */,
int unknown4 /* Not known; CGSCreateGenieWindowAnimation passes some constant */,
int unknown5 /* Not known; CGSCreateGenieWindowAnimation passes some constant, as above */,
CGSAnimationRef *ref);

extern OSStatus CGSWindowIsOrderedIn(const CGSConnection cid, const CGSWindow wid, Boolean *result);

extern OSStatus /* Assumed */ CGSLockWindowAccelerator(const CGSConnection cid, const CGSWindow wid, int /* 2nd argument to __CGSAcquireWindowAccelerator */, int /* Optional, I think; 0 should be okay */, int /* Optional, I think; 0 should be okay */, int /* 3rd argument to __CGSAcquireWindowAccelerator */, int /* 4th argument to __CGSAcquireWindowAccelerator */, int* /* No idea what's put into it (possibly a reference to a lock object), but can be NULL */);

extern OSStatus /* Assumed */ CGSLockWindowBackingStore(const CGSConnection cid /* May be 0 */, const CGSWindow wid, int /* Unknown, but may be 0 */, int, int, int, int);

extern OSStatus CGSDragWindowRelativeToMouse(const CGSConnection cid, const CGSWindow wid, NSPoint point);


void CGSSetConnectionProperty(CGSConnection, CGSConnection, CFStringRef, CFTypeRef);

#endif