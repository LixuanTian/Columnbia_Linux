// linux_compat.h - Linux compatibility layer for Windows MFC code
// This file provides Linux equivalents for Windows/MFC types and functions

#ifndef LINUX_COMPAT_H
#define LINUX_COMPAT_H
#define LINUX_PORT

#ifdef LINUX_PORT

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>

// CString replacement using std::string
class CString : public std::string {
private:
	mutable std::vector<char> buffer;
	
public:
	CString() : std::string() {}
	CString(const char* s) : std::string(s ? s : "") {}
	CString(const std::string& s) : std::string(s) {}
	CString(const CString& s) : std::string(s) {}
	
	// MFC-like methods
	int GetLength() const { return length(); }
	bool IsEmpty() const { return empty(); }
	void Empty() { clear(); }
	void Format(const char* fmt, ...);
	char* GetBuffer(int nMinBufLength) { 
		buffer.resize(nMinBufLength + 1);
		if (!empty()) {
			strncpy(&buffer[0], c_str(), nMinBufLength);
			buffer[nMinBufLength] = '\0';
		} else {
			buffer[0] = '\0';
		}
		return &buffer[0];
	}
	void ReleaseBuffer(int nNewLength = -1) {
		if (!buffer.empty()) {
			if (nNewLength >= 0) {
				buffer[nNewLength] = '\0';
			}
			assign(&buffer[0]);
			buffer.clear();
		}
	}
	CString Left(int nCount) const { return substr(0, nCount); }
	CString Right(int nCount) const { 
		if (nCount >= (int)length()) return *this;
		return CString(substr(length() - nCount)); 
	}
	CString Mid(int nFirst, int nCount = -1) const {
		if (nCount < 0) return substr(nFirst);
		return substr(nFirst, nCount);
	}
	int Find(char ch, int nStart = 0) const {
		size_t pos = find(ch, nStart);
		return (pos == std::string::npos) ? -1 : (int)pos;
	}
	int Find(const char* lpszSub, int nStart = 0) const {
		size_t pos = find(lpszSub, nStart);
		return (pos == std::string::npos) ? -1 : (int)pos;
	}
	void MakeUpper() {
		for (size_t i = 0; i < length(); i++)
			(*this)[i] = toupper((*this)[i]);
	}
	void MakeLower() {
		for (size_t i = 0; i < length(); i++)
			(*this)[i] = tolower((*this)[i]);
	}
	
	// String comparison (case-sensitive)
	int Compare(const char* s) const {
		if (!s) return empty() ? 0 : 1;
		return strcmp(c_str(), s);
	}
	int Compare(const CString& s) const {
		return strcmp(c_str(), s.c_str());
	}
	
	// String comparison (case-insensitive)
	int CompareNoCase(const char* s) const {
		if (!s) return empty() ? 0 : 1;
		return strcasecmp(c_str(), s);
	}
	int CompareNoCase(const CString& s) const {
		return strcasecmp(c_str(), s.c_str());
	}
	
	// Operators
	CString& operator=(const char* s) {
		std::string::operator=(s ? s : "");
		return *this;
	}
	CString& operator=(const std::string& s) {
		std::string::operator=(s);
		return *this;
	}
	CString operator+(const char* s) const {
		CString result(*this);
		result += s;
		return result;
	}
	CString operator+(const CString& s) const {
		CString result(*this);
		result += s;
		return result;
	}
	CString& operator+=(const char* s) {
		std::string::operator+=(s);
		return *this;
	}
	CString& operator+=(const CString& s) {
		std::string::operator+=(s);
		return *this;
	}
	CString& operator+=(char c) {
		std::string::operator+=(c);
		return *this;
	}
};

// CArray template replacement
template<class TYPE, class ARG_TYPE = const TYPE&>
class CArray : public std::vector<TYPE> {
public:
	CArray() : std::vector<TYPE>() {}
	
	int GetSize() const { return this->size(); }
	int GetUpperBound() const { return this->size() - 1; }
	void SetSize(int nNewSize, int nGrowBy = -1) { this->resize(nNewSize); }
	void RemoveAll() { this->clear(); }
	TYPE& ElementAt(int nIndex) { return (*this)[nIndex]; }
	const TYPE& ElementAt(int nIndex) const { return (*this)[nIndex]; }
	void Add(ARG_TYPE newElement) { this->push_back(newElement); }
	void SetAt(int nIndex, ARG_TYPE newElement) { (*this)[nIndex] = newElement; }
	TYPE GetAt(int nIndex) const { return (*this)[nIndex]; }
	void InsertAt(int nIndex, ARG_TYPE newElement, int nCount = 1) {
		this->insert(this->begin() + nIndex, nCount, newElement);
	}
	void RemoveAt(int nIndex, int nCount = 1) {
		this->erase(this->begin() + nIndex, this->begin() + nIndex + nCount);
	}
	void Copy(const CArray& src) {
		this->clear();
		this->insert(this->end(), src.begin(), src.end());
	}
};

