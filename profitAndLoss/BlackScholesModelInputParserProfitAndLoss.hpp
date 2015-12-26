#pragma once
#include "BlackScholesModelInputParser.hpp"

class BlackScholesModelInputParserProfitAndLoss : public input_parsers::BlackScholesModelInputParser
{
public:
	virtual double get_trend() const = 0;
	virtual int get_rebalancing_times() const = 0;
	virtual double get_fd_step() const = 0;

	virtual ~BlackScholesModelInputParserProfitAndLoss() {};
};