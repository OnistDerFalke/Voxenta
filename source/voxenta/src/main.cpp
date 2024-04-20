#include "voxenta/windows/main_window.h"

#include "voxenta/effect_manager.h"

int main(int, char**)
{
#if defined(VOXENTA_EFFECTS_HOT_RELOAD)
    effect_manager::reload_effects();
#endif
    main_window();
    return 0;
}
