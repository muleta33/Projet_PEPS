#include "ProfitAndLossInputParser.hpp"

using namespace input_parameters;


PnlVect * ProfitAndLossInputParser::get_trend() const
{
	PnlVect * result;
	//parser.extract("trend", result, underlying_number);
	parser.extract("trend", result, 2*underlying_number);
	return result;
}

int ProfitAndLossInputParser::get_rebalancing_times() const
{
	int result;
	parser.extract("rebalancing times", result);
	return result;
}

double ProfitAndLossInputParser::get_fd_step() const
{
	double result;
	parser.extract("fd step", result);
	return result;
}

int ProfitAndLossInputParser::get_sample_number() const
{
	int result;
	parser.extract("sample number", result);
	return result;
}

PnlVect * ProfitAndLossInputParser::get_spot() const
{
	PnlVect * result;
	/*parser.extract("spot", result, underlying_number);*/
	parser.extract("spot", result, 2*underlying_number);
	return result;
}