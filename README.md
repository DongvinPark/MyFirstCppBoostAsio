# Boost.Asio를 사용한 C++ 프로젝트 기본 형식

## 실행방법
1. Linux OS가 설치된 AWS EC2 또는 Ubuntu 가 설치된 실제 컴퓨터를 준비한다.
2. 현재 깃허브 리포지토리를 준비된 머신에 클론한다.
3. 준비된 머신에 GCC, G++, CMake, Boost.Asio를 설치한다.
4. 리눅스 환경이라면, build_and_run_on_linux.sh 에 'chmod +x build_and_run_on_linux.sh' 실행권한을 준 다음, 실행하면 된다.
5. MacOS 환경이라면, 프로젝트 root 디렉토리 내에 build 디렉토리를 만든 다음, 그 디렉토리로 이동해서 'cmake ..', 'make' 명령어를 실행하여 실행파일을 만든다. 그 후 실행파일을 './실행파일이름' 명령어로 수동으로 실행하면 된다.