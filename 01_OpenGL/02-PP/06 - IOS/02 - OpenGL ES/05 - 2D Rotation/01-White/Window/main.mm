//
//  main.m
//  Window
//
//  Created by user258815 on 8/4/24.
//

#import <UIKit/UIKit.h>

#import "AppDelegate.h"

int main(int argc, char* argv[])
{
    //code
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    //get appDeligate class name in the form of string
    
    NSString* delegateClassName = NSStringFromClass([AppDelegate class]);
    
    int result = UIApplicationMain(argc,argv,nil,delegateClassName);
    
    [pool release];
    
    return(result);
    
}
