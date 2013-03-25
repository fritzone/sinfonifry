#ifndef COMMONS_H
#define COMMONS_H

/**
 * @brief The PLUGIN_COMPONENT enum has the types a plugin can return to tell
 *        the caller to which component this plugin belongs to (just to avoid
 *        the situation where the core component tries to load a client
 *        plugin for example)
 */
enum PLUGIN_COMPONENT
{
    PLUGIN_CLIENT = 1,  /* this is a client plugin */
    PLUGIN_CORE = 2,
    PLUGIN_WEB = 3,

    PLUGIN_LAST
};

#endif
