#include <badge/App.hpp>
#include <badge/Badge.hpp>

// 能力示例：只关心按键钩子，不碰 GPIO / ADC。
class ButtonExample : public badge::App {
public:
    void onStart() override {
        badge::screen().showText("Press a button");
    }

    void onButton(badge::Button btn, badge::ButtonEvent ev) override {
        (void)btn;
        (void)ev;
        badge::screen().showText("button");
    }
};

BADGE_APP(ButtonExample);
