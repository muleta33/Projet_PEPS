#include "CoreProfitAndLossInputParser.hpp"

using namespace input_parsers;


double CoreProfitAndLossInputParser::get_trend() const
{
	double result;
	parser.extract("trend", result);
	return result;
}

int CoreProfitAndLossInputParser::get_rebalancing_times() const
{
	int result;
	parser.extract("rebalancing times", result);
	return result;
}

double CoreProfitAndLossInputParser::get_fd_step() const
{
	double result;
	parser.extract("fd step", result);
	return result;
}

int CoreProfitAndLossInputParser::get_sample_number() const
{
	int result;
	parser.extract("sample number", result);
	return result;
}

PnlVect * CoreProfitAndLossInputParser::get_spot() const
{
	PnlVect * result;
	parser.extract("spot", result, underlying_number);
	return result;
}