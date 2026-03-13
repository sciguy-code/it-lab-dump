# Student Record Management System (srms.c)

---

## The Core Idea

the whole program is essentially split into four layers that build on top of each other, which matches exactly what the assignment asked. you start small, just store a few students with basic info, then slowly extend that data, add computation on top of it, and finally extract insights from it. it's a pretty natural progression once you see it that way.

the two structs — `Student` and `NewStudent` — aren't there to be fancy. they exist because part 2a explicitly says don't add books-related fields yet. so `Student` is the lean version, and `NewStudent` is the extended one. you could argue this feels redundant (and you'd be right), but the assignment is deliberately teaching you how to handle struct evolution in memory.

---

## How Memory Is Being Used

### sem_res — the dynamic part of Student

`sem_res` is a `float *`, a pointer to a dynamically allocated array. the size of that array depends on the student's year — if they're in year 2, they've completed 4 semesters, so you allocate `4 * sizeof(float)`. this is the right call because not every student has the same number of results, and hardcoding 8 would waste memory and feel sloppy.

```
heap after malloc for 3 students:

  snew (array on heap)
  [ id | name | year | sem_res* | books_issued* ]   <-- student 0
  [ id | name | year | sem_res* | books_issued* ]   <-- student 1
  [ id | name | year | sem_res* | books_issued* ]   <-- student 2
        |                 |
        |                 +---> [ s0 | s1 | s2 | s3 ]  (separate heap block)
        +---------------------> [ b0 | b1 | b2 | b3 ]  (another heap block)
```

so you've got the main array, and then each student points to their own little heap island for results and books. this is what people mean when they say "non-contiguous heap allocation."

### the realloc situation

this is the most interesting part of the whole program. the intuition of doing `realloc(s, N * sizeof(struct NewStudent))` seems perfectly reasonable — you're just asking for more space. but it silently breaks for N > 1, and here's the mental model for why:

```
before (Student, say 64 bytes each, N=3):

  offset:  0        64       128
           [Stu 0] [Stu 1] [Stu 2]

after realloc to NewStudent (say 72 bytes each):

  offset:  0        72       144
           [    ] [    ] [    ]
           ^        ^
           |        |
     Stu 0 fits   Stu 1's data was at byte 64,
     just fine    but the slot now starts at 72.
                  so you're reading garbage.
```

student 0 is fine because it starts at offset 0 in both cases. but everyone after index 0 is misaligned. so the fix is: allocate a fresh `NewStudent` block, loop through and manually copy each field, then `free` the old block. it's more lines but it's correct. and honestly, this is what `realloc` does internally anyway — when it can't extend in-place, it allocates new space, copies everything, and frees the old. you're just doing the same thing with full control over the copy logic.

---

## The GPA and Topper Functions

`calcGPA` takes a `struct NewStudent *` and uses `*(s->sem_res + i)` to walk through the results. this is pointer arithmetic doing what array indexing does — `s->sem_res[i]` and `*(s->sem_res + i)` are literally the same thing under the hood.

`findToppers` runs two passes. the first one is a simple linear scan to find the global best GPA. the second one loops over years 1 through 4 and for each year does another inner loop over all students, filtering by `year == yr`. it's clean and readable.

the one nasty bug that was in the original code here was:

```c
// wrong — overwrites gpa with old best, best never updates
gpa = best;

// correct — updates best when a higher gpa is found
best = gpa;
```

that kind of swap bug is easy to miss because both variables are floats and it compiles without complaint.

---

## Trade-offs In This Design

**good things:**

- memory scales with actual data. a year-1 student doesn't carry around 8 empty semester slots.
- the struct migration approach (copy + free) is safe and explicit. no silent corruption.
- functions are single-responsibility — calcGPA does one thing, findToppers does one thing.

**not-so-good things:**

- the heap is fragmented. every student has two separate malloc'd blocks for sem_res and books_issued floating around independently. if you have 1000 students, that's 2000 extra heap allocations on top of the main array. allocators don't love this.
- there's no input validation beyond year range. a student could have negative marks, a blank name, or an id of -999 and the program wouldn't care.
- `findToppers` recalculates GPA twice for every student in the year-wise loop (once in calcGPA, once in the outer call). for a small N it doesn't matter, but it's wasteful.

---

## How You Could Optimize This

**1. store gpa once (cache it)**

right now `calcGPA` is called multiple times per student. in `findToppers`, for N students across 4 years, some students get their GPA recalculated 4-5 times. a simple fix: calculate GPA for all N students once into a `float gpa_cache[N]` array before the loops, then just reference that. O(N) calculations instead of O(N \* years).

**2. contiguous memory for sem_res**

instead of each student having their own malloc'd `sem_res` block, you could allocate one big flat float array upfront (total size = sum of all sems across all students), and then just set each student's `sem_res` pointer to the right offset in that big block. this is called a "slab allocation" pattern and it's much better for cache performance because the data is contiguous.

```
instead of this (scattered):

  student0.sem_res --> [heap island A]
  student1.sem_res --> [heap island B]
  student2.sem_res --> [heap island C]

do this (contiguous slab):

  float *slab = malloc(total_sems * sizeof(float));
  student0.sem_res = slab + 0;
  student1.sem_res = slab + 2;   // year 1 = 2 sems
  student2.sem_res = slab + 6;   // year 2 = 4 sems
```

**3. sort students by GPA once**

if you needed to query toppers multiple times (say, top 3 per year), it would be worth sorting the array by GPA in descending order after computing all GPAs once. then finding toppers is just a single pass. right now it's O(N) per query, but with a sorted structure it's O(1) after the initial O(N log N) sort.
