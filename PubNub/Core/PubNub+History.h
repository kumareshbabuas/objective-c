#import <Foundation/Foundation.h>
#import "PubNub+Core.h"


#pragma mark Class forward

@class PNHistoryResult, PNErrorStatus;


NS_ASSUME_NONNULL_BEGIN

#pragma mark - Types

/**
 @brief  Channel history fetch completion block.
 
 @param result Reference on result object which describe service response on history request.
 @param status Reference on status instance which hold information about processing results.
 
 @since 4.0
 */
typedef void(^PNHistoryCompletionBlock)(PNHistoryResult * _Nullable result, PNErrorStatus * _Nullable status);


#pragma mark - API group interface

/**
 @brief      \b PubNub client core class extension to provide access to 'history' API group.
 @discussion Set of API which allow to fetch events which has been moved from remote data object live feed to 
             persistent storage.
 
 @author Sergey Mamontov
 @since 4.0
 @copyright © 2009-2016 PubNub, Inc.
 */
@interface PubNub (History)


///------------------------------------------------
/// @name Full history
///------------------------------------------------

/**
 @brief      Allow to fetch up to \b 100 events from specified \c channel's events storage.
 @discussion \b Example:
 
 @code
// Client configuration.
PNConfiguration *configuration = [PNConfiguration configurationWithPublishKey:@"demo" 
                                                                 subscribeKey:@"demo"];
self.client = [PubNub clientWithConfiguration:configuration];
[self.client historyForChannel:@"storage" withCompletion:^(PNHistoryResult * _Nullable result,
                                                           PNErrorStatus * _Nullable status) {

    // Check whether request successfully completed or not.
    if (!status.isError) {

       // Handle downloaded history using: 
       //   result.data.start - oldest message time stamp in response
       //   result.data.end - newest message time stamp in response
       //   result.data.messages - list of messages
    }
    // Request processing failed.
    else {
    
       // Handle message history download error. Check 'category' property to find out possible 
       // issue because of which request did fail.
       //
       // Request can be resent using: [status retry];
    }
}];
 @endcode
 
 @param channel Name of the channel for which events should be pulled out from storage.
 @param block   History pull processing completion block which pass two arguments: \c result - in case of 
                successful request processing \c data field will contain results of history request operation;
                \c status - in case if error occurred during request processing.
 
 @since 4.0
 */
- (void)historyForChannel:(NSString *)channel withCompletion:(PNHistoryCompletionBlock)block;

/**
 @brief      Allow to fetch requested amount of events from specified \c channel's events storage.
 @discussion Allow to fetch up to \c limit number of events from \c channel's events storage. If \b 0 set for
             \c limit - all events available in \c channel's events storage will be pulled out.
 @warning    Memory consumption will grow proportional to number of fetched messages. Consider to use 
             reasanoble \c limit value. 
 @discussion \b Example:
 
 @code
// Client configuration.
PNConfiguration *configuration = [PNConfiguration configurationWithPublishKey:@"demo" 
                                                                 subscribeKey:@"demo"];
self.client = [PubNub clientWithConfiguration:configuration];
[self.client historyForChannel:@"storage" limit:250 withCompletion:^(PNHistoryResult * _Nullable result,
                                                                     PNErrorStatus * _Nullable status) {

    // Check whether request successfully completed or not.
    if (!status.isError) {

       // Handle downloaded history using: 
       //   result.data.start - oldest message time stamp in response
       //   result.data.end - newest message time stamp in response
       //   result.data.messages - list of messages
    }
    // Request processing failed.
    else {
    
       // Handle message history download error. Check 'category' property to find out possible 
       // issue because of which request did fail.
       //
       // Request can be resent using: [status retry];
    }
}];
 @endcode
 
 @param channel Name of the channel for which events should be pulled out from storage.
 @param limit   Maximum number of events which should be returned in response. If value more than \b 100
                (service limit per request) will be passed, series of requests will be done to fetch as much
                message as available to fulfill \c limit request. \b 0 allow to fetch all available events.
 @param block   History pull processing completion block which pass two arguments: \c result - in case of 
                successful request processing \c data field will contain results of history request operation;
                \c status - in case if error occurred during request processing.
 
 @since 4.4
 */
- (void)historyForChannel:(NSString *)channel limit:(NSUInteger)limit 
           withCompletion:(PNHistoryCompletionBlock)block;


///------------------------------------------------
/// @name History in specified frame
///------------------------------------------------

