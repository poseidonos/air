# AIR User Guide

## 1. Overview

### 1.1. Objectives

**The Analytics In Real-time (AIR)** provides a set of APIs for profiling performance, latency, histogram, and so on. It achieves run-time manipulation by designing all of the modules into event-driven and operating in a single dynamic chain, which optimizes collected data processing and minimizes profiling overhead. The AIR currently supports:

- Performance Profiling
- Latency Profiling
- Queue Profiling
- Count Profiling
- Utilization Profiling
- Histogram Profiling

### 1.2. Terminology & Acronyms

| Terms      | Terminology & Acronyms                    |
| :--------- | :---------------------------------------- |
| PoseidonOS | Samsung proprietary Storage Management OS |
| AIR        | System Profiler, Analytics In Real-time   |
| CLI        | Command Line Interface                    |
| TUI        | Text-based User Interface                 |
| GUI        | Graphical User Interface                  |



## 2. Build

### 2.1. AIR Static Library (Stand Alone)

#### 2.1.1. Configuration

Before build the AIR, configuration setting should be the first.

- **Set Configuration**

|         | Mandatory                                                    | Optaional                                            |
| :------ | :----------------------------------------------------------- | :--------------------------------------------------- |
| DEFAULT | StreamingInterval, AirBuild, NodeBuild, NodeRun, NodeSamplingRatio, NodeIndexSize | -                                                    |
| GROUP   | Group                                                        | NodeBuild, NodeRun, NodeSamplingRatio, NodeIndexSize |
| FILTER  | Filter, Item                                                 | -                                                    |
| BUCKET  | Bucket, Bounds, Scale                                        | -                                                    |
| NODE    | Node, Filter, Type, Group, Bucket(Histogram)                 | Build, Run, SamplingRatio, IndexSize                 |

