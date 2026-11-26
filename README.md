*This project has been created as part of the 42 curriculum by mbendaou.*

# Get Next Line

## Description

`get_next_line()` is a C function that reads a file descriptor one line at a time.

By calling the function repeatedly, for example inside a loop, the entire file or standard input can be read line by line.

```c
char *get_next_line(int fd);
```

- Returns the line that was read, including the terminating `\n` if it exists.
- Returns the last line even if it does not end with a `\n`.
- Returns `NULL` when there is nothing left to read or when an error occurs.
- The number of bytes read by each `read()` call is defined at compile time using `-D BUFFER_SIZE=n`.
- If `BUFFER_SIZE` is not defined, a default value of `5000` is used.

Allowed external functions: `read`, `malloc`, and `free`.

No `libft`, no `lseek`, and no global variables.

The main goal of this project is to understand static variables, file descriptors, the `read()` system call, linked lists, and careful memory management.

## Files

| File | Content |
|---|---|
| `get_next_line.c` | `get_next_line`, `fill_stash`, `before_newline`, `after_newline`, `clean_node` |
| `get_next_line_utils.c` | `ft_lst_new`, `ft_add_back`, `ft_clear`, `line_len`, `find_newline` |
| `get_next_line.h` | `t_list`, function prototypes, default `BUFFER_SIZE` |
| `get_next_line_bonus.c` | Bonus implementation supporting multiple file descriptors |
| `get_next_line_utils_bonus.c` | Bonus linked-list utility functions |
| `get_next_line_bonus.h` | Bonus structure and function prototypes |

## Instructions

### Compilation

Mandatory part:

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
get_next_line.c get_next_line_utils.c main.c
```

Bonus part:

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
get_next_line_bonus.c get_next_line_utils_bonus.c main.c
```

`-D BUFFER_SIZE=n` is optional. Without it, the default value defined in the header is used.

`main.c` is a test program and is not part of the project submission.

### Usage

Include the required headers and the project header:

```c
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line.h"
```

Example:

```c
int main(void)
{
    int     fd;
    char    *line;

    fd = open("file.txt", O_RDONLY);
    if (fd == -1)
        return (1);

    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }

    close(fd);
    return (0);
}
```

The caller owns every returned line and must free it after use.

## Algorithm

### Overview

The function uses a static variable named `stash`, implemented as a singly linked list.

The stash stores data that has been read from the file descriptor but has not yet been returned to the caller.

Each node contains a chunk of text copied from one `read()` operation.

A call to `get_next_line(fd)` follows these steps:

1. **Check the arguments**

   If `fd < 0` or `BUFFER_SIZE <= 0`, the function returns `NULL`.

2. **Fill the stash**

   `fill_stash()` reads data from the file descriptor until a newline is found in the stash, the end of the file is reached, or an error occurs.

   Each successful `read()` result is stored in a new node.

   The read buffer is null-terminated after the number of bytes actually read.

   If `read()` returns `-1`, the stash is freed and the function returns an error.

3. **Extract the line**

   `before_newline()` calculates the length of the next line using `line_len()`.

   It then allocates a new string and copies the characters from the stash into it, including the newline if present.

4. **Clean the stash**

   `clean_node()` removes the data that belongs to the returned line.

   Nodes that have been completely consumed are freed.

   If a node contains data after the newline, `after_newline()` keeps the remaining characters for the next call.

5. **Return the line**

   The extracted line is returned to the caller.

### Why does `fill_stash()` check the last node?

The implementation checks the last node of the stash for a newline before performing another `read()`.

When a new chunk is added, it is checked for a newline. Therefore, if the last node contains a newline, the function already has enough data to return the next line.

This prevents unnecessary calls to `read()` when a complete line is already available in memory.

It is especially important when reading from standard input, where an unnecessary `read()` could block and wait for more input.

### Example

File content:

```text
Hello
World
```

Assume `BUFFER_SIZE = 4`.

| Call | Data read | Returned line | Remaining stash |
|---|---|---|---|
| 1 | `Hell`, `o\nWo` | `Hello\n` | `Wo` |
| 2 | `rld\n` | `World\n` | Empty |
| 3 | EOF | `NULL` | Empty |

The stash keeps the characters that belong to the next line, allowing the function to continue reading correctly on the following call.

## Why a Linked List?

A linked list allows the function to store multiple chunks of data without repeatedly reallocating and copying the entire accumulated string.

Each node owns its own string, and the cleaning process frees the data that has already been consumed.

Advantages:

- Handles lines of different lengths.
- Stores data dynamically as needed.
- Keeps unread data between function calls.
- Avoids repeatedly copying the entire stash after every `read()`.

Trade-offs:

- Each node requires memory allocation for both the node and its string.
- `ft_add_back()` traverses the list to find the last node.
- A very long line with a small `BUFFER_SIZE` may require many nodes and allocations.
- The Norm limits the number of functions per file, which makes organizing a linked-list implementation more challenging.

## Error Handling and Memory Management

- If `read()` returns `-1`, the stash is freed and `NULL` is returned.
- If a memory allocation fails, allocated memory is released before returning.
- The read buffer is allocated dynamically using `malloc()`.
- The read buffer is freed after `fill_stash()` finishes.
- Every returned line must be freed by the caller.
- The stash is cleaned as the function consumes each line.

The implementation was tested with different buffer sizes and memory-checking tools to detect leaks and invalid memory accesses.

## Bonus: Multiple File Descriptors

The bonus part supports reading from multiple file descriptors simultaneously.

The static variable is an array of linked-list pointers:

```c
static t_list *stash[1048576];
```

Each file descriptor has its own stash, allowing the function to maintain independent reading states.

For example, reading from file descriptor 3, then 4, then 5, and returning to 3 does not mix their reading states.

The implementation accepts file descriptors in the range `0` to `1048575`.

A hard limit of `1048576` entries is used to define the maximum number of file descriptors supported by this implementation. This is a limit chosen by the implementation and does not necessarily match the system's actual file descriptor limit.

## Testing

The project was tested with different buffer sizes:

- `BUFFER_SIZE=1`
- `BUFFER_SIZE=42`
- `BUFFER_SIZE=9999`
- `BUFFER_SIZE=10000000`
- Default `BUFFER_SIZE`

Test cases included:

- Empty files
- Files containing only a newline
- Files without a final newline
- Very long lines
- Multiple consecutive empty lines
- Invalid file descriptors
- Standard input
- Reading from multiple file descriptors in the bonus part

Additional checks:

```sh
valgrind --leak-check=full ./a.out
```

```sh
norminette
```

Community testers were also used as an additional way to check the implementation.

## Resources

- [`read()`](https://man7.org/linux/man-pages/man2/read.2.html)
- [`malloc()` and `free()`](https://man7.org/linux/man-pages/man3/malloc.3.html)
- [Linked lists](https://www.geeksforgeeks.org/data-structures/linked-list/)
- AI tools and peer discussions

## Use of AI

AI tools were used as learning and debugging assistants during this project.

They helped with:

- Understanding file descriptors, `read()`, static variables, and `BUFFER_SIZE`.
- Understanding linked lists and memory management.
- Reviewing code written during the project and identifying possible bugs.
- Understanding compiler errors and Norminette issues.
- Exploring test cases and memory-checking techniques.
- Preparing and improving the first draft of this README.

The implementation was developed through personal practice, debugging, and testing. AI assistance was used to support the learning process and improve understanding of the project.

## Author

mbendaou

42 Network
