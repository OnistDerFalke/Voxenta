#include "voxenta/effect_manager.h"

#include <dlfcn.h>

#if defined(VOXENTA_EFFECTS_HOT_RELOAD)
void* effect_manager::effects_lib_;
effect_list_t* effect_manager::effects_;
#endif

#if defined(VOXENTA_EFFECTS_HOT_RELOAD)
void effect_manager::reload_effects() {

    if (effects_lib_ != nullptr) { // todo: properly check whether library is loaded
        dlclose(effects_lib_);
    }

    effects_lib_ = dlopen("./libvoxenta_effects.so", RTLD_LAZY | RTLD_DEEPBIND);
    effects_ = static_cast<effect_list_t*>(dlsym(effects_lib_, "g_effects"));
}
#endif

const effect_list_t& effect_manager::effects() {
#if defined(VOXENTA_EFFECTS_HOT_RELOAD)
    return *effects_;
#else
    return g_effects;
#endif
}
