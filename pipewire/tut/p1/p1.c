#include <pipewire/pipewire.h>

int main(int argc, char **argv) {
    pw_init(&argc, &argv);

    fprintf(stdout, "Compiled with libpipweire %s\n"
                    "Linked with libpipewire %s\n",
                    pw_get_headers_version(),
                    pw_get_library_version());
    return 0;
}
