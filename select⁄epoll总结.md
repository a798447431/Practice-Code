## select()

### 函数简介

**select()**函数允许进程指示内核等待多个事件中的任何一个发生，并且只在有一个或多个事件发生或经历一段指定的时间后才唤醒它。简单来说就是select()允许一个程序监听多个文件描述符，等待一个或者多个文件描述符的I/O操作变成“就绪”状态(可读，可写，异常)。



### 函数原型

```c
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int select(int nfds, fd_set *readfds, fd_set *writefds,fd_set *exceptfds, struct timeval *timeout);

nfds: 监控的文件描述符集里最大文件描述符加1
readfds:监控有读数据到达文件描述符集合,传入传出参数
writefds:监控写数据到达文件描述符集合,传入传出参数
exceptfds:监控异常发生达文件描述符集合,如带外数据到达异常,传入传出参数
timeout:定时阻塞监控时间,3种情况 {
 1.NULL,永远等下去
 2.设置timeval,等待固定时间
 3.设置timeval里时间均为0,检查描述字后立即返回,轮询
}
struct timeval {
	long tv_sec; //秒
	long tv_usec; //微妙
};
void FD_CLR(int fd, fd_set *set); //把文件描述符集合里fd清0
int FD_ISSET(int fd, fd_set *set); //测试文件描述符集合里fd是否置1
void FD_SET(int fd, fd_set *set); //把文件描述符集合里fd位置1
void FD_ZERO(fd_set *set); //把文件描述符集合里所有位清0

返回值：
	成功返回就绪文件描述符的总数,超时时间内没有任何文件描述符就绪的话就返回0,失败返回-1
```



### 遇到的问题

1. 为什么select()函数中的第一个参数必须要设置为监控的文件描述符集合内的最大文件描述符+1？

   因为可以将被监控的文件描述符集合看做为一个数组，要想监控所有的文件描述符，所传入的第一个参数就要为该数组中最大的文件描述符值，由于数组下标从零开始，所以文件描述符值应该+1。

   

2. 为什么再次调用select()函数时需要用FD_SET宏清零和FD_SET再次添加fd？

   因为select 函数调用之后，readfds、writefds 和 exceptfds 这三个集合中存放的不是我们之前设置进去的 fd，而是有相关有读写或异常事件的 fd，也就是说 select 函数会修改这三个参数的内容，这也要求我们当一个 fd_set 被 select 函数调用后，这个 fd_set 就已经发生了改变，下次如果我们需要使用它，必须使用 FD_ZERO 宏先清零，再重新将我们关心的 fd 设置进去。

   

### select()函数的缺点

1. 每次调用select()函数时都需要遍历传递进来的所有文件描述符，系统开销在文件描述符很多耗费很大。

2. select()函数支持的文件描述符数量默认是1024，当文件描述符超出1024个时就会出错。

   

## pselect()

### 函数简介

**pselect()**函数和select()函数除了几种情况之外基本是一样的，也是等待一系列的文件描述符的状态发生变化的函数。



### 函数原型

```c
#include <sys/select.h>

int pselect(int nfds, fd_set *readfds, fd_set *writefds,fd_set *exceptfds, const struct timespec *timeout, const sigset_t *sigmask);hanshu

struct timespec {
	long tv_sec;  //秒
	long tv_nsec; //纳秒
};
事件chi
用sigmask替代当前进程的阻塞信号集,调用返回后还原原有阻塞信号集

返回值：
	成功返回就绪文件描述符的总数,超时时间内没有任何文件描述符就绪的话就返回0,失败返回-1
```



### 遇到的问题

1. pselect()函数最后一个参数的作用是什么？

   //我还没有理解信号集的作用，只知道最后一个参数值为NULL的情况下，pselect()函数与select()函数的作用是相同的。



### pselect()函数与select函数的不同处

1. 超时时间的结构体不同：select()函数是timeval结构，而pselect()函数是timespec结构，后者可以精确到纳秒。
2. pselect()函数有一个sigmask参数，如果该参数设置为NULL，则和select等价。
3. 函数返回时，select()函数可能更改timeout的值，但是pselect()函数不会，因为pselect的超时值被声明为const，这就保证调用pselect()函数时不会改变此值。所以对于select，我们应该假设timeval结构在select返回时未被定义，因而每次调用select之前都得对它进行初始化。



## poll()

### 函数简介

**poll()**函数与select()函数和pselect()函数作用相同，都是允许一个程序监听多个文件描述符，等待一个或者多个文件描述符的I/O操作变成“就绪”状态，但是poll()函数相对于select()函数来说有一些优化的地方。



### 函数原型

