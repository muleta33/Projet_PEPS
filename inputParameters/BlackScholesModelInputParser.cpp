#include "BlackScholesModelInputParser.hpp"

using namespace input_parameters;

int BlackScholesModelInputParser::get_underlying_number() const
{
	return underlying_number;
}

int BlackScholesModelInputParser::get_monitoring_times() const
{
	int result;
	parser.extract("timestep number", result);
	return result;
}

PnlMat * BlackScholesModelInputParser::get_correlation_matrix() const
{
	// Possibilité de faire une fonction d'extraction d'une matrice dans le parser
	double extractionResult;
	parser.extract("correlation", extractionResult);

	PnlMat * result = pnl_mat_create_from_double(2 * underlying_number, 2 * underlying_number, extractionResult);
	/*for (int i = 0; i < 2 * underlying_number; i++) {
		MLET(result, 2 * underlying_number - 1, i) = 0;
		MLET(result, i, 2 * underlying_number - 1) = 0;
	}*/
	for (int i = 0; i < 2 * underlying_number; ++i)
		MLET(result, i, i) = 1;

	return result;
}

double BlackScholesModelInputParser::get_interest_rate() const
{
	double result;
	parser.extract("interest rate", result);
	return result;
}

double BlackScholesModelInputParser::get_maturity() const
{
	double result;
	parser.extract("maturity", result);
	return result;
}

PnlVect * BlackScholesModelInputParser::get_volatility() const
{
	PnlVect * result;
	//parser.extract("volatility", result, underlying_number);
	parser.extract("volatility", result, 2 * underlying_number);
	return result;
}

PnlVect * BlackScholesModelInputParser::get_foreign_interest_rates() const
{
	PnlVect * result;
	parser.extract("foreign interest rates", result, underlying_number);
	return result;
}
