
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <air/Air_c.h>

static int run = 1;

void* test_func(void* data)
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
}

int main(void)
{
    pthread_t thread;
    int thr_id;

    AIR_INITIALIZE(0);
    AIR_ACTIVATE();

    thr_id = pthread_create(&thread, NULL, test_func, NULL);

    sleep(100);
    run = 0;

    if (0 <= thr_id)
    {
        pthread_join(thread, NULL);
    }

    AIR_DEACTIVATE();
    AIR_FINALIZE();

    return 0;
}
