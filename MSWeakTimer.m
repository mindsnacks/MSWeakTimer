//
//  MSWeakTimer.m
//  MindSnacks
//
//  Created by Javier Soto on 1/23/13.
//
//

#import "MSWeakTimer.h"
#import <objc/message.h>

#if !__has_feature(objc_arc)
    #error MSWeakTimer is ARC only. Either turn on ARC for the project or use -fobjc-arc flag
#endif

#if __IPHONE_OS_VERSION_MIN_REQUIRED < 60000
    #define MSTreatQueuesAsObjects (0)
#else
    #define MSTreatQueuesAsObjects (1)
#endif

#if MSTreatQueuesAsObjects
    #define ms_gcd_property_qualifier strong
    #define ms_retain_gcd_object(object)
    #define ms_release_gcd_object(object)
#else
    #define ms_gcd_property_qualifier assign
    #define ms_retain_gcd_object(object) dispatch_retain(object)
    #define ms_release_gcd_object(object) dispatch_release(object)
#endif

@interface MSWeakTimer ()

@property (nonatomic, assign) NSTimeInterval timeInterval;
@property (nonatomic, assign) id<MSWeakTimerDelegate> delegate;
@property (nonatomic, weak) id target;
@property (nonatomic, assign) SEL selector;
@property (nonatomic, retain) id userInfo;
@property (nonatomic, assign) BOOL repeats;

@property (nonatomic, ms_gcd_property_qualifier) dispatch_queue_t dispatchQueue;

@property (nonatomic, ms_gcd_property_qualifier) dispatch_source_t timer;

- (void)timerFired;

@end

@implementation MSWeakTimer

+ (MSWeakTimer *)scheduledTimerWithTimeInterval:(NSTimeInterval)timeInterval
                                       delegate:(id<MSWeakTimerDelegate>)delegate
                                       userInfo:(id)userInfo
                                        repeats:(BOOL)repeats
                                  dispatchQueue:(dispatch_queue_t)dispatchQueue
{
    MSWeakTimer *weakTimer = [[self alloc] init];

    weakTimer.timeInterval = timeInterval;
    weakTimer.delegate = delegate;
    weakTimer.userInfo = userInfo;
    weakTimer.repeats = repeats;

    ms_retain_gcd_object(dispatchQueue);

    weakTimer.dispatchQueue = dispatchQueue;

    [weakTimer schedule];

    return weakTimer;
}

+ (MSWeakTimer *)scheduledTimerWithTimeInterval:(NSTimeInterval)timeInterval
                                         target:(id)target
                                       selector:(SEL)selector
                                       userInfo:(id)userInfo
                                        repeats:(BOOL)repeats
                                  dispatchQueue:(dispatch_queue_t)dispatchQueue
{
    MSWeakTimer *weakTimer = [[self alloc] init];

    weakTimer.timeInterval = timeInterval;
    weakTimer.target = target;
    weakTimer.selector = selector;
    weakTimer.userInfo = userInfo;
    weakTimer.repeats = repeats;

    ms_retain_gcd_object(dispatchQueue);

    weakTimer.dispatchQueue = dispatchQueue;

    [weakTimer schedule];

    return weakTimer;

}

- (void)dealloc
{
    [self invalidate];

    ms_release_gcd_object(_dispatchQueue);
}

- (NSString *)description
{
    if (self.delegate)
    {
        return [NSString stringWithFormat:@"<%@ %p> time_interval=%f delegate=%@ userInfo=%@ repeats=%d timer=%@", NSStringFromClass([self class]), self, self.timeInterval, self.delegate, self.userInfo, self.repeats, self.timer];
    }
    else
    {
        return [NSString stringWithFormat:@"<%@ %p> time_interval=%f target=%@ selector=%@ userInfo=%@ repeats=%d timer=%@", NSStringFromClass([self class]), self, self.timeInterval, self.target, NSStringFromSelector(self.selector), self.userInfo, self.repeats, self.timer];
    }
}

#pragma mark -

- (void)schedule
{
    self.timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER,
                                        0,
                                        0,
                                        self.dispatchQueue);

    int64_t intervalInNanoseconds = (int64_t)(self.timeInterval * NSEC_PER_SEC);
    dispatch_source_set_timer(self.timer,
                              dispatch_time(DISPATCH_TIME_NOW, intervalInNanoseconds),
                              (uint64_t)intervalInNanoseconds,
                              0);

    __weak typeof(self) weakSelf = self;

    dispatch_source_set_event_handler(self.timer, ^{
        __strong typeof(self) strongSelf = weakSelf;
        
        [strongSelf timerFired];

        if (!strongSelf.repeats)
        {
            [strongSelf invalidate];
        }
    });

    dispatch_resume(self.timer);
}

- (void)fire
{
    [self timerFired];
}

- (void)invalidate
{
    @synchronized(self)
    {
        if (self.timer)
        {
            dispatch_source_cancel(self.timer);
            ms_release_gcd_object(self.timer);
            self.timer = nil;
        }
    }
}

- (void)timerFired
{
    if (self.delegate)
    {
        [self.delegate weakTimerDidFire:self];
    }
    else
    {
        objc_msgSend(self.target, self.selector);
    }
}

@end
