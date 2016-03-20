#include "stdafx.h"
#include "CppUnitTest.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_finance.h"
#include "EurostralMutualFund.hpp"
#include "BlackScholesModelRiskNeutral.hpp"
#include "FakeBasketOptionInputParameters.hpp"
#include "FakeBlackScholesModelInputParametersForCall.hpp"
#include "FakeBlackScholesModelInputParametersForCallBis.hpp"
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
			const FakeBlackScholesModelInputParametersForCall fake_model_input_parameters;
			const FakeBasketOptionInputParameters fake_option_input_parameters;
			models::BlackScholesModelRiskNeutral model(fake_model_input_parameters, generator);
			double strike = fake_option_input_parameters.get_strike();

			BasketOption call(fake_model_input_parameters.get_maturity(), fake_option_input_parameters);

			PnlVect * spot = pnl_vect_create_from_scalar(2, 100);
			LET(spot, 1) = 1;

			int sample_number = 50000;
			MonteCarloPricing pricer(model, call, sample_number);

			double computed_price = 0;
			double computed_confidence_interval = 0;
			pricer.price(spot, computed_price, computed_confidence_interval);

			double reference_price = pnl_bs_call(GET(spot, 0), strike, fake_model_input_parameters.get_maturity(),
				model.interest_rate(), 0, GET(fake_model_input_parameters.get_volatility(), 0));

			bool is_reference_price_in_confidence_interval = false;
			if ((reference_price >= computed_price - computed_confidence_interval)
				&& (reference_price <= computed_price + computed_confidence_interval))
				is_reference_price_in_confidence_interval = true;

			Assert::IsTrue(is_reference_price_in_confidence_interval);

			pnl_vect_free(&spot);
		}


		TEST_METHOD(price_and_price_at_call_coherence)
		{
			const FakeBlackScholesModelInputParametersForCall fake_model_input_parameters;
			const SameSeedRandomGeneration generator1;
			models::BlackScholesModelRiskNeutral model(fake_model_input_parameters, generator1);
			const FakeBlackScholesModelInputParametersForCallBis fake_model_input_parameters_bis;
			const SameSeedRandomGeneration generator2;
			models::BlackScholesModelRiskNeutral model_lower_maturity(fake_model_input_parameters_bis, generator2);
			const FakeBasketOptionInputParameters fake_option_input_parameters;

			BasketOption call(fake_model_input_parameters.get_maturity(), fake_option_input_parameters);
			BasketOption call_lower_maturity(fake_model_input_parameters_bis.get_maturity(), fake_option_input_parameters);

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