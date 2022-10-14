#pragma once

#include <chrono>

using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::milliseconds;
using hr_clock = std::chrono::high_resolution_clock;
using time_point = hr_clock::time_point;

class stopwatch
{
private:
	time_point t0;

public:
	stopwatch(bool delayed = false)
		: t0()
	{
		if (!delayed)
			reset();
	}

	stopwatch(const stopwatch &) = delete;
	void operator=(const stopwatch &) = delete;

	auto now() const { return hr_clock::now(); }
	void reset() { t0 = now(); }

	template <class Duration = milliseconds>
	auto elapsed() const { return duration_cast<Duration>(now() - t0); }
};


class scoped_perf
{
private:
	std::string name;
	stopwatch sw;
	bool running;
	milliseconds dt;

public:
	scoped_perf(std::string name, bool delayed = false)
		: name(std::move(name)), sw(delayed), running(delayed), dt()
	{
		if (!delayed)
			start();
	}

	scoped_perf(const scoped_perf &) = delete;
	void operator=(const scoped_perf &) = delete;

	void start() { running = true; sw.reset(); }
	void stop() { dt = sw.elapsed<decltype(dt)>(); running = false; }
	void report() { std::cout << "{} took {} ms" << dt << std::endl; }

	auto elapsed() const { return dt; }

	~scoped_perf()
	{
		if (running)
		{
			stop();
			report();
		}
	}
};
