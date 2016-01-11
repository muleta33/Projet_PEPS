#include "stdafx.h"
#include "CppUnitTest.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_finance.h"
#include "EurostralMutualFund.hpp"
#include "BlackScholesModelRiskNeutral.hpp"
#include "FakeBasketOptionInputParser.hpp"
#include "FakeBlackScholesModelInputParserForCall.hpp"
#include "FakeBlackScholesModelInputParserForCallBis.hpp"
#include "PNLRandomGeneration.hpp"
#include "SameSeedRandomGeneration.hpp"
#include "MonteCarloPricing.hpp"
#include "BasketOption.hpp"

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
			const FakeBasketOptionInputParser fake_option_parser;
			models::BlackScholesModelRiskNeutral model(fake_model_parser, generator);
			double strike = fake_option_parser.get_strike();

			BasketOption call(fake_model_parser.get_final_simulation_date(), fake_option_parser);

			PnlVect * spot = pnl_vect_create_from_scalar(1, 100);

			int sample_number = 50000;
			MonteCarloPricing pricer(model, call, sample_number);

			double computed_price = 0;
			double computed_confidence_interval = 0;
			pricer.price(spot, computed_price, computed_confidence_interval);

			double reference_price = pnl_bs_call(GET(spot, 0), strike, fake_model_parser.get_final_simulation_date(), 
				model.interest_rate(), 0, GET(fake_model_parser.get_volatility(), 0));

			bool is_reference_price_in_confidence_interval = false;
			if ((reference_price >= computed_price - computed_confidence_interval)
				&& (reference_price <= computed_price + computed_confidence_interval))
				is_reference_price_in_confidence_interval = true;

			Assert::IsTrue(is_reference_price_in_confidence_interval);

			pnl_vect_free(&spot);
		}


		TEST_METHOD(price_and_price_at_call_coherence)
		{
			const FakeBlackScholesModelInputParserForCall fake_model_parser;
			const SameSeedRandomGeneration generator1;
			models::BlackScholesModelRiskNeutral model(fake_model_parser, generator1);
			const FakeBlackScholesModelInputParserForCallBis fake_model_parser_bis;
			const SameSeedRandomGeneration generator2;
			models::BlackScholesModelRiskNeutral model_lower_maturity(fake_model_parser_bis, generator2);
			const FakeBasketOptionInputParser fake_option_parser;

			BasketOption call(fake_model_parser.get_final_simulation_date(), fake_option_parser);
			BasketOption call_lower_maturity(fake_model_parser_bis.get_final_simulation_date(), fake_option_parser);

			PnlVect * spots = pnl_vect_create_from_scalar(1, 120);
			PnlMat * past_values = pnl_mat_create_from_scalar(3, 1, 120);

			int sample_number = 50000;

			double price = 0, price_at = 0, confidence_interval = 0;
			MonteCarloPricing monteCarloPricerAt(model, call, sample_number);
			monteCarloPricerAt.price_at(1, past_values, price_at, confidence_interval);
			MonteCarloPricing monteCarloPricer(model_lower_maturity, call_lower_maturity, sample_number);
			monteCarloPricer.price(spots, price, confidence_interval);

			Assert::AreEqual(price, price_at);

			pnl_vect_free(&spots);
			pnl_mat_free(&past_values);
		}

	};
}