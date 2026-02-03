@echo off
REM SD:SourceDirectory
set SD=V:\code\
REM BD: BuildDirectory
set BD="V:\build\\"
set DisabledWarnings= -wd4100 -wd4189 -wd4706 -wd4530 -wd4505 -wd4201 -D_CRT_SECURE_NO_WARNINGS
set CompilerParams= %DisabledWarnings%  -nologo -Od -FC -MT -W4 -WX -Zi 
set Libraries=User32.lib D3D11.lib D3DCompiler.lib dxguid.lib
set LinkParams=%Libraries%
pushd V:\
if not exist build mkdir build
pushd %BD%
cd
cl %CompilerParams% -Tc %SD%Win32Platform.cpp %LinkParams% /Fe"Direct3D.exe"
popd
popd
