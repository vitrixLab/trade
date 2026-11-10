# Portfolio Snapshot Template

**As of:** <YYYY-MM-DD> <HH:MM> <Timezone> (UTC±HH:MM)  
**Status:** <user_reported / verified / unverified>  
**Instrument label:** <SYMBOL> (issuer, ticker and venue require authoritative verification)  
**Currency:** <CCY>

## Executed activity since prior snapshot

| Date | Action | Shares | Price | Gross | Fees | Net cash flow | Status |
|---|---|---:|---:|---:|---:|---:|---|
| <Date> | <BUY/SELL> | <n> | <price> | <amount> | <fees> | <flow> | <Filled/...> |

**Realized result:** <net P&L>, or <pct>% on estimated fee-inclusive cost basis. See [TRANSACTION_LEDGER.md](TRANSACTION_LEDGER.md) for full reconciliation.

## Position and open order state

| Item | Quantity / amount | Status |
|---|---:|---|
| Shares held | <n> | <status> |
| Average cost per share | <price> | Derived |
| Open buy orders | <n> | <status> |
| Open sell orders | <n> | <status> |

## Cash and planned capital

| Item | Amount |
|---|---:|
| Available cash | <amount> |
| Reserved for open orders | <amount> |
| Planned additional cash-in | <amount> |
| Stated buying power including planned cash-in | <amount> |

Planned cash-in is not deposited; exclude from current cash until credited.

## Market context supplied with this update

<Close, volume, range, order-book observations — mark unverified if screenshot-derived.>

## Research interpretation — not a trade recommendation

- <Interpretation bullets as hypotheses, not validated signals.>

## Data-quality and next-verification gates

1. Confirm instrument identity, issuer, ticker and trading venue from an authoritative source.
2. Reconcile all fills and exact charges against broker contract notes.
3. Confirm open orders and reserved funds against the broker account summary.
4. Confirm available cash and buying power from the broker; exclude planned deposits until credited.
5. Verify OHLCV and order-book observations from the exchange / authorized market data.
6. Verify any earnings or fundamental values from issuer / exchange disclosures; source consensus before using the term "beat."
7. Reassess candidate support/resistance levels only after subsequent price/volume confirmation.

**Evidence classification:** Transactions, order states, cash and market metrics are <user_reported / verified>; P&L arithmetic is derived from those inputs; market-structure and entry-zone statements are hypotheses. No trade authorization is implied.
