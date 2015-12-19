/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014, 2015 - 2016 Axel Menzel <info@rttr.org>                     *
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

#ifndef RTTR_PROPERTY_WRAPPER_OBJECT_H_
#define RTTR_PROPERTY_WRAPPER_OBJECT_H_

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global property read write

template<typename C, access_levels Acc_Level, std::size_t Metadata_Count>
class property_wrapper<object_ptr, C*, void, Acc_Level, return_as_copy, set_value, Metadata_Count> : public property_wrapper_base, public metadata_handler<Metadata_Count>
{
    public:
        property_wrapper(C* pointer, std::array<metadata, Metadata_Count> metadata_list)
        :   metadata_handler<Metadata_Count>(std::move(metadata_list)),
            m_accessor(pointer)
        {
        }

        access_levels get_access_level() const { return Acc_Level; }
        bool is_readonly()  const   { return false; }
        bool is_static()    const   { return true; }
        type get_type()     const   { return type::get<C>(); }
        bool is_array()     const   { return detail::is_array<C>::value; }

        variant get_metadata(const variant& key) const { return metadata_handler<Metadata_Count>::get_metadata(key); }

        bool set_value(instance& object, argument& arg) const
        {
            if (arg.is_type<C>())
            {
                return property_accessor<C>::set_value(*m_accessor, arg);
            }
            else
            {
                return false;
            }
        }

        variant get_value(instance& object) const
        {
            return (variant(*m_accessor));
        }

    private:
        C* m_accessor;
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global property read_only

template<typename C, access_levels Acc_Level, std::size_t Metadata_Count>
class property_wrapper<object_ptr, C*, void, Acc_Level, return_as_copy, read_only, Metadata_Count> : public property_wrapper_base, public metadata_handler<Metadata_Count>
{
    public:
        property_wrapper(C* pointer, std::array<metadata, Metadata_Count> metadata_list)
        :   metadata_handler<Metadata_Count>(std::move(metadata_list)), m_accessor(pointer)
        {
        }

        access_levels get_access_level() const { return Acc_Level; }
        bool is_readonly()  const   { return true; }
        bool is_static()    const   { return true; }
        type get_type()     const   { return type::get<C>(); }
        bool is_array()     const   { return detail::is_array<C>::value; }

        variant get_metadata(const variant& key) const { return metadata_handler<Metadata_Count>::get_metadata(key); }

        bool set_value(instance& object, argument& arg) const
        {
            return false;
        }

        variant get_value(instance& object) const
        {
            return (variant(*m_accessor));
        }

    private:
        C* m_accessor;
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global property read write

template<typename C, access_levels Acc_Level, std::size_t Metadata_Count>
class property_wrapper<object_ptr, C*, void, Acc_Level, return_as_ptr, set_as_ptr, Metadata_Count> : public property_wrapper_base, public metadata_handler<Metadata_Count>
{
    public:
        property_wrapper(C* pointer, std::array<metadata, Metadata_Count> metadata_list)
        :   metadata_handler<Metadata_Count>(std::move(metadata_list)), m_accessor(pointer)
        {
            static_assert(!std::is_pointer<C>::value, "The given type is already a pointer type!");
        }

        access_levels get_access_level() const { return Acc_Level; }
        bool is_readonly()  const   { return false; }
        bool is_static()    const   { return true; }
        type get_type()     const   { return type::get<C*>(); }
        bool is_array()     const   { return detail::is_array<C>::value; }

        variant get_metadata(const variant& key) const { return metadata_handler<Metadata_Count>::get_metadata(key); }

        bool set_value(instance& object, argument& arg) const
        {
            if (arg.is_type<C*>())
            {
                return property_accessor<C*>::set_value(m_accessor, arg);
            }
            else
            {
                return false;
            }
        }

        variant get_value(instance& object) const
        {
            return (variant(m_accessor));
        }

    private:
        C* m_accessor;
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// global property read_only

template<typename C, access_levels Acc_Level, std::size_t Metadata_Count>
class property_wrapper<object_ptr, C*, void, Acc_Level, return_as_ptr, read_only, Metadata_Count> : public property_wrapper_base, public metadata_handler<Metadata_Count>
{
    public:
        property_wrapper(C* pointer, std::array<metadata, Metadata_Count> metadata_list)
        :   metadata_handler<Metadata_Count>(std::move(metadata_list)), m_accessor(pointer)
        {
        }

        access_levels get_access_level() const { return Acc_Level; }
        bool is_readonly()  const   { return true; }
        bool is_static()    const   { return true; }
        type get_type()     const   { return type::get<typename std::add_const<C>::type*>(); }
        bool is_array()     const   { return detail::is_array<C>::value; }

        variant get_metadata(const variant& key) const { return metadata_handler<Metadata_Count>::get_metadata(key); }

        bool set_value(instance& object, argument& arg) const
        {
            return false;
        }

        variant get_value(instance& object) const
        {
            return (variant(const_cast<const C*>(m_accessor)));
        }

    private:
        C* m_accessor;
};

#endif // RTTR_PROPERTY_WRAPPER_OBJECT_H_
