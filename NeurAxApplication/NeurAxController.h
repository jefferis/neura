/* neuraController */
#include "NeuraAxApplicationController.h"

#import <Cocoa/Cocoa.h>
#import "NeurAxApplication.h"


@interface NeurAxController : NSObject
{
	IBOutlet NSImageView *frame;
	IBOutlet NSSlider*	slider;
	IBOutlet NSWindow*	window;
	IBOutlet NSTextField*	slicenumber;
	IBOutlet NSTableView* table;

	NeuraAxApplicationController *neuraAxApplicationController;
}

- (IBAction)loadImage:(id)sender;
- (IBAction)sliderAnimation:(id)sender;
- (IBAction)loadHocFile: (id) sender;
- (IBAction)deleteHocFile: (id) sender;
- (IBAction)projectHocFile: (id) sender;

@end
