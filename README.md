# Analytics In Real-time

Analytics In Real-time (AIR) is a light-weight system profiling tool that provides a set of APIs for profiling performance, latency, histogram, and so on.

# Getting Started with an Application

For now, AIR supports Linux system only.
This is an AIR tutorial to profile an application. Here is 5 Steps to use AIR:

1. Update AIR configuration
2. Build AIR with CMake
3. Insert AIR APIs to an application
4. Build an application with AIR
5. Visualize AIR data

For more details, see [AIR User Guide](docs/user_guide.md) documentation.

## 1. Update configuration

First of all, user needs to define a configuration to use AIR APIs. This configuration includes AIR system wide rules and node specific information.
This configuration will be interpreted at compile-time. Therefore AIR could validate syntax and optimize profiling code at compile-time.

```
[DEFAULT]
    "AirBuild: True, StreamingInterval: 3, NodeBuild: True, NodeRun: On, NodeSamplingRatio: 1000, NodeIndexSize: 10"
[/DEFAULT]

[GROUP]
    "Group:SUBMIT,    NodeIndexSize: 5"
    "Group:COMMON"
[/GROUP]

[FILTER]
    "Filter: AIR_IOtype,    Item: (AIR_READ, AIR_WRITE)"
    "Filter: AIR_Thread,    Item: (AIR_SUBMIT, AIR_PROCESS, AIR_COMPLETE)"
    "Filter: AIR_Range,     Item: (AIR_0 ... AIR_3)"
[/FILTER]

[BUCKET]
    "Bucket: BUCKET_1, Bounds: [0, 100), Scale: 10"
    "Bucket: BUCKET_4, Bounds: [2^0, 2^10), Scale: 2^"
[/BUCKET]

[NODE]
    "Node: PERF_BENCHMARK,  Filter: AIR_IOtype,  Type: Performance,  Group: COMMON"
    "Node: LAT_PROCESS,     Filter: AIR_Range,   Type: Latency,      Group: COMMON"
    "Node: LAT_IO_PATH,     Filter: AIR_Range,   Type: Latency,      Group: COMMON,     IndexSize: 3"
    "Node: UTIL_SUBMIT_THR, Filter: AIR_Thread,  Type: Utilization,  Group: SUBMIT,     Build: False"
    "Node: CNT_TEST_EVENT,  Filter: AIR_Thread,  Type: Count,        Group: SUBMIT,     IndexSize: 10"
    "Node: HIST_SAMPLE_1,   Filter: AIR_Range,   Type: Histogram,    Group: COMMON,     IndexSize: 10,   Bucket: BUCKET_1"
    "Node: HIST_SAMPLE_4,   Filter: AIR_Range,   Type: Histogram,    Group: COMMON,     IndexSize: 10,   Bucket: BUCKET_4"
[/NODE]
```

For more details, see [Test Config](config/system_test.cfg) file.

## 2. Build AIR with CMake

When the configuration file is updated, user has to build AIR to adjust the changes.

### 2.1. Standalone CMake
```
git clone https://github.com/poseidonos/air.git
cd air
mkdir build
cd build
cmake ..
```

If you want to build with your own config file, you should replace the last command with

```
cmake .. -DCONFIG=/your_file_directory/filename
```

After cmake, you can see a Makefile in the build directory. Type 'make' to build AIR.
And then you can install AIR in your system if you are a system administrator.

```
make
sudo make install
```

### 2.2. ExternalProject CMake

If you want to build AIR with other libraries, you can use ExternalProject_Add option.
```
ExternalProject_Add(air
#---Download step-----------------
    GIT_REPOSITORY ${REPO_AIR}
    GIT_TAG ${TAG_AIR}
#---Update/Patch step-------------
    UPDATE_COMMAND mkdir -p ${DIR_AIR}/build
#---Configure step----------------
    SOURCE_DIR ${DIR_AIR}
    CONFIGURE_COMMAND cmake .. -DCONFIG=${CFG_AIR}
#---Build step--------------------
    BINARY_DIR ${DIR_AIR}/build
    BUILD_COMMAND make
#---Install step------------------
    INSTALL_COMMAND make install
#---Test step---------------------
#---Output logging----------------
    LOG_DOWNLOAD ON
    LOG_UPDATE ON
    LOG_PATCH ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    LOG_INSTALL ON
    LOG_OUTPUT_ON_FAILURE ON
    LOG_MERGED_STDOUTERR ON
)
```

## 3. Insert AIR APIs to an application

AIR has a number of APIs which user insert by oneself. If user wants to profile some point of code, user needs to insert a proper API. For more details, see [AIR User Guide](docs/user_guide.md) documentation.

### 3.1. Operation

AIR has a polling thread which deals with raw data processing, air_cli response, JSON format output streaming.
To run this thread, user should insert some APIs below.

```
...
#include <air/Air.h>
...
int main(int argc, char *argv[])
{
	...
    air_initialize(profiling_core);  // AIR running(pinning) core, default: 0
    air_activate();

    // application code
    ...

    air_deactivate();
    air_finalize();
    
    return retVal;
}
...
```

### 3.2. Logging

When AIR collects data, it can classify thread ID where the data came from. User doesn't need to concern about thread ID. AIR automatically handles it.

#### 3.2.1 Use Case 1) PoseidonOS volume performance profiling

```
...
#include <air/Air.h>
...
void
AIO::SubmitAsyncIO(pos_io& posIo)
{
    ...
    case UbioDir::Write:
    {
        airlog("PERF_VOLUME", "AIR_WRITE", posIo.volume_id, posIo.length);
        ...
    }
    case UbioDir::Read:
    {
        airlog("PERF_VOLUME", "AIR_READ", posIo.volume_id, posIo.length);
        ...
    }
    ...
}
```

