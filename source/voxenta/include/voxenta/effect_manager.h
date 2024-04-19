#ifndef VOXENTA_EFFECT_MANAGER_H
#define VOXENTA_EFFECT_MANAGER_H

#include "voxenta/effects/effect.h"

class effect_manager {
public:
    effect_manager() = delete;
#if defined(VOXENTA_EFFECTS_HOT_RELOAD)
    static void reload_effects();
#endif
    static const effect_list_t& effects();
private:
#if defined(VOXENTA_EFFECTS_HOT_RELOAD)
    static void* effects_lib_;
    static effect_list_t* effects_;
#endif
};

#endif //VOXENTA_EFFECT_MANAGER_H
