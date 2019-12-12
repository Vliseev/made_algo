//
// Created by vlad on 24.11.2019.
//
#include <cstring>
#include <vector>

/*Реализуйте структуру данных типа “множество строк” на основе динамической
хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из
строчных латинских букв. Хеш-функция строки должна быть реализована с помощью
вычисления значения многочлена методом Горнера. Начальный размер таблицы должен
быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае,
когда коэффициент заполнения таблицы достигает 3/4. Структура данных должна
поддерживать операции добавления строки в множество, удаления строки из
множества и проверки принадлежности данной строки множеству. Для разрешения
коллизий используйте двойное хеширование.*/

namespace {

const size_t R1 = 117;
const uint64_t M1 = 1893692287;

template <size_t R, size_t M>
uint64_t ByteArrHash(const void *ptr, size_t len) {
    const char *buf = static_cast<const char *>(ptr);
    long long hash_value = 0;
    long long p_pow = 1;
    for (size_t i = 0; i < len; ++i) {
        hash_value = (hash_value + (buf[i] - 'a' + 1) * p_pow) % M;
        p_pow = (p_pow * R) % M;
    }

    return hash_value;
}

}    // namespace

template <size_t R, size_t M>
struct HornerHash {
    size_t operator()(const std::string &s) const {
        return ByteArrHash<R, M>(s.data(), s.size());
    }
};

template <typename T, class Hasher = std::hash<T>, class Eq = std::equal_to<T>>
class Hashtable {
    enum class NodeState { IS_USE, FREE, DELETED };

    struct Node {
        T val_{};
        NodeState state_ = NodeState::FREE;
    };

    size_t size_;
    std::vector<Node> nodes_;
    Hasher hasher_;

    void Rehash();
    size_t NextHash(size_t h1, size_t h2);

   public:
    enum class Status { OK, FAIL };

    bool Insert(const std::string &s);
    bool Erase(const std::string &s);
    bool Find(const std::string &s);
    std::pair<size_t, size_t> GetWhoHash(const std::string &s);

    Hashtable() : size_(0), nodes_(8){};
    Hashtable(const Hashtable &other) = default;
    Hashtable(Hashtable &&other) noexcept = default;
    Hashtable &operator=(const Hashtable &other) = default;
    Hashtable &operator=(Hashtable &&other) noexcept = default;

    ~Hashtable() = default;
};

template <typename T, class Hasher, class Eq>
bool Hashtable<T, Hasher, Eq>::Erase(const std::string &s) {
    for (auto [idx, h2] = GetWhoHash(s);; idx = NextHash(idx, h2)) {
        if (nodes_[idx].state_ == NodeState::FREE) {
            return false;
        } else if (nodes_[idx].state_ == NodeState::IS_USE &&
                   nodes_[idx].val_ == s) {
            nodes_[idx].state_ = NodeState::DELETED;
            return true;
        }
    }
}

template <typename T, class Hasher, class Eq>
bool Hashtable<T, Hasher, Eq>::Insert(const std::string &s) {
    if (Find(s))
        return false;

    if ((size_ + 1) * 4 > nodes_.size() * 3)
        Rehash();

    for (auto [idx, h2] = GetWhoHash(s);; idx = NextHash(idx, h2)) {
        if (nodes_[idx].state_ == NodeState::FREE ||
            nodes_[idx].state_ == NodeState::DELETED) {
            nodes_[idx].val_ = s;
            nodes_[idx].state_ = NodeState ::IS_USE;
            size_++;
            return true;
        }
    }
}
template <typename T, class Hasher, class Eq>
bool Hashtable<T, Hasher, Eq>::Find(const std::string &s) {
    for (auto [idx, h2] = GetWhoHash(s);; idx = NextHash(idx, h2)) {
        if (nodes_[idx].state_ == NodeState::FREE) {
            return false;
        } else if (nodes_[idx].state_ == NodeState::IS_USE &&
                   nodes_[idx].val_ == s) {
            return true;
        }
    }
}

template <typename T, class Hasher, class Eq>
void Hashtable<T, Hasher, Eq>::Rehash() {
    std::vector<Node> tmp_node(nodes_.size() * 2);
    std::swap(tmp_node, nodes_);

    for (auto &i : tmp_node) {
        if (i.state_ == NodeState::IS_USE)
            Insert(i.val_);
    }
}

template <typename T, class Hasher, class Eq>
std::pair<size_t, size_t>
Hashtable<T, Hasher, Eq>::GetWhoHash(const std::string &s) {
    const size_t mask = nodes_.size() - 1;
    return std::pair(hasher_(s) & mask, (2 * hasher_(s) + 1) & mask);
}

template <typename T, class Hasher, class Eq>
size_t Hashtable<T, Hasher, Eq>::NextHash(size_t h1, size_t h2) {
    const size_t mask = nodes_.size() - 1;
    return (h1 + h2) & mask;
}
