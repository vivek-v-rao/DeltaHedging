#ifndef BS_HEDGING_H
#define BS_HEDGING_H

#include <vector>
#include <string>

// Normal CDF approximation
double norm_cdf(const double x);

// Black-Scholes call price
double bs_call_price(const double S, const double K, const double T, 
		     const double r, const double sigma);

// Black-Scholes delta for call
double bs_call_delta(const double S, const double K, const double T, 
		     const double r, const double sigma);

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
				  const double premium, const double position);

#endif // BS_HEDGING_H
