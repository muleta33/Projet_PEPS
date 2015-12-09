#include "stdafx.h"
#include "CppUnitTest.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_finance.h"
#include "EurostralMutualFund.hpp"
#include "BlackScholesModel.hpp"
#include "FakeBlackScholesModelInputParserForCall.hpp"
#include "FakeEurostralMutualFundInputParser.hpp"
#include "PNLRandomGeneration.hpp"
#include "MonteCarloPricer.hpp"
#include "Call.hpp"

#include <cassert>
#include <iostream>
#include <ctime>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(TestMonteCarloPricer)
	{
	public:

		TEST_METHOD(price_call)
		{
			const generators::PnlRandomGeneration generator;
			const FakeBlackScholesModelInputParserForCall fake_model_parser;
			models::BlackScholesModel model(fake_model_parser, generator);
			double strike = 90;
			Call call(fake_model_parser.get_final_simulation_date(), strike);

			PnlVect * spot = pnl_vect_create_from_scalar(1, 100);
			PnlMat * underlying_paths = pnl_mat_create_from_scalar(2, 1, 100);
			MLET(underlying_paths, 1, 0) = 110;

			int sample_number = 50000;
			MonteCarloPricer pricer(sample_number);

			double computed_price = 0;
			double computed_confidence_interval = 0;
			pricer.price(model, call, spot, computed_price, computed_confidence_interval);

			double reference_price = pnl_bs_call(GET(spot, 0), strike, fake_model_parser.get_final_simulation_date(), 
				model.interest_rate(), 0, GET(fake_model_parser.get_volatility(), 0));

			bool is_reference_price_in_confidence_interval = false;
			if ((reference_price >= computed_price - computed_confidence_interval)
				&& (reference_price <= computed_price + computed_confidence_interval))
				is_reference_price_in_confidence_interval = true;

			Assert::IsTrue(is_reference_price_in_confidence_interval);

			pnl_vect_free(&spot);
			pnl_mat_free(&underlying_paths);
		}

	};
}