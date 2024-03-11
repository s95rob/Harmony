#pragma once

#include <cstdio>
#include <cstdint>

class FileReader {
public:
	FileReader() = default;
	FileReader(const char* filepath) { Read(filepath); }

	bool Read(const char* filepath) {
		FILE* fp = fopen(filepath, "rb");

		if (!fp)
			return false;

		fseek(fp, 0, SEEK_END);
		m_Size = ftell(fp);
		rewind(fp);

		if (m_Data)
			delete[] m_Data;

		m_Data = new uint8_t[m_Size];
		fread(m_Data, 1, m_Size, fp);

		fclose(fp);
	}

	uint8_t* Data() const { return m_Data; }
	size_t Size() const { return m_Size; }

	operator bool() const { return m_Data != nullptr; }

private:
	uint8_t* m_Data = nullptr;
	size_t m_Size = 0;
};