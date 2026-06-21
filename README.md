Working on:
  - organizing code.

To do:
  - heap size = 2
  - readme
	- init_dongles(): mutex initialization failure in the middle is not handled.
	- makefile does not rebuild when there is change in a header file.





-------------------------------------
significant changes to
- dongle:
    - update next_available_time_ms.
    - replace heap_remove_top -> heap_remove_request.
    - dongle cooldown filter -> can_take().


-------------------------------------
# codexion_retry

**Golden rule:** everything should be encapsulated. We will use t_simulation context struct passed to all threads.

codexion/
├── Makefile                 # Compilation rules (all, clean, fclean, re, $(NAME))
├── README.md                # Mandatory documentation (with specific required sections)
├── includes/
│   └── codexion.h           # Main header: includes all sub-headers, defines structs, enums, and macros
└── src/
    ├── main.c               # Entry point: arg validation, simulation init, thread creation/joining, cleanup
    ├── simulation.c         # Lifecycle management: allocates and frees the central t_simulation struct
    ├── coder.c              # Coder thread logic: state machine (compile → debug → refactor loop)
    ├── dongle.c             # Resource management: acquiring, releasing, and cooldown enforcement
    ├── scheduler.c          # Custom Min-Heap (Priority Queue) for FIFO and EDF arbitration
    ├── monitor.c            # Monitor thread: precise burnout detection (within 10ms) and simulation halt
    └── utils.c              # Helpers: time fetching (gettimeofday), thread-safe logging, safe malloc/free


[ Your Idea ] ➔ [ Write Pseudocode ] ➔ [ AI Generates Implementation ] ➔ [ Interrogate AI ("Why?") ]
- Prompt Example: "Here is my working function. Can you review it for security vulnerabilities or performance flaws, and explain your optimization suggestions?"

## TO DO
- [ ] revise the Makefile.
- [ ] verify the parsing logic

5. Next Steps for Your "Project Thinking"
To build this effectively, follow this iterative development path:

    Step 1: The Skeleton. Write main.c, simulation.c, and utils.c. Get the program to parse arguments, initialize the struct, create threads that just print "Hello", join them, and free memory. Verify with Valgrind: 0 memory leaks.
    Step 2: The Heap. Build scheduler.c in isolation. Write a tiny main that pushes fake coder requests and pops them. Verify FIFO works, then verify EDF (min-heap based on deadline) works.
    Step 3: Single Coder. Hardcode number_of_coders = 1. Make the coder loop through Compile → Debug → Refactor. Ensure logging is perfectly formatted and mutex-protected.
    Step 4: Two Coders (The Deadlock Test). Introduce 2 coders. Implement the logic where a coder must acquire both dongles. Implement the "drop and retry" mechanism if it can't get the second one.
    Step 5: The Monitor. Add the monitor thread. Test burnout by setting time_to_burnout to a very low number (e.g., 50ms) and time_to_compile to a high number. Verify the log appears within 10ms of the actual deadline.
    Step 6: Scale and Refine. Test with 5+ coders, test the dongle_cooldown, and ensure the EDF scheduler prevents starvation.



---
T1: request_resource(worker)
T2: acquire_resource(worker)
T3: release_resource(worker)
T4: do_work(worker)

STATE
    ↓
TRANSITIONS
    ↓
NEW STATE

This is the foundation.


---