/**
 @brief      Allow to fetch events from specified \c channel's history within specified time frame.
 @note       This method allow to fetch up to \b 100 events at once. If in specified time frame there is more
             then 100 events paging may be required. For paging use last event time token from response and
             some distant future date for next portion of events.
 @discussion \b Example:
 
 @code
// Client configuration.
PNConfiguration *configuration = [PNConfiguration configurationWithPublishKey:@"demo" 
                                                                 subscribeKey:@"demo"];
self.client = [PubNub clientWithConfiguration:configuration];
NSNumber *startDate = @([[NSDate dateWithTimeIntervalSinceNow:-(60*60)] timeIntervalSince1970]);
NSNumber *endDate = @([[NSDate date] timeIntervalSince1970]);
[self.client historyForChannel:@"storage" start:startDate end:endDate
                withCompletion:^(PNHistoryResult * _Nullable result, PNErrorStatus * _Nullable status) {

    // Check whether request successfully completed or not.
    if (!status.isError) {

       // Handle downloaded history using: 
       //   result.data.start - oldest message time stamp in response
       //   result.data.end - newest message time stamp in response
       //   result.data.messages - list of messages
    }
    // Request processing failed.
    else {
    
       // Handle message history download error. Check 'category' property to find out possible 
       // issue because of which request did fail.
       //
       // Request can be resent using: [status retry];
    }
}];
 @endcode
 
 @param channel   Name of the channel for which events should be pulled out from storage.
 @param startDate Reference on time token for oldest event starting from which next should be returned events.
                  Value will be converted to required precision internally.
 @param endDate   Reference on time token for latest event till which events should be pulled out. Value will
                  be converted to required precision internally.
 @param block     History pull processing completion block which pass two arguments: \c result - in case of 
                  successful request processing \c data field will contain results of history request 
                  operation; \c status - in case if error occurred during request processing.
 
 @since 4.0
 */
- (void)historyForChannel:(NSString *)channel start:(nullable NSNumber *)startDate 
                      end:(nullable NSNumber *)endDate withCompletion:(PNHistoryCompletionBlock)block;

/**
 @brief      Allow to fetch events from specified \c channel's history within specified time frame.
 @note       This method allow to fetch up to \b 100 events at once. If in specified time frame there is more
             then 100 events paging may be required. For paging use last event time token from response and
             some distant future date for next portion of events.
 @discussion Extension to \c -historyForChannel:start:end:withCompletion: and allow to specify maximum number
             of events which should be returned with response, but not more then \b 100.
 @discussion \b Example:
 
 @code
// Client configuration.
PNConfiguration *configuration = [PNConfiguration configurationWithPublishKey:@"demo" 
                                                                 subscribeKey:@"demo"];
self.client = [PubNub clientWithConfiguration:configuration];
NSNumber *startDate = @([[NSDate dateWithTimeIntervalSinceNow:-(60*60)] timeIntervalSince1970]);
NSNumber *endDate = @([[NSDate date] timeIntervalSince1970]);
[self.client historyForChannel:@"storage" start:startDate end:endDate limit:50
                withCompletion:^(PNHistoryResult * _Nullable result, PNErrorStatus * _Nullable status) {

    // Check whether request successfully completed or not.
    if (!status.isError) {

       // Handle downloaded history using: 
       //   result.data.start - oldest message time stamp in response
       //   result.data.end - newest message time stamp in response
       //   result.data.messages - list of messages
    }
    // Request processing failed.
    else {
    
       // Handle message history download error. Check 'category' property to find out possible 
       // issue because of which request did fail.
       //
       // Request can be resent using: [status retry];
    }
}];
 @endcode
 
 @param channel   Name of the channel for which events should be pulled out from storage.
 @param startDate Reference on time token for oldest event starting from which next should be returned events.
                  Value will be converted to required precision internally.
 @param endDate   Reference on time token for latest event till which events should be pulled out. Value will 
                  be converted to required precision internally.
 @param limit     Maximum number of events which should be returned in response (not more then \b 100).
 @param block     History pull processing completion block which pass two arguments: \c result - in case of 
                  successful request processing \c data field will contain results of history request 
                  operation; \c status - in case if error occurred during request processing.
 
 @since 4.0
 */
- (void)historyForChannel:(NSString *)channel start:(nullable NSNumber *)startDate 
                      end:(nullable NSNumber *)endDate limit:(NSUInteger)limit 
           withCompletion:(PNHistoryCompletionBlock)block;

