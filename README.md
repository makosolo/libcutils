# libcutils

### algorithm
* 提供各种排序算法：冒泡排序、堆排序、快速排序、插入排序
* 提供各种查找算法：线性遍历、二分法搜索
* 提供各种遍历、删除、统计算法

### charset
* 支持utf8、utf16、gbk、gb2312、uc2、uc4 之间的互相转码，并且支持大小端格式

### container
* 提供哈希、链表、数组、队列、堆栈

### crypto

### platform
* 提供file、socket、thread、time等常用系统接口
* 提供atomic、atomic64接口
* 提供高精度、低精度定时器
* 提供高性能的线程池操作
* 提供event、mutex、semaphore、spinlock等事件、互斥、信号量、自旋锁操作
* 提供获取函数堆栈信息的接口，方便调试和错误定位
* 提供跨平台动态库加载接口（如果系统支持的话）
* 提供io轮询器，针对epoll, poll, select, kqueue进行跨平台封装
* 提供跨平台上下文切换接口，主要用于协程实现，切换效率非常高

### regex
* 支持匹配和替换操作
* 支持全局、多行、大小写不敏感等模式
* 使用pcre, pcre2和posix正则库

### string
* 字符串操作

### utils
* 实现base64/32编解码
* 实现日志输出、断言等辅助调试工具
* 实现url编解码
* 实现位操作相关接口，支持各种数据格式的解析，可以对8bits、16bits、32bits、64bits、float、double以及任意bits的字段进行解析操作，并且同时支持大端、小端和本地端模式，并针对部分操作进行了优化，像static_stream、stream都有相关接口对其进行了封装，方便在流上进行快速数据解析。
* 实现swap16、swap32、swap64等位交换操作，并针对各个平台进行了优化。