#ifndef CONFIG_H
#define CONFIG_H

// String used to delimit block outputs in the status.
#define DELIMITER " | "

// Maximum number of Unicode characters that a block can output.
#define MAX_BLOCK_OUTPUT_LENGTH 64

// Control whether blocks are clickable.
#define CLICKABLE_BLOCKS 1

// Control whether a leading delimiter should be prepended to the status.
#define LEADING_DELIMITER 0

// Control whether a trailing delimiter should be appended to the status.
#define TRAILING_DELIMITER 0

// Define blocks for the status feed as X(icon, cmd, interval, signal).
#define BLOCKS(X)             \
    X("  ", "org-clock-task", 30, 12) \
    X("", "sb-network", 20, 5) \
    X("", "sb-volume", 1, 4) \
    X("", "sb-battery", 30, 3) \
    X("󰃭 ", "date '+%a %b %d %I:%M %p'", 30, 1) \

#endif  // CONFIG_H
