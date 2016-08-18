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

#ifndef RTTR_TYPE_DATABASE_P_H_
#define RTTR_TYPE_DATABASE_P_H_

#include "rttr/type.h"
#include "rttr/detail/metadata/metadata.h"
#include "rttr/property.h"
#include "rttr/method.h"
#include "rttr/constructor.h"
#include "rttr/destructor.h"
#include "rttr/enumeration.h"
#include "rttr/array_range.h"
#include "rttr/string_view.h"
#include "rttr/filter_item.h"

#include "rttr/detail/misc/flat_map.h"
#include "rttr/detail/misc/flat_multimap.h"
#include "rttr/detail/type/type_data.h"

#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <unordered_map>

#define RTTR_MAX_TYPE_COUNT 32767
#define RTTR_MAX_INHERIT_TYPES_COUNT 50

#define RTTR_DEFAULT_TYPE_COUNT 4096

namespace rttr
{
namespace detail
{

struct type_comparator_base;

/*!
 * This class holds all type information.
 * It is not part of the rttr API
 */
class RTTR_LOCAL type_database
{
    public:
        type_database(const type_database&) = delete;
        type_database& operator=(const type_database&) = delete;

        static type_database& instance();

        /////////////////////////////////////////////////////////////////////////////////////

        void register_property(const type& t, std::unique_ptr<property_wrapper_base> prop);
        void register_method(const type& t, std::unique_ptr<method_wrapper_base> meth);
        void register_constructor(const type& t, std::unique_ptr<constructor_wrapper_base> ctor);
        void register_destructor(const type& t, std::unique_ptr<destructor_wrapper_base> dtor);
        void register_enumeration(const type& t, std::unique_ptr<enumeration_wrapper_base> enum_data);
        void register_custom_name(type& t, string_view name );
        void register_metadata( const type& t, std::vector<metadata> data);
        void register_converter(const type& t, std::unique_ptr<type_converter_base> converter);
        void register_comparator(const type& t, const type_comparator_base* comparator);

        type register_type(const type& raw_type,
                           const type& array_raw_type,
                           type_data& info) RTTR_NOEXCEPT;

        type get_by_name(string_view name) const;

        /////////////////////////////////////////////////////////////////////////////////////
        property get_type_property(const type& t, string_view name) const;
        property get_class_property(const type& t, string_view name) const;
        array_range<property> get_class_properties(const type& t) const;
        array_range<property> get_class_properties(const type& t, filter_items filter) const;

        property get_global_property(string_view name) const;
        array_range<property> get_global_properties();

        /////////////////////////////////////////////////////////////////////////////////////

        method get_type_method(const type& t, string_view name) const;
        method get_type_method(const type& t, string_view name,
                               const std::vector<type>& type_list) const;

        method get_class_method(const type& t, string_view name) const;
        method get_class_method(const type& t, string_view name,
                                const std::vector<type>& type_list) const;
        method get_class_method(const type& t, string_view name,
                                const std::vector<argument>& arg_list) const;
        array_range<method> get_class_methods(const type& t) const;
        array_range<method> get_class_methods(const type& t, filter_items filter) const;


        method get_global_method(string_view name) const;
        method get_global_method(string_view name, const std::vector<type>& type_list) const;
        method get_global_method(string_view name, const std::vector<argument>& arg_list) const;
        array_range<method> get_global_methods();

        /////////////////////////////////////////////////////////////////////////////////////

        constructor get_constructor(const type& t) const;
        constructor get_constructor(const type& t, const std::vector<type>& arg_type_list) const;
        constructor get_constructor(const type& t, const std::vector<argument>& arg_list) const;
        array_range<constructor> get_constructors(const type& t);
        array_range<constructor> get_constructors(const type& t, filter_items filter) const;

        /////////////////////////////////////////////////////////////////////////////////////

        destructor get_destructor(const type& t) const;

        /////////////////////////////////////////////////////////////////////////////////////

        const type_converter_base* get_converter(const type& source_type, const type& target_type) const;

        /////////////////////////////////////////////////////////////////////////////////////

        const type_comparator_base* get_comparator(const type& t) const;

        /////////////////////////////////////////////////////////////////////////////////////

        variant get_metadata(const type& t, const variant& key) const;

        /////////////////////////////////////////////////////////////////////////////////////

        enumeration get_enumeration(const type& t) const;

        std::vector<type_data*>& get_type_data_func();

        /////////////////////////////////////////////////////////////////////////////////////

    private:
        type_database();
        ~type_database();

        template<typename T>
        static array_range<T> get_items_for_type(const type& t, const std::vector<T>& vec);
        template<typename T>
        static void update_class_list(const type& t, T item_ptr);

        std::string derive_name(const std::string& src_name, const std::string& raw_name, const std::string& custom_name);
        std::string derive_name(const type& array_raw_type, string_view name);
        //! Returns true, when the name was already registered
        bool register_name(const type& array_raw_type, uint16_t& id, type_data& info);
        void register_base_class_info(type_data& info);
        std::vector<metadata>* get_metadata_list(const type& t) const;
        variant get_metadata(const variant& key, const std::vector<metadata>& data) const;

        using hash_type = std::size_t;
        RTTR_INLINE static hash_type generate_hash(const std::string& text) { return generate_hash(text.c_str()); }
        RTTR_INLINE static hash_type generate_hash(const char* text)
        {
            const std::size_t  magic_prime = static_cast<std::size_t>(0x01000193);
            std::size_t               hash = static_cast<std::size_t>(0xcbf29ce4);

            for (; *text; ++text)
              hash = (hash ^ *text) * magic_prime;

            return hash;
        }

        using rttr_cast_func        = void*(*)(void*);
        using get_derived_info_func = derived_info(*)(void*);

    public:

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

        friend class type;

        template<typename T>
        static RTTR_INLINE T* get_item_by_type(const type& t, const std::vector<data_container<T>>& vec);
        template<typename T>
        static RTTR_INLINE void register_item_type(const type& t, std::unique_ptr<T> new_item, std::vector<data_container<T>>& vec);

        type::type_id                                               m_type_id_counter;      //!< The global incremented id counter, this is unique for every type.
        std::vector<type>                                           m_type_list;            //!< The list of all types.

        flat_map<string_view, type>                                 m_orig_name_to_id;
        flat_map<std::string, type, hash>                           m_custom_name_to_id;

        flat_multimap<string_view, method>                          m_global_methods;
        flat_multimap<string_view, property>                        m_global_properties;

        std::vector<std::unique_ptr<property_wrapper_base> >        m_property_list;
        std::vector<std::unique_ptr<method_wrapper_base> >          m_method_list;
        std::vector<std::unique_ptr<constructor_wrapper_base> >     m_constructor_list;
        std::vector<std::unique_ptr<destructor_wrapper_base> >      m_destructor_list;

        std::vector<data_container<type_converter_base>>            m_type_converter_list;  //!< This list stores all type conversion objects
        std::vector<data_container<const type_comparator_base*>>    m_type_comparator_list;
        std::vector<data_container<enumeration_wrapper_base>>       m_enumeration_list;
        std::vector<data_container<std::vector<metadata>>>          m_metadata_type_list;
        std::vector<type_data*>                                     m_type_data_func_list;
};

} // end namespace detail
} // end namespace rttr

#endif // RTTR_TYPE_DATABASE_P_H_