```
R"AIR(

This configuration file is used as an AIR fundamental setting.
All of setting data would be interpreted at compile-time.
If this configuration syntax is invalid, it means build error may occur.

Paragraphs are organized within sqaure brackets at the beginning and end.
Sentences must be enclosed in double quotation marks.
In paragraph, several sentences could be exist. Here is an example.
[Paragraph1] "Sentence1" "Sentence2" "Sentence3" [/Paragraph2]
User should not change paragraphs.
User could change or add/remove sentences.


- DEFAULT paragraph contains default behavior.
  This paragraph must have only one sentence which has key:value pairs below.

  * AirBuild              {Mandatory key, Valid value: True/False}
  : False value means all AIR API do nothing and all the other options below is useless.
  So that target application might be built without AIR library at compile-time(depending on optimization).
  User can not control this option at run-time.

  * StreamingInterval     {Mandatory key, Valid value: 1~99 (seconds)}
  : AIR internally collects raw sampled data and calculates it to make statistics result.
  StreamingInterval key means periodic base time for calculating & saving the result.
  For example, when this value is set to 3, every 3 seconds, sampled data during 3 seconds will be calculated
  and the statistics result will be sent to the file(/tmp/air_yyyymmdd_pid.json).
  User can control this option by air_cli, air_tui at run-time.

  * NodeBuild             {Mandatory key, Valid value: True/False}
  : False value means airlog(...) API do nothing but other AIR business logic works.
  If a sentence has not this option in GROUP and NODE paragraph, that follows DEFAULT paragraph's rule.
  User can not control this option at run-time.

  * NodeRun               {Mandatory, Valid value: On/Off}
  : Off value means airlog(...) API stop logging. On value means airlog(...) API keep logging.
  If a sentence has not this option in GROUP and NODE paragraph, that follows DEFAULT paragraph's rule.
  User can control this option by air_cli, air_tui at run-time.

  * NodeSamplingRatio     {Mandatory, Valid value: 1~99999 (probability = 1/N)}
  : Queue type of NODE sentences may effect this sampling rule. For example, when this value is set to 1000,
  airlog(...) API actually collect raw data once a thousand.
  User can control this option by air_cli, air_tui at run-time.

  * NodeIndexSize         {Mandatory, Valid value: 1~999}
  : NodeIndex could be used to profiling numerical separation for the same NODE sentence.
  For example, when NodeIndexSize value is set to 10, user can distinguish 10 maximum different(hashed)
  statistics data for the same NODE sentence. The range of the NodeIndex is the same as the range of uint64_t.
  When AIR build, NodeIndexSize is one of the factor to decide memory pool size.
  User can not control this option at run-time.

[DEFAULT]
    "AirBuild: True, StreamingInterval: 3,
     NodeBuild: True, NodeRun: On, NodeSamplingRatio: 1000, NodeIndexSize: 10"
[/DEFAULT]


- GROUP paragraph defines group behavior.
  GROUP sentences may have node related behavior(NodeBuild, NodeRun, ...) such as DEFAULT sentence.
  However, it's not necessary but optional.
  If a GROUP sentence has a different value of NodeRun, group rule has more higher priority than default rule.
  So that, it follows GROUP sentence's NodeRun value. If not defined NodeRun, it follows default behavior.
  This paragraph could have several sentences which have key:value pairs below.

  * Group               {Mandatory, Valid value: unique string in GROUP paragraph}
  : Name of group, this value has to be unique in GROUP paragraph without space.
  User can not control this option at run-time.

  * NodeBuild           {Optional, Valid value: True/False}
  : Same as default rule

  * NodeRun             {Optional, Valid value: On/Off}
  : Same as default rule

  * NodeSamplingRatio   {Optional, Valid value: 1~99999 (probability = 1/N)}
  : Same as default rule

  * NodeIndexSize       {Optional, Valid value: 1~999}
  : Same as default rule

[GROUP]
    "Group:SUBMIT,    NodeIndexSize: 5"
    "Group:COMPLETE"
    "Group:COMMON"
    "Group:UNGROUPED, NodeBuild: True"
[/GROUP]


- FILTER paragraph defines semantic separation for the NODE sentence.
  This paragraph could have several sentences which have key:value pairs below.

  * Filter              {Mandatory, Valid value: unique enum name in target application}
  : Name of filter, this value is used as enum name in target application cause of C-style API extension.

  * Item                {Mandatory, Valid value: unique enumerator in target application}
  : This value is used as enumerator in target application. It's organized within round bracket.
  In round bracket, number of enumerators could be exist. Here is an example.
  (TYPE_A, TYPE_B, TYPE_C)
  Also, range expression with suffix(_#number) is possible. Here is an example.
  (TYPE_1 ... TYPE_30)

[FILTER]
    "Filter: AIR_Basic,     Item: (AIR_BASE)"
    "Filter: AIR_IOtype,    Item: (AIR_READ, AIR_WRITE)"
    "Filter: AIR_Thread,    Item: (AIR_SUBMIT, AIR_PROCESS, AIR_COMPLETE)"
    "Filter: AIR_Range,     Item: (AIR_0 ... AIR_3)"
[/FILTER]


- BUCKET paragraph defines histogram's bucket range, size and scale.
  Only histogram type NODE sentence can have and must have a Bucket option.
  Depending on Scale notation, each BUCKET sentence is distinguished a linear type or a exponential type.
  This paragraph could have several sentences which have key:value pairs below.

  * Bucket            {Mandatory, Valid value: unique string in BUCKET paragraph}
  : Name of bucket, this value has to be unique in BUCKET paragraph without space.
  User can not control this option at run-time.

  * Scale             {Mandatory, Valid value: unsigned integer or 2^~10^}
  : This value means range of each bucket.
  If this value includes circumflex symbol(^), it's exponential type. Or not, it's linear type.
  For example, "Bucket: BUCKET_1, Bounds: [0, 100), Scale: 10" is a linear type and bucket scope is 10.
  Bucket's ranges are 0 <= bucket[0] < 9, 10 <= bucket[1] < 20, ..., 90 <= bucket[9] < 100
  "Bucket: BUCKET_4, Bounds: [2^0, 2^10), Scale: 2^" is a exponential type and bucket scope is 2^.
  Bucket's ranges are 1 <= bucket[0] < 2, 2 <= bucket[1] < 4, ..., 512 <= bucket[9] < 1024
  "Bucket: BUCKET_6, Bounds: (-10^3, 10^5), Scale: 10^" is a exponential type and bucket scope is 10^.
  Bucket's ranges are -1000 < bucket[0] <= -100, -100 < bucket[1] <= -10, -10 < bucket[2] <= -1,
  0 <= bucket[3] <= 0, 1 <= bucket[4] < 10, ..., 10000 <= bucket[8] < 100000

  * Bounds            {Mandatory, Valid value: nations for intervals}
  : This value basically follows mathematics interval. (a, b] = {x -> integer(^exponent), a < x <= b}
  'a' is a lower boundary and 'b' is a upper boundary.
  In a linear type BUCKET sentence, notaion has to be '[ , )' so that it's easy to get bucket size.
  Bucket size is ((b - a) / scale). If bucket size exceed 20, syntax error will occur at compile-time.
  In a exponential type BUCKET sentence, if a > 0, notation has to be '[ , )',
  else if b < 0, notation has to be '( , ]', else notation has to be '( , )'.
  'a' and 'b' have to be defined scale_value^x.

[BUCKET]
    "Bucket: BUCKET_1, Bounds: [0, 100), Scale: 10"
    "Scale: 3, Bucket: BUCKET_2, Bounds: [33, 66)"
    "Bounds: [-100, 80), Scale: 20, Bucket: BUCKET_3"
    "Bucket: BUCKET_4, Bounds: [2^0, 2^10), Scale: 2^"
    "Bucket: BUCKET_5, Bounds: (-4^6, -4^2], Scale: 2^"
    "Bucket: BUCKET_6, Bounds: (-10^3, 10^5), Scale: 10^"
[/BUCKET]


- NODE paragraph defines NODE sentences that tracing point of code with specific data type.
  This paragraph could have several sentences which have key:value pairs below.

  * Node                {Mandatory, Valid value: unique enumerator in target application}
  : Name of node, this value is used as first parameter of airlog(...) API.
  For example, C++ style --> airlog("NodeA", ...), C style --> AIRLOG(NodeA, ...) 

  * Filter              {Mandatory, Valid value: Filter name}
  : Second parameter value of airlog(...) API has to be one of the Item from this Filter.

  * Type                {Mandatory, Valid value: Count/Histogram/Latency/Performance/Queue/Utilization}
  : Forth parameter value of airlog(...) API is calculated differently according to the type value.
  Count       --> +/- count
  Histogram   --> value within bucket bounds
  Latency     --> unique key for matching between start point and end point
  Performance --> io size & io type
  Queue       --> queue depth
  Utilization --> usage(tick, size, ...)

  * Group               {Mandatory, Valid value: Group name}
  : If a NODE sentence doesn't have optional key/value pairs below, those rules follow this group behavior.
  If a NODE sentence has a different value of Run, node rule has more higher priority than group rule.

  * Build               {Optional, Valid value: True/False}
  : Same as DEFAULT rule

  * Run                 {Optional, Valid value: On/Off}
  : Same as DEFAULT rule

  * SamplingRatio       {Optional, Valid value: 1~99999 (probability = 1/N)}
  : Same as DEFAULT rule

  * IndexSize           {Optional, Valid value: 1~999}
  : Same as DEFAULT rule

  * Bucket              {Mandatory(Histogram type only), Valid value: Bucket name}
  : Only histogram type NODE sentence can have Bucket option,
  otherwise syntax error will occur at compile-time.

[NODE]
    "Node: PERF_BENCHMARK,  Filter: AIR_IOtype,  Type: Performance,  Group: COMMON"
    "Node: LAT_SUBMIT,      Filter: AIR_Range,   Type: Latency,      Group: SUBMIT"
    "Node: LAT_PROCESS,     Filter: AIR_Range,   Type: Latency,      Group: COMMON"
    "Node: LAT_COMPLETE,    Filter: AIR_Range,   Type: Latency,      Group: COMPLETE"
    "Node: LAT_IO_PATH,     Filter: AIR_Range,   Type: Latency,      Group: COMMON,     IndexSize: 3"
    "Node: Q_SUBMISSION,    Filter: AIR_Basic,   Type: Queue,        Group: SUBMIT"
    "Node: Q_COMPLETION,    Filter: AIR_Basic,   Type: Queue,        Group: COMPLETE,   Build: False"
    "Node: UTIL_SUBMIT_THR, Filter: AIR_Thread,  Type: Utilization,  Group: SUBMIT"
    "Node: CNT_TEST_EVENT,  Filter: AIR_Thread,  Type: Count,        Group: SUBMIT,     IndexSize: 10"
    "Node: HIST_SAMPLE_1,   Filter: AIR_Range,   Type: Histogram,    Group: COMMON,     IndexSize: 10,   Bucket: BUCKET_1"
    "Node: HIST_SAMPLE_2,   Filter: AIR_Range,   Type: Histogram,    Group: COMMON,     IndexSize: 10,   Bucket: BUCKET_2"
    "Node: HIST_SAMPLE_3,   Filter: AIR_Range,   Type: Histogram,    Group: COMMON,     IndexSize: 10,   Bucket: BUCKET_3"
    "Node: HIST_SAMPLE_4,   Filter: AIR_Range,   Type: Histogram,    Group: COMMON,     IndexSize: 10,   Bucket: BUCKET_4"
    "Node: HIST_SAMPLE_5,   Filter: AIR_Range,   Type: Histogram,    Group: COMMON,     IndexSize: 10,   Bucket: BUCKET_5"
    "Node: HIST_SAMPLE_6,   Filter: AIR_Range,   Type: Histogram,    Group: COMMON,     IndexSize: 10,   Bucket: BUCKET_6"
[/NODE]

)AIR"
```

