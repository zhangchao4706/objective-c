#import "PubNub+Core.h"
#import <CocoaLumberjack/CocoaLumberjack.h>
#import <AFNetworkReachabilityManager.h>
#import <AFHTTPSessionManager.h>
#import "PNLog.h"


#pragma mark Static

/**
 @brief  Cocoa Lumberjack logging level configuration for \b PubNub client class and categories.
 
 @since 4.0
 */
static DDLogLevel ddLogLevel = (DDLogLevel)PNVerboseLogLevel;


#pragma mark - Class forward

@class PNRequest, PNResult, PNStatus;


/**
 @brief      \b PubNub client core extension which expose private fields and methods to support 
             other extensions.
 @discussion Core class manage client configuration as well as access to networking layer through 
             which passed request will be sent to \b PubNub service.
 
 @author Sergey Mamontov
 @since 4.0
 @copyright © 2009-2015 PubNub, Inc.
 */
@interface PubNub (Private)


///------------------------------------------------
/// @name Properties
///------------------------------------------------

/**
 @brief      Stores reference on dispatch queue which is used to synchronize access to \b PubNub 
             client configuration.
 @discussion Client provide ability to change any of it's configuration fields on run-time. Changes
             may happen on different threads in relation to thread on which client may need to read
             them.

 @since 4.0
*/
@property (nonatomic, readonly, strong) dispatch_queue_t configurationAccessQueue;

/**
 @brief      Stores reference on dispatch queue which is used to synchronize access to resources 
             related to subscription process.
 @discussion Subscribe and leave requests issued on this queue because their order should be 
             strictly serialized.
 
 @since 4.0
 */
@property (nonatomic, readonly, strong) dispatch_queue_t subscribeQueue;

/**
 @brief      Stores reference on dispatch queue which is used by non-subscription APIs.
 @discussion Queue is used to issue non-subscription API request to \b PubNub service like message
             posting, history, presence and other.
 
 @since 4.0
 */
@property (nonatomic, readonly, strong) dispatch_queue_t serviceQueue;

/**
 @brief  Stores reference about recent client state (whether it was connected or not).
 
 @since 4.0
 */
@property (nonatomic, readonly, assign) PNStatusCategory recentClientStatus;

/**
 @brief Stores reference on session with pre-configured options useful for 'non-subscription' API 
        group.
 
 @since 4.0
 */
@property (nonatomic, readonly, strong) AFHTTPSessionManager *serviceSession;


///------------------------------------------------
/// @name Operation processing
///------------------------------------------------

/**
 @brief  Complete request configuration and use it to communicate with \b PubNub service.
 
 @param request Reference on request instance which hold base information required to build request
                to get access to remote data objects.
 
 @since 4.0
 */
- (void)processRequest:(PNRequest *)request;

/**
 @brief      Notify about request processing error.
 @discussion In most cases this method will be used by client itself to report about API calls
             processing issues (incomplete or unacceptable parameters has been passed to methods).

 @since 4.0
 */
- (void)handleRequestFailure:(PNRequest *)request withError:(NSError *)error;

/**
 @brief  Handle successful request task execution with further building of result and status 
         objects which will be distributed to the user.
 
 @param request Reference on original request which has been used to build network request to PubNub
                service.
 @param task    Reference on data download task which hold information about NSURL request used to
                load data and server response information.
 @param data    Reference on data which has been downloaded in response for client request.
 
 @since 4.0
 */
- (void)handleRequestSuccess:(PNRequest *)request withTask:(NSURLSessionDataTask *)task
                     andData:(id)data;

/**
 @brief  Handle request task execution failure with further building of result and status objects 
         which will be distributed to the user.
 
 @param request Reference on original request which has been used to build network request to PubNub
                service.
 @param task    Reference on data download task which hold information about NSURL request used to
                load data and server response information.
 @param error   Reference on error which occurred while request executed.
 
 @since 4.0
 */
- (void)handleRequestFailure:(PNRequest *)request withTask:(NSURLSessionDataTask *)task
                    andError:(NSError *)error;

/**
 @brief  Terminate any long-poll based tasks.

 @since 4.0
 */
- (void)cancelAllLongPollRequests;


///------------------------------------------------
/// @name Events notification
///------------------------------------------------

/**
 @brief  Notify user about processing results by calling completion block with specified result and
         status on callback queue.

 @param block  Reference on completion block which has been passed by user during API call.
 @param result Reference on API request processing results.
 @param status Reference on API request processing status (mostly reports about errors).

 @since 4.0
 */
- (void)callBlock:(id)block status:(BOOL)callingStatusBlock withResult:(PNResult *)result
        andStatus:(PNStatus *)status;


///------------------------------------------------
/// @name Processing
///------------------------------------------------

/**
 @brief      Default request result processing method which will return corresponding objects
             depending on request results data.
 @discussion This method mostly used by API endpoints which doesn't require additional data 
             pre-processing before objects will be passed further.
 
 @param result Reference on address pointer where request's result object should be stored.
 @param status Reference on address pointer where request's processing status object should be 
               stored.
 @param request Reference on request which should be processed and correspondoing objects generated.
 
 @since 4.0
 */
- (void)getResult:(PNResult **)result andStatus:(PNStatus **)status forRequest:(PNRequest *)request;

/**
 @brief  Compile status object with all client state information which is available at this moment.
 
 @param status Reference on object which should be completed with client state.
 
 @since 4.0
 */
- (void)completeStatusObject:(PNStatus *)status;

#pragma mark -


@end
