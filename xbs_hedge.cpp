#include "bs_hedge.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <limits>

using namespace std;

int main() {
	auto start_time = chrono::high_resolution_clock::now();

	const double S0 = 100.0;
	const double K = 100.0;
	const double T = 1.0;
	const double r = 0.0;
	const double implied_sigma = 0.22;
	const double realized_sigma = 0.20;
	const double transaction_cost = 0.001;
	const int num_paths = 100000;
	const vector<int> hedging_frequencies = {0, 1, 4, 12, 52, 252, 504};
	const double position = -1.0;

	const double bs_price_implied = bs_call_price(S0, K, T, r, implied_sigma);
	const double bs_price_realized = bs_call_price(S0, K, T, r, realized_sigma);
	const double bs_price_diff = bs_price_implied - bs_price_realized;
	const double expected_profit_no_tcost = bs_price_diff * -position;

	cout << fixed << setprecision(4);
	cout << "Option Parameters:\n";
	cout << "-----------------\n";
	cout << "Option Position:             " << setw(10) << position << "\n";
	cout << "Initial Stock Price (S0):    " << setw(10) << S0 << "\n";
	cout << "Strike Price (K):            " << setw(10) << K << "\n";
	cout << "Time to Maturity (T):        " << setw(10) << T << "\n";
	cout << "Risk-free Rate (r):          " << setw(10) << r << "\n";
	cout << "Implied Volatility (sigma):  " << setw(10) << implied_sigma << "\n";
	cout << "Realized Volatility:         " << setw(10) << realized_sigma << "\n";
	cout << "Transaction Cost:            " << setw(10) << transaction_cost << "\n";
	cout << "Number of Simulations:       " << setw(10) << num_paths << "\n\n";

	cout << "Delta Hedging Simulation Results\n";
	cout << "--------------------------------\n";
	cout << "Rebalancing    Mean Profit    Std Dev    Mean/Std\n";
	cout << "-----------    -----------    -------    --------\n";

	int best_freq = 0;
	double max_ratio = -numeric_limits<double>::infinity();

	for (const int freq : hedging_frequencies) {
		const SimulationResult result = simulate_hedging(S0, K, T, r, implied_sigma, 
			realized_sigma, transaction_cost, 
			freq, num_paths, bs_price_implied, position);

		cout << setw(11) << freq << setw(15) << result.mean_profit 
				<< setw(12) << result.std_dev_profit << setw(12) << result.mean_to_std_ratio << "\n";

		if (result.mean_to_std_ratio > max_ratio) {
			max_ratio = result.mean_to_std_ratio;
			best_freq = freq;
		}
	}

	cout << "\nBlack-Scholes Price (Implied Volatility):  " << setw(10) << bs_price_implied << "\n";
	cout << "Black-Scholes Price (Realized Volatility): " << setw(10) << bs_price_realized << "\n";
	cout << "BS Price Difference (Implied - Realized):  " << setw(10) << bs_price_diff << "\n";
	cout << "Expected Profit (Ignoring T-Costs):        " << setw(10) << expected_profit_no_tcost << "\n";
	cout << "Best Rebalancing Frequency (Max Mean/Std): " << setw(10) << best_freq << "\n";

	auto end_time = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
	cout << "\nElapsed Time:" << setw(8) << duration.count() / 1000.0 << " seconds\n";

	return 0;
}