#### 2.1.2. Build

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

#### 2.1.3. Library

```
# After make
$ ./build/src/libair.a
$ ./build/tool/cli/air_cli
$ ./build/tool/tui/air_tui

# After make install
$ /usr/local/lib/libair.a
$ /usr/local/include/air/Air.h
$ /usr/local/include/air/Air_c.h
$ /usr/local/bin/air_cli
$ /usr/local/bin/air_tui
```

### 2.2. AIR with POS

The build sequence is as follows:

1. build AIR
2. build SPDK
3. build PoseidonOS

The build shell is in the file `"pos_root_dir/lib/build_lib.sh"`. After it succeed, you can build PoseidonOS as usual. The usecase AIR with PoseidonOS is handled in AIR Tutorial.


## 3. API

There are two types of APIs, the one is functions for preparing the profiling and the other is data collecting functions. To use the AIR APIs it is needed that including `"Air.h"` for C++ style or `"Air_c.h"` for C style.

### 3.1. Preparing Functions

| API                                  | Description                           |
| :----------------------------------- | :------------------------------------ |
| air_initialize(uint32_t cpu_num = 0) | Create & Initialize the AIR instances |
| air_activate()                       | The AIR starts profiling              |
| air_deactivate()                     | The AIR stops profiling               |
| air_finalize()                       | Destroy the AIR instances             |

