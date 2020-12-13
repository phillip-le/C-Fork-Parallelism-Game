#ifndef PATH_H
#define PATH_H

#define SITE_LENGTH 3
#define UNOCCUPIED -1

/**
 * The types of sites on the path.
 */
typedef enum {  
    MO = 0,
    V1 = 1,
    V2 = 2,
    DO = 3,
    RI = 4,
    SI = 5,
    BARRIER = 6,
    SITE_ERROR_TYPE
} Site;

/**
 * Representation of the path.
 */
typedef struct {
    int numOfSites;
    Site* sites;
    int* sitePlayerCapacities;
} Path;

/**
 * Creates a path based on the pathString.
 * Param:
 * pathString - contains the path info.
 * playerCount - the number of players.
 * Returns the path based on the pathString.
 */
Path* read_path(char* pathString, int playerCount);

/**
 * Initialises the path based on the given parameters.
 * Param:
 * numOfSites - the number of sites on the path.
 * sites - the sites on the path.
 * sitePlayerCapacities - the number of players each site can hold.
 * Returns the initialised path.
 */
Path* create_path(int numOfSites, Site* sites, int* sitePlayerCapacities);

/**
 * Frees the given path.
 * Param:
 * path - the path to be freed.
 */
void free_path(Path* path);

/**
 * Gets the siteString based on the site.
 * Param:
 * site - the site to get the string version of.
 * Returns the string form of the site.
 */
char* get_site_string(Site site);

/**
 * Gets the next barrier from the current site.
 * Param:
 * path - contains the path info.
 * currentSite - the site number of the current site.
 * Returns the site number of the next barrier site.
 */
int get_next_barrier(Path* path, int currentSite);

#endif