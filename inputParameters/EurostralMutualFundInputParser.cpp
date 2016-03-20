#include "EurostralMutualFundInputParser.hpp"

using namespace input_parameters;

int EurostralMutualFundInputParser::get_underlying_number() const
{
	return underlying_number;
}

double EurostralMutualFundInputParser::get_maturity() const
{
	double result;
	parser.extract("maturity", result);
	return result;
}

double EurostralMutualFundInputParser::get_guaranteed_capital_percentage() const
{
	double result;
	parser.extract("guaranteed capital", result);
	return result;
}

double EurostralMutualFundInputParser::get_indexes_return_percentage() const
{
	double result;
	parser.extract("indexes return", result);
	return result;
}

double EurostralMutualFundInputParser::get_initial_capital() const
{
	double result;
	parser.extract("initial capital", result);
	return result;
}

PnlVect * EurostralMutualFundInputParser::get_underlying_coefficients() const
{
	PnlVect * result;
	parser.extract("underlying coefficients", result, underlying_number);
	return result;
}

PnlVect * EurostralMutualFundInputParser::get_currencies() const
{
	PnlVect * result;
	parser.extract("currencies", result, 2*underlying_number);
	return result;
}