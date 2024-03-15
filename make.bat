@echo off

REM %GDK%はSGDKのあるアドレス
REM %GIT_PRJ%はこのプロジェクトのある場所
REM 環境変数を設定してからこのバッチを起動すること

REM %GDK%\bin\make.exe -f %GDK%\makefile.gen clean
%GDK%\bin\make.exe -f %GDK%\makefile.gen
%GDK%\Gens_KMod_v0.7.3\gens.exe C:\Users\wahah\OneDrive\Documents\GitHub\HIntTest\out\rom.bin