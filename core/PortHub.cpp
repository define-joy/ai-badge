#include <core/PortHub.hpp>

namespace badge::core {
namespace {
PortHub g_hub;
}

PortHub& ports() { return g_hub; }

void attachPorts(const PortHub& hub) { g_hub = hub; }

}  // namespace badge::core
