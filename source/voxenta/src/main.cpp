#include "voxenta/windows/main_window.h"

#include "voxenta/effect_manager.h"

int main(int, char**)
{
    effect_manager::reload_effects();
    main_window();
    return 0;
}
