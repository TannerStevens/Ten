#pragma once
#include <cstdint>
#include <vector>

class IRandom
{
public:
	virtual ~IRandom() = default;
	virtual std::int32_t Next() = 0;
	virtual std::int32_t Next(std::int32_t maxValue) = 0;
	virtual std::int32_t Next(std::int32_t minValue, std::int32_t maxValue) = 0;
	virtual std::double_t NextDouble() = 0;
	virtual std::double_t NextDouble(std::double_t minValue, std::double_t maxValue) = 0;
	virtual void NextBytes(std::vector<std::uint8_t>& buffer) = 0;
};