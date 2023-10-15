//
// Created by codeh on 2016/7/1.
//
#include <stdio.h>      /* printf */
#include <stdarg.h>
#include <iostream>
#include <cassert>
#include "plugin_log.h"
#include "../messages/message_log.h"

std::shared_ptr<plugin_log> plugin_log::_instance(new plugin_log());

const char *_GETFILENAME_(const char *file) {
    assert(file != NULL);
    int len = strlen(file);
    for (int i = len - 1; i >= 0; i--) {
        if (file[i] == '\\' || file[i] == '/') {
            return file + i + 1;
        }
    }
    return file;
}

void plugin_log::_ODS_(char type, const char *fmt, ...) {
    va_list argptr;
    char text[255];
    va_start(argptr, fmt);
    vsnprintf(text, 254, fmt, argptr);
    text[254] = 0;
    va_end(argptr);
    if (_callback == NULL) {
        std::cout << "->" << type << " " << text << std::endl;
        return;
    }

    LOG_TYPE t;
    switch (type) {
        case 'E':
            t = ltError;
            break;
        case 'W':
            t = ltWarning;
            break;
        case 'I':
            t = ltInfo;
            break;
        default:
            t = ltUnknown;
    }
    MESSAGE_PTR pmsg(new message_log(t, _sender, text));
    _callback->on_message(pmsg);
}

