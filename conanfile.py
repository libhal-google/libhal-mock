# Copyright 2023 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.files import copy
from conan.tools.build import check_min_cppstd
from conan.errors import ConanInvalidConfiguration
import os


required_conan_version = ">=2.0.6"


class libhal_mock_conan(ConanFile):
    name = "libhal-mock"
    version = "2.1.0"
    license = "Apache-2.0"
    url = "https://github.com/conan-io/conan-center-index"
    homepage = "https://libhal.github.io/libhal-mock"
    description = (
        "Mocks, fakes, simulation and other testing utilities for libhal")
    topics = ("mock", "fake", "simulation", "testing", "unit test")
    settings = "compiler", "build_type", "os", "arch"
    exports_sources = "include/*", "tests/*", "LICENSE"
    generators = "CMakeToolchain", "CMakeDeps"
    no_copy_source = True

    @property
    def _min_cppstd(self):
        return "20"

    @property
    def _compilers_minimum_version(self):
        return {
            "gcc": "11",
            "clang": "14",
            "apple-clang": "14.0.0"
        }

    def validate(self):
        if self.settings.get_safe("compiler.cppstd"):
            check_min_cppstd(self, self._min_cppstd)

    def build_requirements(self):
        self.test_requires("boost-ext-ut/1.1.9")

    def requirements(self):
        self.requires("libhal/[^2.0.3]", transitive_headers=True)
        self.requires("libhal-util/[^3.0.1]")

    def layout(self):
        cmake_layout(self)

    def build(self):
        if not self.conf.get("tools.build:skip_test", default=False):
            cmake = CMake(self)
            if self.settings.os == "Windows":
                cmake.configure(build_script_folder="tests")
            else:
                cmake.configure(build_script_folder="tests",
                                variables={"ENABLE_ASAN": True})
            cmake.build()
            self.run(os.path.join(self.cpp.build.bindir, "unit_test"))

    def package(self):
        copy(self, "LICENSE", dst=os.path.join(
            self.package_folder, "licenses"), src=self.source_folder)
        copy(self, "*.h", dst=os.path.join(self.package_folder, "include"),
             src=os.path.join(self.source_folder, "include"))
        copy(self, "*.hpp", dst=os.path.join(self.package_folder,
             "include"), src=os.path.join(self.source_folder, "include"))

    def package_info(self):
        self.cpp_info.bindirs = []
        self.cpp_info.frameworkdirs = []
        self.cpp_info.libdirs = []
        self.cpp_info.resdirs = []
        self.cpp_info.set_property("cmake_target_name", "libhal::mock")

    def package_id(self):
        self.info.clear()
