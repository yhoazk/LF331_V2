#include <optional>

namespace ptp {
std::optional<int> create_socket(const char* dst_mac, size_t prio );

bool config_socket(int sfd, size_t idx);
}  // namespace ptp