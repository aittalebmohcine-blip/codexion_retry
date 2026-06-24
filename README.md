*This project has been created as part of the 42 curriculum by <login1>[, <login2>[, <login3>[...]]].*

## Description
Simple simulation of multiple coders competing for limited dongles to perform compilations.
Supports two scheduler types (fifo, edf). A monitor thread detects burnout and stops the simulation.

## Instructions
- Build: `make`
- Run: `./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler_type`
- Example: `./codexion 5 800 200 100 100 3 50 fifo`
- `scheduler_type` must be `fifo` or `edf`.

## Resources
- POSIX threads documentation (pthread_mutex_t, pthread_cond_t, pthread_create/join)
- Dining philosophers / concurrent resource allocation references
- Scheduler references: FIFO and EDF
- AI usage: AI was used only to draft this README file.

## Blocking cases handled
- Deadlock prevention: consistent lock ordering when locking two dongles.
- Starvation prevention: configurable scheduler (FIFO or EDF) and a priority queue of waiters.
- Cooldown handling: dongles have a `next_available_time_ms` and become unavailable for the cooldown period after use.
- Precise burnout detection: monitor checks each coder's last compile time and stops the simulation if burnout occurs.
- Log serialization: all log output is protected to avoid interleaved prints.

## Thread synchronization mechanisms
- Per-dongle `pthread_mutex_t` to protect dongle state and waiter queue.
- Global `sim_mutex` to protect shared simulation state (coder states, done counters, last compile times).
- `log_mutex` to serialize console output from multiple threads.
- `stop_mutex` to protect the simulation stop flag; `wait_mutex` and `wait_cond` are initialized for coordinated waits.
- Threads are created with `pthread_create` and joined with `pthread_join` (monitor + coder threads).
