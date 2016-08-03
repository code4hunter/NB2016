#ifndef _IPLUGIN_H_
#define _IPLUGIN_H_

#include <map>
#include <memory>
#include <string>
#include "market_data.h"


typedef struct{
	std::string key;
	std::string value;
	std::string description;
} PARAM;

typedef std::map<std::string, PARAM> PARAMS;

class ICallback
{
public:
	virtual ~ICallback()
	{
	}

	virtual void on_raw_message(const std::string &sender, const std::string &receiver, const std::string &msg) = 0;
};

typedef std::shared_ptr<ICallback> CALLBACK_PTR;


//class plugin_interface
class plugin_interface
{
public:
	const std::string &get_name() const {
		return _name;
	}

	void set_name(const std::string &_name) {
		plugin_interface::_name = _name;
	}

private:
	std::string _name;
public:
	virtual ~plugin_interface()
	{
	}

	virtual void start(const PARAMS &params, CALLBACK_PTR cb) = 0;
	
	virtual void stop(void) = 0;

	virtual void post_raw_message(const std::string &sender, const std::string &receiver, const std::string &msg) = 0;

	virtual void on_tick(const PTR_MARKET_DATA &md) = 0;


};

typedef std::shared_ptr<plugin_interface> PLUGIN_PTR;
typedef std::map<std::string, PLUGIN_PTR> PLUGINS;

#endif

