#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

@class MediaPlayer;

typedef enum _MediaPlayerNotifyCodes
{
    PLP_BUILD_STARTING              = 1,
    PLP_BUILD_SUCCESSFUL            = 2,
    PLP_BUILD_FAILED                = 3,
    
    PLP_PLAY_STARTING               = 4,
    PLP_PLAY_SUCCESSFUL             = 5,
    PLP_PLAY_FAILED                 = 6,
    
    PLP_CLOSE_STARTING              = 7,
    PLP_CLOSE_SUCCESSFUL            = 8,
    PLP_CLOSE_FAILED                = 9,
    
    PLP_ERROR                       = 10,
    PLP_INTERRUPTED                 = 11,
    PLP_EOS                         = 12,
    
    PLP_PLAY_PLAY                   = 14,
    PLP_PLAY_PAUSE                  = 15,
    PLP_PLAY_STOP                   = 16,
    
    PLP_SEEK_COMPLETED              = 17,
    
    CP_CONNECT_STARTING             = 101,
    CP_CONNECT_SUCCESSFUL           = 102,
    CP_CONNECT_FAILED               = 103,
    CP_INTERRUPTED                  = 104,
    CP_ERROR_DISCONNECTED           = 105,
    CP_STOPPED                      = 106,
    CP_INIT_FAILED                  = 107,
    CP_RECORD_STARTED               = 108,
    CP_RECORD_STOPPED               = 109,
    CP_RECORD_CLOSED                = 110,
    CP_BUFFER_FILLED                = 111,
    CP_ERROR_NODATA_TIMEOUT         = 112,
    CP_SOURCE_AUDIO_DISCONTINUITY   = 113,
    CP_SOURCE_VIDEO_DISCONTINUITY   = 114,
    CP_START_BUFFERING              = 115,
    CP_STOP_BUFFERING               = 116,
    
    VDP_STOPPED                     = 201,
    VDP_INIT_FAILED                 = 202,
    VDP_LASTFRAME                   = 203,
    
    VRP_STOPPED                     = 300,
    VRP_INIT_FAILED                 = 301,
    VRP_NEED_SURFACE                = 302,
    VRP_FIRSTFRAME                  = 305,
    VRP_LASTFRAME                   = 306,
    VRP_FIRSTFRAME_AFTER_PAUSE      = 308,
    VRP_SURFACE_ACQUIRE             = 309,
    VRP_SURFACE_LOST                = 310,
    
    
    ADP_STOPPED                     = 400,
    ADP_INIT_FAILED                 = 401,
    ADP_LASTFRAME                   = 402,
    
    ARP_STOPPED                     = 500,
    ARP_INIT_FAILED                 = 501,
    ARP_LASTFRAME                   = 502,
    ARP_VOLUME_DETECTED             = 503,
    
    CRP_STOPPED                     = 600,
    
    SDP_STOPPED_THREAD              = 701,
    SDP_FAILED_INIT                 = 702,
    SDP_LASTFRAME                   = 703
    
} MediaPlayerNotifyCodes;

typedef enum _MediaPlayerState
{
    MediaPlayerOpening  = 0,
    MediaPlayerOpened   = 1,
    
    MediaPlayerStarted  = 2,
    MediaPlayerPaused   = 3,
    MediaPlayerStopped  = 4,
    
    MediaPlayerClosing  = 5,
    MediaPlayerClosed   = 6
    
} MediaPlayerState;

typedef enum _MediaPlayerModes
{
    PP_MODE_ALL         = -1,        //default
    PP_MODE_VIDEO       = 1,
    PP_MODE_AUDIO       = 2,
    PP_MODE_SUBTITLE    = 4,
    PP_MODE_RECORD      = 8

} MediaPlayerModes;

typedef enum _MediaPlayerRecordFlags
{
    RECORD_NO_START          = 0,
    RECORD_AUTO_START        = 1,
    RECORD_SPLIT_BY_TIME     = 2,
    RECORD_SPLIT_BY_SIZE     = 4,
    RECORD_DISABLE_VIDEO     = 8,
    RECORD_DISABLE_AUDIO     = 10
    
} MediaPlayerRecordFlags;

typedef enum _MediaPlayerRecordStat
{
    RECORD_STAT_LASTERROR        = 0, //last error
    RECORD_STAT_DURATION         = 1, //in msec
    RECORD_STAT_SIZE             = 2, //in bytes
    RECORD_STAT_DURATION_TOTAL   = 3, //in msec
    RECORD_STAT_SIZE_TOTAL       = 4  //in bytes
    
} MediaPlayerRecordStat;


#import "MediaPlayerConfig.h"

// callback
@protocol MediaPlayerCallback<NSObject>

- (int) Status: (MediaPlayer*)player
         args: (int)arg;

- (int) OnReceiveData: (MediaPlayer*)player
              buffer: (void*)buffer
                size: (int) size
                 pts: (long) pts;

- (int) OnReceiveSubtitleString: (MediaPlayer*)player
                           data: (NSString*)data
                       duration: (uint64_t)duration;

@end


// main functionailty
@interface MediaPlayer : NSObject

