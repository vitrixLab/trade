# Quant Trading: A Full-Swing Research Synthesis

## From the Black-Scholes-Merton "Trillion-Dollar Equation" to a Fat-Tailed, Cost-Aware Research Operating Model

**Document control**

- **Status:** Research synthesis / research-only
- **Version:** 1.0.0
- **Date:** 2026-11-11
- **Scope:** Black-Scholes-Merton (BSM), the physics-to-finance lineage, empirical fat tails, volatility modeling, quant strategy design, Renaissance/Medallion, LTCM, backtest overfitting, risk, execution, and model governance
- **Decision authority:** Human investor only
- **Evidence standard:** FACT / ESTIMATE / HYPOTHESIS / MODEL OUTPUT / DECISION / UNKNOWN, following `docs/AI_QUANT_STRATEGY.md`
- **Citation verification:** DOIs resolved through the Crossref API and ISBNs checked through OpenLibrary on 2026-11-11. See §21 for verification notes.
- **Trading status:** No live market feed, broker connection, order routing, or autonomous execution is implied by this document.

> **Research-only notice.** This is an educational and analytical synthesis, not individualized financial advice, a solicitation, or a promise of performance. Every model, backtest, and historical return discussed here is conditional on assumptions, data quality, costs, capacity, and regime. A backtest is evidence to be criticized, not a forecast to be trusted.

---

## 0. Executive summary

1. **The "trillion-dollar equation" is the Black-Scholes-Merton pricing framework.** Black and Scholes published the closed-form option-pricing result in 1973; Merton independently developed the continuous-time rational-option-pricing framework the same year. The underlying object is a no-arbitrage partial differential equation (PDE), not a crystal ball.
2. **The physics lineage is real.** Bachelier modeled speculative prices as a random walk in 1900, before Einstein's 1905 Brownian-motion paper. The diffusion mathematics connects to Fourier's heat equation, and the normal distribution enters through the central limit theorem and the lognormal assumption of geometric Brownian motion.
3. **The video's central tension is correct but compressed.** Real markets are fat-tailed, volatility clusters, prices jump, and correlations break in crises. Those facts invalidate a literal reading of BSM, but they do not make the framework useless. BSM survives as a relative-pricing, hedging, and quoting language—provided its limitations are priced and managed.
4. **BSM does not forecast stock returns.** The expected drift of the stock cancels out of the replication argument. The model prices an option relative to the underlying, the risk-free rate, time, and volatility. The only unobservable input is volatility, which is why implied volatility and the volatility surface became the real language of derivatives markets.
5. **The 1987 crash was the empirical turning point.** Portfolio-insurance hedging, gap risk, and liquidity stress produced a persistent equity-index volatility skew. The smile/skew is the market's visible correction to the flat-volatility BSM world. Post-1987 option prices encode crash fear, jump risk, and stochastic volatility.
6. **Extensions repair parts of the model, not all of it.** Merton jump diffusion, Heston stochastic volatility, Bates jump-stochastic-volatility, local volatility, Lévy models, and stochastic-local-volatility models each improve some dimension of fit. None eliminates model risk, calibration risk, or hedging error.
7. **The quant edge is not the formula.** Durable edges come from data, market microstructure, execution, diversification of many weak signals, risk control, capacity discipline, and organizational execution. The formula is infrastructure, not alpha.
8. **Renaissance's Medallion fund is an extraordinary case study, not a replicable template.** Cornell's reconstruction of Zuckerman's data reports roughly 66.1% average annual gross return and 39.2% net for 1988–2018, with a 63.3% compound gross return and no negative gross year. The fund was closed to outside capital, capacity-constrained, high-turnover, and opaque. Its returns are not audited public data and cannot be treated as a retail target.
9. **LTCM is the counter-case.** Nobel-linked models, high leverage, correlated convergence trades, and a liquidity shock turned a small probability event into a systemic near-failure. The lesson is not "models are useless"; it is "models plus leverage plus crowded correlation plus illiquidity can be fatal."
10. **Backtest overfitting is the silent killer.** Data snooping, multiple testing, look-ahead bias, survivorship bias, unrealistic costs, and capacity blindness can manufacture beautiful results that fail live. PBO, the deflated Sharpe ratio, White's Reality Check, purged cross-validation, and pre-registration are practical defenses.
11. **The full-swing operating model is end-to-end.** Frame an economic hypothesis, use point-in-time data, build robust signals, construct a risk-budgeted portfolio, model frictions, validate out of sample, stress the tails, cap capacity, and govern the model with human approval and kill criteria.
12. **The correct conclusion is humility plus rigor.** BSM is one of the most consequential applications of mathematics to markets. It is also a reminder that a model can be powerful, widely used, and still wrong in the tails. The goal is not to find the perfect equation; it is to build a process that survives being wrong.

---

## 1. The video's claim, stated precisely

The Veritasium episode "The Trillion Dollar Equation" (published 2024-02-28) tells the story of how the Black-Scholes-Merton framework moved from physics-inspired probability theory into the architecture of modern derivatives markets. Its narrative arc is broadly accurate:

- Louis Bachelier used a random walk to price options in 1900.
- Einstein's 1905 work on Brownian motion gave the diffusion a physical foundation.
- Fischer Black, Myron Scholes, and Robert Merton turned continuous-time hedging into an explicit option-pricing formula in 1973.
- Ed Thorp pioneered delta hedging and Kelly-style bet sizing.
- Jim Simons and Renaissance Technologies became the modern symbol of quantitative trading.
- The model's Achilles' heel is that real markets have fat tails and rare extreme events that a normal distribution understates.

What the video compresses—and what this document expands—is the distinction between three different objects:

1. **The BSM PDE:** a no-arbitrage relationship among the option price, the underlying, time, the risk-free rate, and volatility.
2. **The BSM formula:** a closed-form solution for European calls and puts under geometric Brownian motion.
3. **The BSM worldview:** the assumption that continuous hedging, frictionless markets, constant volatility, and lognormal returns are close enough to reality to be used as a default model.

The first object is mathematically elegant and remains foundational. The second is a useful benchmark. The third is empirically false in important ways. Most public confusion—including some of the video's framing—comes from treating the third as if it were the first.

The "trillion-dollar" label refers to the scale of the derivatives and options markets that adopted BSM-style pricing and hedging conventions, not to profits generated by the formula itself. The formula did not create those markets alone; exchanges, clearing infrastructure, regulation, computing power, market-making capital, and demand for risk transfer did. BSM provided a shared language that made those markets easier to price, hedge, and scale.

**Classification:** FACT for the historical publication record; ESTIMATE for market-scale claims; HYPOTHESIS for causal narratives about the equation "causing" the derivatives industry.

---

## 2. From physics to finance: the lineage

### 2.1 Bachelier's random walk (1900)

Louis Bachelier's doctoral thesis, *Théorie de la spéculation*, modeled stock prices as a random walk with independent increments. He derived an option-pricing formula using the normal distribution and the idea that the fair price equalizes expected gains and losses between buyer and seller. In modern language, he was pricing an option under an arithmetic Brownian motion.

Bachelier's work was far ahead of its time, but it had two practical problems:

- Arithmetic Brownian motion allows negative stock prices.
- The model ignored the drift and the risk-free rate in the way later no-arbitrage theory would require.

Bachelier's thesis was not widely adopted by traders, but it established the core idea: price changes can be modeled as a stochastic process, and option values can be derived from the distribution of future prices.

**Citation:** Bachelier (1900), DOI 10.24033/asens.476.

### 2.2 Einstein and Brownian motion (1905)

Einstein's 1905 paper explained Brownian motion as the result of molecular collisions and derived a diffusion equation for the probability density of particle positions. The mathematics is the same family of parabolic PDEs that appears in heat conduction. The connection is not a metaphor: the probability density of a diffusing particle and the probability density of a Brownian-motion price process both satisfy diffusion equations.

Einstein's work gave the random-walk idea a physical basis and a mathematical toolkit. It also reinforced the normal distribution as the natural limiting distribution of many small independent shocks.

**Citation:** Einstein (1905), DOI 10.1002/andp.19053220806.

### 2.3 Fourier, heat, and the diffusion equation

Fourier's heat equation describes how temperature diffuses through a medium. The Brownian-motion probability density satisfies a closely related equation. The BSM PDE is a backward parabolic PDE; after a change of variables, it can be transformed into the heat equation. This is the technical sense in which the "physics connection" in the video is correct.

The practical implication is important: BSM is a diffusion model. It assumes continuous sample paths and local randomness. That is exactly the assumption that jumps and liquidity gaps violate.

**Citation:** The mathematical transformation is standard; see Wilmott, Howison & Dewynne (1995), ISBN 978-0-521-49789-3, and Karatzas & Shreve (1991), ISBN 978-0-387-97655-6.

### 2.4 What Black, Scholes, and Merton added

Black and Scholes (1973) and Merton (1973) made three decisive contributions:

1. **Geometric Brownian motion:** stock prices follow
   \[
   dS_t = \mu S_t\,dt + \sigma S_t\,dW_t,
   \]
   which keeps prices positive and produces lognormal returns.
2. **Dynamic replication:** a portfolio of the underlying and a risk-free asset can replicate the option's payoff locally. The hedge ratio is the option delta.
3. **No-arbitrage pricing:** if the replicating portfolio is riskless, it must earn the risk-free rate. This yields a PDE whose solution is the option price.

Merton generalized the framework, clarified the role of continuous trading, and later extended it to jumps (Merton, 1976). The Cox-Ross-Rubinstein binomial model (1979) made the logic implementable numerically and connected the PDE to discrete-time hedging.

**Citations:** Black & Scholes (1973), DOI 10.1086/260062; Merton (1973), DOI 10.2307/3003143; Cox, Ross & Rubinstein (1979), DOI 10.1016/0304-405X(79)90015-1.

### 2.5 Why the normal distribution appears

The normal distribution enters in two ways:

- **Central limit theorem:** the sum of many small independent shocks tends toward a normal distribution.
- **Lognormal returns:** if log returns are normal, prices are lognormal, which is the BSM assumption.

The normal distribution is mathematically convenient, not empirically universal. The empirical record shows that financial returns have heavier tails than the normal distribution, especially at short horizons and around crises. That is the core of the video's "flaw" segment, and it is correct as far as it goes.

---

## 3. The BSM model: mathematics and meaning

### 3.1 The stochastic process

Under the physical measure \(P\), the stock follows geometric Brownian motion:

\[
dS_t = \mu S_t\,dt + \sigma S_t\,dW_t,
\]

where \(\mu\) is the expected drift, \(\sigma\) is the volatility, and \(W_t\) is a standard Brownian motion. The solution is

\[
S_t = S_0 \exp\left[\left(\mu - \frac{1}{2}\sigma^2\right)t + \sigma W_t\right].
\]

Log returns over a horizon \(T\) are normal:

\[
\ln(S_T/S_0) \sim N\left[\left(\mu - \frac{1}{2}\sigma^2\right)T,\ \sigma^2 T\right].
\]

### 3.2 The replication argument

Let \(V(S,t)\) be the option price. By Itô's lemma,

\[
dV = \left(\frac{\partial V}{\partial t} + \frac{1}{2}\sigma^2 S^2 \frac{\partial^2 V}{\partial S^2}\right)dt + \frac{\partial V}{\partial S}\,dS.
\]

Construct a portfolio that is long one option and short \(\Delta = \partial V/\partial S\) shares:

\[
\Pi = V - \Delta S.
\]

The stochastic \(dS\) terms cancel. The portfolio is locally riskless, so it must earn the risk-free rate \(r\):

\[
d\Pi = r\Pi\,dt.
\]

Substituting and simplifying gives the Black-Scholes-Merton PDE:

\[
\boxed{
\frac{\partial V}{\partial t}
+ \frac{1}{2}\sigma^2 S^2 \frac{\partial^2 V}{\partial S^2}
+ rS\frac{\partial V}{\partial S}
- rV = 0
}
\]

