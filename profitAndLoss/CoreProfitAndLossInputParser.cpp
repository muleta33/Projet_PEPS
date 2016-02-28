#include "CoreProfitAndLossInputParser.hpp"

using namespace input_parsers;


PnlVect * CoreProfitAndLossInputParser::get_trend() const
{
	PnlVect * result;
	//parser.extract("trend", result, underlying_number);
	parser.extract("trend", result, 2*underlying_number);
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
	/*parser.extract("spot", result, underlying_number);*/
	parser.extract("spot", result, 2*underlying_number);
	return result;
}