/**
 @brief      Allow to fetch requested amount of events from specified \c channel's events storage which is
             older than specified time token.
 @discussion Allow to fetch up to \c limit number of events from \c channel's events storage. If \b 0 set for
             \c limit - all events which available in \c channel's events storage and oler than specified date
             will be pulled out.
 @warning    Memory consumption will grow proportional to number of fetched messages. Consider to use 
             reasanoble \c limit value. 
 @discussion \b Example:
 
 @code
// Client configuration.
PNConfiguration *configuration = [PNConfiguration configurationWithPublishKey:@"demo" 
                                                                 subscribeKey:@"demo"];
self.client = [PubNub clientWithConfiguration:configuration];
NSNumber *referenceDate = @([[NSDate dateWithTimeIntervalSinceNow:-(60*60)] timeIntervalSince1970]);
[self.client historyForChannel:@"storage" olderThan:referenceDate limit:320
                withCompletion:^(PNHistoryResult * _Nullable result, PNErrorStatus * _Nullable status) {

    // Check whether request successfully completed or not.
    if (!status.isError) {

       // Handle downloaded history using: 
       //   result.data.start - oldest message time stamp in response
       //   result.data.end - newest message time stamp in response
       //   result.data.messages - list of messages
    }
    // Request processing failed.
    else {
    
       // Handle message history download error. Check 'category' property to find out possible 
       // issue because of which request did fail.
       //
       // Request can be resent using: [status retry];
    }
}];
 @endcode
 
 @param channel Name of the channel for which events should be pulled out from storage.
 @param date    Reference date which will be used by client to fetch events older than reference date. Value 
                will be converted to required precision internally.
 @param limit   Maximum number of events which should be returned in response. If value more than \b 100
                (service limit per request) will be passed, series of requests will be done to fetch as much
                message as available to fulfill \c limit request. \b 0 allow to fetch all available events 
                till specified \c date.
 @param block   History pull processing completion block which pass two arguments: \c result - in case of 
                successful request processing \c data field will contain results of history request 
                operation; \c status - in case if error occurred during request processing.
 
 @since 4.4
 */
- (void)historyForChannel:(NSString *)channel olderThan:(NSNumber *)date limit:(NSUInteger)limit
           withCompletion:(PNHistoryCompletionBlock)block;

/**
 @brief      Allow to fetch requested amount of events from specified \c channel's events storage which is
             newer than specified time token.
 @discussion Allow to fetch up to \c limit number of events from \c channel's events storage. If \b 0 set for
             \c limit - all events which available in \c channel's events storage and oler than specified date
             will be pulled out.
 @warning    Memory consumption will grow proportional to number of fetched messages. Consider to use 
             reasanoble \c limit value. 
 @discussion \b Example:
 
 @code
// Client configuration.
PNConfiguration *configuration = [PNConfiguration configurationWithPublishKey:@"demo" 
                                                                 subscribeKey:@"demo"];
self.client = [PubNub clientWithConfiguration:configuration];
NSNumber *referenceDate = @([[NSDate dateWithTimeIntervalSinceNow:-(60*60)] timeIntervalSince1970]);
[self.client historyForChannel:@"storage" newerThan:referenceDate limit:320
                withCompletion:^(PNHistoryResult * _Nullable result, PNErrorStatus * _Nullable status) {

    // Check whether request successfully completed or not.
    if (!status.isError) {

       // Handle downloaded history using: 
       //   result.data.start - oldest message time stamp in response
       //   result.data.end - newest message time stamp in response
       //   result.data.messages - list of messages
    }
    // Request processing failed.
    else {
    
       // Handle message history download error. Check 'category' property to find out possible 
       // issue because of which request did fail.
       //
       // Request can be resent using: [status retry];
    }
}];
 @endcode
 
 @param channel Name of the channel for which events should be pulled out from storage.
 @param date    Reference date which will be used by client to fetch events newer than reference date. Value 
                will be converted to required precision internally.
 @param limit   Maximum number of events which should be returned in response. If value more than \b 100
                (service limit per request) will be passed, series of requests will be done to fetch as much
                message as available to fulfill \c limit request. \b 0 allow to fetch all available events 
                since specified \c date.
 @param block   History pull processing completion block which pass two arguments: \c result - in case of 
                successful request processing \c data field will contain results of history request 
                operation; \c status - in case if error occurred during request processing.
 
 @since 4.4
 */
- (void)historyForChannel:(NSString *)channel newerThan:(NSNumber *)date limit:(NSUInteger)limit
           withCompletion:(PNHistoryCompletionBlock)block;

