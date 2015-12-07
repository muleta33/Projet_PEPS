#include "stdafx.h"
#include "CppUnitTest.h"
#include "BlackScholesModel.hpp"
#include "PNLRandomGeneration.hpp"
#include "BlackScholesModelInputParser.hpp"
#include "FakeBlackScholesModelInputParser.hpp"
#include "ConstantRandomGeneration.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(TestBlackScholesModel)
	{
	public:
		
		TEST_METHOD(simulate_asset_paths_from_time)
		{
			const FakeBlackScholesModelInputParser fake_model_parser;
			const ConstantRandomGeneration fake_random_generator;
			models::BlackScholesModel under_test(fake_model_parser, fake_random_generator);
			PnlMat *past_matrix = pnl_mat_create_from_scalar(9, 5, 5);
			const PnlMat * result = under_test.simulate_asset_paths_from_time(3.75, past_matrix);
			PnlVect * vol = fake_model_parser.get_volatility();
			for (int i = 0; i < under_test.underlying_number(); i++)
			{
				double sigma = GET(vol, i);
				double res = 5 * exp(0.25*(0.05 - 0.5 * sigma * sigma));
				Assert::AreEqual(res, MGET(result, 8, i));
			}
			pnl_mat_free(&past_matrix);
		}

	};
}