#pragma once

#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {
    // ���� ������
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    // ������ ������ �������� ��������.
    // ���������� ��������� ��������� �� �������� ������������ ������
    // ValueType � ��������� � Type (��� Iterator) ���� � const Type (��� ConstIterator)
    template <typename ValueType>
    class BasicIterator {
        // ����� ������ ����������� �������������, ����� �� ������� ������
        // ��� ������ � ��������� ������� ���������
        friend class SingleLinkedList;

        // �������������� ����������� ��������� �� ��������� �� ���� ������
        explicit BasicIterator(Node* node)
            : node_(node) {}

    public:
        // ����������� ���� ���� �������� ����������� ���������� � ��������� ����� ���������

        // ��������� ��������� � forward iterator
        // (��������, ������� ������������ �������� ���������� � ������������ �������������)
        using iterator_category = std::forward_iterator_tag;
        // ��� ���������, �� ������� ������������ ��������
        using value_type = Type;
        // ���, ������������ ��� �������� �������� ����� �����������
        using difference_type = std::ptrdiff_t;
        // ��� ��������� �� ����������� ��������
        using pointer = ValueType*;
        // ��� ������ �� ����������� ��������
        using reference = ValueType&;

        BasicIterator() = default;

        // �������������� �����������/����������� �����������
        // ��� ValueType, ����������� � Type, ������ ���� ����������� ������������
        // ��� ValueType, ����������� � const Type, ������ ���� ��������������� ������������
        BasicIterator(const BasicIterator<Type>& other) noexcept
            : node_(other.node_) {}

        // ����� ���������� �� ������� �������������� �� ���������� ��������� = ��� �������
        // ����������������� ������������ �����������, ���� ������� �������� = �
        // �������� ���������� ������������� ��� �� ���
        BasicIterator& operator=(const BasicIterator& rhs) = default;

        // �������� ��������� ���������� (� ���� ������� ��������� ��������� ����������� ��������)
        // ��� ��������� �����, ���� ��� ��������� �� ���� � ��� �� ������� ������ ���� �� end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return this->node_ == rhs.node_;
        }

        // �������� �������� ���������� �� �����������
        // �������������� !=
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return this->node_ != rhs.node_;
        }

        // �������� ��������� ���������� (� ���� ������� ��������� ��������)
        // ��� ��������� �����, ���� ��� ��������� �� ���� � ��� �� ������� ������ ���� �� end()
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return this->node_ == rhs.node_;
        }

        // �������� �������� ���������� �� �����������
        // �������������� !=
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return this->node_ != rhs.node_;
        }

        // �������� ��������������. ����� ��� ������ �������� ��������� �� ��������� ������� ������
        // ���������� ������ �� ������ ����
        // ��������� ���������, �� ������������ �� ������������ ������� ������, �������� � �������������� ���������
        BasicIterator& operator++() noexcept {
            assert(node_ != nullptr);

            node_ = node_->next_node;
            return *this;
        }

        // �������� ��������������. ����� ��� ������ �������� ��������� �� ��������� ������� ������
        // ���������� ������� �������� ���������
        // ��������� ���������, �� ������������ �� ������������ ������� ������,
        // �������� � �������������� ���������
        BasicIterator operator++(int) noexcept {
            auto old_node(*this);
            ++(*this); // ����� BasicIterator& operator++()
            return old_node;
        }

        // �������� �������������. ���������� ������ �� ������� �������
        // ����� ����� ��������� � ���������, �� ������������ �� ������������ ������� ������,
        // �������� � �������������� ���������
        [[nodiscard]] reference operator*() const noexcept {
            assert(node_ != nullptr);
            return node_->value;
        }

        // �������� ������� � ����� ������. ���������� ��������� �� ������� ������� ������
        // ����� ����� ��������� � ���������, �� ������������ �� ������������ ������� ������,
        // �������� � �������������� ���������
        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_ != nullptr);
            return &(node_->value);
        }

    private:
        Node* node_ = nullptr;
    };