/**
 @brief      Allow to fetch all events from specified \c channel's history in specified time frame.
 @discussion Client will try to pull all events from start date (excluding it) up to end date (excluding). If
             specified date won't be reached with single request additional requests will be done 
             automatically.
 @discussion \b Example:
 
 @code
// Client configuration.
PNConfiguration *configuration = [PNConfiguration configurationWithPublishKey:@"demo" 
                                                                 subscribeKey:@"demo"];
self.client = [PubNub clientWithConfiguration:configuration];
NSNumber *startDate = @([[NSDate dateWithTimeIntervalSinceNow:-(60*60)] timeIntervalSince1970]);
NSNumber *endDate = @([[NSDate date] timeIntervalSince1970]);
[self.client historyForChannel:@"storage" between:@[startDate, endDate]
                withCompletion:^(PNHistoryResult * _Nullable result, PNErrorStatus * _Nullable status) {

    // Check whether request successfully completed or not.
    if (!status.isError) {

       // Handle downloaded history using: 
       //   result.data.start - oldest message time stamp in response
       //   result.data.end - newest message time stamp in response
       //   result.data.messages - list of messages
    }
    // Request processing failed.
    else {
    
       // Handle message history download error. Check 'category' property to find out possible 
       // issue because of which request did fail.
       //
       // Request can be resent using: [status retry];
    }
}];
 @endcode
 
 @param channel   Name of the channel for which events should be pulled out from storage.
 @param timeFrame Reference on array with \b two time token elements between which all events should be 
                  returned. Time tokens will be converted to required precision internally.
 @param block     History pull processing completion block which pass two arguments: \c result - in case of 
                  successful request processing \c data field will contain results of history request 
                  operation; \c status - in case if error occurred during request processing.
 
 @since 4.4
 */
- (void)historyForChannel:(NSString *)channel between:(NSArray<NSNumber *> *)timeFrame
           withCompletion:(PNHistoryCompletionBlock)block;


///------------------------------------------------
/// @name History in frame with extended response
///------------------------------------------------

/**
 @brief      Allow to fetch requested amount of events from specified \c channel's events storage.
 @discussion Allow to fetch up to \c limit number of events from \c channel's events storage. If \b 0 set for
             \c limit - all events available in \c channel's events storage will be pulled out.
 @discussion Extension to \c -historyForChannel:limit:withCompletion: and allow to specify whether event dates
             (time tokens) should be included in response or not.
 @warning    Memory consumption will grow proportional to number of fetched messages. Consider to use 
             reasanoble \c limit value. 
 @discussion \b Example:
 
 @code
// Client configuration.
PNConfiguration *configuration = [PNConfiguration configurationWithPublishKey:@"demo" 
                                                                 subscribeKey:@"demo"];
self.client = [PubNub clientWithConfiguration:configuration];
[self.client historyForChannel:@"storage" limit:250 includeTimeToken:YES
                withCompletion:^(PNHistoryResult * _Nullable result, PNErrorStatus * _Nullable status) {

    // Check whether request successfully completed or not.
    if (!status.isError) {

       // Handle downloaded history using: 
       //   result.data.start - oldest message time stamp in response
       //   result.data.end - newest message time stamp in response
       //   result.data.messages - list of messages
    }
    // Request processing failed.
    else {
    
       // Handle message history download error. Check 'category' property to find out possible 
       // issue because of which request did fail.
       //
       // Request can be resent using: [status retry];
    }
}];
 @endcode
 
 @param channel                Name of the channel for which events should be pulled out from storage.
 @param limit                  Maximum number of events which should be returned in response. If value more 
                               than \b 100 (service limit per request) will be passed, series of requests will
                               be done to fetch as much message as available to fulfill \c limit request. \b 0
                               allow to fetch all available events till specified \c date.
 @param shouldIncludeTimeToken Whether event dates (time tokens) should be included in response or not.
 @param block                  History pull processing completion block which pass two arguments: 
                               \c result - in case of successful request processing \c data field will contain
                               results of history request operation; \c status - in case if error occurred
                               during request processing.
 
 @since 4.4
 */
- (void)historyForChannel:(NSString *)channel limit:(NSUInteger)limit 
         includeTimeToken:(BOOL)shouldIncludeTimeToken withCompletion:(PNHistoryCompletionBlock)block;

