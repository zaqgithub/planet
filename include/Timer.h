#pragma once
class Timer {
public:
	Timer() {
		begin =std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
	}
	~Timer() {
		end = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
		std::cout << "The time has been used is:" << (end.count() - begin.count())/1000.0<<"ms" << std::endl;
	}
private:
	std::chrono::microseconds begin;
	std::chrono::microseconds end;
};