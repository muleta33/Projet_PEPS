#pragma once
#include "BlackScholesModelInputParserProfitAndLoss.hpp"

class FakeBlackScholesModelInputParserProfitAndLoss : public BlackScholesModelInputParserProfitAndLoss
{
private:
	PnlVect * vol;

public:
	FakeBlackScholesModelInputParserProfitAndLoss()
	{
		vol = pnl_vect_create(3);
		LET(vol, 0) = 0.1;
		LET(vol, 1) = 0.3;
		LET(vol, 2) = 0.15;
	}

	~FakeBlackScholesModelInputParserProfitAndLoss()
	{
		pnl_vect_free(&vol);
		vol = nullptr;
	}

	int get_underlying_number() const { return 3; }
	virtual int get_monitoring_times() const { return 2; }
	virtual int get_rebalancing_times() const { return 52; }
	virtual double get_fd_step() const { return 0.1; }
	virtual double get_correlation_parameter() const { return 0.3; }
	virtual double get_interest_rate() const { return 0.05; }
	virtual double get_trend() const { return 0.07; }
	virtual double get_final_simulation_date() const { return 1; }
	virtual PnlVect * get_volatility() const { return vol; }

};