```c
#include <poll.h>

int poll(struct pollfd *fds, nfds_t nfds, int timeout);

struct pollfd {  //pollfd结构体
	int fd; //文件描述符 
	short events; //监控事件
	short revents; //返回的事件 
};

//对应select()函数中的fd_set *readfds
POLLIN 普通可读事件chi
POLLRDNORM 数据可读
POLLRDBAND 优先级带数据可读
POLLPRI 高优先级可读数据

//对应select()函数中的fd_set *writefds
POLLOUT 普通或可写
POLLWRNORM 数据可写
POLLWRBAND 优先级带数据可写

//对应select()函数中的fd_set *exceptfds
POLLERR 发生错误
POLLHUP 发生挂起
POLLNVAL 描述字不是一个打开的文件

nfds 监控数组中有多少文件描述符需要被监控

timeout //等待时间
= -1 阻塞
= 0  立即返回,不阻塞进程
> 0  等待指定毫秒数,如当前系统时间精度不够毫秒,向上取值

返回值：
	与select()函数相同，成功返回就绪文件描述符的总数,超时时间内没有任何文件描述符就绪的话就返回0,失败返回-1
```



### poll()函数与select函数的不同处

​	感觉与select()函数和pselect()函数没有很大不同，poll()函数没有了最大连接数的限制，将监控的读，写和异常三个文件描述符集合设置成宏，与所监控的文件描述符放入到一个结构体中，在使用方面与select()函数没有区别。



## epoll() --事件池

### 函数简介

**epoll()**函数是select(),peselect(),poll()三个函数的优化版，解决了这三个函数的一些问题。



### 函数原型

epoll()函数共分为三部分，有:

1.先创建一个epoll, 参数size用来告诉内核监听的文件描述符个数,最大的文件描述符个数可以在系统中修改

//最大的文件描述符个数具体与什么有关不太明白。

```c
int epoll_create(int size) //size为监听的最大文件描述符个数
//查了一下好像是创建了一个红黑树来对文件描述符进行插入删除和查找，但是我还没学红黑树。
```

2.控制某个epoll监控的文件描述符上的事件:注册、修改、删除。

```c
#include <sys/epoll.h>
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)

epfd 为epoll_creat的返回值 

op 表示对文件描述符的操作,用3个宏来表示:
EPOLL_CTL_ADD (注册新的fd到epfd),
EPOLL_CTL_MOD (修改已经添加的fd的监听事件),
EPOLL_CTL_DEL (从epfd删除一个fd);

event 告诉内核需要监听的事件

struct epoll_event {
	__uint32_t events; //事件，是以下几个宏之一或者集合，就是告诉系统我要监听什么事
	epoll_data_t data; //存放触发了时间的文件描述符的数据
};

typedef union epoll_data {
    void *ptr;
    int fd;
    __uint32_t u32; //没看懂
    __uint64_t u64; //没看懂
} epoll_data_t;

EPOLLIN  文件描述符可读
EPOLLOUT 文件描述符可写
EPOLLPRI //没看懂
EPOLLERR 文件描述符发生错误
EPOLLHUP 文件描述符被挂断;
EPOLLET  //将EPOLL设为边缘触发 
//水平模式（这是默认的模式，如果不使用EPOLLET 作为flag的话，就是这个模式），epoll只是一个更快的poll，并且能够在任何poll使用的地方使用，因为它们两者具有相同的语义。
EPOLLONESHOT //只监听一次事件,当监听完这次事件之后,如果还需要继续监听这个socket的话,需要再次把这个socket加入到EPOLL队列里
```

3.等待文件描述符状态变化
```c
#include <sys/epoll.h>
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)
events 事件的集合
maxevents events的大小 //这个maxevents的值不能大于创建epoll_create()时的size 不太理解
timeout 超时时间
= -1 阻塞
= 0  立即返回,非阻塞
> 0  微秒值
```

返回值:
	成功返回有多少文件描述符处于就绪态, 超时返回0, 出错返回-1



### epoll()函数与前三个函数相比进行的优化：

1. select()函数每次调用时都会将文件描述符的集合复制到内核中，开销比较大，而epoll()函数通过调用epoll_ctl()函数，当每次注册新的事件时会将全部文件描述符复制到内核中，在epoll_wait()中就不会重复复制。//没太明白用户态和内核态的意义

2. select()函数返回了就绪的文件描述符的个数，但是不知道是哪几个，所以函数会遍历一遍全部的文件描述符。而epoll()函数使用了一个队列存储了已经就绪的文件描述符，只需在epoll_wait()中直接遍历这个队列就能确认。//查了一下好像是采用了监听回调的机制，暂时还没有弄懂。

3. epoll()函数和poll()函数一样，监视的描述符数量没有限制。