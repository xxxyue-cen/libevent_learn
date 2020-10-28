#include <iostream>
#include <event.h>

using namespace std;
#define  PORT  9000
void client_cb(evutil_socket_t s,short w,void *arg)
{
//判断超时
    event *ev = (event *)arg;
if(w &EV_TIMEOUT)
{
    cout <<"timeout"<<endl;

    cout <<"."<<endl;
    event_free(ev);
    evutil_closesocket(s);
    return ;
}
   char buf[1024] = {0};
   int len = recv(s,buf,sizeof(buf)-1,0);
   if(len > 0 )
   {
       cout <<buf<<endl;

       send(s,"Ok",2,0);

   } else
   {
       //清理event
       cout <<"."<<endl;
        event_free(ev);
        evutil_closesocket(s);
        return;
   }


}


void listen_cb(evutil_socket_t s,short  which,void *arg)
{
    
    cout<<"listen_cb"<<endl;
    sockaddr_in sin;
    socklen_t  size = sizeof(sin);
   evutil_socket_t client =   accept(s,(sockaddr *)&sin,&size);
   char ip[16] = {0};
   evutil_inet_ntop(AF_INET,&sin.sin_addr,ip,sizeof(ip)-1);
   cout<<"client ip is"<<ip<<endl;


   //客户端事件读取
    event_base *base = (event_base *)arg;
   //水平触发
    // event *ev = event_new(base,client,EV_READ|EV_PERSIST,client_cb,event_self_cbarg());
    //边缘触发
    event *ev = event_new(base,client,EV_READ|EV_PERSIST|EV_ET,client_cb,event_self_cbarg());

    timeval t = {10,0};
    event_add(ev,&t);

}


int main() {

    std::cout << "test, event  server !" << std::endl;

    event_base *base = event_base_new();

    evutil_socket_t sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock  <= 0 )
    {
        perror("sock");
        return  -1;
    }

    //设置地址复用与非阻塞
    evutil_make_listen_socket_reuseable(sock); //地址
    evutil_make_socket_nonblocking(sock); //非阻塞

    sockaddr_in sin;
    memset(&sin,0,sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);

    int re = ::bind(sock,(sockaddr *)&sin,sizeof(sin));
    if(re != 0)
    {
        perror("bind");
        return  -2;
    }

    listen(sock,10);

    event *ev =event_new(base,sock,EV_READ|EV_PERSIST,listen_cb,base);

    event_add(ev,0);

    event_base_dispatch(base);
    evutil_closesocket(sock);
    event_base_free(base);


    return 0;
}
