#ifndef TEMPLATE_ASYNC_LISTERNER_H
#define TEMPLATE_ASYNC_LISTERNER_H

/*
 @author chamika
 Date : 2017/May
 */

#include <map>
#include <stdio.h>
#include <string>
#include <utils/ILogManager.h>

#include <boost/preprocessor/stringize.hpp>
//#include <boost/thread/thread.hpp>
//#include <boost/thread/xtime.hpp>
#include <condition_variable>
#include <thread>

#define FUNCTION_NAME(x) std::string(BOOST_PP_STRINGIZE(x))

namespace avatarmobile {
template <class T> class TemplateAsyncListerner {
    typedef void (T::*CallbackMethod)(std::string &tag);
    //-------------------------------------------------------------------
    
protected:
    std::string mTag;
    std::thread *thrd;
    bool mStart;
    bool mStop;
    
    int mWaitDuration;
    bool mWaked;
    
    CallbackMethod mMethod;
    CallbackMethod mTimeOutMethod;
    T *mInstance;
    
    std::mutex mMutex;
    std::condition_variable mWakeCondition;
    
    //-------------------------------------------------------------------
    
    void update() {
        std::unique_lock<std::mutex> lock(mMutex);
        while (!mStop) {
            // Wait for specific time
            if (mWaitDuration > 0) {
                /*boost::system_time const timeout =
                 boost::get_system_time() +
                 boost::posix_time::seconds(mWaitDuration);
                 */
                if (mWakeCondition.wait_for(lock,
                                            std::chrono::seconds(mWaitDuration),
                                            [this]{return mWaked;})) {
                    
                    if (mStart) {
                        (mInstance->*mMethod)(mTag);
                    }
                    if (mStop)
                        break;
                } else {
                    (mInstance->*mTimeOutMethod)(mTag);
                    mStop = true;
                }
            }
            // Just wait untill wake
            else {
                mWakeCondition.wait(lock);
                
                if (mStop)
                    break;
                if (mStart) {
                    (mInstance->*mMethod)(mTag);
                }
            }
        };
    }
    
public:
    //-------------------------------------------------------------------
    
    TemplateAsyncListerner(std::string &tag, T *pInstance, CallbackMethod pMethod)
    : mTag(tag), mInstance(pInstance), mMethod(pMethod), mWaitDuration(0) {
        this->mStop = false;
        this->mStart = false;
        this->mWaked = false;
        this->thrd = NULL;
    }
    
    //-------------------------------------------------------------------
    
    TemplateAsyncListerner(std::string &tag, T *pInstance, CallbackMethod pMethod,
                           CallbackMethod timeOutMethod, int const &waitDuration)
    : mTag(tag), mInstance(pInstance), mMethod(pMethod),
    mTimeOutMethod(timeOutMethod), mWaitDuration(waitDuration) {
        this->mStop = false;
        this->mStart = false;
        this->mWaked = false;
        this->thrd = NULL;
    }
    
    //-------------------------------------------------------------------
    
    ~TemplateAsyncListerner() {
        if (!(this->mStop)) {
            stop();
        }
        delete thrd;
    }
    
    //-------------------------------------------------------------------
    
    void wake(bool stop = false) {
        this->mStop = stop;
        this->mWaked = true;
        this->mWakeCondition.notify_one();
    }
    
    //-------------------------------------------------------------------
    
    void start() {
        this->thrd = new std::thread(&TemplateAsyncListerner::update, this);
        this->mStart = true;
    }
    
    //-------------------------------------------------------------------
    
    void stop() {
        this->mStop = true;
        this->mStart = false;
        this->mWaked = false; // Not waked
        this->mWakeCondition.notify_all();
        //this->thrd->interrupt();
        this->thrd->join();
    }
};
} // namespace avatarmobile

#endif
