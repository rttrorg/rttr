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

#ifndef __RTTR_PROPERTY_CONTAINER_MEMBER_OBJECT_H__
#define __RTTR_PROPERTY_CONTAINER_MEMBER_OBJECT_H__

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// pointer to member - read write

template<typename C, typename A>
class property_container<member_object_ptr, A(C::*), void, return_as_copy, set_value> : public property_container_base
{
    typedef A (C::*accessor);
    public:
        property_container(const std::string& name, const type declaring_type, accessor acc)
        :   property_container_base(name, declaring_type), _acc(acc)
        {
        }

        bool is_readonly()  const   { return false; }
        bool is_static()    const   { return false; }
        type get_type()     const   { return type::get<A>(); }
        bool is_array()     const   { return detail::is_array<A>::value; }

        bool set_value(detail::instance& object, detail::argument& arg) const
        {
            C* ptr = object.try_convert<C>();
            if (ptr && arg.is_type<A>())
                return property_accessor<A>::set_value((ptr->*_acc), arg);
            else
                return false;
        }

        variant get_value(detail::instance& object) const
        {
            if (C* ptr = object.try_convert<C>())
                return variant((ptr->*_acc));
            else
                return variant();
        }

    private:
        accessor _acc;
};


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// pointer to member - read only (because of std::false_type)

template<typename C, typename A>
class property_container<member_object_ptr, A(C::*), void, return_as_copy, read_only> : public property_container_base
{
    typedef A (C::*accessor);
    public:
        property_container(const std::string& name, const type declaring_type, accessor acc)
        :   property_container_base(name, declaring_type),
            _acc(acc)
        {
        }

        bool is_readonly()  const   { return true; }
        bool is_static()    const   { return false; }
        type get_type()     const   { return type::get<A>(); }
        bool is_array()     const   { return detail::is_array<A>::value; }

        bool set_value(detail::instance& object, detail::argument& arg) const
        {
            return false;
        }

        variant get_value(detail::instance& object) const
        {
            if (C* ptr = object.try_convert<C>())
                return variant((ptr->*_acc));
            else
                return variant();
        }

    private:
        accessor _acc;
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// pointer to member - read write

template<typename C, typename A>
class property_container<member_object_ptr, A(C::*), void, return_as_ptr, set_as_ptr> : public property_container_base
{
    typedef A (C::*accessor);
    public:
        property_container(const std::string& name, const type declaring_type, accessor acc)
        :   property_container_base(name, declaring_type), _acc(acc)
        {
            static_assert(!std::is_pointer<A>::value, "The given type is already a pointer type!");
        }

        bool is_readonly()  const   { return false; }
        bool is_static()    const   { return false; }
        type get_type()     const   { return type::get<A*>(); }
        bool is_array()     const   { return detail::is_array<A>::value; }

        bool set_value(detail::instance& object, detail::argument& arg) const
        {
            C* ptr = object.try_convert<C>();
            if (ptr && arg.is_type<A*>())
            {
                return property_accessor<A*>::set_value(&(ptr->*_acc), arg);
            }
            else
            {
                return false;
            }
        }

        variant get_value(detail::instance& object) const
        {
            if (C* ptr = object.try_convert<C>())
                return variant(&(ptr->*_acc));
            else
                return variant();
        }

    private:
        accessor _acc;
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// pointer to member - read only

template<typename C, typename A>
class property_container<member_object_ptr, A(C::*), void, return_as_ptr, read_only> : public property_container_base
{
    typedef A (C::*accessor);
    public:
        property_container(const std::string& name, const type declaring_type, accessor acc)
        :   property_container_base(name, declaring_type),
            _acc(acc)
        {
            static_assert(!std::is_pointer<A>::value, "The given type is already a pointer type!");
        }

        bool is_readonly()  const   { return true; }
        bool is_static()    const   { return false; }
        type get_type()     const   { return type::get<typename std::add_const<A>::type*>(); }
        bool is_array()     const   { return detail::is_array<A>::value; }

        bool set_value(detail::instance& object, detail::argument& arg) const
        {
            return false;
        }

        variant get_value(detail::instance& object) const
        {
            if (C* ptr = object.try_convert<C>())
                return variant(const_cast<const A*>(&(ptr->*_acc)));
            else
                return variant();
        }

    private:
        accessor _acc;
};

#endif // __RTTR_PROPERTY_CONTAINER_MEMBER_OBJECT_H__
