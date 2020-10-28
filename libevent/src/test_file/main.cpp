#include <iostream>
#include <event.h>
#include <fcntl.h>
#include <unistd.h>
#include <thread>
using namespace std;
void  read_file(evutil_socket_t fd,short event,void *arg)
{
    cout<<"."<<endl;

    char buffer[1024] = {0};
    int len = read(fd,buffer,sizeof(buffer)-1);
    if(len > 0 )
    {
        cout <<buffer<<endl;
    } else
    {
        cout <<"."<<endl;
        this_thread::sleep_for(3000ms);
    }

}


int main() {


    std::cout << "test file!" << std::endl;

    event_config *conf = event_config_new();
    //设置支持文件描述符
    event_config_require_features(conf,EV_FEATURE_FDS);

    event_base *base = event_base_new_with_config(conf);
    event_config_free(conf);

    if(!base)
    {
        cout <<"event_base_new_with_config  failed"<<endl;
        return  -1;
    }

    int sock = open("/var/log/wifi.log",O_RDONLY|O_NONBLOCK,0);
    if(sock <= 0)
    {
        perror("open");
        return -2;
    }

    lseek(sock,0,SEEK_END);


    event *fev = event_new(base,sock,EV_READ|EV_PERSIST,read_file,event_self_cbarg());

    event_add(fev,NULL);


    event_base_dispatch(base);
    event_base_free(base);

    return 0;
}
