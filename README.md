# Trading Analysis Engine & Research Workspace

A research-first workspace for recording portfolio state, market data, major events, hypotheses, quantitative findings, and evidence-backed investment plans. Replace the `<PLACEHOLDER>` values below with your instrument, region, currency, and venue details.

## Operating principles
- Research and analysis are separate from trade authorization and execution.
- Every observation must carry a source, timestamp, timezone, retrieval time, and confidence.
- Distinguish facts, estimates, hypotheses, model outputs, and decisions.
- Never treat stale, missing, or unverified data as current.
- Include fees, taxes, slippage, liquidity, and position sizing in all trade analysis.
- Backtests must account for survivorship bias, look-ahead bias, corporate actions, and transaction costs.
- AI-generated findings require human review; no autonomous order placement is authorized by this repository.

## Directory layout
- `docs/` — operating protocol, strategy framework, data-source registry, event ledger, market observation template, research findings log, portfolio and transaction ledger templates.
- `schemas/` — JSON Schema contracts for events, market bars, macro observations, portfolio snapshots, and research findings.
- `examples/` — illustrative sample fixtures (unverified; do not treat as market data).
- `config/` — data-provider registry example (no secrets).
- `api/` — OpenAPI contract for the read-only intelligence API.
- `include/manifest/` and `src/` — C++20 manifest/foundation engine (file hashing, repository scanning, git provenance).
- `tests/` — GoogleTest suite for the foundation engine.

## Status
Initial repository bootstrap. No live market feed, broker connection, or automated trading capability is implied.
