
@echo off

echo --- 1 per month --- >> call300000.txt
for /l %%A in (1 1 10) do (
start /wait cmd /c "..\x64\Debug\profitAndLoss.exe ..\dat\Call_month_300000.dat >> call300000.txt
)

echo --- 1 per week --- >> call300000.txt
for /l %%A in (1 1 10) do (
start /wait cmd /c "..\x64\Debug\profitAndLoss.exe ..\dat\Call_week_300000.dat >> call300000.txt
)

echo --- 1 per day --- >> call300000.txt
for /l %%A in (1 1 10) do (
start /wait cmd /c "..\x64\Debug\profitAndLoss.exe ..\dat\Call_day_300000.dat >> call300000.txt
)

echo --- 5 per day --- >> call300000.txt
for /l %%A in (1 1 10) do (
start /wait cmd /c "..\x64\Debug\profitAndLoss.exe ..\dat\Call_5_per_day_300000.dat >> call300000.txt
)
