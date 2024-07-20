//
//  Window.m
//  
//
//  Created by user258815 on 6/16/24.
//

#import <Foundation/Foundation.h>

#import <Cocoa/Cocoa.h>

@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>

@end

@interface View : NSView							
@end

//main function

int main(int argc, char* argv[])
{
    //code
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    NSApp = [NSApplication sharedApplication];
    
    [NSApp setDelegate : [[AppDelegate alloc] init]];
    
    [NSApp run];
    
    [pool release];
    
    return 0;
}

//implementation of custom AppDelegate interface

@implementation AppDelegate

    {
        
        
        NSWindow *window;
        
        View *view;
        
    }

    
    -(void)applicationDidFinishLaunching:(NSNotification *)notification
    {
        //code
        
        //declare rectangle for frame of our window
        
        NSRect win_rect = NSMakeRect(0.0,0.0,800.0,600.0);
        
        //create the window
        
        window = [[NSWindow alloc]initWithContentRect:win_rect
                                             styleMask: NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable
                                              backing:NSBackingStoreBuffered
                                                defer:NO];
        
        
        //give title to the window
        
        [window setTitle:@"Prathamesh Paropkari : Window Code"];
        
        //centre the window
        
        [window center];
        
        //create the custom view
        
        view = [[View alloc]initWithFrame:win_rect];
        
        //set this newly created custom as view of our custom Window
        
        [window setContentView: view];
        
        [window setDelegate:self];
        
        //now actually show the window, give it keyboard focus and make it top in the z order
        
        [window makeKeyAndOrderFront:self];
    }
    
    -(void)applicationWillTerminate:(NSNotification *)notification
    {
        
    }
    
    -(void)windowWillClose:(NSNotification *)notification
    {
        //code
        
        [NSApp terminate:self];
    }
    
    -(void)dealloc
    {
        [super dealloc];
        
        [view release];
        
        [window release];
        
        
        
    }
    
    

    

@end

//implementation of custome view interface

@implementation View

   /* {
        NSString *string;
    }*/
    
    -(id)initWithFrame:(NSRect)frame
    {
        //code
        
        self = [super initWithFrame:frame];
        
        if(self)
        {
            //string = @"Hello World !!!";
        }
        
        return(self);
    }
    
    -(void)drawRect:(NSRect)dirtyRect
    {
        //declare the background color black
        
        NSColor *bgColor = [NSColor blackColor];
        
        //Set this color
        
        [bgColor set];
        
        //fill the window rectangle / dirty rectangle with above black color
        
        NSRectFill(dirtyRect);
        
        //create the font for our string
        
       // NSFont *stringFont = [NSFont fontWithName:@"Helvetica" size:32];
        
        //create Sting Color
        
       // NSColor *fgColor = [NSColor greenColor];
        
        ///create dicsnory for our string attributes for above two
        
       /* NSDictionary *stringDictionary = [NSDictionary dictionaryWithObjectsAndKeys:stringFont,NSFontAttributeName,fgColor,NSForegroundColorAttributeName,nil];*/
        
        //create the size of the string according to the size of the font and other attributes
        
       // NSSize stringSize = [string sizeWithAttributes:stringDictionary];
        
        //create point for srawing of string
        
       // NSPoint stringPoint = NSMakePoint((dirtyRect.size.width/2 - stringSize.width/2),(dirtyRect.size.height/2 - stringSize.height/2));
        
        //finally draw the string Hellow World !!
        
        //[string drawAtPoint:stringPoint withAttributes:stringDictionary];
        
    }
    
    -(BOOL)acceptsFirstResponder
    {
        //code
        
        [[self window]makeFirstResponder:self];
        
        return(YES);
    }
    
    -(void)keyDown:(NSEvent *)event
    {
        //code
        
        int key = (int)[[event characters]characterAtIndex:0];
        
        switch(key)
        {
            case 27:
                [self release];
                [NSApp terminate:self];
                
                break;
                
            /*case 'f':
            case'F':
                [[self window]toggleFullScreen:self];
                break;*/
                
            default:
                break;
        }
    }
    
    -(void)mouseDown:(NSEvent *)event
    {
       /* string = @"Left Mouse Button Is Clicked";
        [self setNeedsDisplay:YES];*/
        
        
    }
    
    -(void)rightMouseDown:(NSEvent *)event
    {
        /*string = @"Right Mouse Button Is Clicked";
        [self setNeedsDisplay:YES];*/
    }
    
    -(void)dealloc
    {
        [super dealloc];
        
    }


@end




    
