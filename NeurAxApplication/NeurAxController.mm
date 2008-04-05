#import "NeurAxController.h"



@implementation NeurAxController

- (void) awakeFromNib
  {
	NSSlide *test=[NSSlide alloc];
	test=[test init];
	test=[test noImageLoaded];	
		
	[frame setImage:test];
	[frame setImageScaling: NSScaleToFit];
	
	//////////////////      Start the application controller    	/////////////////////
	neuraAxApplicationController = new NeuraAxApplicationController();
	}


-(IBAction) loadHocFile: (id) sender {
	NSArray		*fileTypes = [NSArray arrayWithObjects:@"hoc",@"HOC",nil];
    
	[[NSOpenPanel openPanel]  // Get the shared open panel
    beginSheetForDirectory:NSHomeDirectory()	// Point it at the user's home
    file:nil
	types: fileTypes
    modalForWindow:window  // This makes it show up as a sheet, attached to window
    modalDelegate:self		// Tell me when you're done.
    didEndSelector:@selector(openPanelForHocDidEnd:returnCode:contextInfo:)  // Call this method when you're done..
    contextInfo:NULL];  
  }


- (void) openPanelForHocDidEnd:(NSOpenPanel *)sheet returnCode:(int)returnCode contextInfo:(void  *)contextInfo
  {
	NSString* name=[sheet filename];
	char *temp;								
	temp =new char[100];
//	[name getCString: temp];
//	[table addTableColumn: name]
	}


-(IBAction) loadImage: (id) sender {
	NSArray		*fileTypes = [NSArray arrayWithObjects:@"nih",@"NIH",@"tif",@"TIF",@"tiff",@"TIFF",nil];
    
	[[NSOpenPanel openPanel]  // Get the shared open panel
    beginSheetForDirectory:NSHomeDirectory()	// Point it at the user's home
    file:nil
	types: fileTypes
    modalForWindow:window  // This makes it show up as a sheet, attached to window
    modalDelegate:self		// Tell me when you're done.
    didEndSelector:@selector(openPanelDidEnd:returnCode:contextInfo:)  // Call this method when you're done..
    contextInfo:NULL];  
  }


- (void) openPanelDidEnd:(NSOpenPanel *)sheet returnCode:(int)returnCode contextInfo:(void  *)contextInfo
  {
	NSString* name=[sheet filename];
	char *temp;								
	temp =new char[100];
	[name getCString: temp];
	
	/////////////////////   Loading from nih   //////////////////////////////////
	NSString *ending = [NSString stringWithCString: ".nih"];
	NSString *ENDING = [NSString stringWithCString: ".NIH"];
	if([name hasSuffix: ending] || [name hasSuffix: ENDING])	
		neuraAxApplicationController->LoadAxonCubeFromNih(temp);
    
	/////////////////////   Loading from tif   //////////////////////////////////
	NSString *ending1 = [NSString stringWithCString: ".tif"];
	NSString *ENDING1 = [NSString stringWithCString: ".TIF"];
	NSString *ending2 = [NSString stringWithCString: ".tiff"];
	NSString *ENDING2 = [NSString stringWithCString: ".TIFF"];
	if([name hasSuffix: ending1] || [name hasSuffix: ENDING1] || [name hasSuffix: ending2] || [name hasSuffix: ENDING2])	
		neuraAxApplicationController->LoadAxonCubeFromTif(temp);
	
	
	ViewCube *presentView=neuraAxApplicationController->GetViewCube();
   
	int sizeX=presentView->GetSizeX();
	int sizeY=presentView->GetSizeY();
	int sizeZ=presentView->GetSizeZ();
    
	//////////////////      Set the value of the slider    	/////////////////////
	[slider setMinValue: 0.];
	[slider setMaxValue: sizeZ];
	int slice=[slider intValue];
	////////////////////////////////////////////////////////////////////////////
	[slicenumber setIntValue:slice];
	
	unsigned char *dataOfSlice=presentView->GetSlice(slice);
   
    
	NSSlide *slideView=[NSSlide alloc];
	slideView=[slideView init];
    
	slideView=[slideView createViewWithData: dataOfSlice withSizeX: sizeX withSizeY: sizeY];
	[frame setImage:slideView];
	[frame setImageScaling: NSScaleToFit];
	
}



- (IBAction)sliderAnimation:(id)sender
{
	NSSlide *slideView=[NSSlide alloc];
	slideView=[slideView init];
	
	//////////////////      Get data access   	/////////////////////
	ViewCube *presentView=neuraAxApplicationController->GetViewCube();
	int sizeX=presentView->GetSizeX();
	int sizeY=presentView->GetSizeY();
	//int sizeZ=presentView->GetSizeZ();
	
	//////////////////      Get the value of the slider    	/////////////////////
	int slice=[slider intValue];
	[slicenumber setIntValue:slice];
	////////////////////////////////////////////////////////////////////////////
	unsigned char *dataOfSlice=presentView->GetSlice(slice);
	
	slideView=[slideView createViewWithData: dataOfSlice withSizeX: sizeX withSizeY: sizeY];
		
	[frame setImage:slideView];
	[frame setImageScaling: NSScaleToFit];
}
	




@end
