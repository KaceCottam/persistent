#ifndef LIST_HPP
#define LIST_HPP
#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>

template<class T>
class List
{
 public:
  struct Node
  {
    constexpr Node() noexcept            = default;
    constexpr Node(const Node&) noexcept = default;
    constexpr Node(Node&&) noexcept      = default;

    constexpr Node(const T& data, const std::shared_ptr<Node>& next) noexcept
        : data{std::move(data)}
        , next{next} {};

    constexpr Node(T&& data, const std::shared_ptr<Node>& next) noexcept
        : data{std::move(data)}
        , next{next} {};

    constexpr T& operator*() noexcept { return data; }
    constexpr T* operator->() noexcept { return &data; }

    [[nodiscard]] constexpr static std::shared_ptr<Node> of(
        const T&                     data,
        const std::shared_ptr<Node>& next) noexcept
    {
      return std::make_shared<Node>(data, next);
    }

    T                     data{};
    std::shared_ptr<Node> next{};
  };
  class Iterator
  {
   public:
    constexpr Iterator() noexcept                = default;
    constexpr Iterator(const Iterator&) noexcept = default;
    constexpr Iterator(Iterator&&) noexcept      = default;

    constexpr explicit Iterator(std::weak_ptr<Node> node) noexcept
        : current{node} {};

    constexpr bool valid() const noexcept { return !current.expired(); }
    constexpr      operator bool() const noexcept { return this->valid(); }

    constexpr Iterator& operator++()
    {
      if (!this->valid()) throw std::out_of_range("Invalid iterator");
      current = current.lock()->next;
      return *this;
    }
    constexpr Iterator operator++() const
    {
      if (!this->valid()) throw std::out_of_range("Invalid iterator");
      return Iterator(current.lock()->next);
    }
    constexpr Iterator operator++(int)
    {
      if (!this->valid()) throw std::out_of_range("Invalid iterator");
      auto temp = *this;
      ++*this;
      return temp;
    }

    constexpr Iterator& operator+=(std::size_t idx) const
    {
      if (idx == 0) return *this;
      return (*this)++ += (idx - 1);
    }

    constexpr Iterator& operator+=(std::size_t idx)
    {
      if (idx == 0) return *this;
      return ++(*this) += (idx - 1);
    }

    constexpr Node& operator*()
    {
      if (this->valid()) return *current.lock();
      throw std::out_of_range("Invalid iterator");
    };
    constexpr std::shared_ptr<Node> operator->()
    {
      if (this->valid()) return current.lock();
      throw std::out_of_range("Invalid iterator");
    }

   private:
    std::weak_ptr<Node> current{};
  };

  constexpr List() noexcept            = default;
  constexpr List(const List&) noexcept = default;
  constexpr List(List&&) noexcept      = default;

  constexpr explicit List(const std::shared_ptr<Node>& node) noexcept
      : head{node} {};

  constexpr explicit List(std::shared_ptr<Node>&& node) noexcept
      : head{std::move(node)} {};

  constexpr List(const std::initializer_list<T> values) noexcept
  {
    for (const auto& i : values) push_back(i);
  }

  [[nodiscard]] constexpr List<T> push_front(const T& data) const noexcept
  {
    return List(Node::of(data, head));
  }

  [[maybe_unused]] constexpr List<T>& push_front(const T& data) noexcept
  {
    head = Node::of(data, head);
    return *this;
  }

  [[nodiscard]] constexpr List<T> push_back(const T& data) const noexcept
  {
    if (this->size() == 1)
      return List<T>{Node::of(head->data, Node::of(data, nullptr))};
    return List<T>{head->next}.push_back(data);
  }

  [[maybe_unused]] constexpr List<T>& push_back(const T& data) noexcept
  {
    if (this->empty()) return push_front(data);
    auto finalNode  = begin() += (this->size() - 1);
    finalNode->next = Node::of(data, nullptr);
    return *this;
  }

  [[nodiscard]] constexpr List<T> pop_front() const noexcept
  {
    return List(head->next);
  }

  [[maybe_unused]] constexpr List<T>& pop_front() noexcept
  {
    head = head->next;
    return *this;
  }

  [[nodiscard]] constexpr std::size_t size() const noexcept
  {
    std::size_t size_{0};
    auto        iter = begin();
    while (iter.valid())
      {
        ++iter;
        ++size_;
      }
    return size_;
  }

  [[nodiscard]] constexpr bool empty() const noexcept
  {
    return head == nullptr;
  }

  [[nodiscard]] constexpr Iterator begin() const noexcept
  {
    return Iterator{head};
  }

  [[nodiscard]] constexpr Iterator end() const noexcept { return Iterator{}; }

  [[nodiscard]] constexpr T& operator[](const std::size_t idx)
  {
    auto iter = begin() += idx;
    return **iter;
  }

  [[nodiscard]] constexpr T operator[](const std::size_t idx) const
  {
    auto iter = begin() += idx;
    return **iter;
  }

 private:
  std::shared_ptr<Node> head{nullptr};
};
#endif  // ! LIST_HPP

