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

//��Ϊ�����Ѿ�������ʱ�䣬ʵ�����ǰ���ʼʱ����ǰƽ����time_ms
void CStopwatchMS::SetTime_ms(int64_t time_ms) {
	m_llTimeStart_ms = clock() - time_ms; //��ǰʱ���ȥ������ʱ��Ϊ��ʼʱ��
	m_llTimeStop_ms = m_llTimeStart_ms + time_ms;
}

//��ȡ��������ʼ�������˶೤ʱ��
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
	m_llTimeStart_ms = clock() - previous_time; //������ʼʱ�䣬��Ϊstop���ٴ�start֮����м�������ʱ�䲻������Stopwatch��ʱ����
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
//�趨��ǰ��ͣ��ʱ��
void CStopwatchUS::SetTime_us(int64_t time_us) {
	int64_t current_us;
	//��ȡ��ǰϵͳʱ��
	::GetTime_US(current_us);
	//�������õ�ʱ������ǰ����ʼʱ��m_llTimeStart_us
	m_llTimeStart_us = current_us - time_us;
	//�������õ�ʱ����������ͣ��ĵ�ǰʱ��m_llTimeStop_us
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
	//��ȡ��һ��ͣ��֮ǰ������ʱ�䡣
	const int64_t previous_time = m_llTimeStop_us - m_llTimeStart_us;

	////��ȡ��ǰʱ��
	//LONGLONG current_us;
	//CSplitVideoUtility::GetTime_US(current_us);

	////���½���ǰʱ��ǰ��ͣ��ʱ�䣬��Ϊ��ʼ���������´�ÿ�λ�õ�ʱ�䣬��ȥ��ʼ���Ǿ���ͣ��ļ�ʱʱ�䡣
	//m_llTimeStart_us = current_us - previous_time;

	//������εĹ��ܾ��൱��
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