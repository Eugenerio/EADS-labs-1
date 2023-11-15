#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include <iostream>
using namespace std;


template <typename Key, typename Info>
class Sequence{
private:
    class Node {
    public:
        Key key;
        Info info;
        Node* next;

        Node(const Key& _key, const Info& _info, Node* _next = nullptr)
                : key(_key), info(_info), next(_next) {}
    };

    Node* head = nullptr;
    Node* tail = nullptr;
    unsigned int length = 0;

    /**
     * @brief Finds the node with the specified key and occurrence.
     * @param key The key to search for.
     * @param occurrence Specifies which occurrence of the key to consider. Defaults to 1.
     * @return Node* Pointer to the found node or nullptr if not found.
     */
    Node* getNode(const Key& key, unsigned int occurrence = 1) {
        Node* current = head;
        unsigned int count = 0;

        while (current != nullptr) {
            if (current->key == key && ++count == occurrence) {
                return current;
            }
            current = current->next;
        }

        return nullptr;
    }

    /**
     * @brief Finds the node before the node with the specified key and occurrence.
     * @param key The key to search for.
     * @param occurrence Specifies which occurrence of the key to consider. Defaults to 1.
     * @return Node* Pointer to the node before the found node or nullptr if not found.
     */
    Node* getNodeBefore(const Key& key, unsigned int occurrence = 1) {
        Node* prev = nullptr;
        Node* curr = head;
        unsigned int count = 0;

        while (curr != nullptr) {
            if (curr->key == key && ++count == occurrence) {
                return prev;
            }
            prev = curr;
            curr = curr->next;
        }

        return nullptr;
    }


public:
    Sequence() {};
    ~Sequence(){
        clear();
    };
    Sequence(const Sequence &src) {
        *this = src;
    };

    /**
     * @brief Assignment operator for the Sequence class.
     * Clears the current sequence and copies elements from the source sequence.
     * @param src The source sequence to copy.
     * @return Reference to the updated sequence.
     */
    Sequence& operator=(const Sequence& src){
        if (this != &src) {
            clear();
            auto iterator = src.begin();

            // Copy elements from the src using the iterator
            for (int i = 0; i < src.getLength(); i++) {
                pushBack(iterator.key(), iterator.info());
                iterator++;
            }
        }
        return *this;
    };

    class Iterator{
    private:
        Node *current;

    public:
        Iterator(Node *ptr = nullptr) : current(ptr){};
        ~Iterator()= default;
        Iterator(const Iterator &src)
        {
            this->current = src.current;
        };

        /**
         * @brief Assignment operator for Iterator.
         * @param src Iterator to be assigned.
         * @return Reference to the assigned Iterator.
         */
        Iterator& operator=(const Iterator& src) {
            if (this != &src) {
                current = src.current;
            }
            return *this;
        }

        /**
         * @brief Equality operator for Iterator.
         * @param src Iterator to be compared.
         * @return True if both iterators are equal, false otherwise.
         */
        bool operator==(const Iterator& src) const {
            return current == src.current;
        }

        /**
         * @brief Inequality operator for Iterator.
         * @param src Iterator to be compared.
         * @return True if iterators are not equal, false otherwise.
         */
        bool operator!=(const Iterator& src) const {
            return current != src.current;
        }

        /**
         * @brief Prefix increment operator for Iterator.
         * @return Reference to the incremented Iterator.
         */
        Iterator& operator++() {
            if (current == nullptr) {
                throw std::runtime_error("Iterator is null");
            }
            current = current->next;
            return *this;
        }

        /**
         * @brief Postfix increment operator for Iterator.
         * @param int Number of positions to increment.
         * @return Iterator before increment.
         */
        Iterator operator++(int) {
            if (current == nullptr) {
                throw std::runtime_error("Iterator is null");
            }
            Iterator temp = *this;
            current = current->next;
            return temp;
        }

        /**
         * @brief Addition operator for Iterator.
         * @param interval Number of positions to move forward.
         * @return Iterator after moving forward.
         */
        Iterator operator+(int interval) {
            if (current == nullptr) {
                throw std::runtime_error("Iterator is null");
            }
            Iterator temp = *this;
            while (interval > 0 && temp.current != nullptr) {
                temp++;
                interval--;
            }
            return temp;
        }

