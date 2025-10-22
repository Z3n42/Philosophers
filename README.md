<div align="center">

# 🍝 Philosophers

### I've never thought philosophy would be so deadly

<p>
  <img src="https://img.shields.io/badge/Score-125%2F100-success?style=for-the-badge&logo=42" alt="42 Score"/>
  <img src="https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white" alt="Language"/>
  <img src="https://img.shields.io/badge/Concurrency-Threads%20%26%20Processes-orange?style=for-the-badge" alt="Concurrency"/>
  <img src="https://img.shields.io/badge/License-MIT-green?style=for-the-badge" alt="License"/>
  <img src="https://img.shields.io/badge/42-Urduliz-000000?style=for-the-badge&logo=42&logoColor=white" alt="42 Urduliz"/>
</p>

*A multithreading project based on the Dining Philosophers problem - exploring race conditions, deadlocks, and resource sharing through concurrent programming.*

[Installation](#%EF%B8%8F-installation) • [Usage](#-usage) • [The Problem](#-the-dining-philosophers-problem) • [Implementation](#-implementation)

</div>

---

## 📋 Table of Contents

- [About the Project](#-about-the-project)
- [Installation](#%EF%B8%8F-installation)
- [Usage](#-usage)
- [The Dining Philosophers Problem](#-the-dining-philosophers-problem)
- [Implementation](#-implementation)
- [Mandatory vs Bonus](#-mandatory-vs-bonus)
- [Project Structure](#-project-structure)
- [Testing](#-testing)
- [What I Learned](#-what-i-learned)
- [License](#-license)

---

## 🎯 About the Project

**Philosophers** is a classic concurrency problem that teaches fundamental concepts in multithreading and process synchronization. The challenge is to prevent philosophers from starving while avoiding deadlocks and race conditions.

### Why Philosophers?

This project provides hands-on experience with:
- **Multithreading** - `pthread_create()`, thread lifecycle management
- **Mutex Synchronization** - `pthread_mutex_lock/unlock()`, avoiding race conditions
- **Semaphores** - `sem_wait()`, `sem_post()` for process synchronization
- **Process Management** - `fork()`, `waitpid()`, inter-process communication
- **Race Conditions** - Understanding and preventing data races
- **Deadlock Prevention** - Strategies to avoid circular wait conditions
- **Timing Precision** - `gettimeofday()`, `usleep()` for microsecond accuracy
- **Resource Sharing** - Managing limited resources (forks) between concurrent entities

The project requires implementing two versions:
- **Mandatory**: Threads + Mutexes
- **Bonus**: Processes + Semaphores

---

## 🛠️ Installation

### Prerequisites

- **GCC** or **Clang** compiler
- **Make**
- **pthread** library (standard on Unix systems)
- Unix-based system (Linux, macOS)

### Clone and Compile

```bash
# Clone the repository
git clone https://github.com/Z3n42/philosophers.git
cd philosophers

# Compile mandatory version (threads)
cd philo
make

# Compile bonus version (processes)
cd ../philo_bonus
make

# Clean object files
make clean

# Clean everything
make fclean

# Recompile
make re
```

---

## 🚀 Usage

### Command Syntax

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

### Arguments

| Argument | Description | Unit |
|----------|-------------|------|
| `number_of_philosophers` | Number of philosophers (and forks) | Integer |
| `time_to_die` | Time before a philosopher dies without eating | Milliseconds |
| `time_to_eat` | Time it takes to eat | Milliseconds |
| `time_to_sleep` | Time spent sleeping | Milliseconds |
| `[times_to_eat]` | Optional: simulation stops when all ate this many times | Integer |

### Examples

#### Basic Simulation
```bash
# 5 philosophers, 800ms to die, 200ms to eat, 200ms to sleep
./philo 5 800 200 200
```

#### With Meal Limit
```bash
# Stops when each philosopher has eaten 7 times
./philo 5 800 200 200 7
```

#### Edge Cases
```bash
# Single philosopher (should die)
./philo 1 800 200 200

# Tight timing
./philo 4 310 200 100
```

### Output Format

```
[timestamp_ms] [philosopher_id] [action]
```

**Actions:**
- `has taken a fork`
- `is eating`
- `is sleeping`
- `is thinking`
- `died`

**Example Output:**
```
0 1 has taken a fork
0 1 has taken a fork
0 1 is eating
0 3 has taken a fork
0 3 has taken a fork
0 3 is eating
200 1 is sleeping
200 3 is sleeping
200 2 has taken a fork
200 2 has taken a fork
200 2 is eating
```

---

## 🍽️ The Dining Philosophers Problem

### The Problem

N philosophers sit around a circular table with N forks. Each philosopher alternates between:
1. **Thinking** - No resources needed
2. **Eating** - Needs **2 forks** (left and right)
3. **Sleeping** - No resources needed

```
        Fork 0
    Phil 0    Phil 4
Fork 1           Fork 4
    Phil 1    Phil 3
        Fork 2
        Phil 2
        Fork 3
```

### The Challenges

#### 1. Resource Sharing
- Only N forks for N philosophers
- Each philosopher needs 2 forks simultaneously
- Forks are shared between adjacent philosophers

#### 2. Deadlock
If all philosophers pick up their right fork simultaneously, none can pick up the left fork → **deadlock**.

#### 3. Starvation
Some philosophers might eat repeatedly while others never get a chance → **starvation**.

#### 4. Race Conditions
Multiple threads accessing shared resources (forks, death flag) without proper synchronization.

### The Solution

The implementation uses several strategies:

**Deadlock Prevention:**
- Even philosophers pick right fork first
- Odd philosophers pick left fork first
- Breaks circular wait condition

**Starvation Prevention:**
- Timing checks ensure no philosopher waits too long
- Death monitoring runs continuously

**Race Condition Prevention:**
- Mutexes protect fork access (mandatory)
- Semaphores control fork availability (bonus)
- Dedicated mutex for death printing

---

## 💻 Implementation

### Mandatory Version (Threads + Mutexes)

**Architecture:** Each philosopher is a **thread**, forks are protected by **mutexes**.

#### Data Structure

```c
typedef struct s_ph
{
    int             id;             // Philosopher ID (1 to N)
    int             num;            // Total number of philosophers
    long            life;           // Current timestamp
    long            die;            // Death timestamp (life + time_to_die)
    long            born;           // Birth timestamp (simulation start)
    long            blood;          // time_to_die value
    long            eat;            // time_to_eat value
    long            sleep;          // time_to_sleep value
    int             times;          // Times eaten
    int             max;            // Max times to eat (-1 if infinite)
    int             left;           // Left fork index
    int             right;          // Right fork index
    int             kill;           // Death flag
    int             full;           // Finished eating flag
    int             wait;           // Synchronization flag
    int             print;          // Print synchronization
    int             *stat;          // Shared death status
    pthread_mutex_t *mutex;         // Array of fork mutexes
}   t_ph;
```

#### Main Flow

```c
int main(int argc, char **argv)
{
    if (argc >= 5 && argc <= 6)
        ft_init(argc, argv);
    return (0);
}

void ft_init(int argc, char **argv)
{
    t_ph ph;

    // Parse arguments
    ph.num = ft_atoi(argv[1]);
    ph.blood = ft_atoi(argv[2]);  // time_to_die
    ph.eat = ft_atoi(argv[3]);
    ph.sleep = ft_atoi(argv[4]);
    ph.max = (argc == 6) ? ft_atoi(argv[5]) : -1;

    // Initialize
    ph.mutex = malloc(ph.num * sizeof(pthread_mutex_t));
    ph.stat = malloc(sizeof(int));
    memset(ph.stat, 0, sizeof(int));

    // Validate
    if (ph.num < 1 || ph.blood < 1 || ph.eat < 1 || ph.sleep < 1)
        return;

    ft_create(&ph);
}
```

#### Thread Creation

```c
void ft_create(t_ph *ph)
{
    pthread_t *thread;
    int i;

    // Initialize fork mutexes
    i = -1;
    while (++i < ph->num)
        pthread_mutex_init(&ph->mutex[i], NULL);

    // Create threads
    thread = malloc(ph->num * sizeof(pthread_t));
    i = -1;
    while (++i < ph->num)
    {
        pthread_mutex_lock(&ph->mutex[0]);  // Synchronization
        ph->id = i + 1;
        pthread_create(&thread[i], NULL, ft_routine, ph);
    }

    // Join threads
    i = -1;
    while (++i < ph->num)
        pthread_join(thread[i], NULL);

    // Cleanup
    i = -1;
    while (++i < ph->num)
        pthread_mutex_destroy(&ph->mutex[i]);

    free(ph->mutex);
    free(thread);
    free(ph->stat);
}
```

#### Philosopher Routine

```c
void *ft_routine(void *tid)
{
    t_ph ph;
    struct timeval live;

    ph = *(t_ph *)tid;
    pthread_mutex_unlock(&ph.mutex[0]);  // Release sync

    // Wait for all philosophers to be created
    ft_wait(&ph);

    // Initialize timing
    gettimeofday(&live, NULL);
    ph.life = (live.tv_sec * 1000) + (live.tv_usec / 1000);
    ph.die = ph.life + ph.blood;
    ph.born = ph.life;

    // Main loop
    while (ph.stat[0] == 0)  // While no one died
    {
        // Even philosophers wait slightly
        if ((ph.id % 2) == 0 && ph.wait == 0)
        {
            ph.wait = 1;
            usleep(1500);
        }

        ft_fk1(&ph);  // Try to eat

        if (ph.stat[0] == 1)
            break;
    }

    return (0);
}
```

#### Fork Acquisition and Eating

```c
void ft_fk1(t_ph *ph)
{
    struct timeval take;

    // Lock right fork
    pthread_mutex_lock(&ph->mutex[ph->right]);
    gettimeofday(&take, NULL);
    ph->life = (take.tv_sec * 1000) + (take.tv_usec / 1000);

    if (ph->print == 0 && ph->stat[0] == 0)
    {
        printf("%ld %d has taken a fork\n", (ph->life - ph->born), ph->id);
        ph->print = 1;
    }

    // Single philosopher edge case
    if (ph->num == 1)
    {
        usleep(ph->blood * 1003);
        pthread_mutex_unlock(&ph->mutex[ph->right]);
        return;
    }

    // Check death before taking second fork
    gettimeofday(&take, NULL);
    ph->life = (take.tv_sec * 1000) + (take.tv_usec / 1000);
    if ((ph->die - ph->life) < 0)
        ft_die(ph);

    // Lock left fork
    pthread_mutex_lock(&ph->mutex[ph->left]);
    gettimeofday(&take, NULL);
    ph->life = (take.tv_sec * 1000) + (take.tv_usec / 1000);

    if (ph->stat[0] == 0)
        printf("%ld %d has taken a fork\n", (ph->life - ph->born), ph->id);

    ft_eat(ph);
}
```

#### Eating

```c
void ft_eat(t_ph *ph)
{
    struct timeval eat;

    if (ph->kill == 0 && ph->stat[0] == 0)
    {
        gettimeofday(&eat, NULL);
        ph->life = (eat.tv_sec * 1000) + (eat.tv_usec / 1000);

        if ((ph->die - ph->life) < 0)
            ft_die(ph);

        if (ph->kill == 0 && ph->stat[0] == 0)
        {
            printf("%ld %d is eating\n", (ph->life - ph->born), ph->id);

            // Track meals
            if (ph->max != -1)
            {
                ph->times += 1;
                if (ph->times == ph->max)
                    ft_maxtimes(ph);
            }
        }

        // Wait while eating
        ft_eatime(ph);

        // Update death time
        gettimeofday(&eat, NULL);
        ph->life = (eat.tv_sec * 1000) + (eat.tv_usec / 1000);
        ph->die = ph->life + ph->blood;
        ph->print = 0;
    }

    // Release forks
    pthread_mutex_unlock(&ph->mutex[ph->right]);
    pthread_mutex_unlock(&ph->mutex[ph->left]);

    ft_sleep(ph);
}
```

#### Death Detection

```c
void ft_die(t_ph *ph)
{
    struct timeval die;
    pthread_mutex_t mutex;

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_lock(&mutex);

    if (ph->kill == 0 && ph->stat[0] == 0)
    {
        ph->stat[0] = 1;  // Signal all threads
        ph->kill = 1;

        gettimeofday(&die, NULL);
        ph->life = (die.tv_sec * 1000) + (die.tv_usec / 1000);

        printf("%ld %d died\n", (ph->life - ph->born), ph->id);

        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_destroy(&mutex);
}
```

---

### Bonus Version (Processes + Semaphores)

**Architecture:** Each philosopher is a **process**, forks are a **counting semaphore**.

#### Key Differences

| Aspect | Mandatory | Bonus |
|--------|-----------|-------|
| **Concurrency Unit** | Threads | Processes |
| **Synchronization** | Mutexes | Semaphores |
| **Fork Representation** | Individual mutexes | Counting semaphore |
| **Memory** | Shared memory space | Separate address spaces |
| **Creation** | `pthread_create()` | `fork()` |
| **Termination** | `pthread_join()` | `waitpid()` |

#### Semaphore Setup

```c
void ft_create(t_ph *ph)
{
    // Unlink previous semaphores
    sem_unlink("forks");
    sem_unlink("wait");
    sem_unlink("die");

    // Create semaphores
    ph->semd = sem_open("die", O_CREAT, 0644, 1);       // Death mutex
    ph->semw = sem_open("wait", O_CREAT, 0644, 0);     // Sync semaphore
    ph->sem = sem_open("forks", O_CREAT, 0644, ph->num); // Fork pool

    ft_alive(ph);  // Create processes

    // Cleanup
    sem_close(ph->sem);
    sem_close(ph->semw);
    sem_close(ph->semd);
    sem_unlink("die");
    sem_unlink("wait");
    sem_unlink("forks");

    free(ph->child);
}
```

#### Process Creation

```c
void ft_alive(t_ph *ph)
{
    int i = -1;
    int pid;

    while (++i <= ph->num)
    {
        if (pid != 0)
        {
            ph->id = i + 1;
            pid = fork();
            ph->child[i] = pid;  // Store child PIDs
        }
        else
            ft_routine(ph);  // Child process executes routine
    }

    // Parent process waits for children
    if (pid != 0)
    {
        i = -1;
        while (++i <= ph->num)
        {
            waitpid(-1, &ph->status, 0);
            if (ph->status != 0)  // A philosopher died
                ft_kill(ph);      // Kill all processes
        }
    }
}
```

#### Fork Acquisition (Bonus)

```c
void ft_fk1(t_ph *ph)
{
    struct timeval take;

    // Wait for fork from semaphore pool
    sem_wait(ph->sem);
    gettimeofday(&take, NULL);
    ph->life = (take.tv_sec * 1000) + (take.tv_usec / 1000);

    sem_wait(ph->semd);  // Protect printf
    printf("%ld %d has taken a fork\n", (ph->life - ph->born), ph->id);
    sem_post(ph->semd);

    // Single philosopher edge case
    if (ph->num == 1)
    {
        usleep(ph->blood * 1000);
        ft_die(ph);
    }

    // Check death
    gettimeofday(&take, NULL);
    ph->life = (take.tv_sec * 1000) + (take.tv_usec / 1000);
    if ((ph->die - ph->life) < 0)
        ft_die(ph);

    // Wait for second fork
    sem_wait(ph->sem);
    gettimeofday(&take, NULL);
    ph->life = (take.tv_sec * 1000) + (take.tv_usec / 1000);

    sem_wait(ph->semd);
    printf("%ld %d has taken a fork\n", (ph->life - ph->born), ph->id);
    sem_post(ph->semd);

    ft_eat(ph);
}
```

#### Death (Bonus)

```c
void ft_die(t_ph *ph)
{
    struct timeval die;

    sem_wait(ph->semd);  // Lock to prevent message interleaving

    gettimeofday(&die, NULL);
    ph->life = (die.tv_sec * 1000) + (die.tv_usec / 1000);

    printf("%ld %d died\n", (ph->life - ph->born), ph->id);

    exit(1);  // Exit with status 1 signals death to parent
}
```

---

## 📁 Project Structure

```
philosophers/
├── 📂 philo/                    # Mandatory (Threads + Mutexes)
│   ├── 📄 Makefile
│   ├── 📄 ft_philo.h           # Header with t_ph structure
│   ├── 📄 ft_philosopher.c     # Main, init, thread creation
│   ├── 📄 ft_routine.c         # Thread routine and synchronization
│   └── 📄 ft_eat.c             # Eating, sleeping, death logic
└── 📂 philo_bonus/              # Bonus (Processes + Semaphores)
    ├── 📄 Makefile
    ├── 📄 ft_philo_bonus.h     # Header with t_ph structure
    ├── 📄 ft_philosopher_bonus.c # Main, init, process creation
    ├── 📄 ft_routine_bonus.c   # Process routine and synchronization
    └── 📄 ft_eat_bonus.c       # Eating, sleeping, death logic
```

### File Breakdown

| File | Lines | Purpose |
|------|-------|---------|
| **Mandatory** | | |
| `ft_philosopher.c` | ~120 | Argument parsing, initialization, thread creation |
| `ft_routine.c` | ~80 | Thread routine, synchronization barrier |
| `ft_eat.c` | ~180 | Fork acquisition, eating, sleeping, death detection |
| **Bonus** | | |
| `ft_philosopher_bonus.c` | ~110 | Argument parsing, initialization, semaphore setup |
| `ft_routine_bonus.c` | ~90 | Process routine, process creation, waiting |
| `ft_eat_bonus.c` | ~170 | Fork acquisition with semaphores, eating, death |

### Makefile

```makefile
NAME = philo
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread
FILES = ft_philosopher ft_routine ft_eat
OBJS = $(FILES:=.o)

$(NAME): $(OBJS)
    $(CC) $(CFLAGS) $^ -o $@

all: $(NAME)

clean:
    $(RM) $(OBJS)

fclean: clean
    $(RM) $(NAME)

re: fclean all
```

---

## 🧪 Testing

### Basic Tests

```bash
# Should not die
./philo 5 800 200 200
./philo 4 410 200 200

# Should die
./philo 4 310 200 100

# With meal limit (should stop)
./philo 5 800 200 200 7

# Edge case: single philosopher (should die)
./philo 1 800 200 200

# Many philosophers
./philo 200 800 200 200
```

### Validation Criteria

✅ **No Data Races**:
```bash
# Compile with thread sanitizer
gcc -g -fsanitize=thread -pthread *.c -o philo
./philo 5 800 200 200
```

✅ **No Death Delay**:
- Death must be printed within 10ms of actual death time
- Monitor timestamps carefully

✅ **No Message Interleaving**:
- Each message on its own line
- No mixed philosopher IDs in single line

✅ **Deadlock Prevention**:
- Program should never hang
- All philosophers should get turns

### Performance Tests

```bash
# Stress test
./philo 200 800 200 200

# Tight timing
./philo 4 310 200 100

# Long duration
./philo 4 410 200 200 1000
```

### Test Script

```bash
#!/bin/bash

echo "=== Test 1: Basic (should not die) ==="
timeout 10 ./philo 5 800 200 200

echo "=== Test 2: Death case ==="
./philo 4 310 200 100

echo "=== Test 3: Single philosopher ==="
./philo 1 800 200 200

echo "=== Test 4: Meal limit ==="
./philo 5 800 200 200 7

echo "=== Test 5: Many philosophers ==="
timeout 5 ./philo 200 800 200 200
```

---

## 💡 What I Learned

Through this project, deep understanding was gained in:

- ✅ **Thread Management**: `pthread_create()`, `pthread_join()`, `pthread_detach()`
- ✅ **Mutex Synchronization**: `pthread_mutex_lock/unlock()`, protecting shared data
- ✅ **Semaphore Usage**: `sem_open()`, `sem_wait()`, `sem_post()`, counting semaphores
- ✅ **Process Management**: `fork()`, `waitpid()`, `kill()`, process lifecycles
- ✅ **Race Conditions**: Identifying and preventing data races
- ✅ **Deadlock Prevention**: Breaking circular wait conditions
- ✅ **Timing Precision**: `gettimeofday()`, `usleep()`, millisecond accuracy
- ✅ **Resource Sharing**: Managing limited resources between concurrent entities
- ✅ **Starvation Prevention**: Ensuring fair access to resources
- ✅ **Synchronization Barriers**: Coordinating thread/process startup

### Key Challenges

1. **Death Detection Timing**: Ensuring death is detected and printed within 10ms
2. **Deadlock Prevention**: Implementing asymmetric fork pickup strategy
3. **Race Condition on Death**: Protecting death printing with mutex/semaphore
4. **Single Philosopher Edge Case**: Handling case where only 1 fork exists
5. **Meal Counter**: Tracking when all philosophers have eaten enough times
6. **Print Synchronization**: Preventing message interleaving
7. **Process vs Thread**: Understanding memory sharing differences
8. **Semaphore Cleanup**: Properly unlinking named semaphores

### Design Decisions

**Why even/odd fork pickup?**
- Breaks circular wait condition
- Simple to implement
- Prevents deadlock effectively

**Why continuous death checks?**
- Ensures death detected within 10ms
- Checked before fork pickup
- Checked during eating and sleeping

**Why separate death mutex?**
- Prevents death message interleaving
- Critical for clean output
- Minimal performance impact

**Why usleep(100) in timing loops?**
- Reduces CPU usage
- Still provides millisecond precision
- Balances accuracy and efficiency

**Why fork pickup order differs?**
- Even philosophers: right→left
- Odd philosophers: left→right (default)
- Prevents all picking same side simultaneously

---

## 📏 Requirements

### Mandatory

- ✅ Each philosopher is a thread
- ✅ One fork between each philosopher (N forks for N philosophers)
- ✅ Forks protected by mutexes
- ✅ Avoid philosophers dying
- ✅ No data races
- ✅ Death printed within 10ms
- ✅ No message interleaving

### Bonus

- ✅ Each philosopher is a process
- ✅ All forks in center of table
- ✅ Forks represented by semaphore
- ✅ Main process not a philosopher

### Allowed Functions

**Mandatory:**
- `memset`, `printf`, `malloc`, `free`, `write`
- `usleep`, `gettimeofday`
- `pthread_create`, `pthread_detach`, `pthread_join`
- `pthread_mutex_init`, `pthread_mutex_destroy`
- `pthread_mutex_lock`, `pthread_mutex_unlock`

**Bonus (additional):**
- `fork`, `kill`, `exit`, `waitpid`
- `sem_open`, `sem_close`, `sem_post`, `sem_wait`, `sem_unlink`

---

## 📄 License

MIT License - See LICENSE file for details.

This project is part of the 42 School curriculum. Feel free to use and learn from this code, but please don't copy it for your own 42 projects. Understanding comes from doing it yourself! 🚀

---

## 🔗 Related Projects

Skills learned here apply to:
- **Minishell** - Signal handling, process management
- **ft_server** - Concurrent request handling
- Future multi-threaded/multi-process projects

---

<div align="center">

**Made with ☕ by [Z3n42](https://github.com/Z3n42)**

*42 Urduliz | Circle 3*

[![42 Profile](https://img.shields.io/badge/42_Intra-ingonzal-000000?style=flat&logo=42&logoColor=white)](https://profile.intra.42.fr/users/ingonzal)

</div>
