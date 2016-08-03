//
// Created by codeh on 2016/3/25.
//

#ifndef MIDWARE2_PLUGINPROXY_H
#define MIDWARE2_PLUGINPROXY_H

#include "plugin_interface.h"
#include "dynamic_library.h"

#include <memory>
#include <string>
#include <c++/mutex>

class plugin_proxy {
    typedef plugin_interface *CREATE_T(void);

    typedef void DESTROY_T(plugin_interface *);

    static dynamic_library::symbol_type _create;
    static dynamic_library::symbol_type _destroy;
    static std::mutex _mutex_load;

public:
    const std::string &get_lib() const {
        return _lib;
    }

private:
    std::string _lib;

    struct deleter {    // a verbose array deleter:
        void operator()(plugin_interface *p) {
            ((DESTROY_T *) _destroy)(p);
        }
    };

//    auto deleter = [](plugin_interface*p){
//        ((DESTROY_T*)_destroy)(p);
//    };

public:
    plugin_proxy();

    virtual  ~plugin_proxy();

    PLUGIN_PTR load(const std::string &lib);
};

#endif //MIDWARE2_PLUGINPROXY_H
