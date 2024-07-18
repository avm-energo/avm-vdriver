#pragma once

#include <QString>

namespace Defaults {
#ifdef LOCALDEBUG
constexpr char logDirectory[] = "/home/anton/.local/share/vdriver";
constexpr char configDirectory[] = "/home/anton/.config/vdriver/conf.d";
constexpr char logFileName[] = "/home/anton/.local/share/vdriver/vdriver.log";
constexpr char settingsFileName[] = "/home/anton/.config/vdriver/vdriver.conf";
#else
constexpr char logDirectory[] = "/var/log";
constexpr char logFileName[] = "/var/log/vdriver.log";
constexpr char configDirectory[] = "/usr/local/etc/vdriver/conf.d";
constexpr char settingsFileName[] = "/usr/local/etc/vdriver/vdriver.conf";
#endif
constexpr char ip[] = "10.0.1.1";
constexpr int maxDevPortsCount = 5;
}; // namespace Defaults
