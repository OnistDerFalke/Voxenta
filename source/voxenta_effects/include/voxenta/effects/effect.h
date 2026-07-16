#ifndef VOXENTA_EFFECT_H
#define VOXENTA_EFFECT_H

#include <memory>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

enum class pin_type {
    image,
    float_value,
    int_value,
    bool_value
};

struct pin_info {
    const char* name;
    pin_type    type;
    bool        has_range = false;
    float       min = 0.0f;
    float       max = 0.0f;
};

struct pin_value {

    pin_type type = pin_type::image;
    bool connected = false;

    cv::Mat image;
    float f = 0.0f;
    int i = 0;
    bool b = false;

    static pin_value make_image(cv::Mat m) { 
        pin_value v; v.type = pin_type::image;       
        v.image = std::move(m); 
        return v; 
    }

    static pin_value make_float(float val) { 
        pin_value v; v.type = pin_type::float_value;
        v.f = val; 
        return v; 
    }

    static pin_value make_int(int val) { 
        pin_value v; 
        v.type = pin_type::int_value;
        v.i = val; 
        return v; 
    }

    static pin_value make_bool(bool val) { 
        pin_value v; 
        v.type = pin_type::bool_value;   
        v.b = val; 
        return v; 
    }
};

class effect {
public:
    virtual ~effect() = default;
    effect();

    virtual const char* get_name() = 0;
    virtual const char* get_description() = 0;
    virtual bool run_ui() = 0;

    virtual std::vector<pin_info> inputs() const { return { { "Image", pin_type::image } }; }
    virtual std::vector<pin_info> outputs() const { return { { "Image", pin_type::image } }; }

    virtual bool is_internal_node() const { return false; }
    virtual std::vector<pin_value> run(const std::vector<pin_value>& inputs) = 0;
    virtual std::unique_ptr<effect> clone() const = 0;
    virtual void set_external_image(const cv::Mat&) {}
    virtual cv::Mat get_preview_image() const { return m_last_output_image; }
    virtual void set_downstream_range(bool has_range, float min, float max) {}

    static std::string describe_image(const cv::Mat& img);

protected:
    static void convert_to_rgb(cv::Mat* img);

    void remember_inputs(const std::vector<pin_value>& in) { m_last_inputs = in; }
    bool  pin_connected(size_t i) const { return i < m_last_inputs.size() && m_last_inputs[i].connected; }
    float pin_float(size_t i)     const { return i < m_last_inputs.size() ? m_last_inputs[i].f : 0.0f; }
    int   pin_int(size_t i)       const { return i < m_last_inputs.size() ? m_last_inputs[i].i : 0; }
    bool  pin_bool(size_t i)      const { return i < m_last_inputs.size() ? m_last_inputs[i].b : false; }

    void remember_output(const std::vector<pin_value>& out)
    {
        for (const auto& v : out) {
            if (v.type == pin_type::image) { m_last_output_image = v.image; return; }
        }
    }

    static bool param_slider_float(const char* label, float* value, float min, float max, bool connected, float live_value);
    static bool param_slider_int(const char* label, int* value, int min, int max, bool connected, int live_value);
    static bool param_checkbox(const char* label, bool* value, bool connected, bool live_value);
    static bool source_value_float(const char* label, float* value, bool has_range, float min, float max);
    static bool source_value_int(const char* label, int* value, bool has_range, int min, int max);

private:
    std::vector<pin_value> m_last_inputs;
    cv::Mat m_last_output_image;
};

template<typename Derived>
class effect_clonable : public effect {
public:
    std::unique_ptr<effect> clone() const override
    {
        return std::make_unique<Derived>(static_cast<const Derived&>(*this));
    }
};

using effect_list_t = std::vector<std::reference_wrapper<effect>>;

#if !defined(VOXENTA_EFFECTS_HOT_RELOAD)
extern effect_list_t g_effects;
#endif

#endif