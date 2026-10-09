# PowerShell helper to gather built .vst3 and optionally run Inno Setup (ISCC)
# Usage: Open PowerShell in repo root and run: .\Installer\package.ps1 [-RunISCC]
param(
	[switch]$RunISCC
)

$dist = Join-Path -Path (Get-Location) -ChildPath "dist"
if(!(Test-Path $dist)) { New-Item -ItemType Directory -Path $dist | Out-Null }

Write-Host "Searching for .vst3 bundles in build folders..."
$searchPaths = @(
	"./Builds",
	"./build",
	"./bin",
	"./Debug",
	"./Release",
	"."
)

$found = @()
foreach($p in $searchPaths) {
	if(Test-Path $p) {
		$bundles = Get-ChildItem -Path $p -Recurse -Filter "*.vst3" -Directory -ErrorAction SilentlyContinue
		if($bundles) { $found += $bundles }
	}
}

# Also do a broad recursive search as a fallback
if($found.Count -eq 0) {
	Write-Host "No bundles found in common build folders, doing broad recursive search (this may take a while)..."
	$found = Get-ChildItem -Path . -Recurse -Filter "*.vst3" -Directory -ErrorAction SilentlyContinue
}

if($found.Count -eq 0) {
	Write-Host "No .vst3 bundles found. Build the plugin first and copy the Funkit.vst3 folder into ./dist."
	exit 1
}

# Prefer a bundle named Funkit.vst3
$target = $found | Where-Object { $_.Name -ieq "Funkit.vst3" } | Select-Object -First 1
if(-not $target) { $target = $found | Select-Object -First 1 }

Write-Host "Found bundle: $($target.FullName)"
$dest = Join-Path $dist "Funkit.vst3"

# Remove existing dist copy
if(Test-Path $dest) { Remove-Item -Recurse -Force $dest }

Write-Host "Copying to $dest"
Copy-Item -Path $target.FullName -Destination $dest -Recurse

if($RunISCC) {
	$iscc = Get-Command ISCC.exe -ErrorAction SilentlyContinue
	if(-not $iscc) {
		# Common Inno Setup path
		$candidate = "C:\\Program Files (x86)\\Inno Setup 6\\ISCC.exe"
		if(Test-Path $candidate) { $iscc = $candidate }
	}

	if($iscc) {
		Write-Host "Running ISCC to create installer..."
		& $iscc "Installer\FunkitInstaller.iss"
		if($LASTEXITCODE -eq 0) { Write-Host "Installer built: FunkitInstaller.exe" } else { Write-Host "ISCC returned exit code $LASTEXITCODE" }
	} else {
		Write-Host "ISCC not found. Install Inno Setup and re-run with -RunISCC to build the installer."
	}
} else {
	Write-Host "Done. To create the installer, run ISCC on Installer\\FunkitInstaller.iss after placing ./dist/Funkit.vst3 in place."
}
