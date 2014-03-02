YoHub
======
*A lightweight, asynchronous event-driven network application framework in C++.*

### Features
* _Reactors with worker pool_. YoHub supports multiple reactors, each reactor will deliver its active events to the worker pool for execution. Reactor and worker pool are both configurable that allow you to build your robust network application.
* _Asynchronous model_. The asynchronous programming model is inspired from [kylin](http://dirlt.com/kylin.html), yohub only notifies the client after the completion of I/O operations. Since the event loop is separated from the worker pool, edge-triggered readiness notification model is chosen for yohub. Nevertheless, edge-triggered readiness notification makes real-time response be possible and makes event handling be more efficient when cooperated with worker pool.
* _Thread safe_. Synchronous id (`sync-id` for short) is introduced to yuhub. Each `sockfd` is associated with such a `sync-id`, and each `sync-id` is binded to a consistent thread. In this way, yohub is able to reduce the usage of synchronization primitives (ie. `mutex lock`), thus has improved the throughput.
* _Smart pointer for connections_. For most scenarios, yohub uses `shared_ptr` to automatic memory management. YoHub takes advantage of smart pointers which can help us take care of the life cycle of those asychronous connections, and has greatly reduced our programming burden.

### Performance
