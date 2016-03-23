
@echo off



echo --- 5 per day --- >> call50000Change2.txt
for /l %%A in (1 1 50) do (
timeout /t 2 /nobreak > NUL
start /wait cmd /c "..\x64\Release\profitAndLoss.exe ..\dat\Call_5_per_day_change.dat >> call50000Change2.txt
)
