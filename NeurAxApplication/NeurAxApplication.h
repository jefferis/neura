/* neuraApplication */

#import <Cocoa/Cocoa.h>


@interface NSSlide : NSImage 
- (NSSlide*)noImageLoaded;
- (NSSlide*)createViewWithData:(unsigned char*) data withSizeX:(int) sizeX withSizeY:(int) sizeY;
@end