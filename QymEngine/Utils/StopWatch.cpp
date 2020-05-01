#include "StopWatch.h"
#include <time.h>			

#define SV_S_TO_MS (1000)
#define SV_MS_TO_US (1000)
#define SV_US_TO_100NS (10)
#define SV_MS_TO_100NS (SV_MS_TO_US * SV_US_TO_100NS)
#define SV_S_TO_US (SV_S_TO_MS * SV_MS_TO_US)
#define SV_S_TO_100NS (SV_S_TO_MS * SV_MS_TO_US * SV_US_TO_100NS)


#if _WIN32
#include "windows.h"

static bool s_HaveFrequency = false;
static LARGE_INTEGER s_Frequency;

inline HRESULT GetTime_US(int64_t &time_us)
{
	if (!s_HaveFrequency)
	{
		QueryPerformanceFrequency(&s_Frequency);
		s_HaveFrequency = true;
	}
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	time_us = ((counter.QuadPart) * SV_S_TO_US) / s_Frequency.QuadPart;
	return S_OK;
}

#else
#include <sys/time.h>

static inline void GetTime_US(int64_t &time_us)
{
	timeval time;
	gettimeofday(&time, 0);
	time_us = time.tv_usec + time.tv_sec * SV_S_TO_US;
}
#endif
//
//HRESULT GetTime_US(int64_t &time_us)
//{
//	LARGE_INTEGER counter;
//	LARGE_INTEGER frequency;
//	QueryPerformanceCounter(&counter);
//	QueryPerformanceFrequency(&frequency);
//	time_us = (counter.QuadPart * SV_S_TO_US) / frequency.QuadPart;
//	return S_OK;
//}

CStopwatchMS::CStopwatchMS() :
m_bRunning(false),
m_llTimeStart_ms(0),
m_llTimeStop_ms(0)
{
	this->Reset();
	m_bRunning = false;
}

//人为设置已经经过的时间，实际上是把起始时间向前平移了time_ms
void CStopwatchMS::SetTime_ms(int64_t time_ms) {
	m_llTimeStart_ms = clock() - time_ms; //当前时间减去经过的时间为起始时间
	m_llTimeStop_ms = m_llTimeStart_ms + time_ms;
}

//获取从启动开始，经过了多长时间
int64_t CStopwatchMS::GetTime_ms() {
	if (m_bRunning) {
		m_llTimeStop_ms = clock();
	}
	return m_llTimeStop_ms - m_llTimeStart_ms;
}

void CStopwatchMS::Reset() {
	this->SetTime_ms(0);
}

void CStopwatchMS::Start() {
	const int64_t previous_time = m_llTimeStop_ms - m_llTimeStart_ms;
	m_llTimeStart_ms = clock() - previous_time; //重置起始时间，因为stop到再次start之间会有间隔，这段时间不计算在Stopwatch总时间内
	m_bRunning = true;
}

void CStopwatchMS::Stop() {
	m_llTimeStop_ms = clock();
	m_bRunning = false;
}

bool CStopwatchMS::Running() const {
	return m_bRunning;
}


CStopwatchUS::CStopwatchUS() :
m_bRunning(false),
m_llTimeStart_us(0),
m_llTimeStop_us(0)
{
	this->Reset();
	m_bRunning = false;
}
//设定当前的停表时间
void CStopwatchUS::SetTime_us(int64_t time_us) {
	int64_t current_us;
	//获取当前系统时间
	::GetTime_US(current_us);
	//根据设置的时间量，前推起始时间m_llTimeStart_us
	m_llTimeStart_us = current_us - time_us;
	//根据设置的时间量，设置停表的当前时间m_llTimeStop_us
	m_llTimeStop_us = m_llTimeStart_us + time_us;
}

int64_t CStopwatchUS::GetTime_us() {
	if (m_bRunning) {
		::GetTime_US(m_llTimeStop_us);
	}
	return m_llTimeStop_us - m_llTimeStart_us;
}

void CStopwatchUS::Reset() {
	this->SetTime_us(0);
}

void CStopwatchUS::Start() {
	//获取上一次停表之前经过的时间。
	const int64_t previous_time = m_llTimeStop_us - m_llTimeStart_us;

	////获取当前时间
	//LONGLONG current_us;
	//CSplitVideoUtility::GetTime_US(current_us);

	////重新将当前时间前推停表时间，作为起始。这样，下次每次获得的时间，减去起始就是就是停表的计时时间。
	//m_llTimeStart_us = current_us - previous_time;

	//上面这段的功能就相当于
	SetTime_us(previous_time);

	m_bRunning = true;
}

void CStopwatchUS::Stop() {
	::GetTime_US(m_llTimeStop_us);
	m_bRunning = false;
}

bool CStopwatchUS::Running() const {
	return m_bRunning;
}