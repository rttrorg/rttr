/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014 Axel Menzel <info@axelmenzel.de>                             *
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

#ifndef __RTTR_PROPERTY_CONTAINER_FUNC_H__
#define __RTTR_PROPERTY_CONTAINER_FUNC_H__

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global function getter/setter - function pointer

template<typename Getter, typename Setter>
class property_container<function_ptr, Getter, Setter, return_as_copy, set_value> : public property_container_base
{
    using return_type   = typename function_traits<Getter>::return_type;
    using arg_type      = typename param_types<Setter, 0>::type;

    public:
        property_container(const std::string& name, const type declaring_type, Getter getter, Setter setter)
        :   property_container_base(name, declaring_type),
            _getter(getter),
            _setter(setter)
        {
            static_assert(function_traits<Getter>::arg_count == 0, "Invalid number of argument, please provide a getter-function without arguments.");
            static_assert(function_traits<Setter>::arg_count == 1, "Invalid number of argument, please provide a setter-function with exactly one argument.");
            static_assert(std::is_same<return_type, arg_type>::value, "Please provide the same signature for getter and setter!");
        }

        bool is_readonly()  const   { return false; }
        bool is_static()    const   { return true; }
        type get_type()     const   { return type::get<return_type>(); }
        bool is_array()     const   { return detail::is_array<return_type>::value; }

        bool set_value(detail::instance& object, detail::argument& arg) const
        {
            if (arg.is_type<arg_type>())
            {
                _setter(arg.get_value<arg_type>());
                return true;
            }
            return false;
        }

        variant get_value(detail::instance& object) const
        {
            return variant(_getter());
        }

    private:
        Getter _getter;
        Setter _setter;
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global function getter

template<typename Getter>
class property_container<function_ptr, Getter, void, return_as_copy, read_only> : public property_container_base
{
    using return_type = typename function_traits<Getter>::return_type;

    public:
        property_container(const std::string& name, const type declaring_type, Getter func)
        :   property_container_base(name, declaring_type),
           _accessor(func)
        {
            static_assert(function_traits<Getter>::arg_count == 0, "Invalid number of argument, please provide a getter-function without arguments.");
        }

        bool is_readonly()  const   { return true; }
        bool is_static()    const   { return true; }
        type get_type()     const   { return type::get<return_type>(); }
        bool is_array()     const   { return detail::is_array<return_type>::value; }

        bool set_value(detail::instance& object, detail::argument& arg) const
        {
            return false;
        }

        variant get_value(detail::instance& object) const
        {
            return (variant(_accessor()));
        }

    private:
        Getter _accessor;
};



/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global function getter/setter

template<typename Getter, typename Setter>
class property_container<function_ptr, Getter, Setter, return_as_ptr, set_as_ptr> : public property_container_base
{
    using return_type   = typename function_traits<Getter>::return_type;
    using arg_type      = typename param_types<Setter, 0>::type;

    public:
        property_container(const std::string& name, const type declaring_type, Getter getter, Setter setter)
        :   property_container_base(name, declaring_type),
            _getter(getter),
            _setter(setter)
        {
            static_assert(std::is_reference<return_type>::value, "Please provide a getter-function with a reference as return value!");
            static_assert(std::is_reference<arg_type>::value, "Please provide a setter-function with a reference as argument!");

            static_assert(function_traits<Getter>::arg_count == 0, "Invalid number of argument, please provide a getter-function without arguments.");
            static_assert(function_traits<Setter>::arg_count == 1, "Invalid number of argument, please provide a setter-function with exactly one argument.");

            static_assert(std::is_same<return_type, arg_type>::value, "Please provide the same signature for getter and setter!");
        }

        bool is_readonly()  const   { return false; }
        bool is_static()    const   { return true; }
        type get_type()     const   { return type::get<typename std::remove_reference<return_type>::type*>(); }
        bool is_array()     const   { return detail::is_array<return_type>::value; }

        bool set_value(detail::instance& object, detail::argument& arg) const
        {
            using arg_type = typename std::remove_reference<arg_type>::type;
            if (arg.is_type<arg_type*>())
            {
                _setter(*arg.get_value<arg_type*>());
                return true;
            }
            return false;
        }

        variant get_value(detail::instance& object) const
        {
            return variant(&(_getter()));
        }

    private:
        Getter  _getter;
        Setter  _setter;
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global function getter

template<typename Getter>
class property_container<function_ptr, Getter, void, return_as_ptr, read_only> : public property_container_base
{
    using return_type = typename function_traits<Getter>::return_type;
    public:
        property_container(const std::string& name, const type declaring_type, Getter func)
        :   property_container_base(name, declaring_type),
           _accessor(func)
        {
            static_assert(std::is_reference<return_type>::value, "Please provide a function with a reference as return value!");
        }

        bool is_readonly()  const   { return true; }
        bool is_static()    const   { return true; }
        type get_type()     const   { return type::get<typename std::add_const<typename std::remove_reference<return_type>::type>::type*>(); }
        bool is_array()     const   { return detail::is_array<return_type>::value; }

        bool set_value(detail::instance& object, detail::argument& arg) const
        {
            return false;
        }

        variant get_value(detail::instance& object) const
        {
            return (variant(const_cast<const typename std::remove_reference<return_type>::type*>(&(_accessor()))));
        }

    private:
        Getter  _accessor;
};


#endif // __RTTR_PROPERTY_CONTAINER_FUNC_H__
