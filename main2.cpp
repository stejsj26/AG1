#include <iostream>
#include <cstdio>
#include <cstdlib>

class BST {
private:
	struct node {
		node *m_Left;
		node *m_Right;
		node *m_Parent;
		int m_Value;
	};
	node *root;
public:
	BST() : root(nullptr) {}
	
	~BST();
	
	void destroyTree(node *&);
	
	node *search(const int &, node *&);
	
	void insertNode(const int &, node *&, node *const &parent = nullptr);
	
	node *removeNode(const int &, node *);
	
	std::pair<node *, int> findParent(const int &value);
	
	std::pair<node *, int> findSuccessor(const int &, node *const &);
	
	int rotateTree(const int &, const int &);
	
	void rotateLeft(node *&);
	
	void rotateRight(node *&);
	
	inline node *findMinValue(node *const &) const;
	
	inline node *findMaxValue(node *const &) const;
	
	void readInput();
	
	inline void printOutput(node *const &, const int &) const;
	
	inline void printOutput(const int &) const;
};

BST::~BST() {
	destroyTree(root);
	root = nullptr;
}

void BST::destroyTree(node *&source) {
	if (source == nullptr)
		return;
	else {
		destroyTree(source->m_Right);
		destroyTree(source->m_Left);
		delete source;
	}
}

BST::node *BST::search(const int &value, node *&source) {
	if (source == nullptr)
		return nullptr;
	else if (value > source->m_Value)
		return search(value, source->m_Right);
	else if (value < source->m_Value)
		return search(value, source->m_Left);
	else
		return source;
}

void BST::insertNode(const int &value, BST::node *&source, BST::node *const &parent) {
	if (source == nullptr) {
		source = new node;
		source->m_Left = source->m_Right = nullptr;
		source->m_Value = value;
		source->m_Parent = parent;
	} else if (value < source->m_Value)
		insertNode(value, source->m_Left, source);
	else if (value > source->m_Value)
		insertNode(value, source->m_Right, source);
}

BST::node *BST::removeNode(const int &value, BST::node *source) {
	node *tmp = nullptr;
	if (!source)
		return nullptr;
	else if (value < source->m_Value)
		source->m_Left = removeNode(value, source->m_Left);
	else if (value > source->m_Value)
		source->m_Right = removeNode(value, source->m_Right);
	else if ((source->m_Left && source->m_Right)) {
		tmp = findMinValue(source->m_Right);
		source->m_Value = tmp->m_Value;
		source->m_Right = removeNode(source->m_Value, source->m_Right);
	} else if (!source->m_Left && !source->m_Right && source != root) {
		delete source;
		return nullptr;
	} else if (!source->m_Left && !source->m_Right && source == root) {
		delete source;
		root = nullptr;
		return nullptr;
	} else if (!source->m_Left && source->m_Right) {
		tmp = source;
		source->m_Value = source->m_Right->m_Value;
		source->m_Left = source->m_Right->m_Left;
		if (source->m_Right->m_Left)
			source->m_Right->m_Left->m_Parent = source;
		source->m_Right->m_Left = nullptr;
		source->m_Right = removeNode(tmp->m_Value, source->m_Right);
	} else if (!source->m_Right && source->m_Left) {
		tmp = source;
		source->m_Value = source->m_Left->m_Value;
		source->m_Right = source->m_Left->m_Right;
		if (source->m_Left->m_Right)
			source->m_Left->m_Right->m_Parent = source;
		source->m_Left->m_Right = nullptr;
		source->m_Left = removeNode(tmp->m_Value, source->m_Left);
	}
	return source;
}

std::pair<BST::node *, int> BST::findParent(const int &value) {
	node *tmp = search(value, root);
	if (tmp == nullptr)
		return std::make_pair(nullptr, -1);
	else
		return std::make_pair(tmp->m_Parent, 1);
}

