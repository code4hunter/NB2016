//
// Created by codeh on 2016/7/1.
//

#ifndef MIDWARE2_PLUGIN_LOG_H
#define MIDWARE2_PLUGIN_LOG_H

#include "plugin_interface.h"

const char *_GETFILENAME_(const char *file);

//#define PLOG(type, fmt,...) plugin_log::Instance()->_ODS_(type, "[%s:%d]: "#fmt, __FILE__,__LINE__, ##__VA_ARGS__)
#define PLOG(type, fmt, ...) plugin_log::Instance()->_ODS_(type, fmt, ## __VA_ARGS__)

class plugin_log {
    CALLBACK_PTR _callback;
    std::string _sender;
    static std::shared_ptr<plugin_log> _instance;

    friend class std::shared_ptr<plugin_log>;

public:
    void _ODS_(char type, const char *fmt, ...);

    void init_log(const std::string &sender, CALLBACK_PTR &cb) {
        _sender = sender;
        _callback = cb;
    }

    static plugin_log *Instance(void) {
        return _instance.get();
    }
};

#endif //MIDWARE2_PLUGIN_LOG_H
