#include <badge/App.hpp>
#include <badge/Badge.hpp>

// 能力示例：震动外观。马达脚只存在 pinmap 里。
class HapticExample : public badge::App {
public:
    void onStart() override {
        badge::screen().showText("Haptic");
        (void)badge::haptic().pulse(100);
    }

    void onButton(badge::Button, badge::ButtonEvent ev) override {
        if (ev == badge::ButtonEvent::Click) {
            (void)badge::haptic().pattern(1);
        }
    }
};

BADGE_APP(HapticExample);
