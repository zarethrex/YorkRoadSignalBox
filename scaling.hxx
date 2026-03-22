#ifndef SCALING_HXX
#define SCALING_HXX

#include <QScreen>
#include <QGuiApplication>

namespace YRB {
    inline class Scaler {
        private:
            const float model_width_ = 540;
            const float model_height_ = 500;
            bool initialised_{false};
            int screen_width_;
            int screen_height_;
        public:
            Scaler() = default;
            void init() {
                if(auto *screen_ = QGuiApplication::primaryScreen()) {
                    if(initialised_) return;
                    const QRect screen_size_ = screen_->geometry();
                    screen_height_ = screen_size_.height()*0.98;
                    screen_width_ = (model_width_/model_height_)*screen_height_;
                    initialised_ = true;
                }
            }

            float scale_width(float width) const {
                return (screen_width_/model_width_)*width;
            }

            float scale_height(float height) const {
                return (screen_height_/model_height_)*height;
            }
            float screen_width() const {return screen_width_;}
            float screen_height() const {return screen_height_;}
    } SCALER;
};

#endif // SCALING_HXX
