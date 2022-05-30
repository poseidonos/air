
#include <air/Air_c.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int run = 1;

void*
test_func(void* data)
{
    int key = 0;

    while (run)
    {
        AIRLOG(PERF_BENCHMARK, AIR_READ, 0, 4096);
        AIRLOG(LAT_PROCESS, AIR_0, 0, key);
        usleep(100);
        AIRLOG(LAT_PROCESS, AIR_1, 0, key);
        key++;
        AIRLOG(Q_SUBMISSION, AIR_BASE, 2, 22);
        AIRLOG_DUMMY(Q_COMPLETION, AIR_BASE, 3, 33);
    }

    return NULL;
}

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
    printf("%s\n", argv[0]);
    printf("runtime:%d\n", runtime);

    pthread_t thread;
    int thr_id;

    AIR_INITIALIZE(0);
    AIR_ACTIVATE();

    thr_id = pthread_create(&thread, NULL, test_func, NULL);

    sleep(runtime);
    run = 0;

    if (0 <= thr_id)
    {
        pthread_join(thread, NULL);
    }

    AIR_DEACTIVATE();
    AIR_FINALIZE();

    return 0;
}
