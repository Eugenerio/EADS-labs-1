#include "sequence.hpp"
#include "catch2/catch_test_macros.hpp"
#include <iostream>
#include <sstream>

typedef Sequence<int, int> TestSeq;

TEST_CASE("empty sequence") {
    TestSeq empty;
    CHECK(empty.getLength() == 0);
    CHECK(empty.isEmpty());
}

TEST_CASE("push and pop stuff to a sequence") {
    TestSeq seq;
    seq.pushFront(1, 1);
    seq.pushFront(2, 2);
    seq.pushFront(3, 3);
    CHECK(seq.getLength() == 3);
    seq.popFront();
    seq.popFront();
    seq.popFront();
    CHECK(seq.getLength() == 0);

    seq.pushBack(4, 4);
    seq.pushBack(5, 5);
    seq.pushBack(6, 6);
    CHECK(seq.getLength() == 3);
    seq.popBack();
    seq.popBack();
    CHECK(seq.getLength() == 1);

    seq.clear();
    CHECK(seq.getLength() == 0);
    CHECK(seq.isEmpty());
}

TEST_CASE("Copy constructor testing"){
    TestSeq seq;
    seq.pushBack(1, 1);
    seq.pushBack(2, 2);
    seq.pushBack(3, 3);

    TestSeq seq1 = seq;
    CHECK(seq1.getLength() == seq.getLength());
    CHECK(seq1.exists(1));
    CHECK(seq1.exists(2));
    CHECK(seq1.exists(3));

    seq.remove(2);
    CHECK(seq.getLength() == 2);
    CHECK(seq.exists(2) == false);

    CHECK(seq1.getLength() == 3);
    CHECK(seq1.exists(2));

    TestSeq seq2;
    seq2.pushBack(4, 4);
    seq2.pushBack(5, 5);
    seq2 = seq;
    CHECK(seq2.getLength() == seq.getLength());
    CHECK(seq2.exists(1));
    CHECK(seq2.exists(3));

    seq.remove(1);
    CHECK(seq.getLength() == 1);
    CHECK(seq.exists(1) == false);

    CHECK(seq2.getLength() == 2);
    CHECK(seq2.exists(1));
}

TEST_CASE("Iterator"){
    TestSeq seq;
    seq.pushBack(1, 1);
    seq.pushBack(2, 2);
    seq.pushBack(3, 3);
    seq.pushBack(4, 4);

    TestSeq::Iterator it = seq.begin();
    TestSeq::Iterator endIterator = seq.end();

    CHECK(it == seq.begin());
    CHECK(it.key() == 1);
    CHECK(it.info() == 1);
    CHECK(endIterator == seq.end());
    CHECK(it != endIterator);
    CHECK(endIterator.info() == 4);
    CHECK(endIterator.key() == 4);

    // copy constructor
    TestSeq::Iterator copyIt = TestSeq::Iterator(it);
    CHECK(copyIt.key() == it.key());
    CHECK(copyIt == it);

    //prefix operator++
    ++it;
    CHECK(it.key() == 2);
    CHECK(it.info() == 2);

    //postfix operator++
    it++;
    CHECK(it.key() == 3);
    CHECK(it.info() == 3);

    //addition operator+
    it = it + 1;
    CHECK(it.key() == 4);
    CHECK(it.info() == 4);

    CHECK(it == endIterator);

    //addition operator+ with an interval
    copyIt = copyIt + 4;
    CHECK(copyIt == seq.empty());

    //ending iterator
    it = it + 1;
    CHECK(it == seq.empty());
}

