// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the EUROSTRALPRICER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// EUROSTRALPRICER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef EUROSTRALPRICER_EXPORTS
#define EUROSTRALPRICER_API __declspec(dllexport)
#else
#define EUROSTRALPRICER_API __declspec(dllimport)
#endif

#include "EurostralMutualFund.hpp"
#include "BlackScholesModelRiskNeutral.hpp"
#include "MonteCarloPricing.hpp"
#include "MonteCarloHedging.hpp"
#include "PNLRandomGeneration.hpp"

// This class is exported from the EurostralPricer.dll
class EUROSTRALPRICER_API EurostralPricer {

private:
	generators::PnlRandomGeneration *random_generator; 
	products::EurostralMutualFund *fund;
	models::BlackScholesModelRiskNeutral *model;
	MonteCarloPricing *pricer;
	MonteCarloHedging *hedger;
	const int underlying_number = 3;

public:
	EurostralPricer(double * past_data, int number_of_past_dates, int sample_number);
	~EurostralPricer();
	void price(double * spots, double &price, double &confidence_interval);
	void price_at(const double time, int number_of_rows_past, double * past, double * spots, double &price, double &confidence_interval);
	void hedge(double * spots, double * hedging_results);
	void hedge_at(const double time, int number_of_rows_past, double * past, double * spots, double * hedging_results);
};
