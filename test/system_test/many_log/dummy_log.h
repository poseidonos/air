
#ifndef DUMMY_LOG_H
#define DUMMY_LOG_H

class DummyLog
{
public:
    void Run(void);
    void Stop(void);
    void TestLog(void);
private:
    static bool run;
};

#endif // #define DUMMY_LOG_H
