#include "input_img_window.h"

/* Creates ImGui window inside main window with the size given */
input_img_window::input_img_window(ImVec2 mws) {
    this->mws = mws;
}

/* Shows the window with a loaded input image */
void input_img_window::show(cv::Mat image, bool just_uploaded) {

    //Setting new position and size
    auto border = std::min(mws.x, mws.y) * 0.01f;
    auto input_window_pos = ImVec2(border,border);
    auto input_window_size = ImVec2((mws.x-3*border)/2,(mws.y-(3*border))*2/3);
    ImGui::SetNextWindowPos(input_window_pos);
    ImGui::SetNextWindowSize(input_window_size);

    //Setting context
    ImGui::Begin("Input image", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_ShowBorders);
    if(!image.empty()) {
        if(just_uploaded || need_update) {

            //Scaling the image
            cv::Mat scaled_image;
            int x, y;
            if(image.rows>=image.cols)
            {
                x = image.cols * (input_window_size.y)/image.rows;
                y = input_window_size.y;
            }
            else
            {
                x = input_window_size.x;
                y = image.rows * (input_window_size.x)/image.cols;
            }
            image_size = cv::Size(x,y);
            resize(image, scaled_image, image_size);
            image = scaled_image;
            image_size = cv::Size(x*0.95f,y*0.95f);
            resize(image, scaled_image, image_size);
            image = scaled_image;

            //Converting OpenCV Mat to OpenGL texture
            GLuint texture;
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows,
                         0, GL_RGB, GL_UNSIGNED_BYTE, image.ptr());
            image_texture = reinterpret_cast<void *>( static_cast<intptr_t>( texture ));

            need_update = false;
        }

        //Placing an image on input window rect
        auto input_image_pos = ImVec2((input_window_size.x - image_size.width)/2,
                                      (input_window_size.y - image_size.height)/2);
        ImGui::SetCursorPos(input_image_pos);
        ImGui::Image(image_texture, ImVec2(image_size.width, image_size.height));
    }
    ImGui::End();
}

/* Updates the window size */
void input_img_window::set_mws(ImVec2 size) {
    this->mws = size;
    need_update = true;
}

