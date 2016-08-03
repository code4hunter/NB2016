//
// Created by codeh on 2016/3/25.
//

#include "plugin_proxy.h"
#include "dynamic_library.h"
#include <c++/stdexcept>

dynamic_library::symbol_type plugin_proxy::_create = NULL;
dynamic_library::symbol_type plugin_proxy::_destroy = NULL;
std::mutex plugin_proxy::_mutex_load;

plugin_proxy::plugin_proxy() {
    _create = NULL;
    _destroy = NULL;
}

plugin_proxy::~plugin_proxy() {
}

PLUGIN_PTR  plugin_proxy::load(const std::string &lib) {
    std::unique_lock<std::mutex> lck(_mutex_load);
    if (_create == NULL || _destroy == NULL) {
        dynamic_library dl;
        _create = dl.loadEntryPoint(lib + ":create");
        _destroy = dl.loadEntryPoint(lib + ":destroy");
        if (_create == 0 || _destroy == 0) {
            throw std::runtime_error("Load Entry Point error:" + dl.getErrorMessage());
        }
    }
    PLUGIN_PTR ptr;
    if (_create && _destroy) {
        plugin_interface *plug = ((CREATE_T *) _create)();
        ptr.reset(plug, deleter());
    }
    return ptr;
}

