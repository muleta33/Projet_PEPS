
@echo off

echo --- 1 per month --- >> callExact.txt
for /l %%A in (1 1 50) do (
timeout /t 2 /nobreak > NUL
start /wait cmd /c "..\x64\Debug\profitAndLoss.exe ..\dat\Call_month.dat >> callExact.txt"
)

echo --- 1 per week --- >> callExact.txt
for /l %%A in (1 1 50) do (
timeout /t 2 /nobreak > NUL
start /wait cmd /c "..\x64\Debug\profitAndLoss.exe ..\dat\Call_week.dat >> callExact.txt"
)

echo --- 1 per day --- >> callExact.txt
for /l %%A in (1 1 50) do (
timeout /t 2 /nobreak > NUL
start /wait cmd /c "..\x64\Debug\profitAndLoss.exe ..\dat\Call_day.dat >> callExact.txt"
)

echo --- 10 per day --- >> callExact.txt
for /l %%A in (1 1 50) do (
timeout /t 2 /nobreak > NUL
start /wait cmd /c "..\x64\Debug\profitAndLoss.exe ..\dat\Call_10_per_day.dat >> callExact.txt"
)

echo --- 100 per day --- >> callExact.txt
for /l %%A in (1 1 50) do (
timeout /t 2 /nobreak > NUL
start /wait cmd /c "..\x64\Debug\profitAndLoss.exe ..\dat\Call_100_per_day.dat >> callExact.txt"
)

echo --- 1000 per day --- >> callExact.txt
for /l %%A in (1 1 50) do (
timeout /t 2 /nobreak > NUL
start /wait cmd /c "..\x64\Debug\profitAndLoss.exe ..\dat\Call_1000_per_day.dat >> callExact.txt"
)