TEST_CASE("Search Element"){
    TestSeq seq;
    seq.pushBack(1, 1);
    seq.pushBack(2, 2);
    seq.pushBack(1, 11);
    seq.pushBack(3, 3);

    //search
    TestSeq::Iterator it;
    CHECK(seq.search(it, 1)); // find the first occurrence of the key 1
    CHECK(it.key() == 1);
    CHECK(it.info() == 1);

    CHECK(seq.search(it, 1, 2)); // find the second occurrence of the key 1
    CHECK(it.key() == 1);
    CHECK(it.info() == 11);

    CHECK(!seq.search(it, 4));

    //find before
    TestSeq::Iterator beforeIt;
    CHECK(seq.searchBefore(beforeIt, 2)); // search for the node before key 2
    CHECK(beforeIt.key()  == 1);
    CHECK(beforeIt.info()  == 1);

    CHECK(!seq.searchBefore(beforeIt, 1)); // No node before the first occurrence of key 1
    CHECK(!seq.searchBefore(beforeIt, 4)); // key 4 does not exist

    //occurrencesOf method
    CHECK(seq.occurrencesOf(1) == 2); //key 1 occurs twice
    CHECK(seq.occurrencesOf(2) == 1); //key 2 occurs once
    CHECK(seq.occurrencesOf(4) == 0); //key 4 does not exist

    //exists method
    CHECK(seq.exists(1)); //key 1 exists
    CHECK(seq.exists(2)); //key 2 exists
    CHECK(!seq.exists(4)); //key 4 does not exist
}

TEST_CASE("Remove"){
    TestSeq seq;
    seq.pushBack(1, 1);
    seq.pushBack(2, 2);
    seq.pushBack(3, 3);

    //remove: element is the first one
    CHECK(seq.remove(1) == true);
    CHECK(seq.getLength() == 2);
    CHECK(seq.begin().key() == 2);
    CHECK(seq.begin().info() == 2);
    CHECK(seq.end().key() == 3);
    CHECK(seq.end().info() == 3);
    seq.pushFront(1, 1);

    //remove: element is in the middle
    CHECK(seq.remove(2) == true);
    CHECK(seq.getLength() == 2);
    CHECK(seq.begin().key() == 1);
    CHECK(seq.begin().info() == 1);
    CHECK(seq.end().key() == 3);
    CHECK(seq.end().info() == 3);

    //remove: element is the last
    CHECK(seq.remove(3) == true);
    CHECK(seq.getLength() == 1);
    CHECK(seq.begin().key() == 1);
    CHECK(seq.begin().info() == 1);
    CHECK(seq.end().key() == 1);
    CHECK(seq.end().info() == 1);

    //remove: sequence has only one element
    CHECK(seq.remove(1) == true);
    CHECK(seq.isEmpty() == true);

    //remove: empty sequence
    CHECK(seq.remove(1) == false);

    seq.pushBack(1, 1);
    seq.pushBack(1, 11);
    seq.pushBack(3, 3);

    CHECK(seq.remove(1, 2) == true); //remove the element with occurrence 2
    CHECK(seq.getLength() == 2);
    CHECK(seq.begin().key() == 1);
    CHECK(seq.begin().key() == 1);
    CHECK(seq.end().key() == 3);
    CHECK(seq.end().key() == 3);
}

TEST_CASE("InsertAfter"){
    TestSeq seq;
    seq.pushBack(1, 1);
    seq.pushBack(3, 3);

    TestSeq::Iterator it;

    // insert after: element is the first one
    CHECK(seq.insertAfter(2, 2, 1) == true);
    CHECK(seq.getLength() == 3);
    it = seq.begin();
    CHECK(it.key() == 1);
    CHECK(it.info() == 1);
    it++;
    CHECK(it.key() == 2);
    CHECK(it.info() == 2);
    CHECK(seq.end().key() == 3);
    CHECK(seq.end().info() == 3);

    // insert after: element is in the middle
    CHECK(seq.insertAfter(4, 4, 2) == true);
    CHECK(seq.getLength() == 4);
    CHECK(seq.begin().key() == 1);
    CHECK(seq.begin().info() == 1);
    CHECK(it.key() == 2);
    CHECK(it.info() == 2);
    it++;
    CHECK(it.key() == 4);
    CHECK(it.info() == 4);
    CHECK(seq.end().key() == 3);
    CHECK(seq.end().info() == 3);

    // insert after: element is the last one
    CHECK(seq.insertAfter(5, 5, 3) == true);
    CHECK(seq.getLength() == 5);
    CHECK(seq.begin().key() == 1);
    CHECK(seq.begin().info() == 1);
    CHECK(it.key() == 4);
    CHECK(it.info() == 4);
    it++;
    CHECK(it.key() == 3);
    CHECK(it.info() == 3);
    CHECK(seq.end().key() == 5);
    CHECK(seq.end().info() == 5);

    // insert after: sequence has only one element
    TestSeq singleSeq;
    singleSeq.pushBack(1, 1);
    CHECK(singleSeq.insertAfter(2, 2, 1) == true);
    CHECK(singleSeq.getLength() == 2);
    CHECK(singleSeq.begin().key() == 1);
    CHECK(singleSeq.begin().info() == 1);
    CHECK(singleSeq.end().key() == 2);
    CHECK(singleSeq.end().info() == 2);

    // insert after: empty sequence
    TestSeq emptySeq;
    CHECK(emptySeq.insertAfter(1, 1, 1) == false);
}

