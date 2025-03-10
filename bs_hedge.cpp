#include "bs_hedge.h"
#include <cmath>
#include <random>

using namespace std;

// Normal CDF approximation
double norm_cdf(const double x) {
	return 0.5 * (1.0 + erf(x / sqrt(2.0)));
}

// Black-Scholes price (call or put)
double bs_price(const double S, const double K, const double T, 
		const double r, const double sigma, OptionType type) {
	const double d1 = (log(S/K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
	const double d2 = d1 - sigma * sqrt(T);
	if (type == OptionType::Call || type == OptionType::Straddle) {
		double call_price = S * norm_cdf(d1) - K * exp(-r * T) * norm_cdf(d2);
		if (type == OptionType::Call) return call_price;
	// For straddle, add put price
		double put_price = K * exp(-r * T) * norm_cdf(-d2) - S * norm_cdf(-d1);
		return call_price + put_price;
	} else { // Put
		return K * exp(-r * T) * norm_cdf(-d2) - S * norm_cdf(-d1);
	}
}

// Black-Scholes delta (call or put)
double bs_delta(const double S, const double K, const double T, 
		const double r, const double sigma, OptionType type) {
	const double d1 = (log(S/K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
	if (type == OptionType::Call) {
		return norm_cdf(d1);
	} else if (type == OptionType::Put) {
		return norm_cdf(d1) - 1.0; // Put delta = Call delta - 1
	} else { // Straddle
		return norm_cdf(d1) + (norm_cdf(d1) - 1.0); // Call delta + Put delta
	}
}

SimulationResult simulate_hedging(const double S0, const double K, const double T, 
				  const double r, const double implied_sigma, 
				  const double realized_sigma, const double transaction_cost, 
				  const int num_steps, const int num_paths,
				  const double premium, const double position, 
				  OptionType type) {
	random_device rd;
	mt19937 gen(rd());
	normal_distribution<> dist(0, 1);

	const double dt = (num_steps == 0) ? T : T / num_steps;
	const double abs_position = abs(position);
	const double sign = (position >= 0) ? 1.0 : -1.0;
	vector<double> profits(num_paths);

	for (int i = 0; i < num_paths; i++) {
		double S = S0;
		double cash = (sign == -1.0) ? premium * abs_position : -premium * abs_position;
		double prev_delta = bs_delta(S, K, T, r, implied_sigma, type) * abs_position * (sign == -1.0 ? 1.0 : -1.0);
		cash -= prev_delta * S * (1.0 + transaction_cost);

		if (num_steps > 0) {
			for (int t = 0; t < num_steps; t++) {
				const double time_left = T - t * dt;
				const double current_delta = bs_delta(S, K, time_left, r, implied_sigma, type) * abs_position * (sign == -1.0 ? 1.0 : -1.0);
				const double delta_change = current_delta - prev_delta;

				if (delta_change != 0) {
					const double trade_amount = abs(delta_change) * S;
					const double trans_cost = trade_amount * transaction_cost;
					if (delta_change > 0) {
						cash -= trade_amount + trans_cost;
					} else {
						cash += trade_amount - trans_cost;
					}
				}

				const double z = dist(gen);
				S *= exp((r - 0.5 * realized_sigma * realized_sigma) * dt + 
					 realized_sigma * sqrt(dt) * z);
				prev_delta = current_delta;

				cash *= exp(r * dt);
			}
		} else {
			const double z = dist(gen);
			S *= exp((r - 0.5 * realized_sigma * realized_sigma) * T + 
				 realized_sigma * sqrt(T) * z);
			cash *= exp(r * T);
		}

	// Payoff calculation based on option type
		double payoff = 0.0;
		if (type == OptionType::Call || type == OptionType::Straddle) {
			payoff += max(0.0, S - K) * abs_position; // Call payoff
		}
		if (type == OptionType::Put || type == OptionType::Straddle) {
			payoff += max(0.0, K - S) * abs_position; // Put payoff
		}

		const double unwind_proceeds = abs(prev_delta) * S;
		const double unwind_cost = unwind_proceeds * transaction_cost;
		if (prev_delta > 0) {
			cash += unwind_proceeds - unwind_cost;
		} else {
			cash -= unwind_proceeds + unwind_cost;
		}
		if (sign == -1.0) {
			cash -= payoff;
		} else {
			cash += payoff;
		}

		profits[i] = cash;
	}

	double mean = 0.0;
	for (const double profit : profits) mean += profit;
	mean /= num_paths;

	double variance = 0.0;
	for (const double profit : profits) {
		variance += (profit - mean) * (profit - mean);
	}
	variance /= num_paths;
	const double std_dev = sqrt(variance);
	const double ratio = (std_dev != 0) ? mean / std_dev : 0.0;

	return {mean, std_dev, ratio};
}