/**
 @brief      Allow to fetch events from specified \c channel's history within specified time frame.
 @note       This method allow to fetch up to \b 100 events at once. If in specified time frame there is more
             then 100 events paging may be required. For paging use last event time token from response and
             some distant future date for next portion of events.
 @discussion Extension to \c -historyForChannel:start:end:withCompletion: and allow to specify whether event 
             dates (time tokens) should be included in response or not.
 @discussion \b Example:
 
 @code
// Client configuration.
PNConfiguration *configuration = [PNConfiguration configurationWithPublishKey:@"demo" 
                                                                 subscribeKey:@"demo"];
self.client = [PubNub clientWithConfiguration:configuration];
NSNumber *startDate = @([[NSDate dateWithTimeIntervalSinceNow:-(60*60)] timeIntervalSince1970]);
NSNumber *endDate = @([[NSDate date] timeIntervalSince1970]);
[self.client historyForChannel:@"storage" start:startDate end:endDate includeTimeToken:YES
                withCompletion:^(PNHistoryResult * _Nullable result, PNErrorStatus * _Nullable status) {

    // Check whether request successfully completed or not.
    if (!status.isError) {

       // Handle downloaded history using: 
       //   result.data.start - oldest message time stamp in response
       //   result.data.end - newest message time stamp in response
       //   result.data.messages - list of dictionaries. Each entry will include two keys: 
       //                          "message" - for body and "timetoken" for date when message has
       //                          been sent.
    }
    // Request processing failed.
    else {
    
       // Handle message history download error. Check 'category' property to find out possible 
       // issue because of which request did fail.
       //
       // Request can be resent using: [status retry];
    }
}];
 @endcode
 
 @param channel                Name of the channel for which events should be pulled out from storage.
 @param startDate              Reference on time token for oldest event starting from which next should be 
                               returned events. Value will be converted to required precision internally.
 @param endDate                Reference on time token for latest event till which events should be pulled 
                               out. Value will be converted to required precision internally.
 @param shouldIncludeTimeToken Whether event dates (time tokens) should be included in response or not.
 @param block                  History pull processing completion block which pass two arguments:
                               \c result - in case of successful request processing \c data field will contain
                               results of history request operation; \c status - in case if error occurred
                               during request processing.
 
 @since 4.0
 */
- (void)historyForChannel:(NSString *)channel start:(nullable NSNumber *)startDate 
                      end:(nullable NSNumber *)endDate includeTimeToken:(BOOL)shouldIncludeTimeToken 
           withCompletion:(PNHistoryCompletionBlock)block;

/**
 @brief      Allow to fetch events from specified \c channel's history within specified time frame.
 @note       This method allow to fetch up to \b 100 events at once. If in specified time frame there is more
             then 100 events paging may be required. For paging use last event time token from response and
             some distant future date for next portion of events.
 @discussion Extension to \c -historyForChannel:start:end:includeTimeToken:withCompletion: and allow to 
             specify maximum number of events which should be returned with response, but not more then 
             \b 100.
 @discussion \b Example:
 
 @code
// Client configuration.
PNConfiguration *configuration = [PNConfiguration configurationWithPublishKey:@"demo" 
                                                                 subscribeKey:@"demo"];
self.client = [PubNub clientWithConfiguration:configuration];
NSNumber *startDate = @([[NSDate dateWithTimeIntervalSinceNow:-(60*60)] timeIntervalSince1970]);
NSNumber *endDate = @([[NSDate date] timeIntervalSince1970]);
[self.client historyForChannel:@"storage" start:startDate end:endDate limit:35 includeTimeToken:YES
                withCompletion:^(PNHistoryResult * _Nullable result, PNErrorStatus * _Nullable status) {

    // Check whether request successfully completed or not.
    if (!status.isError) {

       // Handle downloaded history using: 
       //   result.data.start - oldest message time stamp in response
       //   result.data.end - newest message time stamp in response
       //   result.data.messages - list of dictionaries. Each entry will include two keys: 
       //                          "message" - for body and "timetoken" for date when message has
       //                          been sent.
    }
    // Request processing failed.
    else {
    
       // Handle message history download error. Check 'category' property to find out possible 
       // issue because of which request did fail.
       //
       // Request can be resent using: [status retry];
    }
}];
 @endcode
 
 @param channel                Name of the channel for which events should be pulled out from storage.
 @param startDate              Reference on time token for oldest event starting from which next should be 
                               returned events. Value will be converted to required precision internally.
 @param endDate                Reference on time token for latest event till which events should be pulled 
                               out. Value will be converted to required precision internally.
 @param limit                  Maximum number of events which should be returned in response (not more then
                               \b 100).
 @param shouldIncludeTimeToken Whether event dates (time tokens) should be included in response or not.
 @param block                  History pull processing completion block which pass two arguments:
                               \c result - in case of successful request processing \c data field will contain
                               results of history request operation; \c status - in case if error occurred 
                               during request processing.
 
 @since 4.0
 */
