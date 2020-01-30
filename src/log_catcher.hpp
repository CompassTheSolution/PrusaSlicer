#ifndef LOG_CATCHER_HPP
#define LOG_CATCHER_HPP

void init_logging(std::function<void(const char *string)> logger);

#endif