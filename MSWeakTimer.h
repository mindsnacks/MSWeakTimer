//
//  MSWeakTimer.h
//  MindSnacks
//
//  Created by Javier Soto on 1/23/13.
//
//

#import <Foundation/Foundation.h>

/**
 `MSWeakTimer` behaves similar to an `NSTimer` but doesn't retain the target.
 This timer is implemented using GCD, so you can schedule and unschedule it on arbitrary queues (unlike regular NSTimers!)
 */
@interface MSWeakTimer : NSObject

/**
 * Creates a timer with the specified parameters and waits for a call to `-schedule` to start ticking.
 * @note It's safe to retain the returned timer by the object that is also the target.
 * or the provided `dispatchQueue`.
 * @param timeInterval how frequently `selector` will be invoked on `target`. If the timer doens't repeat, it will only be invoked once, approximately `timeInterval` seconds from the time you call this method.
 * @param repeats if `YES`, `selector` will be invoked on `target` until the `MSWeakTimer` object is deallocated or until you call `invalidate`. If `NO`, it will only be invoked once.
 * @param dispatchQueue the queue where the delegate method will be dispatched. It can be either a serial or concurrent queue.
 * @see `invalidate`.
 */
- (id)initWithTimeInterval:(NSTimeInterval)timeInterval
                    target:(id)target
                  selector:(SEL)selector
                  userInfo:(id)userInfo
                   repeats:(BOOL)repeats
             dispatchQueue:(dispatch_queue_t)dispatchQueue;

/**
 * Creates an `MSWeakTimer` object and schedules it to start ticking inmediately.
 */
+ (instancetype)scheduledTimerWithTimeInterval:(NSTimeInterval)timeInterval
                                        target:(id)target
                                      selector:(SEL)selector
                                      userInfo:(id)userInfo
                                       repeats:(BOOL)repeats
                                 dispatchQueue:(dispatch_queue_t)dispatchQueue;

/**
 * Starts the timer if it hadn't been schedule yet.
 * @warning calling this method on an already scheduled timer results in undefined behavior.
 */
- (void)schedule;

/**
 * Causes the timer to be fired synchronously manually on the queue from which you call this method.
 * You can use this method to fire a repeating timer without interrupting its regular firing schedule.
 * If the timer is non-repeating, it is automatically invalidated after firing, even if its scheduled fire date has not arrived.
 */
- (void)fire;

/**
 * You can call this method on repeatable timers in order to stop it from running and trying
 * to call the delegate method.
 * @note `MSWeakTimer` won't invoke the `selector` on `target` again after calling this method.
 * You can call this method from any queue, it doesn't have to be the queue from where you scheduled it.
 * Since it doesn't retain the delegate, unlike a regular `NSTimer`, your `dealloc` method will actually be called
 * and it's easier to place the `invalidate` call there, instead of figuring out a safe place to do it.
 */
- (void)invalidate;

- (id)userInfo;

@end
