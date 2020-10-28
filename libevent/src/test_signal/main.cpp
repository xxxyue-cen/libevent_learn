
#include <iostream>
#include <event2/event.h>
#include <signal.h>

using namespace  std;
//sock 文件描述符
//which 事件类型
//arg传递的参数
static void ctrl_c(int sock,short which, void *arg)
{
    cout<<"ctrl_c"<<endl;
}

static void kill(int sock,short which, void *arg)
{
    cout<<"kill"<<endl;
    event *ev = (event * )arg;

    if(!evsignal_pending(ev,NULL)) //如果处于待决状态
    {
        event_del(ev);
        event_add(ev,NULL);
    }
}
int main() {
    event_base *base = event_base_new();

    //添加一个信号
    //ctrl + c
    //evsignal_new 的隐藏状态  EV_SIGNAL｜EV_PERSIST

    event *csig = evsignal_new(base, SIGINT, ctrl_c, base);
    if(!csig)
    {
        cerr<<"SIGINT evsiganl_new failed!"<<endl;
        return -1;
    }
    //添加事件到pending
    if(event_add(csig,0)!= 0 )
    {
        cerr<<"event_add ctrl_c failed!"<<endl;
        return -1;
    }

    //添加  kill信号
    //非持久事件  只进入一次
    //event_self_cbarg 传递当前的event
    event *ksig = event_new(base,SIGTERM,EV_SIGNAL,kill,event_self_cbarg());

    //添加事件到pending
    if(event_add(ksig,0)!= 0 )
    {
        cerr<<"event_add Kill  failed!"<<endl;
        return -1;
    }

    //进入事件循环
    event_base_dispatch(base);

    event_base_free(base);
    event_free(csig);

    return 0;
}