**Example**

```
...
#include <air/Air.h>
...
int main(void)
{
    ...
    air_initialize(0);
    air_activate();
    
    /*  do work  */
    
    air_deactivate();
    air_finalize();
    
    return 0;
}
```

### 3.2. Data Collecting Functions

Each log point has target and logger type. To use AIR, you have to specify target and logger type in code directly. After then, you can insert user APIs in your codes to profile your application.

#### 3.2.1. Logger Type

**[Performance Logger]**
Performance logger measures IOPS and bandwidth.

- IOPS / Bandwidth
- Average IOPS / Average Bandwidth

**[Latency Logger]**
Latency logger measures for getting latency of specific sections between function calls.

- Mean latency
- Min latency
- Max latency
- Quartile latency
- Tail latencies
- Standard deviation

**[Queue Logger]**
Queue logger measures queue depth.

- Queue depth average
- Queue depth max

**[Utilization Logger]**
Utilization logger measures appending + number such as tick, size and so on.

- Usage

**[Count Logger]**
Count logger measures +/- count number.

- Count

**[Histogram Logger]**
Histogram logger measures bounds number.

- Count of each bucket
- Count of underflow
- Count of overflow
- Average value of total count

#### 3.2.2. API

The API logs the information at the current time. Add API where you want to measure the data at that time.

#### 3.2.2.1. Performance Log

Add performance log point in your application code area where I/O occurs.

**[Definition]**

```
airlog(string_literal node_name, string_literal filter_item, uint64_t index, uint64_t io_size)
```

**[Parameter]**

| Parameter   | Description                                                  |
| :---------- | :----------------------------------------------------------- |
| node_name   | String literal type value called node_name. It indicates the log point. This value is converted integer value at compile-time. |
| filter_item | String literal type value called node_name. It indicates the semantic separation. This value is converted integer value at compile-time. (e.g. AIR_WRITE, AIR_READ) |
| index       | Unsigned integer. user defined numerical separation. (e.g. volume_id) |
| io_size     | Unsigned Integer. It indicates I/O size of log point and its unit is byte. AIR calculates IOPS and bandwidth through io_size value. Also AIR can count 10 different size of I/Os. |

**[API Use Case]**