// CFile replacement
class CFile {
public:
	enum OpenFlags {
		modeRead = 0x0001,
		modeWrite = 0x0002,
		modeCreate = 0x1000,
		modeNoTruncate = 0x2000
	};
	
	CFile() : m_file(NULL), m_bOpen(false) {}
	~CFile() { Close(); }
	
	bool Open(const char* lpszFileName, unsigned int nOpenFlags) {
		Close();
		const char* mode = "r";
		if (nOpenFlags & modeWrite) {
			if (nOpenFlags & modeCreate)
				mode = "w";
			else if (nOpenFlags & modeNoTruncate)
				mode = "a";
			else
				mode = "w";
		}
		m_file = fopen(lpszFileName, mode);
		m_bOpen = (m_file != NULL);
		return m_bOpen;
	}
	
	void Close() {
		if (m_file) {
			fclose(m_file);
			m_file = NULL;
		}
		m_bOpen = false;
	}
	
	void Write(const void* lpBuf, unsigned int nCount) {
		if (m_file)
			fwrite(lpBuf, 1, nCount, m_file);
	}
	
	unsigned int Read(void* lpBuf, unsigned int nCount) {
		if (m_file)
			return fread(lpBuf, 1, nCount, m_file);
		return 0;
	}
	
	void Flush() {
		if (m_file)
			fflush(m_file);
	}
	
	FILE* GetHandle() { return m_file; }
	
private:
	FILE* m_file;
	bool m_bOpen;
};

// _timeb replacement
struct _timeb {
	time_t time;
	unsigned short millitm;
};

// _ftime replacement
inline void _ftime(struct _timeb* timeptr) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	timeptr->time = tv.tv_sec;
	timeptr->millitm = tv.tv_usec / 1000;
}

// Windows directory functions
inline int _mkdir(const char* dirname) {
	return mkdir(dirname, 0755);
}

inline char* _getcwd(char* buffer, int maxlen) {
	return getcwd(buffer, maxlen);
}

// ASSERT macro
#ifndef ASSERT
#include <cassert>
#define ASSERT(x) assert(x)
#endif

// TRACE macro (simplified)
#ifndef TRACE
#define TRACE printf
#endif

// itoa function (Windows compatibility)
// Linux doesn't have itoa, so we provide our own implementation
inline char* itoa(int value, char* str, int base) {
	if (base < 2 || base > 36) {
		*str = '\0';
		return str;
	}
	
	char* ptr = str;
	char* ptr1 = str;
	char tmp_char;
	int tmp_value;
	
	// Handle negative numbers for base 10
	if (value < 0 && base == 10) {
		*ptr++ = '-';
		value = -value;
		ptr1++;
	}
	
	// Convert to string (reversed)
	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "0123456789abcdefghijklmnopqrstuvwxyz"[tmp_value - value * base];
	} while (value);
	
	*ptr-- = '\0';
	
	// Reverse the string
	while (ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}
	
	return str;
}

// AFX macros
#define AFX_MANAGE_STATE(x)
#define DECLARE_DYNCREATE(class_name)
#define IMPLEMENT_DYNCREATE(class_name, base_class_name)
#define DECLARE_MESSAGE_MAP()
#define BEGIN_MESSAGE_MAP(theClass, baseClass)
#define END_MESSAGE_MAP()
#define afx_msg

// Dummy types for removed GUI code
class CWnd {};
class CDialog : public CWnd {};
class CFrameWnd : public CWnd {};
class CDocument {};
class CEditView {};
class CWinApp {};
class CDC {};
class CArchive {};
class CDumpContext {};
class CCreateStruct {};
class CButton {};
class CToolTipCtrl {};
class CDataExchange {};
typedef void* HWND;
typedef void* LPMSG;
typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;
typedef const char* LPCTSTR;
typedef char* LPTSTR;
typedef CCreateStruct CREATESTRUCT;

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

#endif // LINUX_PORT

#endif // LINUX_COMPAT_H
