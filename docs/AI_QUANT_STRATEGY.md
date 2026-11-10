# AI Quant Investment Strategy Framework

**Status:** Draft framework / research-only  
**Scope:** Replace with your instrument universe and approved venues  
**Decision authority:** Human investor only  
**Version:** 0.1.0

## 1. Mission

Build a traceable research system that combines global macro context, regional market conditions, company-level fundamentals, technical market structure, portfolio constraints, and major-event analysis into explainable investment scenarios.

The system produces research, not guaranteed predictions. It does not authorize or execute trades.

## 2. Analytical layers

1. **Global macro:** policy rates, inflation, employment, GDP, liquidity, sovereign yields, USD strength, energy/commodity prices, global risk appetite.
2. **Regional macro:** central bank policy, CPI, GDP, FX, government yields/debt, fiscal policy, remittances, trade, employment and liquidity.
3. **Market / sector:** exchange index and sector breadth, turnover, volatility, flows where available, sector catalysts and valuation regimes.
4. **Instrument:** issuer disclosures, earnings, cash flow, balance sheet, corporate actions, valuation, liquidity, price/volume structure and technical levels.
5. **Portfolio:** cost basis, exposure, concentration, cash, reserved funds, fees, taxes, pending orders, drawdown and risk budget.
6. **Event intelligence:** event chronology, expected transmission channels, actual market response, affected assets and post-event reassessment.

## 3. Evidence classification

Every material statement must be labelled:

- **FACT:** directly supported by a cited primary or reliable source.
- **ESTIMATE:** measured or calculated from identified inputs.
- **HYPOTHESIS:** plausible causal explanation not yet established.
- **MODEL OUTPUT:** result from a specified model, dataset and parameter set.
- **DECISION:** human-approved action or policy.
- **UNKNOWN:** insufficient or conflicting evidence.

Never convert a hypothesis or model output into a fact. Never infer causation from correlation alone.

## 4. Quant research pipeline

1. Ingest source data and preserve the original source reference.
2. Normalize timestamps, currency, units, ticker/instrument identifiers and corporate-action adjustments.
3. Validate freshness, missingness, duplicates, outliers and cross-source consistency.
4. Generate macro, fundamental, technical, sentiment/event and portfolio features.
5. Form competing scenarios: bullish, base, bearish and tail-risk.
6. Evaluate expected return, downside, volatility, liquidity, fees, taxes and slippage.
7. Test strategies out of sample and across regimes.
8. Record findings, confidence, invalidation conditions and unresolved questions.
9. Produce a human-readable investment memo.
10. Require explicit human approval before any trade is placed.

## 5. Strategy families to research

- Trend / momentum: moving-average structure, rate of change, relative strength.
- Mean reversion: deviation from a validated baseline, volatility and liquidity filters.
- Breakout / range: support/resistance, volume confirmation and false-breakout risk.
- Fundamental / value: earnings quality, cash flow, leverage, valuation and catalysts.
- Event-driven: policy decisions, earnings, corporate actions and macro surprises.
- Risk management: volatility targeting, position sizing, stop/invalidation rules and portfolio drawdown controls.

No strategy is presumed profitable. Each must have a written thesis, data requirements, entry/exit logic, transaction-cost assumptions, risk controls and test results.

## 6. Model governance

Each model record must include purpose, owner, version, training/test period, feature definitions, data lineage, assumptions, limitations, benchmark, evaluation metrics, drift checks and approval status.

Minimum evaluation: net returns after costs, maximum drawdown, volatility, Sharpe/Sortino where meaningful, hit rate, payoff ratio, turnover, exposure, benchmark-relative results and robustness by regime. Avoid look-ahead bias, survivorship bias, leakage and overfitting.

## 7. Trade-plan schema

A proposed plan must state:
- Instrument identity and venue, verified from an authoritative source.
- Timestamp and data freshness.
- Thesis and evidence references.
- Entry zone and trigger; do not use an unverified price.
- Invalidation / stop condition and gap risk.
- Targets and expected reward-to-risk.
- Position size and maximum loss.
- Fees, taxes, slippage and liquidity assumptions.
- Alternative scenarios and what would change the view.
- Human decision: pending / approved / rejected / executed.
- Post-trade review.

## 8. Safety boundaries

Research-only by default. No broker credentials, secrets or personal account identifiers in the repository. No autonomous order placement, cancellation, or modification. No claim of real-time data unless a connected source and retrieval timestamp prove it. This framework is educational and analytical, not a promise of performance or individualized financial advice.
