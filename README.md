*This project has been created as part of the 42 curriculum by mait-tal.*

## Description
This project is a small multithreaded simulation of several coders competing for a limited number of dongles while performing different tasks such as compilation, debugging, and refactoring. The simulation models how concurrent workers interact with shared resources and how the system reacts to overload, cooldown periods, and scheduling decisions.

The program supports two scheduling strategies: FIFO and EDF. A dedicated monitor thread observes the simulation state and stops it when burnout is detected, making it a practical example of concurrency control and thread coordination in C.

## Instructions
- Build the project with `make`
- Run the executable with:
  `./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler_type`
- The `scheduler_type` argument must be either `fifo` or `edf`
- Example:
  `./codexion 5 800 200 100 100 3 50 fifo`

### Parameter overview
- `number_of_coders`: how many coder threads are created
- `time_to_burnout`: the maximum allowed time before the monitor stops the simulation
- `time_to_compile`, `time_to_debug`, `time_to_refactor`: task durations for each coder action
- `number_of_compiles_required`: the target number of completed tasks before completion
- `dongle_cooldown`: the cooldown period applied to a dongle after it is used
- `scheduler_type`: selects how waiting coders are prioritized

## How it works
Each coder thread repeatedly tries to acquire the resources it needs to progress through its cycle of work. When a dongle is unavailable, the coder waits according to the active scheduler. The monitor periodically checks whether the simulation should stop due to burnout or other abnormal conditions. This creates a lightweight but realistic example of shared-resource synchronization in a concurrent program.

## Resources
- POSIX threads documentation (`pthread_mutex_t`, `pthread_cond_t`, `pthread_create`, `pthread_join`)
- Classic concurrency references on deadlock, starvation, and resource allocation
- Scheduler references for FIFO and EDF policies
- AI usage: AI was used to help draft and refine this README file, as well as to clarify some concurrency concepts during implementation.

## Blocking cases handled
- Deadlock prevention: the simulation uses consistent lock ordering when locking two dongles.
- Starvation prevention: waiting coders are managed through a configurable scheduler and a priority queue of waiters.
- Cooldown handling: dongles become temporarily unavailable after use and only become available again after their cooldown expires.
- Precise burnout detection: the monitor checks each coder's last activity time and stops the simulation if burnout is detected.
- Log serialization: shared output is protected so log messages from different threads do not interleave.

## Thread synchronization mechanisms
- Per-dongle `pthread_mutex_t` objects protect each dongle's state and its waiter queue.
- A global `sim_mutex` protects shared simulation state such as coder status, counters, and last activity timestamps.
- A `log_mutex` ensures console output is serialized across all threads.
- Threads are created with `pthread_create` and joined with `pthread_join`, allowing the monitor and coder threads to work together without race conditions.
