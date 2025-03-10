// does simulations for call, put, and straddle together

#include "bs_hedge.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <limits>
#include <vector>

using namespace std;

string option_type_to_string(OptionType type) {
	switch (type) {
		case OptionType::Call: return "Call";
		case OptionType::Put: return "Put";
		case OptionType::Straddle: return "Straddle";
		default: return "Unknown";
	}
}

int main() {
	auto start_time = chrono::high_resolution_clock::now();

    // Option parameters
	const double S0 = 100.0;
	const double K = 100.0;
	const double T = 1.0;
	const double r = 0.0;
	const double implied_sigma = 0.25;
	const double realized_sigma = 0.20;
	const double transaction_cost = 0.001;
	const int num_paths = 1000;
	const vector<int> hedging_frequencies = {0, 1, 4, 12, 52, 252, 504, 756, 1008, 2520};
	const double position = -1.0;

    // We'll simulate all three option types: Call, Put, and Straddle.
	vector<OptionType> optionTypes = { OptionType::Call, OptionType::Put, OptionType::Straddle };

    // For each option type, precompute Black-Scholes values.
	struct OptionBSData {
		double bs_price_implied;
		double bs_price_realized;
		double bs_price_diff;
		double expected_profit_no_tcost;
	};
	vector<OptionBSData> bsData(optionTypes.size());

	for (size_t i = 0; i < optionTypes.size(); i++) {
		OptionType opt = optionTypes[i];
		double bs_price_implied = bs_price(S0, K, T, r, implied_sigma, opt);
		double bs_price_realized = bs_price(S0, K, T, r, realized_sigma, opt);
		double bs_price_diff = bs_price_implied - bs_price_realized;
		double expected_profit_no_tcost = bs_price_diff * -position;
		bsData[i] = { bs_price_implied, bs_price_realized, bs_price_diff, expected_profit_no_tcost };
	}

    // Output option parameters
	cout << fixed << setprecision(4);
	cout << "Option Parameters:\n";
	cout << "-----------------\n";
	cout << "Initial Stock Price (S0):    " << setw(10) << S0 << "\n";
	cout << "Strike Price (K):            " << setw(10) << K << "\n";
	cout << "Time to Maturity (T):        " << setw(10) << T << "\n";
	cout << "Risk-free Rate (r):          " << setw(10) << r << "\n";
	cout << "Implied Volatility (sigma):  " << setw(10) << implied_sigma << "\n";
	cout << "Realized Volatility:         " << setw(10) << realized_sigma << "\n";
	cout << "Transaction Cost:            " << setw(10) << transaction_cost << "\n";
	cout << "Number of Simulations:       " << setw(10) << num_paths << "\n";
	cout << "Position:                    " << setw(10) << position << "\n\n";

    // Print simulation results header.
    // The table will have the rebalancing frequency and then, for each option type, three columns.
	cout << "Delta Hedging Simulation Results\n";
	cout << "--------------------------------\n";
	cout << setw(11) << "Rebalancing";
	for (const OptionType& opt : optionTypes) {
		string optStr = option_type_to_string(opt);
		cout << setw(15) << optStr + " Mean";
		cout << setw(12) << optStr + " Std";
		cout << setw(12) << optStr + " M/S";
	}
	cout << "\n";

	cout << setw(11) << "-----------";
	for (size_t i = 0; i < optionTypes.size(); i++) {
		cout << setw(15) << "-----------";
		cout << setw(12) << "-------";
		cout << setw(12) << "--------";
	}
	cout << "\n";

    // Track best rebalancing frequency (by max mean/std ratio) for each option type.
	vector<int> best_freq(optionTypes.size(), 0);
	vector<double> max_ratio(optionTypes.size(), -numeric_limits<double>::infinity());

    // Loop over each hedging frequency.
	for (const int freq : hedging_frequencies) {
		cout << setw(11) << freq;
	// For each option type, run the simulation.
		for (size_t i = 0; i < optionTypes.size(); i++) {
			OptionType opt = optionTypes[i];
			SimulationResult result = simulate_hedging(S0, K, T, r, implied_sigma,
				realized_sigma, transaction_cost, freq, num_paths,
				bsData[i].bs_price_implied, position, opt);
			cout << setw(15) << result.mean_profit;
			cout << setw(12) << result.std_dev_profit;
			cout << setw(12) << result.mean_to_std_ratio;

			if (result.mean_to_std_ratio > max_ratio[i]) {
				max_ratio[i] = result.mean_to_std_ratio;
				best_freq[i] = freq;
			}
		}
		cout << "\n";
	}

    // Output Black-Scholes prices and expected profits for each option type.
	cout << "\nBlack-Scholes Prices and Expected Profit (Ignoring Transaction Costs):\n";
	for (size_t i = 0; i < optionTypes.size(); i++) {
		OptionType opt = optionTypes[i];
		cout << option_type_to_string(opt) << " Option:\n";
		cout << "  Black-Scholes Price (Implied Volatility):  " << setw(10) << bsData[i].bs_price_implied << "\n";
		cout << "  Black-Scholes Price (Realized Volatility): " << setw(10) << bsData[i].bs_price_realized << "\n";
		cout << "  BS Price Difference (Implied - Realized):  " << setw(10) << bsData[i].bs_price_diff << "\n";
		cout << "  Expected Profit (Ignoring T-Costs):        " << setw(10) << bsData[i].expected_profit_no_tcost << "\n";
		cout << "  Best Rebalancing Frequency (Max Mean/Std):   " << setw(10) << best_freq[i] << "\n\n";
	}

	auto end_time = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
	cout << "\nElapsed Time (s):" << setw(10) << duration.count() / 1000.0 << "\n";

	return 0;
}
