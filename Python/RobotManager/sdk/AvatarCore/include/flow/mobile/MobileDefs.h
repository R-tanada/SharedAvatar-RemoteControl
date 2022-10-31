#ifndef DEFS_H
#define DEFS_H

#include <string>

enum RenderType {
    NONE,            // No render, only for send video
    GLSINK,          // GLsink Renderer : Use GLSinkView
    APPSINK_OPENGL,  // Appsink OpenGL Renderer : Use AppSinkGLView
    APPSINK_CG       // Appsink Apple Core Graphics Renderer

};

typedef struct {
    bool sendVideo;
    bool sendAudio;
    bool sendData;
    bool receiveAudio;
    bool receiveVideo;
    bool receiveData;
    RenderType receiveRenderType;
    RenderType receiveBottomRenderType;
    RenderType senderRenderType;
} Configuration;


typedef std::string PipelineTag;

namespace avatarmobile {
    typedef enum AVATAR_STATUS {
        Online,
        AvatarReady,
        Glassdoor,
        Timeout,
        Offline,
        AvataroutDone,
        CloudProxyDisabled
    } AvatarStatus;

    typedef enum AVATAROUT_REASON {
        Dealloc,
        FlutterCall,
        MemoryOut,
        Appbackground
    } AvatarOutReason;
}
#endif
