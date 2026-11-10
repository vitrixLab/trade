# API Integration Blueprint

**Status:** Draft contract — read-only intelligence API  
**Scope:** Instrument directory, market bars, macro observations, events, portfolio snapshots, research findings  
**Implementation:** Not implemented by this specification; provider integrations, authentication, persistence, and deployment are out of scope.

## 1. Purpose

Define a stable, versioned read-only API surface for a trading analysis system. The API serves normalized research data; it does not place, modify, or cancel orders.

## 2. Endpoints

| Endpoint | Method | Purpose |
|---|---|---|
| /api/v1/instruments | GET | List verified instruments with identity status |
| /api/v1/market/bars | GET | Retrieve normalized OHLCV bars by instrument, interval, and time range |
| /api/v1/macro/observations | GET | Retrieve normalized macro observations by series and time range |
| /api/v1/events | GET | Retrieve financial intelligence events by region, time range, and category |
| /api/v1/portfolio/snapshots | GET | Retrieve the authorized user's read-only portfolio snapshots |
| /api/v1/research/findings | GET | Retrieve research findings by review status |

See `api/openapi.yaml` for the full OpenAPI 3.1 contract.

## 3. Security

- Bearer JWT authentication (placeholder; replace with your provider).
- No broker credentials, API keys, or personal account identifiers in request/response schemas.
- All write/mutating operations (order placement, cancellation, modification) are explicitly out of scope.

## 4. Data contracts

Responses follow `schema_version: "1.0.0"` and carry a `data` array. Individual records conform to schemas under `schemas/`:
- `instrument.schema.json` → Instrument
- `market-bar.schema.json` → MarketBar
- `macro-observation.schema.json` → MacroObservation
- `event.schema.json` → Event
- `portfolio-snapshot.schema.json` → PortfolioSnapshot
- `research-finding.schema.json` → ResearchFinding

## 5. Integration guidelines

- Treat every timestamp as UTC unless a local timezone is explicitly attached.
- Represent money as decimal strings, never JSON floating-point.
- Mark provenance_status on every source reference (`verified`, `unverified`, `estimated`, `derived`).
- Do not cache or redistribute vendor market data contrary to provider terms.
- Prefer exchange and issuer primary disclosures for material facts.

## 6. Out of scope

- Broker connectivity and order execution.
- Market data ingestion pipelines (feeds, normalization, storage).
- Authentication/authorization implementation beyond the bearer placeholder.
- Deployment, rate limiting, and observability.
