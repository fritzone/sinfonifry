#ifndef COMMONS_H
#define COMMONS_H

/**
 * @brief The PLUGIN_COMPONENT enum has the types a plugin can return to tell
 *        the caller to which component this plugin belongs to (just to avoid
 *        the situation where the core component tries to load a client
 *        plugin for example).
 *
 *        PLUGIN_CLIENT - this is a client side plugin. This plugin is expected
 *                        to offer functionality as per the client plugin
 *                        requirements, ie. send back statistical data that will
 *                        be handled by the corresponding core level plugin.
 *
 *        PLUGIN_CORE - this is a core component. This is supposed to prepare
 *                      the data of the corresponding client plugin for further
 *                      visualization by the corresponding visualization plugin.
 *
 *        PLUGIN_VISUALIZATION - this a visualization plugin. In the current
 *                      system architecture this is a library that will be
 *                      loaded by the visualization part and should produce
 *                      data that is understandeable by the above layer.
 */
enum PLUGIN_COMPONENT
{
    PLUGIN_CLIENT         = 1,
    PLUGIN_CORE           = 2,
    PLUGIN_VISUALIZATION  = 3,

    PLUGIN_COMPONENT_LAST
};

/**
 * @brief The PLUGIN_LOAD_STATUS enum tells us the load status of the plugin:
 *
 *         PLUGIN_LOADED - The plugin succesfully initialized itself
 *                         and the system can continue starting normally. The
 *                         plugin will be added to the active plugins in the
 *                         system. An INFO level log entry will be created
 *                         with the name of the loaded plugin.
 *
 *         PLUGIN_LOADED_WITH_WARNINGS  - The plugin loaded, but some
 *                         warnings were recorded during the load sequence. In
 *                         this case the system will continue loading and the
 *                         pugin will be added to the active plugins in the
 *                         system and a log entry will appear with the value of
 *                         @see last_error() on the WARNING level
 *
 *         PLUGIN_LOAD_FAILED - The plugin failed to load. In this case
 *                         the plugin will not be added to the internal list of
 *                         active plugins and an ERROR level entry will be
 *                         added to the log file with the value of
 *                         @see last_error()
 *
 *         PLUGIN_LOAD_SYSTEM_FAILURE - in this case an ERROR level entry will
 *                         be added to the log file with the value of
 *                         @see last_error() and the system will interrupt
 *                         its normal startup, unload all the loaded plugins
 *                         and exit.
 *
 */
enum PLUGIN_LOAD_STATUS
{
    PLUGIN_LOADED               = 1,
    PLUGIN_LOADED_WITH_WARNINGS = 2,
    PLUGIN_LOAD_FAILED          = 3,
    PLUGIN_LOAD_SYSTEM_FAILURE  = 4,

    PLUGIN_LOAD_STATUS_LAST
};


/**
 * @brief The ALLOCATION_BEHAVIOR enum tells us how to deal with data returned
 *        from the plugins. It is typically used as an out parameter of the
 *        @see execute() method.
 *
 *        DO_NOT_FREE_ME - this tells us to leave the data alone, ie do not
 *                         try to free it. Typical usage situation where you
 *                         return a global static variable.
 *
 *        FREE_ME - the caller function liberates the data with the
 *                         C function free(). Typical usage situation is when
 *                         you have allocated the data with calloc() or
 *                         malloc and you want consistent behavior.
 *        DELETE_ME - the caller function liberates the allocated
 *                         memory with the delete [] C++ keyword. You want
 *                         to use this when you have allcoated the return
 *                         value with the new[] keyword.
 */
enum ALLOCATION_BEHAVIOR
{
    DO_NOT_FREE_ME = 1,
    FREE_ME        = 2,
    DELETE_ME      = 3,

    ALLOCATION_BEHAVIOR_LAST
};

/**
 * @brief The PLUGIN_UNLOAD_STATUS enum has codes to signal to the system the
 *        status of a plugins' unload process, when requested so by the system.
 *
 *        PLUGIN_UNLOAD_STATUS_PROCEED - the plugin succesfully unloaded itself.
 *                                The system shutdown can continue as planned,
 *                                INFO level log entry will come up in the log.
 *
 *        PLUGIN_UNLOAD_STATUS_TRY_AGAIN - the plugin did not unload itself yet,
 *                                the systems will try for once more to unload
 *                                it when all the plugins were unloaded (ie: it
 *                                puts this plugin at the end of the unload
 *                                queue and calls again this method). An INFO
 *                                level log entry will be added to the log.
 *
 *        PLUGIN_UNLOAD_STATUS_FAILED - the plugin was not succesfully unloaded.
 *                               The system will forcefully close the library
 *                               hosting this plugin and add an ERROR entry in
 *                               the log.
 */
enum PLUGIN_UNLOAD_STATUS
{
    PLUGIN_UNLOAD_STATUS_PROCEED     = 1,
    PLUGIN_UNLOAD_STATUS_TRY_AGAIN   = 2,
    PLUGIN_UNLOAD_STATUS_FAILED      = 3,

    PLUGIN_UNLOAD_STATUS_LAST
};

/**
 * @brief The PLUGIN_UNLOAD_REQUEST enum has the codes that are used when
 *        calling the @see unload() function
 *
 *        PLUGIN_UNLOAD_REQUEST_SHUTDOWN - the plugin is being unloaded due to
 *                                the sinfonifry system being shut down.
 *
 *        PLUGIN_UNLOAD_REQUEST_BY_REQUEST - the plugin is being unloaded due
 *                                to a manual request from the web gui
 *
 *        PLUGIN_UNLOAD_REQUEST_LAST_WARNIG - the plugin is being unloaded as
 *                                a result of a previous call of the @see
 *                                unload() method which returned the @see
 *                                PLUGIN_UNLOAD_STATUS_TRY_AGAIN value
 *
 */
enum PLUGIN_UNLOAD_REQUEST
{
    PLUGIN_UNLOAD_REQUEST_SHUTDOWN    = 1,
    PLUGIN_UNLOAD_REQUEST_BY_REQUEST  = 2,
    PLUGIN_UNLOAD_REQUEST_LAST_WARNIG = 3,

    PLUGIN_UNLOAD_REQUEST_LAST
};

#endif
