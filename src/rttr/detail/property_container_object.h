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

#ifndef __RTTR_PROPERTY_CONTAINER_OBJECT_H__
#define __RTTR_PROPERTY_CONTAINER_OBJECT_H__

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global property read write

template<typename C>
class property_container<object_ptr, C*, void, return_as_copy, set_value> : public property_container_base
{
    public:
        property_container(const std::string& name, const type declaring_type, C* pointer)
        :   property_container_base(name, declaring_type),
           _accessor(pointer)
        {
        }

        bool is_readonly()  const   { return false; }
        bool is_static()    const   { return true; }
        type get_type()     const   { return type::get<C>(); }
        bool is_array()     const   { return detail::is_array<C>::value; }

        bool set_value(detail::instance& object, detail::argument& arg) const
        {
            if (arg.is_type<C>())
            {
                return property_accessor<C>::set_value(*_accessor, arg);
            }
            else
            {
                return false;
            }
        }

        variant get_value(detail::instance& object) const
        {
            return (variant(*_accessor));
        }

    private:
        C* _accessor;
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global property read_only

template<typename C>
class property_container<object_ptr, C*, void, return_as_copy, read_only> : public property_container_base
{
    public:
        property_container(const std::string& name, const type declaring_type, C* pointer)
        :   property_container_base(name, declaring_type),
            _accessor(pointer)
        {
        }

        bool is_readonly()  const   { return true; }
        bool is_static()    const   { return true; }
        type get_type()     const   { return type::get<C>(); }
        bool is_array()     const   { return detail::is_array<C>::value; }

        bool set_value(detail::instance& object, detail::argument& arg) const
        {
            return false;
        }

        variant get_value(detail::instance& object) const
        {
            return (variant(*_accessor));
        }

    private:
        C* _accessor;
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global property read write

template<typename C>
class property_container<object_ptr, C*, void, return_as_ptr, set_as_ptr> : public property_container_base
{
    public:
        property_container(const std::string& name, const type declaring_type, C* pointer)
        :   property_container_base(name, declaring_type),
           _accessor(pointer)
        {
            static_assert(!std::is_pointer<C>::value, "The given type is already a pointer type!");
        }

        bool is_readonly()  const   { return false; }
        bool is_static()    const   { return true; }
        type get_type()     const   { return type::get<C*>(); }
        bool is_array()     const   { return detail::is_array<C>::value; }

        bool set_value(detail::instance& object, detail::argument& arg) const
        {
            if (arg.is_type<C*>())
            {
                return property_accessor<C*>::set_value(_accessor, arg);
            }
            else
            {
                return false;
            }
        }

        variant get_value(detail::instance& object) const
        {
            return (variant(_accessor));
        }

    private:
        C* _accessor;
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global property read_only

template<typename C>
class property_container<object_ptr, C*, void, return_as_ptr, read_only> : public property_container_base
{
    public:
        property_container(const std::string& name, const type declaring_type, C* pointer)
        :   property_container_base(name, declaring_type),
            _accessor(pointer)
        {
        }

        bool is_readonly()  const   { return true; }
        bool is_static()    const   { return true; }
        type get_type()     const   { return type::get<typename std::add_const<C>::type*>(); }
        bool is_array()     const   { return detail::is_array<C>::value; }

        bool set_value(detail::instance& object, detail::argument& arg) const
        {
            return false;
        }

        variant get_value(detail::instance& object) const
        {
            return (variant(const_cast<const C*>(_accessor)));
        }

    private:
        C* _accessor;
};

#endif // __RTTR_PROPERTY_CONTAINER_OBJECT_H__
