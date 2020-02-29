// Why?
#define _WIN32_WINNT 0x0502
// The standard Windows includes.
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <shellapi.h>
#include <wchar.h>

#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <vector>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <stdio.h>

#include "PrusaSlicer_server_CLI.hpp"
#include "PrusaSlicer_server_server.hpp"

#include "log_catcher.hpp"

extern "C" {

int wmain(int argc, wchar_t **argv)
{

    std::vector<wchar_t*> argv_extended;
    argv_extended.emplace_back(argv[0]);

    for (int i = 1; i < argc; ++ i) {
        argv_extended.emplace_back(argv[i]);
    }
    argv_extended.emplace_back(nullptr);

    wchar_t path_to_exe[MAX_PATH + 1] = { 0 };
    ::GetModuleFileNameW(nullptr, path_to_exe, MAX_PATH);
    wchar_t drive[_MAX_DRIVE];
    wchar_t dir[_MAX_DIR];
    wchar_t fname[_MAX_FNAME];
    wchar_t ext[_MAX_EXT];
    _wsplitpath(path_to_exe, drive, dir, fname, ext);
    _wmakepath(path_to_exe, drive, dir, nullptr, nullptr);

	init_logging(sendToClient);
	//runServer(0);

    // argc minus the trailing nullptr of the argv
//    return slic3r_main((int)argv_extended.size() - 1, argv_extended.data());

    {
		int argc = (int)argv_extended.size() - 1;
		wchar_t ** argv = argv_extended.data();
        // Convert wchar_t arguments to UTF8.
        std::vector<std::string> 	argv_narrow;
        std::vector<char*>			argv_ptrs(argc + 1, nullptr);
        for (int i = 0; i < argc; ++ i)
            argv_narrow.emplace_back(boost::nowide::narrow(argv[i]));
        for (int i = 0; i < argc; ++ i)
            argv_ptrs[i] = argv_narrow[i].data();
        // Call the UTF8 main.
        return Slic3r::CLI().run(argc, argv_ptrs.data());
    }
}


}

