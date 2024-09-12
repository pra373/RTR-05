//
//  MyView.h
//  Window
//
//  Created by user258815 on 8/4/24.
//

#import<UIKit/UIKit.h>

@interface GLESView : UIView<UIGestureRecognizerDelegate>

-(void)startDisplayLink;
-(void)stopDisplayLink;

@end


