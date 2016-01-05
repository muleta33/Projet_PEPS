#include "stdafx.h"
#include "CppUnitTest.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_finance.h"
#include "EurostralMutualFund.hpp"
#include "BlackScholesModelRiskNeutral.hpp"
#include "FakeBlackScholesModelInputParserForCall.hpp"
#include "FakeBlackScholesModelInputParserForCallBis.hpp"
#include "PNLRandomGeneration.hpp"
#include "ConstantRandomGeneration.hpp"
#include "MonteCarloHedging.hpp"
#include "Call.hpp"

#include <cassert>
#include <iostream>
#include <ctime>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(TestMonteCarloHedging)
	{
	public:

		TEST_METHOD(hedge_call_with_almost_unreachable_strike)
		{
			const generators::PnlRandomGeneration generator;
			const FakeBlackScholesModelInputParserForCall fake_model_parser;
			models::BlackScholesModelRiskNeutral model(fake_model_parser, generator);
			double strike = 150;
			Call call(fake_model_parser.get_final_simulation_date(), strike);

			PnlVect * spot = pnl_vect_create_from_scalar(1, 45);

			int sample_number = 50000;
			double shift = 0.01;
			MonteCarloHedging monteCarloHedging(sample_number, shift);

			PnlVect * delta = pnl_vect_create_from_zero(1);
			monteCarloHedging.hedge(model, call, spot, delta);

			bool is_delta_very_low = false;
			if ((GET(delta, 0) < 0.01) && (GET(delta, 0) >= 0))
				is_delta_very_low = true;

			Assert::IsTrue(is_delta_very_low);

			pnl_vect_free(&spot);
			pnl_vect_free(&delta);
		}


		TEST_METHOD(hedge_call_with_almost_sure_reached_strike)
		{
			const generators::PnlRandomGeneration generator;
			const FakeBlackScholesModelInputParserForCall fake_model_parser;
			models::BlackScholesModelRiskNeutral model(fake_model_parser, generator);
			double strike = 75;
			Call call(fake_model_parser.get_final_simulation_date(), strike);

			PnlVect * spot = pnl_vect_create_from_scalar(1, 150);

			int sample_number = 50000;
			double shift = 0.01;
			MonteCarloHedging monteCarloHedging(sample_number, shift);

			PnlVect * delta = pnl_vect_create_from_zero(1);
			monteCarloHedging.hedge(model, call, spot, delta);

			bool is_delta_very_high = false;
			if ((GET(delta, 0) > 0.99) && (GET(delta, 0) <= 1.005))
				is_delta_very_high = true;

			Assert::IsTrue(is_delta_very_high);

			pnl_vect_free(&spot);
			pnl_vect_free(&delta);
		}


		TEST_METHOD(hedge_call)
		{
			const generators::PnlRandomGeneration generator;
			const FakeBlackScholesModelInputParserForCall fake_model_parser;
			models::BlackScholesModelRiskNeutral model(fake_model_parser, generator);
			double strike = 90;
			Call call(fake_model_parser.get_final_simulation_date(), strike);

			PnlVect * spot = pnl_vect_create_from_scalar(1, 100);

			int sample_number = 50000;
			double shift = 0.05;
			MonteCarloHedging monteCarloHedging(sample_number, shift);

			PnlVect * computed_delta = pnl_vect_create_from_zero(1);
			monteCarloHedging.hedge(model, call, spot, computed_delta);

			double * reference_price = new double;
			double * reference_delta = new double;
			pnl_cf_call_bs(GET(spot, 0), strike, fake_model_parser.get_final_simulation_date(), model.interest_rate(),
				0, GET(fake_model_parser.get_volatility(), 0), reference_price, reference_delta);

			Assert::AreEqual(*reference_delta, GET(computed_delta, 0), 0.005);

			pnl_vect_free(&spot);
			pnl_vect_free(&computed_delta);
			delete reference_price; 
			delete reference_delta;
		}


		TEST_METHOD(hedge_and_hedge_at_call_coherence)
		{
			const ConstantRandomGeneration generator;
			const FakeBlackScholesModelInputParserForCall fake_model_parser;
			models::BlackScholesModelRiskNeutral model(fake_model_parser, generator);
			const FakeBlackScholesModelInputParserForCallBis fake_model_parser_bis;
			models::BlackScholesModelRiskNeutral model_lower_maturity(fake_model_parser_bis, generator);
			double strike = 50;

			Call call(fake_model_parser.get_final_simulation_date(), strike);
			Call call_lower_maturity(fake_model_parser_bis.get_final_simulation_date(), strike);

			PnlVect * spots = pnl_vect_create_from_scalar(1, 50);
			PnlMat * past_values = pnl_mat_create_from_scalar(3, 1, 50);

			int sample_number = 50000;
			double shift = 0.01;
			MonteCarloHedging monteCarloHedging(sample_number, shift);

			PnlVect * delta_at = pnl_vect_create_from_zero(1);
			monteCarloHedging.hedge_at(1, model, call, past_values, delta_at);
			PnlVect * delta = pnl_vect_create_from_zero(1);
			monteCarloHedging.hedge(model_lower_maturity, call_lower_maturity, spots, delta);

			Assert::AreEqual(GET(delta, 0), GET(delta_at, 0));

			pnl_vect_free(&spots);
			pnl_mat_free(&past_values);
			pnl_vect_free(&delta_at);
			pnl_vect_free(&delta);
		}

	};
}