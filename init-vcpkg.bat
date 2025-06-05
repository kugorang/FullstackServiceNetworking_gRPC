@echo off
cd /d %~dp0

if not exist vcpkg (
    echo Cloning vcpkg...
    git clone https://github.com/Microsoft/vcpkg.git
    cd vcpkg
    call bootstrap-vcpkg.bat
    cd ..
)

cd vcpkg
echo Installing gRPC...
vcpkg install grpc:x64-windows
cd ..

echo vcpkg setup complete!
pause