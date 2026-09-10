---
name: issue-suggestions
description: DefineJoy AI Badge 发布后，把开发者自己的改进点整理成 GitHub issue（define-joy/ai-badge）。仅在用户明确要求收集建议时使用。起草或提交前必须确认同意。
---

<p align="right">
  <strong>简体中文</strong> · <a href="SKILL.md">English</a>
</p>

# 收集建议并提单

把发布开发者自己遇到的改进点变成 `define-joy/ai-badge` 上的 issue。仅在开发者明确要求时运行。

## 安全门（先做）

1. 确认同意。拒绝则立刻停止。
2. 检查 GitHub：`gh auth status`，再用 `gh`。都不可用则给出可粘贴正文并停止。
3. 未经审阅不得提交。先草稿、展示、等待批准。
4. 禁止凭证、设备密钥、个人数据、未脱敏日志。

## 收集

kit API、Runtime、adapters、构建或文档上的痛点。不要把公开 issue 列表当来源。去重。创建前搜索 `define-joy/ai-badge` 是否已有同类 issue。

## 提交

批准后：`gh issue create --repo define-joy/ai-badge`（或第一个可用的 GitHub 通道）。读回已创建的 issue。

本 skill 不发布固件、不开代码 PR、不自动提交。
