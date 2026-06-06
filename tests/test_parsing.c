#include "../includes/codexion.h"

/* ENTRY POINT */
int	main(int argc, char **argv)
{
    t_config	config;

    memset(&config, 0, sizeof(t_config));
    if (!parse_arguments(argc, argv, &config))
        return (1);

    printf("Configuration parsed successfully:\n");
    printf("  number_of_coders: %d\n", config.number_of_coders);
    printf("  time_to_burnout: %d\n", config.time_to_burnout);
    printf("  time_to_compile: %d\n", config.time_to_compile);
    printf("  time_to_debug: %d\n", config.time_to_debug);
    printf("  time_to_refactor: %d\n", config.time_to_refactor);
    printf("  number_of_compiles_required: %d\n", config.number_of_compiles_required);
    printf("  dongle_cooldown: %d\n", config.dongle_cooldown);
    printf("  scheduler: %s\n", config.scheduler == SCHED_FIFO ? "fifo" : "edf");
    return (0);
}
