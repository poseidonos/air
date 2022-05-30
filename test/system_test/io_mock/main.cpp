
#include <air/Air.h>
#include <unistd.h>

#include <iostream>
#include <thread>

#include "dummy_io.h"

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

    DummyIO dummy_io;
    dummy_io.Run();

    int q0 = 0, q1 = 1;

    auto thr_print = std::thread(&DummyIO::Print, dummy_io);
    auto thr_submit = std::thread(&DummyIO::SubmitIO, dummy_io);
    auto thr_process1 = std::thread(&DummyIO::ProcessIO, dummy_io, q0);
    auto thr_process2 = std::thread(&DummyIO::ProcessIO, dummy_io, q1);
    auto thr_complete = std::thread(&DummyIO::CompleteIO, dummy_io);

    sleep(runtime);

    dummy_io.Stop();

    thr_print.join();
    thr_submit.join();
    thr_process1.join();
    thr_process2.join();
    thr_complete.join();

    air_deactivate();
    air_finalize();

    std::cout << argv[0] << " end\n";
    return 0;
}
