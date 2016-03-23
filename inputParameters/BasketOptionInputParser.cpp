
#include "BasketOptionInputParser.hpp"

using namespace input_parameters;

int BasketOptionInputParser::get_underlying_number() const
{
	return underlying_number;
}

double BasketOptionInputParser::get_maturity() const
{
	double result;
	parser.extract("maturity", result);
	return result;
}

PnlVect * BasketOptionInputParser::get_underlying_coefficients() const
{
	PnlVect * result;
	parser.extract("underlying coefficients", result, underlying_number);
	return result;
}

double BasketOptionInputParser::get_strike() const
{
	double result;
	parser.extract("strike", result);
	return result;
}

PnlVect * BasketOptionInputParser::get_currencies() const
{
	PnlVect * result;
	parser.extract("currencies", result, 2*underlying_number);
	return result;
}