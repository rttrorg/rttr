#!/usr/bin/env python
# -*- coding: utf-8 -*-

from conans import ConanFile, CMake, tools, RunEnvironment
from conans.errors import ConanException
import os
from io import StringIO

def run_test(conanfile,bin_path, args, output=None):
    if conanfile.settings.os == "Windows":
        conanfile.run("%s %s" % bin_path, args,output=output)
    elif conanfile.settings.os == "Macos":
        conanfile.run("DYLD_LIBRARY_PATH=%s %s %s" % (os.environ.get('DYLD_LIBRARY_PATH', ''), bin_path, args),output=output)
    else:
        conanfile.run("LD_LIBRARY_PATH=%s %s %s" % (os.environ.get('LD_LIBRARY_PATH', ''), bin_path, args),output=output)
    

class TestPackageConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def test(self):
        with tools.environment_append(RunEnvironment(self).vars):
            bin_path = os.path.join("bin", "test_package")
            if self.settings.os == "Windows":
                self.run(bin_path)
            elif self.settings.os == "Macos":
                self.run("DYLD_LIBRARY_PATH=%s %s" % (os.environ.get('DYLD_LIBRARY_PATH', ''), bin_path))
            else:
                self.run("LD_LIBRARY_PATH=%s %s" % (os.environ.get('LD_LIBRARY_PATH', ''), bin_path))

