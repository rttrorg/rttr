/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014, 2015 - 2017 Axel Menzel <info@rttr.org>                     *
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

#ifndef RTTR_TYPE_REGISTER_P_H_
#define RTTR_TYPE_REGISTER_P_H_

#include "rttr/detail/base/core_prerequisites.h"
#include "rttr/detail/misc/flat_multimap.h"
#include "rttr/detail/misc/flat_map.h"
#include "rttr/enumeration.h"
#include "rttr/variant.h"
#include "rttr/detail/metadata/metadata.h"

#include "rttr/string_view.h"

#include <memory>
#include <string>
#include <vector>

namespace rttr
{
class type;
class property;
class method;

namespace detail
{

class constructor_wrapper_base;
class destructor_wrapper_base;
class property_wrapper_base;
class method_wrapper_base;
class enumeration_wrapper_base;

struct type_data;

/*!
 * This class contains all logic to register properties, methods etc.. for a specific type.
 * It is not part of the rttr API
 */
class RTTR_LOCAL type_register_private
{
public:

    static type register_type(type_data& info) RTTR_NOEXCEPT;

    static void constructor(const type& t, std::unique_ptr<constructor_wrapper_base> ctor);
    static void destructor(const type& t, std::unique_ptr<destructor_wrapper_base> dtor);
    static void property(const type& t, std::unique_ptr<property_wrapper_base> prop);
    static void method(const type& t, std::unique_ptr<method_wrapper_base> meth);
    static void register_custom_name(type& t, string_view custom_name);

    /////////////////////////////////////////////////////////////////////////////////////
    static flat_multimap<string_view, ::rttr::property>& get_global_property_storage();
    static flat_multimap<string_view, ::rttr::method>& get_global_method_storage();
    /////////////////////////////////////////////////////////////////////////////////////

    static std::vector<type_data*>& get_type_data_storage();
    static std::vector<type>& get_type_storage();
    static flat_map<string_view, type>& get_orig_name_to_id();
    static flat_map<std::string, type, hash>& get_custom_name_to_id();

    /////////////////////////////////////////////////////////////////////////////////////

    static void register_enumeration(const type& t, std::unique_ptr<enumeration_wrapper_base> enum_data);
    static void register_metadata( const type& t, std::vector<metadata> data);
    static void converter(const type& t, std::unique_ptr<type_converter_base> converter);
    static void comparator(const type& t, const type_comparator_base* comparator);
    static void equal_comparator(const type& t, const type_comparator_base* comparator);
    static void less_than_comparator(const type& t, const type_comparator_base* comparator);

    /////////////////////////////////////////////////////////////////////////////////////

    static const type_converter_base* get_converter(const type& source_type, const type& target_type);
    static const type_comparator_base* get_comparator(const type& t);
    static const type_comparator_base* get_equal_comparator(const type& t);
    static const type_comparator_base* get_less_than_comparator(const type& t);
    static variant get_metadata(const type& t, const variant& key);
    static enumeration get_enumeration(const type& t);

    /////////////////////////////////////////////////////////////////////////////////////

private:

    template<typename T, typename Data_Type = conditional_t<std::is_pointer<T>::value, T, std::unique_ptr<T>>>
    struct data_container
    {
        data_container(type::type_id id) : m_id(id) {}
        data_container(type::type_id id, Data_Type data) : m_id(id), m_data(std::move(data)) {}
        data_container(data_container<T, Data_Type>&& other) : m_id(other.m_id), m_data(std::move(other.m_data)) {}
        data_container<T, Data_Type>& operator = (data_container<T, Data_Type>&& other)
        {
            m_id = other.m_id;
            m_data = std::move(other.m_data);
            return *this;
        }

        struct order_by_id
        {
            RTTR_INLINE bool operator () ( const data_container<T>& _left, const data_container<T>& _right )  const
            {
                return _left.m_id < _right.m_id;
            }
            RTTR_INLINE bool operator () ( const type::type_id& _left, const data_container<T>& _right ) const
            {
                return _left < _right.m_id;
            }
            RTTR_INLINE bool operator () ( const data_container<T>& _left, const type::type_id& _right ) const
            {
                return _left.m_id < _right;
            }
        };

        type::type_id   m_id;
        Data_Type       m_data;
    };

    template<typename T>
    static RTTR_INLINE T* get_item_by_type(const type& t, const std::vector<data_container<T>>& vec);
    template<typename T>
    static RTTR_INLINE void register_item_type(const type& t, std::unique_ptr<T> new_item, std::vector<data_container<T>>& vec);

    static std::vector<metadata>* get_metadata_list(const type& t);
    static variant get_metadata(const variant& key, const std::vector<metadata>& data);

    static std::vector<data_container<type_converter_base>>& get_type_converter_list();
    static std::vector<data_container<const type_comparator_base*>>& get_type_comparator_list();

    static std::vector<data_container<const type_comparator_base*>>& get_type_equal_comparator_list();
    static std::vector<data_container<const type_comparator_base*>>& get_type_less_comparator_list();

    static std::vector<data_container<enumeration_wrapper_base>>& get_enumeration_list();
    static std::vector<data_container<std::vector<metadata>>>& get_metadata_type_list();

    static void register_comparator_impl(const type& t, const type_comparator_base* comparator,
                                         std::vector<data_container<const type_comparator_base*>>& comparator_list);
    static const type_comparator_base* get_type_comparator_impl(const type& t,
                                                                const std::vector<data_container<const type_comparator_base*>>& comparator_list);

    static ::rttr::property get_type_property(const type& t, string_view name);
    static ::rttr::method get_type_method(const type& t, string_view name);
    static ::rttr::method get_type_method(const type& t, string_view name,
                                          const std::vector<type>& type_list);

    template<typename T>
    static void update_class_list(const type& t, T item_ptr);

    static std::string derive_name(const type_data& wrapper_type,
                                   const type_data& array_raw_type,
                                   string_view name);
    //! Returns true, when the name was already registered
    static bool register_name(uint16_t& id, type_data& info);
    static void register_base_class_info(type_data& info);

};

} // end namespace detail
} // end namespace rttr

#endif // RTTR_TYPE_REGISTER_P_H_
