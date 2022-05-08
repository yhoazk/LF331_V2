#include <pipewire/pipewire.h>

static void registry_event_global(void *data, uint32_t id,
                        uint32_t permissions, const char *type, uint32_t version,
                        const struct spa_dict *props){
    printf("Obj: id%u type:%s/%d\n", id, type, version);
}

static const struct pw_registry_events registry_events = {
    PW_VERSION_REGISTRY_EVENTS,
    .global = registry_event_global
};

int main(int argc, char **argv) {

    struct pw_main_loop *loop;
    struct pw_context *context;
    struct pw_core *core;
    struct pw_registry *registry;
    struct spa_hook registry_listener;
    // the init function loads and configures the modules and sets up the loggging
    pw_init(&argc, &argv);

    fprintf(stdout, "Compiled with libpipweire %s\n"
                    "Linked with libpipewire %s\n",
                    pw_get_headers_version(),
                    pw_get_library_version());
    // Pipewire implemets two types of mainloop
    // This mainloop is an abstraction of a bigg poll loop
    // waiting for events to occur and thigs to do
    // most of the actions are executed in this context
    loop = pw_main_loop_new( NULL /*properties*/ );
    // the main loop requires a context. This context will manage
    // the resources for us and will make it possible to connect
    // to the pipewire daemon
    context = pw_context_new(pw_main_loop_get_loop(loop),
                NULL, /*properties */
                0 /*user data size*/ );
    // With the context created now is possible to connect
    // with the daemon. This creates a socket between the
    // client and the server and makes a proxy obj with 
    // ID 0 for the core. 
    core = pw_context_connect(context, NULL, 0);
    if (NULL == core) {
        fprintf(stderr, "Error connecting to daemon\n");
    }
    // for now we're not going to handle events in this
    // core proxy but in the registry obj
    registry = pw_core_get_registry(core, PW_VERSION_REGISTRY, 0);

    spa_zero(registry_listener);
    // from the core we get the registry proxy obj and is used to listen for events
    // the spa_hook is used to keep track of the listener and a reference to the struct
    // pw_registry_events that contsins the vents we want to listent to.
    pw_registry_add_listener(registry, &registry_listener, &registry_events, NULL);
    //everythins is set, we can start the main loop
    pw_main_loop_run(loop);
    // since we dont call `pw_main_loop_quit` the loop never ends

    pw_proxy_destroy((struct pw_proxy*)registry);
    pw_core_disconnect(core);
    pw_context_destroy(context);
    pw_main_loop_destroy(loop);

    return 0;
}
