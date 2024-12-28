#Warn  ; We keep this as it's still useful in v2
SendMode "Input"

BinaryFolder := A_ScriptDir "\..\..\..\..\builds\build-64\bin"

SetWorkingDir BinaryFolder

;
; First instance
;
Run "MafiaMPLauncher.exe"

; console
WinWait "mafiamp: dev-console"
WinActivate
WinSetTitle "mafiamp-cli-1"
WinMove 0, (A_ScreenHeight / 1.5), (A_ScreenWidth / 2), (A_ScreenHeight / 3), "mafiamp-cli-1"

; game window
WinWait "Mafia: Multiplayer"
WinActivate
WinSetTitle "mafiamp-game-1"
WinMove 0, 0, (A_ScreenWidth / 2), (A_ScreenHeight / 1.5), "mafiamp-game-1"
Sleep 500

;
; Second instance
;
Run "MafiaMPLauncher.exe"

; console
WinWait "mafiamp: dev-console"
WinActivate
WinSetTitle "mafiamp-cli-2"
WinMove (A_ScreenWidth / 2), (A_ScreenHeight / 1.5), (A_ScreenWidth / 2), (A_ScreenHeight / 3), "mafiamp-cli-2"

; game window
WinWait "Mafia: Multiplayer"
WinActivate
WinSetTitle "mafiamp-game-2"
WinMove (A_ScreenWidth / 2), 0, (A_ScreenWidth / 2), (A_ScreenHeight / 1.5), "mafiamp-game-2"
Sleep 500

; Check if windows exist
Sleep 5000

if !WinExist("mafiamp-game-1") or !WinExist("mafiamp-game-2") {
    WinKill "mafiamp-game-1"
    WinKill "mafiamp-game-2"
    Sleep 2000
    Reload
}