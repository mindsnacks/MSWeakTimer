//
//  MSWeakTimer.h
//  MindSnacks
//
//  Created by Javier Soto on 1/23/13.
//
//

#import <Foundation/Foundation.h>

@interface MSWeakTimer : NSObject

/**
 * @class `MSWeakTimer` behaves similar to an `NSTimer` but doesn't retain the target.
 * @discussion this timer is implemented using GCD, so you can schedule and unschedule it on arbitrary queues
 * (unlike regular NSTimers!)
 * It's safe to retain this timer by the object that is also the target.
 * You can call -invalidate from any queue, doesn't have to be the queue from where you scheduled it
 * or the provided dispatchQueue.
 * @param dispatchQueue the queue where the delegate method will be dispatched. It can be either a serial or concurrent queue. Note: the queue is retained.
 * @see `-invalidate`.
 */
+ (MSWeakTimer *)scheduledTimerWithTimeInterval:(NSTimeInterval)timeInterval
                                         target:(id)target
                                       selector:(SEL)selector
                                       userInfo:(id)userInfo
                                        repeats:(BOOL)repeats
                                  dispatchQueue:(dispatch_queue_t)dispatchQueue;

/**
 * @discussion causes the timer to be fired asynchronously on the provided dispatchQueue.
 * You can use this method to fire a repeating timer without interrupting its regular firing schedule.
 * If the timer is non-repeating, it is automatically invalidated after firing, even if its scheduled fire date has not arrived.
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

/**
 * @discussion returns a Boolean value that indicates whether the receiver is currently valid.
 * @return YES if the receiver is still capable of firing or NO if the timer has been invalidated and is no longer capable of firing.
 */
- (BOOL)isValid;

/**
 * @discussion returns the date at which the receiver will fire.
 * @return the date at which the receiver will fire. If the timer is no longer valid, this method returns the absolute reference date (the first instant of 1 January 2001, GMT).
 */
- (NSDate *)fireDate;

/**
 @discussion you typically use this method to adjust the firing time of a repeating timer. For example, you could use it in situations where you want to repeat an action multiple times in the future, but at irregular time intervals. Adjusting the firing time of a single timer would likely incur less expense than creating multiple timer objects and then destroying them.
 @param date the new date at which to fire the receiver. If the new date is in the past, this method sets the fire time to the current time.
 */
- (void)setFireDate:(NSDate *)date;

/**
 * @discussion returns the receiver’s time interval.
 * @return the receiver’s time interval. If the receiver is a non-repeating timer, returns 0 (even if a time interval was set).
 */
- (NSTimeInterval)timeInterval;

- (id)userInfo;

@end