```
airlog("PERF_Volume", "AIR_WRITE" ubio->volume_id, ubio->size);
/* Done Write */
     …
airlog("PERF_Volume", "AIR_READ" ubio->volume_id, ubio->size);
/* Done Read */
```

**[TUI result]**

```
( )[O]++Node:PERF_Volume("performance")
        SUM_Period(iops:2.0m  , bw:8.1GB  )
      "Thread_01"(22239), index:3, filter:"AIR_READ" Period(iops:1.0m  , bw:4.2GB  , "4096(sz)-3081587(cnt)"), Total(iops_avg:943.0k, bw_avg:3.9GB  )
      "Thread_05"(22243), index:1, filter:"AIR_READ" Period(iops:958.7k, bw:3.9GB  , "4096(sz)-2876159(cnt)"), Total(iops_avg:901.3k, bw_avg:3.7GB  )
```

#### 3.2.2.2. Latency Log

This API gets latency information between logging points of the specific path. Insert the API on code where you measure the latency breakdown. Note that this API can be inserted inter-threads or inter-functions.

**[Definition]**

```
airlog(string_literal node_name, string_literal filter_item, uint64_t index, uint64_t key)
```

**[Parameter]**

| Parameter   | Description                                                  |
| :---------- | :----------------------------------------------------------- |
| node_name   | String literal type value called node_name. It indicates the log point. This value is converted integer value at compile-time. |
| filter_item | String literal type value called node_name. It indicates the semantic separation. This value is converted integer value at compile-time. (e.g. BDEV_SUBMIT, BDEV_COPY, BDEV_COMPLETE) |
| index       | Unsigned integer. user defined numerical separation. (e.g. volume_id) |
| key         | Unsigned integer. Unique key. This value is used to match between start time-stamp and end time-stamp. |

**[API Use Case]**

```
airlog("AsyncIO_Read", "BDEV_SUBMIT", 0, ubio->address);
…
If (READ)
{
airlog("AsyncIO_Read", "BDEV_COPY", 0, ubio->address); 
/* Do Read */
}
…
airlog("AsyncIO_Read", 0, "BDEV_COMPLETE", ubio->address);

/*
    latency break down
    read:  BDEV_SUBMIT -> BDEV_COPY -> BDEV_COMPLETE
*/
```

**[TUI result]**

```
( )[O]++Node:AsyncIO_Read("latency")
      ""(0), index:0, filter:"BDEV_SUBMIT~BDEV_COPY" Period(avg:308ns, median:158ns, max:1us  , sample:100.0 ), Total(avg:373ns, median:172ns, max:10us , sample:300.0 )
      ""(0), index:0, filter:"BDEV_COPY~BDEV_COMPLETE" Period(avg:6us  , median:6us  , max:14us , sample:100.0 ), Total(avg:110us, median:110us, max:259us, sample:180.0 )
```

#### 3.2.2.3. Queue Log

Add queue log point in your application code area where you measure queue.

**[Definition]**

```
airlog(string_literal node_name, string_literal filter_item, uint64_t index, uint64_t q_depth)
```

**[Parameter]**

| Parameter   | Description                                                  |
| :---------- | :----------------------------------------------------------- |
| node_name   | String literal type value called node_name. It indicates the log point. This value is converted integer value at compile-time. |
| filter_item | String literal type value called node_name. It indicates the semantic separation. This value is converted integer value at compile-time. (e.g. EventScheduler, IOScheduler) |
| index       | Unsigned integer. user defined numerical separation. (e.g. queue_id) |
| q_depth     | Unsigned integer. Depth of queue to be sampled.              |

**[API Use Case]**

```
airlog("Q_Scheduler", "IOScheduler", 3, queue.size());
/* Do Enqueue or Dequeue */
```

**[TUI result]**

```
( )[O]++Node:Q_Scheduler("queue")
      "IOScheduler"(3528), index:3, filter:"IOScheduler" Period(avg:209.7 , max:256.0 ), Total(avg:192.1 , max:256.0 )
```

#### 3.2.2.4. Utilization Log

Add utilization log point in your application code area where you measure usage.

**[Definition]**

```
airlog(string_literal node_name, string_literal filter_item, uint64_t index, uint64_t usage)
```

**[Parameter]**

