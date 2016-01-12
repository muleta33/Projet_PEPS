
@echo off

echo --- 1 per month --- >> call50000.txt
for /l %%A in (1 1 20) do (
start /wait cmd /c "..\x64\Debug\profitAndLoss.exe ..\dat\Call_month.dat >> call50000.txt
)

echo --- 1 per week --- >> call50000.txt
for /l %%A in (1 1 20) do (
start /wait cmd /c "..\x64\Debug\profitAndLoss.exe ..\dat\Call_week.dat >> call50000.txt
)

echo --- 1 per day --- >> call50000.txt
for /l %%A in (1 1 20) do (
start /wait cmd /c "..\x64\Debug\profitAndLoss.exe ..\dat\Call_day.dat >> call50000.txt
)

echo --- 5 per day --- >> call50000.txt
for /l %%A in (1 1 20) do (
start /wait cmd /c "..\x64\Debug\profitAndLoss.exe ..\dat\Call_5_per_day.dat >> call50000.txt
)
