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

#include <vector>
#include <string>
#include <memory>
#include <functional>

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
        void register_custom_name(const type& t, std::string );
        void register_metadata( const type& t, std::vector<metadata> data);
        void register_converter(const type& t, std::unique_ptr<type_converter_base> converter);
        void register_comparator(const type& t, const type_comparator_base* comparator);

        uint16_t register_type(const char* name, 
                               const type& raw_type,
                               const type& wrapped_type,
                               const type& array_raw_type,
                               std::vector<base_class_info> base_classes, 
                               get_derived_func derived_func_ptr,
                               variant_create_func var_func_ptr,
                               std::size_t type_size,
                               bool is_class,
                               bool is_enum,
                               bool is_array,
                               bool is_pointer,
                               bool is_arithmetic,
                               bool is_function_pointer,
                               bool is_member_object_pointer,
                               bool is_member_function_pointer,
                               std::size_t pointer_dimension);

        uint16_t get_by_name(const char* name) const;

        /////////////////////////////////////////////////////////////////////////////////////

        const property_wrapper_base* get_class_property(const type& t, const char* name) const;
        std::vector<const property_wrapper_base*> get_all_class_properties(const type& t) const;
        uint16_t get_class_property_count(const type& t) const;
    
        const property_wrapper_base* get_global_property(const char* name) const;
        std::vector<const property_wrapper_base*> get_all_global_properties() const;
        uint16_t get_global_property_count(const type& t) const;

        /////////////////////////////////////////////////////////////////////////////////////

        const method_wrapper_base* get_class_method(const type& t, const char* name) const;

        template<typename Container, typename Compare_Type>
        const method_wrapper_base* get_class_method(const type& t, const char* name, const Container& container) const;

        const method_wrapper_base* get_class_method(const type& t, const char* name, 
                                                    const std::vector<type>& param_type_list) const;
        const method_wrapper_base* get_class_method(const type& t, const char* name, 
                                                    const std::vector<argument>& arg_list) const;

        std::vector<const method_wrapper_base*> get_all_class_methods(const type& t) const;
        uint16_t get_class_method_count(const type& t) const;

        /////////////////////////////////////////////////////////////////////////////////////

        const method_wrapper_base* get_global_method(const char* name) const;
        template<typename Container, typename Compare_Type>
        const method_wrapper_base* get_global_method(const char* name, const Container& container) const;

        const method_wrapper_base* get_global_method(const char* name, const std::vector<type>& arg_type_list) const;
        const method_wrapper_base* get_global_method(const char* name, const std::vector<argument>& arg_list) const;
        std::vector<const method_wrapper_base*> get_all_global_methods() const;
        uint16_t get_global_method_count(const type& t) const;

        /////////////////////////////////////////////////////////////////////////////////////

        const constructor_wrapper_base* get_constructor(const type& t) const;
        template<typename Container, typename Comparer_Type>
        const constructor_wrapper_base* get_constructor(const type& t, const Container& container) const;

        const constructor_wrapper_base* get_constructor(const type& t, const std::vector<type>& arg_type_list) const;
        const constructor_wrapper_base* get_constructor(const type& t, const std::vector<argument>& arg_list) const;
        std::vector<const constructor_wrapper_base*> get_constructors(const type& t) const;

        /////////////////////////////////////////////////////////////////////////////////////

        const destructor_wrapper_base* get_destructor(const type& t) const;

        /////////////////////////////////////////////////////////////////////////////////////

        const type_converter_base* get_converter(const type& source_type, const type& target_type) const;

        /////////////////////////////////////////////////////////////////////////////////////

        const type_comparator_base* get_comparator(const type& t) const;

        /////////////////////////////////////////////////////////////////////////////////////

        variant get_metadata(const type& t, const variant& key) const;

        /////////////////////////////////////////////////////////////////////////////////////

        const enumeration_wrapper_base* get_enumeration(const type& t) const;

        /////////////////////////////////////////////////////////////////////////////////////
        
    private:
        type_database();

        std::string derive_name(const std::string& src_name, const std::string& raw_name, const std::string& custom_name);
        std::string derive_name(const type& array_raw_type, const char* name);
        //! Returns true, when the name was already registered
        bool register_name(const char* name, const type& array_raw_type, uint16_t& id);
        void register_base_class_info(const type& src_type, const type& raw_type, std::vector<base_class_info> base_classes);
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

        struct name_to_id
        {
            type::type_id   m_id;
            hash_type       m_hash_value;

            struct order_by_name
            {
                RTTR_INLINE bool operator () ( const name_to_id& _left, const name_to_id& _right )  const
                {
                    return _left.m_hash_value < _right.m_hash_value;
                }
                RTTR_INLINE bool operator () ( const hash_type& _left, const name_to_id& _right ) const
                {
                    return _left < _right.m_hash_value;
                }
                RTTR_INLINE bool operator () ( const name_to_id& _left, const hash_type& _right ) const
                {
                    return _left.m_hash_value < _right;
                }
            };
        };

        template<typename T>
        struct class_member
        {
            class_member(type::type_id id) : m_class_id(id), m_register_index(0), m_name_hash(0) {}
            class_member(type::type_id id, hash_type hash_value) : m_class_id(id), m_register_index(0), m_name_hash(hash_value) {}
            class_member(type::type_id id, uint16_t register_index, hash_type hash_value, std::unique_ptr<T> data)
            :   m_class_id(id), m_register_index(register_index), m_name_hash(hash_value), m_data(move(data)) {}
           
            class_member(class_member<T>&& other) : m_class_id(other.m_class_id), m_register_index(other.m_register_index), 
                                                    m_name_hash(other.m_name_hash), m_data(std::move(other.m_data)) {}

            class_member<T>& operator = (class_member<T>&& other) 
            {
                m_class_id = other.m_class_id;
                m_register_index = other.m_register_index;
                m_name_hash = other.m_name_hash;
                m_data = std::move(other.m_data);

                other.m_class_id = 0;
                other.m_register_index = 0;
                other.m_name_hash = 0;
                return *this;
            }

            struct order
            {
                RTTR_INLINE bool operator () (const class_member<T>& _left, const class_member<T>& _right)  const
                {
                    if (_left.m_class_id < _right.m_class_id)
                        return true;
                    else if (_left.m_class_id > _right.m_class_id)
                        return false;
                    // because the search for a name is a common use case,
                    // we sort it first after the hash of the name and than after m_register_index
                    if (_left.m_name_hash < _right.m_name_hash)
                        return true;
                    else if (_left.m_name_hash > _right.m_name_hash)
                        return false;

                    if (_left.m_register_index < _right.m_register_index)
                        return true;
                    
                    return false;
                }
            };

            type::type_id       m_class_id;
            uint16_t            m_register_index;
            hash_type           m_name_hash;
            std::unique_ptr<T>  m_data;
        };

        template<typename T>
        struct global_member
        {
            global_member(hash_type hash_value, std::unique_ptr<T> data) : m_name_hash(hash_value), m_data(move(data)) {}
            global_member(hash_type hash_value) : m_name_hash(hash_value) {}
            global_member(global_member<T>&& other) : m_name_hash(other.m_name_hash), m_data(std::move(other.m_data)) {}
            global_member<T>& operator = (global_member<T>&& other) 
            {
                m_name_hash = other.m_name_hash;
                m_data = std::move(other.m_data);
                return *this;
            }

            struct order
            {
                RTTR_INLINE bool operator () (const global_member<T>& _left, const global_member<T>& _right)  const
                {
                    return _left.m_name_hash < _right.m_name_hash;
                }
                RTTR_INLINE bool operator () ( const hash_type& _left, const global_member& _right ) const
                {
                    return _left < _right.m_name_hash;
                }
                RTTR_INLINE bool operator () ( const global_member& _left, const hash_type& _right ) const
                {
                    return _left.m_name_hash < _right;
                }
            };

            hash_type           m_name_hash;
            std::unique_ptr<T>  m_data;
        };


        template<typename T, typename Data_Type = conditional_t<std::is_pointer<T>::value, T, std::unique_ptr<T>>>
        struct type_data
        {
            type_data(type::type_id id) : m_id(id) {}
            type_data(type::type_id id, Data_Type data) : m_id(id), m_data(std::move(data)) {}
            type_data(type_data<T, Data_Type>&& other) : m_id(other.m_id), m_data(std::move(other.m_data)) {}
            type_data<T, Data_Type>& operator = (type_data<T, Data_Type>&& other) 
            {
                m_id = other.m_id;
                m_data = std::move(other.m_data);
                return *this;
            }

            struct order_by_id
            {
                RTTR_INLINE bool operator () ( const type_data<T>& _left, const type_data<T>& _right )  const
                {
                    return _left.m_id < _right.m_id;
                }
                RTTR_INLINE bool operator () ( const type::type_id& _left, const type_data<T>& _right ) const
                {
                    return _left < _right.m_id;
                }
                RTTR_INLINE bool operator () ( const type_data<T>& _left, const type::type_id& _right ) const
                {
                    return _left.m_id < _right;
                }
            };

            type::type_id   m_id;
            Data_Type       m_data;
        };

        template<typename T>
        static RTTR_INLINE T* get_item_by_type(const type& t, const std::vector<type_data<T>>& vec);
        template<typename T>
        static RTTR_INLINE void register_item_type(const type& t, std::unique_ptr<T> new_item, std::vector<type_data<T>>& vec);

        type::type_id                                               m_type_id_counter;      //!< The global incremented id counter, this is unique for every type.
        std::vector<const char*>                                    m_orig_names;           //!< Contains all the raw names provied by 'type::register_type'; The type id is the index in this container
        std::vector<std::string>                                    m_custom_names;         //!< Contains all the names of m_orig_names, but the names are cleaned up (garbage strings are removed)
                                                                                            //!< and also custom names, provided during manual register (e.g. class_)
        std::vector<name_to_id>                                     m_orig_name_to_id;      //!< This is a sorted vector which contains hash values of the names in \p m_orig_names
        std::vector<name_to_id>                                     m_custom_name_to_id;    //!< This is a sorted vector which contains hash values of the names in \p m_custom_names

        std::vector<type::type_id>                                  m_base_class_list;      //!< This list contains for every type the id's of it's base classes (a.k.a. parent class)
        std::vector<type::type_id>                                  m_derived_class_list;   //!< This list contains for every type the id's of it's derived classes (a.k.a child class)
        std::vector<get_derived_info_func>                          m_get_derived_info_func_list;
        std::vector<rttr_cast_func>                                 m_conversion_list;      //!< This list contains for every type a conversion function to its base classes

        std::vector<type::type_id>                                  m_raw_type_list;
        std::vector<type::type_id>                                  m_wrapped_type_list;
        std::vector<type::type_id>                                  m_array_raw_type_list;
        std::vector<variant_create_func>                            m_variant_create_func_list; //!< This list contains a function to create from an 'argument' a variant

        std::vector<std::size_t>                                    m_type_size;

        std::vector<bool>                                           m_is_class_list;
        std::vector<bool>                                           m_is_enum_list;
        std::vector<bool>                                           m_is_array_list;
        std::vector<bool>                                           m_is_pointer_list;
        std::vector<bool>                                           m_is_arithmetic_list;
        std::vector<bool>                                           m_is_function_pointer_list;
        std::vector<bool>                                           m_is_member_object_pointer_list;
        std::vector<bool>                                           m_is_member_function_pointer_list;
        std::vector<std::size_t>                                    m_pointer_dim_list;
                                                                    
        std::vector<global_member<property_wrapper_base>>           m_global_property_list;
        std::vector<global_member<method_wrapper_base>>             m_global_method_list;

        std::vector<class_member<property_wrapper_base>>            m_class_property_list;
        std::vector<class_member<method_wrapper_base>>              m_class_method_list;

        std::vector<type_data<constructor_wrapper_base>>            m_constructor_list;
        std::vector<type_data<destructor_wrapper_base>>             m_destructor_list;

        std::vector<type_data<type_converter_base>>                 m_type_converter_list;  //!< This list stores all type conversion objects
        std::vector<type_data<const type_comparator_base*>>         m_type_comparator_list;
        std::vector<type_data<enumeration_wrapper_base>>            m_enumeration_list;
        std::vector<type_data<std::vector<metadata>>>               m_metadata_type_list;
};

} // end namespace detail
} // end namespace rttr

#endif // RTTR_TYPE_DATABASE_P_H_