- (void)historyForChannel:(NSString *)channel start:(nullable NSNumber *)startDate 
                      end:(nullable NSNumber *)endDate limit:(NSUInteger)limit
         includeTimeToken:(BOOL)shouldIncludeTimeToken withCompletion:(PNHistoryCompletionBlock)block;

/**
 @brief      Allow to fetch events from specified \c channel's history within specified time frame.
 @note       This method allow to fetch up to \b 100 events at once. If in specified time frame there is more
             then 100 events paging may be required. For paging use last event time token from response and
             some distant future date for next portion of events.
 @discussion Extension to \c -historyForChannel:start:end:limit:withCompletion: and allow to specify whether
             events order in response should be reversed or not.
 @discussion \b Example:
 
 @code
// Client configuration.
PNConfiguration *configuration = [PNConfiguration configurationWithPublishKey:@"demo" 
                                                                 subscribeKey:@"demo"];
self.client = [PubNub clientWithConfiguration:configuration];
NSNumber *startDate = @([[NSDate dateWithTimeIntervalSinceNow:-(60*60)] timeIntervalSince1970]);
NSNumber *endDate = @([[NSDate date] timeIntervalSince1970]);
[self.client historyForChannel:@"storage" start:startDate end:endDate limit:35 reverse:YES
                withCompletion:^(PNHistoryResult * _Nullable result, PNErrorStatus * _Nullable status) {

    // Check whether request successfully completed or not.
    if (!status.isError) {

       // Handle downloaded history using: 
       //   result.data.start - oldest message time stamp in response
       //   result.data.end - newest message time stamp in response
       //   result.data.messages - list of messages
    }
    // Request processing failed.
    else {
    
       // Handle message history download error. Check 'category' property to find out possible 
       // issue because of which request did fail.
       //
       // Request can be resent using: [status retry];
    }
}];
 @endcode
 
 @param channel            Name of the channel for which events should be pulled out from storage.
 @param startDate          Reference on time token for oldest event starting from which next should be 
                           returned events. Value will be converted to required precision internally.
 @param endDate            Reference on time token for latest event till which events should be pulled out. 
                           Value will be converted to required precision internally.
 @param limit              Maximum number of events which should be returned in response (not more then 
                           \b 100).
 @param shouldReverseOrder Whether events order in response should be reversed or not.
 @param block              History pull processing completion block which pass two arguments: \c result - in 
                           case of successful request processing \c data field will contain results of history
                           request operation; \c status - in case if error occurred during request processing.
 
 @since 4.0
 */
- (void)historyForChannel:(NSString *)channel start:(nullable NSNumber *)startDate
                      end:(nullable NSNumber *)endDate limit:(NSUInteger)limit 
                  reverse:(BOOL)shouldReverseOrder withCompletion:(PNHistoryCompletionBlock)block;

/**
 @brief      Allow to fetch events from specified \c channel's history within specified time frame.
 @note       This method allow to fetch up to \b 100 events at once. If in specified time frame there is more
             then 100 events paging may be required. For paging use last event time token from response and
             some distant future date for next portion of events.
 @discussion Extension to \c -historyForChannel:start:end:limit:reverse:withCompletion: and allow to specify 
             whether events order in response should be reversed or not.
 @discussion \b Example:
 
 @code
// Client configuration.
PNConfiguration *configuration = [PNConfiguration configurationWithPublishKey:@"demo" 
                                                                 subscribeKey:@"demo"];
self.client = [PubNub clientWithConfiguration:configuration];
NSNumber *startDate = @([[NSDate dateWithTimeIntervalSinceNow:-(60*60)] timeIntervalSince1970]);
NSNumber *endDate = @([[NSDate date] timeIntervalSince1970]);
[self.client historyForChannel:@"storage" start:startDate end:endDate limit:35 reverse:YES 
              includeTimeToken:YES 
                withCompletion:^(PNHistoryResult * _Nullable result, PNErrorStatus * _Nullable status) {

    // Check whether request successfully completed or not.
    if (!status.isError) {

       // Handle downloaded history using: 
       //   result.data.start - oldest message time stamp in response
       //   result.data.end - newest message time stamp in response
       //   result.data.messages - list of dictionaries. Each entry will include two keys: 
       //                          "message" - for body and "timetoken" for date when message has
       //                          been sent.
    }
    // Request processing failed.
    else {
    
       // Handle message history download error. Check 'category' property to find out possible 
       // issue because of which request did fail.
       //
       // Request can be resent using: [status retry];
    }
}];
 @endcode
 
 @param channel                Name of the channel for which events should be pulled out from storage.
 @param startDate              Reference on time token for oldest event starting from which next should be 
                               returned events. Value will be converted to required precision internally.
 @param endDate                Reference on time token for latest event till which events should be pulled 
                               out. Value will be converted to required precision internally.
 @param limit                  Maximum number of events which should be returned in response (not more then 
                               \b 100).
 @param shouldReverseOrder     Whether events order in response should be reversed or not.
 @param shouldIncludeTimeToken Whether event dates (time tokens) should be included in response or not.
 @param block                  History pull processing completion block which pass two arguments: 
                               \c result - in case of successful request processing \c data field will contain
                               results of history request operation; \c status - in case if error occurred 
                               during request processing.
 
 @since 4.0
 */
