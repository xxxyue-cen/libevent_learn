#include <event2/event.h>
#include <iostream>
#include <event2/listener.h>
using namespace std;

#define  PORT  8000

 void listen_cb(struct evconnlistener *e, evutil_socket_t s, struct sockaddr *a, int socklen, void *arg)
 {
     cout <<"回调函数"<<endl;

 }

int main()
{
#ifdef _WIN32
    //初始化socket库
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2),&wsa);

#else
	 if(signal(SIGPIPE,SIG_IGN) == SIG_ERR)
     {
	     return 1;
     }
#endif
    std::cout << "test server!"<<endl;
    //创建libevent的上下文

    sockaddr_in sin;
    memset(&sin,0,sizeof(sin));

    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);

    event_base * base = event_base_new();
    if (base)
    {
        cout << "event_base_new success!" << endl;
    }
    //监听端口
    //socket  bind  listen

    evconnlistener *ev =  evconnlistener_new_bind(base, //libevent 上下文
                          listen_cb, //回调函数  调用接收的
                          base,  //回调函数获取的参数
                          LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, //地址重用  listen关闭同时清理socket
                          10,//链接队列大小  同listen参数
                          (sockaddr*)&sin,
                          sizeof(sin)
                          );

    //事件分发
    if(base)
        event_base_dispatch(base);

    if(base)
        evconnlistener_free(ev);

    if(base)
    event_base_free(base);

    return 0;
}
