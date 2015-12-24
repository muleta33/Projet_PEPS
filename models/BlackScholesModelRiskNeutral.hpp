#pragma once
#include <vector>
#include "UnderlyingModel.hpp"
#include "BlackScholesModelRoutine.hpp"
#include "BlackScholesModelInputParser.hpp"

namespace models
{

	class BlackScholesModelRiskNeutral : public UnderlyingModel
	{
	public:
		BlackScholesModelRiskNeutral(const input_parsers::BlackScholesModelInputParser &parser, const generators::RandomGeneration &random_generator);
		const PnlMat* const simulate_asset_paths_from_time(const double from_time, const PnlMat * const past_values) const;
		const PnlMat* const simulate_asset_paths_from_start(const PnlVect * const spot) const;
		void get_shifted_asset_paths(const PnlMat * const asset_path, int underlying_to_shift, double shift, double from_time, int past_values_number, PnlMat * shifted_asset_path) const;
		~BlackScholesModelRiskNeutral();

	private:
		PnlMat * generated_asset_paths_;
		const double TIME_PRECISION = 10.e-5;
		double timestep_;
		BlackScholesModelRoutine* routine;
	};

}