        /**
         * @brief Get the key on which the iterator is pointing.
         * @return Reference to the key.
         */
        Key& key() const {
            if (current == nullptr) {
                throw std::runtime_error("Iterator is null");
            }
            return current->key;
        }

        /**
         * @brief Get the info on which the iterator is pointing.
         * @return Reference to the info.
         */
        Info& info() const {
            if (current == nullptr) {
                throw std::runtime_error("Iterator is null");
            }
            return current->info;
        }
    };

    /**
     *@brief Get the length of the sequence
     *
     * @return (int) length of the sequence
    */
    [[nodiscard]] int getLength() const{
        return length;
    };

    /**
     * Checks if the sequence is empty
     *
     * @return true if the sequence is empty
     * @return false if the sequence is not empty
    */
    [[nodiscard]] bool isEmpty() const{
        return getLength() == 0;
    };

    /**
     * @brief Inserts a new element with the provided key and info after the specified target element
     * of a given key and occurrence.
     *
     * @param key The key of the new element to insert.
     * @param info The info of the new element to insert.
     * @param target_key The key after which the new element should be inserted.
     * @param occurrence Specifies after which occurrence of `target_key` to insert.
     * @return true if the element was successfully inserted, false otherwise.
     */
    bool insertAfter(const Key &key, const Info &info, const Key &target_key, unsigned int occurrence = 1) {
        Node *target = getNode(target_key, occurrence);

        if (target == nullptr) {
            return false; // Target is not found.
        }

        Node *newNode = new Node(key, info, target->next);
        target->next = newNode;
        if (target == tail) {
            tail = newNode; // Update the tail if the target is the last node.
        }
        length++;
        return true;
    }

    /**
     * @brief Inserts a new element with the provided key and info before the specified target element
     * of a given key and occurrence.
     *
     * @param key The key of the new element to insert.
     * @param info The info of the new element to insert.
     * @param target_key The key before which the new element should be inserted.
     * @param occurrence Specifies before which occurrence of `target_key` to insert.
     * @return true if the element was successfully inserted, false otherwise.
     */
    bool insertBefore(const Key &key, const Info &info, const Key &target_key, unsigned int occurrence = 1) {
        if (!exists(target_key, occurrence)) {
            return false;
        }
        Node *before = getNodeBefore(target_key, occurrence);

        if (before == nullptr) {
            // Required element is the first in the sequence.
            pushFront(key, info);
        } else {
            Node *newNode = new Node(key, info, before->next);
            before->next = newNode;
            length++;
        }

        return true;
    }

    /**
     * @brief Adds an element to the beginning of the sequence.
     *
     * @param key The key to be inserted.
     * @param info The info to be inserted.
     */
    void pushFront(const Key &key, const Info &info) {
        Node *newNode = new Node(key, info, head);
        head = newNode;
        if (tail == nullptr) {
            tail = newNode;
        }
        length++;
    }

    /**
     * @brief adds an element to the end of the sequence
     *
     * @param key info to be inserted
     * @param info info to be inserted
    */
    void pushBack(const Key &key, const Info &info){
        Node *newNode = new Node(key, info, nullptr);
        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        length++;
    };

    /**
     * @brief Removes the specified element of a given key and occurrence.
     *
     * @param key The key of the element to remove.
     * @param occurrence Specifies which occurrence of the key to consider. Defaults to 1.
     * @return true if an element was successfully removed, false otherwise.
     */
    bool remove(const Key &key, unsigned int occurrence = 1) {
        if (!exists(key, occurrence)) {
            return false;
        }
        Node *before = getNodeBefore(key, occurrence);

        if (before == nullptr) {
            popFront();
        } else {
            Node *target = before->next;
            before->next = target->next;
            if (target == tail) {
                tail = before;
            }
            delete target;
            length--;
        }
        return true;
    }

    /**
    * @brief removes first element in sequence
    *
    * @return true if element removed successfully
    * @return false if element is not removed
    */
    bool popFront() {
        if(isEmpty()){
            return false; // sequence is empty
        }

        Node *temp = head;
        head = head->next;

        if(head == nullptr){
            // if the sequence becomes empty after removal
            tail = nullptr;
        }

        delete temp;
        length--;
        return true; // successfully popped the first element
    }

