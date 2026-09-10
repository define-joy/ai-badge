# DefineJoy AI Badge

DefineJoy 的 ESP32-S3 玩法开发底座。硬件引脚和驱动藏在适配层，用户只写钩子和外观 API。

先读 [docs/framework.md](docs/framework.md)。给 AI 助手的入口是 [AGENTS.md](AGENTS.md)。

License: [MIT](LICENSE) © 2026 DefineJoy.

```text
kit/include/badge/   用户 API（App + Facade）
core/                大循环 / 事件泵 / 电源
ports/               无 GPIO 的接口
adapters/            板级实现（含占位引脚）
apps/                玩法；默认 playground
boot/main/           IDF 跳板（不要写业务）
tests/host/          不链 ESP-IDF 的生命周期测试
```

Host 测试（不需要 ESP-IDF）：

```text
cmake -S tests/host -B tests/host/build
cmake --build tests/host/build
./tests/host/build/badge_host_tests
```
