# Launches rename_map_gui.py inside WSL, with its window shown on the
# Windows desktop via WSLg. Meant to be started by RenameMapGUI.bat
# (double-click that from Explorer) rather than run directly.
#
# Requires Windows 11, or Windows 10 with `wsl --update` applied, so WSLg
# is available. Also requires python3-tk in the WSL distro:
#   sudo apt install python3-tk

$ErrorActionPreference = "Stop"

# $PSScriptRoot is the UNC path Explorer launched us from, e.g.
# \\wsl.localhost\Ubuntu\home\belial\AuburnGold\dev_scripts\map_renamer
if ($PSScriptRoot -match '^\\\\wsl(?:\.localhost)?\$?\\([^\\]+)\\(.*)$') {
    $distro = $Matches[1]
    $linuxPath = "/" + ($Matches[2] -replace '\\', '/')
} else {
    Write-Host "Couldn't figure out the WSL path from:"
    Write-Host "  $PSScriptRoot"
    Write-Host "Open this folder through Explorer's WSL entry (\\wsl.localhost\<distro>\...) and run the .bat from there."
    Read-Host "Press Enter to close"
    exit 1
}

$scriptPath = "$linuxPath/rename_map_gui.py"
Write-Host "Launching $scriptPath in WSL distro '$distro'..."

& wsl.exe -d $distro -- python3 $scriptPath
$exitCode = $LASTEXITCODE

if ($exitCode -ne 0) {
    Write-Host ""
    Write-Host "rename_map_gui.py exited with code $exitCode."
    Write-Host "If this says tkinter is missing, run inside WSL: sudo apt install python3-tk"
    Read-Host "Press Enter to close"
}
