#NoEnv
#Warn
SendMode Input

BinaryFolder := A_ScriptDir "\..\..\..\..\builds\build-64\bin"

SetWorkingDir, %BinaryFolder% 

;
; First instance
;
Run MafiaMPLauncher.exe

; console
WinWait mafiamp: dev-console
WinActivate
WinSetTitle, mafiamp-cli-1
WinMove, mafiamp-cli-1,, 0, (A_ScreenHeight / 1.5), (A_ScreenWidth / 2), (A_ScreenHeight / 3)

; game window
WinWait Mafia: Advanced Multiplayer Edition
WinActivate
; WinSetTitle, mafiamp-game-1
WinMove, Mafia: Advanced Multiplayer Edition,, 0, 0,,
Sleep 200

;
; Second instance
;
Run MafiaMPLauncher.exe

; console
WinWait mafiamp: dev-console
WinActivate
WinSetTitle, mafiamp-cli-2
WinMove, mafiamp-cli-2,, (A_ScreenWidth / 2), (A_ScreenHeight / 1.5), (A_ScreenWidth / 2), (A_ScreenHeight / 3)

; game window
WinWait Mafia: Advanced Multiplayer Edition
WinActivate
; WinSetTitle, oakwood-game-2
WinMove, Mafia: Advanced Multiplayer Edition,, (A_ScreenWidth / 2), 0,,
Sleep 200
