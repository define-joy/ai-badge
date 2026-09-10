#include <badge/App.hpp>
#include <badge/Badge.hpp>

class HelloApp : public badge::App {
public:
    void onStart() override {
        badge::screen().showText("Hello Badge");
        (void)badge::haptic().pulse(80);
    }

    void onButton(badge::Button, badge::ButtonEvent ev) override {
        if (ev == badge::ButtonEvent::Click) {
            (void)badge::leds().next();
        }
    }
};

BADGE_APP(HelloApp);
