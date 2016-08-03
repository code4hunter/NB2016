#include "log.h"

void InitLog()
{
	// Create a text file sink
	typedef sinks::synchronous_sink< sinks::text_file_backend > file_sink;
	shared_ptr< file_sink > sink(new file_sink(
		keywords::file_name = "%Y%m%d_%5N.log",      // file name pattern
		keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
		keywords::rotation_size = 16384                     // rotation size, in characters
		));

	// Set up where the rotated files will be stored
	sink->locked_backend()->set_file_collector(sinks::file::make_collector(
		keywords::target = "logs",                          // where to store rotated files
		keywords::max_size = 16 * 1024 * 1024,              // maximum total size of the stored files, in bytes
		keywords::min_free_space = 100 * 1024 * 1024        // minimum free space on the drive, in bytes
		));

	// Upon restart, scan the target directory for files matching the file_name pattern
	sink->locked_backend()->scan_for_files();

	sink->set_formatter
		(
		expr::format("%4%[%2%][%3%]%1%:%5%")
		% expr::attr< unsigned int >("RecordID")
		% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
		% expr::attr< attrs::current_thread_id::value_type >("ThreadID")
		% expr::attr< severity_level >("Severity")
		% expr::smessage
		);

	sink->set_filter(expr::attr< severity_level >("Severity") >= I);

	// Add it to the core
	logging::core::get()->add_sink(sink);

	typedef sinks::synchronous_sink< sinks::text_ostream_backend > CONSOLE_SINK;
	shared_ptr< CONSOLE_SINK > console_sink = logging::add_console_log();
	console_sink->set_formatter(
		expr::format("%4%[%2%][%3%]%1%:%5%")
		% expr::attr< unsigned int >("RecordID")
		% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
		% expr::attr< attrs::current_thread_id::value_type >("ThreadID")
		% expr::attr< severity_level >("Severity")
		% expr::smessage
		);

	// Add some attributes too
	logging::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());
	logging::core::get()->add_global_attribute("RecordID", attrs::counter< unsigned int >());
	logging::core::get()->add_global_attribute("ThreadID", attrs::current_thread_id());
}

void EndLog() {
	logging::core::get()->set_logging_enabled(false);
}