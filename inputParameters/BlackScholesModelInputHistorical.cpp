#include "BlackScholesModelInputHistorical.hpp"


using namespace input_parameters;

PnlMat * BlackScholesModelInputHistorical::get_correlation_matrix() const {
	return correlation_matrix_;
}

PnlVect * BlackScholesModelInputHistorical::get_volatility() const {
	return volatilities_;
}

int BlackScholesModelInputHistorical::get_underlying_number() const
{
	return underlying_number_;
}

int BlackScholesModelInputHistorical::get_monitoring_times() const
{
	int result;
	parser.extract("timestep number", result);
	return result;
}

double BlackScholesModelInputHistorical::get_interest_rate() const
{
	double result;
	parser.extract("interest rate", result);
	return result;
}

double BlackScholesModelInputHistorical::get_maturity() const
{
	double result;
	parser.extract("maturity", result);
	return result;
}

PnlVect * BlackScholesModelInputHistorical::get_foreign_interest_rates() const
{
	PnlVect * result;
	parser.extract("foreign interest rates", result, underlying_number_);
	return result;
}