- (void)historyForChannel:(NSString *)channel start:(nullable NSNumber *)startDate
                      end:(nullable NSNumber *)endDate limit:(NSUInteger)limit 
                  reverse:(BOOL)shouldReverseOrder includeTimeToken:(BOOL)shouldIncludeTimeToken 
           withCompletion:(PNHistoryCompletionBlock)block;

/**
 @brief      Allow to fetch requested amount of events from specified \c channel's events storage which is
             older than specified time token.
 @discussion Allow to fetch up to \c limit number of events from \c channel's events storage. If \b 0 set for
             \c limit - all events which available in \c channel's events storage and oler than specified date
             will be pulled out.
 @discussion Extension to \c -historyForChannel:olderThan:limit:withCompletion: and allow to specify whether 
             event dates (time tokens) should be included in response or not.
 @warning    Memory consumption will grow proportional to number of fetched messages. Consider to use 
             reasanoble \c limit value. 
 @discussion \b Example:
 
 @code
// Client configuration.
PNConfiguration *configuration = [PNConfiguration configurationWithPublishKey:@"demo" 
                                                                 subscribeKey:@"demo"];
self.client = [PubNub clientWithConfiguration:configuration];
NSNumber *referenceDate = @([[NSDate dateWithTimeIntervalSinceNow:-(60*60)] timeIntervalSince1970]);
[self.client historyForChannel:@"storage" olderThan:referenceDate limit:320 includeTimeToken:YES
                withCompletion:^(PNHistoryResult * _Nullable result, PNErrorStatus * _Nullable status) {

    // Check whether request successfully completed or not.
    if (!status.isError) {

       // Handle downloaded history using: 
       //   result.data.start - oldest message time stamp in response
       //   result.data.end - newest message time stamp in response
       //   result.data.messages - list of messages
    }
    // Request processing failed.
    else {
    
       // Handle message history download error. Check 'category' property to find out possible 
       // issue because of which request did fail.
       //
       // Request can be resent using: [status retry];
    }
}];
 @endcode
 
 @param channel                Name of the channel for which events should be pulled out from storage.
 @param date                   Reference date which will be used by client to fetch events older than 
                               reference date. Value will be converted to required precision internally.
 @param limit                  Maximum number of events which should be returned in response. If value more 
                               than \b 100 (service limit per request) will be passed, series of requests will
                               be done to fetch as much message as available to fulfill \c limit request. \b 0
                               allow to fetch all available events till specified \c date.
 @param shouldIncludeTimeToken Whether event dates (time tokens) should be included in response or not.
 @param block                  History pull processing completion block which pass two arguments: 
                               \c result - in case of successful request processing \c data field will contain
                               results of history request operation; \c status - in case if error occurred 
                               during request processing.
 
 @since 4.4
 */
- (void)historyForChannel:(NSString *)channel olderThan:(NSNumber *)date limit:(NSUInteger)limit
         includeTimeToken:(BOOL)shouldIncludeTimeToken withCompletion:(PNHistoryCompletionBlock)block;

