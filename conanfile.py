from conans import ConanFile, CMake
import re
import os


def rttr_version_detect(filename):
    conan_file_path = os.path.split(os.path.realpath(__file__))[0]
    absfilepath = os.path.join(conan_file_path, filename)
    version = []
    with open(absfilepath, "r") as f:
        st = f.read()
    for verstr in "MAJOR", "MINOR", "PATCH":
        match = re.search("RTTR_VERSION_%s (.)" % verstr, st)
        version.append(match.groups()[0])
    return ".".join(version)

class RttrConan(ConanFile):
    name = "rttr"
    version = rttr_version_detect("CMake/config.cmake")
    license = "MIT"
    url = "https://github.com/rttrorg/rttr"
    description = "An open source library, which adds (dynamic) reflection to C++"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False],
               "rtti": [True, False],
               "static_runtime" : [True, False]}
    default_options = "shared=False", "rtti=True", "static_runtime=False"
    generators = "cmake"
    exports = "README.md", "CMake/config.cmake"
    exports_sources = "src/*" , "3rd_party/*", "*.txt", "*.cmake"

    def source(self):
        # not needed, package file is with source
        pass
    
        
    def build(self):
        cmake = CMake(self)
        disable_opts = ["BENCHMARKS", "DOCUMENTATION", "EXAMPLES", "PACKAGE", "UNIT_TESTS"]
        for opt in disable_opts:
            cmake.definitions["BUILD_%s" % opt] = False

        a = lambda o : True if o else False

        cmake.definitions["BUILD_INSTALLER"] = True
        cmake.definitions["BUILD_WITH_RTTI"] = a(self.options.rtti)
        cmake.definitions["BUILD_STATIC"] = a(not self.options.shared)
        cmake.definitions["BUILD_RTTR_DYNAMIC"] = a(self.options.shared)

        if self.options["static_runtime"]:
            cmake.definitions["BUILD_WITH_STATIC_RUNTIME_LIBS"] = "ON"

        cmake.configure()
        cmake.build()
        # need to do install here, otherwise cmake config files don't get
        # generated properly
        cmake.install()

    def package_info(self):
        suffix = "_d" if self.settings.build_type == "Debug" else ""
        prefix = "lib" if self.settings.os == "Windows" and not self.options.shared else ""
        self.cpp_info.libs = ["%srttr_core%s" % (prefix, suffix)]


