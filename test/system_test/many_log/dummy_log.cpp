#include "test/system_test/many_log/dummy_log.h"

#include <pthread.h>
#include <unistd.h>

#include <iostream>

#include "src/api/Air.h"

bool DummyLog::run = true;

void
DummyLog::Run(void)
{
    run = true;
}

void
DummyLog::Stop(void)
{
    run = false;
}

void
DummyLog::TestLog(void)
{
    cpu_set_t cpu {0};
    CPU_SET(2, &cpu);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu);
    pthread_setname_np(pthread_self(), "TestLog");
    uint64_t loop_counter {0};

    std::cout << "TestLog Loop Start...\n";
    while (run)
    {
        airlog("LAT_IO_PATH", "range_0", 0, loop_counter);
        airlog("LAT_IO_PATH", "range_0", 1, loop_counter);
        airlog("LAT_IO_PATH", "range_0", 2, loop_counter);

        airlog("LAT_PROCESS", "range_0", 0, loop_counter);
        airlog("LAT_PROCESS", "range_0", 1, loop_counter);
        airlog("LAT_PROCESS", "range_0", 2, loop_counter);
        airlog("LAT_PROCESS", "range_0", 3, loop_counter);
        airlog("LAT_PROCESS", "range_0", 4, loop_counter);
        airlog("LAT_PROCESS", "range_0", 5, loop_counter);
        airlog("LAT_PROCESS", "range_0", 6, loop_counter);
        airlog("LAT_PROCESS", "range_0", 7, loop_counter);
        airlog("LAT_PROCESS", "range_0", 8, loop_counter);
        airlog("LAT_PROCESS", "range_0", 9, loop_counter);

        airlog("LAT_DUMMY_00", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_01", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_02", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_03", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_04", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_05", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_06", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_07", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_08", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_09", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_10", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_11", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_12", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_13", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_14", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_15", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_16", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_17", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_18", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_19", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_20", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_21", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_22", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_23", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_24", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_25", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_26", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_27", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_28", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_29", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_30", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_31", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_32", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_33", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_34", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_35", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_36", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_37", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_38", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_39", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_40", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_41", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_42", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_43", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_44", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_45", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_46", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_47", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_48", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_49", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_50", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_51", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_52", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_53", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_54", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_55", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_56", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_57", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_58", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_59", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_60", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_61", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_62", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_63", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_64", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_65", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_66", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_67", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_68", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_69", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_70", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_71", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_72", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_73", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_74", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_75", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_76", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_77", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_78", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_79", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_80", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_81", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_82", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_83", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_84", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_85", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_86", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_87", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_88", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_89", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_90", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_91", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_92", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_93", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_94", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_95", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_96", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_97", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_98", "range_0", 0, loop_counter);
        airlog("LAT_DUMMY_99", "range_0", 0, loop_counter);

        airlog("PERF_BENCHMARK", "write", 0, 4096);
        airlog("PERF_BENCHMARK", "read", 0x1234, 512);

        airlog("LAT_IO_PATH", "range_1", 0, loop_counter);
        airlog("LAT_IO_PATH", "range_1", 1, loop_counter);
        airlog("LAT_IO_PATH", "range_1", 2, loop_counter);

        airlog("LAT_PROCESS", "range_1", 0, loop_counter);
        airlog("LAT_PROCESS", "range_1", 1, loop_counter);
        airlog("LAT_PROCESS", "range_1", 2, loop_counter);
        airlog("LAT_PROCESS", "range_1", 3, loop_counter);
        airlog("LAT_PROCESS", "range_1", 4, loop_counter);
        airlog("LAT_PROCESS", "range_1", 5, loop_counter);
        airlog("LAT_PROCESS", "range_1", 6, loop_counter);
        airlog("LAT_PROCESS", "range_1", 7, loop_counter);
        airlog("LAT_PROCESS", "range_1", 8, loop_counter);
        airlog("LAT_PROCESS", "range_1", 9, loop_counter);

        airlog("LAT_DUMMY_00", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_01", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_02", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_03", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_04", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_05", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_06", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_07", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_08", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_09", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_10", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_11", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_12", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_13", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_14", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_15", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_16", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_17", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_18", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_19", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_20", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_21", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_22", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_23", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_24", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_25", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_26", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_27", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_28", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_29", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_30", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_31", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_32", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_33", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_34", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_35", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_36", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_37", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_38", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_39", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_40", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_41", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_42", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_43", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_44", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_45", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_46", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_47", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_48", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_49", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_50", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_51", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_52", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_53", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_54", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_55", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_56", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_57", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_58", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_59", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_60", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_61", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_62", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_63", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_64", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_65", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_66", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_67", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_68", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_69", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_70", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_71", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_72", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_73", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_74", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_75", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_76", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_77", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_78", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_79", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_80", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_81", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_82", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_83", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_84", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_85", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_86", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_87", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_88", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_89", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_90", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_91", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_92", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_93", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_94", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_95", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_96", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_97", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_98", "range_1", 0, loop_counter);
        airlog("LAT_DUMMY_99", "range_1", 0, loop_counter);

        airlog("PERF_BENCHMARK", "write", 0, 1024);
        airlog("PERF_BENCHMARK", "read", 0x1234, 512);

        airlog("LAT_IO_PATH", "range_2", 0, loop_counter);
        airlog("LAT_IO_PATH", "range_2", 1, loop_counter);
        airlog("LAT_IO_PATH", "range_2", 2, loop_counter);

        airlog("LAT_PROCESS", "range_2", 0, loop_counter);
        airlog("LAT_PROCESS", "range_2", 1, loop_counter);
        airlog("LAT_PROCESS", "range_2", 2, loop_counter);
        airlog("LAT_PROCESS", "range_2", 3, loop_counter);
        airlog("LAT_PROCESS", "range_2", 4, loop_counter);
        airlog("LAT_PROCESS", "range_2", 5, loop_counter);
        airlog("LAT_PROCESS", "range_2", 6, loop_counter);
        airlog("LAT_PROCESS", "range_2", 7, loop_counter);
        airlog("LAT_PROCESS", "range_2", 8, loop_counter);
        airlog("LAT_PROCESS", "range_2", 9, loop_counter);

        airlog("LAT_DUMMY_00", "range_2", 0, loop_counter);
        airlog("LAT_DUMMY_01", "range_2", 0, loop_counter);
        airlog("LAT_DUMMY_02", "range_2", 0, loop_counter);
        airlog("LAT_DUMMY_03", "range_2", 0, loop_counter);
        airlog("LAT_DUMMY_04", "range_2", 0, loop_counter);
        airlog("LAT_DUMMY_05", "range_2", 0, loop_counter);
        airlog("LAT_DUMMY_06", "range_2", 0, loop_counter);
        airlog("LAT_DUMMY_07", "range_2", 0, loop_counter);
        airlog("LAT_DUMMY_08", "range_2", 0, loop_counter);
        airlog("LAT_DUMMY_09", "range_2", 0, loop_counter);
        airlog("LAT_DUMMY_10", "range_2", 0, loop_counter);
        airlog("LAT_DUMMY_11", "range_2", 0, loop_counter);
        airlog("LAT_DUMMY_12", "range_2", 0, loop_counter);
        airlog("LAT_DUMMY_13", "range_2", 0, loop_counter);
        airlog("LAT_DUMMY_14", "range_2", 0, loop_counter);
        airlog("LAT_DUMMY_15", "range_2", 0, loop_counter);
        airlog("LAT_DUMMY_16", "range_2", 0, loop_counter);
        airlog("LAT_DUMMY_17", "range_2", 0, loop_counter);
        airlog("LAT_DUMMY_18", "range_2", 0, loop_counter);
        airlog("LAT_DUMMY_19", "range_2", 0, loop_counter);

        airlog("PERF_BENCHMARK", "write", 0, 512);
        airlog("PERF_BENCHMARK", "read", 0x1234, 512);

        airlog("LAT_IO_PATH", "range_3", 0, loop_counter);
        airlog("LAT_IO_PATH", "range_3", 1, loop_counter);
        airlog("LAT_IO_PATH", "range_3", 2, loop_counter);

        airlog("LAT_PROCESS", "range_3", 0, loop_counter);
        airlog("LAT_PROCESS", "range_3", 1, loop_counter);
        airlog("LAT_PROCESS", "range_3", 2, loop_counter);
        airlog("LAT_PROCESS", "range_3", 3, loop_counter);
        airlog("LAT_PROCESS", "range_3", 4, loop_counter);
        airlog("LAT_PROCESS", "range_3", 5, loop_counter);
        airlog("LAT_PROCESS", "range_3", 6, loop_counter);
        airlog("LAT_PROCESS", "range_3", 7, loop_counter);
        airlog("LAT_PROCESS", "range_3", 8, loop_counter);
        airlog("LAT_PROCESS", "range_3", 9, loop_counter);

        airlog("LAT_DUMMY_00", "range_3", 0, loop_counter);
        airlog("LAT_DUMMY_01", "range_3", 0, loop_counter);
        airlog("LAT_DUMMY_02", "range_3", 0, loop_counter);
        airlog("LAT_DUMMY_03", "range_3", 0, loop_counter);
        airlog("LAT_DUMMY_04", "range_3", 0, loop_counter);
        airlog("LAT_DUMMY_05", "range_3", 0, loop_counter);
        airlog("LAT_DUMMY_06", "range_3", 0, loop_counter);
        airlog("LAT_DUMMY_07", "range_3", 0, loop_counter);
        airlog("LAT_DUMMY_08", "range_3", 0, loop_counter);
        airlog("LAT_DUMMY_09", "range_3", 0, loop_counter);
        airlog("LAT_DUMMY_10", "range_3", 0, loop_counter);
        airlog("LAT_DUMMY_11", "range_3", 0, loop_counter);
        airlog("LAT_DUMMY_12", "range_3", 0, loop_counter);
        airlog("LAT_DUMMY_13", "range_3", 0, loop_counter);
        airlog("LAT_DUMMY_14", "range_3", 0, loop_counter);
        airlog("LAT_DUMMY_15", "range_3", 0, loop_counter);
        airlog("LAT_DUMMY_16", "range_3", 0, loop_counter);
        airlog("LAT_DUMMY_17", "range_3", 0, loop_counter);
        airlog("LAT_DUMMY_18", "range_3", 0, loop_counter);
        airlog("LAT_DUMMY_19", "range_3", 0, loop_counter);

        loop_counter++;
    }
    std::cout << "TestLog Loop end...\n";
}