TEST_CASE("InsertBefore"){
    TestSeq seq;
    seq.pushBack(1, 1);
    seq.pushBack(3, 3);

    TestSeq::Iterator it;

    // insert before: element is the first one
    CHECK(seq.insertBefore(2, 2, 1) == true);
    CHECK(seq.getLength() == 3);
    it = seq.begin();
    CHECK(it.key() == 2);
    CHECK(it.info() == 2);
    it++;
    CHECK(it.key() == 1);
    CHECK(it.info() == 1);
    CHECK(seq.end().key() == 3);
    CHECK(seq.end().info() == 3);

    // insert before: element is in the middle
    CHECK(seq.insertBefore(4, 4, 1) == true);
    CHECK(seq.getLength() == 4);
    it = seq.begin();
    CHECK(it.key() == 2);
    CHECK(it.info() == 2);
    it++;
    CHECK(it.key() == 4);
    CHECK(it.info() == 4);
    it++;
    CHECK(it.key() == 1);
    CHECK(it.info() == 1);
    CHECK(seq.end().key() == 3);
    CHECK(seq.end().info() == 3);

    // insert before: element is the last one
    CHECK(seq.insertBefore(5, 5, 3) == true);
    CHECK(seq.getLength() == 5);
    it = seq.begin();
    CHECK(it.key() == 2);
    CHECK(it.info() == 2);
    it++;
    CHECK(it.key() == 4);
    CHECK(it.info() == 4);
    it++;
    CHECK(it.key() == 1);
    CHECK(it.info() == 1);
    it++;
    CHECK(it.key() == 5);
    CHECK(it.info() == 5);
    CHECK(seq.end().key() == 3);
    CHECK(seq.end().info() == 3);

    // insert before: sequence has only one element
    TestSeq singleSeq;
    singleSeq.pushBack(1, 1);
    CHECK(singleSeq.insertBefore(2, 2, 1) == true);
    CHECK(singleSeq.getLength() == 2);
    it = singleSeq.begin();
    CHECK(it.key() == 2);
    CHECK(it.info() == 2);
    it++;
    CHECK(it.key() == 1);
    CHECK(it.info() == 1);

    // insert before: empty sequence
    TestSeq emptySeq;
    CHECK(emptySeq.insertBefore(1, 1, 1) == false);
}

TEST_CASE("Print"){
    TestSeq seq;
    seq.pushBack(1, 1);
    seq.pushBack(2, 2);
    seq.pushBack(3, 3);

    ostringstream output;
    output << seq;

    string expectedOutput = "[(1, 1), (2, 2), (3, 3)]";
    CHECK(output.str() == expectedOutput);
}

//TEST_CASE("Split Position #1"){
//    TestSeq seq;
//    for(int i = 0; i < 25; ++i){
//        seq.pushBack(i, i);
//    }
//
//    TestSeq seq1, seq2;
//
//    split_pos(seq, 2, 2, 3, 4, seq1, seq2);
//
//    TestSeq::Iterator it = seq.begin();
//    int expectedSeq[] = {0, 1, 22, 23, 24};
//    for (int i : expectedSeq){
//        CHECK(it.key() == i);
//        ++it;
//    }
//
//    TestSeq::Iterator it1 = seq1.begin();
//    int expectedSeq1[] = {2, 3, 7, 8, 12, 13, 17, 18};
//    for (int i : expectedSeq1)
//    {
//        CHECK(it1.key() == i);
//        ++it1;
//    }
//
//    TestSeq::Iterator it2 = seq2.begin();
//    int expectedSeq2[] = {4, 5, 6, 9, 10, 11, 14, 15, 16, 19, 20, 21};
//    for (int i : expectedSeq2)
//    {
//        CHECK(it2.key() == i);
//        ++it2;
//    }
//}