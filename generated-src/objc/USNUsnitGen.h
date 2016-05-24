// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from usnit.djinni

#import "USNLangType.h"
#import "USNUsnitType.h"
#import <Foundation/Foundation.h>
@class USNUsnitGen;
@protocol USNUsnitEventGen;


@interface USNUsnitGen : NSObject

+ (nullable USNUsnitGen *)instance;

- (BOOL)initialize:(nonnull NSString *)conf
              lang:(USNLangType)lang
          callback:(nullable id<USNUsnitEventGen>)callback;

- (BOOL)save;

- (void)setType:(USNUsnitType)type;

- (void)setInput:(float)value;

- (nonnull NSString *)getResult:(USNUsnitType)type;

- (nonnull NSString *)getUnitName:(USNUsnitType)type;

- (USNUsnitType)getLongType;

- (USNUsnitType)getMassType;

- (USNUsnitType)getSquareType;

- (USNUsnitType)getVolumeType;

@end
