#include "voxenta/effects/effect.h"

std::vector<effect*> g_effects;

effect::effect() {
    g_effects.emplace_back(this);
}

extern "C" {
    __attribute__((visibility("default")))
    std::vector<effect*>* voxenta_effects() {
        return &g_effects;
    }
}
