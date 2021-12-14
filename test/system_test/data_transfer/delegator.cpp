#include "test/system_test/data_transfer/delegator.h"

#include <pthread.h>
#include <unistd.h>

#include <iostream>
#include <string>

int
ex1_function(const air::JSONdoc& data)
{
    usleep(100000); // 100ms
    std::cout << "call ex1_function\n";

    std::cout << "Nodes: ";
    for (auto& i : data)
    {
        std::cout << i.first << ", ";
    }
    std::cout << "\n";
    if (data.HasKey("PERF_BENCHMARK"))
    {
        std::cout << data["PERF_BENCHMARK"] << std::endl;
        auto& objs = data["PERF_BENCHMARK"]["objs"];
        for (auto& obj_it : objs)
        {
            std::cout << "{\n";
            auto& obj = objs[obj_it.first];
            for (auto& item : obj)
            {
                std::cout << "  " << item.first << ":" << obj[item.first] << std::endl;
            }
            std::cout << "}\n";
        }
    }
    if (data.HasKey("timestamp"))
    {
        std::cout << "timestamp:" << data["timestamp"] << std::endl;
    }
    if (data.HasKey("interval"))
    {
        std::cout << "interval:" << data["interval"] << std::endl;
    }

    return 0;
}

void
Delegator::CallByLambda(void)
{
    std::cout << call_count << "," << foo->d << "\n";
}

void
Delegator::Test(void)
{
    call_count = 10;

    air_request_data({"PERF_BENCHMARK", "LAT_IO_PATH"}, ex1_function);
    air_request_data({"PERF_BENCHMARK"},
        [this](const air::JSONdoc& data) -> int {
            usleep(200000); // 200ms
            std::cout << "call ex2_lambda_ref, call_count:";
            this->call_count++;
            this->CallByLambda();
            //std::cout << data << std::endl;
            if (0 > this->call_count || 20 < this->call_count)
            {
                std::cout << "invalid status" << std::endl;
                return -1;
            }
            return 0;
        });
    air_request_data({"LAT_IO_PATH"},
        [copy = *this](const air::JSONdoc& data) mutable -> int {
            usleep(300000); // 300ms
            std::cout << "call ex3_lambda_copy, call_count:";
            copy.call_count++;
            copy.CallByLambda();
            //std::cout << data << std::endl;
            return 0;
        });
}
