#pragma once
#include <vector>
#include "UnderlyingModel.hpp"
#include "BlackScholesModelInputParser.hpp"
#include "RandomGeneration.hpp"

namespace models
{
	class BlackScholesModelRoutine : public UnderlyingModel
	{
	public:
		BlackScholesModelRoutine(const input_parsers::BlackScholesModelInputParser &parser, const generators::RandomGeneration &random_generator);
		~BlackScholesModelRoutine();

	protected:
		PnlMat * generated_asset_paths_;
		double timestep_;
		void add_one_simulation_to_generated_asset_paths(int at_line, double timelength, const PnlVect * const last_values) const;
		void fill_remainder_of_generated_asset_paths(int from_line) const;
		virtual double get_trend() const = 0;

	private:

		double final_simulation_date_;
		PnlVect * volatilities;
		double correlation_parameter_;
		PnlMat * cholesky_matrix_corr_;
		int compute_cholesky_matrix();
		const generators::RandomGeneration &random_generator_;
		PnlVect * gaussian_vector_for_simulation_;
	};

}