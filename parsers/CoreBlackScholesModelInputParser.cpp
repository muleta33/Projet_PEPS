#include "CoreBlackScholesModelInputParser.hpp"

using namespace input_parsers;

int CoreBlackScholesModelInputParser::get_underlying_number() const
{
	return underlying_number;
}

int CoreBlackScholesModelInputParser::get_monitoring_times() const
{
	int result;
	parser.extract("timestep number", result);
	return result;
}

PnlMat * CoreBlackScholesModelInputParser::get_correlation_matrix() const
{
	// Possibilité de faire une fonction d'extraction d'une matrice dans le parser
	double extractionResult;
	parser.extract("correlation", extractionResult);

	/*PnlMat * result = pnl_mat_create_from_double(underlying_number, underlying_number, extractionResult);*/
	PnlMat * result = pnl_mat_create_from_double(2*underlying_number, 2*underlying_number, extractionResult);
	/*for (int i = 0; i < underlying_number; ++i)*/
	for (int i = 0; i < 2 * underlying_number; ++i)
		MLET(result, i, i) = 1;
	return result;
}

double CoreBlackScholesModelInputParser::get_interest_rate() const
{
	double result;
	parser.extract("interest rate", result);
	return result;
}

double CoreBlackScholesModelInputParser::get_maturity() const
{
	double result;
	parser.extract("maturity", result);
	return result;
}

PnlVect * CoreBlackScholesModelInputParser::get_volatility() const
{
	PnlVect * result;
	//parser.extract("volatility", result, underlying_number);
	parser.extract("volatility", result, 2*underlying_number);
	return result;
}

PnlVect * CoreBlackScholesModelInputParser::get_foreign_interest_rates() const
{
	PnlVect * result;
	parser.extract("foreign interest rates", result, underlying_number);
	return result;
}