std::pair<BST::node *, int> BST::findSuccessor(const int &value, BST::node *const &source) {
	node *tmp = search(value, root);
	if (tmp == nullptr)
		return std::make_pair(tmp, -1);
	else if (value == findMaxValue(root)->m_Value) {
		return std::make_pair(nullptr, 1);
	} else if (tmp->m_Right) {
		return std::make_pair(findMinValue(tmp->m_Right), 1);
	} else {
		node *tmp1 = tmp->m_Parent;
		node *tmp2 = tmp;
		while (tmp2 != nullptr && tmp2 == tmp1->m_Right) {
			tmp2 = tmp1;
			tmp1 = tmp1->m_Parent;
		}
		return std::make_pair(tmp1, 1);
	}
}

int BST::rotateTree(const int &value, const int &orientation) {
	node *source = search(value, root);
	if (source == nullptr) {
		return -1;
	} else if ((orientation == 1 && !source->m_Right) || (orientation == 2 && !source->m_Left)) {
		return 0;
	} else {
		if (orientation == 1)
			rotateLeft(source);
		else
			rotateRight(source);
		return 1;
	}
}

void BST::rotateLeft(BST::node *&source) {
	node *tmp = source->m_Right;
	source->m_Right = tmp->m_Left;
	if (tmp->m_Left != nullptr)
		tmp->m_Left->m_Parent = source;
	tmp->m_Parent = source->m_Parent;
	if (source->m_Parent == nullptr)
		root = tmp;
	else if (source == source->m_Parent->m_Right)
		source->m_Parent->m_Right = tmp;
	else
		source->m_Parent->m_Left = tmp;
	tmp->m_Left = source;
	source->m_Parent = tmp;
}

void BST::rotateRight(BST::node *&source) {
	node *tmp = source->m_Left;
	source->m_Left = tmp->m_Right;
	if (tmp->m_Right != nullptr)
		tmp->m_Right->m_Parent = source;
	tmp->m_Parent = source->m_Parent;
	if (source->m_Parent == nullptr)
		root = tmp;
	else if (source == source->m_Parent->m_Left)
		source->m_Parent->m_Left = tmp;
	else
		source->m_Parent->m_Right = tmp;
	tmp->m_Right = source;
	source->m_Parent = tmp;
}

inline BST::node *BST::findMinValue(BST::node *const &source) const {
	if (source == nullptr)
		return nullptr;
	else if (source->m_Left != nullptr)
		return findMinValue(source->m_Left);
	else return source;
}

inline BST::node *BST::findMaxValue(BST::node *const &source) const {
	if (source == nullptr)
		return nullptr;
	else if (source->m_Right != nullptr)
		return findMaxValue(source->m_Right);
	else return source;
}

void BST::readInput() {
	int tempValue, operation, orientation;
	while (std::cin) {
		std::cin >> operation;
		switch (operation) {
			case 1:
				std::cin >> tempValue;
				insertNode(tempValue, root);
				break;
			case 2:
				std::cin >> tempValue;
				removeNode(tempValue, root);
				break;
			case 3:
				std::cin >> tempValue;
				if (findParent(tempValue).second == -1)
					printOutput(-1);
				else
					printOutput(findParent(tempValue).first, 3);
				break;
			case 4:
				std::cin >> tempValue;
				if (findSuccessor(tempValue, root).second == -1)
					printOutput(-1);
				else
					printOutput(findSuccessor(tempValue, root).first, 4);
				break;
			case 5:
				std::cin >> tempValue >> orientation;
				printOutput(rotateTree(tempValue, orientation));
				break;
			default:
				return;
		}
	}
}

inline void BST::printOutput(node *const &source, const int &op) const {
	switch (op) {
		case 3:
			if (!source) {
				std::cout << "noparent" << std::endl;
				return;
			}
			std::cout << source->m_Value << std::endl;
			return;
		case 4:
			if (!source) {
				std::cout << "nosuccessor" << std::endl;
				return;
			}
			std::cout << source->m_Value << std::endl;
			return;
		default:
			return;
	}
}

inline void BST::printOutput(const int &check) const {
	switch (check) {
		case -1:
			std::cout << "notfound" << std::endl;
			return;
		case 0:
			std::cout << "norotate" << std::endl;
			return;
		default:
			return;
	}
}

int main() {
	BST bst;
	bst.readInput();
	return 0;
}
