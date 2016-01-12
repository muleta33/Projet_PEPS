
@echo off

echo --- 1 per semester --- >> eurostralMutualFund.txt
for /l %%A in (1 1 20) do (
start /wait cmd /c "..\x64\Debug\profitAndLoss.exe ..\dat\EurostralMutualFund_semester.dat >> eurostralMutualFund.txt
)

echo --- 1 per month --- >> eurostralMutualFund.txt
for /l %%A in (1 1 20) do (
start /wait cmd /c "..\x64\Debug\profitAndLoss.exe ..\dat\EurostralMutualFund_month.dat >> eurostralMutualFund.txt
)

echo --- 1 per week --- >> eurostralMutualFund.txt
for /l %%A in (1 1 20) do (
start /wait cmd /c "..\x64\Debug\profitAndLoss.exe ..\dat\EurostralMutualFund_week.dat >> eurostralMutualFund.txt
)