    /**
     * @brief removed last element in sequence
     *
     * @return true if element removed successfully
     * @return false if element is not removed
     */
    bool popBack(){
        if(isEmpty()){
            return false; // sequence is empty
        }

        if(head == tail){
            // sequence has only one element
            delete tail;
            head = nullptr;
            tail = nullptr;
        } else{
            //traverse the list to find the second-to-last node
            Node *previous = nullptr;
            Node *current = head;
            while(current->next != nullptr){
                previous = current;
                current = current->next;
            }

            // Remove the tail
            delete tail;
            tail = previous;
            tail->next = nullptr;
        }

        length--;
        return true;
     };

    /**
     * Clears the sequence
    */
     void clear() {
         while (head) {
             Node* temp = head;
             head = head->next;
             delete temp;
         }
         tail = nullptr;
         length = 0;
     };

    /**
    *
    * Retrieves the info associated to a specified element of a given key and occurrence.
    *
    * @param[out] info Reference to where the info will be stored.
    * @param key The key for which to retrieve the associated info.
    * @param occurrence Specifies which occurrence of the key to consider. Defaults to 1.
    * @return true if the element was found and the associated info was retrieved, false otherwise.
    */
    bool getInfo(Info &info, const Key &key, unsigned int occurrence = 1) const{
        Node *current = head;
        unsigned int count = 0;

        while (current != nullptr) {
            if (current->key == key) {
                count++;
                if (count == occurrence) {
                    // Found the specified element with the given key and occurrence.
                    info = current->info;
                    return true;
                }
            }
            current = current->next;
        }

        // Element with the specified key and occurrence is not found.
        return false;
    };

    /**
     * Searches for the specified element of a given key and occurrence.
     *
     * @param key The key to search for.
     * @param occurrence Specifies which occurrence of the key to consider. Defaults to 1.
     * @return true if the specified element is found, false otherwise.
     */
     bool exists(const Key &key, unsigned int occurrence = 1) const{
         return occurrencesOf(key) != 0;
     };

     /**
      * Counts the number of occurrences of the given key
      *
      * @param key is the key which occurrences this function counts
     */
     unsigned  int occurrencesOf(const Key &key) const{
         Node *current = head;
         unsigned int count = 0;

         while(current != nullptr){
             if(current->key == key){
                 // found the occurrence of the specified key
                 count++;
             }
             current = current->next;
         }

         // return the total number for occurrences of the specified key
         return count;
     };

    /**
     * Search for a specific key in the sequence and return its position
     * @param key The key to search for.
     * @param occurrence Specifies which occurrence of the key to consider, defaults to 1.
     * @param [out] it is iterator pointing on found element
     * @return true if the specified element is found, false otherwise.
    */
    bool search(Iterator &iterator, const Key &key, unsigned int occurrence = 1) {
        Node *searchNode = getNode(key, occurrence);
        if (searchNode) {
            iterator = Iterator(searchNode);
            return true;
        }
        return false; // not found
    }

    /**
     * Searches for the specified element before element with given key and occurrence
     *
     * @param key The key to search for.
     * @param occurrence Specifies which occurrence of the key to consider, defaults to 1.
     * @param [out] it is iterator pointing on found element.
     * @return true if the specified element is found, false otherwise.
    */
    bool searchBefore(Iterator &iterator, const Key &key, unsigned int occurrence = 1) {
        Node *beforeNode = getNodeBefore(key, occurrence);
        if (beforeNode) {
            iterator = Iterator(beforeNode);
            return true;
        }
        return false;
    }

    /**
     * Returns first or last element of sequence.
     *
     * @param [out] it is an iterator pointing on found element.
     * @return true if the specified element is found, false otherwise.
    */
    Iterator begin() const{
        return Iterator(head);
    };
    Iterator end() const{
        return Iterator(tail);
    };

    /**
     *
     * @return Iterator pointing null
    */
    Iterator empty() const{
        return Iterator(nullptr);
    }
};

template <typename Key, typename Info>
ostream &operator<<(ostream &os, const Sequence<Key, Info> &sequence){
    os << "[";
    for(auto iterator = sequence.begin(); iterator != sequence.empty(); iterator++){
        os << "(" << iterator.key() << ", " << iterator.info() << ")";
        if (iterator != sequence.end()){
            os << ", ";
        }
    }
    os << "]";

    return os;
}

#endif