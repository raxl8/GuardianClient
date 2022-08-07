#pragma once

class PacketStream
{
public:
	PacketStream()
		: m_ReadCursor(0ULL), m_WriteCursor(0ULL)
	{
	}

	PacketStream(uint8_t* data, size_t size)
		: m_Data(data, data + size), m_ReadCursor(0ULL), m_WriteCursor(0ULL)
	{
	}

	PacketStream(const std::vector<uint8_t>& data)
		: m_Data(data), m_ReadCursor(0ULL), m_WriteCursor(0ULL)
	{
	}

	inline const std::vector<uint8_t>& GetData() { return m_Data; }
	inline uint64_t GetReadCursor() { return m_ReadCursor; }
	inline uint64_t GetWriteCursor() { return m_WriteCursor; }

	template<typename T>
	T Read(uint64_t offset = 0)
	{
		m_ReadCursor += offset;
		if (!EnsureReadableSize(sizeof(T)))
			return T{};

		auto data = ReadBytes(sizeof(T));
		if (data.size() != sizeof(T))
			return T{};

		return *(T*)&data[0];
	}

	std::vector<uint8_t> ReadBytes(size_t size, uint64_t offset = 0)
	{
		m_ReadCursor += offset;
		if (!EnsureReadableSize(size))
			return {};

		std::vector<uint8_t> data(size);
		std::memcpy(&data[0], &m_Data[m_ReadCursor], size);
		m_ReadCursor += size;

		return data;
	}

	std::string ReadString(uint64_t offset = 0)
	{
		m_ReadCursor += offset;

		int length = Read<int>();
		auto value = std::string(&m_Data[m_ReadCursor], &m_Data[m_ReadCursor] + length);
		m_ReadCursor += length;

		return value;
	}

	template<typename T>
	void Write(const T& value, uint64_t offset = 0)
	{
		WriteBytes(&value, sizeof(T), offset);
	}

	void WriteBytes(const void* data, size_t size, uint64_t offset = 0)
	{
		m_WriteCursor += offset;

		EnsureWriteableSize(size);

		std::memcpy(&m_Data[m_WriteCursor], data, size);
		m_WriteCursor += size;
	}

	void WriteBytes(const std::vector<uint8_t>& data, uint64_t offset = 0)
	{
		WriteBytes(&data[0], data.size(), offset);
	}

	void WriteString(std::string_view value, uint64_t offset = 0)
	{
		m_WriteCursor += offset;

		auto stringLength = (int)value.size();
		EnsureWriteableSize(sizeof(stringLength) + value.size());

		Write(stringLength);

		if (value.size() > 0)
		{
			std::memcpy(&m_Data[m_WriteCursor], value.data(), value.size());
			m_WriteCursor += value.size();
		}
	}

private:
	bool EnsureReadableSize(size_t size)
	{
		return m_ReadCursor + size <= m_Data.size();
	}

	void EnsureWriteableSize(size_t size)
	{
		if (m_WriteCursor + size > m_Data.size())
		{
			m_Data.resize(m_WriteCursor + size);
		}
	}

private:
	std::vector<uint8_t> m_Data;
	uint64_t m_ReadCursor;
	uint64_t m_WriteCursor;
};
