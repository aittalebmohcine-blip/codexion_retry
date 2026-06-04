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

## TO DO
- [ ] revise the Makefile.
