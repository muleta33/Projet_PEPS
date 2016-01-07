#pragma once
#include "ProfitAndLossInputParser.hpp"
#include "Parser.hpp"

namespace input_parsers
{
	class CoreProfitAndLossInputParser : public ProfitAndLossInputParser
	{
	private:
		const Parser & parser;
		int underlying_number;
	public:
		PnlVect * get_trend() const;
		int get_rebalancing_times() const;
		double get_fd_step() const;
		int get_sample_number() const;
		PnlVect * get_spot() const;

		CoreProfitAndLossInputParser(const Parser & p) : parser(p)
		{
			parser.extract("option size", underlying_number);
		}

		virtual ~CoreProfitAndLossInputParser() {};
	};
}