| Parameter   | Description                                                  |
| :---------- | :----------------------------------------------------------- |
| node_name   | String literal type value called node_name. It indicates the log point. This value is converted integer value at compile-time. |
| filter_item | String literal type value called node_name. It indicates the semantic separation. This value is converted integer value at compile-time. (e.g. IDLE, BUSY) |
| index       | Unsigned integer. user defined numerical separation. (e.g. function_id) |
| usage       | Unsigned integer. Tick, size and so on                       |

**[API Use Case]**

```
airlog("UTIL_reactor", "BUSY", function_id, tick);
...
airlog("UTIL_reactor", "IDLE", function_id, tick);
```

**[TUI result]**

```
( )[O]++Node:UTIL_reactor("utilization")
      "Reactor_01"(22236), index:0, filter:"BUSY" Period(usage:33.9m , 20.47%), Total(usage:96.5m , 20.59%)
      "Reactor_01"(22236), index:1, filter:"BUSY" Period(usage:33.9m , 20.47%), Total(usage:96.5m , 20.59%)
      "Reactor_02"(22237), index:0, filter:"IDLE" Period(usage:30.9m , 18.69%), Total(usage:86.2m , 18.39%)
      "Reactor_02"(22238), index:1, filter:"BUSY" Period(usage:28.8m , 17.40%), Total(usage:82.7m , 17.67%)
      "Reactor_03"(22239), index:0, filter:"BUSY" Period(usage:9.2m  , 5.59%), Total(usage:25.5m , 5.44%)
      "Reactor_03"(22239), index:1, filter:"IDLE" Period(usage:28.8m , 17.38%), Total(usage:81.1m , 17.32%)
```

#### 3.2.2.5. Count Log

Add count in your application's logging code where you measure count.

**[Definition]**

```
airlog(string_literal node_name, string_literal filter_item, uint64_t index, int64_t count)
```

**[Parameter]**

| Parameter   | Description                                                  |
| :---------- | :----------------------------------------------------------- |
| node_name   | String literal type value called node_name. It indicates the log point. This value is converted integer value at compile-time. |
| filter_item | String literal type value called node_name. It indicates the semantic separation. This value is converted integer value at compile-time. |
| index       | Unsigned integer. user defined numerical separation.         |
| count       | Signed integer.                                              |

**[API Use Case]**

```
airlog("COUNT_FlowControl", "FreeBlock", 0, 1);
...
airlog("COUNT_FlowControl", "FreeBlock", 0, -1);
```

**[TUI result]**

```
( )[O]++Node:COUNT_FlowControl("count")
      "IOWorker"(7343), index:0, filter:"FreeBlock" Period(count:12 ), Total(count:30.3 )
      "IOWorker"(7347), index:0, filter:"FreeBlock" Period(count:3  ), Total(count:6.2  )
```

#### 3.2.2.6. Histogram Log

Add bucket count in your application's logging code where you measure number.

**[Definition]**

```
airlog(string_literal node_name, string_literal filter_item, uint64_t index, int64_t number)
```

**[Parameter]**

| Parameter   | Description                                                  |
| :---------- | :----------------------------------------------------------- |
| node_name   | String literal type value called node_name. It indicates the log point. This value is converted integer value at compile-time. |
| filter_item | String literal type value called node_name. It indicates the semantic separation. This value is converted integer value at compile-time. |
| index       | Unsigned integer. user defined numerical separation.         |
| number      | Signed integer. This value to be counted at specific bucket. |

**[API Use Case]**

```
airlog("HIST_SAMPLE_3", "AIR_0", 0, -100);
...
airlog("HIST_SAMPLE_6", "AIR_BASE", 1, 99999);
```

**[TUI result]**