public:
    SingleLinkedList()
        : head_(Node())
        , size_(0u) {
    }

    // ����������� ����������� ������
    SingleLinkedList(const SingleLinkedList& other)
        : head_(Node())
        , size_(0u) {
        SingleLinkedList temp_list;
        try {
            temp_list.InitializeListFromRange(other.begin(), other.end());
        }
        catch (...) {
            temp_list.Clear();
            throw;
        }
        swap(temp_list);
    }

    // ������������� ������ ������� ��������� ��� ������ std::initializer_list �� ����� O(N)
    SingleLinkedList(std::initializer_list<Type> values)
        : head_(Node())
        , size_(0u) {
        InitializeListFromRange(values.begin(), values.end());
    }

    // �������� ������������
    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (*this == rhs) {
            return *this;
        }

        SingleLinkedList temp_list(rhs);
        swap(temp_list);
        return *this;
    }

    // �������� ��������� ������ �� ���� ����� ����������� ������
    ~SingleLinkedList() {
        Clear();
    }

    // ���������� ���������� ��������� � ������ �� ����� O(1)
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    // ��������, ������ �� ������ �� ����� O(1)
    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    // ��������� ������� value � ������ ������ �� ����� O(1)
    void PushFront(const Type& value) {
        Node* new_node = new Node(value, head_.next_node);
        head_.next_node = new_node;
        ++size_;
    }

    // ������� ������ �� ����� O(N)
    void Clear() noexcept {
        while (head_.next_node != nullptr) {
            Node* ptr_to_delete = head_.next_node;
            head_.next_node = ptr_to_delete->next_node;
            delete ptr_to_delete;
        }
        size_ = 0u;
    }

    // ���������� ���������� ������� �� ����� O(1)
    void swap(SingleLinkedList& other) noexcept {
        std::swap(other.head_.next_node, this->head_.next_node);
        std::swap(other.size_, this->size_);
    }

    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    // ��������, ����������� ��������� ��������� ������
    using Iterator = BasicIterator<Type>;
    // ����������� ��������, ��������������� ������ ��� ������ � ��������� ������
    using ConstIterator = BasicIterator<const Type>;

    // ���������� ��������, ����������� �� ������ �������
    // ���� ������ ������, ������������ �������� ����� ����� end()
    [[nodiscard]] Iterator begin() noexcept {
        return Iterator{ head_.next_node };
    }

    // ���������� ��������, ����������� �� �������, ��������� �� ��������� ��������� ������������ ������
    // �������������� ���� �������� ������ � ������� ������������� ������� � �������������� ���������
    [[nodiscard]] Iterator end() noexcept {
        return Iterator{ nullptr };
    }

    // ���������� ����������� ��������, ����������� �� ������ �������
    // ���� ������ ������, ������������ �������� ����� ����� end()
    // ��������� ������ ������������ ������ ������ cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator{ head_.next_node };
    }

    // ���������� ����������� ��������, ����������� �� �������, ��������� �� ��������� ��������� ������������ ������
    // �������������� ���� �������� ������ � ������� ������������� ������� � �������������� ���������
    // ��������� ������ ������������ ������ ������ cend()
    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator{ nullptr };
    }

    // ���������� ����������� ��������, ����������� �� ������ �������
    // ���� ������ ������, ������������ �������� ����� ����� cend()
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{ head_.next_node };
    }

    // ���������� ����������� ��������, ����������� �� �������, ��������� �� ��������� ��������� ������������ ������
    // �������������� ���� �������� ������ � ������� ������������� ������� � �������������� ���������
    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator{ nullptr };
    }

    // ���������� ��������, ����������� �� ������� ����� ������ ��������� ������������ ������.
    // �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator{ &head_ };
    }

    // ���������� ����������� ��������, ����������� �� ������� ����� ������ ��������� ������������ ������.
    // �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator{ const_cast<Node*>(&head_) };
    }

    // ������� ������ �������� ��������� ������ �� ����� O(1)
    void PopFront() noexcept {
        assert(!IsEmpty()); // ������ �� ������ ���� ������

        Node* node_to_delete = head_.next_node;
        head_.next_node = node_to_delete->next_node;
        delete node_to_delete;
        --size_;
    }

    // ��������� � ������ ����� ��������, ��������� �� pos
    // ���������� �������� �� ����������� �������
    // ����� ���������� O(1)
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_ != nullptr); // ������� ������ ���� �������

        Node* prev_ptr = pos.node_;

        Node* new_node = new Node(value, prev_ptr->next_node);
        prev_ptr->next_node = new_node;
        ++size_;

        return Iterator{ new_node };
    }

    // ������� �������, ��������� �� pos
    // ���������� �������� �� �������, ��������� �� ��������
    // ����� ���������� O(1)
    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(!IsEmpty() && pos.node_ != nullptr); // ������ �� ������ ���� ����, ������� ������ ���� �������

        Node* ptr_to_leave = pos.node_;
        Node* ptr_to_delete = ptr_to_leave->next_node;

        ptr_to_leave->next_node = ptr_to_delete->next_node;
        delete ptr_to_delete;
        --size_;

        return Iterator{ ptr_to_leave->next_node };
    }

private:
    // ��������� ����, ������������ ��� ������� "����� ������ ���������"
    Node head_;
    size_t size_;

    // ����� �������������� ������ ���������� �� ����������� ���������
    template <typename It>
    void InitializeListFromRange(It first, It last) {
        assert(size_ == 0 && head_.next_node == nullptr);

        Node* old_node_ptr = &head_;
        for (; first != last; ++first) {
            old_node_ptr->next_node = new Node(*first, nullptr);
            old_node_ptr = old_node_ptr->next_node;
            ++size_;
        }
    }
};

// ���������� ���������� ������� �� ����� O(1)
template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

// �������� ��������� SingleLinkedList
template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return lhs.GetSize() != rhs.GetSize() 
        ? false 
        : std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

// �������� ����������� SingleLinkedList
template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

// ���������� true, ���� lhs ����������������� ������ rhs
template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

// ���������� true, ���� lhs ����������������� ������ ��� ����� rhs
template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (lhs < rhs) || (lhs == rhs);
}

// ���������� true, ���� lhs ����������������� ������ rhs
template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs <= rhs);
}

// ���������� true, ���� lhs ����������������� ������ ��� ����� rhs
template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}
