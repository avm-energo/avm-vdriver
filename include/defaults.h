#pragma once

#include <QString>

namespace Defaults {
constexpr char logFileName[] = "avm-vdriver.log";
constexpr char settingsFileName[] = "avm-vdriver.conf";
constexpr char ip[] = "10.0.1.1";
constexpr int maxDevPortsCount = 6; // service + 5 working ports
}; // namespace Defaults
