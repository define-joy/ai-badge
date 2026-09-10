// IDF 跳板。禁止在此写业务、引脚或 demo 菜单。
#include <core/Runtime.hpp>

extern "C" void app_main(void) {
    badge::core::Runtime::instance().run();
}
