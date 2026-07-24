/*
 *   Student Record Management System
 *
 *   goes in:
 *   2a - dynamic allocation for N students (malloc)
 *   2b - extending student data with books issued (realloc)
 *   2c - GPA calculation via pointer-based function
 *   2d - overall and year-wise topper identification
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -------------------------------------------------------
 * part 2a: basic student structure
 * sem_res is a dynamic array - size depends on year
 * (year 1 = 2 sems, year 2 = 4 sems, and so on)
 * ------------------------------------------------------- */

struct Student
{
    int id;
    char name[50];
    int year;
    float *sem_res;
};

/* -------------------------------------------------------
 * part 2b: extended structure - same fields + books_issued
 * books_issued is also dynamic (one entry per semester)
 * ------------------------------------------------------- */

struct NewStudent
{
    int id;
    char name[50];
    int year;
    float *sem_res;
    int *books_issued;
};

/* -------------------------------------------------------
 * part 2c: calculate GPA for a student
 * takes a pointer to NewStudent, returns average of results
 * ------------------------------------------------------- */

float calcGPA(struct NewStudent *s)
{
    int sems = s->year * 2;
    float total = 0.0;

    for (int i = 0; i < sems; i++)
        total += *(s->sem_res + i);

    return total / sems;
}

/* -------------------------------------------------------
 * part 2d: Find overall topper + year-wise toppers
 * ------------------------------------------------------- */

void findToppers(struct NewStudent *s, int n)
{
    /*  overall topper  */
    float best_gpa = -1.0;
    int top_idx = -1;

    for (int i = 0; i < n; i++)
    {
        float g = calcGPA(s + i);
        if (g > best_gpa)
        {
            best_gpa = g;
            top_idx = i;
        }
    }

    printf("\n<<------------ TOPPER RESULTS ------------>>\n");
    printf("Overall Topper : %s  |  GPA : %.2f\n",
           (s + top_idx)->name, best_gpa);

    /* Year-wise toppers  */

    printf("\nYear-wise Toppers:\n");
    printf("------------------------------------\n");

    for (int yr = 1; yr <= 4; yr++)
    {
        float yr_best = -1.0;
        int yr_idx = -1;

        for (int i = 0; i < n; i++)
        {
            if ((s + i)->year == yr)
            {
                float g = calcGPA(s + i);
                if (g > yr_best)
                {
                    yr_best = g;
                    yr_idx = i;
                }
            }
        }

        if (yr_idx != -1)
        {
            printf("Year %d  -->  %s  |  GPA : %.2f\n",
                   yr, (s + yr_idx)->name, yr_best);
        }
    }
}

/* -------------------------------------------------------
 * helper: print a single student's record
 * ------------------------------------------------------- */

void printStudent(struct NewStudent *s)
{
    int sems = s->year * 2;

    printf("\n  ID            : %d", s->id);
    printf("\n  Name          : %s", s->name);
    printf("\n  Year          : %d", s->year);

    printf("\n  Sem Results   : ");
    for (int j = 0; j < sems; j++)
        printf("%.2f  ", *(s->sem_res + j));

    printf("\n  Books Issued  : ");
    for (int j = 0; j < sems; j++)
        printf("%d  ", *(s->books_issued + j));

    printf("\n  GPA           : %.2f\n", calcGPA(s));
}

/* ======================================================= */
int main()
{
    int n;
    printf("Enter number of students: ");
    scanf("%d", &n);

    /* ---------------------------------------------------
     * PART 2a: malloc N students
     * --------------------------------------------------- */
    struct Student *s = (struct Student *)malloc(n * sizeof(struct Student));

    if (s == NULL)
    {
        printf("malloc failed. Exiting.\n");
        return 1;
    }

    for (int i = 0; i < n; i++)
    {
        printf("\n--- Enter details for Student %d ---\n", i + 1);

        printf("  ID    : ");
        scanf("%d", &(s + i)->id);

        printf("  Name  : ");
        scanf("%s", (s + i)->name);

        printf("  Year (1-4) : ");
        scanf("%d", &(s + i)->year);

        /* validate year range */
        while ((s + i)->year < 1 || (s + i)->year > 4)
        {
            printf("  Invalid year! Enter 1-4 : ");
            scanf("%d", &(s + i)->year);
        }

        int sems = (s + i)->year * 2;

        /* dynamically allocate semester results based on year */
        (s + i)->sem_res = (float *)malloc(sems * sizeof(float));

        if ((s + i)->sem_res == NULL)
        {
            printf("malloc failed for sem_res. Exiting.\n");
            free(s);
            return 1;
        }

        printf("  Enter %d semester result(s):\n", sems);
        for (int j = 0; j < sems; j++)
        {
            printf("    Sem %d : ", j + 1);
            scanf("%f", (s + i)->sem_res + j);
        }
    }

    /* ---------------------------------------------------
     * PART 2b: realloc - migrate to NewStudent struct
     *
     * we can't simply realloc a struct Student array into
     * a struct NewStudent array directly because the two
     * structs have different sizes, so the per-element
     * offsets shift for N > 1.
     *
     * right approach: allocate a fresh NewStudent block,
     * copy the existing Student data over, then free the
     * old block. This is effectively what realloc does
     * internally - allocate + copy + free - but done here
     * manually to handle the type size change safely.
     * ---------------------------------------------------*/

    struct NewStudent *snew =
        (struct NewStudent *)malloc(n * sizeof(struct NewStudent));

    if (snew == NULL)
    {
        printf("realloc step failed. Exiting.\n");
        /* clean up sem_res before exit */
        for (int i = 0; i < n; i++)
            free((s + i)->sem_res);
        free(s);
        return 1;
    }

    /* copy existing Student data into NewStudent slots */
    for (int i = 0; i < n; i++)
    {
        (snew + i)->id = (s + i)->id;
        (snew + i)->year = (s + i)->year;
        (snew + i)->sem_res = (s + i)->sem_res; /* pointer transfer */
        strcpy((snew + i)->name, (s + i)->name);
        (snew + i)->books_issued = NULL; /* not filled yet */
    }

    /* old Student array freed; sem_res pointers now owned by snew */
    
    free(s);
    s = NULL;

    /* now fill the new field: books_issued per semester */
    
    for (int i = 0; i < n; i++)
    {
        int sems = (snew + i)->year * 2;

        (snew + i)->books_issued = (int *)malloc(sems * sizeof(int));

        if ((snew + i)->books_issued == NULL)
        {
            printf("malloc failed for books_issued. Exiting.\n");
            return 1;
        }

        printf("\nBooks issued for %s (enter per semester):\n",
               (snew + i)->name);

        for (int j = 0; j < sems; j++)
        {
            printf("  Sem %d : ", j + 1);
            scanf("%d", (snew + i)->books_issued + j);
        }
    }

    /* ---------------------------------------------------
     * print all student records
     * --------------------------------------------------- */
    printf("\n\n========== STUDENT RECORDS ==========\n");

    for (int i = 0; i < n; i++)
    {
        printf("\nStudent %d:", i + 1);
        printStudent(snew + i);
    }

    /* ---------------------------------------------------
     * PART 2d: find toppers
     * --------------------------------------------------- */
    findToppers(snew, n);

    /* ---------------------------------------------------
     * free all dynamically allocated memory
     * --------------------------------------------------- */
    for (int i = 0; i < n; i++)
    {
        free((snew + i)->sem_res);
        free((snew + i)->books_issued);
    }
    free(snew);

    return 0;
}