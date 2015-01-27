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

#include "rttr/detail/misc_type_traits.h"
#include "rttr/detail/utility.h"
#include "rttr/detail/type_converter.h"

namespace rttr
{

/////////////////////////////////////////////////////////////////////////////////

template<>
class RTTR_API variant::variant_container<void> : public variant_container_base
{
    public:
        variant_container();
    
        variant_container_base* clone() const;
    
        type get_type() const;

        void* get_ptr() const;

        type get_raw_type() const;
        
        void* get_raw_ptr() const;

        bool can_convert(const type& target_type) const;

        bool is_array() const;

        detail::array_container_base* to_array() const;

        std::string to_string(bool* ok) const;
        int to_int(bool* ok) const;
        bool to_bool(bool* ok) const;
        float to_float(bool* ok) const;
        double to_double(bool* ok) const;
    
    private: // unimplemented
        variant_container & operator=(const variant_container &);
};

/////////////////////////////////////////////////////////////////////////////////

template<>
class RTTR_API variant::variant_container<std::string> : public variant_container_base
{
    public:
        variant_container(const std::string& arg);

        variant_container(std::string&& arg);
    
        variant_container_base* clone() const;
    
        type get_type() const;

        void* get_ptr() const;

        type get_raw_type() const;
        
        void* get_raw_ptr() const;

        bool can_convert(const type& target_type) const;

        bool is_array() const;

        detail::array_container_base* to_array() const;

        std::string to_string(bool* ok) const;
        int to_int(bool* ok) const;
        bool to_bool(bool* ok) const;
        float to_float(bool* ok) const;
        double to_double(bool* ok) const;

        std::string _value;

    private: // unimplemented
        variant_container & operator=(const variant_container &);
};

/////////////////////////////////////////////////////////////////////////////////

template<>
class RTTR_API variant::variant_container<int> : public variant_container_base
{
    public:
        variant_container(const int& arg);

        variant_container(int&& arg);
    
        variant_container_base* clone() const;
    
        type get_type() const;

        void* get_ptr() const;

        type get_raw_type() const;
        
        void* get_raw_ptr() const;

        bool can_convert(const type& target_type) const;

        bool is_array() const;

        detail::array_container_base* to_array() const;

        std::string to_string(bool* ok) const;
        int to_int(bool* ok) const;
        bool to_bool(bool* ok) const;
        float to_float(bool* ok) const;
        double to_double(bool* ok) const;

        int _value;

    private: // unimplemented
        variant_container & operator=(const variant_container &);
};

/////////////////////////////////////////////////////////////////////////////////

template<>
class RTTR_API variant::variant_container<bool> : public variant_container_base
{
    public:
        variant_container(const bool& arg);

        variant_container(bool&& arg);
    
        variant_container_base* clone() const;
    
        type get_type() const;

        void* get_ptr() const;

        type get_raw_type() const;
        
        void* get_raw_ptr() const;

        bool can_convert(const type& target_type) const;

        bool is_array() const;

        detail::array_container_base* to_array() const;

        std::string to_string(bool* ok) const;
        int to_int(bool* ok) const;
        bool to_bool(bool* ok) const;
        float to_float(bool* ok) const;
        double to_double(bool* ok) const;

        bool _value;

    private: // unimplemented
        variant_container & operator=(const variant_container &);
};

/////////////////////////////////////////////////////////////////////////////////

template<>
class RTTR_API variant::variant_container<float> : public variant_container_base
{
    public:
        variant_container(const float& arg);

        variant_container(float&& arg);
    
        variant_container_base* clone() const;
    
        type get_type() const;

        void* get_ptr() const;

        type get_raw_type() const;
        
        void* get_raw_ptr() const;

        bool is_array() const;

        detail::array_container_base* to_array() const;

        bool can_convert(const type& target_type) const;

        std::string to_string(bool* ok) const;
        int to_int(bool* ok) const;
        bool to_bool(bool* ok) const;
        float to_float(bool* ok) const;
        double to_double(bool* ok) const;

        float _value;

    private: // unimplemented
        variant_container & operator=(const variant_container &);
};

/////////////////////////////////////////////////////////////////////////////////

template<>
class RTTR_API variant::variant_container<double> : public variant_container_base
{
    public:
        variant_container(const double& arg);

        variant_container(double&& arg);
    
        variant_container_base* clone() const;
    
        type get_type() const;

        void* get_ptr() const;

        type get_raw_type() const;
        
        void* get_raw_ptr() const;

        bool is_array() const;

        detail::array_container_base* to_array() const;

        bool can_convert(const type& target_type) const;

        std::string to_string(bool* ok) const;
        int to_int(bool* ok) const;
        bool to_bool(bool* ok) const;
        float to_float(bool* ok) const;
        double to_double(bool* ok) const;

        double _value;

    private: // unimplemented
        variant_container & operator=(const variant_container &);
};



/////////////////////////////////////////////////////////////////////////////////

} // end namespace rttr
