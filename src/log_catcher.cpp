#include <stdio.h>

#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes/current_process_name.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/basic_sink_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/value_ref.hpp>
#include <boost/log/utility/formatting_ostream.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
//[ example_extension_app_launcher_definition
// The backend starts an external application to display notifications
class log_catcher :
    public sinks::basic_formatted_sink_backend<
        char,                                                                   /*< target character type >*/
        sinks::synchronized_feeding                                             /*< in order not to spawn too many application instances we require records to be processed serial >*/
    >
{
public:
    // The function consumes the log records that come from the frontend
    void consume(logging::record_view const& rec, string_type const& string);
};

static std::function<void(const char *string)> log_callback = nullptr;

//]
//[ example_extension_app_launcher_consume
// The function consumes the log records that come from the frontend
void log_catcher::consume(logging::record_view const& rec, string_type const& string)
{
	if (log_callback != nullptr)
		log_callback(string.c_str());
	printf("%s\n", string.c_str());
}
//]

// The function initializes the logging library
void init_logging(std::function<void(const char *string)> callback)
{
    boost::shared_ptr< logging::core > core = logging::core::get();
    typedef sinks::synchronous_sink< log_catcher > sink_t;
    boost::shared_ptr< sink_t > sink(new sink_t());
	log_callback = callback;
    core->add_sink(sink);
}
//]