// initializer
- (id) init:(CGRect) bounds;

// VideoView
- (UIView *) contentView;

- (void) Open: (MediaPlayerConfig*)config
     callback: (id<MediaPlayerCallback>)callback;
- (void) Close;

- (MediaPlayerConfig*) getConfig;

- (MediaPlayerState) getState;

- (int) updateView;

- (void) Play: (int)drawNumberOfFramesAndPause; // now supported only drawNumberOfFramesAndPause:1 - draw one frame and pause
- (void) Pause;
- (void) Stop;

- (void) setFFRate: (int)rate;

// Get video shot RGBA32
// Parameters:
// buffer - allocated buffer for shot
// buffer_size - in: allocated before buffer size,
//              out: real image size
// width, height - in: desired scale size. -1 for original.
//                out: real image sizes
//               note: work only for software decoding
// bytes_per_row - image bytes ber row
//
// Return: 0 - ok, (-1) - error, (-2) - need more buffer space for image
- (int) getVideoShot: (void*)buffer
         buffer_size: (int32_t*)buffer_size
               width: (int32_t*)width
              height: (int32_t*)height
       bytes_per_row: (int32_t*)bytes_per_row;

- (void) setStreamPosition: (int64_t)lTime;
- (int64_t) getStreamDuration;
- (int64_t) getStreamPosition;
- (int64_t) getRenderPosition;
- (void) setLiveStreamPath: (NSString*)path;
- (void) setStartLiveStreamPosition: (int64_t)offset;
- (void) setLiveStreamPosition: (int64_t)lTime;
- (BOOL) getLiveStreamPosition: (int64_t*)first
                       current: (int64_t*)current
                          last: (int64_t*)last
                      duration: (int64_t*)duration
                   stream_type: (int*)stream_type;

- (void) toggleMute: (BOOL)mute;

- (void) setKey: (NSString*)key;

- (int) getDataDelayOnSource;
- (int) getDataBitrateOnSource;

- (int) getStatFPS;

- (int) subtitleGetCount;
- (int) subtitleSelect: (int)stream_i;
- (int) subtitleGetSelected;
- (int) subtitleSourceAdd: (NSString*)path;
- (int) subtitleSourceRemove: (NSString*)path;

// record support
- (void) recordSetup: (NSString*)path
               flags: (MediaPlayerRecordFlags)flags
           splitTime: (int32_t)splitTime
           splitSize: (int32_t)splitSize
              prefix: (NSString*)prefix;

- (void) recordStart;
- (void) recordStop;

- (NSString*) recordGetFileName: (int32_t)param; //0 - last stopped; 1 - last started
- (int64_t) recordGetStat: (MediaPlayerRecordStat)param;


// Helper methods

// Get available directions for move mode
// Return: possible move 0 - nothing, 1 - to left, 2 - to right, 4 - to top, 8 - to bottom
- (int) getAvailableDirectionsForAspectRatioMoveMode;

// Get view sizes and video aspects like video width/height, aspect calculations etc.
// Parameters:
// view_orientation - current view orientation
// view_width - current view width
// view_height - current view height
// video_width - video width
// video_height - video height
// aspect_left - left video position after current aspect ratio mode calculations
// aspect_top - top video position after current aspect ratio mode calculations
// aspect_width - video width after current aspect ratio mode calculations
// aspect_height - video height after current aspect ratio mode calculations
// aspect_zoom - current zoom coefficient
- (void)getViewSizesAndVideoAspects:(int*)view_orientation
                         view_width:(int*)view_width
                        view_height:(int*)view_height
                        video_width:(int*)video_width
                       video_height:(int*)video_height
                        aspect_left:(int*)aspect_left
                         aspect_top:(int*)aspect_top
                       aspect_width:(int*)aspect_width
                      aspect_height:(int*)aspect_height
                        aspect_zoom:(int*)aspect_zoom;


// Get inernal buffers states
// Parameters:
// source_videodecoder_filled - bytes filled in buffer between source and video decoder
// source_videodecoder_size - buffer size between source and video decoder
// videodecoder_videorenderer_filled - buffer size between video decoder and video renderer
// videodecoder_videorenderer_size - buffer size between video decoder and video renderer
// source_audiodecoder_filled - buffer size between source and audio decoder
// source_audiodecoder_size - buffer size between source and audio decoder
// audiodecoder_audiorenderer_filled - buffer size between audio decoder and audio renderer
// audiodecoder_audiorenderer_size - buffer size between audio decoder and audio renderer
- (void)getInternalBuffersState:(int*)source_videodecoder_filled
             source_videodecoder_size:(int*)source_videodecoder_size
    videodecoder_videorenderer_filled:(int*)videodecoder_videorenderer_filled
      videodecoder_videorenderer_size:(int*)videodecoder_videorenderer_size
           source_audiodecoder_filled:(int*)source_audiodecoder_filled
             source_audiodecoder_size:(int*)source_audiodecoder_size
    audiodecoder_audiorenderer_filled:(int*)audiodecoder_audiorenderer_filled
      audiodecoder_audiorenderer_size:(int*)audiodecoder_audiorenderer_size;

@end














