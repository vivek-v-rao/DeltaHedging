# DeltaHedging
Delta hedging of European options in the Black-Scholes framework, with transaction costs and different rebalancing frequencies. Sample output:

```
Option Parameters:
-----------------
Option Position:                -1.0000
Initial Stock Price (S0):      100.0000
Strike Price (K):              100.0000
Time to Maturity (T):            1.0000
Risk-free Rate (r):              0.0000
Implied Volatility (sigma):      0.2200
Realized Volatility:             0.2000
Transaction Cost:                0.0010
Number of Simulations:           100000

Delta Hedging Simulation Results
--------------------------------
Rebalancing    Mean Profit    Std Dev    Mean/Std
-----------    -----------    -------    --------
          0         0.7165      6.0284      0.1189
          1         0.6707      6.0354      0.1111
          4         0.6307      3.2691      0.1929
         12         0.5965      1.9617      0.3041
         52         0.4688      0.9903      0.4734
        252         0.2073      0.4675      0.4433
        504         0.0089      0.3306      0.0270

Black-Scholes Price (Implied Volatility):      8.7591
Black-Scholes Price (Realized Volatility):     7.9656
BS Price Difference (Implied - Realized):      0.7935
Expected Profit (Ignoring T-Costs):            0.7935
Best Rebalancing Frequency (Max Mean/Std):         52

Elapsed Time: 14.5970 seconds
```
