#include "test/system_test/many_log/dummy_log.h"

#include <air/Air.h>
#include <pthread.h>
#include <unistd.h>

#include <iostream>

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
        airlog("LAT_IO_PATH", "AIR_0", 0, loop_counter);
        airlog("LAT_IO_PATH", "AIR_0", 1, loop_counter);
        airlog("LAT_IO_PATH", "AIR_0", 2, loop_counter);

        airlog("LAT_PROCESS", "AIR_0", 0, loop_counter);
        airlog("LAT_PROCESS", "AIR_0", 1, loop_counter);
        airlog("LAT_PROCESS", "AIR_0", 2, loop_counter);
        airlog("LAT_PROCESS", "AIR_0", 3, loop_counter);
        airlog("LAT_PROCESS", "AIR_0", 4, loop_counter);
        airlog("LAT_PROCESS", "AIR_0", 5, loop_counter);
        airlog("LAT_PROCESS", "AIR_0", 6, loop_counter);
        airlog("LAT_PROCESS", "AIR_0", 7, loop_counter);
        airlog("LAT_PROCESS", "AIR_0", 8, loop_counter);
        airlog("LAT_PROCESS", "AIR_0", 9, loop_counter);

        airlog("LAT_DUMMY_00", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_01", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_02", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_03", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_04", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_05", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_06", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_07", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_08", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_09", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_10", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_11", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_12", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_13", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_14", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_15", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_16", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_17", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_18", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_19", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_20", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_21", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_22", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_23", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_24", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_25", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_26", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_27", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_28", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_29", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_30", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_31", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_32", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_33", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_34", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_35", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_36", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_37", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_38", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_39", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_40", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_41", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_42", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_43", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_44", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_45", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_46", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_47", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_48", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_49", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_50", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_51", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_52", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_53", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_54", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_55", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_56", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_57", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_58", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_59", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_60", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_61", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_62", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_63", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_64", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_65", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_66", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_67", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_68", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_69", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_70", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_71", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_72", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_73", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_74", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_75", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_76", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_77", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_78", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_79", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_80", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_81", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_82", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_83", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_84", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_85", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_86", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_87", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_88", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_89", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_90", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_91", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_92", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_93", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_94", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_95", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_96", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_97", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_98", "AIR_0", 0, loop_counter);
        airlog("LAT_DUMMY_99", "AIR_0", 0, loop_counter);

        airlog("PERF_BENCHMARK", "AIR_WRITE", 0, 4096);
        airlog("PERF_BENCHMARK", "AIR_READ", 0x1234, 512);

        airlog("LAT_IO_PATH", "AIR_1", 0, loop_counter);
        airlog("LAT_IO_PATH", "AIR_1", 1, loop_counter);
        airlog("LAT_IO_PATH", "AIR_1", 2, loop_counter);

        airlog("LAT_PROCESS", "AIR_1", 0, loop_counter);
        airlog("LAT_PROCESS", "AIR_1", 1, loop_counter);
        airlog("LAT_PROCESS", "AIR_1", 2, loop_counter);
        airlog("LAT_PROCESS", "AIR_1", 3, loop_counter);
        airlog("LAT_PROCESS", "AIR_1", 4, loop_counter);
        airlog("LAT_PROCESS", "AIR_1", 5, loop_counter);
        airlog("LAT_PROCESS", "AIR_1", 6, loop_counter);
        airlog("LAT_PROCESS", "AIR_1", 7, loop_counter);
        airlog("LAT_PROCESS", "AIR_1", 8, loop_counter);
        airlog("LAT_PROCESS", "AIR_1", 9, loop_counter);

        airlog("LAT_DUMMY_00", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_01", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_02", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_03", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_04", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_05", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_06", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_07", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_08", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_09", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_10", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_11", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_12", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_13", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_14", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_15", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_16", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_17", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_18", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_19", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_20", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_21", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_22", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_23", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_24", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_25", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_26", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_27", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_28", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_29", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_30", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_31", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_32", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_33", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_34", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_35", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_36", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_37", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_38", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_39", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_40", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_41", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_42", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_43", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_44", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_45", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_46", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_47", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_48", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_49", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_50", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_51", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_52", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_53", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_54", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_55", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_56", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_57", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_58", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_59", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_60", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_61", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_62", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_63", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_64", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_65", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_66", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_67", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_68", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_69", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_70", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_71", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_72", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_73", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_74", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_75", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_76", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_77", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_78", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_79", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_80", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_81", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_82", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_83", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_84", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_85", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_86", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_87", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_88", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_89", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_90", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_91", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_92", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_93", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_94", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_95", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_96", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_97", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_98", "AIR_1", 0, loop_counter);
        airlog("LAT_DUMMY_99", "AIR_1", 0, loop_counter);

        airlog("PERF_BENCHMARK", "AIR_WRITE", 0, 1024);
        airlog("PERF_BENCHMARK", "AIR_READ", 0x1234, 512);

        airlog("LAT_IO_PATH", "AIR_2", 0, loop_counter);
        airlog("LAT_IO_PATH", "AIR_2", 1, loop_counter);
        airlog("LAT_IO_PATH", "AIR_2", 2, loop_counter);

        airlog("LAT_PROCESS", "AIR_2", 0, loop_counter);
        airlog("LAT_PROCESS", "AIR_2", 1, loop_counter);
        airlog("LAT_PROCESS", "AIR_2", 2, loop_counter);
        airlog("LAT_PROCESS", "AIR_2", 3, loop_counter);
        airlog("LAT_PROCESS", "AIR_2", 4, loop_counter);
        airlog("LAT_PROCESS", "AIR_2", 5, loop_counter);
        airlog("LAT_PROCESS", "AIR_2", 6, loop_counter);
        airlog("LAT_PROCESS", "AIR_2", 7, loop_counter);
        airlog("LAT_PROCESS", "AIR_2", 8, loop_counter);
        airlog("LAT_PROCESS", "AIR_2", 9, loop_counter);

        airlog("LAT_DUMMY_00", "AIR_2", 0, loop_counter);
        airlog("LAT_DUMMY_01", "AIR_2", 0, loop_counter);
        airlog("LAT_DUMMY_02", "AIR_2", 0, loop_counter);
        airlog("LAT_DUMMY_03", "AIR_2", 0, loop_counter);
        airlog("LAT_DUMMY_04", "AIR_2", 0, loop_counter);
        airlog("LAT_DUMMY_05", "AIR_2", 0, loop_counter);
        airlog("LAT_DUMMY_06", "AIR_2", 0, loop_counter);
        airlog("LAT_DUMMY_07", "AIR_2", 0, loop_counter);
        airlog("LAT_DUMMY_08", "AIR_2", 0, loop_counter);
        airlog("LAT_DUMMY_09", "AIR_2", 0, loop_counter);
        airlog("LAT_DUMMY_10", "AIR_2", 0, loop_counter);
        airlog("LAT_DUMMY_11", "AIR_2", 0, loop_counter);
        airlog("LAT_DUMMY_12", "AIR_2", 0, loop_counter);
        airlog("LAT_DUMMY_13", "AIR_2", 0, loop_counter);
        airlog("LAT_DUMMY_14", "AIR_2", 0, loop_counter);
        airlog("LAT_DUMMY_15", "AIR_2", 0, loop_counter);
        airlog("LAT_DUMMY_16", "AIR_2", 0, loop_counter);
        airlog("LAT_DUMMY_17", "AIR_2", 0, loop_counter);
        airlog("LAT_DUMMY_18", "AIR_2", 0, loop_counter);
        airlog("LAT_DUMMY_19", "AIR_2", 0, loop_counter);

        airlog("PERF_BENCHMARK", "AIR_WRITE", 0, 512);
        airlog("PERF_BENCHMARK", "AIR_READ", 0x1234, 512);

        airlog("LAT_IO_PATH", "AIR_3", 0, loop_counter);
        airlog("LAT_IO_PATH", "AIR_3", 1, loop_counter);
        airlog("LAT_IO_PATH", "AIR_3", 2, loop_counter);

        airlog("LAT_PROCESS", "AIR_3", 0, loop_counter);
        airlog("LAT_PROCESS", "AIR_3", 1, loop_counter);
        airlog("LAT_PROCESS", "AIR_3", 2, loop_counter);
        airlog("LAT_PROCESS", "AIR_3", 3, loop_counter);
        airlog("LAT_PROCESS", "AIR_3", 4, loop_counter);
        airlog("LAT_PROCESS", "AIR_3", 5, loop_counter);
        airlog("LAT_PROCESS", "AIR_3", 6, loop_counter);
        airlog("LAT_PROCESS", "AIR_3", 7, loop_counter);
        airlog("LAT_PROCESS", "AIR_3", 8, loop_counter);
        airlog("LAT_PROCESS", "AIR_3", 9, loop_counter);

        airlog("LAT_DUMMY_00", "AIR_3", 0, loop_counter);
        airlog("LAT_DUMMY_01", "AIR_3", 0, loop_counter);
        airlog("LAT_DUMMY_02", "AIR_3", 0, loop_counter);
        airlog("LAT_DUMMY_03", "AIR_3", 0, loop_counter);
        airlog("LAT_DUMMY_04", "AIR_3", 0, loop_counter);
        airlog("LAT_DUMMY_05", "AIR_3", 0, loop_counter);
        airlog("LAT_DUMMY_06", "AIR_3", 0, loop_counter);
        airlog("LAT_DUMMY_07", "AIR_3", 0, loop_counter);
        airlog("LAT_DUMMY_08", "AIR_3", 0, loop_counter);
        airlog("LAT_DUMMY_09", "AIR_3", 0, loop_counter);
        airlog("LAT_DUMMY_10", "AIR_3", 0, loop_counter);
        airlog("LAT_DUMMY_11", "AIR_3", 0, loop_counter);
        airlog("LAT_DUMMY_12", "AIR_3", 0, loop_counter);
        airlog("LAT_DUMMY_13", "AIR_3", 0, loop_counter);
        airlog("LAT_DUMMY_14", "AIR_3", 0, loop_counter);
        airlog("LAT_DUMMY_15", "AIR_3", 0, loop_counter);
        airlog("LAT_DUMMY_16", "AIR_3", 0, loop_counter);
        airlog("LAT_DUMMY_17", "AIR_3", 0, loop_counter);
        airlog("LAT_DUMMY_18", "AIR_3", 0, loop_counter);
        airlog("LAT_DUMMY_19", "AIR_3", 0, loop_counter);

        loop_counter++;
    }
    std::cout << "TestLog Loop end...\n";
}
