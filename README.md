YoHub  [![Total views](https://sourcegraph.com/api/repos/github.com/kedebug/yohub/counters/views.png)](https://sourcegraph.com/github.com/kedebug/yohub)
======
*A lightweight, asynchronous event-driven network application framework in C++.*

### Features
* _Reactors with worker pool_. YoHub supports multiple reactors, each reactor will deliver its active events to the worker pool for execution. Reactor and worker pool are both configurable that allow you to build your robust network application.
* _Asynchronous model_. The asynchronous programming model is inspired from [kylin](http://dirlt.com/kylin.html), yohub only notifies the client after the completion of I/O operations. Since the event loop is separated from the worker pool, edge-triggered readiness notification model is chosen for yohub. Nevertheless, edge-triggered readiness notification makes real-time response be possible and makes event handling be more efficient when cooperated with worker pool.
* _Thread safe_. Synchronous id (`sync-id` for short) is introduced to yuhub. Each `sockfd` is associated with such a `sync-id`, and each `sync-id` is binded to a consistent thread. In this way, yohub is able to reduce the usage of synchronization primitives (ie. `mutex lock`), thus has improved the throughput.
* _Smart pointer for connections_. For most scenarios, yohub uses `shared_ptr` to automatic memory management, which is inspired from [muduo](http://code.google.com/p/muduo/). YoHub takes advantage of smart pointers which can help us take care of the life cycle of those asychronous connections, and has greatly reduced our programming burden.

### Performance
YoHub is designed to be a high-performance network application framework, which can be used to solve the C100K problem. The benchmark pragram is token from the [libevent-1.4.14b](http://libevent.org/) distribution, modified to compatible with the event pool in yohub. Both libevent and yohub are configured to use the epoll interface and run on a Core4 Quad CPU at 2.5GHz with Ubuntu 12.04 operating system.

* __100 active clients.__

![100](https://raw.githubusercontent.com/kedebug/yohub/master/bench/100.png)

* __1000 active clients.__

![1000](https://raw.githubusercontent.com/kedebug/yohub/master/bench/1000.png)

The benchmark program takes the paramter `-n` equals to `-w`, which means every client will be notified to read and wirte. The report of our benchmark shows that yohub has lower costs than libevent-1.4.14b. With the increment of the file descriptor, this advantage can be obviously seen in above pictures.

### Usage
To use yohub, boost library is required to be installed in your machine. After that:
```bash
$ git clone git@github.com:kedebug/yohub.git
$ cd yohub
$ cmake . && make
```
Please see the files under [yohub/demo/](https://github.com/kedebug/yohub/tree/master/demo) directory for further usage.

### License
The MIT License (MIT)

Copyright (c) 2014 kedebug

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
