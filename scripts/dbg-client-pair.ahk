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
WinSetTitle, mafiamp-game-1
WinMove, mafiamp-game-1,, 0, 0,(A_ScreenWidth / 2), (A_ScreenHeight / 1.5)
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
WinSetTitle, mafiamp-game-2
WinMove, mafiamp-game-2,, (A_ScreenWidth / 2), 0,(A_ScreenWidth / 2), (A_ScreenHeight / 1.5)
Sleep 200

; Check if windows exist
Sleep 5000

if !WinExist("mafiamp-game-1") or !WinExist("mafiamp-game-2") {
    WinKill, mafiamp-game-1
    WinKill, mafiamp-game-2
    Reload
}
