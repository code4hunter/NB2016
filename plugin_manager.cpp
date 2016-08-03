//
// Created by codeh on 15/12/22.
//

#include "plugin_manager.h"
#include "plugin_proxy.h"
#include "session_manager.h"
#include "messages/message_md_index.h"
#include "messages/message_md_stock.h"
#include "messages/message_md_future.h"
#include "messages/message_log.h"
#include <iostream>

std::shared_ptr<PluginManager> PluginManager::_instance(new PluginManager());

void PluginManager::on_message(const MESSAGE_PTR &msg) {
    session_manager::Instance()->dispatch_message(msg);
    dispatch_message(msg);
}

void PluginManager::on_messages(const LIST_MESSAGE_PTR &msgs) {
    session_manager::Instance()->dispatch_messages(msgs);
    for (LIST_MESSAGE_PTR::const_iterator it = msgs.begin(); it != msgs.end(); ++it) {
        dispatch_message(*it);
    }
    std::cout << "Dispatch: " << msgs.size() << std::endl;
}

PLUGIN_PTR PluginManager::create_plugin(const std::string &pathname) {
    std::unique_lock<std::mutex> lck(_plugin_mutex);
    plugin_proxy proxy;
    PLUGIN_PTR p = proxy.load(pathname);
    return p;
}

void PluginManager::start_plugin(PLUGIN_PTR &plugin, const PARAMS &config) {
    std::unique_lock<std::mutex> lck(_plugin_mutex);
    if (plugin) {
        CALLBACK_PTR cb(_instance);
        plugin->start(config, cb);
    }
}

void PluginManager::stop_plugin(PLUGIN_PTR &plugin) {
    std::unique_lock<std::mutex> lck(_plugin_mutex);
    if (plugin) {
        plugin->stop();
    }
}

void PluginManager::add_plugin(const std::string &name, PLUGIN_PTR &plugin) {
    std::unique_lock<std::mutex> lck(_plugin_mutex);
    plugin->set_name(name);
    if (_plugins.find(plugin->get_name()) == _plugins.end()) {
        _plugins[plugin->get_name()] = plugin;
    }
    else {
        throw std::runtime_error("Plugin Name exists:" + plugin->get_name());
    }
}

void PluginManager::remove_plugin(PLUGIN_PTR &plugin) {
    std::unique_lock<std::mutex> lck(_plugin_mutex);
    plugin->stop();
    _plugins.erase(plugin->get_name());
}

void PluginManager::remove_all(void) {
    std::unique_lock<std::mutex> lck(_plugin_mutex);
    for (PLUGINS::iterator it = _plugins.begin(); it != _plugins.end(); ++it) {
        it->second->stop();
    }
    _plugins.clear();
}

void PluginManager::post_message(const MESSAGE_PTR &msg) {
    std::unique_lock<std::mutex> lck(_plugin_mutex);
    const std::string &receiver = msg->get_receiver();
    PLUGINS::iterator it = _plugins.find(receiver);
    if (it != _plugins.end()) {
        it->second->post_message(msg);
    }
}

void PluginManager::on_raw_message(const std::string &sender, const std::string &receiver, const std::string &msg) {
    std::cout << "->PluginManager From:" << sender << " To:" << receiver << " Text:" << msg << std::endl;
}

void PluginManager::post_raw_message(const std::string &sender, const std::string &receiver, const std::string &msg) {
    std::unique_lock<std::mutex> lck(_plugin_mutex);
    PLUGINS::iterator it = _plugins.find(receiver);
    if (it != _plugins.end()) {
        it->second->post_raw_message(sender, receiver, msg);
    }
}

PluginManager::~PluginManager() {
    std::cout << "~PluginManager" << std::endl;
}

void PluginManager::stop_plugin(const std::string &name) {
    std::unique_lock<std::mutex> lck(_plugin_mutex);
    PLUGINS::iterator it = _plugins.find(name);
    if (it != _plugins.end()) {
        it->second->stop();
    }
}

void PluginManager::remove_plugin(const std::string &name) {
    std::unique_lock<std::mutex> lck(_plugin_mutex);
    PLUGINS::iterator it = _plugins.find(name);
    if (it != _plugins.end()) {
        it->second->stop();
        _plugins.erase(it);
    }
}

void PluginManager::stop_all(void) {
    std::unique_lock<std::mutex> lck(_plugin_mutex);
    for (PLUGINS::iterator it = _plugins.begin(); it != _plugins.end(); ++it) {
        it->second->stop();
    }
}

void PluginManager::dispatch_message(const MESSAGE_PTR &msg) {
    unsigned int type = msg->type();
    std::string key;
    if (type == message_md_index_snapshot::MSG_MD_INDEX) {
        const message_md_index_snapshot *pmd = static_cast<message_md_index_snapshot *>(msg.get());
        key = pmd->get_key_id();
    }
    else if (type == message_md_stock_snapshot::MSG_MD_STOCK) {
        const message_md_stock_snapshot *pmd = static_cast<message_md_stock_snapshot *>(msg.get());
        key = pmd->get_key_id();

    }
    else if (type == message_md_future_snapshot::MSG_MD_FUTURE) {
        const message_md_future_snapshot *pmd = static_cast<message_md_future_snapshot *>(msg.get());
        key = pmd->get_key_id();
    }
    else if (type == message_log::MSG_LOG) {
        const message_log *plog = static_cast<message_log *>(msg.get());
        switch (plog->get_ltype()) {
            case ltInfo:
                LOG(I) << plog->get_sender() << " " << plog->get_text();
                break;
            case ltWarning:
                LOG(W) << plog->get_sender() << " " << plog->get_text();
                break;
            case ltError:
                LOG(E) << plog->get_sender() << " " << plog->get_text();
                break;
            default:
                LOG(C) << plog->get_sender() << " " << plog->get_text();
                break;
        }
    }
    if (key.size() == 0) return;

    std::unique_lock<std::mutex> lck(_pool_mutex);

    FEED_POOL::iterator poolit = _feed_pool.find(msg->get_sender());
    if (poolit == _feed_pool.end()) {
        std::pair<std::map<std::string, POOL>::iterator, bool> ret;
        ret = _feed_pool.insert(std::pair<std::string, POOL>(msg->get_sender(), POOL()));
        poolit = ret.first;
    }
    std::pair<std::map<std::string, MESSAGE_PTR>::iterator, bool> ret;
    ret = poolit->second.pool.insert(std::pair<std::string, MESSAGE_PTR>(key, msg));
    if (!ret.second) {
        ret.first->second = msg;
    }
}

void PluginManager::get_pool_messages(const std::string sender, LIST_MESSAGE_PTR &msgs) {
    std::unique_lock<std::mutex> lck(_pool_mutex);
    FEED_POOL::iterator poolit = _feed_pool.find(sender);
    if (poolit != _feed_pool.end()) {
        for (std::map<std::string, MESSAGE_PTR>::const_iterator it = poolit->second.pool.begin(); it !=
                                                                                                  poolit->second.pool.end(); ++it) {
            msgs.push_back(it->second);
        }
    }
    show_pool();
}

void PluginManager::show_pool(void) {
    for (FEED_POOL::const_iterator it = _feed_pool.begin(); it != _feed_pool.end(); ++it) {
        std::cout << "POOL: " << it->first << "," << it->second.pool.size() << std::endl;
    }
}