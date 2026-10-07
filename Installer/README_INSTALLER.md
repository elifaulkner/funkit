Building the Funkit VST3 installer (Windows, Inno Setup)

What this produces
- A dist/Funkit.vst3 folder containing the built VST3 plugin bundle
- An Inno Setup script Installer/FunkitInstaller.iss that packages the bundle into an EXE installer

Prerequisites (on your Windows machine)
- Visual Studio (or your preferred build system) with the JUCE project configured
- Inno Setup (https://jrsoftware.org/isinfo.php) to compile the .iss script (ISCC.exe)

Steps to build and package locally
1. Build the plugin in Release x64
   - Open the JUCE project (Funkit.jucer) in the Projucer or open the generated Visual Studio solution under Builds/VisualStudio2022 (or the Builds folder for your exporter)
   - Build the plugin for Release and x64 target (so it produces a Funkit.vst3 bundle). Note: JUCE will output a folder named Funkit.vst3 (it contains the plugin DLL and resources).

2. Prepare the dist folder
   - Create a folder named dist at the repository root
   - Copy the built Funkit.vst3 folder into dist so the path is ./dist/Funkit.vst3

3. Compile the Inno Setup script
   - Install Inno Setup and ensure ISCC.exe is on your PATH (or note its full path)
   - From the repo root, run (PowerShell or CMD):
	   "C:\\Program Files (x86)\\Inno Setup 6\\ISCC.exe" Installer\\FunkitInstaller.iss
   - If ISCC is on PATH you can simply run:
	   ISCC Installer\\FunkitInstaller.iss
   - This will produce FunkitInstaller.exe in the current folder

4. Test the installer
   - Run FunkitInstaller.exe on a machine and verify that Funkit.vst3 is installed into "C:\\Program Files\\Common Files\\VST3\\Funkit.vst3"

Automation script (optional)
- An example PowerShell script Installer/package.ps1 is included to copy any found .vst3 into ./dist and optionally run ISCC if installed.

If you want, I can:
- Try to build a Release x64 binary here and place it in ./dist (but the current environment may not produce a Windows VST3 bundle)
- Attempt to run ISCC here (Inno Setup may not be installed in this environment)
- Add an MSI/WiX or NSIS script instead
