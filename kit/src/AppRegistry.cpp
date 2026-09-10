#include <badge/App.hpp>

namespace badge::detail {
namespace {
AppFactory g_factory = nullptr;
}

void registerAppFactory(AppFactory factory) { g_factory = factory; }

App* createRegisteredApp() {
    if (!g_factory) return nullptr;
    return g_factory();
}

}  // namespace badge::detail
