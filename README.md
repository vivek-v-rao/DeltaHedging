# DeltaHedging
This program simulates delta hedging of European options in the Black-Scholes framework, with transaction costs and different rebalancing frequencies. Hedging more often reduces variance but increases transaction costs. The `Option Type` can be `Call`, `Put`, or `Straddle`. Sample output:

```
Option Parameters:
-----------------
Option Type:                   Straddle
Option Position:                -1.0000
Initial Stock Price (S0):      100.0000
Strike Price (K):              100.0000
Time to Maturity (T):            1.0000
Risk-free Rate (r):              0.0000
Implied Volatility (sigma):      0.2500
Realized Volatility:             0.2000
Transaction Cost:                0.0010
Number of Simulations:            10000

Delta Hedging Simulation Results
--------------------------------
Rebalancing    Mean Profit    Std Dev    Mean/Std
-----------    -----------    -------    --------
          0         4.1485     11.8594      0.3498
          1         3.9619     12.0190      0.3296
          4         3.8591      6.6110      0.5837
         12         3.6983      4.1029      0.9014
         52         3.4400      2.2811      1.5080
        252         2.9649      1.3888      2.1349
        504         2.5815      1.1150      2.3152
        756         2.3201      0.9864      2.3520
       1008         2.0844      0.8823      2.3625
       2520         1.0321      0.4957      2.0820

Black-Scholes Price (Implied Volatility):     19.8953
Black-Scholes Price (Realized Volatility):    15.9311
BS Price Difference (Implied - Realized):      3.9642
Expected Profit (Ignoring T-Costs):            3.9642
Best Rebalancing Frequency (Max Mean/Std):       1008

Elapsed Time (s):    8.9850
```
