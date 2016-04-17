//
//  OCInteract.m
//  USNIT
//
//  Created by dyno on 4/13/16.
//  Copyright © 2016 dyno. All rights reserved.
//

#import "Interact.h"
#import <Foundation/Foundation.h>


void InteractLog(const char* log){
    NSString *oclog = [NSString stringWithCString:log encoding:NSUTF8StringEncoding];
    NSLog(@"%@", oclog);
}