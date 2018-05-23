// This file is distributed under the BSD License.
// See "license.txt" for details.
// Copyright 2009-2012, Jonathan Turner (jonathan@emptycrate.com)
// Copyright 2009-2017, Jason Turner (jason@emptycrate.com)
// http://www.chaiscript.com

#ifndef CHAISCRIPT_UNKNOWN_HPP_
#define CHAISCRIPT_UNKNOWN_HPP_


namespace chaiscript
{
  namespace detail
  {
    struct Loadable_Module
    {
      Loadable_Module(const std::string &, const std::string &)
      {
        throw chaiscript::exception::load_module_error("Loadable module support not available for your platform");
      }

      ModulePtr m_moduleptr;
    };
  }
}
#endif

