## YoHub
A lightweight event-driven tcp server via C++.

## Design
YoHub select event-driven programming model and use threads to handle high concurrency scenario. The core architecture is implemented with reactors in threads design. We are trying to limit the usage of the lock primitive, so below two points are guaranteed:
- messages/requests are always sequentially.
- working flow of a client is always in a immutable thread (one loop).
