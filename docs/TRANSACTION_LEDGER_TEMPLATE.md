# Trading Ledger Template — <YYYY-MM-DD>

**Record date:** <YYYY-MM-DD> (<Timezone>)  
**Recorded at:** <YYYY-MM-DD> <HH:MM> <Timezone> (UTC±HH:MM)  
**Instrument label:** <SYMBOL> (issuer, ticker and venue still require authoritative verification)  
**Currency:** <CCY>  
**Evidence status:** USER_REPORTED — transcribed from user-supplied records; not independently broker-verified.

## 1. Executed transactions

| Date | Side | Quantity | Price/share | Gross amount | Fees | Net cash flow | Reported status |
|---|---:|---:|---:|---:|---:|---:|---|
| <Date> | <BUY/SELL> | <n> | <price> | <amount> | <fees> | <flow> | <Filled/...> |

### Realized P&L reconciliation

- Gross purchase value: <amount>
- Purchase fees: <amount>
- Total cost basis including reported fees: <amount>
- Gross sale value: <amount>
- Selling fees: <amount>
- Net sale proceeds: <amount>
- Realized net P&L: <amount>
- Net ROI on fee-inclusive cost basis: <pct>%

The first-buy fee is approximate where marked ~; therefore total cost basis, net P&L and ROI are approximate to that extent.

## 2. Orders not executed

| Posted | Side | Quantity | Limit | Estimated total cost | Time in force / expiry | Reported status |
|---|---:|---:|---:|---:|---|---|
| <Date> | <BUY/SELL> | <n> | <price> | <amount> | <TIF> | <Pending/Expired/...> |

Neither order is an executed transaction. Do not count unfilled orders as holdings or cash debits.

## 3. Reported capital and cash

| Item | Amount | Treatment |
|---|---:|---|
| Available cash after sale | <amount> | User-reported; not independently reconciled |
| Pending order reservation | <amount> | Keep separate from available cash |
| Planned additional cash-in | <amount> | Not deposited; exclude from current cash |
| Stated total buying power including planned cash-in | <amount> | Arithmetic; not verified broker buying power |

**Reconciliation warning:** Reported available cash, reserved order amount and stated buying power do not establish a complete broker cash ledger. Confirm the app's exact available-to-trade, cash balance and reserved balance fields before using them for sizing.

## 4. Position state after the reported sale

<Net position statement; zero shares if buys offset by sells, subject to fill reconciliation.>

## 5. Fee-drag analysis

- Buy-side fees: <amount> / <gross buy> = <pct>%
- Sell-side fees: <amount> / <gross sell> = <pct>%
- Combined fees / gross traded value: <amount> / <total> = <pct>%

The practical break-even move must cover buy-side and sell-side fees plus spread and slippage.

## 6. Source and verification controls

- Source supplied by user: <app/screenshots and transcription>.
- Original screenshot files/receipt identifiers and broker order IDs: not present in this record.
- Verification status: user_reported, not verified_official.
- Before upgrading status, reconcile against broker contract notes/order history and the official fee schedule.
- Do not infer an executed trade from an order screen or pending status.
- Do not treat this ledger as a live portfolio statement or trade instruction.