The drift \(\mu\) has disappeared. This is the most important conceptual point in the entire framework: **BSM is a relative-pricing model, not a forecast of the stock's expected return.**

### 3.3 The closed-form call price

For a European call with strike \(K\), maturity \(T\), risk-free rate \(r\), dividend yield \(q\), and volatility \(\sigma\), the BSM price is

\[
C = S_0 e^{-qT}\Phi(d_1) - K e^{-rT}\Phi(d_2),
\]

where

\[
d_1 = \frac{\ln(S_0/K) + (r - q + \frac{1}{2}\sigma^2)T}{\sigma\sqrt{T}},
\qquad
d_2 = d_1 - \sigma\sqrt{T},
\]

and \(\Phi(\cdot)\) is the standard normal cumulative distribution function. The put price follows from put-call parity:

\[
C - P = S_0 e^{-qT} - K e^{-rT}.
\]

### 3.4 The Greeks

The Greeks are the sensitivities that make the model operational:

- **Delta:** \(\Delta_C = e^{-qT}\Phi(d_1)\), \(\Delta_P = -e^{-qT}\Phi(-d_1)\).
- **Gamma:** \(\Gamma = \frac{e^{-qT}\phi(d_1)}{S_0\sigma\sqrt{T}}\).
- **Vega:** \(\mathcal{V} = S_0 e^{-qT}\phi(d_1)\sqrt{T}\).
- **Theta (call, no dividends):** \(\Theta_C = -\frac{S_0\phi(d_1)\sigma}{2\sqrt{T}} - rK e^{-rT}\Phi(d_2)\).
- **Rho (call):** \(\rho_C = K T e^{-rT}\Phi(d_2)\).

Delta is the hedge ratio. Gamma measures the instability of the hedge. Vega measures exposure to implied volatility. Theta measures time decay. A delta-hedged option position is, to first order, a bet on realized volatility versus implied volatility.

### 3.5 The assumptions

The standard BSM derivation assumes:

1. The underlying follows geometric Brownian motion with constant drift and volatility.
2. The option is European and can be exercised only at maturity.
3. Markets are frictionless: no transaction costs, taxes, or short-sale restrictions.
4. Trading is continuous and assets are infinitely divisible.
5. The risk-free rate is known and constant.
6. The underlying pays no dividends, or dividends are known and continuous.
7. There are no arbitrage opportunities.
8. Volatility is known and constant.
9. Liquidity is unlimited and prices are continuous.
10. Borrowing and lending are available at the risk-free rate.

Some assumptions can be relaxed without destroying the framework: dividends, deterministic rates, early exercise (via numerical methods), and discrete hedging (with error). Others are more fundamental: jumps, stochastic volatility, liquidity gaps, and incomplete markets change the economics of hedging and pricing.

### 3.6 What BSM does not say

- It does **not** predict the expected return of the stock. The drift cancels.
- It does **not** say options are mispriced. It says what a price must be relative to the replicating portfolio under the assumptions.
- It does **not** require investors to be risk-neutral. Risk-neutral pricing is a mathematical device, not a claim about preferences.
- It does **not** guarantee that continuous hedging is feasible. It assumes it.
- It does **not** imply that all options on the same underlying should have the same implied volatility. The smile is evidence that the model is misspecified.
- It does **not** say markets are efficient. It is a no-arbitrage relationship, not a test of informational efficiency.
- It does **not** say quant trading is easy. The formula is a tool; edge comes from elsewhere.

### 3.7 Risk-neutral versus physical measure

Under the risk-neutral measure \(Q\), the stock evolves as

\[
dS_t = (r - q)S_t\,dt + \sigma S_t\,dW_t^Q,
\]

and the call price is the discounted risk-neutral expectation of the payoff:

\[
C = e^{-rT}\mathbb{E}^Q\left[(S_T - K)^+\right].
\]

Under the physical measure \(P\), the drift is \(\mu\), and the actual distribution may have jumps, fat tails, and stochastic volatility. The two measures are linked by risk premia and market prices of risk. In incomplete markets—precisely the world of jumps and stochastic volatility—the risk-neutral measure is not unique, and prices depend on hedging assumptions and risk preferences.

This is why the statement "BSM assumes normal returns" needs a qualification. BSM assumes lognormal returns **under the model's measure**; the empirical failure is that actual returns are not lognormal and that continuous hedging cannot reproduce the option payoff in the presence of jumps and volatility shifts.

### 3.8 Implied volatility: the market's correction

Given a market option price, the **implied volatility** is the value of \(\sigma\) that makes the BSM formula equal the market price. If BSM were literally true, implied volatility would be the same across strikes and maturities for a given underlying. In practice, it is not. The pattern of implied volatility across strikes is the **smile** or **skew**; across maturities it is the **term structure**.

Implied volatility is best understood as a quoting convention: a compact way to translate option prices into a volatility number that traders can compare, hedge, and risk-manage. It is not necessarily a forecast of future realized volatility. The gap between implied and realized volatility is a risk premium, a hedging error, and a source of trading P&L.

---

## 4. The empirical problem: markets are not lognormal

### 4.1 Fat tails

Mandelbrot (1963) documented that speculative price changes have heavier tails than the normal distribution. Fama (1965) reached similar conclusions for stock prices. Cont (2001) summarizes the stylized facts: the unconditional distribution of returns has a power-law or Pareto-like tail with a finite tail index, typically between 2 and 5 for many data sets. That range excludes both the normal distribution (tail index effectively infinite) and stable distributions with infinite variance.

Fat tails matter because:

- Extreme moves occur more often than a normal model predicts.
- Variance and standard deviation are unstable and can be dominated by a few observations.
- Higher moments may not exist in a useful sense.
- Risk measures based on normality understate tail losses.
- Diversification can fail when correlations spike in a crisis.

A normal distribution assigns vanishingly small probability to a 5-sigma or 6-sigma move. In markets, such moves happen often enough to wipe out strategies that treat them as impossible. The exact frequency is sample-dependent and should not be reduced to a single number, but the qualitative conclusion is robust: **the tails are the risk.**

**Citations:** Mandelbrot (1963), DOI 10.1086/294632; Fama (1965), DOI 10.1086/294743; Cont (2001), DOI 10.1080/713665670.

### 4.2 Volatility clustering

Volatility is not constant. Large price changes tend to cluster, and volatility exhibits positive autocorrelation. Engle (1982) introduced autoregressive conditional heteroskedasticity (ARCH); Bollerslev (1986) generalized it to GARCH. The GARCH(1,1) model is

\[
\sigma_t^2 = \omega + \alpha \varepsilon_{t-1}^2 + \beta \sigma_{t-1}^2,
\]

with persistence \(\alpha + \beta\) and long-run variance \(\omega/(1-\alpha-\beta)\). The empirical regularity is that \(\alpha + \beta\) is often close to 1, meaning shocks to volatility decay slowly.

Volatility clustering has practical consequences:

- Option prices depend on expected future volatility, not a constant.
- Risk models need conditional volatility forecasts.
- Position sizing should adapt to volatility regimes.
- Backtests that assume constant volatility misstate drawdowns and Sharpe ratios.

**Citations:** Engle (1982), DOI 10.2307/1912773; Bollerslev (1986), DOI 10.1016/0304-4076(86)90063-1; Cont (2001), DOI 10.1080/713665670.

### 4.3 Jumps

Merton (1976) showed that if stock returns contain jumps, the continuous-time replication argument fails. A jump is a discontinuous change in price; a delta hedge cannot adjust continuously through it. The hedge portfolio bears jump risk, and the option price depends on the market price of jump risk.

Merton's jump-diffusion model adds a Poisson jump component:

\[
\frac{dS_t}{S_{t^-}} = (\mu - \lambda \kappa)\,dt + \sigma\,dW_t + (J - 1)\,dN_t,
\]

where \(N_t\) is a Poisson process with intensity \(\lambda\), \(J\) is the jump size, and \(\kappa = \mathbb{E}[J - 1]\). The option price becomes a Poisson-weighted mixture of BSM prices with adjusted parameters. Jumps fatten the tails and produce steep short-dated smiles.

**Citation:** Merton (1976), DOI 10.1016/0304-405X(76)90022-2.

### 4.4 Skew and the volatility smile

If BSM were true, implied volatility would be flat across strikes. In equity index options, it is not: out-of-the-money puts typically trade at higher implied volatility than at-the-money options, producing a negative skew. This pattern became pronounced after the 1987 crash and has persisted.

The smile/skew reflects:

- The risk-neutral distribution's negative skew and fat left tail.
- Crash fear and demand for downside protection.
- Jump risk and stochastic volatility.
- Supply-demand imbalances in option markets.
- Correlation risk and the cost of hedging tail events.

The smile is not a small perturbation. It is a direct, market-priced rejection of the constant-volatility lognormal model. Any serious options trading operation must model the surface, not just a single volatility number.

**Citations:** Rubinstein (1994), DOI 10.1111/j.1540-6261.1994.tb00079.x; Jackwerth & Rubinstein (1996), DOI 10.1111/j.1540-6261.1996.tb05219.x; Derman, Kani & Chriss (1996), DOI 10.3905/jod.1996.407952; Bates (2000), DOI 10.1016/S0304-4076(99)00021-4.

### 4.5 Tail dependence and correlation breakdown

Correlations are not stable. In stress episodes, many assets fall together, and diversification benefits shrink or disappear. This is **tail dependence**: the probability of joint extreme losses is higher than a multivariate normal model implies. The result is that a portfolio that looks diversified in normal times can behave like a single concentrated bet in a crisis.

This is one of the deep lessons of LTCM: convergence trades that were individually small and apparently uncorrelated became correlated under a global flight to liquidity. The model saw risk; the portfolio experienced a liquidity spiral.

**Citations:** Edwards (1999), DOI 10.1257/jep.13.2.189; Federal Reserve History (n.d.), "Near Failure of Long-Term Capital Management"; Lowenstein (2000), ISBN 978-0-375-75825-6.

### 4.6 What survives from BSM

BSM survives in several important ways:

- **Relative pricing:** it provides a consistent relationship between the option and the underlying.
- **Hedging:** delta, gamma, and vega are still the language of options risk management.
- **Implied volatility:** it provides a common quoting convention across strikes and maturities.
- **No-arbitrage bounds:** it anchors put-call parity and static arbitrage constraints.
- **Benchmarking:** it is a baseline against which richer models are measured.
- **Risk decomposition:** it separates directional, convexity, volatility, and time exposures.

The correct posture is not "BSM is true" or "BSM is dead." It is: **BSM is a local approximation and a benchmark; use it where its hedging error is acceptable, and model the deviations where it is not.**

### 4.7 Normal versus empirical: a compact comparison

| Property | Normal / BSM world | Empirical markets | Consequence |
|---|---|---|---|
| Tail decay | Exponential | Power-law / heavy | Extreme moves more likely |
| Kurtosis | 3 | Excess kurtosis > 0 | Variance-based risk understates tails |
| Skewness | 0 | Often negative for equities | Crash risk and put skew |
| Volatility | Constant | Clustered and stochastic | BSM misprices short-dated options |
| Dependence | Linear, stable | Tail dependence, regime shifts | Diversification fails in crises |
| Path | Continuous | Jumps and gaps | Delta hedging is discontinuous |
| Liquidity | Unlimited | Finite and state-dependent | Market impact and gap risk |
| Distribution | Lognormal | Non-lognormal, horizon-dependent | Implied vol smile/skew |

**Classification:** The existence of fat tails, volatility clustering, jumps, and the smile is FACT (supported by the cited literature). The precise tail exponent and the best model are ESTIMATE/MODEL OUTPUT and depend on data and method.

---

## 5. The 1987 crash and the birth of the smile

