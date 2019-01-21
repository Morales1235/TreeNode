#ifndef TREENODE_H
#define TREENODE_H

#include <type_traits>

template< typename T>
class CanCallEmpty;

template<typename T, template<class...> typename Container = std::vector>
struct TreeNode
{
    TreeNode(T&& oInitializer);
    TreeNode(const TreeNode& other);
    TreeNode& operator=(const TreeNode& other);
    TreeNode(TreeNode&& other);
    TreeNode& operator=(TreeNode&& other);
    virtual ~TreeNode() {}
    typename std::remove_reference<T>::type* operator ->();
    T& operator*();

    T value;
    Container<TreeNode<T>> children;
    bool IsLeaf() const { return children.empty(); }
};

template<typename T, template<class...> typename Container>
TreeNode<T, Container>::TreeNode(T&& oInitializer)
 : value(std::forward<T> (oInitializer))
{
    static_assert(CanCallEmpty< decltype( Container< TreeNode<T> >{})  >::value, "Container has to provde 'empty()' function.");
}

template<typename T, template<class...> typename Container>
TreeNode<T, Container>::TreeNode(const TreeNode& other)
    : value(other.value), children(other.children)
{
}

template<typename T, template<class...> typename Container>
TreeNode<T, Container>::TreeNode(TreeNode&& other)
    : value(std::forward<T>(other.value)), children(std::move(other.children))
{
}

template<typename T, template<class...> typename Container>
TreeNode<T, Container>& TreeNode<T, Container>::operator=(const TreeNode<T, Container>& other)
{
    value = (other.value);
    children =  other.children;
    return *this;
}

template<typename T, template<class...> typename Container>
typename std::remove_reference<T>::type *TreeNode<T, Container>::operator ->()
{
    return &value;
}

template<typename T, template<class...> typename Container>
T& TreeNode<T, Container>::operator*()
{
    return value;
}

template<typename T, template<class...> typename Container>
TreeNode<T, Container>& TreeNode<T, Container>::operator=(TreeNode<T, Container>&& other)
{
    value = (std::forward<T>(other.value));
    children = (std::move(other.children));
    return *this;
}


template <typename T>
class CanCallEmpty
{
    typedef char yes[1];
    typedef char no[2];

    template <typename C> static yes& test( typeof(&C::empty) ) ;
    template <typename C> static no& test(...);

public:
    enum { value = sizeof(test<T>(0)) == sizeof(yes) };
};

#endif // TREENODE_H
