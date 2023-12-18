/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014 - 2018 Axel Menzel <info@rttr.org>                           *
*                                                                                   *
*   This file is part of RTTR (Run Time Type Reflection)                            *
*   License: MIT License                                                            *
*                                                                                   *
*   Permission is hereby granted, free of charge, to any person obtaining           *
*   a copy of this software and associated documentation files (the "Software"),    *
*   to deal in the Software without restriction, including without limitation       *
*   the rights to use, copy, modify, merge, publish, distribute, sublicense,        *
*   and/or sell copies of the Software, and to permit persons to whom the           *
*   Software is furnished to do so, subject to the following conditions:            *
*                                                                                   *
*   The above copyright notice and this permission notice shall be included in      *
*   all copies or substantial portions of the Software.                             *
*                                                                                   *
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      *
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     *
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          *
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   *
*   SOFTWARE.                                                                       *
*                                                                                   *
*************************************************************************************/

#ifndef RTTR_PROPERTY_WRAPPER_FUNCTOR_H_
#define RTTR_PROPERTY_WRAPPER_FUNCTOR_H_

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global function getter/setter - function pointer

template<typename Declaring_Typ, typename Getter, typename Setter, access_levels Acc_Level, std::size_t Metadata_Count, typename Visitor_List>
class property_wrapper<function_ptr, Declaring_Typ, Getter, Setter, Acc_Level, return_as_copy, set_value, Metadata_Count, Visitor_List>
    : public property_wrapper_base, public metadata_handler<Metadata_Count>
{
    using return_type   = typename function_traits<Getter>::return_type;
    using arg_type      = typename param_types<Setter, 1>::type;

    public:
        property_wrapper(string_view name,
                         Getter get, Setter set,
                         std::array<metadata, Metadata_Count> metadata_list) RTTR_NOEXCEPT
        :   property_wrapper_base(name, type::get<Declaring_Typ>()),
            metadata_handler<Metadata_Count>(std::move(metadata_list)),
            m_getter(get), m_setter(set)
        {
            static_assert(function_traits<Getter>::arg_count == 1, "Invalid number of argument, please provide a getter-functor with a self argument.");
            static_assert(function_traits<Setter>::arg_count == 2, "Invalid number of argument, please provide a setter-functor with self and value arguments.");
            static_assert(std::is_same<return_type, arg_type>::value, "Please provide the same signature for getter and setter!");

            init();
        }

        access_levels get_access_level() const RTTR_NOEXCEPT    { return Acc_Level; }
        bool is_valid()     const RTTR_NOEXCEPT                 { return true;  }
        bool is_readonly()  const RTTR_NOEXCEPT                 { return false; }
        bool is_static()    const RTTR_NOEXCEPT                 { return false; }
        type get_type()     const RTTR_NOEXCEPT                 { return type::get<return_type>(); }

        variant get_metadata(const variant& key) const { return metadata_handler<Metadata_Count>::get_metadata(key); }

        bool set_value(instance& object, argument& arg) const
        {
            Declaring_Typ* ptr = object.try_convert<Declaring_Typ>();
            if (ptr && arg.is_type<arg_type>())
            {
                m_setter(ptr, arg.get_value<arg_type>());
                return true;
            }
            return false;
        }

        variant get_value(instance& object) const
        {
            if (Declaring_Typ* ptr = object.try_convert<Declaring_Typ>())
                return variant(m_getter(ptr));
            else
                return variant();
        }

        void visit(visitor& visitor, property prop) const RTTR_NOEXCEPT
        {
            auto obj = make_property_getter_setter_info<Declaring_Typ, return_as_copy, Getter, Setter>(prop, m_getter, m_setter);
            visitor_iterator<Visitor_List>::visit(visitor, make_property_getter_setter_visitor_invoker(obj));
        }

    private:
        Getter m_getter;
        Setter m_setter;
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global function getter

template<typename Declaring_Typ, typename Getter, access_levels Acc_Level, std::size_t Metadata_Count, typename Visitor_List>
class property_wrapper<function_ptr, Declaring_Typ, Getter, void, Acc_Level, return_as_copy, read_only, Metadata_Count, Visitor_List>
    : public property_wrapper_base, public metadata_handler<Metadata_Count>
{
    using return_type = typename function_traits<Getter>::return_type;

    public:
        property_wrapper(string_view name,
                         Getter get, std::array<metadata, Metadata_Count> metadata_list) RTTR_NOEXCEPT
        :   property_wrapper_base(name, type::get<Declaring_Typ>()),
            metadata_handler<Metadata_Count>(std::move(metadata_list)),
            m_accessor(get)
        {
            static_assert(function_traits<Getter>::arg_count == 1, "Invalid number of argument, please provide a getter-functor with a self argument.");

            init();
        }

        access_levels get_access_level() const RTTR_NOEXCEPT    { return Acc_Level; }
        bool is_valid()     const RTTR_NOEXCEPT                 { return true;  }
        bool is_readonly()  const RTTR_NOEXCEPT                 { return true; }
        bool is_static()    const RTTR_NOEXCEPT                 { return false; }
        type get_type()     const RTTR_NOEXCEPT                 { return type::get<return_type>(); }
        bool is_array()     const RTTR_NOEXCEPT                 { return std::is_array<return_type>::value; }

        variant get_metadata(const variant& key) const { return metadata_handler<Metadata_Count>::get_metadata(key); }

        bool set_value(instance& object, argument& arg) const
        {
            return false;
        }

        variant get_value(instance& object) const
        {
            if (Declaring_Typ* ptr = object.try_convert<Declaring_Typ>())
                return variant(m_accessor(ptr));
            else
                return variant();
        }

        void visit(visitor& visitor, property prop) const RTTR_NOEXCEPT
        {
            auto obj = make_property_info<Declaring_Typ, return_as_copy, Getter>(prop, m_accessor);
            visitor_iterator<Visitor_List>::visit(visitor, make_property_visitor_invoker<read_only>(obj));
        }

    private:
        Getter m_accessor;
};



/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global function getter/setter

template<typename Declaring_Typ, typename Getter, typename Setter, access_levels Acc_Level, std::size_t Metadata_Count, typename Visitor_List>
class property_wrapper<function_ptr, Declaring_Typ, Getter, Setter, Acc_Level, return_as_ptr, set_as_ptr, Metadata_Count, Visitor_List>
    : public property_wrapper_base, public metadata_handler<Metadata_Count>
{
    using return_type   = typename function_traits<Getter>::return_type;
    using arg_type      = typename param_types<Setter, 1>::type;

    public:
        property_wrapper(string_view name,
                         Getter get, Setter set,
                         std::array<metadata, Metadata_Count> metadata_list) RTTR_NOEXCEPT
        :   property_wrapper_base(name, type::get<Declaring_Typ>()),
            metadata_handler<Metadata_Count>(std::move(metadata_list)),
            m_getter(get), m_setter(set)
        {
            static_assert(std::is_reference<return_type>::value, "Please provide a getter-function with a reference as return value!");
            static_assert(std::is_reference<arg_type>::value, "Please provide a setter-function with a reference as argument!");

            static_assert(function_traits<Getter>::arg_count == 1, "Invalid number of argument, please provide a getter-functor with a self argument.");
            static_assert(function_traits<Setter>::arg_count == 2, "Invalid number of argument, please provide a setter-functor with self and value arguments.");

            static_assert(std::is_same<return_type, arg_type>::value, "Please provide the same signature for getter and setter!");

            init();
        }

        access_levels get_access_level() const RTTR_NOEXCEPT    { return Acc_Level; }
        bool is_valid()     const RTTR_NOEXCEPT                 { return true;  }
        bool is_readonly()  const RTTR_NOEXCEPT                 { return false; }
        bool is_static()    const RTTR_NOEXCEPT                 { return false; }
        type get_type()     const RTTR_NOEXCEPT                 { return type::get<typename std::remove_reference<return_type>::type*>(); }

        variant get_metadata(const variant& key) const { return metadata_handler<Metadata_Count>::get_metadata(key); }

        bool set_value(instance& object, argument& arg) const
        {
            Declaring_Typ* ptr = object.try_convert<Declaring_Typ>();
            using arg_type_t = remove_reference_t<arg_type>;
            if (ptr && arg.is_type<arg_type_t*>())
            {
                m_setter(ptr, *arg.get_value<arg_type_t*>());
                return true;
            }
            return false;
        }

        variant get_value(instance& object) const
        {
            if (Declaring_Typ* ptr = object.try_convert<Declaring_Typ>())
                return variant(&(m_getter(ptr)));
            else
                return variant();
        }

        void visit(visitor& visitor, property prop) const RTTR_NOEXCEPT
        {
            auto obj = make_property_getter_setter_info<Declaring_Typ, return_as_ptr, Getter, Setter>(prop, m_getter, m_setter);
            visitor_iterator<Visitor_List>::visit(visitor, make_property_getter_setter_visitor_invoker(obj));
        }

    private:
        Getter  m_getter;
        Setter  m_setter;
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global function getter

template<typename Declaring_Typ, typename Getter, access_levels Acc_Level, std::size_t Metadata_Count, typename Visitor_List>
class property_wrapper<function_ptr, Declaring_Typ, Getter, void, Acc_Level, return_as_ptr, read_only, Metadata_Count, Visitor_List>
    : public property_wrapper_base, public metadata_handler<Metadata_Count>
{
    using return_type = typename function_traits<Getter>::return_type;
    public:
        property_wrapper(string_view name,
                         Getter get, std::array<metadata, Metadata_Count> metadata_list) RTTR_NOEXCEPT
        :   property_wrapper_base(name, type::get<Declaring_Typ>()),
            metadata_handler<Metadata_Count>(std::move(metadata_list)),
            m_accessor(get)
        {
            static_assert(function_traits<Getter>::arg_count == 1, "Invalid number of argument, please provide a getter-functor with a self argument.");
            static_assert(std::is_reference<return_type>::value, "Please provide a function with a reference as return value!");

            init();
        }

        access_levels get_access_level() const RTTR_NOEXCEPT { return Acc_Level; }
        bool is_valid()     const RTTR_NOEXCEPT { return true;  }
        bool is_readonly()  const RTTR_NOEXCEPT { return true; }
        bool is_static()    const RTTR_NOEXCEPT { return false; }
        type get_type()     const RTTR_NOEXCEPT { return type::get<typename std::add_const<typename std::remove_reference<return_type>::type>::type*>(); }

        variant get_metadata(const variant& key) const { return metadata_handler<Metadata_Count>::get_metadata(key); }

        bool set_value(instance& object, argument& arg) const
        {
            return false;
        }

        variant get_value(instance& object) const
        {
            if (Declaring_Typ* ptr = object.try_convert<Declaring_Typ>())
                return (variant(const_cast<const typename std::remove_reference<return_type>::type*>(&(m_accessor(ptr)))));
            else
                return variant();
        }

        void visit(visitor& visitor, property prop) const RTTR_NOEXCEPT
        {
            auto obj = make_property_info<Declaring_Typ, return_as_copy, Getter>(prop, m_accessor);
            visitor_iterator<Visitor_List>::visit(visitor, make_property_visitor_invoker<read_only>(obj));
        }

    private:
        Getter  m_accessor;
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global function getter/setter

template<typename Declaring_Typ, typename Getter, typename Setter, access_levels Acc_Level, std::size_t Metadata_Count, typename Visitor_List>
class property_wrapper<function_ptr, Declaring_Typ, Getter, Setter, Acc_Level, get_as_ref_wrapper, set_as_ref_wrapper, Metadata_Count, Visitor_List>
    : public property_wrapper_base, public metadata_handler<Metadata_Count>
{
    using return_type   = typename function_traits<Getter>::return_type;
    using arg_type      = typename param_types<Setter, 1>::type;

    public:
        property_wrapper(string_view name,
                         Getter get, Setter set,
                         std::array<metadata, Metadata_Count> metadata_list) RTTR_NOEXCEPT
        :   property_wrapper_base(name, type::get<Declaring_Typ>()),
            metadata_handler<Metadata_Count>(std::move(metadata_list)),
            m_getter(get), m_setter(set)
        {
            static_assert(std::is_reference<return_type>::value, "Please provide a getter-function with a reference as return value!");
            static_assert(std::is_reference<arg_type>::value, "Please provide a setter-function with a reference as argument!");

            static_assert(function_traits<Getter>::arg_count == 1, "Invalid number of argument, please provide a getter-functor with a self argument.");
            static_assert(function_traits<Setter>::arg_count == 2, "Invalid number of argument, please provide a setter-functor with self and value arguments.");

            static_assert(std::is_same<return_type, arg_type>::value, "Please provide the same signature for getter and setter!");

            init();
        }

        access_levels get_access_level() const RTTR_NOEXCEPT    { return Acc_Level; }
        bool is_valid()     const RTTR_NOEXCEPT                 { return true;  }
        bool is_readonly()  const RTTR_NOEXCEPT                 { return false; }
        bool is_static()    const RTTR_NOEXCEPT                 { return false; }
        type get_type()     const RTTR_NOEXCEPT                 { return type::get< std::reference_wrapper<remove_reference_t<return_type>> >(); }

        variant get_metadata(const variant& key) const { return metadata_handler<Metadata_Count>::get_metadata(key); }

        bool set_value(instance& object, argument& arg) const
        {
            Declaring_Typ* ptr = object.try_convert<Declaring_Typ>();
            using arg_type_t = remove_reference_t<arg_type>;
            if (ptr && arg.is_type<std::reference_wrapper<arg_type_t>>())
            {
                m_setter(ptr, arg.get_value<std::reference_wrapper<arg_type_t>>().get());
                return true;
            }
            return false;
        }

        variant get_value(instance& object) const
        {
            if (Declaring_Typ* ptr = object.try_convert<Declaring_Typ>())
                return variant(std::ref(m_getter(ptr)));
            else
                return variant();
        }

        void visit(visitor& visitor, property prop) const RTTR_NOEXCEPT
        {
            auto obj = make_property_getter_setter_info<Declaring_Typ, get_as_ref_wrapper, Getter, Setter>(prop, m_getter, m_setter);
            visitor_iterator<Visitor_List>::visit(visitor, make_property_getter_setter_visitor_invoker(obj));
        }

    private:
        Getter  m_getter;
        Setter  m_setter;
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global function getter

template<typename Declaring_Typ, typename Getter, access_levels Acc_Level, std::size_t Metadata_Count, typename Visitor_List>
class property_wrapper<function_ptr, Declaring_Typ, Getter, void, Acc_Level, get_as_ref_wrapper, read_only, Metadata_Count, Visitor_List>
    : public property_wrapper_base, public metadata_handler<Metadata_Count>
{
    using return_type = typename function_traits<Getter>::return_type;
    using policy_type = std::reference_wrapper<add_const_t<remove_reference_t<return_type>>>;

    public:
        property_wrapper(string_view name,
                         Getter get, std::array<metadata, Metadata_Count> metadata_list) RTTR_NOEXCEPT
        :   property_wrapper_base(name, type::get<Declaring_Typ>()),
            metadata_handler<Metadata_Count>(std::move(metadata_list)),
            m_accessor(get)
        {
            static_assert(function_traits<Getter>::arg_count == 1, "Invalid number of argument, please provide a getter-functor with a self argument.");
            static_assert(std::is_reference<return_type>::value, "Please provide a function with a reference as return value!");

            init();
        }

        access_levels get_access_level() const RTTR_NOEXCEPT { return Acc_Level; }
        bool is_valid()     const RTTR_NOEXCEPT { return true; }
        bool is_readonly()  const RTTR_NOEXCEPT { return true; }
        bool is_static()    const RTTR_NOEXCEPT { return false; }
        type get_type()     const RTTR_NOEXCEPT { return type::get<policy_type>(); }

        variant get_metadata(const variant& key) const { return metadata_handler<Metadata_Count>::get_metadata(key); }

        bool set_value(instance& object, argument& arg) const
        {
            return false;
        }

        variant get_value(instance& object) const
        {
            if (Declaring_Typ* ptr = object.try_convert<Declaring_Typ>())
                return variant(std::cref(m_accessor(ptr)));
            else
                return variant();
        }

        void visit(visitor& visitor, property prop) const RTTR_NOEXCEPT
        {
            auto obj = make_property_info<Declaring_Typ, return_as_copy, Getter>(prop, m_accessor);
            visitor_iterator<Visitor_List>::visit(visitor, make_property_visitor_invoker<read_only>(obj));
        }

    private:
        Getter  m_accessor;
};


#endif // RTTR_PROPERTY_WRAPPER_FUNCTOR_H_
