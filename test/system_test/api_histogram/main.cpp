/*
 *   MIT License
 *
 *   Copyright (c) 2021 Samsung Electronics Corporation
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in all
 *   copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *   SOFTWARE.
 */

#include <unistd.h>

#include <iostream>
#include <thread>

#include "src/api/Air.h"
#include "test/system_test/api_histogram/histogram_log.h"

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

    HistogramLog histogram_log;
    histogram_log.Run();
    auto thr_histogram = std::thread(&HistogramLog::TestLog, histogram_log);

    sleep(runtime);

    histogram_log.Stop();
    thr_histogram.join();

    air_deactivate();
    air_finalize();

    std::cout << argv[0] << " end\n";
    return 0;
}