/**
 @brief      Allow to fetch requested amount of events from specified \c channel's events storage which is
             newer than specified time token.
 @discussion Allow to fetch up to \c limit number of events from \c channel's events storage. If \b 0 set for
             \c limit - all events which available in \c channel's events storage and oler than specified date
             will be pulled out.
 @discussion Extension to \c -historyForChannel:newerThan:limit:withCompletion: and allow to specify whether 
             event dates (time tokens) should be included in response or not.
 @warning    Memory consumption will grow proportional to number of fetched messages. Consider to use 
             reasanoble \c limit value. 
 @discussion \b Example:
 
 @code
// Client configuration.
PNConfiguration *configuration = [PNConfiguration configurationWithPublishKey:@"demo" 
                                                                 subscribeKey:@"demo"];
self.client = [PubNub clientWithConfiguration:configuration];
NSNumber *referenceDate = @([[NSDate dateWithTimeIntervalSinceNow:-(60*60)] timeIntervalSince1970]);
[self.client historyForChannel:@"storage" newerThan:referenceDate limit:320 includeTimeToken:YES
                withCompletion:^(PNHistoryResult * _Nullable result, PNErrorStatus * _Nullable status) {

    // Check whether request successfully completed or not.
    if (!status.isError) {

       // Handle downloaded history using: 
       //   result.data.start - oldest message time stamp in response
       //   result.data.end - newest message time stamp in response
       //   result.data.messages - list of messages
    }
    // Request processing failed.
    else {
    
       // Handle message history download error. Check 'category' property to find out possible 
       // issue because of which request did fail.
       //
       // Request can be resent using: [status retry];
    }
}];
 @endcode
 
 @param channel                Name of the channel for which events should be pulled out from storage.
 @param date                   Reference date which will be used by client to fetch events newer than 
                               reference date. Value will be converted to required precision internally.
 @param limit                  Maximum number of events which should be returned in response. If value more 
                               than \b 100 (service limit per request) will be passed, series of requests will
                               be done to fetch as much message as available to fulfill \c limit request. 
                               \b 0 allow to fetch all available events since specified \c date.
 @param shouldIncludeTimeToken Whether event dates (time tokens) should be included in response or not.
 @param block                  History pull processing completion block which pass two arguments:
                               \c result - in case of successful request processing \c data field will contain
                               results of history request operation; \c status - in case if error occurred 
                               during request processing.
 
 @since 4.4
 */
- (void)historyForChannel:(NSString *)channel newerThan:(NSNumber *)date limit:(NSUInteger)limit
         includeTimeToken:(BOOL)shouldIncludeTimeToken withCompletion:(PNHistoryCompletionBlock)block;

/**
 @brief      Allow to fetch all events from specified \c channel's history in specified time frame.
 @discussion Client will try to pull all events from start date (excluding it) up to end date (excluding). If
             specified date won't be reached with single request additional requests will be done 
             automatically.
 @discussion Extension to \c -historyForChannel:between:withCompletion: and allow to specify whether event 
             dates (time tokens) should be included in response or not.
 @discussion \b Example:
 
 @code
// Client configuration.
PNConfiguration *configuration = [PNConfiguration configurationWithPublishKey:@"demo" 
                                                                 subscribeKey:@"demo"];
self.client = [PubNub clientWithConfiguration:configuration];
NSNumber *startDate = @([[NSDate dateWithTimeIntervalSinceNow:-(60*60)] timeIntervalSince1970]);
NSNumber *endDate = @([[NSDate date] timeIntervalSince1970]);
[self.client historyForChannel:@"storage" between:@[startDate, endDate] includeTimeToken:YES
                withCompletion:^(PNHistoryResult * _Nullable result, PNErrorStatus * _Nullable status) {

    // Check whether request successfully completed or not.
    if (!status.isError) {

       // Handle downloaded history using: 
       //   result.data.start - oldest message time stamp in response
       //   result.data.end - newest message time stamp in response
       //   result.data.messages - list of messages
    }
    // Request processing failed.
    else {
    
       // Handle message history download error. Check 'category' property to find out possible 
       // issue because of which request did fail.
       //
       // Request can be resent using: [status retry];
    }
}];
 @endcode
 
 @param channel                Name of the channel for which events should be pulled out from storage.
 @param timeFrame              Reference on array with \b two time token elements between which all events 
                               should be returned. Time tokens will be converted to required precision 
                               internally.
 @param shouldIncludeTimeToken Whether event dates (time tokens) should be included in response or not.
 @param block                  History pull processing completion block which pass two arguments: 
                               \c result - in case of successful request processing \c data field will contain
                               results of history request operation; \c status - in case if error occurred 
                               during request processing.
 
 @since 4.4
 */
- (void)historyForChannel:(NSString *)channel between:(NSArray<NSNumber *> *)timeFrame
         includeTimeToken:(BOOL)shouldIncludeTimeToken withCompletion:(PNHistoryCompletionBlock)block;

#pragma mark -


@end

NS_ASSUME_NONNULL_END
