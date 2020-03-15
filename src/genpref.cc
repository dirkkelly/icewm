#include "config.h"
#include "base.h"
#include "ascii.h"
#include "appnames.h"

char const *ApplicationName;

#define CFGDESC
#include "ykey.h"
#include "sysdep.h"

void addWorkspace(const char *, const char *, bool) {}
void setLook(const char *, const char *, bool) {}
void addBgImage(const char *, const char *, bool) {}

//#include "bindkey.h"
//#include "default.h"
#define CFGDEF
#define GENPREF
#include "yprefs.h"
#include "bindkey.h"
#include "default.h"
#include "themable.h"
#include "icewmbg_prefs.h"

void show(cfoption *options) {
    for (unsigned int i = 0; options[i].type != cfoption::CF_NONE; i++) {
        if (nonempty(options[i].description))
            printf("#  %s\n", options[i].description);

        switch (options[i].type) {
        case cfoption::CF_BOOL:
            printf("# %s=%d # 0/1\n",
                   options[i].name, options[i].boolval());
            break;
        case cfoption::CF_INT:
            printf("# %s=%d # [%d-%d]\n",
                   options[i].name, *options[i].v.i.int_value,
                   options[i].v.i.min, options[i].v.i.max);
            break;
        case cfoption::CF_UINT:
            printf("# %s=%u # [%u-%u]\n",
                   options[i].name, *options[i].v.u.uint_value,
                   options[i].v.u.min, options[i].v.u.max);
            break;
        case cfoption::CF_STR:
            printf("# %s=\"%s\"\n", options[i].name, Elvis(options[i].str(), ""));
            break;
        case cfoption::CF_KEY:
            printf("# %s=\"%s\"\n", options[i].name, options[i].key()->name);
            break;
        case cfoption::CF_FUNC:
            if (0 == strcmp("WorkspaceNames", options[i].name)) {
                printf("WorkspaceNames=\" 1 \", \" 2 \", \" 3 \", \" 4 \"\n");
            }
            else if (0 == strcmp("Look", options[i].name)) {
                char look[16] = QUOTE(CONFIG_DEFAULT_LOOK);
                look[4] = ASCII::toLower(look[4]);
                printf("# %s=\"%s\"\n", options[i].name, look + 4);
            }
            else {
                printf("# %s=\"\"\n", options[i].name);
            }
            break;
        case cfoption::CF_NONE:
            break;
        }
        if (options[i].description)
            puts("");
    }
}

static void genpref()
{
    printf("# %s preferences(%s) - generated by genpref\n\n", PACKAGE, VERSION);
    printf("# This file should be copied to %s or $HOME/.icewm/\n", CFGDIR);
    printf("# NOTE: All settings are commented out by default.\n"
           "# Be sure to uncomment them if you change them!\n"
           "\n");

    const unsigned wmapp_count = ACOUNT(wmapp_preferences);
    wmapp_preferences[wmapp_count - 2] = wmapp_preferences[wmapp_count - 1];
    show(wmapp_preferences);

    show(icewm_preferences);

    printf("# -----------------------------------------------------------\n"
           "# Themable preferences. Themes will override these.\n"
           "# To override the themes, place them in ~/.icewm/prefoverride\n"
           "# -----------------------------------------------------------\n\n");

    show(icewm_themable_preferences);

    printf("\n"
           "#\n"
           "# icewmbg preferences\n"
           "#\n"
           "# IMPORTANT: To set the background run icewmbg before icewm.\n"
           "#\n"
           "\n"
           );
    show(icewmbg_prefs);

    fflush(stdout);
}

int main(int argc, char **argv)
{
    const char help[] =
        "  -o, --output=FILE   Write preferences to FILE.\n";

    check_argv(argc, argv, help, VERSION);

    char* output = 0;
    for (char **arg = argv + 1; arg < argv + argc; ++arg) {
        if (**arg == '-') {
            char *value(0);
            if (GetArgument(value, "o", "output", arg, argv + argc)) {
                output = value;
            }
            else {
                warn("Unrecognized option '%s'.", *arg);
            }
        }
    }
    if (output) {
        if (0 == freopen(output, "w", stdout)) {
            fail("%s", output);
            exit(1);
        }
    }

    genpref();

    return 0;
}

// vim: set sw=4 ts=4 et:
