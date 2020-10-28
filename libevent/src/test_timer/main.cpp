#include <iostream>
#include <event.h>

#include <thread>
using  namespace std;
static  timeval t1 = {1,0};

void  timer1(int sock,short which ,void *arg)
{
    cout <<"[timer1]"<<flush;
    // no pending
    event *ev = (event *)arg;
    if(!evtimer_pending(ev,&t1))
    {
        evtimer_del(ev);
        evtimer_add(ev,&t1);
    }

}

void timer2(int sock,short which,void *arg)
{
    cout<<"[timer2]"<<endl;
    this_thread::sleep_for(3000ms);
}

void timer3(int sock,short which,void *arg)
{
    cout<<"[timer3]"<<endl;
}



int main() {
    
    event_base *base = event_base_new();

    cout <<"test_timer"<<endl;
    //非持久事件，只进入一次
    event *env =  evtimer_new(base,timer1,event_self_cbarg());
    if(!env)
    {
        cout <<"evtimer_new timer failed"<<endl;
        return -1;
    }

    evtimer_add(env,&t1);

    static timeval t2;
    t2.tv_sec = 1;
    t2.tv_usec = 200000;
    event *ev2 = event_new(base,-1,EV_PERSIST,timer2,0);
    event_add(ev2,&t2);

    event *ev3 = event_new(base,-1,EV_PERSIST,timer3,0);
    static timeval tv_in = {3,0};
    const timeval *t3;
    t3 = event_base_init_common_timeout(base,&tv_in);
    event_add(ev3,t3);

    event_base_dispatch(base);
    event_base_free(base);
    return 0;
}
