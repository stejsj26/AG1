#include <iostream>
#include <algorithm>
#include <map>


class Carol {
public:
	Carol(const int &length, const int &error, const int &startingPos, const int &endingPos) : m_Length(
			length), m_Error(error), m_StartingPos(startingPos), m_EndingPos(endingPos), m_Counter(0) {}
	
	void readRules();
	
	inline bool checkPalindrome(std::string &) const;
	
	void countCarols();
	
	void addSymbol(std::string, const int &);
	
	inline void printCarols() const;

private:
	int m_Length;
	int m_Error;
	int m_StartingPos;
	int m_EndingPos;
	long m_Counter;
	const static int MODULO = 1000000007;
	const static int MAX_POS = 500;
	std::map<int, char> m_Array[MAX_POS];
};

void Carol::readRules() {
	int pos, cnt;
	char tone;
	for (int i = 0; i < m_Length; ++i) {
		std::cin >> cnt;
		for (int j = 0; j < cnt; ++j) {
			std::cin >> pos >> tone;
			m_Array[i].insert(std::make_pair(pos, tone));
		}
	}
}

inline bool Carol::checkPalindrome(std::string &str) const {
	size_t len = str.length();
	for (size_t i = 0; i < len / 2; ++i) {
		if (str[i] != str[len - 1 - i])
			return false;
	}
	return true;
}

void Carol::countCarols() {
	std::string str;
	addSymbol(str, m_StartingPos);
	printCarols();
}

void Carol::addSymbol(std::string str, const int &start) {
	int cnt = 0;
	if (start == m_EndingPos) {
		if (checkPalindrome(str))
			++m_Counter;
		return;
	}
	for (auto i = m_Array[start].begin(); i != m_Array[start].end(); ++i) {
		++cnt;
		if (start == m_StartingPos)
			str.clear();
		if (cnt > 1 && !str.empty())
			str.erase(str.begin() + (str.size() - 1));
		str += i->second;
		addSymbol(str, i->first);
	}
	
}

inline void Carol::printCarols() const {
	std::cout << m_Counter % MODULO << std::endl;
}

Carol *readInput() {
	int length, error, startingPos, endingPos;
	std::cin >> length >> error;
	std::cin >> startingPos >> endingPos;
	return (new Carol(length, error, startingPos, endingPos));
}

int main() {
	Carol *carol = readInput();
	carol->readRules();
	carol->countCarols();
	delete carol;
	return 0;
}
