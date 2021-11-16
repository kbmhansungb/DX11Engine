#pragma once

// 포스트 프로세스가 아닌 디퍼드 프로세스로 이름을 지은 이유는
// Render to texture의 Present부분에서 호출 하기 때문임.
// 
// 디퍼드 파이프라인의 헤더를 정리해서 모아둠
// 주로 포스트 프로세스등이 모여있을 것이라고 예상됨
// 21.11.16

#include "LightProcess.h"
#include "Blur.h"