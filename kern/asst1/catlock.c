/*
 * catlock.c
 *
 * 30-1-2003 : GWA : Stub functions created for CS161 Asst1.
 *
 * NB: Please use LOCKS/CV'S to solve the cat syncronization problem in
 * this file.
 */


/*
 *
 * Includes
 *
 */

#include <types.h>
#include <lib.h>
#include <test.h>
#include <thread.h>
#include <synch.h>


/*
 *
 * Constants
 *
 */

/*
 * Number of food bowls.
 */

#define NFOODBOWLS 2

/*
 * Number of times each animal will eat.
 */

#define NEAT 4

/*
 * Number of cats.
 */

#define NCATS 6

/*
 * Number of mice.
 */

#define NMICE 2

/*
 *
 * Animal Names
 *
 */
 
/*
 * Number of animal names.
 */

#define NANIMALNAMES 10

/*
 * Mouse names.
 */

const char* const mouse_names[NANIMALNAMES] = {"Danger Mouse", "Catnip", "Amelia", "Hank", "Basil", "Jerry", "Pikachu", "Nibbles", "Stuart Little", "Pinky"};

/*
 * Cat names.
 */

const char* const cat_names[NANIMALNAMES] = {"Tom", "Simba", "Pink Panther", "Hobbes", "Chester Cheetah", "Garfield", "Meowth", "Stimpy", "Bagheera", "Tigger"};

/*
 *
 * Global Variables
 *
 */

struct lock *bowl_lock[NFOODBOWLS], *eating_lock, *finished_lock;
int eating = 0;  // If +, cats eating = currently_eating. If -, mice eating = abs(currently_eating)
int finished = 0;

/*
 *
 * Function Definitions
 *
 */

/*
 * setup()
 *
 * Sets up the created locks.
 *
 * Returns:
 *      nothing.
 *
 */
static void setup() {
    int i;
    
    for (i = 0; i < NFOODBOWLS; i++) {
        bowl_lock[i] = lock_create("bowl_lock");
        assert(bowl_lock[i] != NULL);
    }
    
    eating_lock = lock_create("eating_lock");
    assert(eating_lock != NULL);
    
    finished_lock = lock_create("finished_lock");
    assert(finished_lock != NULL);
    
    eating = 0;
    finished = 0;
}

/*
 * cleanup()
 *
 * Cleans up the created locks.
 *
 * Returns:
 *      nothing.
 *
 */
static void cleanup() {
    int i;
    
    for (i = 0; i < NFOODBOWLS; i++) {
        lock_destroy(bowl_lock[i]);
    }
    lock_destroy(eating_lock);
    lock_destroy(finished_lock);
}

/*
 * eat()
 *
 * Arguments:
 *      char * animal: the type of animal eating.
 *      int num: the number of the animal eating.
 *      int iteration: the iteration of this animal eating.
 *
 * Returns:
 *      nothing.
 *
 */
static void eat(const char *animal, int num, int iteration) {
    int bowl = (int) (random() % NFOODBOWLS);
    lock_acquire(bowl_lock[bowl]);
    kprintf(
        "%s: %s starts eating at bowl %d, iteration %d\n",
        animal,
        (strcmp(animal, "cat") == 0) ? cat_names[num] : mouse_names[num],
        bowl + 1,
        iteration + 1
    );
    clocksleep(1);
    kprintf(
        "%s: %s ends eating at bowl %d, iteration %d\n",
        animal,
        (strcmp(animal, "cat") == 0) ? cat_names[num] : mouse_names[num],
        bowl + 1,
        iteration + 1
    );
    lock_release(bowl_lock[bowl]);
}

/*
 * catlock()
 *
 * Arguments:
 *      void * unusedpointer: currently unused.
 *      unsigned long catnumber: holds the cat identifier from 0 to NCATS -
 *      1.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Write and comment this function using locks/cv's.
 *
 */
static void catlock(void * unusedpointer, unsigned long catnumber) {
    (void) unusedpointer;
    
    int i;
    
    for (i = 0; i < NEAT;) {
        lock_acquire(eating_lock);
        // Test if no one is eating or just cats are eating
        if (eating >= 0 && eating < NFOODBOWLS) {
            eating++;
            lock_release(eating_lock);
            
            eat("cat", catnumber, i);
            
            lock_acquire(eating_lock);
            eating--;
            lock_release(eating_lock);
            
            i++;  // Completed one eating iteration
            clocksleep(5);  // Add digestion time to allow for frequent animal switching
        } else {
            lock_release(eating_lock);  // Cannot eat at this time
        }
        thread_yield();
    }
    
    lock_acquire(finished_lock);
    finished++;
    lock_release(finished_lock);
}


/*
 * mouselock()
 *
 * Arguments:
 *      void * unusedpointer: currently unused.
 *      unsigned long mousenumber: holds the mouse identifier from 0 to
 *              NMICE - 1.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Write and comment this function using locks/cv's.
 *
 */
static void mouselock(void * unusedpointer, unsigned long mousenumber) {
    (void) unusedpointer;
    
    int i;
    
    for (i = 0; i < NEAT;) {
        lock_acquire(eating_lock);
        // Test if no one is eating or just mice are eating
        if (eating <= 0 && eating * -1 < NFOODBOWLS) {
            eating--;
            lock_release(eating_lock);
            
            eat("mouse", mousenumber, i);
            
            lock_acquire(eating_lock);
            eating++;
            lock_release(eating_lock);
            
            i++;  // Completed one eating iteration
            clocksleep(5);  // Add digestion time to allow for frequent animal switching
        } else {
            lock_release(eating_lock);  // Cannot eat at this time
        }
        thread_yield();
    }
    
    lock_acquire(finished_lock);
    finished++;
    lock_release(finished_lock);
}


/*
 * catmouselock()
 *
 * Arguments:
 *      int nargs: unused.
 *      char ** args: unused.
 *
 * Returns:
 *      0 on success.
 *
 * Notes:
 *      Driver code to start up catlock() and mouselock() threads.  Change
 *      this code as necessary for your solution.
 */
int catmouselock(int nargs, char ** args) {
    int index, error;

    /*
     * Avoid unused variable warnings.
     */

    (void) nargs;
    (void) args;
    
    assert(NANIMALNAMES > NCATS)
    assert(NANIMALNAMES > NMICE)
    
    setup();

    /*
     * Start NCATS catlock() threads.
     */

    for (index = 0; index < NCATS; index++) {
        error = thread_fork("catlock thread", NULL, index, catlock, NULL);

        /*
         * panic() on error.
         */

        if (error) {
            panic("catlock: thread_fork failed: %s\n", strerror(error) );
        }
    }

    /*
     * Start NMICE mouselock() threads.
     */

    for (index = 0; index < NMICE; index++) {
        error = thread_fork("mouselock thread", NULL, index, mouselock, NULL);

        /*
         * panic() on error.
         */

        if (error) {
            panic("mouselock: thread_fork failed: %s\n", strerror(error));
        }
    }
    
    while (finished < NCATS + NMICE) {
        thread_yield();
    }
    
    cleanup();

    return 0;
}

/*
 * End of catlock.c
 */

