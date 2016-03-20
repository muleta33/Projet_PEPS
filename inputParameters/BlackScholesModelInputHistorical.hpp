#pragma once
#include "BlackScholesModelInputParameters.hpp"
#include "Parser.hpp"
#include "InputParametersUtilities.hpp"

namespace input_parameters
{
	class BlackScholesModelInputHistorical : public BlackScholesModelInputParameters
	{
	private:
		const Parser & parser;
		int underlying_number_;
		PnlMat * correlation_matrix_;
		PnlVect * volatilities_;
	public:
		int get_underlying_number() const;
		int get_monitoring_times() const;
		PnlMat * get_correlation_matrix() const;
		double get_interest_rate() const;
		double get_maturity() const;
		PnlVect * get_volatility() const;
		PnlVect * get_foreign_interest_rates() const;

		BlackScholesModelInputHistorical(const Parser & p, const PnlMat * past_data) : parser(p)
		{
			parser.extract("option size", underlying_number_);
			PnlMat * past_returns_ = pnl_mat_create(past_data->m - 1, past_data->n);
			compute_returns(past_returns_, past_data);
			volatilities_ = pnl_vect_create(2 * underlying_number_);
			compute_volatilities(volatilities_, past_returns_);
			correlation_matrix_ = pnl_mat_create(2 * underlying_number_, 2 * underlying_number_);
			compute_correlation_matrix(correlation_matrix_, past_returns_, volatilities_);
			pnl_mat_free(&past_returns_);
		}

		~BlackScholesModelInputHistorical() 
		{
			pnl_vect_free(&volatilities_);
		};
	};
}