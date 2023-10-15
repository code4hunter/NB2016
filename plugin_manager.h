//
// Created by codeh on 15/12/22.
//

#ifndef MIDWARE2_PLUGINMANAGER_H
#define MIDWARE2_PLUGINMANAGER_H

#include "plugin_interface.h"
#include <memory>
#include <mutex>

class PluginManager : public ICallback {
    PLUGINS _plugins;
    std::mutex _plugin_mutex;
public:
    virtual ~PluginManager();

private:
    PluginManager() {
    }

public:
    PLUGIN_PTR create_plugin(const std::string &pathname);

    void start_plugin(PLUGIN_PTR &plugin, const PARAMS &config);

    void stop_plugin(PLUGIN_PTR &plugin);

    void stop_plugin(const std::string &name);

    void add_plugin(const std::string &name, PLUGIN_PTR &plugin);

    void remove_plugin(PLUGIN_PTR &plugin);

    void remove_plugin(const std::string &name);

    void stop_all(void);

    void remove_all(void);

    void post_message(const MESSAGE_PTR &msg);

    void post_raw_message(const std::string &sender, const std::string &receiver, const std::string &msg);

    virtual void on_message(const MESSAGE_PTR &msg);

    virtual void on_messages(const LIST_MESSAGE_PTR &msgs) override;

    virtual void on_raw_message(const std::string &sender, const std::string &receiver, const std::string &msg);

    static PluginManager *Instance(void) {
        return _instance.get();
    }

    void get_pool_messages(const std::string sender, LIST_MESSAGE_PTR &msgs);

private:

    static std::shared_ptr<PluginManager> _instance;

    friend class std::shared_ptr<PluginManager>;

    std::mutex _pool_mutex;
    struct POOL {
        std::map<std::string, MESSAGE_PTR> pool;
    };
    typedef std::map<std::string, POOL> FEED_POOL;
    FEED_POOL _feed_pool;

    void dispatch_message(const MESSAGE_PTR &msg);

    void show_pool(void);
};


#endif //MIDWARE2_PLUGINMANAGER_H
