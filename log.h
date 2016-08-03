#ifndef MID_LOG_INIT_TAIRAN_H
#define MID_LOG_INIT_TAIRAN_H

#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/ref.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/barrier.hpp>

#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/utility/setup/console.hpp>

namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

using boost::shared_ptr;

// Here we define our application severity levels.
enum severity_level
{
	D,	//DEBUG,
	I,	//INFO,
	W,	//WARNING,
	E,	//ERROR,
	C	//CRITICAL
};

// The formatting logic for the severity level
template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (
	std::basic_ostream< CharT, TraitsT >& strm, severity_level lvl)
{
	static const char* const str[] =
	{
		"D",
		"I",
		"W",
		"E",
		"C"
	};
	if (static_cast< std::size_t >(lvl) < (sizeof(str) / sizeof(*str)))
		strm << str[lvl];
	else
		strm << static_cast< int >(lvl);
	return strm;
}

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(g_lg, src::severity_logger_mt<severity_level>)

void InitLog();

void EndLog();

#define LOG(lvl) BOOST_LOG_SEV( g_lg::get(),lvl)

#endif
