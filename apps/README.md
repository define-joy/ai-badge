# 玩法目录

用户只在这里写代码。每个子目录一个 `app.cpp`，用 `BADGE_APP(YourClass)` 注册。

```text
idf.py -DBADGE_APP=playground build
idf.py -DBADGE_APP=examples/buttons build
idf.py -DBADGE_APP=examples/haptic build
```

不要把引脚、驱动或 `app_main` 放进本目录。
