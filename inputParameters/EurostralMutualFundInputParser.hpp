#pragma once
#include "EurostralMutualFundInputParameters.hpp"
#include "Parser.hpp"

namespace input_parameters
{
	class EurostralMutualFundInputParser : public EurostralMutualFundInputParameters
	{
	private:
		const Parser & parser;
		int underlying_number;
	public:
		double get_maturity() const;
		int get_underlying_number() const;
		double get_guaranteed_capital_percentage() const;
		double get_indexes_return_percentage() const;
		double get_initial_capital() const;
		PnlVect * get_underlying_coefficients() const;
		PnlVect * get_currencies() const;

		EurostralMutualFundInputParser(const Parser & p) : parser(p)
		{
			parser.extract("option size", underlying_number);
		}

		virtual ~EurostralMutualFundInputParser() {};
	};
}