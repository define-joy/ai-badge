---
name: experience-pr
description: DefineJoy AI Badge 发布后，把可复用经验沉淀为文档 PR。仅在用户明确要求收集经验时使用。先确认同意；不要在当前工作分支上直接提交，应开独立分支。
---

<p align="right">
  <strong>简体中文</strong> · <a href="SKILL.md">English</a>
</p>

# 收集经验并开文档 PR

把可长期复用的事实（硬件、API 契约、构建坑）写入 `docs/`，在独立分支上对 `define-joy/ai-badge` 提 PR。

## 安全门（先做）

1. 确认同意。拒绝则停止。
2. 检查 GitHub（`gh auth status`）。
3. 不要在开发者当前工作分支上 commit。新建分支（例如 `docs/experience-<topic>`）。
4. 展示 diff，得到明确批准后再 commit / push / `gh pr create`。
5. 禁止凭证与隐私数据。

## 收集

留下对下一版固件有用的内容：引脚定稿、Adapter 行为、Runtime 不变量、测试缺口。丢掉一次性调试笔记。

优先改 `docs/board-facts.md`、`docs/framework.md`、`adapters/board/bom_map.md`，不要另起一套规则。路由变化时同步 `AGENTS.md`。

## 提交

批准后用英文祈使句 Conventional Commit，push 到 `origin`，对 `define-joy/ai-badge` 的 `main` 开 PR。分开报告 Build / Host tests / Device tests / Unverified（纯文档则 NOT RUN 并说明原因）。