The 1987 crash is the hinge between the theoretical BSM world and the modern volatility surface. Before October 1987, equity index implied volatilities were relatively flat across strikes. After the crash, deep out-of-the-money puts traded at substantially higher implied volatilities than at-the-money options, and the skew persisted.

The mechanism is a combination of:

- **Portfolio insurance:** dynamic hedging strategies that were supposed to replicate a put by selling futures as prices fell. The strategy assumed continuous, liquid markets. In a gap down, everyone tried to sell at once, liquidity disappeared, and the hedge failed.
- **Jump risk:** the crash was a discontinuous move, not a diffusion.
- **Correlation and liquidity:** selling pressure fed on itself; the market's own risk-management rules amplified the decline.
- **Demand for crash protection:** after 1987, investors and dealers priced tail risk more explicitly.

The crash did not disprove the mathematics of BSM. It disproved the assumption that continuous hedging is always feasible at observed prices. The market responded by pricing the smile: options that protect against crashes became more expensive relative to the flat-volatility benchmark.

**Citations:** Derman (1994), "The Volatility Smile and Its Implied Tree"; Derman, Kani & Chriss (1996), DOI 10.3905/jod.1996.407952; Rubinstein (1994), DOI 10.1111/j.1540-6261.1994.tb00079.x; Bates (2000), DOI 10.1016/S0304-4076(99)00021-4. The portfolio-insurance mechanism is discussed in Jacobs (1999), "The October 1987 Crash and Portfolio Insurance" (non-DOI source; see §20.9).

**Lesson:** The most important model risk is not always the shape of the distribution. It is the assumption that you can trade when you need to.

---

## 6. Extensions and replacements: what came after BSM

### 6.1 Merton jump diffusion (1976)

Merton added Poisson jumps to geometric Brownian motion. The model captures sudden discontinuous moves, produces fat tails, and can generate a short-dated smile. The cost is additional parameters: jump intensity, jump-size mean, and jump-size volatility. Jumps are latent and hard to estimate; the model can fit prices without necessarily improving hedging.

### 6.2 Stochastic volatility: Heston (1993)

Heston modeled variance as a mean-reverting diffusion:

\[
dS_t = \mu S_t\,dt + \sqrt{v_t} S_t\,dW_t^S,
\]
\[
dv_t = \kappa(\theta - v_t)\,dt + \xi\sqrt{v_t}\,dW_t^v,
\]
\[
d\langle W^S, W^v\rangle_t = \rho\,dt.
\]

The Heston model generates a volatility smile/skew, mean-reverting volatility, and a closed-form characteristic-function solution for European options. It is a workhorse for volatility trading and calibration, but it struggles with short-dated skew and can be unstable to calibrate.

### 6.3 Bates and jump-stochastic-volatility models

Bates (1996) combined jumps and stochastic volatility, which improves the fit to currency options and equity skew. Later empirical work—Bakshi, Cao & Chen (1997); Andersen, Benzoni & Lund (2002); Eraker, Johannes & Polson (2003)—found that both jumps and stochastic volatility are needed to describe equity index returns and option prices. The models fit better than BSM, but parameter uncertainty and hedging error remain.

### 6.4 Local volatility, Lévy models, and stochastic-local volatility

- **Local volatility:** Dupire-style models make volatility a deterministic function of price and time, calibrated to the entire option surface. They can fit today's prices but may produce unrealistic dynamics and poor hedges.
- **Lévy models:** Carr & Wu (2004) and others use time-changed Lévy processes to capture jumps and skew. They provide a rich distributional fit but can be hard to hedge and calibrate.
- **Stochastic-local volatility:** combines local volatility with stochastic variance. It is widely used in practice for exotics and surface dynamics, at the cost of complexity and model risk.
- **Rough volatility:** models variance as a rough fractional process, motivated by empirical scaling of volatility. It is an active research area, not a settled replacement.

### 6.5 Empirical performance and model risk

The empirical literature does not crown a single winner. Richer models usually fit option prices better in-sample, but:

- Calibration is noisy and unstable.
- Parameters are not stable across time.
- Hedging performance can be worse than the fit suggests.
- Model risk is itself a source of P&L and loss.
- The "best" model depends on the instrument, horizon, and purpose.

The practical conclusion is that model choice is a risk-management decision, not a quest for the true data-generating process. Use an ensemble or a hierarchy of models; test hedging performance out of sample; stress the parameters; and treat the surface as a market state variable, not a fixed law.

**Citations:** Heston (1993), DOI 10.1093/rfs/6.2.327; Bates (1996), DOI 10.1093/rfs/9.1.69; Bakshi, Cao & Chen (1997), DOI 10.1111/j.1540-6261.1997.tb02749.x; Andersen, Benzoni & Lund (2002), DOI 10.1111/1540-6261.00460; Eraker, Johannes & Polson (2003), DOI 10.1111/1540-6261.00566; Carr & Wu (2004), DOI 10.1016/S0304-405X(03)00171-5; Gatheral (2006), ISBN 978-0-471-79251-2; Derman & Miller (2016), ISBN 978-1-119-28925-8.

---

## 7. From pricing to trading: delta hedging, gamma, and the vol P&L identity

### 7.1 Delta hedging

The BSM replication argument says that if you own an option and short \(\Delta\) shares, the position is locally riskless. In practice, delta is rebalanced discretely, not continuously. Each rebalance incurs transaction costs, slippage, and market impact. The delta-hedged position therefore has residual P&L from:

- Discrete hedging error (proportional to Gamma and the square of price changes).
- Realized volatility different from the implied volatility used to set the hedge.
- Jumps and gaps between rebalances.
- Bid-ask spread and market impact on the hedge trades.

### 7.2 The volatility P&L identity

For a delta-hedged European option, the approximate instantaneous P&L is

\[
d\Pi \approx \frac{1}{2}\Gamma S^2 \left(\sigma_{\text{realized}}^2 - \sigma_{\text{implied}}^2\right)dt + \text{higher-order terms}.
\]

This identity is the economic core of volatility trading:

- Long gamma profits when realized volatility exceeds implied volatility.
- Short gamma profits when realized volatility is below implied volatility.
- The strategy is short skew risk, short vega risk, and short jump risk.

This explains why "volatility is an asset class" and why option market-making is a business of managing the gap between implied and realized volatility, not of predicting stock prices.

### 7.3 Gamma scalping and volatility arbitrage

Gamma scalping is the active rebalancing of delta to harvest the convexity term when the underlying moves. Volatility arbitrage extends this across the surface: trading skew, term structure, and dispersion (index versus components) under risk limits. These strategies are high-turnover, cost-sensitive, and exposed to gap risk and vol-of-vol risk. They require real-time execution and disciplined risk limits.

### 7.4 Why BSM is a market language

Even practitioners who do not believe in constant-volatility lognormal dynamics use BSM because:

- It standardizes quotes across strikes and maturities through implied volatility.
- It provides Greeks for risk aggregation.
- It supports put-call parity and static arbitrage bounds.
- It is the baseline for calibration of richer models.

In this sense, BSM is like Newtonian mechanics: not exact in all regimes, but the default working language for most practical problems.

---

## 8. The quant trading landscape

### 8.1 What "quantitative trading" actually means

"Quantitative" is a process descriptor, not a strategy. A quant strategy uses systematic rules, data, and models to generate signals, size positions, manage risk, and execute trades. The defining feature is reproducibility and testability, not the use of advanced mathematics.

Quantitative strategies span:

| Family | Typical horizon | Edge source | Typical instruments | Primary risks |
|---|---|---|---|---|
| Market making | Seconds–minutes | Spread capture, inventory management, adverse selection | Equities, futures, FX, options | Toxic flow, latency, model risk |
| Statistical arbitrage | Minutes–days | Relative mispricing, mean reversion | Equities, futures, ETFs | Crowding, convergence risk, borrow costs |
| Trend following | Days–months | Behavioral drift, risk premia | Futures, FX, commodities, equities | Whipsaw, crowding, regime shifts |
| Factor investing | Months–years | Risk premia, behavioral anomalies | Equities, bonds | Factor drawdowns, replication crisis |
| Event-driven | Days–weeks | Information asymmetry, catalysts | Equities, credit, derivatives | Gap risk, legal/event risk |
| Volatility trading | Days–months | Implied vs realized volatility | Options, variance swaps, VIX derivatives | Vol-of-vol, jump risk, liquidity |
| Macro quant | Weeks–months | Regime nowcasting, cross-asset relationships | Rates, FX, commodities, equities | Structural breaks, policy uncertainty |
| Mean reversion | Hours–days | Liquidity, overreaction | Equities, futures | Structural breaks, short squeezes |
| High-frequency | Microseconds–seconds | Latency, microstructure, inventory | Equities, futures | Technology, regulation, adverse selection |

### 8.2 What creates edge

A durable edge usually comes from one or more of:

- **Information advantage:** faster or better data, alternative data, or superior signal processing.
- **Behavioral advantage:** exploiting overreaction, underreaction, anchoring, or herding.
- **Structural advantage:** liquidity provision, market-making rebates, or reduced trading costs.
- **Risk advantage:** holding risks others avoid, or avoiding risks others crowd into.
- **Execution advantage:** lower impact, better fills, and smarter order routing.
- **Scale advantage:** spreading fixed research and technology costs over more capital (until capacity binds).

Medallion is the canonical example of an information-plus-execution edge: many weak, short-horizon signals, traded at scale with disciplined costs and risk controls.

### 8.3 Horizon, turnover, and capacity

Every strategy has three constraints that interact:

1. **Horizon:** how long the signal is expected to persist.
2. **Turnover:** how often positions change, which drives transaction costs and market impact.
3. **Capacity:** the maximum capital the strategy can absorb before alpha decays.

High-turnover strategies (market making, stat arb, vol arbitrage) need low costs and tight execution. Low-turnover strategies (factor investing, macro) need capacity and regime robustness. The "full-swing" process must match the strategy to these constraints, not just to a backtested Sharpe ratio.

---

## 9. Renaissance Technologies' Medallion fund: the ultimate case study

### 9.1 What is documented

Bradford Cornell's paper "Medallion Fund: The Ultimate Counterexample?" reconstructs Medallion's performance from Gregory Zuckerman's book appendix (Zuckerman, 2019). The data covers January 1988 through December 2018. Cornell emphasizes that the figures are gross returns after trading costs but before management fees; net returns reflect Medallion's "5-and-44" fee structure (5% management fee, 44% performance fee in later years; 20% in earlier years).

Cornell's key reported findings:

- $100 invested at the start of 1988 grew to approximately $398.7 million by end-2018 (compound gross return of 63.3%).
- Average annual gross return: about 66.1%; average annual net return: about 39.2%.
- Gross standard deviation: about 31.7%; net: about 20.3%.
- Sharpe ratio on gross returns: above 2.0 (Cornell's estimate).
- No negative gross annual return in the 31-year window. (Net of fees, 1989 was negative in the reproduced table: gross +1.00%, net −3.20%.)
- Market beta approximately −1.0; Fama-French SMB and HML loadings negative and statistically insignificant.
- Win rate about 50.75% across millions of short-term trades (per Robert Mercer, cited via Zuckerman/Cornell).
- Fund grew from roughly $20 million to roughly $10 billion in capacity without observed performance decay.

**Important caveats.** These figures are not audited public disclosures. They are an academic reconstruction based on a journalist's appendix. Medallion closed to outside investors and bought out remaining external accounts by the mid-2000s. One untouched dollar did not actually compound at 66% for 31 years; the averages are across annual return observations, and distributions/fees intervene. The figures should be read as a documented estimate of a private fund's historical performance, not as an investable track record or a prediction.

### 9.2 Selected return table

| Year | Gross % | Net % | Fund size ($M) |
|---|---|---|---|
| 1988 | 16.30 | 9.04 | 20 |
| 1989 | 1.00 | −3.20 | 20 |
| 1990 | 77.80 | 58.24 | 30 |
| 2000 | 128.10 | 98.48 | 1,900 |
| 2008 | 152.10 | 82.38 | 5,200 |
| 2018 | 76.40 | 39.98 | 10,000 |
| Average | 66.07 | 39.20 | — |
| Standard deviation | 31.66 | 20.34 | — |

Full year-by-year data are reproduced in Cornell (2020), Table 1.

### 9.3 What the numbers do and do not prove

The numbers prove that Medallion's reported performance was extraordinary and not explainable by market beta or standard Fama-French factors. They do **not** prove:

- That the strategy is public, audited, or replicable.
- That the edge came from a specific mathematical formula (BSM is not Medallion's engine; short-horizon statistical arbitrage is).
- That the returns will persist after 2018.
- That a retail investor can access similar capacity, data, leverage, or execution.

Medallion's story is evidence that systematic, data-driven, high-turnover trading can generate large risk-adjusted returns over long samples when combined with talent, data, execution, cost control, and capacity discipline. It is not evidence that any individual can reproduce those returns.

### 9.4 Likely sources of edge

Based on Cornell and Zuckerman, Medallion's edge appears to come from:

- Thousands of weakly correlated, short-horizon signals rather than a few large bets.
- Rigorous data cleaning and point-in-time construction.
- High-frequency trading infrastructure and low-latency execution.
- Transaction-cost minimization (gross returns are after trading costs).
- Risk limits and position sizing.
- Capacity management: the fund is closed and capital is distributed back to employees.
- Organizational secrecy and talent density.

The 50.75% win rate is the crucial detail: a small per-trade edge compounds across millions of trades only if costs are controlled and positions are sized correctly. That is a process lesson, not a return promise.

### 9.5 What is transferable

| Transferable | Not transferable |
|---|---|
| Process discipline and hypothesis-driven research | Renaissance's proprietary data and signals |
| Diversification of many weak bets | Medallion's execution infrastructure and latency |
| Cost control and transaction-cost modeling | Leverage, prime-broker relationships, and capital |
| Capacity discipline and closure to outside capital | Talent density and organizational culture |
| Risk limits, drawdown controls, and kill switches | The specific returns and fee structure |
| Data governance and survivorship-bias avoidance | The track record itself |

**Classification:** Medallion performance figures are ESTIMATE (reconstruction, medium-high confidence). Transferability claims are ANALYSIS. The claim that Medallion-like returns are publicly replicable is effectively UNKNOWN, with strong capacity and access barriers.

---

## 10. Long-Term Capital Management: the leverage lesson

LTCM (founded 1994, with Myron Scholes and Robert Merton among its principals) ran fixed-income relative-value and convergence trades with very high leverage. By early 1998 it held roughly $5 billion of equity, borrowed more than $125 billion, and controlled over $1 trillion in notional derivatives. Edwards (1999) reports leverage above 20-to-1; Federal Reserve History puts debt at about $30 per $1 of capital at end-1997, i.e., roughly 25–30-to-1.

In August–September 1998, the Russian default and a flight to liquidity caused the very spreads LTCM was betting on to widen. Liquidity vanished, correlations went to one, margin calls mounted, and the fund faced insolvency. The Federal Reserve Bank of New York organized a consortium recapitalization of about $3.6 billion to prevent a disorderly liquidation.

**Lessons:**

- Leverage converts small model errors into insolvency.
- Correlation assumptions estimated in calm regimes fail in crises.
- Funding liquidity risk can kill a solvent-looking portfolio.
- Models describe averages, not the path before a tail event (the quote attributed to LTCM strategist Ayman Hindy: "The models tell you where things will be in five years, but not what happens before you get there").
- Authority bias (Nobel laureates, elite credentials) is not a substitute for stress testing.

**Classification:** FACT for leverage, losses, and the Fed intervention (Edwards 1999; Federal Reserve History). The causal interpretation is ANALYSIS.

---

## 11. The methodology problem: backtest overfitting

### 11.1 Data snooping and multiple testing

Sullivan, Timmermann & White (1999) showed that the best technical trading rule found by searching thousands of rules can look profitable in-sample purely by chance. White (2000) developed the Reality Check bootstrap to test whether any rule in a set outperforms a benchmark out of the box. Politis & Romano (1994) provided the stationary bootstrap for dependent financial data.

Harvey, Liu & Zhu (2016) argue that because researchers test thousands of candidate factors, conventional t-statistics are inflated; they propose a threshold near 3.0 for a new factor to be credible.

### 11.2 Probability of backtest overfitting (PBO)

Bailey, Borwein, López de Prado & Zhu (2017) formalize PBO: the probability that the strategy selected as best in-sample performs below the median of all tried strategies out-of-sample. They estimate it with combinatorially symmetric cross-validation (CSCV). PBO above 0.5 means the selection did worse than random choice; a customary rejection threshold is PBO > 0.05.

### 11.3 The deflated Sharpe ratio

Bailey & López de Prado (2014) adjust the Sharpe ratio for selection bias, backtest overfitting, non-normality, sample length, and number of trials. The result is a more realistic probability that a reported Sharpe ratio is genuine.

### 11.4 Purged cross-validation and embargo

López de Prado (2018) advocates purged K-fold cross-validation with an embargo to prevent information leakage between training and validation folds in time-series financial data. Standard random k-fold cross-validation is inappropriate for serially dependent returns.

### 11.5 Costs, capacity, and realism

A backtest without transaction costs, slippage, market impact, borrow costs, financing, and taxes is not a backtest; it is a fantasy. Almgren & Chriss (2001) provide an optimal-execution framework that trades off expected cost against risk; Kyle (1985) models price impact; Easley, López de Prado & O'Hara (2012) model flow toxicity and informed trading. High-turnover strategies live or die by these frictions.

**Practical defenses:**

- Pre-register the hypothesis before looking at results.
- Use point-in-time data with no survivorship bias.
- Validate on truly out-of-sample, walk-forward periods.
- Report PBO, deflated Sharpe, and multiple-testing adjustments.
- Model costs as a function of participation rate and volatility.
- Stress-test across regimes (1987, 1998, 2008, 2020).
- Cap capacity before alpha decays.
- Require human approval; never automate on a backtest alone.

**Citations:** Sullivan, Timmermann & White (1999), DOI 10.1111/0022-1082.00163; White (2000), DOI 10.1111/1468-0262.00152; Politis & Romano (1994), DOI 10.1080/01621459.1994.10476870; Harvey, Liu & Zhu (2016), DOI 10.1093/rfs/hhv059; Bailey et al. (2017), DOI 10.21314/JCF.2016.322; Bailey & López de Prado (2014), DOI 10.3905/jpm.2014.40.5.094; López de Prado (2018), DOI 10.3905/jpm.2018.44.6.120; Almgren & Chriss (2001), DOI 10.21314/jor.2001.041; Kyle (1985), DOI 10.2307/1913210; Easley, López de Prado & O'Hara (2012), DOI 10.1093/rfs/hhs053.

---

## 12. A fat-tailed, cost-aware full-swing framework

"Full-swing" means end-to-end: from first principles and data, through signal and portfolio construction, to execution, risk, attribution, and governance. The loop below is designed for this repository's research-only posture.

### 12.1 The Full-Swing Quant Loop

| Stage | Purpose | Guardrails |
|---|---|---|
| **1. Frame** | State an economic hypothesis, universe, horizon, and edge source. | Hypothesis must precede data search; document why the edge should exist. |
| **2. Fetch** | Acquire point-in-time market, fundamental, macro, and event data. | Record source, timestamp, timezone, retrieval time, and freshness. |
| **3. Filter** | Clean data: corporate actions, delistings, splits, dividends, missingness, outliers. | No look-ahead; no future information in training features. |
| **4. Feature** | Build signals: technical, fundamental, macro, sentiment, event, volatility. | Normalize robustly; winsorize outliers; document definitions. |
| **5. Forecast** | Form expected return, risk, and scenarios (bull/base/bear/tail). | Distinguish model output from fact; state invalidation conditions. |
| **6. Form** | Construct portfolio under constraints: leverage, position, sector, factor, liquidity. | Risk budget before return target; use shrinkage/covariance estimation. |
| **7. Friction** | Model commissions, spread, slippage, market impact, borrow, financing, taxes. | Costs must be inside the objective, not an afterthought. |
| **8. Forward-test** | Walk-forward, out-of-sample, purged CV, PBO, deflated Sharpe. | No data leakage; report multiple-testing adjustments. |
| **9. Feedback** | Attribute P&L to alpha, beta, costs, and slippage; monitor drift. | Reconcile against live results monthly. |
| **10. Failsafe** | Kill criteria, drawdown limits, regime stops, human approval, audit trail. | No autonomous execution; human decision required. |

### 12.2 Stage gates

A strategy may advance only if it passes:

1. **Economic rationale:** a defensible reason the edge should exist and persist.
2. **Data quality:** point-in-time, survivorship-bias-free, freshness verified.
3. **Out-of-sample performance:** walk-forward results stable across regimes.
4. **Cost-adjusted net return:** positive after realistic frictions.
5. **Risk-adjusted metrics:** Sharpe/Sortino/Calmar/CVaR within mandate.
6. **Robustness:** parameter stability, subperiod consistency, regime robustness.
7. **Capacity:** estimated AUM capacity exceeds target allocation with margin.
8. **Governance:** documented model, approved by investment committee, kill criteria defined.

### 12.3 Scorecard (proposed decision thresholds; illustrative, not advice)

| Metric | Suggested research gate | Notes |
|---|---|---|
| Net Sharpe after costs | > 1.0 | Threshold is illustrative; strategy-dependent. |
| Sortino ratio | > 1.0 | Penalizes downside more than Sharpe. |
| Maximum drawdown | Within mandate (e.g., < 15–20%) | Depends on investor risk budget. |
| CVaR 95% / 99% | Reported and stress-tested | Coherent tail measure; not a guarantee. |
| Deflated Sharpe p-value | < 0.05 | Adjusts for multiple trials. |
| PBO | < 0.2 | Below means selection likely better than median out-of-sample. |
| t-statistic | > 3.0 | Harvey et al. threshold for new factors. |
| Turnover cost drag | < 20% of gross alpha | Costs must not consume the edge. |
| Capacity | > target AUM × 3 | Buffer before capacity binds. |
| Regime concentration | < 50% of P&L from one regime | Avoid hidden factor bets. |

These are **research gates**, not financial recommendations. Each threshold should be calibrated to the instrument universe, horizon, and risk budget.

### 12.4 Kill criteria

Stop or de-risk a strategy when any of the following occurs:

- Cumulative drawdown exceeds the pre-approved limit.
- Realized volatility breaches the vol-target cap for N consecutive days.
- Model drift detected: feature distributions shift vs. training period.
- Correlation regime changes (e.g., all assets correlate to 1 in a crisis).
- Liquidity dries up: days-to-liquidate exceeds horizon.
- Capacity approached: strategy AUM nears estimated capacity.
- Data source breaks or freshness fails.
- Regulatory or venue rule changes affect the instrument.
- Human investment committee votes to pause.

### 12.5 Alignment with this repository

This framework follows `docs/AI_QUANT_STRATEGY.md` (evidence classification, quant research pipeline, model governance, trade-plan schema, safety boundaries) and `docs/OPERATING_PROTOCOL.md` (daily/weekly/monthly workflows). It adds the quantitative and derivatives-specific detail missing from those templates.

---

## 13. Strategy families and testable hypotheses

### 13.1 How to read this section

The hypotheses below are **research proposals**, not strategy recommendations. Each must be tested with point-in-time data, realistic costs, out-of-sample validation, and risk limits before any capital is considered. The scorecard gates in §12.3 apply.

### 13.2 Hypotheses

| ID | Hypothesis | Strategy family | Primary evidence | Falsification condition |
|---|---|---|---|---|
| H1 | Time-series momentum on liquid futures, vol-targeted at 10% annualized, produces positive net alpha after costs. | Trend / momentum | Moskowitz, Ooi & Pedersen (2012); Hurst, Ooi & Pedersen (2017) | Net Sharpe ≤ 0.5 after costs in walk-forward, or PBO > 0.2. |
| H2 | Cross-sectional momentum plus volatility scaling improves risk-adjusted returns vs. unscaled momentum. | Trend / momentum | Asness, Moskowitz & Pedersen (2013); Moreira & Muir (2017) | No improvement in out-of-sample Sharpe after turnover costs. |
| H3 | Mean reversion works only in low-volatility regimes; a vol-regime filter removes crash drawdowns. | Mean reversion | Ornstein-Uhlenbeck / pairs literature | Strategy loses money in high-vol regimes even after filtering. |
| H4 | A CVaR-based position-sizing rule reduces maximum drawdown vs. variance-only sizing. | Risk management | Acerbi & Tasche (2002); Rockafellar & Uryasev (2000) | CVaR sizing does not reduce drawdown in out-of-sample stress periods. |
| H5 | The deflated Sharpe ratio and PBO identify overfit strategies better than raw Sharpe. | Methodology | Bailey et al. (2017); Bailey & López de Prado (2014) | Strategies passing raw Sharpe but failing deflated Sharpe/PBO do not show persistent out-of-sample decay. |
| H6 | Equity index implied skew (25-delta put minus ATM) contains information about future tail risk. | Volatility | Rubinstein (1994); Bates (2000); Derman & Miller (2016) | Skew has no out-of-sample predictive power for realized left-tail losses. |
| H7 | A delta-hedged short-volatility strategy earns a premium but experiences unrecoverable crash drawdowns. | Volatility | Heston (1993); Merton (1976); Gatheral (2006) | Crashes erase all accumulated premium in a single stress period. |
| H8 | An ensemble of trend, carry, value, and quality is more robust than any single factor. | Factor investing | Asness et al. (2013, 2019); Koijen et al. (2018) | Ensemble underperforms the best single factor after costs and capacity. |
| H9 | Strategy capacity binds before alpha decays; estimated AUM capacity predicts future turnover-cost drag. | Capacity | Almgren & Chriss (2001); Kyle (1985) | Strategy maintains alpha at 5× current capacity with unchanged costs. |
| H10 | A vol/liquidity regime filter improves out-of-sample robustness without look-ahead bias. | Regime | Cont (2001); Moreira & Muir (2017) | Filter introduces look-ahead or worsens out-of-sample Sharpe. |
| H11 | Human approval and kill criteria reduce operational and model risk vs. autonomous execution. | Governance | `OPERATING_PROTOCOL.md`; `AI_QUANT_STRATEGY.md` | Cannot be backtested directly; assessed via incident logs and post-trade reviews. |
| H12 | Medallion-like gross returns (~66% average) are not publicly replicable at retail scale. | Case study | Cornell (2020); Zuckerman (2019) | A public, audited, investable vehicle documents comparable 30-year net-of-fee returns. |

### 13.3 Strategy-family notes

- **Trend / momentum:** Long-sample evidence exists across futures, FX, commodities, and equities, but whipsaws and crowding are real. Volatility targeting helps but can increase leverage in calm regimes; cap leverage and stress for gap risk.
- **Mean reversion:** Stat arb and pairs trading depend on cointegration or mean-reversion parameters being stable. Structural breaks, borrow costs, and short squeezes are the failure modes.
- **Factor investing:** Value, momentum, quality, low volatility, size, and carry have empirical support, but the replication crisis (Hou, Xue & Zhang, 2020; Feng, Giglio & Xiu, 2020) shows many published factors do not survive costs and multiple testing.
- **Volatility trading:** Requires modeling the entire surface, not one number. Short-vol strategies collect premium most of the time and lose it quickly in crashes; tail hedging and position limits are mandatory.
- **Event-driven:** Earnings, macro surprises, and corporate actions create information events. Gap risk and adverse selection dominate; event studies must control for multiple testing.
- **Macro quant:** Small samples, structural breaks, and policy regime changes make macro signals fragile. Use them for regime filters and scenario analysis, not standalone timing.
- **Market making / HFT:** Requires infrastructure, latency, adverse-selection models, and venue-specific microstructure knowledge. Outside the scope of this research-only template.

**Citations:** Moskowitz, Ooi & Pedersen (2012), DOI 10.1016/j.jfineco.2011.11.003; Hurst, Ooi & Pedersen (2017), DOI 10.3905/jpm.2017.44.1.015; Asness, Moskowitz & Pedersen (2013), DOI 10.1111/jofi.12021; Moreira & Muir (2017), DOI 10.1111/jofi.12513; Koijen et al. (2018), DOI 10.1016/j.jfineco.2017.11.002; Asness et al. (2019), DOI 10.1007/s11142-018-9470-2; Hou, Xue & Zhang (2020), DOI 10.1093/rfs/hhy131; Feng, Giglio & Xiu (2020), DOI 10.1111/jofi.12883; Acerbi & Tasche (2002), DOI 10.1016/S0378-4266(02)00283-2; Rockafellar & Uryasev (2000), DOI 10.21314/JOR.2000.038; Kyle (1985), DOI 10.2307/1913210; Almgren & Chriss (2001), DOI 10.21314/jor.2001.041; Cont (2001), DOI 10.1080/713665670.

---

## 14. Risk management and position sizing

### 14.1 Volatility targeting

Position size is inversely proportional to forecast volatility:

\[
w_t \propto \frac{\sigma_{\text{target}}}{\sigma_t}.
\]

Vol targeting smooths the equity curve and can improve Sharpe ratios, but it has failure modes: it increases leverage in low-volatility regimes (exactly when crashes can arrive), it assumes volatility is forecastable, and it ignores tail shape. Use vol targeting **with** a maximum leverage cap, a vol ceiling, and stress tests.

### 14.2 Fractional Kelly

For continuous lognormal returns, the growth-optimal fraction is approximately

\[
f^* = \frac{\mu - r}{\sigma^2}.
\]

Kelly maximizes expected log wealth but is aggressive; fractional Kelly (e.g., half-Kelly) is the practical standard because it reduces drawdown with modest growth sacrifice. The ergodicity literature (Peters, 2019) emphasizes that time-average growth—not ensemble-average expected return—is what matters for a single capital path. Drawdowns and path dependence dominate the utility of most investors.

**Citations:** Kelly (1956), DOI 10.1002/j.1538-7305.1956.tb03809.x; Thorp (2008), DOI 10.1016/b978-044453248-0.50015-0; Peters (2019), DOI 10.1038/s41567-019-0732-0.

### 14.3 CVaR and stress testing

Variance is not a coherent tail risk measure. Expected shortfall (CVaR) at level \(\alpha\),

\[
\text{CVaR}_\alpha = \mathbb{E}[L \mid L \ge \text{VaR}_\alpha],
\]

is coherent and focuses on the tail beyond VaR. Acerbi & Tasche (2002) prove coherence; Rockafellar & Uryasev (2000) provide optimization. Use CVaR for position limits and stress scenarios (1987, 1998, 2008, 2020, vol events). Stress tests should include gap risk, correlation-to-one, liquidity freeze, and model-parameter shocks.

**Citations:** Acerbi & Tasche (2002), DOI 10.1016/S0378-4266(02)00283-2; Rockafellar & Uryasev (2000), DOI 10.21314/JOR.2000.038.

### 14.4 Correlation, liquidity, and tail dependence

- Stress correlations upward in crisis scenarios; assume diversification fails when you need it most.
- Model days-to-liquidate as a function of ADV, participation rate, and volatility.
- Include bid-ask spread, slippage, and market impact in risk (not just return).
- Monitor factor exposures and tail dependence (copulas, EVT) rather than only Pearson correlation.

### 14.5 Drawdown control

Set maximum drawdown, time-in-drawdown, and de-risking rules before trading. A drawdown limit is a decision rule, not a forecast. Combine with vol targeting: reduce size as drawdown deepens, and halt at the circuit breaker.

---

## 15. Execution and capacity

### 15.1 Execution costs

For high-turnover strategies, execution is often the difference between profit and loss. Model:

- Commissions and exchange fees.
- Bid-ask spread (half-spread per round trip).
- Slippage and temporary market impact (Almgren-Chriss).
- Permanent market impact and adverse selection (Kyle lambda, VPIN).
- Borrow costs for shorting; financing for leverage.
- Taxes and FX conversion.

### 15.2 Capacity estimation

Estimate capacity from:

\[
\text{Capacity} \approx \min_i \left( \frac{\text{ADV}_i \times \text{participation cap}_i}{\text{turnover}_i} \right).
\]

Include a margin of safety. Strategy decay and crowding mean capacity is not static; re-estimate regularly. Do not scale AUM until capacity and cost drag are validated.

**Citations:** Almgren & Chriss (2001), DOI 10.21314/jor.2001.041; Kyle (1985), DOI 10.2307/1913210; Easley, López de Prado & O'Hara (2012), DOI 10.1093/rfs/hhs053.

---

## 16. Model governance and human oversight

Following the repository's operating protocol:

- **Research and execution are separate.** No autonomous order placement is authorized.
- **Every observation carries a source, timestamp, timezone, retrieval time, and confidence.**
- **Distinguish facts, estimates, hypotheses, model outputs, and decisions.**
- **Models require documentation:** purpose, owner, version, training/test period, features, data lineage, assumptions, limitations, benchmark, metrics, drift checks, approval status.
- **Human approval is required** before any trade plan is executed.
- **Post-trade review** records what happened, what changed, and what was unknown.

Model inventory, version control, data lineage, validation reports, and an investment committee are the minimum governance layer. Kill criteria (§12.4) must be pre-committed, not decided ad hoc during a drawdown.

---

## 17. A 12-week research plan

| Week | Focus | Deliverable |
|---|---|---|
| 1–2 | Foundations and data audit | Data-source registry, point-in-time pipeline, survivorship-bias check, freshness logs. |
| 3–4 | Baseline strategies | Trend and mean-reversion signals with a cost model; initial walk-forward results. |
| 5–6 | Fat-tail risk | GARCH/stochastic-volatility forecasts, CVaR limits, stress-test scenarios. |
| 7–8 | Options/volatility module | BSM calculator, implied-vol surface, delta-hedging simulation, realized-vs-implied P&L. |
| 9–10 | Validation | Purged cross-validation, PBO, deflated Sharpe, multiple-testing adjustment, regime splits. |
| 11–12 | Paper portfolio and governance | Kill criteria, scorecard, drawdown rules, human-approval workflow, decision memo. |

Deliverables are research artifacts, not trading authorization.

---

## 18. What the video gets right, wrong, and omits

| Dimension | Right | Oversimplified / omitted |
|---|---|---|
| Origin story | Bachelier → Einstein → BSM is accurate | Omits that Bachelier's model allowed negative prices and was ignored for decades. |
| Normal distribution | Correctly identifies it as central | Implies real returns are close to normal; empirical fat tails are the exception in the model but the rule in markets. |
| The flaw | Fat tails and black swans are real | Treats fat tails as the only flaw; omits jumps, stochastic volatility, liquidity, leverage, and model risk. |
| Trillion-dollar claim | BSM enabled derivatives markets | Overstates causality; exchanges, clearing, regulation, computing, and demand mattered too. |
| Simons / Medallion | Correctly identifies a quant landmark | Omits capacity constraints, closure to outsiders, fees, data, execution, and that Medallion is not a BSM strategy. |
| Thales / Newton | Illustrative anecdotes | Anecdotes are not evidence of a general pricing theory or of quant skill. |
| BSM's role | Framed as pricing equation | Omits that BSM is a relative-pricing/no-arbitrage relationship, not a return forecast. |
| Model risk | Acknowledged as Achilles' heel | Does not explain that extensions exist and that BSM remains a quoting/hedging language. |

**Bottom line:** The video is an excellent popular account of how mathematics changed markets. It is not a trading manual, and its conclusions about fat tails should lead to more robust risk management, not to abandoning models entirely.

---

## 19. Evidence table

| # | Claim | Classification | Confidence | Primary evidence | Key caveat |
|---|---|---|---|---|---|
| 1 | BSM is a no-arbitrage PDE derived from delta hedging. | FACT | High | Black & Scholes (1973); Merton (1973) | Model assumptions are strong; empirical fit is imperfect. |
| 2 | BSM assumes GBM/lognormal returns and continuous hedging. | FACT | High | Black & Scholes (1973); Merton (1973) | Real markets violate continuity and constant volatility. |
| 3 | Real asset returns have fat tails. | FACT | High | Mandelbrot (1963); Fama (1965); Cont (2001) | Tail index and exact form are sample-dependent. |
| 4 | Volatility clusters and is time-varying. | FACT | High | Engle (1982); Bollerslev (1986); Cont (2001) | GARCH is one model among many; not a complete description. |
| 5 | Jumps invalidate continuous replication. | FACT | High | Merton (1976) | Jump parameters are latent and hard to estimate. |
| 6 | Implied-volatility smile/skew persists post-1987. | FACT | High | Rubinstein (1994); Derman et al. (1996); Bates (2000) | Smile is a quoting convention as much as a distribution forecast. |
| 7 | Stochastic volatility and jumps improve empirical fit. | FACT / ESTIMATE | High–Medium | Heston (1993); Bakshi et al. (1997); Eraker et al. (2003) | Better fit does not guarantee better hedging out of sample. |
| 8 | BSM is a relative-pricing model, not a return forecast. | ANALYSIS | High | Derivation (§3.2); market practice | The drift of the stock cancels out. |
| 9 | Delta-hedged P&L depends on realized vs implied variance. | MODEL OUTPUT | High | BSM Greeks; Gatheral (2006) | Approximate; discrete hedging and jumps add error. |
| 10 | Backtest overfitting is a documented, pervasive risk. | FACT | High | Bailey et al. (2017); Sullivan et al. (1999); White (2000) | Defenses reduce but do not eliminate overfitting. |
| 11 | Multiple testing inflates t-statistics; higher bars are needed. | FACT / ESTIMATE | High | Harvey, Liu & Zhu (2016) | Thresholds (e.g., t > 3) are rules of thumb. |
| 12 | PBO and deflated Sharpe improve strategy selection. | MODEL OUTPUT / ESTIMATE | Medium–High | Bailey et al. (2017); Bailey & López de Prado (2014) | Still assumption-dependent; use with out-of-sample tests. |
| 13 | Trend/momentum has long-sample evidence across asset classes. | FACT / ESTIMATE | Medium–High | Moskowitz et al. (2012); Hurst et al. (2017) | Returns vary by regime, costs, and capacity. |
| 14 | Value, momentum, quality, and carry are pervasive but not free. | FACT / ESTIMATE | Medium | Asness et al. (2013, 2019); Koijen et al. (2018); Frazzini & Pedersen (2014) | Factor crowding and drawdowns are real. |
| 15 | Many published anomalies fail replication. | FACT | High | Hou, Xue & Zhang (2020); Harvey et al. (2016) | "Fail" means after costs and multiple-testing adjustment. |
| 16 | Medallion's reported returns are extraordinary. | FACT (reported) | Medium–High | Cornell (2020); Zuckerman (2019) | Not audited; reconstruction from an appendix. |
| 17 | Medallion's returns are not explained by market or Fama-French risk. | ESTIMATE | Medium | Cornell (2020) | Based on reported data; no public audit. |
| 18 | Medallion-like returns are not publicly replicable. | HYPOTHESIS / UNKNOWN | Medium–High | Capacity, closure, and access barriers | Could change if a public audited vehicle emerges. |
| 19 | LTCM's leverage and liquidity mismatch were central to its collapse. | FACT | High | Edwards (1999); Federal Reserve History | Interpretation of model blame is ANALYSIS. |
| 20 | Fat-tailed risk requires CVaR and stress tests, not variance alone. | FACT / ANALYSIS | High | Acerbi & Tasche (2002); McNeil et al. (2015) | CVaR is coherent but still model-dependent. |
| 21 | Volatility targeting can improve risk-adjusted returns but is not a panacea. | ESTIMATE | Medium | Moreira & Muir (2017) | Implementation, leverage caps, and regime risk matter. |
| 22 | Execution costs and capacity can erase high-turnover alpha. | FACT / ESTIMATE | High | Almgren & Chriss (2001); Kyle (1985) | Magnitude is strategy- and venue-specific. |
| 23 | Human governance is required for model risk. | DECISION | High | Repository operating protocol | Governance reduces but does not eliminate risk. |

---

## 20. Annotated bibliography

### 20.1 Foundations and option pricing (peer-reviewed, DOI verified)

- Bachelier, L. (1900). *Théorie de la spéculation*. Annales scientifiques de l'École Normale Supérieure, 17, 21–86. https://doi.org/10.24033/asens.476
- Einstein, A. (1905). *Über die von der molekularkinetischen Theorie der Wärme geforderte Bewegung von in ruhenden Flüssigkeiten suspendierten Teilchen*. Annalen der Physik, 322(8), 549–560. https://doi.org/10.1002/andp.19053220806
- Black, F., & Scholes, M. (1973). The Pricing of Options and Corporate Liabilities. Journal of Political Economy, 81(3), 637–654. https://doi.org/10.1086/260062
- Merton, R. C. (1973). Theory of Rational Option Pricing. The Bell Journal of Economics and Management Science, 4(1), 141–183. https://doi.org/10.2307/3003143
- Cox, J. C., Ross, S. A., & Rubinstein, M. (1979). Option Pricing: A Simplified Approach. Journal of Financial Economics, 7(3), 229–263. https://doi.org/10.1016/0304-405X(79)90015-1
- Merton, R. C. (1976). Option Pricing When Underlying Stock Returns Are Discontinuous. Journal of Financial Economics, 3(1–2), 125–144. https://doi.org/10.1016/0304-405X(76)90022-2
- Heston, S. L. (1993). A Closed-Form Solution for Options with Stochastic Volatility with Applications to Bond and Currency Options. Review of Financial Studies, 6(2), 327–343. https://doi.org/10.1093/rfs/6.2.327
- Bates, D. S. (1996). Jumps and Stochastic Volatility: Exchange Rate Processes Implicit in Deutsche Mark Options. Review of Financial Studies, 9(1), 69–107. https://doi.org/10.1093/rfs/9.1.69
- Rubinstein, M. (1994). Implied Binomial Trees. The Journal of Finance, 49(3), 771–818. https://doi.org/10.1111/j.1540-6261.1994.tb00079.x
- Jackwerth, J. C., & Rubinstein, M. (1996). Recovering Probability Distributions from Option Prices. The Journal of Finance, 51(5), 1611–1631. https://doi.org/10.1111/j.1540-6261.1996.tb05219.x
- Derman, E., Kani, I., & Chriss, N. (1996). Implied Trinomial Trees of the Volatility Smile. The Journal of Derivatives, 3(4), 7–22. https://doi.org/10.3905/jod.1996.407952
- Bates, D. S. (2000). Post-'87 Crash Fears in the S&P 500 Futures Option Market. Journal of Econometrics, 94(1–2), 181–238. https://doi.org/10.1016/S0304-4076(99)00021-4
- Bakshi, G., Cao, C., & Chen, Z. (1997). Empirical Performance of Alternative Option Pricing Models. The Journal of Finance, 52(5), 2003–2049. https://doi.org/10.1111/j.1540-6261.1997.tb02749.x
- Andersen, T. G., Benzoni, L., & Lund, J. (2002). An Empirical Investigation of Continuous-Time Equity Return Models. The Journal of Finance, 57(3), 1239–1284. https://doi.org/10.1111/1540-6261.00460
- Eraker, B., Johannes, M. S., & Polson, N. (2003). The Impact of Jumps in Volatility and Returns. The Journal of Finance, 58(3), 1269–1300. https://doi.org/10.1111/1540-6261.00566
- Carr, P., & Wu, L. (2004). Time-Changed Lévy Processes and Option Pricing. Journal of Financial Economics, 71(1), 113–141. https://doi.org/10.1016/S0304-405X(03)00171-5

### 20.2 Empirical distributions and volatility (peer-reviewed, DOI verified)

- Mandelbrot, B. B. (1963). The Variation of Certain Speculative Prices. The Journal of Business, 36(4), 394–419. https://doi.org/10.1086/294632
- Fama, E. F. (1965). The Behavior of Stock-Market Prices. The Journal of Business, 38(1), 34–105. https://doi.org/10.1086/294743
- Engle, R. F. (1982). Autoregressive Conditional Heteroscedasticity with Estimates of the Variance of United Kingdom Inflation. Econometrica, 50(4), 987–1007. https://doi.org/10.2307/1912773
- Bollerslev, T. (1986). Generalized Autoregressive Conditional Heteroskedasticity. Journal of Econometrics, 31(3), 307–327. https://doi.org/10.1016/0304-4076(86)90063-1
- Lo, A. W. (1991). Long-Term Memory in Stock Market Prices. Econometrica, 59(5), 1279–1313. https://doi.org/10.2307/2938368
- Cont, R. (2001). Empirical Properties of Asset Returns: Stylized Facts and Statistical Issues. Quantitative Finance, 1(2), 223–236. https://doi.org/10.1080/713665670
- Barndorff-Nielsen, O. E., & Shephard, N. (2002). Econometric Analysis of Realized Volatility and Its Use in Estimating Stochastic Volatility Models. Journal of the Royal Statistical Society Series B, 64(2), 253–280. https://doi.org/10.1111/1467-9868.00336
- Andersen, T. G., Bollerslev, T., Diebold, F. X., & Labys, P. (2003). Modeling and Forecasting Realized Volatility. Econometrica, 71(2), 579–625. https://doi.org/10.1111/1468-0262.00418
- Hansen, P. R., & Lunde, A. (2005). A Forecast Comparison of Volatility Models: Does Anything Beat a GARCH(1,1)? Journal of Applied Econometrics, 20(7), 873–889. https://doi.org/10.1002/jae.800
- Corsi, F. (2009). A Simple Approximate Long-Memory Model of Realized Volatility. Journal of Financial Econometrics, 7(2), 174–196. https://doi.org/10.1093/jjfinec/nbp001
- Patton, A. J. (2011). Volatility Forecast Comparison Using Imperfect Volatility Proxies. Journal of Econometrics, 160(1), 246–256. https://doi.org/10.1016/j.jeconom.2010.03.034
- Ang, A., Hodrick, R. J., Xing, Y., & Zhang, X. (2006). The Cross-Section of Volatility and Expected Returns. The Journal of Finance, 61(1), 259–299. https://doi.org/10.1111/j.1540-6261.2006.00836.x
- Taleb, N. N. (2007). Black Swans and the Domains of Statistics. The American Statistician, 61(3), 198–200. https://doi.org/10.1198/000313007X219996
- Peters, E. E. (2019). The Ergodicity Problem in Economics. Nature Physics, 15(12), 1216–1221. https://doi.org/10.1038/s41567-019-0732-0

### 20.3 Quant research methodology (peer-reviewed, DOI verified)

- Sullivan, R., Timmermann, A., & White, H. (1999). Data-Snooping, Technical Trading Rule Performance, and the Bootstrap. The Journal of Finance, 54(5), 1647–1691. https://doi.org/10.1111/0022-1082.00163
- White, H. (2000). A Reality Check for Data Snooping. Econometrica, 68(5), 1097–1126. https://doi.org/10.1111/1468-0262.00152
- Politis, D. N., & Romano, J. P. (1994). The Stationary Bootstrap. Journal of the American Statistical Association, 89(428), 1303–1313. https://doi.org/10.1080/01621459.1994.10476870
- Harvey, C. R., Liu, Y., & Zhu, H. (2016). …and the Cross-Section of Expected Returns. Review of Financial Studies, 29(1), 5–68. https://doi.org/10.1093/rfs/hhv059
- Bailey, D. H., Borwein, J. M., López de Prado, M., & Zhu, Q. J. (2017). The Probability of Backtest Overfitting. Journal of Computational Finance, 20(4), 39–69. https://doi.org/10.21314/JCF.2016.322
- Bailey, D. H., Borwein, J. M., López de Prado, M., & Zhu, Q. J. (2014). Pseudo-Mathematics and Financial Charlatanism: The Effects of Backtest Overfitting on Out-of-Sample Performance. Notices of the American Mathematical Society, 61(5), 458–471. https://doi.org/10.1090/noti1105
- Bailey, D. H., & López de Prado, M. (2014). The Deflated Sharpe Ratio: Correcting for Selection Bias, Backtest Overfitting, and Non-Normality. The Journal of Portfolio Management, 40(5), 94–107. https://doi.org/10.3905/jpm.2014.40.5.094
- López de Prado, M. (2018). The 10 Reasons Most Machine Learning Funds Fail. The Journal of Portfolio Management, 44(6), 120–133. https://doi.org/10.3905/jpm.2018.44.6.120
- Hou, K., Xue, C., & Zhang, L. (2020). Replicating Anomalies. The Review of Financial Studies, 33(5), 2019–2133. https://doi.org/10.1093/rfs/hhy131
- Feng, G., Giglio, S., & Xiu, D. (2020). Taming the Factor Zoo: A Test of New Factors. The Journal of Finance, 75(3), 1327–1370. https://doi.org/10.1111/jofi.12883

### 20.4 Factors, trend, and cross-sectional evidence (peer-reviewed, DOI verified)

- Fama, E. F., & French, K. R. (1992). The Cross-Section of Expected Stock Returns. The Journal of Finance, 47(2), 427–465. https://doi.org/10.1111/j.1540-6261.1992.tb04398.x
- Fama, E. F., & French, K. R. (1993). Common Risk Factors in the Returns on Stocks and Bonds. Journal of Financial Economics, 33(1), 3–56. https://doi.org/10.1016/0304-405X(93)90023-5
- Jegadeesh, N., & Titman, S. (1993). Returns to Buying Winners and Selling Losers: Implications for Stock Market Efficiency. The Journal of Finance, 48(1), 65–91. https://doi.org/10.1111/j.1540-6261.1993.tb04702.x
- De Bondt, W. F. M., & Thaler, R. H. (1985). Does the Stock Market Overreact? The Journal of Finance, 40(3), 793–805. https://doi.org/10.1111/j.1540-6261.1985.tb05004.x
- Moskowitz, T. J., Ooi, Y. H., & Pedersen, L. H. (2012). Time Series Momentum. Journal of Financial Economics, 104(2), 228–250. https://doi.org/10.1016/j.jfineco.2011.11.003
- Asness, C. S., Moskowitz, T. J., & Pedersen, L. H. (2013). Value and Momentum Everywhere. The Journal of Finance, 68(3), 929–985. https://doi.org/10.1111/jofi.12021
- Koijen, R. S. J., Moskowitz, T. J., Pedersen, L. H., & Vrugt, E. B. (2018). Carry. Journal of Financial Economics, 127(2), 197–225. https://doi.org/10.1016/j.jfineco.2017.11.002
- Frazzini, A., & Pedersen, L. H. (2014). Betting Against Beta. Journal of Financial Economics, 111(1), 1–25. https://doi.org/10.1016/j.jfineco.2013.10.005
- Asness, C. S., Frazzini, A., & Pedersen, L. H. (2019). Quality Minus Junk. Review of Accounting Studies, 24(1), 34–112. https://doi.org/10.1007/s11142-018-9470-2
- Moreira, A., & Muir, T. (2017). Volatility-Managed Portfolios. The Journal of Finance, 72(4), 1611–1644. https://doi.org/10.1111/jofi.12513
- Hurst, B., Ooi, Y. H., & Pedersen, L. H. (2017). A Century of Evidence on Trend-Following Investing. The Journal of Portfolio Management, 44(1), 15–29. https://doi.org/10.3905/jpm.2017.44.1.015

### 20.5 Market microstructure and execution (peer-reviewed, DOI verified)

- Kyle, A. S. (1985). Continuous Auctions and Insider Trading. Econometrica, 53(6), 1315–1326. https://doi.org/10.2307/1913210
- Almgren, R., & Chriss, N. (2001). Optimal Execution of Portfolio Transactions. The Journal of Risk, 3(2), 5–39. https://doi.org/10.21314/jor.2001.041
- Easley, D., López de Prado, M., & O'Hara, M. (2012). Flow Toxicity and Liquidity in a High-Frequency World. Review of Financial Studies, 25(5), 1457–1493. https://doi.org/10.1093/rfs/hhs053

### 20.6 Risk, leverage, and tail events (peer-reviewed, DOI verified)

- Kelly, J. L. (1956). A New Interpretation of Information Rate. Bell System Technical Journal, 35(4), 917–926. https://doi.org/10.1002/j.1538-7305.1956.tb03809.x
- Thorp, E. O. (2008). The Kelly Criterion in Blackjack Sports Betting, and the Stock Market. In Handbook of Asset and Liability Management, 385–428. https://doi.org/10.1016/b978-044453248-0.50015-0
- Sharpe, W. F. (1966). Mutual Fund Performance. The Journal of Business, 39(1), 119–138. https://doi.org/10.1086/294846
- Lo, A. W. (2002). The Statistics of Sharpe Ratios. Financial Analysts Journal, 58(4), 36–52. https://doi.org/10.2469/faj.v58.n4.2453
- Acerbi, C., & Tasche, D. (2002). On the Coherence of Expected Shortfall. Journal of Banking & Finance, 26(7), 1487–1503. https://doi.org/10.1016/S0378-4266(02)00283-2
- Rockafellar, R. T., & Uryasev, S. (2000). Optimization of Conditional Value-at-Risk. The Journal of Risk, 2(3), 21–41. https://doi.org/10.21314/JOR.2000.038
- Li, D. X. (2000). On Default Correlation: A Copula Function Approach. The Journal of Fixed Income, 9(4), 43–54. https://doi.org/10.3905/jfi.2000.319253
- Edwards, F. R. (1999). Hedge Funds and the Collapse of Long-Term Capital Management. Journal of Economic Perspectives, 13(2), 189–210. https://doi.org/10.1257/jep.13.2.189
- Cornell, B. (2020). Medallion Fund: The Ultimate Counterexample? The Journal of Portfolio Management, 46(4), 156–159. https://doi.org/10.3905/jpm.2020.1.128

### 20.7 Books (ISBN verified via OpenLibrary, edition-specific)

| Author(s) | Title | Year | Publisher | ISBN-13 | Relevance |
|---|---|---|---|---|---|
| Hull, J. C. | Options, Futures, and Other Derivatives (10th ed.) | 2017 | Pearson | 978-0-13-447208-9 | Standard derivatives textbook; BSM and Greeks. |
| Natenberg, S. | Option Volatility and Pricing (2nd ed.) | 2014 | McGraw-Hill | 978-0-07-181877-3 | Practical options trading and vol surfaces. |
| Gatheral, J. | The Volatility Surface | 2006 | Wiley | 978-0-471-79251-2 | Implied volatility surface and smiles. |
| Taleb, N. N. | Dynamic Hedging | 1997 | Wiley | 978-0-471-15280-4 | Options hedging, gamma, and model risk. |
| Taleb, N. N. | The Black Swan (2nd ed.) | 2010 | Random House | 978-0-8129-7381-5 | Fat tails, black swans, and limits of prediction. |
| Taleb, N. N. | Fooled by Randomness | 2005 | Random House | 978-0-8129-7521-5 | Luck, skill, and survivorship in trading. |
| Taleb, N. N. | Statistical Consequences of Fat Tails | 2020 | STEM Academic Press | 978-1-5445-0805-4 | Technical treatment of fat-tailed risk. |
| Mandelbrot, B. B., & Hudson, R. L. | The (Mis)Behavior of Markets | 2004 | Basic Books | 978-0-465-00468-3 | Fractals, fat tails, and market criticism. |
| Zuckerman, G. | The Man Who Solved the Market | 2019 | Portfolio/Penguin | 978-0-7352-1798-0 | Renaissance Technologies and Medallion. |
| Mallaby, S. | More Money Than God | 2010 | Penguin Press | 978-0-14-311941-8 | Hedge fund history, including quant funds. |
| Patterson, S. | The Quants | 2010 | Crown Business | 978-0-307-45337-2 | Rise of quantitative hedge funds. |
| Lowenstein, R. | When Genius Failed | 2000 | Random House | 978-0-375-75825-6 | LTCM collapse and leverage risk. |
| Thorp, E. O. | A Man for All Markets | 2017 | Random House | 978-0-8129-7990-9 | Ed Thorp, card counting, and early quant trading. |
| López de Prado, M. | Advances in Financial Machine Learning | 2018 | Wiley | 978-1-119-48208-6 | Purged CV, meta-labeling, backtest overfitting. |
| López de Prado, M. | Machine Learning for Asset Managers | 2020 | Cambridge University Press | 978-1-108-79289-9 | ML methods for portfolio management. |
| Hastie, T., Tibshirani, R., & Friedman, J. | The Elements of Statistical Learning (2nd ed.) | 2009 | Springer | 978-0-387-84857-0 | Statistical learning foundations. |
| James, G., Witten, D., Hastie, T., & Tibshirani, R. | An Introduction to Statistical Learning (2nd ed.) | 2021 | Springer | 978-1-071-61417-4 | Applied statistical learning. |
| Goodfellow, I., Bengio, Y., & Courville, A. | Deep Learning | 2016 | MIT Press | 978-0-262-03561-3 | Deep learning textbook. |
| Sutton, R. S., & Barto, A. G. | Reinforcement Learning: An Introduction (2nd ed.) | 2018 | MIT Press | 978-0-262-03924-6 | Reinforcement learning foundations. |
| Shreve, S. E. | Stochastic Calculus for Finance II: Continuous-Time Models | 2004 | Springer | 978-0-387-40101-0 | Continuous-time finance and BSM derivation. |
| Glasserman, P. | Monte Carlo Methods in Financial Engineering | 2003 | Springer | 978-0-387-00451-8 | Simulation methods for derivatives. |
| Wilmott, P. | Paul Wilmott on Quantitative Finance (2nd ed.) | 2006 | Wiley | 978-0-470-01870-5 | Quantitative finance reference. |
| Joshi, M. S. | The Concepts and Practice of Mathematical Finance (2nd ed.) | 2008 | Cambridge University Press | 978-0-521-51408-8 | Mathematical finance introduction. |
| Wilmott, P., Howison, S., & Dewynne, J. | The Mathematics of Financial Derivatives | 1995 | Cambridge University Press | 978-0-521-49789-3 | BSM PDE and heat-equation connection. |
| Karatzas, I., & Shreve, S. E. | Brownian Motion and Stochastic Calculus (2nd ed.) | 1991 | Springer | 978-0-387-97655-6 | Stochastic calculus foundations. |
| Mantegna, R. N., & Stanley, H. E. | An Introduction to Econophysics | 1999 | Cambridge University Press | 978-0-521-62008-6 | Physics methods in finance. |
| Sornette, D. | Why Stock Markets Crash | 2017 | Princeton University Press | 978-0-691-17595-9 | Crashes, bubbles, and critical phenomena. |
| Voit, J. | The Statistical Mechanics of Financial Markets | 2003 | Springer | 978-3-540-00978-8 | Econophysics and statistical mechanics. |
| Peters, E. E. | Fractal Market Analysis | 1994 | Wiley | 978-0-471-58524-4 | Fractals and market efficiency. |
| Vince, R. | The Mathematics of Money Management | 1992 | Wiley | 978-0-471-54738-9 | Position sizing and Kelly. |
| MacLean, L. C., Thorp, E. O., & Ziemba, W. T. (eds.) | The Kelly Capital Growth Investment Criterion | 2011 | World Scientific | 978-981-429-349-5 | Kelly criterion theory and practice. |
| Chan, E. P. | Quantitative Trading | 2009 | Wiley | 978-0-470-28488-9 | Retail quant strategy framework. |
| Chan, E. P. | Algorithmic Trading | 2013 | Wiley | 978-1-118-46014-6 | Algorithmic strategy implementation. |
| Harris, L. | Trading and Exchanges | 2002 | Oxford University Press | 978-0-19-514470-3 | Market microstructure and trading mechanics. |
| Hasbrouck, J. | Empirical Market Microstructure | 2007 | Oxford University Press | 978-0-19-530164-9 | Empirical microstructure methods. |
| Guida, T. (ed.) | Big Data and Machine Learning in Quantitative Investment | 2019 | Wiley | 978-1-119-52219-5 | Big data and ML in investing. |
| Pardo, R. | The Evaluation and Optimization of Trading Strategies | 2008 | Wiley | 978-0-470-12801-5 | Backtesting methodology and statistics. |
| Davey, K. | Building Winning Algorithmic Trading Systems | 2014 | Wiley | 978-1-118-77898-2 | Algorithmic system development. |
| Aronson, D. | Evidence-Based Technical Analysis | 2006 | Wiley | 978-0-470-00874-4 | Testing technical rules rigorously. |
| Carver, R. | Systematic Trading | 2015 | Harriman House | 978-0-85719-445-9 | Systematic rule-based trading. |
| Carver, R. | Leveraged Trading | 2019 | Harriman House | 978-0-85719-721-4 | Leverage, risk, and position sizing. |
| Vince, R. | The Handbook of Portfolio Mathematics | 2007 | Wiley | 978-0-471-75768-9 | Portfolio mathematics and leverage. |
| Rachev, S. T., et al. | Fat-Tailed and Skewed Asset Return Distributions | 2005 | Wiley | 978-0-471-71886-4 | Heavy-tailed distributions in finance. |
| Rachev, S. T., & Mittnik, S. | Stable Paretian Models in Finance | 2000 | Wiley | 978-0-471-95314-2 | Stable distributions and finance. |
| Mandelbrot, B. B. | The Fractal Geometry of Nature | 1982 | W. H. Freeman | 978-0-7167-1186-5 | Fractals and roughness. |
| Wilmott, P. | Derivatives: The Theory and Practice of Financial Engineering | 1998 | Wiley | 978-0-471-98389-7 | Derivatives pricing and engineering. |
| Campbell, J. Y., Lo, A. W., & MacKinlay, A. C. | The Econometrics of Financial Markets | 1997 | Princeton University Press | 978-0-691-04301-2 | Financial econometrics. |
| Tsay, R. S. | Analysis of Financial Time Series (3rd ed.) | 2010 | Wiley | 978-0-470-41435-4 | Financial time-series econometrics. |
| Hamilton, J. D. | Time Series Analysis | 1994 | Princeton University Press | 978-0-691-04289-3 | Time-series econometrics. |
| Ilmanen, A. | Expected Returns | 2011 | Wiley | 978-1-119-99072-7 | Risk premia and expected returns. |
| Grinold, R. C., & Kahn, R. N. | Active Portfolio Management (2nd ed.) | 2000 | McGraw-Hill | 978-0-07-024882-3 | Active management framework. |
| Derman, E., & Miller, M. | The Volatility Smile | 2016 | Wiley | 978-1-119-28925-8 | Volatility trading and smile dynamics. |
| Chriss, N. | Black-Scholes and Beyond: Option Pricing Models | 1997 | McGraw-Hill | 978-0-7863-1025-8 | Option pricing models beyond BSM. |
| McNeil, A. J., Frey, R., & Embrechts, P. | Quantitative Risk Management | 2015 | Princeton University Press | 978-0-691-16627-8 | Quantitative risk management. |

### 20.8 Non-DOI sources and investigative journalism

- Veritasium. (2024). *The Trillion Dollar Equation*. Published 2024-02-28. https://www.veritasium.com/videos/2024/2/28/the-trillion-dollar-equation
- Cornell, B. (2019/2020). *Medallion Fund: The Ultimate Counterexample?* SSRN abstract 3504766; Cornell Capital Group PDF. https://www.cornell-capital.com/uploads/papers/medallion-fund.pdf
- Federal Reserve History. *Near Failure of Long-Term Capital Management*. https://www.federalreservehistory.org/
- Merton, R. C. (1976). *Option Pricing When Underlying Stock Returns Are Discontinuous* (author PDF). http://www.cmat.edu.uy/~mordecki/hk2010/merton76.pdf
- Cont, R. (2001). *Empirical Properties of Asset Returns* (author PDF). http://rama.cont.perso.math.cnrs.fr/pdf/empirical.pdf
- Derman, E. (1994). *The Volatility Smile and Its Implied Tree*. Emanuel Derman website. https://emanuelderman.com/the-volatility-smile-and-its-implied-tree/
- Derman, E. (2003). *Trading Volatility* (GAIM presentation). https://emanuelderman.com/wp-content/uploads/2003/05/gaim-trading_volatility.pdf
- Jacobs, B. I. (1999). *The October 1987 Crash and Portfolio Insurance* (non-DOI journal article). Cited for the portfolio-insurance mechanism.
- BIS. OTC derivatives statistics. https://www.bis.org/statistics/derstats.htm (for current notional scale; no specific figure cited here).

---

## 21. Verification notes and limitations

**Verification method.** All DOIs in §20 were resolved through the Crossref API on 2026-11-11 and returned matching titles, journals, and years. ISBNs in §20.7 were checked through OpenLibrary on 2026-11-11; they refer to specific editions and may differ from other printings. Book metadata without an author name in OpenLibrary was completed from publisher records and standard citations.

**Medallion data.** The Medallion figures come from Cornell (2020), which reconstructs Zuckerman (2019). They are not audited public disclosures. Gross returns are after trading costs but before management fees; net returns reflect Medallion's fee schedule. The table header in Cornell (2020) reads "1998 to 2018" but the data begin in 1988; this document treats the sample as 1988–2018. Net-of-fees 1989 was negative (−3.20%) even though gross was positive.

**Scope limitations.** This is a curated synthesis, not a systematic review. It emphasizes English-language canonical sources. Historical anecdotes (Thales, Newton) are narrative devices, not evidence. No live market data were used, and no strategy in this document has been backtested or traded.

**Not investment advice.** The hypotheses, scorecard gates, kill criteria, and research plan are research proposals. They require human review, data validation, cost modeling, out-of-sample testing, and explicit approval before any capital is allocated.

---

## Appendix A. Formula sheet

**Geometric Brownian motion:**
\[
dS_t = \mu S_t\,dt + \sigma S_t\,dW_t
\]

**Black-Scholes-Merton PDE:**
\[
\frac{\partial V}{\partial t} + \frac{1}{2}\sigma^2 S^2 \frac{\partial^2 V}{\partial S^2} + rS\frac{\partial V}{\partial S} - rV = 0
\]

**European call:**
\[
C = S_0 e^{-qT}\Phi(d_1) - K e^{-rT}\Phi(d_2), \quad d_{1,2} = \frac{\ln(S_0/K) + (r - q \pm \frac{1}{2}\sigma^2)T}{\sigma\sqrt{T}}
\]

**Greeks:**
\[
\Delta_C = e^{-qT}\Phi(d_1), \quad \Gamma = \frac{e^{-qT}\phi(d_1)}{S_0\sigma\sqrt{T}}, \quad \mathcal{V} = S_0 e^{-qT}\phi(d_1)\sqrt{T}
\]

**Delta-hedged vol P&L (approx.):**
\[
d\Pi \approx \frac{1}{2}\Gamma S^2 \left(\sigma_{\text{realized}}^2 - \sigma_{\text{implied}}^2\right)dt
\]

**GARCH(1,1):**
\[
\sigma_t^2 = \omega + \alpha \varepsilon_{t-1}^2 + \beta \sigma_{t-1}^2
\]

**Kelly fraction (continuous approximation):**
\[
f^* \approx \frac{\mu - r}{\sigma^2}
\]

**CVaR:**
\[
\text{CVaR}_\alpha = \mathbb{E}[L \mid L \ge \text{VaR}_\alpha]
\]

---

## Appendix B. Backtest and model-risk checklist

- [ ] Hypothesis stated before data inspection.
- [ ] Point-in-time data; no survivorship or delisting bias.
- [ ] Timestamps, timezones, currency, and corporate-action adjustments recorded.
- [ ] No look-ahead features; all signals lagged appropriately.
- [ ] Transaction costs, spread, slippage, market impact, borrow, financing, and taxes included.
- [ ] Train/validation/test split is chronological; no leakage.
- [ ] Purged cross-validation with embargo used for hyperparameter tuning.
- [ ] PBO and deflated Sharpe reported.
- [ ] Multiple-testing adjustment applied (e.g., t-stat > 3 for new factors).
- [ ] Walk-forward results stable across regimes (calm, crisis, high/low vol).
- [ ] Capacity estimated and target AUM below capacity with margin.
- [ ] CVaR and stress tests computed (1987, 1998, 2008, 2020).
- [ ] Drawdown limit, vol cap, leverage cap, and kill criteria defined.
- [ ] Model documented: owner, version, data lineage, assumptions, limitations.
- [ ] Human approval recorded; no autonomous execution.

---

## Appendix C. Experiment record template

```
EXPERIMENT ID: EXP-YYYYMMDD-##
Title:
Strategy family:
Hypothesis (one sentence):
Universe and instrument identifiers:
Data source / sample period (point-in-time?):
Feature definitions:
Entry / exit / sizing rules:
Benchmark:
Transaction cost model:
Train / validation / test split:
Metrics (net return, DD, vol, Sharpe/Sortino, CVaR, hit rate, payoff, turnover):
Regime robustness results:
Parameter stability / sensitivity:
Known biases and limitations:
PBO / deflated Sharpe:
Result: pass / inconclusive / fail
Human review and date:
Decision status: research only / monitor / proposed / human-approved / rejected
Invalidation / next test:
```

This template mirrors `docs/RESEARCH_FINDINGS_LOG.md`.

---

## Appendix D. Glossary

- **Alpha:** return in excess of a benchmark after adjusting for risk.
- **Beta:** sensitivity to a market factor.
- **BSM:** Black-Scholes-Merton option-pricing framework.
- **CVaR / Expected Shortfall:** expected loss beyond VaR at a confidence level.
- **Delta / Gamma / Vega / Theta:** option sensitivities to price, price-squared, volatility, and time.
- **Fat tail:** tail heavier than the normal distribution; extreme events more likely.
- **Implied volatility:** volatility input that makes BSM price equal market price.
- **Kelly criterion:** growth-optimal bet sizing rule.
- **Look-ahead bias:** using future information in training features.
- **PBO:** probability of backtest overfitting.
- **Smile / skew:** pattern of implied volatility across strikes.
- **Survivorship bias:** including only assets that survived to the present.
- **Turnover:** frequency of position changes; drives transaction costs.
- **VaR:** value at risk; a quantile of the loss distribution.
- **Volatility clustering:** high-volatility periods tend to follow high-volatility periods.

---

**End of synthesis.** Prepared for the Trading Analysis Engine research workspace. Status: research-only. No autonomous trading is authorized by this document.
