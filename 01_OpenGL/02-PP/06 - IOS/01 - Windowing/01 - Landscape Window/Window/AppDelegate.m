//
//  AppDelegate.m
//  Window
//
//  Created by user258815 on 8/4/24.
//

#import "AppDelegate.h"

#import "ViewController.h"

#import"MyView.h"

@implementation AppDelegate
{
@private
    
    UIWindow *window;
    
    ViewController *viewController;
    
    MyView *myView;
    
    
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
    
    myView = [[MyView alloc]initWithFrame:screenRect];
    
    //set this view as view of view controller
    
    [viewController setView:myView];
    
    //decrement the reference count of view incremented by set view
    
    [myView release];
    
    // set focus on this window and make it visible
    
    [window makeKeyAndVisible];
    
    
    
    
    return(YES);
}

-(void)applicationWillResignActive:(UIApplication *)application
{
    //code
    
    
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
    
}

-(void)applicationWillTerminate:(UIApplication *)application
{
    //code
    
    
}

-(void)dealloc
{
    //code
    
    [super dealloc];
    
    [MyView release];
    
    [ViewController release];
    
    [window release];
}
@end
