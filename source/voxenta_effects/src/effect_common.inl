#define REGISTER_EFFECT(effect_name) \
    namespace detail { static inline effect_name g_##effect_name; }
