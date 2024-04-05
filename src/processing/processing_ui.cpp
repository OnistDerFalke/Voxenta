#include "processing/processing_ui.h"
#include "processing/effect.h"
#include "../../effects/effect_brightness.h"
#include "../../effects/effect_contrast.h"
#include "../../effects/effect_negative.h"
#include "../../effects/effect_grayscale.h"
#include "../../effects/effect_binarization.h"

#include <imgui.h>
#include <imgui_internal.h>

processing_ui::processing_ui() {
    // UI state needs to be stored every frame
    this->ui_state = containers::value_container();
    last_index = -1;

    // Initialize effects
    std::vector<effect*> effects;
    effects.push_back(new effect_brightness());
    effects.push_back(new effect_contrast());
    effects.push_back(new effect_negative());
    effects.push_back(new effect_grayscale());
    effects.push_back(new effect_binarization());

    // UI functions pointers, UI shows properties elements of the chosen effect index
    // Setting processing functions pointers, effect applied depends on chosen effect index
    for (auto& effect : effects) {
        functions.emplace_back([effect](bool* ff, containers::value_container* us) {
            return effect->pop_data(ff, us);
        });
    }
}

/* Runs the UI properties method that was assigned to index given */
containers::processing_data processing_ui::run_method(int index) {
    if (index != last_index)
        first_frame = true;
    last_index = index;

    auto& function_ptr = std::get<std::function<containers::processing_data(bool*, containers::value_container*)>>(functions[index]);
    return function_ptr(&first_frame, &ui_state);
}

