#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include <iostream>
using namespace std;


template <typename Key, typename Info>
class Sequence{
private:
    class Node{
    private:
        Node* next;

    public:
        Key key;
        Info info;

        Node(const Key &_key, const Info &_info, Node *_next = nullptr)
            : key(_key), info(_info), next(_next){}

        friend class Sequence;
    };

    Node *head = nullptr;
    Node *tail = nullptr;
    unsigned int length = 0;

    // methods
    Node *getNode(const Key &key, unsigned int occurrence = 1){
        Node *current = head;
        unsigned int count = 0;

        while(current != nullptr){
            if(current->key == key){
                count++;
                if(count == occurrence){
                    // found node with the specified key and occurrence
                    return current;
                }
            }
            current = current->next;
        }
        // node with the specified key and occurrence is not found
        return nullptr;
    };
    Node *getNodeBefore(const Key &key, unsigned int occurrence = 1){
        Node *prev = nullptr;
        Node *curr = head;
        unsigned int count = 0;

        while(curr != nullptr){
            if(curr->key == key){
                count++;
                if(count == occurrence){
                    // found the node with the specified key and occurrence
                    // return the node before this node (prev)
                    return prev;
                }
            }
            prev = curr;
            curr = curr->next;
        }

        return nullptr;
    };

public:
    Sequence() {};
    ~Sequence(){
        clear();
    };
    Sequence(const Sequence &src) {
        *this = src;
    };
    Sequence &operator=(const Sequence &src){
        if(this != &src){
            clear();
            auto iterator = src.begin();

            // copy elements from the src using the iterator
            for(int i = 0; i < src.getLength(); i++){
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
        ~Iterator(){};
        Iterator(const Iterator &src)
        {
            this->current = src.current;
        };
        Iterator &operator=(const Iterator &src)
        {
            if (this != &src)
            {
                current = src.current;
            }
            return *this;
        };

        bool operator==(const Iterator &src) const
        {
            return current == src.current;
        };
        bool operator!=(const Iterator &src) const
        {
            return current != src.current;
        };

        Iterator &operator++() {
            if (current == nullptr)
            {
                throw std::runtime_error("Iterator is null");
            }
            if (current != nullptr) {
                current = current->next;
            }
            return *this;
        }

        Iterator operator++(int) {
            if (current == nullptr)
            {
                throw std::runtime_error("Iterator is null");
            }
            Iterator temp = *this;
            if (current != nullptr) {
                current = current->next;
            }
            return temp;
        }

        Iterator operator+(int interval) {
            if (current == nullptr)
            {
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
         *
         * @return Key& on which iterator is pointing
        */
        Key &key() const {
            if (current == nullptr)
            {
                throw std::runtime_error("Iterator is null");
            }
            return current->key;
        }

        /**
         *
         * @return Info& on which iterator is pointing
        */
        Info &info() const {
            if (current == nullptr)
            {
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
    int getLength() const{
        return length;
    };

    /**
     * Checks if the sequence is empty
     *
     * @return true if the sequence is empty
     * @return false if the sequence is not empty
    */
    bool isEmpty() const{
        return getLength() == 0;
    };

    /**
     * Inserts a new element with the provided key and info after the specified target element
     * of a given key and occurrence.
     *
     *
     * @param key The key of the new element to insert.
     * @param info The info of the new element to insert.
     * @param target_key The key after which the new element should be inserted.
     * @param occurrence Specifies after which occurrence of `target_key` to insert.
     * @return true if the element was successfully inserted, false otherwise.
     */
    bool insertAfter(const Key &key, const Info &info, const Key &target_key, unsigned int occurrence = 1){
        Node *target = getNode(target_key, occurrence);

        if(target == nullptr){
            return false; // target is not found
        }

        Node *newNode = new Node(key, info, target->next);
        target->next = newNode;
        if(target == tail){
            tail = newNode; // update the tail if target is the last node
        }
        length++;
        return true;
    };

    bool insertBefore(const Key &key, const Info &info, const Key &target_key, unsigned int occurrence = 1){
        if(!exists(target_key, occurrence)){
            return false;
        }
        Node *before = getNodeBefore(target_key, occurrence);

        if(before == nullptr){
            // required element is the first in the sequence
            pushFront(key, info);
        } else{
            Node *newNode = new Node(key, info, before->next);
            before->next = newNode;
            length++;
        }

        return true;
    };

    /**
     * @brief adds element to the beginning of the sequence
     *
     * @param key key to be inserted
     * @param info info to be inserted
     */
    void pushFront(const Key &key, const Info &info){
        Node* newNode = new Node(key, info, head);
        head = newNode;
        if(tail == nullptr){
            tail = newNode;
        }
        length++;
    };

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
     * Removes the specified element of a given key and occurrence.
     *
     * @param key The key of the element to remove.
     * @param occurrence Specifies which occurrence of the key to consider. Defaults to 1.
     * @return true if an element was successfully removed, false otherwise.
     */
     bool remove(const Key &key, unsigned int occurrence = 1){
         if(!exists(key, occurrence)){
             return false;
         }
         Node *before = getNodeBefore(key, occurrence);

         if(before == nullptr){
             popFront();
         } else{
             Node *target = before->next;
             before->next = target->next;
             if(target == tail){
                 tail = before;
             }
             delete target;
             length--;
         }
         return true;
     };

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
    bool getInfo(Info &info, const Key &key, unsigned int occurrence = 1) const;

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
    int search(Iterator &iterator, const Key &key, unsigned int occurrence = 1){
        Node *searchNode = getNode(key, occurrence);

        if(searchNode != nullptr){
            // update the provider iterator to point to the found element
            iterator = Iterator(searchNode);
            return true;
        }

        return false; // not found
    };

    /**
     * Searches for the specified element before element with given key and occurrence
     *
     * @param key The key to search for.
     * @param occurrence Specifies which occurrence of the key to consider, defaults to 1.
     * @param [out] it is iterator pointing on found element.
     * @return true if the specified element is found, false otherwise.
    */
    bool searchBefore(Iterator &iterator, const Key &key, unsigned int occurrence = 1){
        Node *beforeNode = getNodeBefore(key, occurrence);

        if(beforeNode != nullptr){
            iterator = Iterator(beforeNode);
            return true;
        }

        return false;
    };

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

template <typename Key, typename Info>
void split_pos(const Sequence<Key, Info> &seq, int start_pos, int len1, int len2, int count, Sequence<Key, Info> &seq1, Sequence<Key, Info> &seq2);

template <typename Key, typename Info>
void split_key(const Sequence<Key, Info> &seq, const Key &start_key, int start_occ, int len1, int len2, int count, Sequence<Key, Info> &seq1, Sequence<Key, Info> &seq2);

//TODO:
// sort
// reverse
// swap
// subSequence

#endif