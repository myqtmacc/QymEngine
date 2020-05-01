#pragma once
#include <cstdint>

#define SV_S_TO_MS (1000)
#define SV_MS_TO_US (1000)
#define SV_US_TO_100NS (10)
#define SV_MS_TO_100NS (SV_MS_TO_US * SV_US_TO_100NS)
#define SV_S_TO_US (SV_S_TO_MS * SV_MS_TO_US)
#define SV_S_TO_100NS (SV_S_TO_MS * SV_MS_TO_US * SV_US_TO_100NS)

class CStopwatchMS {
public:
	CStopwatchMS();
	void SetTime_ms(int64_t time_ms);
	int64_t GetTime_ms();
	void Reset();
	void Start();
	void Stop();
	bool Running() const;
private:
	bool m_bRunning;
	int64_t m_llTimeStart_ms;
	int64_t m_llTimeStop_ms;
};

class CStopwatchUS
{
public:
	CStopwatchUS();
	void SetTime_us(int64_t time_us);
	int64_t GetTime_us();
	void Reset();
	void Start();
	void Stop();
	bool Running() const;
private:
	bool m_bRunning;
	int64_t m_llTimeStart_us;
	int64_t m_llTimeStop_us;
};