```
( )[O]++Node:HIST_SAMPLE_3("histogram")
      "HistogramLog"(19471), index:0x0, filter:"AIR_0", bucket_type:"linear"
        Unit    value:(minimum/average/maximum) count:(underflow -100    -80     -60     -40     -20     0       20      40      60      80      overflow)
        Period         -101    -50     -20             18.9K      |18.9K  |18.9K  |18.9K  |18.9K  |18.9K  |0      |0      |0      |0      |      0
        Cumulation     -101    -50     -20             94.3K      |94.3K  |94.3K  |94.3K  |94.3K  |94.3K  |0      |0      |0      |0      |      0
      "HistogramLog"(19471), index:0x1, filter:"AIR_0", bucket_type:"linear"
        Unit    value:(minimum/average/maximum) count:(underflow -100    -80     -60     -40     -20     0       20      40      60      80      overflow)
        Period         0       40      80              0          |0      |0      |0      |0      |0      |18.9K  |18.9K  |18.9K  |18.9K  |      18.9K
        Cumulation     0       40      80              0          |0      |0      |0      |0      |0      |94.3K  |94.3K  |94.3K  |94.3K  |      94.3K
( )[O]++Node:HIST_SAMPLE_6("histogram")
      "HistogramLog"(19471), index:0x0, filter:"AIR_0", bucket_type:"exponential"
        Unit    value:(minimum/average/maximum) count:(underflow -1.0K   -100    -10     -1  0   1       10      100     1.0K    10.0K   100.0K  overflow)
        Period         -1.0K   6.7K    100.0K          18.9K      |18.9K  |18.9K  |18.9K  |18.9K  |18.9K  |18.9K  |18.9K  |18.9K  |18.9K  |      18.9K
        Cumulation     -1.0K   6.7K    100.0K          94.3K      |94.3K  |94.3K  |94.3K  |94.3K  |94.3K  |94.3K  |94.3K  |94.3K  |94.3K  |      94.3K
```

### 3.3. C Support

From AIR 0.2.0-alpha, AIR can profile SPDK source code. Since SPDK is based on C, the C wrapping APIs operating only in C based code are used. The usage is same as described above. The differences are the API function name(C++ style: small letter, C style: capital letter), 1st & 2nd parameters(C++ style: string literal, C style: enum). And these APIs are declared in header file `Air_c.h`.

| C Wrapping APIs                                              |
| :----------------------------------------------------------- |
| AIR_INITIALIZE(uint32_t cpu_num)                             |
| AIR_ACTIVATE()                                               |
| AIR_DEACTIVATE()                                             |
| AIR_FINALIZE()                                               |
| AIRLOG(uint32_t node_id, uint32_t filter_id, uint64_t index, uint64_t value) |



## 4. TUI

The AIR TUI(Text User Interface) is the visualization tool shows profiling results in text. The profiling results are shown according to the type of node in real-time. In every dump interval, profiling result is updated. The profiling result of each node is thread-aware, also each thread breaks down. Resource profiling results are shown bottom of the normal data and detailed data page. This keys are basic commands of TUI. The detailed description is below.

```
$ air_tui
```

| key    | Description                                                  |
| :----- | :----------------------------------------------------------- |
| 1-9    | air_cli --air-stream-interval                                |
| i      | air_cli --node-init (option value depends on cursor position) |
| o      | air_cli --node-run=true_x (option value depends on cursor position) |
| x      | air_cli --node-run=false_x (option value depends on cursor position) |
| q, esc | quit TUI                                                     |
| ↑,↓    | move cursor position                                         |
| ←,→    | Fold or spread group/node data                               |



## 5. CLI

The AIR command-line interface (CLI) is the software interface used to access your AIR profiler. The CLI provides a set of commands that you can use to monitor its operations and adjust the configuration as needed in run-time.

```
$ air_cli --pid=<target_pid> [options] ...
```

### 5.1. Options

- ***--air-run=bool***

  Enable/Disable the air. Default is true.

  ex) ./air_cli --pid=<target_pid> --run=<true/false>



- ***--air-stream-interval=int***

  **`second`** Integer. Set interval second of dumping the logged data. Allow value range to be given, such as 1-30. Default is 3.



- ***--node-run=bool_str:[int],[int]***

  By configuration, air will run according to the each trace point's specific option. User can enable one to multiple trace point by matching following options.

  **`node:int`** trace enum id

  **`range:int,int`** trace enum id (first to last)

  **`group:int`** trace point's group id

  **`all`** all trace point



- ***--node-init=str:[int],[int]***

  By configuration, air will run according to the each trace point's specific option. User can initialize one to multiple trace point by matching following options.

  **`node:int`** trace enum id

  **`range:int,int`** trace enum id (first to last)

  **`group:int`** trace point's group id

  **`all`** all trace point

- ***--node-sample-ratio=int_str:[int],[int]***

  **`ratio`** Integer. The following integer is 1/probability of sampling. Allow value range to be given, such as 1-10,000. Default is 1000.

  By configuration, air will run according to the each trace point's specific option. User can set sampling ratio one to multiple trace point by matching following options.

  **`node:int`** trace enum id

  **`range:int,int`** trace enum id (first to last)

  **`group:int`** trace point's group id

  **`all`** all trace point

  ※ Only stochastic logger type tracer will be affected.
  