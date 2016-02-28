#include "CoreEurostralMutualFundInputParser.hpp"

using namespace input_parsers;

int CoreEurostralMutualFundInputParser::get_underlying_number() const
{
	return underlying_number;
}

double CoreEurostralMutualFundInputParser::get_maturity() const
{
	double result;
	parser.extract("maturity", result);
	return result;
}

double CoreEurostralMutualFundInputParser::get_guaranteed_capital_percentage() const
{
	double result;
	parser.extract("guaranteed capital", result);
	return result;
}

double CoreEurostralMutualFundInputParser::get_indexes_return_percentage() const
{
	double result;
	parser.extract("indexes return", result);
	return result;
}

double CoreEurostralMutualFundInputParser::get_initial_capital() const
{
	double result;
	parser.extract("initial capital", result);
	return result;
}

PnlVect * CoreEurostralMutualFundInputParser::get_underlying_coefficients() const
{
	PnlVect * result;
	parser.extract("underlying coefficients", result, underlying_number);
	return result;
}

PnlVect * CoreEurostralMutualFundInputParser::get_currencies() const
{
	PnlVect * result;
	parser.extract("currencies", result, 2*underlying_number);
	return result;
}