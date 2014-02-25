YoHub
======
*A lightweight, asynchronous event-driven network application framework in C++.*

### Features
* _Reactors with thread pool_. YoHub supports multiple reactors, each reactor will deliver its active events to the customized thread pool (CPU-threads) for execution. Reactor and CPU-threads are both configurable thus allow you to build your robust network application.
* _Asynchronous model_. The asynchronous programming model is mostly inspired from [kylin](http://dirlt.com/kylin.html), yohub notify the client only after the completion of I/O operations. Since the event loop is separated from the CPU-threads, edge-triggered readiness notification model is chosen for yohub.
* _Thread safe_. Synchronous id (sync-id) is introduced to yuhub. Each `sockfd` is associated with such a sync-id, and each sync-id is bind to a consistent thread. By this way, yohub can reduce the usage of synchronization primitives (ie. `mutex`) for connections and improve its throughput.
* _Smart pointer for connection_. For most scenarios, yohub use `shared_ptr` for automatic connection management. YoHub take advantage of smart pointers and only focus on its program logic withous much concern on the life cycle of those asynchronous connections.

### Performance
