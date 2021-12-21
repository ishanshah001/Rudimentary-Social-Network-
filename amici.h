/// @file amici.h
/// Description Header file for amici.c
///             Module that represents amici
/// @author Ishan Shah is4761
///

#include "table.h"

// structure for person
typedef struct person_s {
    char *first_name;                 ///< first name of the person
    char *last_name;                 ///< last name of the person
    char *handle;               ///< handle of the person
    struct person_s **friends;  ///< dynamic collection of friends
    size_t friend_count;        ///< current number of friends 
    size_t max_friends;         ///< current limit on friends
}person_t;



// Delete the entire contents of the system, releasing all dynamic storage; 
// Re-initializes to an empty network by making number of people and number of friendships as 0
// Prints a new prompt.
// initialises a new hash table
void init_table(void);


// Print two values: the size of the network and the number of friendships
// if handles alpha42 and beta991 are friends, that should be counted as one friendship even though 
// each one appears in the other's list of friendships
// Format: Statistics: 1 person, 1 friendship (for one of each)
//     or: Statistics: N people, N friendships (for 0 or 2+ of each)
void stats();



// Add the specified user having the indicated first and last names to the database with the specified handle. 
// Handles must be unique; names, however, may be duplicated.
// If any arguments are missing, or if there are too many arguments, prints a usage message and reissues the command prompt.
// If the handle is found to be already in the system, it reports the error.
// @pre: The names and handle must be non-null and non-empty.
// @param: fname: first name of user
// @param: lname: last name of user
// @param: handle: handle of user
// @param: table: hash table in which the user data is stored
// returns 0 if successfully added
// returns 1 in case of an error
int add(char *fname, char *lname, char *handle, Table table);


// Checks if 2 users are friends are not
// Called by make_friend and unfriend function for error checking
// @pre: Both users are present in the network
// @param: first: first user
// @param: second: second user
// returns true if the 2 users are firends
// returns false if the 2 handles are not friends
bool check_friend(person_t *first, person_t *second);


// Counts the number of existing friendships for the specified user, and prints that.
// If the specified handle is not found, it reports the error.
// @pre: The handle must be non-null and non-empty.
// @param: handle: the handle whose data needs to be printed
// @param: table: the hash table in which data is stored
// returns 0 if no error
// returns 1 in case of error
int size(char *handle, Table table);


// Create a friendship between the two users identified by the indicated handles. 
// The handles must both exist, must be different, and there must not already be a friendship between these users.
// @pre: Both handles must be non-null and non-empty.
// @param: handle1: handle of first user
// @param: handle2: handle of second user
// @param: table: hash table in which the user data is stored and needs to be updated
// returns 0 if successfully made friends
// returns 1 in case of error
int make_friend(char *handle1, char *handle2, Table table);


// Dissolve the friendship that exists between the specified users.
// The two handles must exist, and there must be a friendship between the users.
// @pre: Both handles must be non-null and non-empty.
// @param: handle1: handle of first user
// @param: handle2: handle of second user
// @param: table: hash table in which the user data is stored and needs to be updated
// returns 0 if successfully dissolved the friendship
// returns 1 in case of error
int unfriend(char *handle1, char *handle2, Table table);


// Finds the entry for the specified user, and prints the user's name and handle, followed by a list of the user's current friendships.
// The specified handle must be in the system.
// @pre: The handle must be non-null and non-empty.
// @param: handle: handle of the user whose data needs to be printed
// @param: table: hash table in which the user data is stored and needs to be printed
// returns 0 idf successfully executed
// returns 1 in case of error
int print_person(char *handle, Table table);

