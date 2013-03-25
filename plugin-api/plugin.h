#ifndef PLUGIN_H
#define PLUGIN_H

/**
 * The definition of a plugins' exposed interface towards the application. This
 * header file contains all the methods that the plugins of the components
 * should expose to the outside world, but you need to implement only the
 * methods that are relevant to the plugin you develop.
 *
 * For Client plugins the following methods should be implemented:
 *
 *  1. @see int load();
 *
 *  2. @see PLUGIN_COMPONENT component();
 *     Always should return PLUGIN_CLIENT
 *
 *  3. @see const char* execute();
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "commons.h"

enum ALLOCATION_BEHAVIOR
{
    DO_NOT_FREE_ME = 0,
    FREE_ME = 1,

    LAST
};

/**
 * @brief execute - this is the method which does the real work of the
 *                  plugin, (ie: it gathers the data this plugin is supposed
 *                  to send back to the server, formats it in a way that the
 *                  core components plugin understands it and returns it).
 *                  Always return dynamically allocated memory with malloc
 *                  or calloc the caller of this method IS FREEING the returned
 *                  value with free if you set the *free_returned_value to 1.
 *
 * @param free_returned_value - whether to free the returned value or not.
 *
 * @return - the gathered data formatted for the core side component. If this
 *           return NULL or empty string it is ignored by the caller.
 */
char* execute(ALLOCATION_BEHAVIOR* free_returned_value);

/**
 * @brief component returns the @see PLUGIN_COMPONENT of this plugin
 * @return the @see PLUGIN_COMPONENT of this plugin
 */
PLUGIN_COMPONENT component();

/**
 * @brief load - loads the
 * @return
 */
int load();

// called when the system goes down.
void unload();

// an internal about string. User should NOT free the data returned from here
const char* about();

// the name of the plugin. User should NOT free this
const char* name();

// does some setup work. Plugin dependent
int setup(const char*);

/*****************************************************************************
 *             Typedefs used for the function pointers                       *
 *****************************************************************************/

/* typedef for the execute(ALLOCATION_BEHAVIOR*) function */
typedef char* (*PEXECUTE)(ALLOCATION_BEHAVIOR*);


/*
 * Data below is used by the components to identify the plugin structure
 */

/**
 * @brief The plugin struct represents
 */
struct plugin_descriptor
{
    // the name of the plugin
    char* name;

    // the name of the library this plugin resides in
    char* lib_name;

    // the handle of the library
    void* lib_handle;

    // the function pointer which returns the current data of the plugin

    PEXECUTE execute;

};


#ifdef __cplusplus
}
#endif

#endif // PLUGIN_H
