#pragma once

#include <iostream>

#include <utility>
#include <vector>

namespace linq_exceptions
{
    namespace error_messages
    {
        const std::string out_of_range = "Enumerator is out of range";
        const std::string dereference_error = "Can't get value from out of range enumerator";
    }
    class linq_exception : public std::exception
    {
    private:
        std::string message_;
    public:
        explicit linq_exception(std::string ex) : message_(std::move(ex))
        {}

        [[nodiscard]] const char *what() const noexcept override
        {
            return message_.c_str();
        }
    };
}

namespace linq
{
    namespace impl
    {
        template<class T>
        class take_enumerator;

        template<class T>
        class drop_enumerator;

        template<class T, class U, class F>
        class select_enumerator;

        template<class T, class F>
        class until_enumerator;

        template<class T, class F>
        class where_enumerator;

        template<class T>
        class enumerator
        {
        public:
            virtual enumerator &operator++() = 0;

            virtual explicit operator bool() = 0;

            virtual const T &operator*() = 0;

            auto take(int amount)
            {
                return take_enumerator<T>(*this, amount);
            }

            auto drop(int count)
            {
                return drop_enumerator<T>(*this, count);
            }

            template<typename U = T, typename F>
            auto select(F func)
            {
                return select_enumerator<U, T, F>(*this, std::move(func));
            }

            template<typename F>
            auto until(F func)
            {
                return until_enumerator<T, F>(*this, std::move(func));
            }

            auto until_eq(T value)
            {
                const auto func = [value](T a) -> bool
                {
                    return a == value;
                };
                return until_enumerator<T, decltype(func)>(*this, std::move(func));
            }

            template<typename F>
            auto where(F func)
            {
                return where_enumerator<T, F>(*this, std::move(func));
            }

            auto where_neq(T value)
            {
                const auto func = [value](T a) -> bool
                {
                    return a != value;
                };
                return where_enumerator<T, decltype(func)>(*this, std::move(func));
            }

            std::vector<T> to_vector()
            {
                std::vector<T> result;
                while (static_cast<bool>(*this))
                {
                    result.push_back(**this);
                    ++(*this);
                }
                return result;
            }

            template<typename Iter>
            void copy_to(Iter it)
            {
                while (static_cast<bool>(*this))
                {
                    *it = std::move(**this);
                    ++it;
                    ++(*this);
                }
            }
        };

        template<class T, class Iter>
        class base_enumerator : public enumerator<T>
        {
        public:
            base_enumerator(Iter beg, Iter end) : beg_(beg), end_(end)
            {}

            base_enumerator &operator++() override
            {
                ++beg_;
                return *this;
            }

            explicit operator bool() override
            {
                return beg_ != end_;
            }

            const T &operator*() override
            {
                return *beg_;
            }

        private:
            Iter beg_;
            Iter end_;
        };

        template<class T>
        class take_enumerator : public enumerator<T>
        {
        public:
            take_enumerator(enumerator<T> &parent, int amount) :
                    parent_(parent), amount_remaining_(amount)
            {}

            take_enumerator &operator++() override
            {
                if (amount_remaining_ <= 0 || !static_cast<bool>(parent_))
                {
                    throw linq_exceptions::linq_exception(linq_exceptions::error_messages::out_of_range);
                }
                --amount_remaining_;
                ++parent_;
                return *this;
            }

            explicit operator bool() override
            {
                return (amount_remaining_ > 0 && static_cast<bool>(parent_));
            }

            const T &operator*() override
            {
                return *parent_;
            }

        private:
            enumerator<T> &parent_;
            int amount_remaining_;
        };

        template<class T>
        class drop_enumerator : public enumerator<T>
        {
        public:
            drop_enumerator(enumerator<T> &parent, int count) : parent_(parent)
            {
                for (std::size_t i = 0; i < count && static_cast<bool>(parent_); ++i)
                {
                    ++parent_;
                }
            }

            drop_enumerator &operator++() override
            {
                ++parent_;
                return *this;
            }

            explicit operator bool() override
            {
                return static_cast<bool>(parent_);
            }

            const T &operator*() override
            {
                if (!static_cast<bool>(parent_))
                {
                    throw linq_exceptions::linq_exception(linq_exceptions::error_messages::dereference_error);
                }
                return *parent_;
            }

        private:
            enumerator<T> &parent_;
        };

        template<class T, class U, class F>
        class select_enumerator : public enumerator<T>
        {
        public:
            select_enumerator(enumerator<U> &parent, F func) : parent_(parent), func_(std::move(func))
            {
                update_current();
            }

            select_enumerator &operator++() override
            {
                ++parent_;
                update_current();
                return *this;
            }

            explicit operator bool() override
            {
                return static_cast<bool>(parent_);
            }

            const T &operator*() override
            {
                if (!static_cast<bool>(parent_))
                {
                    throw linq_exceptions::linq_exception(linq_exceptions::error_messages::dereference_error);
                }
                return current_;
            }

        private:
            enumerator<U> &parent_;
            T current_;
            F func_;

            void update_current()
            {
                if (static_cast<bool>(parent_))
                {
                    current_ = func_(*parent_);
                }
            }
        };

        template<class T, class F>
        class until_enumerator : public enumerator<T>
        {
        public:
            until_enumerator(enumerator<T> &parent, F predicate) : parent_(parent), predicate_(std::move(predicate))
            {}

            until_enumerator &operator++() override
            {
                if (!static_cast<bool>(parent_) || ending_flag)
                {
                    throw linq_exceptions::linq_exception(linq_exceptions::error_messages::out_of_range);
                }
                ++parent_;
                updated_flag = false;
                return *this;
            }

            explicit operator bool() override
            {
                if (!updated_flag && !ending_flag)
                {
                    updated_flag = true;
                    ending_flag = !static_cast<bool>(parent_) || predicate_(*parent_);
                }
                return static_cast<bool>(parent_) && !ending_flag;
            }

            const T &operator*() override
            {
                if (!static_cast<bool>(parent_))
                {
                    throw linq_exceptions::linq_exception(linq_exceptions::error_messages::dereference_error);
                }
                return *parent_;
            }

        private:
            enumerator<T> &parent_;
            F predicate_;

            bool ending_flag = false;
            bool updated_flag = false;
        };

        template<class T, class F>
        class where_enumerator : public enumerator<T>
        {
        public:
            where_enumerator(enumerator<T> &parent, F predicate) : parent_(parent), predicate_(std::move(predicate))
            {
                while (static_cast<bool>(parent_) && !predicate_(*parent_))
                {
                    ++parent_;
                }
            }

            where_enumerator &operator++() override
            {
                if (!static_cast<bool>(parent_))
                {
                    throw linq_exceptions::linq_exception(linq_exceptions::error_messages::out_of_range);
                }
                do
                {
                    ++parent_;
                } while (static_cast<bool>(parent_) && !predicate_(*parent_));
                return *this;
            }

            explicit operator bool() override
            {
                return static_cast<bool>(parent_);
            }

            const T &operator*() override
            {
                if (!static_cast<bool>(parent_))
                {
                    throw linq_exceptions::linq_exception(linq_exceptions::error_messages::dereference_error);
                }
                return *parent_;
            }

        private:
            enumerator<T> &parent_;
            F predicate_;
        };
    } // namespace impl

    template<class Iter>
    auto from(Iter beg, Iter end)
    {
        return impl::base_enumerator<typename std::iterator_traits<Iter>::value_type, Iter>(beg, end);
    }
} // namespace linq