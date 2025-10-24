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

[Installation](#%EF%B8%8F-installation) • [Usage](#-usage) • [The Problem](#%EF%B8%8F-the-dining-philosophers-problem) • [Implementation](#-implementation) • [Testing](#-testing)

</div>

---

## 📋 Table of Contents

- [About the Project](#-about-the-project)
- [Installation](#%EF%B8%8F-installation)
- [Usage](#-usage)
- [The Dining Philosophers Problem](#%EF%B8%8F-the-dining-philosophers-problem)
- [Implementation](#-implementation)
- [Project Structure](#-project-structure)
- [Testing](#-testing)
- [What I Learned](#-what-i-learned)
- [Requirements](#-requirements)
- [License](#-license)

---

## 🎯 About the Project

**Philosophers** is a classic concurrency problem that teaches fundamental concepts in multithreading and process synchronization. The challenge is to prevent philosophers from starving while avoiding deadlocks and race conditions.

### Why Philosophers?

<table>
<tr>
<td width="50%" valign="top">

### 🧵 Core Concurrency
- **Multithreading** with `pthread_create()`
- **Mutex synchronization** for race prevention
- Thread lifecycle management and joining
- Shared memory space between threads

</td>
<td width="50%" valign="top">

### 🔒 Synchronization Primitives
- **Mutexes** for fork protection (mandatory)
- **Semaphores** for process sync (bonus)
- Timing precision with `gettimeofday()`
- `usleep()` for microsecond accuracy

</td>
</tr>
<tr>
<td width="50%" valign="top">

### 🎯 Bonus Features
- **Process management** with `fork()`
- **Counting semaphores** for resource pools
- Inter-process communication
- Separate address spaces per process

</td>
<td width="50%" valign="top">

### 🛡️ Problem Prevention
- **Deadlock prevention** with asymmetric pickup
- **Race condition** elimination
- **Starvation prevention** with fair timing
- Death detection within 10ms

</td>
</tr>
</table>

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

<details>
<summary><b>Basic Simulations</b></summary>

```bash
# 5 philosophers, 800ms to die, 200ms to eat, 200ms to sleep
./philo 5 800 200 200

# Tight timing (challenging)
./philo 4 310 200 100

# Should not die
./philo 4 410 200 200
```

</details>

<details>
<summary><b>With Meal Limit</b></summary>

```bash
# Stops when each philosopher has eaten 7 times
./philo 5 800 200 200 7

# Many meals
./philo 4 410 200 200 1000
```

</details>

<details>
<summary><b>Edge Cases</b></summary>

```bash
# Single philosopher (should die - only 1 fork)
./philo 1 800 200 200

# Many philosophers (stress test)
./philo 200 800 200 200
```

</details>

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

N philosophers sit around a circular table with N forks. Each philosopher alternates between three states:

```
        Fork 0
    Phil 0    Phil 4
Fork 1           Fork 4
    Phil 1    Phil 3
        Fork 2
        Phil 2
        Fork 3
```

**Philosopher States:**
1. **Thinking** - No resources needed
2. **Eating** - Needs **2 forks** (left AND right)
3. **Sleeping** - No resources needed

### The Challenges

<table>
<tr>
<th>Challenge</th>
<th>Description</th>
<th>Solution</th>
</tr>
<tr>
<td><b>Resource Sharing</b></td>
<td>Only N forks for N philosophers, each needs 2 simultaneously</td>
<td>Mutexes/semaphores protect fork access</td>
</tr>
<tr>
<td><b>Deadlock</b></td>
<td>All philosophers pick right fork → none can get left</td>
<td>Even/odd pickup strategy breaks circular wait</td>
</tr>
<tr>
<td><b>Starvation</b></td>
<td>Some philosophers eat repeatedly while others never do</td>
<td>Continuous death monitoring ensures fairness</td>
</tr>
<tr>
<td><b>Race Conditions</b></td>
<td>Multiple threads accessing shared death flag</td>
<td>Dedicated mutex for death printing</td>
</tr>
</table>

### Solution Strategies

#### Deadlock Prevention
- **Even philosophers**: Pick right fork first
- **Odd philosophers**: Pick left fork first
- Breaks circular wait condition

#### Starvation Prevention
- Timing checks ensure no philosopher waits too long
- Death monitoring runs continuously
- `time_to_die` enforces maximum wait

#### Race Condition Prevention
- Mutexes protect fork access (mandatory)
- Semaphores control fork availability (bonus)
- Dedicated mutex/semaphore for death printing

---

## 💻 Implementation

### Data Structure

The implementation uses a single comprehensive structure:

<details>
<summary><b>t_ph - Philosopher Structure</b></summary>

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
    int             times;          // Times eaten so far
    int             max;            // Max times to eat (-1 if infinite)
    int             left;           // Left fork index
    int             right;          // Right fork index
    int             kill;           // Death flag (local)
    int             full;           // Finished eating flag
    int             wait;           // Synchronization flag
    int             print;          // Print synchronization
    int             *stat;          // Shared death status pointer
    pthread_mutex_t *mutex;         // Array of fork mutexes (mandatory)

    // Bonus-specific fields
    sem_t           *sem;           // Fork semaphore (bonus)
    sem_t           *semw;          // Wait semaphore (bonus)
    sem_t           *semd;          // Death semaphore (bonus)
    int             *child;         // Child PIDs (bonus)
    int             status;         // Wait status (bonus)
}   t_ph;
```

**Key Fields:**
- `life`: Current time for comparison
- `die`: Calculated death time (life + blood)
- `born`: Simulation start for timestamps
- `blood`: time_to_die parameter
- `stat`: Shared flag to signal death to all threads/processes

</details>

### Mandatory Version (Threads + Mutexes)

**Architecture:** Each philosopher is a **thread**, forks are protected by **mutexes**.

<details>
<summary><b>Main Flow and Initialization</b></summary>

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

    // Allocate shared resources
    ph.mutex = malloc(ph.num * sizeof(pthread_mutex_t));
    ph.stat = malloc(sizeof(int));
    memset(ph.stat, 0, sizeof(int));

    // Validate arguments
    if (ph.num < 1 || ph.blood < 1 || ph.eat < 1 || ph.sleep < 1)
        return;

    ft_create(&ph);
}
```

</details>

<details>
<summary><b>Thread Creation and Synchronization</b></summary>

```c
void ft_create(t_ph *ph)
{
    pthread_t *thread;
    int i;

    // Initialize fork mutexes
    i = -1;
    while (++i < ph->num)
        pthread_mutex_init(&ph->mutex[i], NULL);

    // Create philosopher threads
    thread = malloc(ph->num * sizeof(pthread_t));
    i = -1;
    while (++i < ph->num)
    {
        pthread_mutex_lock(&ph->mutex[0]);  // Synchronization barrier
        ph->id = i + 1;
        ph->left = i;
        ph->right = (i + 1) % ph->num;
        pthread_create(&thread[i], NULL, ft_routine, ph);
    }

    // Wait for all threads to complete
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

**Synchronization Barrier:**
- `mutex[0]` locked before each thread creation
- Each thread unlocks it immediately after copying data
- Ensures all philosophers start at roughly the same time

</details>

<details>
<summary><b>Philosopher Thread Routine</b></summary>

```c
void *ft_routine(void *tid)
{
    t_ph ph;
    struct timeval live;

    ph = *(t_ph *)tid;
    pthread_mutex_unlock(&ph.mutex[0]);  // Release sync barrier

    // Wait for all philosophers to be created
    ft_wait(&ph);

    // Initialize timing
    gettimeofday(&live, NULL);
    ph.life = (live.tv_sec * 1000) + (live.tv_usec / 1000);
    ph.die = ph.life + ph.blood;
    ph.born = ph.life;
    ph.times = 0;
    ph.kill = 0;
    ph.wait = 0;
    ph.print = 0;

    // Main philosopher loop
    while (ph.stat[0] == 0)  // While no one has died
    {
        // Even philosophers wait slightly to prevent deadlock
        if ((ph.id % 2) == 0 && ph.wait == 0)
        {
            ph.wait = 1;
            usleep(1500);
        }

        ft_fk1(&ph);  // Attempt to eat

        if (ph.stat[0] == 1)
            break;
    }

    return (0);
}
```

**Even/Odd Strategy:**
- Even-numbered philosophers wait 1.5ms before first attempt
- Creates staggered start to prevent simultaneous fork grabbing

</details>

<details>
<summary><b>Fork Acquisition and Eating</b></summary>

```c
void ft_fk1(t_ph *ph)
{
    struct timeval take;

    // Lock right fork first
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

    // Check if about to die before taking second fork
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

void ft_eat(t_ph *ph)
{
    struct timeval eat;

    if (ph->kill == 0 && ph->stat[0] == 0)
    {
        gettimeofday(&eat, NULL);
        ph->life = (eat.tv_sec * 1000) + (eat.tv_usec / 1000);

        // Check death before eating
        if ((ph->die - ph->life) < 0)
            ft_die(ph);

        if (ph->kill == 0 && ph->stat[0] == 0)
        {
            printf("%ld %d is eating\n", (ph->life - ph->born), ph->id);

            // Track meal count
            if (ph->max != -1)
            {
                ph->times += 1;
                if (ph->times == ph->max)
                    ft_maxtimes(ph);
            }
        }

        // Wait while eating
        ft_eatime(ph);

        // Update death deadline after eating
        gettimeofday(&eat, NULL);
        ph->life = (eat.tv_sec * 1000) + (eat.tv_usec / 1000);
        ph->die = ph->life + ph->blood;
        ph->print = 0;
    }

    // Release both forks
    pthread_mutex_unlock(&ph->mutex[ph->right]);
    pthread_mutex_unlock(&ph->mutex[ph->left]);

    ft_sleep(ph);
}
```

**Critical Death Checks:**
- Before taking second fork
- Before printing "is eating"
- Ensures death detected within 10ms requirement

</details>

<details>
<summary><b>Death Detection</b></summary>

```c
void ft_die(t_ph *ph)
{
    struct timeval die;
    pthread_mutex_t mutex;

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_lock(&mutex);

    if (ph->kill == 0 && ph->stat[0] == 0)
    {
        ph->stat[0] = 1;  // Signal all threads to stop
        ph->kill = 1;

        gettimeofday(&die, NULL);
        ph->life = (die.tv_sec * 1000) + (die.tv_usec / 1000);

        printf("%ld %d died\n", (ph->life - ph->born), ph->id);

        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_destroy(&mutex);
}
```

**Race Prevention:**
- Temporary mutex prevents multiple death messages
- `stat[0]` shared flag stops all threads
- Only first death is printed

</details>

### Bonus Version (Processes + Semaphores)

**Architecture:** Each philosopher is a **process**, forks are a **counting semaphore**.

<details>
<summary><b>Mandatory vs Bonus Comparison</b></summary>

<table>
<tr>
<th>Aspect</th>
<th>Mandatory</th>
<th>Bonus</th>
</tr>
<tr>
<td><b>Concurrency Unit</b></td>
<td>Threads (<code>pthread_t</code>)</td>
<td>Processes (<code>fork()</code>)</td>
</tr>
<tr>
<td><b>Synchronization</b></td>
<td>Mutexes (<code>pthread_mutex_t</code>)</td>
<td>Semaphores (<code>sem_t</code>)</td>
</tr>
<tr>
<td><b>Fork Representation</b></td>
<td>Individual mutex per fork</td>
<td>Counting semaphore (N resources)</td>
</tr>
<tr>
<td><b>Memory Space</b></td>
<td>Shared memory</td>
<td>Separate address spaces</td>
</tr>
<tr>
<td><b>Creation Function</b></td>
<td><code>pthread_create()</code></td>
<td><code>fork()</code></td>
</tr>
<tr>
<td><b>Waiting Function</b></td>
<td><code>pthread_join()</code></td>
<td><code>waitpid()</code></td>
</tr>
<tr>
<td><b>Death Signaling</b></td>
<td>Shared <code>stat[0]</code> flag</td>
<td>Exit status (exit code 1)</td>
</tr>
</table>

</details>

<details>
<summary><b>Semaphore Setup (Bonus)</b></summary>

```c
void ft_create(t_ph *ph)
{
    // Unlink any previous semaphores
    sem_unlink("forks");
    sem_unlink("wait");
    sem_unlink("die");

    // Create named semaphores
    ph->semd = sem_open("die", O_CREAT, 0644, 1);        // Death mutex
    ph->semw = sem_open("wait", O_CREAT, 0644, 0);      // Sync semaphore
    ph->sem = sem_open("forks", O_CREAT, 0644, ph->num); // Fork pool

    ph->child = malloc((ph->num + 1) * sizeof(int));

    ft_alive(ph);  // Create philosopher processes

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

**Semaphore Purposes:**
- `forks`: Counting semaphore initialized to N (fork pool)
- `die`: Binary semaphore for death message protection
- `wait`: Synchronization barrier for process startup

</details>

<details>
<summary><b>Process Creation (Bonus)</b></summary>

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
            ph->left = i;
            ph->right = (i + 1) % ph->num;
            pid = fork();
            ph->child[i] = pid;  // Store child PID
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
                ft_kill(ph);      // Kill all other processes
        }
    }
}

void ft_kill(t_ph *ph)
{
    int i = -1;

    while (++i <= ph->num)
    {
        if (ph->child[i] > 0)
            kill(ph->child[i], SIGKILL);
    }
}
```

**Process Management:**
- Parent forks N child processes
- Each child runs philosopher routine
- Parent waits for any child to exit
- If one dies (status ≠ 0), kill all others

</details>

<details>
<summary><b>Fork Acquisition with Semaphores (Bonus)</b></summary>

```c
void ft_fk1(t_ph *ph)
{
    struct timeval take;

    // Wait for fork from pool
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

**Counting Semaphore Logic:**
- `sem_wait()` decrements available forks
- If count = 0, process blocks until fork available
- `sem_post()` increments after releasing fork

</details>

<details>
<summary><b>Death Handling (Bonus)</b></summary>

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

**Key Difference:**
- Uses `exit(1)` instead of shared flag
- Parent detects death via `waitpid()` status
- Triggers `ft_kill()` to terminate all siblings

</details>

---

## 📁 Project Structure

```
philosophers/
├── 📂 philo/                    # Mandatory (Threads + Mutexes)
│   ├── 📄 Makefile
│   ├── 📄 ft_philo.h           # Header with t_ph structure
│   ├── 📄 ft_philosopher.c     # Main, init, thread creation (~120 lines)
│   ├── 📄 ft_routine.c         # Thread routine, synchronization (~80 lines)
│   └── 📄 ft_eat.c             # Eating, sleeping, death logic (~180 lines)
│
└── 📂 philo_bonus/              # Bonus (Processes + Semaphores)
    ├── 📄 Makefile
    ├── 📄 ft_philo_bonus.h     # Header with t_ph structure
    ├── 📄 ft_philosopher_bonus.c # Main, init, process creation (~110 lines)
    ├── 📄 ft_routine_bonus.c   # Process routine, fork management (~90 lines)
    └── 📄 ft_eat_bonus.c       # Eating, sleeping, death logic (~170 lines)
```

### File Breakdown

| File | Lines | Purpose |
|------|-------|---------|
| **Mandatory** | | |
| `ft_philosopher.c` | ~120 | Argument parsing, initialization, thread creation, join |
| `ft_routine.c` | ~80 | Thread routine, synchronization barrier, wait logic |
| `ft_eat.c` | ~180 | Fork acquisition, eating, sleeping, death detection |
| **Bonus** | | |
| `ft_philosopher_bonus.c` | ~110 | Argument parsing, semaphore setup, cleanup |
| `ft_routine_bonus.c` | ~90 | Process creation, waiting, killing processes |
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

.PHONY: all clean fclean re
```

---

## 🧪 Testing

### Test Categories

<table>
<tr>
<th>Category</th>
<th>Command</th>
<th>Expected Result</th>
</tr>
<tr>
<td><b>Should Not Die</b></td>
<td><code>./philo 5 800 200 200</code></td>
<td>Runs indefinitely without death</td>
</tr>
<tr>
<td><b></b></td>
<td><code>./philo 4 410 200 200</code></td>
<td>Tight timing but survives</td>
</tr>
<tr>
<td><b>Should Die</b></td>
<td><code>./philo 4 310 200 100</code></td>
<td>Philosopher dies (time_to_die too short)</td>
</tr>
<tr>
<td><b>Meal Limit</b></td>
<td><code>./philo 5 800 200 200 7</code></td>
<td>Stops after all eat 7 times</td>
</tr>
<tr>
<td><b></b></td>
<td><code>./philo 4 410 200 200 1000</code></td>
<td>Long duration with meal tracking</td>
</tr>
<tr>
<td><b>Edge Cases</b></td>
<td><code>./philo 1 800 200 200</code></td>
<td>Dies immediately (only 1 fork)</td>
</tr>
<tr>
<td><b></b></td>
<td><code>./philo 200 800 200 200</code></td>
<td>Stress test with many philosophers</td>
</tr>
</table>

### Validation Criteria

#### No Data Races
```bash
# Compile with thread sanitizer
gcc -g -fsanitize=thread -pthread *.c -o philo
./philo 5 800 200 200
```

#### Death Timing
- Death must be printed **within 10ms** of actual death time
- Monitor timestamps: `time_of_death - time_last_ate ≤ time_to_die + 10ms`

#### No Message Interleaving
- Each message on its own line
- No mixed philosopher IDs in single line
- Proper mutex/semaphore protection of printf

#### Deadlock Prevention
- Program should never hang
- All philosophers should eventually get turns
- Even/odd strategy prevents circular wait

### Test Script

```bash
#!/bin/bash

echo "=== Test 1: Basic (should not die) ==="
timeout 10 ./philo 5 800 200 200

echo "\n=== Test 2: Death case ==="
./philo 4 310 200 100

echo "\n=== Test 3: Single philosopher ==="
./philo 1 800 200 200

echo "\n=== Test 4: Meal limit ==="
./philo 5 800 200 200 7

echo "\n=== Test 5: Many philosophers ==="
timeout 5 ./philo 200 800 200 200

echo "\n=== Test 6: Tight timing ==="
timeout 10 ./philo 4 410 200 200
```

### Performance Tests

```bash
# Stress test
./philo 200 800 200 200

# Very tight timing
./philo 4 310 200 100

# Long duration
timeout 60 ./philo 4 410 200 200 1000

# Many meals
./philo 5 800 200 200 100
```

---

## 💡 What I Learned

Through this project, deep understanding was gained in:

- ✅ **Thread Management**: `pthread_create()`, `pthread_join()`, `pthread_detach()`, lifecycle
- ✅ **Mutex Synchronization**: `pthread_mutex_lock/unlock()`, protecting shared data
- ✅ **Semaphore Usage**: `sem_open()`, `sem_wait()`, `sem_post()`, counting semaphores
- ✅ **Process Management**: `fork()`, `waitpid()`, `kill()`, process lifecycles
- ✅ **Race Conditions**: Identifying and preventing data races with synchronization
- ✅ **Deadlock Prevention**: Breaking circular wait with asymmetric resource acquisition
- ✅ **Timing Precision**: `gettimeofday()`, `usleep()`, millisecond-level accuracy
- ✅ **Resource Sharing**: Managing limited resources between concurrent entities
- ✅ **Starvation Prevention**: Ensuring fair access through continuous monitoring
- ✅ **Synchronization Barriers**: Coordinating thread/process startup timing

### Key Challenges

<table>
<tr>
<th>Challenge</th>
<th>Solution Implemented</th>
</tr>
<tr>
<td><b>Death Detection Timing</b></td>
<td>Continuous checks before fork pickup, before eating, during sleeping. Ensures ≤10ms detection.</td>
</tr>
<tr>
<td><b>Deadlock Prevention</b></td>
<td>Even/odd fork pickup strategy: even philosophers pick right first, odd pick left first.</td>
</tr>
<tr>
<td><b>Race on Death Flag</b></td>
<td>Dedicated mutex (mandatory) or semaphore (bonus) protects death message printing.</td>
</tr>
<tr>
<td><b>Single Philosopher</b></td>
<td>Special case: only 1 fork exists, philosopher waits then dies. <code>if (ph->num == 1)</code></td>
</tr>
<tr>
<td><b>Meal Counter</b></td>
<td>Track <code>ph->times</code>, signal when all reach <code>ph->max</code> via <code>ft_maxtimes()</code></td>
</tr>
<tr>
<td><b>Print Synchronization</b></td>
<td>Mutex/semaphore around printf prevents interleaved output from multiple threads/processes.</td>
</tr>
<tr>
<td><b>Process vs Thread Memory</b></td>
<td>Bonus uses separate address spaces - can't share pointers directly, uses semaphores.</td>
</tr>
<tr>
<td><b>Semaphore Cleanup</b></td>
<td><code>sem_unlink()</code> removes named semaphores, prevents resource leaks.</td>
</tr>
</table>

### Design Decisions

**Why even/odd fork pickup order?**
- Breaks circular wait condition
- Simple to implement: `if (ph->id % 2) == 0`
- Prevents all philosophers grabbing same side simultaneously
- Deadlock-free guarantee

**Why continuous death checks?**
- Ensures detection within 10ms requirement
- Checked before fork pickup
- Checked before eating
- Checked during sleep/think transitions

**Why separate death mutex/semaphore?**
- Prevents interleaved death messages
- Critical for clean output validation
- Minimal performance impact
- Only used when death occurs

**Why usleep(1500) for even philosophers?**
- Creates staggered start
- Reduces initial contention
- Simple deadlock prevention
- 1.5ms negligible delay

**Why fork pickup differs by philosopher?**
- Even: right → left
- Odd: left → right (or vice versa)
- Ensures not all pick same fork first
- Provably deadlock-free

**Why counting semaphore for forks (bonus)?**
- Models central fork pool naturally
- Simpler than individual semaphores
- Automatic resource management
- OS handles blocking/unblocking

---

## 📏 Requirements

### Mandatory

- ✅ Each philosopher is a thread
- ✅ One fork between each philosopher (N forks for N philosophers)
- ✅ Forks protected by mutexes (one mutex per fork)
- ✅ Avoid philosophers dying (unless time_to_die is too short)
- ✅ No data races
- ✅ Death printed within 10ms of occurrence
- ✅ No message interleaving
- ✅ Program arguments validated

### Bonus

- ✅ Each philosopher is a process
- ✅ All forks in center of table
- ✅ Forks represented by counting semaphore (value = N)
- ✅ Main process not a philosopher (parent waits, children execute)
- ✅ Same behavior as mandatory version

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
- **webserv** - Multi-threaded server architecture
- Future multi-threaded/multi-process projects

---

<div align="center">

**Made with ☕ by [Z3n42](https://github.com/Z3n42)**

*42 Urduliz | Circle 3*

[![42 Profile](https://img.shields.io/badge/42_Intra-ingonzal-000000?style=flat&logo=42&logoColor=white)](https://profile.intra.42.fr/users/ingonzal)

</div>
