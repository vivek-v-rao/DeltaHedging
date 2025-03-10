#ifndef BS_HEDGE_H
#define BS_HEDGE_H

#include <vector>
#include <string>

// Option types
enum class OptionType { Call, Put, Straddle };

// Normal CDF approximation
double norm_cdf(const double x);

// Black-Scholes price (generalized for call or put)
double bs_price(const double S, const double K, const double T, 
		const double r, const double sigma, OptionType type);

// Black-Scholes delta (generalized for call or put)
double bs_delta(const double S, const double K, const double T, 
		const double r, const double sigma, OptionType type);

struct SimulationResult {
	double mean_profit;
	double std_dev_profit;
	double mean_to_std_ratio;
};

/**
 * @brief Simulates delta hedging of an option position using Monte Carlo simulation
 * 
 * Models stock price paths and implements delta hedging to compute profit statistics.
 */
SimulationResult simulate_hedging(const double S0, const double K, const double T, 
				  const double r, const double implied_sigma, 
				  const double realized_sigma, const double transaction_cost, 
				  const int num_steps, const int num_paths,
				  const double premium, const double position, 
				  OptionType type);

#endif // BS_HEDGE_H
