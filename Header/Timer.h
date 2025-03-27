#ifndef _TIMER_H_
#define _TIMER_H_

class Timer
{
public:
    Timer();
    ~Timer();

    void start();
    void stop();
    void paused();
    void unpaused();

    int get_ticks();

    bool IsStarted();
    bool IsPaused();
private:
    int start_tick;
    int pause_tick;

    bool is_paused;
    bool is_started;
};
#endif // _TIMER_H_
