//
//  AppDelegate.m
//  Window
//
//  Created by user258815 on 8/4/24.
//

#import "AppDelegate.h"

#import "ViewController.h"

#import"GLESView.h"

@implementation AppDelegate
{
@private
    
    UIWindow *window;
    
    ViewController *viewController;
    
    GLESView *glesView;
    
    
}

-(bool)application:(UIApplication *)application didFinishLaunchingWithOptions: (NSDictionary *)launchOptions
{
    //code
    
    //get the screen rectangle (Iphone)
    
    CGRect screenRect = [[UIScreen mainScreen]bounds];
    
    //create window
    
    window = [[UIWindow alloc]initWithFrame:screenRect];
    
    //create viewController
    
    viewController = [[ViewController alloc]init];
    
    //set this viewController as window's viewController
    
    [window setRootViewController:viewController];
    
    //create custome view
    
    glesView = [GLESView alloc];
    
    [glesView initWithFrame:screenRect];
    //set this view as view of view controller
    
    [viewController setView:glesView];
    
    //decrement the reference count of view incremented by set view
    
    [GLESView release];
    
    // set focus on this window and make it visible
    
    [window makeKeyAndVisible];
    
    
    //start display link
    
    [glesView startDisplayLink];
    
    
    return(YES);
}

-(void)applicationWillResignActive:(UIApplication *)application
{
    //code
    
    //stop display link
    
    [glesView stopDisplayLink];
    
    
}

-(void)applicationDidEnterBackground:(UIApplication *)application
{
    //code
    
    
}

-(void)applicationWillEnterForeground:(UIApplication *)application
{
    //code
    
   
}

-(void)applicationDidBecomeActive:(UIApplication *)application
{
    //code
    
    //start display link
    
    [glesView startDisplayLink];
}

-(void)applicationWillTerminate:(UIApplication *)application
{
    //code
    
    //stop display link
    
    [glesView stopDisplayLink];
    
    
}

-(void)dealloc
{
    //code
    
    [super dealloc];
    
    [GLESView release];
    
    [ViewController release];
    
    [window release];
}
@end
