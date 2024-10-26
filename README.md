# Boost.Asio를 사용한 C++ 프로젝트 기본 형식

## Linux & MacOS(M series chip) 실행방법
1. Linux OS가 설치된 AWS EC2 또는 Ubuntu 가 설치된 실제 컴퓨터를 준비한다.
2. 현재 깃허브 리포지토리를 준비된 머신에 클론한다.
3. 준비된 머신에 GCC, G++, CMake, Boost.Asio를 설치한다. Mac이라면 homebrew를 사용한다.
4. 리눅스 환경이라면, build_and_run_on_linux로 시작하는 .sh 파일에 'chmod +x .sh 파일명' 명령어로 실행권한을 준 다음, 실행하면 된다.
5. MacOS 환경이라면, 프로젝트 root 디렉토리 내에 build 디렉토리를 만든 다음, 그 디렉토리로 이동해서 'cmake ..', 'make' 명령어를 순서대로 실행하여 실행파일을 만든다. 그 후 실행파일을 './실행파일이름' 명령어로 수동으로 실행하면 된다.
6. Linux 또는 MacOS 환경에서 JetBrains CLion IDE를 사용하고, 3번 단계에서 언급한 툴들이 전부 설치돼 있다면 현재 프로젝트를 바로 IDE로 열고 실행시키면 된다.

## Window 환경 CLion 실행방법
- Window 환경에서 실행할 경우, vcpkg라는 윈도우용 C++ 패키지 매니저 설치 및 셋팅, 해당 패키지 매니저로 boost 라이브러리 설치, CLion에 boost 포함시키기라는 다소 많은 단계를 거쳐야 한다.
1. 우선 윈도우 커맨드 프롬프트를 연다.
윈도우 키 + R 눌러서 실행창 띄운 다음 cmd 라고 입력해서 엔터 치면 된다.<br><br/>

2. C 드라이브로 이동한 다음,
vcpkg 공식 깃허브를 깃 클론 완료한 다음, vcpkg라는 디렉토리로 진입한다.<br>
```text
git clone https://github.com/microsoft/vcpkg.git
```

3. 아래의 명령어를 사용해서 vcpkg의 부트스트랩 스트립트를 실행시킨다.
   이 명령어의 맨 앞 글자에 '마침표'가 있을을 기억하라.
```text
.\bootstrap-vcpkg.bat
```

4. 아래의 명령어를 사용해서 boost의 모든 라이브러리를 vcpkg를 이용해서 전부 설치한다. 30분 이상 걸릴 수도 있다.
```text
.\vcpkg install boost
```

5. 설치가 전부 끝났다면, CLion을 실행한 후,
```text
File > setting > Build, Execution, Deployment > CMake
```
 의 순서로 메뉴에 진입한다.
그 안의 여러 칸들 중,
"CMake options"에다가 다음을 입력한다.
```text
-DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
```
이게 정확하게 작동하기 위해서는,
vcpkg 깃허브 리포지토리를 정확하게 C 드라이브 최상위 디렉토리에다가 클론한 상태여야 한다. 중요한 것은, -DCMAKE_TOOLCHAIN_FILE이 필요로 하는 값이 vcpkg.cmake 라는 파일이 실제로 위치하는 디렉토리 경로여야 한다는 점이다<br>

6. 현재 프로젝트를 첫 git clone 했을 때, CMakeLists.txt에 작성돼 있던 내용을 전부 삭제하고, 아래의 내용으로 통째로 대체해야 한다.
<br>물론, vcpkg와 boost 라이브러리의 설치 및 CLion IDE의 셋팅이 위에서 설명한 대로 진행돼 있는 상태여야 한다.
```CMake
cmake_minimum_required(VERSION 3.29)
project(MyBoostAsioProject)

set(CMAKE_CXX_STANDARD 20)

# Add Vcpkg toolchain file if using Vcpkg
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

find_package(Boost REQUIRED COMPONENTS system)

add_executable(MyBoostAsioProject src/main.cpp)

# Link Boost.System and Winsock (ws2_32)
target_link_libraries(MyBoostAsioProject Boost::system ws2_32)
```