The above airlog(...) codes are C++ style APIs for performance profiling. Through this APIs, AIR can get the IOPS, Bandwidth per volume & per thread.

#### 3.2.2 Use Case 2) SPDK bdev latency profiling

```
...
#include <air/Air_c.h>
...
static int _bdev_pos_eventq_rw(struct spdk_io_channel *ch, struct spdk_bdev_io *bdev_io){
    ...
    case SPDK_BDEV_IO_TYPE_READ:
        AIRLOG(LAT_BDEV_READ, BDEV_IO_BEGIN, vol_id, (uint64_t)bdev_io);
        ...
    case SPDK_BDEV_IO_TYPE_WRITE:
        AIRLOG(LAT_BDEV_WRITE, BDEV_IO_BEGIN, vol_id, (uint64_t)bdev_io);
        ...
}
...
static void bdev_pos_io_complete(struct pos_io* io, int status)
{
    ...
    if (READ == io->ioType) {
        AIRLOG(LAT_BDEV_READ, BDEV_IO_END, io->volume_id, (uint64_t)io->context);
    }
    else if (WRITE == io->ioType) {
        AIRLOG(LAT_BDEV_WRITE, BDEV_IO_END, io->volume_id, (uint64_t)io->context);
    }
    ...
}
```

The above AIRLOG(...) codes are C style APIs for latency profiling. Through this APIs, AIR can get some statistics such as min/avg/max value of bdev layer's IO latency.

## 4. Build an application with AIR

After AIR build, user can link libair.a library to their application

### 4.1. CMake
```
target_link_libraries(${target_application} air ...)
```

### 4.2. Makefile
```
LDFLAGS += -L/usr/local/lib -lair
```

## 5. Visualize AIR data

There is a way to visualize AIR data.

### 5.1 TUI

TUI is a primitive and direct way. User can see the result on the machine where the application run. TUI displays the latest time result. To run TUI, just execute `air_tui` on the terminal.

```
TUI mode: [online], status: [play], page: [6/6],  interval: [3],  timestamp: 2022-3-4:11:12:2, pid: 27019
key {↑/↓}: move, {→/←}: (un)fold, {b}: perv page, {n}: next page, {space}: play/pause
    {1~9}: streaming interval, {i}: init, {o}: run, {x}: stop, {q(esc)}: quit
(*)   Top
( )   +Group:COMMON
( )[O]++Node:LAT_IO_PATH("latency")
      ""(0), index:0x0, filter:"AIR_0~AIR_1" Period(avg:262ns, median:126ns, max:5.7us, sample:99     ), Cumulation(avg:507ns, median:188ns, max:95us , sample:499    )
      ""(0), index:0x0, filter:"AIR_1~AIR_2" Period(avg:224us, median:222us, max:233us, sample:54     ), Cumulation(avg:224us, median:222us, max:233us, sample:54     )
      ""(0), index:0x1, filter:"AIR_0~AIR_1" Period(avg:224ns, median:140ns, max:1.1us, sample:94     ), Cumulation(avg:259ns, median:151ns, max:6.5us, sample:479    )
( )[O]++Node:LAT_PROCESS("latency")
      ""(0), index:0x200, filter:"AIR_0~AIR_1" Period(avg:497ns, median:361ns, max:1.6us, sample:99     ), Cumulation(avg:685ns, median:426ns, max:9.7us, sample:497    )
      ""(0), index:0x201, filter:"AIR_0~AIR_1" Period(avg:587ns, median:436ns, max:4.1us, sample:100    ), Cumulation(avg:780ns, median:533ns, max:9.8us, sample:498    )
( )[O]++Node:PERF_BENCHMARK("performance")
        SUM Period(iops:2.1M   , bw:8.8GB   ), Cumulation(iops:2.1M   , bw:8.7GB   )
      "CompleteIO"(27027), index:0x200, filter:"AIR_READ" Period(iops:1.1M   , bw:4.4GB   , count:[{"4096": 3609331}]), Cumulation(iops:1.1M   , bw:4.4GB   )
      "CompleteIO"(27027), index:0x201, filter:"AIR_READ" Period(iops:1.1M   , bw:4.4GB   , count:[{"4096": 3624199}]), Cumulation(iops:1.1M   , bw:4.4GB   )
( )   +Group:COMPLETE
( )[O]++Node:LAT_COMPLETE("latency")
      ""(0), index:0x0, filter:"AIR_0~AIR_1" Period(avg:92ns , median:67ns , max:512ns, sample:100    ), Cumulation(avg:152ns, median:99ns , max:5.8us, sample:500    )
( )[.]..Node:Q_COMPLETION
( )   +Group:SUBMIT
( )[O]++Node:CNT_TEST_EVENT("count")
      "CompleteIO"(27027), index:0x0, filter:"AIR_COMPLETE" Period(count:3.6M   ), Cumulation(count:14.4M  )
      "CompleteIO"(27027), index:0x1, filter:"AIR_COMPLETE" Period(count:-3.6M  ), Cumulation(count:-14.3M )
( )[O]++Node:LAT_SUBMIT("latency")
      ""(0), index:0x0, filter:"AIR_0~AIR_1" Period(avg:236ns, median:136ns, max:940ns, sample:100    ), Cumulation(avg:331ns, median:195ns, max:7.1us, sample:482    )
( )[O]++Node:Q_SUBMISSION("queue")
      "ProcessIO"(27025), index:0x0, filter:"AIR_BASE" Period(avg:251.4  , max:256.0  ), Cumulation(avg:250.8  , max:256.0  )
      "ProcessIO"(27026), index:0x1, filter:"AIR_BASE" Period(avg:251.9  , max:256.0  ), Cumulation(avg:251.5  , max:256.0  )
```

In TUI, user can control AIR CLI in real-time.
