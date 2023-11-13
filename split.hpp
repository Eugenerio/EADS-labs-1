#ifndef SPLIT_HPP
#define SPLIT_HPP

#include "sequence.hpp"

template <typename Key, typename Info>
void split_pos(/*const*/ Sequence<Key, Info> &seq, int start_pos, int len1, int len2, int count, Sequence<Key, Info> &seq1, Sequence<Key, Info> &seq2)
{
    typename Sequence<Key, Info>::Iterator it = seq.begin();
    Sequence<Key, Info> new_sequence;

    int current_pos = 0;

    while (current_pos < start_pos && it != seq.end())
    {
        new_sequence.pushBack(it.key(), it.info());
        ++it;
        ++current_pos;
    }

    // Copy elements to seq2

    for (int c = 0; c < count; c++)
    {
        int copied_count = 0;
        while (copied_count < len1 && it != seq.empty())
        {
            seq1.pushBack(it.key(), it.info());
            it++;
            copied_count++;
        }
        copied_count = 0;
        while (copied_count < len2 && it != seq.empty())
        {
            seq2.pushBack(it.key(), it.info());
            it++;
            copied_count++;
        }
    }

    while (it != seq.empty())
    {
        new_sequence.pushBack(it.key(), it.info());
        ++it;
    }

    seq = new_sequence;
}

template <typename Key, typename Info>
void split_key(/*const*/ Sequence<Key, Info> &seq, const Key &start_key, int start_occ, int len1, int len2, int count, Sequence<Key, Info> &seq1, Sequence<Key, Info> &seq2)
{
    typename Sequence<Key, Info>::Iterator target = seq.begin();

    if (!seq.search(target, start_key, start_occ))
    {
        throw std::runtime_error("Target not found");
    }

    Sequence<Key, Info> new_sequence;
    for (auto it = seq.begin(); it != target; it++)
    {
        new_sequence.pushBack(it.key(), it.info());
    }
    auto it = target;
    for (int c = 0; c < count; c++)
    {

        int copied_count = 0;
        while (copied_count < len1 && it != seq.empty())
        {
            seq1.pushBack(it.key(), it.info());
            it++;
            copied_count++;
        }

        copied_count = 0;
        while (copied_count < len2 && it != seq.empty())
        {
            seq2.pushBack(it.key(), it.info());
            it++;
            copied_count++;
        }
    }

    while (it != seq.empty())
    {
        new_sequence.pushBack(it.key(), it.info());
        ++it;
    }

    seq = new_sequence;
}

#endif