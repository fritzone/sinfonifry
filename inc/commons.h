#ifndef COMMONS_H
#define COMMONS_H

/**
 * @brief The PLUGIN_COMPONENT enum has the types a plugin can return to tell
 *        the caller to which component this plugin belongs to (just to avoid
 *        the situation where the core component tries to load a client
 *        plugin for example).
 *
 *        @li @c PLUGIN_CLIENT - this is a client side plugin. This plugin is expected
 *                        to offer functionality as per the client plugin
 *                        requirements, ie. send back statistical data that will
 *                        be handled by the corresponding core level plugin.
 *
 *        @li @c PLUGIN_CORE - this is a core component. This is supposed to prepare
 *                      the data of the corresponding client plugin for further
 *                      visualization by the corresponding visualization plugin.
 *
 *        @li @c PLUGIN_VISUALIZATION - this a visualization plugin. In the current
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


#endif
