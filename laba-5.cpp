#include <iostream>
#include <vector>

using namespace std;


template <class T>
class Set {
    /*
        Класс коллекция представляет собой двоичное дерево состоящее
        из struct ElementSet, которые ссылаются между собой тремя ссылками

        ------------------(root)---------------------
        ----------------ElementSet------------------
        ----------------/-------\-------------------
        ------------(left)-----(right)--------------
        ---------ElementSet---ElementSet------------

        Описание ElementSet

        struct ElementSet
            up -> link to upper ElementSet
            left -> link to less left ElementSet
            right -> link to bigger right ElementSet
            value -> value of ElementSet
    */

	// Private var-s

	struct ElementSet {
        T value;
		int hash; // <- по идеи не нужно
        ElementSet *up = nullptr;
        ElementSet *left = nullptr;
        ElementSet *right = nullptr;

        ElementSet(const T &val) : value(val), hash(std::hash<T>()(val)) {}
    };

    ElementSet *root = nullptr;
	ElementSet *elementMin = nullptr;
	ElementSet *elementMax = nullptr;
	ElementSet *elementMiddle = nullptr;

	int diff_points = -1;


	// add element to tree as ElementSet
    void add(ElementSet *&element, const T &value, ElementSet *parent = nullptr) {
        if (!element) {
            element = new ElementSet(value);
            element->up = parent;

			if (!this->root) this->root = element;

		// Рекурсивное движение по дереву влево-вправо в зависимости сравнения,
		// по итогу натыкаемся на element, у которого нет left или right
        } else if (value < element->value) {
            this->add(element->left, value, element);
			
        } else if (value > element->value) {
            this->add(element->right, value, element);
        }


		// Заранее определяем мин макс при заполнении дерева
		if (!elementMin || element->value < elementMin->value ) {
			this->elementMin = element; 
		}
		if (!elementMax || element->value > elementMax->value ) {
			this->elementMax = element; 
		}
    }
	void clear(ElementSet *element) {
        if (element) {
            clear(element->left);
            clear(element->right);
            delete element;
        }
    }

	int get_hash(const T &value) const {
		return std::hash<T>()(value);
	}

	void printElements(ostream& os, ElementSet* element) const {
		if (element) {
			printElements(os, element->left);
			os << element->value << " ";
			printElements(os, element->right);
		}
	}


	

	void getMiddleRecursive(ElementSet* element) {
		if (!element) {
			return;
		}

		getMiddleRecursive(element->right);
		
		if (this->elementMin && this->elementMax) {
			int average = (elementMax->value + elementMin->value) / 2;

			if (!elementMiddle) {
				this->elementMiddle = element;
				this->diff_points = abs(average - element->value);

			} else {
				int currentDiff = abs(average - elementMiddle->value);
				int newDiff = abs(average - element->value);

				if (newDiff < currentDiff) {
					this->elementMiddle = element;
					this->diff_points = newDiff;
				}
			}
		}

		getMiddleRecursive(element->left);
	}



	public:
		Set() {

		};
		Set(const Set &other_set) {
			cout << "Constructor other\n";
		};
		~Set() {
			clear(this->root);
		}

		// Вставка элемента в множество
        Set operator + (const T &value) {
            this->add(this->root, value);

			return *this;
        };
        friend Set operator + (const T &value, const Set &set) {
			set.add(set.root, value);

			return *set;
        };
		Set &operator += (const T &value)
		{
			this->add(this->root, value);

			return *this;
		};

		// Удаление элемента из множества
		// Set operator - (const T &t) const;
		Set &operator-=(const T &t)
		{

			return *this;
		};

		void createTreeRecursive(ElementSet* element, int depth = 0) const {
			if (!element) {
				for (int i = 0; i < depth; ++i) cout << "\t";
				cout << "None" << endl;
				return;
			}

			createTreeRecursive(element->right, depth + 1);
			
			for (int i = 0; i < depth; ++i) cout << "\t";
			cout << element->value << " <" << endl;

			createTreeRecursive(element->left, depth + 1);
		}

		void createTree() const {
			createTreeRecursive(this->root);
		}

		

		friend ostream& operator << (ostream& os, const Set<T>& set) {
			set.printElements(os, set.root);
			return os;
		}



		T getMin() const {
			return this->elementMin->value;
		}

		T getMax() const {
			return this->elementMax->value;
		}

		void optimize() {
			this->getMiddleRecursive(this->root);

			return;
		}

		T getMiddle() const {
			this->getMiddleRecursive(this->root);

			return this->elementMiddle->value;
		}
};
int main(void)
{
    Set<int> a;
	Set<int> b;
    
	for (int i=50; i>=0; i--) a+=i;

	b = a + 1;

	a.createTree();
    // for (int i=0; i<=50; i+=2) a-=i;
    // for (int i=0; i<=50; i+=3) a-=i;
    // cout << a << endl;
    // for (int i=0; i<=50; i+=2) a-=i;
    // for (int i=0; i<=50; i+=3) a-=i;

    //   cout << a << endl;
    //   Set<int> b = ((a + 999)+9999) - 89;
    //   cout << b.size() << endl;
    //   cout << b.exists(1) << b.exists(2)
    //     << b.exists(3) << b.exists(4)
    //     << b.exists(5) << endl;
    //   Set<int> *c = new Set<int>;
    //   *c +=1; *c +=5; *c +=6;
    //   cout << c->size() << endl;
    //   cout << a-*c << endl;
    //   *c += 1111;
    //   cout << a+*c << endl;
    //   delete c;
    return 0;
};