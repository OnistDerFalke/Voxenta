#include "voxenta/effects/effect.h"
#include <imgui.h>

class effect_float_source final : public effect_clonable<effect_float_source> {
    float m_value = 0.5f;
    bool  m_has_range = false;
    float m_range_min = 0.0f;
    float m_range_max = 0.0f;
public:
    const char* get_name() override { return "Float"; }
    const char* get_description() override
    {
        return "Outputs a float value that can drive other nodes' parameters. "
            "Shows a slider when connected to a bounded parameter, otherwise "
            "a free-form input field.";
    }
    std::vector<pin_info> inputs()  const override { return {}; }
    std::vector<pin_info> outputs() const override { return { { "Value", pin_type::float_value } }; }

    void set_downstream_range(bool has_range, float min, float max) override
    {
        m_has_range = has_range;
        m_range_min = min;
        m_range_max = max;
    }

    bool run_ui() override
    {
        return effect::source_value_float("Value", &m_value, m_has_range, m_range_min, m_range_max);
    }

    std::vector<pin_value> run(const std::vector<pin_value>&) override
    {
        return { pin_value::make_float(m_value) };
    }
};

class effect_int_source final : public effect_clonable<effect_int_source> {
    int   m_value = 2;
    bool  m_has_range = false;
    float m_range_min = 0.0f;
    float m_range_max = 0.0f;
public:
    const char* get_name() override { return "Int"; }
    const char* get_description() override
    {
        return "Outputs an integer value that can drive other nodes' parameters. "
            "Shows a slider when connected to a bounded parameter, otherwise "
            "a free-form input field.";
    }
    std::vector<pin_info> inputs()  const override { return {}; }
    std::vector<pin_info> outputs() const override { return { { "Value", pin_type::int_value } }; }

    void set_downstream_range(bool has_range, float min, float max) override
    {
        m_has_range = has_range;
        m_range_min = min;
        m_range_max = max;
    }

    bool run_ui() override
    {
        return effect::source_value_int("Value", &m_value, m_has_range,
            static_cast<int>(m_range_min), static_cast<int>(m_range_max));
    }

    std::vector<pin_value> run(const std::vector<pin_value>&) override
    {
        return { pin_value::make_int(m_value) };
    }
};

class effect_bool_source final : public effect_clonable<effect_bool_source> {
    bool m_value = false;
public:
    const char* get_name() override { return "Bool"; }
    const char* get_description() override { return "Outputs a constant boolean value that can drive other nodes' parameters."; }
    std::vector<pin_info> inputs()  const override { return {}; }
    std::vector<pin_info> outputs() const override { return { { "Value", pin_type::bool_value } }; }
    bool run_ui() override { return ImGui::Checkbox("Value", &m_value); }
    std::vector<pin_value> run(const std::vector<pin_value>&) override
    {
        return { pin_value::make_bool(m_value) };
    }
};

#include "../effect_common.inl"
REGISTER_EFFECT(effect_float_source)
REGISTER_EFFECT(effect_int_source)
REGISTER_EFFECT(effect_bool_source)