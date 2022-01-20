
#include <air/Air.h>
#include <unistd.h>

#include <iostream>
#include <thread>

#include "test/system_test/data_transfer/delegator.h"
#include "test/system_test/data_transfer/dummy_log.h"

int
main(int argc, char* argv[])
{
    char option;
    int runtime = 10;
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

    Delegator* d = new Delegator {3};
    d->Test();

    sleep(runtime);

    delete d;
    d = nullptr;

    sleep(10);

    dummy_log.Stop();
    thr_dummy.join();

    air_deactivate();
    air_finalize();

    std::cout << argv[0] << " end\n";
    return 0;
}
