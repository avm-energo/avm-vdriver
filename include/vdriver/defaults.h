#pragma once

#include <QString>

namespace Defaults {
constexpr char logFileName[] = "/var/log/vdriver.log";
constexpr char configDirectory[] = "/usr/local/etc/vdriver/conf.d";
constexpr char settingsFileName[] = "/usr/local/etc/vdriver/vdriver.conf";
constexpr char ip[] = "10.0.1.1";
constexpr int maxDevPortsCount = 5;
}; // namespace Defaults
