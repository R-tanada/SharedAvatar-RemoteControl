//
// Created by Chamika Deshan on 05/19/21.
//

#ifndef MOBILE_APPLOG_H
#define MOBILE_APPLOG_H

#include <string>

namespace avatarmobile {
    // Notes : Cant' use ISingleton, loggers use before core init
    class AppLog {
        private:
            bool mLogEnabled;
        public:
            static AppLog& getInstance()
            {
                static AppLog INSTANCE;
                return INSTANCE;
            }

            void enable(bool enabled);

            void logInfo(const std::string &msg);

            void logWarn(const std::string &msg);

            void logError(const std::string &msg);
        private:
            AppLog(AppLog const&);
            AppLog(): mLogEnabled(true){};
            void operator=(AppLog const&);

        };

#define appLogger AppLog::getInstance()

}

#endif //ANDROID_APPLOG_H
