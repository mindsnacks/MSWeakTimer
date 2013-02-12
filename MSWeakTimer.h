//
//  MSWeakTimer.h
//  MindSnacks
//
//  Created by Javier Soto on 1/23/13.
//
//

#import <Foundation/Foundation.h>

@protocol MSWeakTimerDelegate;

/**
 * @class `MSWeakTimer` behaves similar to an `NSTimer` but doesn't retain the target/delegate.
 * @discussion this timer is implemented using GCD, so you can schedule and unschedule it on arbitrary queues
 * (unlike regular NSTimers!)
 * It's safe to retain this timer by the object that is also the delegate.
 * You can call -invalidate from any queue, doesn't have to be the queue from where you scheduled it
 * or the provided dispatchQueue.
 * @param dispatchQueue the queue where the delegate method will be dispatched. Note: the queue is retained.
 * @see `-invalidate`.
 */
@interface MSWeakTimer : NSObject

+ (MSWeakTimer *)scheduledTimerWithTimeInterval:(NSTimeInterval)timeInterval
                                       delegate:(id<MSWeakTimerDelegate>)delegate
                                       userInfo:(id)userInfo
                                        repeats:(BOOL)repeats
                                  dispatchQueue:(dispatch_queue_t)dispatchQueue;

/**
 * @discussion causes the timer to be fired synchronously with this call.
 * @note this fires the timer on the caller queue, not on the provided dispatch_queue.
 */
- (void)fire;

/**
 * @discussion you should call this method on repeatable timers in order to stop it from running and trying
 * to perform call the delegate method. You can simply do it in the -dealloc method of the delegate.
 * (which usually is the object that owns the timer).
 * Since it doesn't retain the delegate, unlike a regular NSTimer, your -dealloc method will actually be called
 * and it's easier to place the -invalidate call there, instead of figuring out a safe place to do it.
 */
- (void)invalidate;

- (id)userInfo;

@end

@protocol MSWeakTimerDelegate <NSObject>

- (void)weakTimerDidFire:(MSWeakTimer *)timer;

@end
