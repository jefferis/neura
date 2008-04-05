#import "NeurAxApplication.h"
#import "datacube.h"

typedef struct _monochromePixel
	{ 
	unsigned char grayValue; 
	unsigned char alpha; 
	} monochromePixel;

typedef struct _colorPixel
	{ 
	unsigned char red, blue, green; 
//	unsigned char alpha; 
	} colorPixel;

@implementation NSSlide

- (NSSlide*)createViewWithData:(unsigned char*) data withSizeX:(int) sizeX withSizeY:(int) sizeY
{

    NSSize mySize;
    int widthInPixels=sizeX;
    int heightInPixels = sizeY;
    mySize.width = widthInPixels;
    mySize.height = heightInPixels;
    
  				
    NSImage 
	*monochromeImage = [[[self class] alloc] initWithSize:mySize];
    
    int
	row, column;
    
    // Need a place to put the monochrome pixels.
    NSBitmapImageRep *blackAndWhiteRep = 
	[[NSBitmapImageRep alloc] 
      initWithBitmapDataPlanes: nil  // Nil pointer tells the kit to allocate the pixel buffer for us.
		    pixelsWide: widthInPixels 
		    pixelsHigh: heightInPixels
		 bitsPerSample: 8
	       samplesPerPixel: 2  
		      hasAlpha: YES
		      isPlanar: NO 
		colorSpaceName: NSCalibratedWhiteColorSpace // 0 = black, 1 = white in this color space.
		   bytesPerRow: 0     // Passing zero means "you figure it out."
		  bitsPerPixel: 16];  // This must agree with bitsPerSample and samplesPerPixel.
    
    monochromePixel
	*pixels = (monochromePixel *)[blackAndWhiteRep bitmapData];  // -bitmapData returns a void*, not an NSData object ;-)
    
    
    for (row = 0; row < heightInPixels; row++)
	for (column = 0; column < widthInPixels; column++)
	{
	    monochromePixel 
	    *thisPixel = &(pixels[((widthInPixels * row) + column)]);
	    
	    thisPixel->alpha=255;
	    thisPixel->grayValue=(unsigned char)(data[row*sizeX+column]);
	}
	    
	    [monochromeImage addRepresentation:blackAndWhiteRep];
    [blackAndWhiteRep release];

    return [monochromeImage autorelease];
}





- (NSSlide*)noImageLoaded
{
	NSSize mySize;
	int widthInPixels=100, heightInPixels = 100;
	mySize.width = widthInPixels;
	mySize.height = heightInPixels;
		
	NSImage 
		*coloredImage = [[[self class] alloc] initWithSize:mySize];
		
	int
		row, column;
	
	// Need a place to put the monochrome pixels.
	NSBitmapImageRep *coloredRep = 
    [[NSBitmapImageRep alloc] 
      initWithBitmapDataPlanes: nil  // Nil pointer tells the kit to allocate the pixel buffer for us.
      pixelsWide: widthInPixels 
      pixelsHigh: heightInPixels
      bitsPerSample: 8
      samplesPerPixel: 3 
      hasAlpha: NO
      isPlanar: NO 
      colorSpaceName: NSCalibratedRGBColorSpace // 0 = black, 1 = white in this color space.
      bytesPerRow: 0     // Passing zero means "you figure it out."
      bitsPerPixel: 24];  // This must agree with bitsPerSample and samplesPerPixel.
  
  colorPixel
    *pixels = (colorPixel *)[coloredRep bitmapData];  // -bitmapData returns a void*, not an NSData object ;-)
	
	
	for (row = 0; row < heightInPixels; row++)
		for (column = 0; column < widthInPixels; column++)
			{
			colorPixel 
				*thisPixel = &(pixels[((widthInPixels * row) + column)]);
			
			if(abs(row-column)<1 || abs(row-100+column)<1)
				{
					thisPixel->red=(unsigned char)0;
					thisPixel->green=(unsigned char)200;
					thisPixel->blue=(unsigned char)100;
				}	
					
 			}
	
 	[coloredImage addRepresentation:coloredRep];
 	[coloredRep release];
 
	return [coloredImage autorelease];
	
}


@end
