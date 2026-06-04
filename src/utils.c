#include "../includes/codexion.h"

/* ************************************************************************** */
/*                            ARGUMENT PARSING                                */
/* ************************************************************************** */

/**
 * @brief Checks if a string represents a valid positive integer.
 * Rejects empty strings, negative signs, decimals, and letters.
 */
static int is_valid_positive_number(const char *str)
{
    int i;

    if (!str || str[0] == '\0')
        return (0);
    
    i = 0;
    while (str[i] != '\0')
    {
        if (str[i] < '0' || str[i] > '9')
            return (0);
        i++;
    }
    return (1);
}

/**
 * @brief Safely parses a validated string of digits into a long long integer.
 */
static long long safe_parse_ll(const char *str)
{
    long long result;
    int i;

    result = 0;
    i = 0;
    while (str[i] >= '0' && str[i] <= '9')
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return (result);
}

/**
 * @brief Validates and parses all 8 command-line arguments into the simulation struct.
 * @return 0 on success, 1 on failure (prints error to stderr).
 */
int parse_arguments(t_simulation *sim, int argc, char **argv)
{
    if (argc != 9)
    {
        fprintf(stderr, "Error: Invalid number of arguments.\n");
        fprintf(stderr, "Usage: %s <nb_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <compiles_required> <dongle_cooldown> <scheduler>\n", argv[0]);
        return (1);
    }

    if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
    {
        fprintf(stderr, "Error: Scheduler must be exactly 'fifo' or 'edf'.\n");
        return (1);
    }

    if (!is_valid_positive_number(argv[1]) ||
        !is_valid_positive_number(argv[2]) ||
        !is_valid_positive_number(argv[3]) ||
        !is_valid_positive_number(argv[4]) ||
        !is_valid_positive_number(argv[5]) ||
        !is_valid_positive_number(argv[6]) ||
        !is_valid_positive_number(argv[7]))
    {
        fprintf(stderr, "Error: The first 7 arguments must be valid positive integers.\n");
        return (1);
    }

    sim->num_coders = (int)safe_parse_ll(argv[1]);
    if (sim->num_coders < 1)
    {
        fprintf(stderr, "Error: Number of coders must be at least 1.\n");
        return (1);
    }

    sim->time_to_burnout = safe_parse_ll(argv[2]);
    sim->time_to_compile = safe_parse_ll(argv[3]);
    sim->time_to_debug = safe_parse_ll(argv[4]);
    sim->time_to_refactor = safe_parse_ll(argv[5]);
    sim->compiles_required = (int)safe_parse_ll(argv[6]);
    sim->dongle_cooldown = safe_parse_ll(argv[7]);

    if (strcmp(argv[8], "fifo") == 0)
        sim->scheduler = SCHEDULER_FIFO;
    else
        sim->scheduler = SCHEDULER_EDF;

    return (0);
}
