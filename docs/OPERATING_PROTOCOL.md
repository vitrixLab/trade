# Research Operating Protocol

## Daily workflow
1. Capture portfolio state and pending orders from user-confirmed records.
2. Capture market close/open context and relevant global overnight developments.
3. Review new macro releases, central-bank actions, issuer disclosures and major events.
4. Validate data freshness and source quality.
5. Update market observations and event ledger.
6. Run approved strategy calculations and risk checks.
7. Write findings with competing scenarios and confidence.
8. Publish a concise decision memo: what changed, why it matters, what is unknown, and what to monitor.
9. Keep all trade actions under explicit human control.

## Weekly workflow
- Review global and regional macro regime.
- Review holdings, concentration, liquidity and pending orders.
- Evaluate model drift, forecast errors and strategy performance.
- Reconcile event hypotheses with observed market outcomes.
- Review stale or conflicting data and source availability.
- Produce a weekly synthesis and next-week event calendar.

## Monthly workflow
- Reconcile portfolio returns net of fees and taxes.
- Compare results with a suitable benchmark.
- Review drawdown, exposure, turnover and strategy stability.
- Reassess assumptions and invalidate weak hypotheses.
- Archive datasets, model versions and decision records.

## Data and security controls
- Never commit passwords, API keys, broker tokens, account numbers or identity documents.
- Use environment variables / secret managers for future integrations.
- Store only the minimum account data necessary; prefer redacted exports.
- Preserve original data and separately store normalized/derived datasets.
- Label all dates with timezone and all amounts with currency.
- No automated trading or broker write access without a separately reviewed authorization design.

## Output format for each synthesis
1. Executive summary
2. What changed since last review
3. Global drivers
4. Regional drivers
5. Instrument / sector analysis
6. Portfolio and cash constraints
7. Bull / base / bear scenarios
8. Risks, invalidation points and unknowns
9. Research-backed plan and monitoring triggers
10. Sources, timestamps and confidence
