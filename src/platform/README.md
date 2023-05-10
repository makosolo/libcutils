提供file、directory、socket、thread、time等常用系统接口
提供atomic、atomic64接口
提供高精度、低精度定时器
提供高性能的线程池操作
提供event、mutex、semaphore、spinlock等事件、互斥、信号量、自旋锁操作
提供获取函数堆栈信息的接口，方便调试和错误定位
提供跨平台动态库加载接口（如果系统支持的话）
提供io轮询器，针对epoll, poll, select, kqueue进行跨平台封装
提供跨平台上下文切换接口，主要用于协程实现，切换效率非常高