#include "path.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include "utility.h"

/**
 * Gets the site based on the string given.
 * Param:
 * site - string containing info about the site.
 * Returns the matching site, else returns SITE_ERROR_TYPE if a matching site
 *         could not be found.
 */
Site get_site(char* site) {
    if (strlen(site) != 3) {
        return SITE_ERROR_TYPE;
    }
    if (!strcmp(site, "::-")) {
        return BARRIER;
    }
    if (!isdigit(site[2]) || site[2] - '0' == 0) {
        return SITE_ERROR_TYPE;
    }
    char* sites[] = {"Mo", "V1", "V2", "Do", "Ri"};
    for (int i = 0; i < 5; i++) {
        char siteLabel[3];
        str_splice(siteLabel, site, 0, 2);
        if (!strcmp(siteLabel, sites[i])) {
            return i;
        }
    }
    return SITE_ERROR_TYPE;
}

/**
 * Extracts the site info from sites and fills in pathSites and
 *         sitePlayerCapacities.
 * Param:
 * playerCount - the number of players.
 * sites - contains the info about the sites of the path.
 * pathSites - array to be filled with sites based on site info.
 * sitePlayerCapacities - stores the number of players each site can hold.
 * Returns true if the extraction is successful, else false.
 */
bool extract_site_info(int playerCount, char* sites, Site* pathSites, 
        int* sitePlayerCapacities) {
    int capacityIndex = 2, siteNum = 0;
    for (int i = 0; i < (int) strlen(sites); i += SITE_LENGTH) {
        char siteString[SITE_LENGTH + 1];
        str_splice(siteString, sites, i, i + SITE_LENGTH);
        Site site = get_site(siteString);
        if (site != SITE_ERROR_TYPE) {
            pathSites[siteNum] = site;
        } else {
            return false;
        }
        if (site == BARRIER) {
            sitePlayerCapacities[siteNum] = playerCount;
        } else {
            sitePlayerCapacities[siteNum] = siteString[capacityIndex] - '0';
        }
        siteNum++;
    }
    return true;
}

/**
 * Creates a path based on the pathString.
 * Param:
 * pathString - contains the path info.
 * playerCount - the number of players.
 * Returns the path based on the pathString.
 */
Path* read_path(char* pathString, int playerCount) {
    if (pathString == NULL) {
        return NULL;
    }
    char* pathStringCopy = (char*) malloc(sizeof(char) * 
            (strlen(pathString) + 1));
    strcpy(pathStringCopy, pathString);
    // Get the number of sites
    char* invalidPart;
    char* numOfSitesString = strtok(pathStringCopy, ";");
    int numOfSites = strtol(numOfSitesString, &invalidPart, 10);
    if (invalidPart[0] != '\0' || numOfSites < 2) {
        free(pathStringCopy);
        return NULL;
    }

    // Check that the number of sites matches with the info given.
    char* sites = strtok(NULL, ";");
    if ((int) strlen(sites) % SITE_LENGTH != 0 || 
            numOfSites != ((int) strlen(sites)) / 3) {
        free(pathStringCopy);
        return NULL;
    }

    // Extract the site information
    Site* pathSites = (Site*) malloc(sizeof(Site) * numOfSites);
    int* sitePlayerCapacities = (int*) calloc(numOfSites, sizeof(int));
    if (!extract_site_info(playerCount, sites, pathSites, 
            sitePlayerCapacities)) {
        free(pathSites);
        free(sitePlayerCapacities);
        return NULL;
    }

    // Check that the path sites are valid
    if (pathSites[0] != BARRIER || pathSites[numOfSites - 1] != BARRIER) {
        free(pathSites);
        free(sitePlayerCapacities);
        return NULL;
    }

    Path* path = create_path(numOfSites, pathSites, sitePlayerCapacities);
    free(pathStringCopy);
    free(pathSites);
    free(sitePlayerCapacities);
    return path;
}

/**
 * Initialises the path based on the given parameters.
 * Param:
 * numOfSites - the number of sites on the path.
 * sites - the sites on the path.
 * sitePlayerCapacities - the number of players each site can hold.
 * Returns the initialised path.
 */
Path* create_path(int numOfSites, Site* sites, int* sitePlayerCapacities) {
    Path* path = (Path*) malloc(sizeof(Path));
    path->numOfSites = numOfSites;

    path->sites = (Site*) malloc(sizeof(Site) * numOfSites);
    path->sitePlayerCapacities = (int*) calloc(numOfSites, sizeof(int));

    for (int i = 0; i < numOfSites; i++) {
        path->sites[i] = sites[i];
        path->sitePlayerCapacities[i] = sitePlayerCapacities[i];
    }
    return path;
}

/**
 * Frees the given path.
 * Param:
 * path - the path to be freed.
 */
void free_path(Path* path) {
    free(path->sites);
    free(path->sitePlayerCapacities);
    free(path);
}

/**
 * Gets the siteString based on the site.
 * Param:
 * site - the site to get the string version of.
 * Returns the string form of the site.
 */
char* get_site_string(Site site) {
    switch (site) {
        case BARRIER:
            return "::";
        case MO:
            return "Mo";
        case V1:
            return "V1";
        case V2:
            return "V2";
        case DO:
            return "Do";
        case RI:
            return "Ri";
        case SI:
            return "Si";
        default:
            return NULL;
    }
}

/**
 * Gets the next barrier from the current site.
 * Param:
 * path - contains the path info.
 * currentSite - the site number of the current site.
 * Returns the site number of the next barrier site.
 */
int get_next_barrier(Path* path, int currentSite) {
    for (int i = currentSite + 1; i < path->numOfSites; i++) {
        if (path->sites[i] == BARRIER) {
            return i;
        }
    }
    return -1; // Should never reach here
}