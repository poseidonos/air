
#include <air/Air.h>
#include <unistd.h>

#include <iostream>
#include <thread>

#include "test/system_test/many_log/dummy_log.h"

int
main(int argc, char* argv[])
{
    char option;
    int runtime = 60;
    optind = 1;
    while (-1 != (option = getopt(argc, argv, "r:")))
    {
        switch (option)
        {
            case 'r':
                runtime = atoi(optarg);
                break;

            default:
                break;
        }
    }
    std::cout << argv[0] << " start\n";
    std::cout << "runtime:" << runtime << "\n";

    air_initialize(0);
    air_activate();

    DummyLog dummy_log;
    dummy_log.Run();
    auto thr_dummy = std::thread(&DummyLog::TestLog, dummy_log);

    sleep(runtime);

    dummy_log.Stop();
    thr_dummy.join();

    air_deactivate();
    air_finalize();

    std::cout << argv[0] << " end\n";
    return 0;
}
