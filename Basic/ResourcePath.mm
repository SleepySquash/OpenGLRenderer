//
//  ResourcePath.cpp
//  Basic1
//
//  Created by Никита Исаенко on 02.01.2021.
//

#include "ResourcePath.hpp"

#import <Foundation/Foundation.h>

std::string resourcePath(void)
{
    std::string rpath;
    NSBundle* bundle = [NSBundle mainBundle];
    
    if (bundle == nil) {
#ifdef DEBUGr
        NSLog(@"bundle is nil... thus no resources path can be found.");
#endif
    } else {
        NSString* path = [bundle resourcePath];
        rpath = [path UTF8String] + std::string("/");
    }
    
    return rpath;
}
