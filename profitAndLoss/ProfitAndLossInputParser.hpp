#pragma once
#include "pnl/pnl_vector.h"

class ProfitAndLossInputParser
{
public:
	virtual PnlVect * get_trend() const = 0;
	virtual int get_rebalancing_times() const = 0;
	virtual double get_fd_step() const = 0;
	virtual int get_sample_number() const = 0;
	virtual PnlVect * get_spot() const = 0;

	virtual ~ProfitAndLossInputParser() {};
};